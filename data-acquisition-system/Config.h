#pragma once
#include "yaml-cpp/yaml.h"
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


class Config
{
public:
	Config(std::string configPath);
	~Config();

	bool loadConfig();
	DeviceConfig* getDeviceConfig();
	SerialPortConfig* getSerialPortConfig();


private:
	DeviceConfig* _deviceConfig;
	SerialPortConfig* _serialPortConfig;
	std::string _configPath;
};

