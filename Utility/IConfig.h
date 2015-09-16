#pragma once

#include "Utility.h"
#include "ILogger.h"
#include <sstream>

#include <string>
#define GET_CONFIG_VALUE(cfgName, defValue) Utility::IConfig::GetConfig().Get(cfgName, defValue)
#define GET_CONFIG_VALUE_STR(cfgName, defValue) Utility::IConfig::GetConfig().GetArg(cfgName, defValue)

namespace Utility
{

class UTILITY_API IConfig
{
/* ==================== Static variables and functions begin ==================== */
protected:
	static IConfig* config;
public:
	static IConfig& GetConfig();
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
protected:
	IConfig();
	virtual ~IConfig();
/* ==================== Constructors and destructors begin ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/**
	 * Load configuration from a given config file.
	 *
	 * The correct format for storing configuration data is:
	 *	name = value - defines parameter "name" with value "val"
	 *	lines starting with "#" sign are marked as comments and ignored when file is being parsed
	 */
	virtual void LoadFromFile(const std::string& fileName) = 0;

	/**
	 * Returns the value (in a string) of given config parameter, or defValue
	 * if the parameter has not been specified in the configuration file.
	 */
	virtual std::string GetArg(const std::string& name, const std::string& defValue) const = 0;
	
	template <typename Type>
	Type Get(const std::string& name, const Type& defValue)
	{
		std::stringstream ss;
		ss << defValue;
		const std::string resultStr = GetArg(name, ss.str());

		//LOG(Delocust, LOGPLACE, "Config parameter \"%s\" collected with value \"%s\"", name.c_str(), resultStr.c_str());

		Type result;
		std::stringstream resultSS;
		resultSS << resultStr;
		DELOCUST_LOG("Config parameter \"%s\" collected with value \"%s\"", name.c_str(), resultSS.str().c_str());
		resultSS >> result;
		return result;
	}
/* ==================== Non-static member functions end ==================== */
}; /* end class IConfig */

} /* end namespace Utility */