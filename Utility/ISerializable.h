#ifndef __UTILITY_I_SERIALIZABLE_H__
#define __UTILITY_I_SERIALIZABLE_H__

#include "Utility.h"
#include <string>

namespace Utility
{

class UTILITY_API ISerializable
{
public:
	ISerializable(void)
	{
	}
	virtual ~ISerializable(void)
	{
	}
public:
	virtual std::string ToString() const = 0;
}; /* end class ISerializable */

} /* end namespace Utility */

#endif /* __UTILITY_I_SERIALIZABLE_H__ */