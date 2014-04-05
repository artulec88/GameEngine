#pragma once

//#pragma warning(disable:4251)

#include "Utility.h"

namespace Utility
{

	class UTILITY_API ReferenceCounter
	{
	protected: // member variables
		int refCount;

	public: // constructors and destructors
		ReferenceCounter();

	public:
		//int GetReferenceCount() const;
		void AddReference();
		void RemoveReference();
		bool IsReferenced() const;
	}; /* end class ReferenceCounter */

} /* end namespace Utility */