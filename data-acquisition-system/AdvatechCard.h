#pragma once

#include <QObject>
#include "QDateTime"


#include "advatech/bdaqctrl.h"
#include "advatech/compatibility.h"
#include "Config.h"

#include "DataAnalyzer.h"

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

	ErrorCode init(Device& device, DataAnalyzer* dataAnalyzer);
	ErrorCode start();
	ErrorCode stop();
	void close();

	Device deviceConfig;
	std::vector<double>* totalData;
	QDateTime startTime;

private:
	static void BDAQCALL OnDataReadyEvent(void * sender, BfdAiEventArgs * args, void *userParam);
	static void BDAQCALL OnOverRunEvent(void * sender, BfdAiEventArgs * args, void *userParam);
	static void BDAQCALL OnCacheOverflowEvent(void * sender, BfdAiEventArgs * args, void *userParam);
	static void BDAQCALL OnStoppedEvent(void * sender, BfdAiEventArgs * args, void *userParam);


	WaveformAiCtrl* wfAiCtrl = { nullptr };

	int bufferSize = { 0 }; //缓存大小
	double* dataBuffer = { nullptr }; //缓存区

	int startOffset;
    int startTriggModel;
    int startTriggerValue;

	int stopOffset;
    int stopTriggModel;
    int stopTriggerValue;

	RecordState state = { RS_WAIT }; //板卡读数当前状态

    DataAnalyzer* _analyzer;

};
