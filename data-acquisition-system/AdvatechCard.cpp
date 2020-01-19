#include "AdvatechCard.h"
#include <assert.h>
#include <thread>
#include <QUuid>

#include "glog/logging.h"

#include "Utils.h"


AdvatechCard::AdvatechCard()
{
}

AdvatechCard::~AdvatechCard()
{
	stop();
	close();
	if (dataBuffer) {
		delete[] dataBuffer;
	}
}

Automation::BDaq::ErrorCode AdvatechCard::init(Device& device, std::shared_ptr<SqlServerRepository> ssr)
{
	ErrorCode        ret = Success;
	this->lssr = ssr;

	wfAiCtrl = WaveformAiCtrl::Create();

	wfAiCtrl->addDataReadyHandler(OnDataReadyEvent, this);
	wfAiCtrl->addOverrunHandler(OnOverRunEvent, this);
	wfAiCtrl->addCacheOverflowHandler(OnCacheOverflowEvent, this);
	wfAiCtrl->addStoppedHandler(OnStoppedEvent, this);

	std::wstring description;
	Utils::StringToWstring(description, device.description);
	DeviceInformation devInfo(description.c_str());
	ret = wfAiCtrl->setSelectedDevice(devInfo);
	if (ret != Success)
		return ret;

	std::wstring profilePath;
	Utils::StringToWstring(profilePath, device.profilePath);
	ret = wfAiCtrl->LoadProfile(profilePath.c_str());//Loads a profile to initialize the device.
	if (ret != Success)
		return ret;

	Conversion * conversion = wfAiCtrl->getConversion();
	ret = conversion->setClockRate(device.clockRate);
	if (ret != Success)
		return ret;
	ret = conversion->setChannelStart(device.startChannel);
	if (ret != Success)
		return ret;
	ret = conversion->setChannelCount(device.channelCount);
	if (ret != Success)
		return ret;
	Record * record = wfAiCtrl->getRecord();
	ret = record->setSectionCount(0);//The 0 means setting 'streaming' mode.
	if (ret != Success)
		return ret;
	ret = record->setSectionLength(device.sectionLength);
	if (ret != Success)
		return ret;

	deviceConfig = device;

	bufferSize = deviceConfig.channelCount * deviceConfig.sectionLength;
	dataBuffer = new double(bufferSize);

	totalData = new std::vector<double>(deviceConfig.channelCount * deviceConfig.maxCount);

	startOffset = deviceConfig.groupStartChannel - deviceConfig.startChannel;
	stopOffset = deviceConfig.groupStopChannel - deviceConfig.startChannel;

	ret = wfAiCtrl->Prepare();
	if (ret != Success)
		return ret;
	
	return ret;
}

ErrorCode AdvatechCard::stop()
{
	ErrorCode ret = wfAiCtrl->Stop();
	return ret;
}

void AdvatechCard::close()
{
	wfAiCtrl->Dispose();
}

ErrorCode AdvatechCard::start()
{
	ErrorCode ret = wfAiCtrl->Start();
	return ret;
}

void handlerThread(QDateTime startTime, Device dc, std::vector<double> data, std::shared_ptr<SqlServerRepository> ssr, RecordState state)
{
	int channelCount = dc.channelCount;
	int rate = dc.clockRate;
	QUuid id = QUuid::createUuid();
	std::string strId = id.toString().toStdString();

	for (int i=0; i<channelCount; i++) {
		AnalogInput ai;
		ai.channel = i;
		ai.groupID = strId;
		ai.frequency = dc.clockRate;
		for (int j=0; j<data.size()/channelCount; j++) {
			if (data[j * channelCount + i] >= dc.thresholds[i - dc.startChannel]) {
				ai.data.push_back(data[j*channelCount + i]);
			} else if (data[j*channelCount + i] < dc.thresholds[i - dc.startChannel] && ai.data.size() > 0) {

				ai.sampleCount = ai.data.size();
				ai.datetime = startTime.addMSecs(1000.0 * (j - ai.data.size()) / dc.clockRate).toString("yyyy-M-d H:m:s.zzz").toStdString();
				state == RS_STOP ? ssr->addAnalogInput(ai) : ssr->addAnalogInputErr(ai);
				ai.data.clear();
			}
		}
		
		if (ai.data.size()) {
			ai.sampleCount = ai.data.size();
			ai.datetime = startTime.addMSecs(1000.0 * (data.size() / channelCount - ai.data.size()) / dc.clockRate).toString("yyyy-M-d H:m:s.zzz").toStdString();
			state == RS_STOP ? ssr->addAnalogInput(ai) : ssr->addAnalogInputErr(ai);
			ai.data.clear();
		}
	}
}

void BDAQCALL AdvatechCard::OnDataReadyEvent(void * sender, BfdAiEventArgs * args, void *userParam)
{
	int32 returnedCount = 0;
	WaveformAiCtrl * waveformAiCtrl = NULL;
	waveformAiCtrl = (WaveformAiCtrl *)sender;
	AdvatechCard* card = (AdvatechCard*)userParam;
	int32 getDataCount = min(card->bufferSize, args->Count);
	waveformAiCtrl->GetData(getDataCount, card->dataBuffer, 0, &returnedCount);


	for (int32 i = 0; i < getDataCount; i += card->deviceConfig.channelCount) {
		if (card->state == RS_WAIT && card->dataBuffer[i+card->startOffset] >= card->deviceConfig.thresholds[card->startOffset]) {
			card->state = RS_RECORD;
			card->startTime = QDateTime::currentDateTime();
			break;
		} else if (card->state == RS_RECORD && card->dataBuffer[i + card->stopOffset] < card->deviceConfig.thresholds[card->stopOffset]) {
			card->state = RS_STOP;
			break;
		} else if (card->state == RS_ERROR && card->dataBuffer[i + card->stopOffset] < card->deviceConfig.thresholds[card->stopOffset]) {
			card->state = RS_WAIT;
			break;
		}
	}

	if (card->state == RS_RECORD) {
		if (card->totalData->size() + getDataCount > card->totalData->capacity()) {
			card->state = RS_ERROR;
		}
		else {
			card->totalData->insert(card->totalData->end(), card->dataBuffer, card->dataBuffer + getDataCount);
		}
	}

	if (card->state == RS_ERROR || card->state == RS_STOP)
	{
		card->totalData->insert(card->totalData->end(), card->dataBuffer, card->dataBuffer + getDataCount);
		std::thread handler(handlerThread, card->startTime, card->deviceConfig, *card->totalData, card->lssr, card->state);
		card->totalData->clear();
	}
}
	//The function is used to deal with 'OverRun' Event.
void BDAQCALL AdvatechCard::OnOverRunEvent(void * sender, BfdAiEventArgs * args, void *userParam)
{
	LOG(ERROR) << "Streaming AI Overrun: offset = " << args->Offset << ", count = " << args->Count;
}
//The function is used to deal with 'CacheOverflow' Event.
void BDAQCALL AdvatechCard::OnCacheOverflowEvent(void * sender, BfdAiEventArgs * args, void *userParam)
{
	LOG(ERROR) << "Streaming AI Cache Overflow: offset = " << args->Offset << ", count = " << args->Count;
}
//The function is used to deal with 'Stopped' Event.
void BDAQCALL AdvatechCard::OnStoppedEvent(void * sender, BfdAiEventArgs * args, void *userParam)
{
	LOG(ERROR) << "Streaming AI stopped: offset = " << args->Offset << ", count = " << args->Count;
}
