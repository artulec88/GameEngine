#ifndef __UTILITY_ICOMMAND_H__
#define __UTILITY_ICOMMAND_H__

#include "Utility.h"
#include <iostream>
#include <memory>

namespace Utility
{

class ICommand
{
/* ==================== Static variables and functions begin ==================== */
public:
	/// <summary>
	/// Creates an <code>ICommand</code> specific implementation object.
	/// </summary>
	/// <remarks>
	/// To hide the specific implementation from the client we provide the factory-like method to create specific command instances.
	/// </remarks>
	UTILITY_API static std::unique_ptr<Utility::ICommand> CreateCommand(int argc, char* argv[]);
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ICommand(int argc, char* argv[]);
	virtual ~ICommand();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	UTILITY_API virtual bool IsPresent(const std::string& opt) const = 0;
	UTILITY_API virtual std::string Get(const std::string& opt, const std::string& defaultValue) const = 0;
protected:
	//template<typename Type> Type Get(int number, const Type& defaultValue) const;
/* ==================== Non-static member functions end ==================== */
}; /* end class ICommand */

} /* end namespace Utility */

#endif /* __UTILITY_ICOMMAND_H__ */