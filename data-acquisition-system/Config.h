#pragma once
#include "yaml-cpp/yaml.h"
#include "yaml-cpp/exceptions.h"
#include <string>
#include <vector>

struct Device 
{
	std::string description;
	std::string profilePath;
	int startChannel;
	int channelCount;
	int clockRate;
	int sectionLength;
	int groupStartChannel;
	int groupStopChannel;
	int maxCount;
	std::vector<double> thresholds;
};

struct DeviceConfig
{
	std::vector<Device> devices;
};

struct SerialPort
{
	std::string portName;
	std::string period;
};

struct SerialPortConfig
{
	std::vector<SerialPort> serialPorts;
};

struct DatabaseConfig 
{
	std::string host;
	std::string dbname;
	std::string username;
	std::string password;
};


class Config
{
public:
	Config(std::string configPath);
	~Config();

	bool loadConfig();
	DeviceConfig* getDeviceConfig();
	SerialPortConfig* getSerialPortConfig();
	DatabaseConfig* getDatabaseConfig();


private:
	DeviceConfig* _deviceConfig;
	SerialPortConfig* _serialPortConfig;
	DatabaseConfig* _databaseConfig;
	std::string _configPath;
};

