#ifndef __UTILITY_CONFIG_H__
#define __UTILITY_CONFIG_H__

//#pragma warning(disable:4251)

#include "Utility.h"
#include "ILogger.h"
#include <string>
#include <sstream>
#include <map>
#include "IConfig.h"

namespace Utility
{

	/// <summary>
	/// Class used to manage global configuration parameters defined in config.cfg file
	/// </summary>
	class Config : public IConfig
	{
		using ValuesMap = std::map<std::string, std::string>;
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Config default constructor. </summary>
		explicit Config();
		/// <summary> Config destructor. </summary>
		virtual ~Config();
		/// <summary> Config copy constructor. </summary>
		Config(const Config& config) = delete;
		/// <summary> Config move constructor. </summary>
		Config(Config&& config) = delete;
		/// <summary> Config copy assignment operator. </summary>
		Config& operator=(const Config& config) = delete;
		/// <summary> Config move assignment operator. </summary>
		Config& operator=(Config&& config) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>
		/// Load configuration from a given config file.
		/// The correct format for storing configuration data is:
		/// <code>name = value</code> - defines parameter "name" with value "val".
		/// Lines starting with "#" sign are marked as comments and ignored when file is being parsed.
		/// </summary>
		virtual void LoadFromFile(const std::string& fileName) override;

		/**
		* Returns the value of given config parameter, or defValue
		* if the parameter was not specified in the configuration file.
		*/
		//const Type Get(const std::string& name, const Type& defValue)
		//{
		//	if (!isInitialized)
		//	{
		//		WARNING_LOG_UTILITY("The Config instance is not initalized.");
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
		//		NOTICE_LOG_UTILITY("The parameter \"", name, "\" has not been specified. Using default value \"", s.str(), "\"");
		//		cfgNotDefinedValues[name] = s.str();
		//		return defValue;
		//	}

		//	Type value;
		//	std::stringstream s;
		//	s << valueMapIt->second;
		//	s >> value;
		//	return value;
		//}

		virtual std::string GetArg(const std::string& name, const std::string& defValue) const override;

		virtual bool HasArg(const std::string& name) const override;

		/**
		* Returns formatted list of configure parameters, that were used but defined in the configuration file,
		* together with their last used default value.
		*/
		//std::string ReportUndefined();
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		ValuesMap m_cfgValues;
		//ValuesMap m_cfgNotDefinedValues;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Config */

} /* end namespace Utility */

#endif /* __UTILITY_CONFIG_H__ */