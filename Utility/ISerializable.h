#pragma once

#include "Utility.h"
#include <string>

namespace Utility
{

class UTILITY_API ISerializable
{
public:
	ISerializable(void);
	~ISerializable(void);

public:
	virtual std::string ToString() const = 0;
}; /* end class ISerializable */

} /* end namespace Utility */