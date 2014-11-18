#include "StdAfx.h"
#include "IConfig.h"
#include "Config.h"

using namespace Utility;

/* static */ IConfig* IConfig::config = NULL;
/* static */ IConfig& IConfig::GetConfig()
{
	if (IConfig::config == NULL)
	{
		IConfig::config = new Config();
	}
	return *IConfig::config;
}

IConfig::IConfig()
{
}

IConfig::~IConfig()
{
}
