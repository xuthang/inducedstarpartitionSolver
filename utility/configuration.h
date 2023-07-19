#pragma once

#include <string>

class configuration
{
public:
	std::string graphLocation;
	std::string VCLocation;
	bool hasDefinedVC() const { return VCLocation != ""; }
};

const configuration __defaultConfig = {"instances/smallinstance.gr", ""};