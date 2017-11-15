#ifndef __UTILITY_ICONFIG_H__
#define __UTILITY_ICONFIG_H__

#include "Utility.h"
#include "ILogger.h"
#include <sstream>
#include <memory>
#include <map>
#include <string>
#define GET_CONFIG_VALUE(moduleName, cfgName, defValue) utility::IConfig::GetConfig(moduleName).Get(cfgName, defValue)
#define GET_CONFIG_VALUE_STR(moduleName, cfgName, defValue) utility::IConfig::GetConfig(moduleName).GetArg(cfgName, defValue)
#define HAS_CONFIG_VALUE(moduleName, cfgName) utility::IConfig::GetConfig(moduleName).HasArg(cfgName)

namespace utility
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
		/// <summary> Base configuration holder constructor. </summary>
		IConfig();

		/// <summary> Base configuration holder destructor. </summary>
		virtual ~IConfig();

		/// <summary> Base configuration holder copy constructor. </summary>
		/// <param name="config"> The configuration holder to copy construct from. </param>
		IConfig(const IConfig& config) = delete;

		/// <summary> Base configuration holder move constructor. </summary>
		/// <param name="config"> The configuration holder to move construct from. </param>
		IConfig(IConfig&& config) = delete;

		/// <summary> Base configuration holder copy constructor. </summary>
		/// <param name="config"> The configuration holder to copy construct from. </param>
		/// <returns> The reference to the newly copy-assigned configuration holder. </returns>
		IConfig& operator=(const IConfig& config) = delete;

		/// <summary> Base configuration holder move constructor. </summary>
		/// <param name="config"> The configuration holder to move construct from. </param>
		/// <returns> The reference to the newly move-assigned configuration holder. </returns>
		IConfig& operator=(IConfig&& config) = delete;
		/* ==================== Constructors and destructors begin ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary>
		/// Load configuration from a given config file. The correct format for storing configuration data is:
		/// <code>name = value</code> - defines parameter "name" with value "val".
		/// Lines starting with "#" sign are marked as comments and ignored when file is being parsed
		/// </summary>
		/// <param name="fileName"> The name of the file containing the configuration to be loaded. </param>
		virtual void LoadFromFile(const std::string& fileName) = 0;

		/// <summary>
		/// Returns the value (in a string) of given config parameter, or defValue
		/// if the parameter has not been specified in the configuration file.
		/// </summary>
		/// <param name="name"> Name of the configuration parameter. </param>
		/// <param name="defValue"> Default value for the configuration parameter <paramref name="name"/> if no specified value has been defined for it. </param>
		/// <returns> The value specified for the configuration parameter <paramref name="name"/> and <paramref name="defValue"/> if no such value has been defined. </returns>
		virtual std::string GetArg(const std::string& name, const std::string& defValue) const = 0;

		/// <summary>
		/// Gives information whether or not the configuration has a value defined for the argument with given <paramref name="name"/>.
		/// If no such argument is found then <code>false</code> is returned.
		/// </summary>
		/// <returns>
		/// <code>true</code> if the configuration has some value specified for the argument with the specified <paramref name="name"/>.
		/// Otherwise <code>false</code> is returned.
		/// </returns>
		virtual bool HasArg(const std::string& name) const = 0;

		template <typename Type>
		Type Get(const std::string& name, const Type& defValue) const
		{
			std::stringstream ss;
			ss << defValue;
			const std::string resultStr = GetArg(name, ss.str());

			//DELOCUST_LOG_UTILITY("Config parameter \"", name, "\" collected with value \"", resultStr, "\"");

			Type result;
			std::stringstream resultStream;
			resultStream << resultStr;
			DELOCUST_LOG_UTILITY("Config parameter \"", name, "\" collected with value \"", resultStream.str(), "\"");
			resultStream >> result;
			return result;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	//protected:
		/* ==================== Non-static member functions end ==================== */
	}; /* end class IConfig */

} /* end namespace Utility */

#endif /* __UTILITY_ICONFIG_H__ */