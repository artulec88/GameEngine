#pragma once

#include "Utility.h"
#include <iostream>
#include <exception>

/**
 * Example class defining a specific exception
 */
class UTILITY_API FileNotFoundException : public std::exception
{
public:
	FileNotFoundException(void);
	virtual ~FileNotFoundException(void);

public:
	virtual const char* what() const throw()
	{
		return "File not found";
	}
};

