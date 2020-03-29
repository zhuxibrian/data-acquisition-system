#pragma once

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <vector>

struct AnalogInput
{
    int deviceID;
	std::string groupID;
	int channel;
	std::string datetime;
	int frequency;
	int sampleCount;
	std::vector<double> data;
};

struct SerialPortInput
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
	bool addSerialPortData(SerialPortInput& spd);



private:
	QSqlDatabase _db;
	
};
