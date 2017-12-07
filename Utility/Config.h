#ifndef __UTILITY_CONFIG_H__
#define __UTILITY_CONFIG_H__

#include "Utility.h"
#include "IConfig.h"
#include <string>
#include <map>

namespace utility
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
		/// <param name="config"> The configuration to copy construct from. </param>
		Config(const Config& config) = delete;

		/// <summary> Config move constructor. </summary>
		/// <param name="config"> The configuration to move construct from. </param>
		Config(Config&& config) = delete;

		/// <summary> Config copy assignment operator. </summary>
		/// <param name="config"> The configuration to copy assign from. </param>
		/// <returns> The newly copy-constructed configuration. </returns>
		Config& operator=(const Config& config) = delete;

		/// <summary> Config move assignment operator. </summary>
		/// <param name="config"> The configuration to move assign from. </param>
		/// <returns> The newly move-assigned configuration. </returns>
		Config& operator=(Config&& config) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary>
		/// Load configuration from a given config file. The correct format for storing configuration data is:
		/// <code>name = value</code> - defines parameter "name" with value "val".
		/// Lines starting with "#" sign are marked as comments and ignored when file is being parsed.
		/// </summary>
		/// <param name="fileName"> The file name to read the configuration from. </param>
		void LoadFromFile(const std::string& fileName) override;

		/// <summary>
		/// Returns the value for the given attribute with the specified <paramref name="name"/>.
		/// In case the attribute is not found then <paramref name="defValue"/> is returned.
		/// </summary>
		/// <param name="name"> Name of the configuration attribute that we want the value of. </param>
		/// <param name="defValue">
		/// Default value that is returned if configuration attribute with name <paramref name="name"/> is not found.
		/// </param>
		/// <returns>
		/// Value assigned to the configuration attribute with the given name <paramref name="name"/>.
		/// If attribute cannot be found then <paramref name="defValue"/> is returned.
		/// </returns>
		std::string GetArg(const std::string& name, const std::string& defValue) const override;

		/// <summary>
		/// Returns <code>true</code> if the specified parameter <paramref name="name"/> has been
		/// given a value in the configuration. Otherwise <code>false</code> is returned.
		/// </summary>
		/// <param name="name">
		/// Name of the parameter to check whether it's been given a value in the configuration or not.
		/// </param>
		/// <returns>
		/// <code>true</code> if the specified parameter <paramref name="name"/> has been
		/// given a value in the configuration. Otherwise <code>false</code> is returned.
		/// </returns>
		bool HasArg(const std::string& name) const override;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		ValuesMap m_cfgValues;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Config */

} /* end namespace Utility */

#endif /* __UTILITY_CONFIG_H__ */