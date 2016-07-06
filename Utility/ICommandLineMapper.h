#ifndef __UTILITY_ICOMMAND_LINE_MAPPER_H__
#define __UTILITY_ICOMMAND_LINE_MAPPER_H__

#include "Utility.h"
#include <iostream>
#include <memory>

namespace Utility
{
	/// <summary>
	/// The object giving the user an easy access to command-line parameters defined at the startup of the application.
	/// </summary>
	class ICommandLineMapper
	{
		/* ==================== Static variables and functions begin ==================== */
	public:
		/// <summary>
		/// Creates an <code>ICommandLineMapper</code> specific implementation object.
		/// </summary>
		/// <remarks>
		/// To hide the specific implementation from the client we provide the factory-like method to create specific command instances.
		/// </remarks>
		UTILITY_API static std::unique_ptr<Utility::ICommandLineMapper> CreateCommandLineMapper(int argc, char* argv[]);
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ICommandLineMapper();
		virtual ~ICommandLineMapper();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		UTILITY_API virtual inline bool IsPresent(const std::string& opt) const = 0;
		UTILITY_API virtual const std::string& Get(const std::string& opt, const std::string& defaultValue) const = 0;
	protected:
		//template<typename Type> Type Get(int number, const Type& defaultValue) const;
	/* ==================== Non-static member functions end ==================== */
	}; /* end class ICommandLineMapper */

} /* end namespace Utility */

#endif /* __UTILITY_ICOMMAND_LINE_MAPPER_H__ */