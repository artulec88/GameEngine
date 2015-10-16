#pragma once

//#pragma warning(disable:4251)

#include "Utility.h"

namespace Utility
{
	class UTILITY_API ReferenceCounter
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		ReferenceCounter();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		void AddReference();
		void RemoveReference();
		bool IsReferenced() const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		int m_refCount;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class ReferenceCounter */

} /* end namespace Utility */