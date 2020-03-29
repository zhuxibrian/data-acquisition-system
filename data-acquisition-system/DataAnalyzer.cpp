#include "DataAnalyzer.h"
#include <QUuid>


DataAnalyzer::DataAnalyzer(std::shared_ptr<SqlServerRepository> ssr, QObject* parent /*= nullptr*/)
{
    this->ssr = ssr;
}

DataAnalyzer::~DataAnalyzer()
{
}

void DataAnalyzer::pushData(Data* data)
{
    QMutexLocker locker(&_mutex);
    _dataList.push_back(data);
    emit pushDataSignal();
}

void DataAnalyzer::handleData()
{
    while (_dataList.size())
    {
        QMutexLocker locker(&_mutex);
        Data* data = _dataList[0];
        _dataList.pop_front();

        switch (data->dataType)
        {
        case DataTypeEnum::ADVATECHCARD:
            handleAdvatechCardData(data);
            break;
        case DataTypeEnum::SERIALPORT:
            handleSerialPortData(data);
            break;
        default:
            break;
        }
    }
}

void DataAnalyzer::handleAdvatechCardData(Data* d)
{
    AdvatechCardData* cardData = (AdvatechCardData*)d;

    Device dc = cardData->device;
    double* data = cardData->data;
    QDateTime startTime = cardData->datetime;
    int channelCount = dc.channelCount;
    int rate = dc.clockRate;
    std::string strId = QUuid::createUuid().toString().toStdString();

    for (int i = 0; i < channelCount; i++) {
        if (dc.thresholdsTrig[i+dc.startChannel] == TriggerModel::UNTRIGGER)
        {
            continue;
        }

        AnalogInput ai;
        ai.deviceID = dc.deviceID;
        ai.channel = i;
        ai.groupID = strId;
        ai.frequency = dc.clockRate;
        for (int j = 0; j < cardData->dataSize / channelCount; j++) {
            if (isStartValue(dc, i+dc.startChannel, data[j*channelCount + i])) {
                ai.data.push_back(data[j*channelCount + i]);
            }
            else if (isStopValue(dc, i + dc.startChannel, data[j*channelCount + i])) {
                ai.sampleCount = ai.data.size();
                ai.datetime = startTime.addMSecs(1000.0 * (j - ai.data.size()) / dc.clockRate).toString("yyyy-M-d H:m:s.zzz").toStdString();
                cardData->isErrorData ? ssr->addAnalogInputErr(ai) : ssr->addAnalogInput(ai);
                ai.data.clear();
            }
        }

        //通道数据到达最后一个，但是未触发停止阈值时，将数据保存
        if (ai.data.size()) {
            ai.sampleCount = ai.data.size();
            ai.datetime = startTime.addMSecs(1000.0 * (cardData->dataSize/channelCount-ai.data.size()) / dc.clockRate).toString("yyyy-M-d H:m:s.zzz").toStdString();
            cardData->isErrorData ? ssr->addAnalogInputErr(ai) : ssr->addAnalogInput(ai);
            ai.data.clear();
        }
    }

    delete cardData;
}

void DataAnalyzer::handleSerialPortData(Data* d)
{
    SerialPortData* spdata = (SerialPortData*)d;

    delete spdata;
}

bool DataAnalyzer::isStopValue(Device& device, int channel, double value)
{
    return device.thresholdsTrig[channel] == TriggerModel::RISING ? value <= device.thresholdsDown[channel] : value >= device.thresholdsUp[channel];
}

bool DataAnalyzer::isStartValue(Device& device, int channel, double value)
{
    return device.thresholdsTrig[channel] == TriggerModel::RISING ? value >= device.thresholdsUp[channel] : value <= device.thresholdsDown[channel];
}

