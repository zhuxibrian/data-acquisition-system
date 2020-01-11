#pragma once

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <vector>

#define SQL_ADDANALOG_INPUT ("insert into \
Analog_Input(groupId, channel, startTime, frequency, sampleCount, sampleData, isUse) \
value(:groupId, :channel, :startTime, :frequency, :sampleCount, :sampleData, :isUse)")

struct AnalogInput
{
	std::string groupID;
	int channel;
	std::string datetime;
	int frequency;
	int sampleCount;
	std::vector<double> data;
};

class SqlServerRepository : public QObject
{
	Q_OBJECT

public:
	SqlServerRepository(QObject *parent);
	~SqlServerRepository();

	bool connect(std::string host, std::string dbname, 
		std::string username, std::string password);

	bool addAnalogInput(AnalogInput& ai);


private:
	QSqlDatabase _db;
	
};
