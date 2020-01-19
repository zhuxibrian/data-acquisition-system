#include "SqlServerRepository.h"
#include "QtSql/QSqlError"
#include "glog/logging.h"
#include <QtSql/QSqlQuery>
#include <qdebug.h>

#define SQL_ADD_ANALOG_INPUT ("INSERT INTO [dbo].[Analog_Input]\
([groupID]\
	, [channel]\
	, [startTime]\
	, [frequency]\
	, [sampleCount]\
	, [sampleData]\
	, [isUse])\
	VALUES\
	(:groupId, :channel, :startTime, :frequency, :sampleCount, :sampleData, :isUse)")

#define SQL_ADD_ANALOG_INPUT_ERR ("INSERT INTO [dbo].[Analog_Input_Err]\
([groupID]\
	, [channel]\
	, [startTime]\
	, [frequency]\
	, [sampleCount]\
	, [sampleData]\
	, [isUse])\
	VALUES\
	(:groupId, :channel, :startTime, :frequency, :sampleCount, :sampleData, :isUse)")

#define SQL_ADD_SERIAL_PORT_DATA ("INSERT INTO [dbo].[Serial_Port_Data]\
([serialPort]\
	, [time]\
	, [data]\
	, [isUse])\
	VALUES\
	(:serialPort, :time,  :data, :isUse)")

SqlServerRepository::SqlServerRepository()
{
}

SqlServerRepository::~SqlServerRepository()
{
}

bool SqlServerRepository::connect(std::string host, std::string dbname, std::string username, std::string password)
{
	_db = QSqlDatabase::addDatabase("QODBC3");
	_db.setDatabaseName(QString("DRIVER={SQL SERVER};"
		"SERVER=%1;"
		"DATABASE=%2;"
		"UID=%3;"
		"PWD=%4;").arg(QString::fromStdString(host))
		.arg(QString::fromStdString(dbname))
		.arg(QString::fromStdString(username))
		.arg(QString::fromStdString(password)));

	if (!_db.open()) {
		LOG(ERROR) << "database connect error: " << _db.lastError().text().toStdString();
		return false;
	}

	return true;
}

bool SqlServerRepository::addAnalogInput(AnalogInput& ai)
{
	QSqlQuery query;
	query.prepare(SQL_ADD_ANALOG_INPUT);

	QString groupId = QString::fromStdString(ai.groupID);
	int channel = ai.channel;
	QString startTime = QString::fromStdString(ai.datetime);
	int frequency = ai.frequency;
	int sampleCount = ai.sampleCount;

 	QByteArray doubleArray;
 	int len = ai.data.size();
 	doubleArray.resize(len * 8);
 	memcpy(doubleArray.data(), ai.data.data(), len);

	bool isUse = 1;

	query.bindValue(":groupId", groupId);
	query.bindValue(":channel", channel);
	query.bindValue(":startTime", startTime);
	query.bindValue(":frequency", frequency);
	query.bindValue(":sampleCount", sampleCount);
	query.bindValue(":sampleData", doubleArray);
	query.bindValue(":isUse", isUse);


	if (!query.exec()) {
		LOG(ERROR) << "sql exec faild:" << query.lastError().text().toStdString();
		return false;
	}

	return true;
}

bool SqlServerRepository::addAnalogInputErr(AnalogInput& ai)
{
	QSqlQuery query;
	query.prepare(SQL_ADD_ANALOG_INPUT_ERR);

	QString groupId = QString::fromStdString(ai.groupID);
	int channel = ai.channel;
	QString startTime = QString::fromStdString(ai.datetime);
	int frequency = ai.frequency;
	int sampleCount = ai.sampleCount;

	QByteArray doubleArray;
	int len = ai.data.size();
	doubleArray.resize(len * 8);
	memcpy(doubleArray.data(), ai.data.data(), len);

	bool isUse = 1;

	query.bindValue(":groupId", groupId);
	query.bindValue(":channel", channel);
	query.bindValue(":startTime", startTime);
	query.bindValue(":frequency", frequency);
	query.bindValue(":sampleCount", sampleCount);
	query.bindValue(":sampleData", doubleArray);
	query.bindValue(":isUse", isUse);


	if (!query.exec()) {
		LOG(ERROR) << "sql exec faild:" << query.lastError().text().toStdString();
		return false;
	}

	return true;
}

bool SqlServerRepository::addSerialPortData(SerialPortData& spd)
{
	QSqlQuery query;
	query.prepare(SQL_ADD_SERIAL_PORT_DATA);

	QString serialPort = QString::fromStdString(spd.serialPort);
	QString time = QString::fromStdString(spd.time);
	QString data = QString::fromStdString(spd.data);

	bool isUse = 1;

	query.bindValue(":serialPort", serialPort);
	query.bindValue(":time", time);
	query.bindValue(":data", data);
	query.bindValue(":isUse", isUse);


	if (!query.exec()) {
		LOG(ERROR) << "sql exec faild:" << query.lastError().text().toStdString();
		return false;
	}

	return true;
}
