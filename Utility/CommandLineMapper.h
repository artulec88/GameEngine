#ifndef __UTILITY_COMMAND_LINE_MAPPER_H__
#define __UTILITY_COMMAND_LINE_MAPPER_H__

#include "Utility.h"
#include "ICommandLineMapper.h"

#include <map>
#include <string>

namespace utility
{
	class CommandLineMapper : public ICommandLineMapper
	{
		using ArgsToValuesMap = std::map<std::string, std::string>;
		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Command line parameters mapping object constructor. </summary>
		/// <param name="argc"> The number of command line parameters. </param>
		/// <param name="argv"> The array of command line parameter values. </param>
		CommandLineMapper(const int argc, char* argv[]);
		
		/// <summary> Command line parameters mapping object destructor. </summary>
		virtual ~CommandLineMapper();

		/// <summary> Command line parameters mapping object copy constructor. </summary>
		/// <param name="commandLineMapper"> The command line parameters mapping object to copy construct from. </param>
		CommandLineMapper(const CommandLineMapper& commandLineMapper) = delete;

		/// <summary> Command line parameters mapping object move constructor. </summary>
		/// <param name="commandLineMapper"> The command line parameters mapping object to move construct from. </param>
		CommandLineMapper(CommandLineMapper&& commandLineMapper) = delete;

		/// <summary> Command line parameters mapping object copy assignment operator. </summary>
		/// <param name="commandLineMapper"> The command line parameters mapping object to copy assign from. </param>
		CommandLineMapper& operator=(const CommandLineMapper& commandLineMapper) = delete;

		/// <summary> Command line parameters mapping object move assignment operator. </summary>
		/// <param name="commandLineMapper"> The command line parameters mapping object to move assign from. </param>
		CommandLineMapper& operator=(CommandLineMapper&& commandLineMapper) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary>
		/// Returns <code>true</code> if the argument <paramref name="opt"/> is found in the map of property-value pairs and <code>false</code> otherwise.
		/// </summary>
		/// <returns>
		/// <code>true</code> if the argument <paramref name="opt"/> is found in the map of property-value pairs and <code>false</code> otherwise.
		/// </returns>
		bool IsPresent(const std::string& opt) const override
		{
			return m_argsToValuesMap.find(opt) != m_argsToValuesMap.end();
		}

		/// <summary>
		/// Retrieves the <code>std::string</code> representation of the value that was assigned in the command line for the argument <paramref name="opt"/>.
		/// In case of an error (in particular, if no such argument is found) then <paramref name="defaultValue"/> is returned.
		/// </summary>
		/// <param name="opt">The name of the argument we want to retrieve the value for. </param>
		/// <param name="defaultValue">
		/// The default value that will be returned in case of an error (e.g. when argument does not have any value assigned).
		/// </param>
		/// <returns>
		/// The <code>std::string</code> representation of the value that was assigned in the command line for the argument <paramref name="opt"/>.
		/// In case of an error (in particular, if no such argument is found) then <paramref name="defaultValue"/> is returned.
		/// </returns>
		const std::string& Get(const std::string& opt, const std::string& defaultValue) const override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		// TODO: Check if Array<std::string> is a better solution than using STL vector
		//Container
		ArgsToValuesMap m_argsToValuesMap;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CommandLineMapper */

} /* end namespace Utility */

#endif /* __UTILITY_COMMAND_LINE_MAPPER_H__ */