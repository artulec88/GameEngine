#ifndef __UTILITY_ISERIALIZABLE_H__
#define __UTILITY_ISERIALIZABLE_H__

#include "Utility.h"
#include <string>

namespace utility
{

	class ISerializable
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		UTILITY_API ISerializable()
		{
		}
		UTILITY_API virtual ~ISerializable()
		{
		}
		/* ==================== Constructors and destructors begin ==================== */

		/* ==================== Non-static member functions begin ==================== */
		UTILITY_API virtual std::string ToString() const = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member functions end ==================== */
	}; /* end class ISerializable */

} /* end namespace utility */

#endif /* __UTILITY_ISERIALIZABLE_H__ */