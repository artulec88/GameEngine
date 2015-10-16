#pragma once

#include "Utility.h"
#include <iostream>
#include <exception>

namespace Utility
{

/**
 * Example class defining a specific exception
 */
class UTILITY_API FileNotFoundException : public std::exception
{
/* ==================== Constructors and destructors begin ==================== */
public:
	FileNotFoundException(void);
	virtual ~FileNotFoundException(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual const char* what() const throw()
	{
		return "File not found";
	}
/* ==================== Non-static member functions end ==================== */
}; /* end class FileNotFoundException */

} /* end namespace Utility */