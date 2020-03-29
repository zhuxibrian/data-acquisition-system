#pragma once

#include <QObject>
#include <qlist.h>
#include "Config.h"
#include "QDateTime"
#include <QMutex>
#include "SqlServerRepository.h"

enum DataTypeEnum
{
    DT_UNUSED = 0,
    ADVATECHCARD,
    SERIALPORT
};


struct Data 
{
    int dataType;
};

struct AdvatechCardData : public Data
{
    Device device;
    QDateTime datetime;
    bool isErrorData;
    int dataSize;
    double* data;

    AdvatechCardData()
    {
        dataType = DataTypeEnum::ADVATECHCARD;
    }

    ~AdvatechCardData()
    {
        if (data)
        {
            delete[] data;
        }
    }
};

struct SerialPortData : public Data
{
    SerialPort serialPort;
    QDateTime datetime;
    std::string data;

    SerialPortData()
    {
        dataType = DataTypeEnum::SERIALPORT;
    }
};

class DataAnalyzer : public QObject
{
    Q_OBJECT

public:
    DataAnalyzer(std::shared_ptr<SqlServerRepository> ssr, QObject* parent = nullptr);
    ~DataAnalyzer();

    void pushData(Data* data);

signals:
    void pushDataSignal();

public slots:
    void handleData();

private:
    void handleAdvatechCardData(Data* d);
    void handleSerialPortData(Data* d);

    bool isStartValue(Device& device, int channel, double value);
    bool isStopValue(Device& device, int channel, double value);


private:
    QList<Data*> _dataList;
    std::shared_ptr<SqlServerRepository> ssr;


    QMutex _mutex;


    

};
