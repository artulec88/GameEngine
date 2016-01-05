#ifndef __UTILITY_REFERENCE_COUNTER_H__
#define __UTILITY_REFERENCE_COUNTER_H__


//#pragma warning(disable:4251)

#include "Utility.h"

namespace Utility
{

/// <summary>
/// A basic reference-counter class.
/// </summary>
class ReferenceCounter
{
/* ==================== Constructors and destructors begin ==================== */
protected:
	UTILITY_API ReferenceCounter();
	UTILITY_API virtual ~ReferenceCounter();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	UTILITY_API void AddReference();
	UTILITY_API void RemoveReference();
	UTILITY_API bool IsReferenced() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	int m_refCount;
/* ==================== Non-static member variables end ==================== */
}; /* end class ReferenceCounter */

} /* end namespace Utility */

#endif /* __UTILITY_REFERENCE_COUNTER_H__ */