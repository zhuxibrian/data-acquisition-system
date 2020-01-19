#pragma once

#include "objbase.h"
#include <string>

class Utils
{
public:
	Utils();
	~Utils();

	static std::string GuidToString(const GUID &guid);
	static void StringToWstring(std::wstring& szDst, std::string str);
};

