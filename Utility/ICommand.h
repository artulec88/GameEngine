#pragma once

#include <iostream>
#include "Utility.h"

namespace Utility
{

class UTILITY_API ICommand
{
/* ==================== Static variables and functions begin ==================== */
protected:
	static ICommand* command;
public:
	static void SetCommand(int argc, char* argv[]);
	static ICommand& GetCommand();
	static void DeleteCommand();
/* ==================== Static variables and functions end ==================== */

protected: // constructors and destructors
	ICommand();
	virtual ~ICommand();

public:
	virtual bool IsPresent(const std::string& opt) const = 0;
	virtual void Fill(int argc, char* argv[]) = 0;
	virtual std::string Get(const std::string& opt, const std::string& defaultValue) const = 0;

protected:
	//template<typename Type> Type Get(int number, const Type& defaultValue) const;
}; /* end class ICommand */

} /* end namespace Utility */