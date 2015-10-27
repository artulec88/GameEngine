#ifndef __UTILITY_EXCEPTION_H__
#define __UTILITY_EXCEPTION_H__

#include "Utility.h"
#include <iostream>
#include <exception>
#include <string>

namespace Utility { namespace Exception
{

/// <summary>Exception associated with file not found error.</summary>
class UTILITY_API FileNotFoundException : public std::exception
{
/* ==================== Constructors and destructors begin ==================== */
public:
	FileNotFoundException(const std::string& filename) :
		m_filename(filename)
	{
	}
	virtual ~FileNotFoundException(void)
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual const char* what() const throw()
	{
		std::stringstream ss("");
		ss << "File \"" << m_filename << "\" not found.";
		return ss.str().c_str();
	}
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	std::string m_filename;
/* ==================== Non-static member variables end ==================== */
}; /* end class FileNotFoundException */

} /* end namespace Exception */

} /* end namespace Utility */

#endif /* __UTILITY_EXCEPTION_H__ */