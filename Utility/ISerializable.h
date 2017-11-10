#ifndef __UTILITY_ISERIALIZABLE_H__
#define __UTILITY_ISERIALIZABLE_H__

#include "Utility.h"
#include <string>

namespace Utility
{

	class ISerializable
	{
	public:
		UTILITY_API ISerializable()
		{
		}
		UTILITY_API virtual ~ISerializable()
		{
		}
	public:
		UTILITY_API virtual std::string ToString() const = 0;
	}; /* end class ISerializable */

} /* end namespace Utility */

#endif /* __UTILITY_ISERIALIZABLE_H__ */