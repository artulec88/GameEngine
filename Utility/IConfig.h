#ifndef __UTILITY_ICONFIG_H__
#define __UTILITY_ICONFIG_H__

#include "Utility.h"
#include "ILogger.h"
#include <sstream>
#include <memory>
#include <map>
#include <string>
#define GET_CONFIG_VALUE(moduleName, cfgName, defValue) Utility::IConfig::GetConfig(moduleName).Get(cfgName, defValue)
#define GET_CONFIG_VALUE_STR(moduleName, cfgName, defValue) Utility::IConfig::GetConfig(moduleName).GetArg(cfgName, defValue)

namespace Utility
{
	/// <summary>
	/// The interface for the application's configuration container. It gives the easy access to configuration parameters and their values.
	/// </summary>
	class IConfig
	{
		/* ==================== Static variables and functions begin ==================== */
	public:
		static std::map<std::string, std::unique_ptr<IConfig>> configurations;
		UTILITY_API static void CreateConfig(const std::string& moduleName, const std::string& configFileName);
		UTILITY_API static const IConfig& GetConfig(const std::string& moduleName);
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		IConfig();
		virtual ~IConfig();
		IConfig(const IConfig& config) = delete;
		IConfig(IConfig&& config) = delete;
		IConfig& operator=(const IConfig& config) = delete;
		IConfig& operator=(IConfig&& config) = delete;
		/* ==================== Constructors and destructors begin ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>
		/// Load configuration from a given config file. The correct format for storing configuration data is:
		/// <code>name = value</code> - defines parameter "name" with value "val".
		/// Lines starting with "#" sign are marked as comments and ignored when file is being parsed
		/// </summary>
		virtual void LoadFromFile(const std::string& fileName) = 0;

		/// <summary>
		/// Returns the value (in a string) of given config parameter, or defValue
		/// if the parameter has not been specified in the configuration file.
		///
		virtual std::string GetArg(const std::string& name, const std::string& defValue) const = 0;

		template <typename Type>
		Type Get(const std::string& name, const Type& defValue) const
		{
			std::stringstream ss;
			ss << defValue;
			const std::string resultStr = GetArg(name, ss.str());

			//DELOCUST_LOG_UTILITY("Config parameter \"", name, "\" collected with value \"", resultStr, "\"");

			Type result;
			std::stringstream resultSS;
			resultSS << resultStr;
			DELOCUST_LOG_UTILITY("Config parameter \"", name, "\" collected with value \"", resultSS.str(), "\"");
			resultSS >> result;
			return result;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member functions end ==================== */
	}; /* end class IConfig */

} /* end namespace Utility */

#endif /* __UTILITY_ICONFIG_H__ */