#ifndef __UTILITY_I_SERIALIZABLE_H__
#define __UTILITY_I_SERIALIZABLE_H__

#include "Utility.h"
#include <string>

namespace Utility
{

class ISerializable
{
public:
	UTILITY_API ISerializable(void)
	{
	}
	UTILITY_API virtual ~ISerializable(void)
	{
	}
public:
	UTILITY_API virtual std::string ToString() const = 0;
}; /* end class ISerializable */

} /* end namespace Utility */

#endif /* __UTILITY_I_SERIALIZABLE_H__ */