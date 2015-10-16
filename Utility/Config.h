#pragma once

//#pragma warning(disable:4251)

#include "Utility.h"
#include "ILogger.h"
#include <string>
#include <sstream>
#include <map>
#include "IConfig.h"

namespace Utility
{

/**
 * Class used to manage global configuration parameters defined in config.cfg file
 */
class Config : public IConfig
{
/* ==================== Static variables and functions begin ==================== */
protected:
	typedef std::map<std::string, std::string> ValuesMap;
	ValuesMap cfgValues;
	//ValuesMap cfgNotDefinedValues;
	bool isInitialized;
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Config(void);
//	Config(const std::string& fileName);
//	~Config(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/**
	 * Load configuration from a given config file.
	 *
	 * The correct format for storing configuration data is:
	 *	name = value - defines parameter "name" with value "val"
	 *	lines starting with "#" sign are marked as comments and ignored when file is being parsed
	 */
	void LoadFromFile(const std::string& fileName);

	/**
	 * Returns the value of given config parameter, or defValue
	 * if the parameter was not specified in the configuration file.
	 */
	//const Type Get(const std::string& name, const Type& defValue)
	//{
	//	if (!isInitialized)
	//	{
	//		WARNING_LOG("The Config instance is not initalized.");
	//		//std::string fileName;
	//		//std::cout << "Specify the configuration file to read:\t";
	//		// TODO: If the user gives just a filename concatenate it with the string
	//		// "..\\Config\\"
	//		// If the user gives whole path use it instead.
	//		// TODO: Second thing to do is to make stream read the whole line and not stop at first white space.
	//		//std::cin >> fileName;
	//		LoadFromFile("..\\Config\\Config.cfg");
	//	}

	//	ValuesMap::iterator valueMapIt = cfgValues.find(name);
	//	if (valueMapIt == cfgValues.end())
	//	{
	//		std::stringstream s;
	//		s << defValue;
	//		NOTICE_LOG("The parameter \"%s\" has not been specified. Using default value \"%s\"", name.c_str(), s.str().c_str());
	//		cfgNotDefinedValues[name] = s.str();
	//		return defValue;
	//	}

	//	Type value;
	//	std::stringstream s;
	//	s << valueMapIt->second;
	//	s >> value;
	//	return value;
	//}

	std::string GetArg(const std::string& name, const std::string& defValue) const;

	/**
	 * Returns formatted list of configure parameters, that were used but defined in the configuration file,
	 * together with their last used default value.
	 */
	//std::string ReportUndefined();
/* ==================== Non-static member functions end ==================== */
}; /* end class Config */

} /* end namespace Utility */