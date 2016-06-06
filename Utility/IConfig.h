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
		IConfig(const std::string& fileName);
		virtual ~IConfig();
	/* ==================== Constructors and destructors begin ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>
		/// Load configuration from a given config file. The correct format for storing configuration data is:
		/// <code>name = value</code> - defines parameter "name" with value "val".
		/// Lines starting with "#" sign are marked as comments and ignored when file is being parsed
		/// </summary>
		virtual void LoadFromFile() = 0;

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

			//DELOCUST_LOG("Config parameter \"%s\" collected with value \"%s\"", name.c_str(), resultStr.c_str());

			Type result;
			std::stringstream resultSS;
			resultSS << resultStr;
			DELOCUST_LOG("Config parameter \"%s\" collected with value \"%s\"", name.c_str(), resultSS.str().c_str());
			resultSS >> result;
			return result;
		}
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		const std::string m_fileName;
	/* ==================== Non-static member functions end ==================== */
	}; /* end class IConfig */

} /* end namespace Utility */

#endif /* __UTILITY_ICONFIG_H__ */