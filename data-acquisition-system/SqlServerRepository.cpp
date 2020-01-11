#include "SqlServerRepository.h"
#include "QtSql/QSqlError"
#include "glog/logging.h"
#include <QSqlQuery>
#include <qdebug.h>

SqlServerRepository::SqlServerRepository(QObject *parent)
	: QObject(parent)
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
	query.prepare(SQL_ADDANALOG_INPUT);

	QString groupId = QString::fromStdString(ai.groupID);
	int channel = ai.channel;
	QString startTime = QString::fromStdString(ai.datetime);
	int frequency = ai.frequency;
	int sampleCount = ai.sampleCount;

// 	QByteArray doubleArray;
// 	int len = ai.data.size();
// 	doubleArray.resize(len * 8);
// 	memcpy(doubleArray.data(), ai.data.data(), len);

	bool isUse = 1;

	query.bindValue(":groupId", groupId);
	query.bindValue(":channel", channel);
	query.bindValue(":startTime", startTime);
	query.bindValue(":frequency", frequency);
	query.bindValue(":sampleCount", sampleCount);
	query.bindValue(":sampleData", 1);
	query.bindValue(":isUse", isUse);


	if (!query.execBatch()) {
		LOG(ERROR) << "sql exec faild:" << query.lastError().text().toStdString();
		return false;
	}

	return true;
}
