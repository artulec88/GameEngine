#pragma once

#include <iostream>
#include "Utility.h"

namespace Utility
{

class ICommand
{
/* ==================== Static variables and functions begin ==================== */
protected:
	static ICommand* command;
public:
	UTILITY_API static void SetCommand(int argc, char* argv[]);
	UTILITY_API static ICommand& GetCommand();
	UTILITY_API static void DeleteCommand();
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
protected:
	ICommand();
	virtual ~ICommand();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	UTILITY_API virtual bool IsPresent(const std::string& opt) const = 0;
	UTILITY_API virtual void Fill(int argc, char* argv[]) = 0;
	UTILITY_API virtual std::string Get(const std::string& opt, const std::string& defaultValue) const = 0;
protected:
	//template<typename Type> Type Get(int number, const Type& defaultValue) const;
/* ==================== Non-static member functions end ==================== */
}; /* end class ICommand */

} /* end namespace Utility */