#include "Config.h"

#define GOOGLE_GLOG_DLL_DECL
#include "glog/logging.h"

#ifdef _DEBUG
#pragma comment(lib,"../lib/libyaml-cppmdd.lib")
#else
#pragma comment(lib,"../lib/libyaml-cppmd.lib")
#endif // DEBUG


namespace YAML {
	template<>
	struct convert<Device> 
	{
		static bool decode(const Node& node, Device& device)
		{
			device.description = node["description"].as<std::string>();
			device.profilePath = node["profilePath"].as<std::string>();
			device.startChannel = node["startChannel"].as<int>();
			device.channelCount = node["channelCount"].as<int>();
			device.clockRate = node["clockRate"].as<int>();
			device.sectionLength = node["sectionLength"].as<int>();
			device.groupStartChannel = node["groupStartChannel"].as<int>();
			device.groupStopChannel = node["groupStopChannel"].as<int>();

			for (std::size_t i=0; i<node["thresholds"].size(); i++) {
				device.thresholds.push_back(node["thresholds"][i].as<double>());
			}

			return true;
		}
	};

	template<>
	struct convert<DeviceConfig>
	{
		static bool decode(const Node& node, DeviceConfig& dc)
		{
			for (std::size_t i = 0; i<node.size(); i++) {
				dc.devices.push_back(node[i].as<Device>());
			}
			return true;
		}
	};

	template<>
	struct convert<SerialPort>
	{
		static bool decode(const Node& node, SerialPort& serialPort)
		{
			serialPort.portName = node["portName"].as<std::string>();
			serialPort.period = node["period"].as<std::string>();
			return true;
		}
	};

	template<>
	struct convert<SerialPortConfig>
	{
		static bool decode(const Node& node, SerialPortConfig& sc)
		{
			for (std::size_t i = 0; i < node.size(); i++) {
				sc.serialPorts.push_back(node[i].as<SerialPort>());
			}
			return true;
		}
	};
}

Config::Config(std::string configPath) :
	_deviceConfig(nullptr),
	_serialPortConfig(nullptr),
	_configPath(configPath)
{

}

Config::~Config()
{
	if (_deviceConfig) {
		delete _deviceConfig;
	}

	if (_serialPortConfig) {
		delete _serialPortConfig;
	}
}

bool Config::loadConfig()
{
	YAML::Node node = YAML::LoadFile(_configPath);
	_deviceConfig = new DeviceConfig;
	*_deviceConfig = node["devices"].as<DeviceConfig>();
	_serialPortConfig = new SerialPortConfig;
	*_serialPortConfig = node["serialPort"].as<SerialPortConfig>();

	LOG(INFO) << "load config success. Card count is "
		<< _deviceConfig->devices.size()
		<< ". Serial port count is "
		<< _serialPortConfig->serialPorts.size();

	return true;
}
