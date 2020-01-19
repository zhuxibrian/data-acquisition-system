#pragma once

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <vector>



struct AnalogInput
{
	std::string groupID;
	int channel;
	std::string datetime;
	int frequency;
	int sampleCount;
	std::vector<double> data;
};

struct SerialPortData
{
	std::string serialPort;
	std::string time;
	std::string data;
};

class SqlServerRepository : public QObject
{
	Q_OBJECT

public:
	SqlServerRepository();
	~SqlServerRepository();

	bool connect(std::string host, std::string dbname, 
		std::string username, std::string password);

	bool addAnalogInput(AnalogInput& ai);
	bool addAnalogInputErr(AnalogInput& ai);
	bool addSerialPortData(SerialPortData& spd);



private:
	QSqlDatabase _db;
	
};
