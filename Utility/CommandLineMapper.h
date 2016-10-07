#ifndef __UTILITY_COMMAND_LINE_MAPPER_H__
#define __UTILITY_COMMAND_LINE_MAPPER_H__

//#pragma warning(disable:4251)

#include "Utility.h"
#include "ICommandLineMapper.h"

#include <map>
#include <string>

namespace Utility
{
	class CommandLineMapper : public ICommandLineMapper
	{
		typedef std::map<std::string, std::string> ArgsToValuesMap;
		/* ==================== Constructors and destructors begin ==================== */
	public:
		CommandLineMapper(int argc, char* argv[]);
		virtual ~CommandLineMapper();
		CommandLineMapper(const CommandLineMapper& commandLineMapper) = delete;
		CommandLineMapper(CommandLineMapper&& commandLineMapper) = delete;
		CommandLineMapper& operator=(const CommandLineMapper& commandLineMapper) = delete;
		CommandLineMapper& operator=(CommandLineMapper&& commandLineMapper) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual const std::string& Get(const std::string& opt, const std::string& defaultValue) const;
		virtual inline bool IsPresent(const std::string& opt) const { return m_argsToValuesMap.find(opt) != m_argsToValuesMap.end(); }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		// TODO: Check if Array<std::string> is a better solution than using STL vector
		//Container
		ArgsToValuesMap m_argsToValuesMap;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CommandLineMapper */

} /* end namespace Utility */

#endif /* __UTILITY_COMMAND_LINE_MAPPER_H__ */