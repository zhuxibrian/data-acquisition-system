#include "Config.h"
#include "yaml-cpp/yaml.h"

#define GOOGLE_GLOG_DLL_DECL
#include "glog/logging.h"

#ifdef _DEBUG
#pragma comment(lib,"../lib/libyaml-cppmdd.lib")
#else
#pragma comment(lib,"../lib/libyaml-cppmd.lib")
#endif // DEBUG

Config::Config()
{
	YAML::Node node2 = YAML::LoadFile("./config/config.yaml");
	LOG(INFO) << "read config.";

}


Config::~Config()
{
}
