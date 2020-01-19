#pragma once

#include <QObject>
#include "QDateTime"


#include "advatech/bdaqctrl.h"
#include "advatech/compatibility.h"
#include "Config.h"

#include "SqlServerRepository.h"

using namespace Automation::BDaq;

enum RecordState
{
	RS_WAIT = 0,
	RS_RECORD,
	RS_STOP,
	RS_ERROR
};



class AdvatechCard : public QObject
{
	Q_OBJECT

public:
	AdvatechCard();
	~AdvatechCard();

	ErrorCode init(Device& device, std::shared_ptr<SqlServerRepository> ssr);
	ErrorCode start();
	ErrorCode stop();
	void close();

	//std::vector<std::vector<AnalogInput>> dataVector;
	Device deviceConfig;
	std::vector<double>* totalData;
	QDateTime startTime;
	std::shared_ptr<SqlServerRepository> lssr;

private:
	static void BDAQCALL OnDataReadyEvent(void * sender, BfdAiEventArgs * args, void *userParam);
	static void BDAQCALL OnOverRunEvent(void * sender, BfdAiEventArgs * args, void *userParam);
	static void BDAQCALL OnCacheOverflowEvent(void * sender, BfdAiEventArgs * args, void *userParam);
	static void BDAQCALL OnStoppedEvent(void * sender, BfdAiEventArgs * args, void *userParam);


	WaveformAiCtrl* wfAiCtrl = { nullptr };
	int bufferSize = { 0 };
	double* dataBuffer = { nullptr };

	int startOffset;
	int stopOffset;

	RecordState state = { RS_WAIT };

};
