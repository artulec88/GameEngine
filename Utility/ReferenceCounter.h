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
		/// <summary>
		/// Reference counter constructor. It creates an instance of the <code>ReferenceCounter</code> class and sets the initial references to <code>1</code>.
		/// </summary>
		UTILITY_API ReferenceCounter();
		
		/// <summary>
		/// Reference counter destructor.
		/// </summary>
		UTILITY_API virtual ~ReferenceCounter();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>
		/// Adds one reference to the instance.
		/// </summary>
		UTILITY_API void AddReference();

		/// <summary>
		/// Removes one reference to the instance.
		/// </summary>
		UTILITY_API void RemoveReference();

		/// <summary>
		/// Indicates whether the instance is still referenced or not.
		/// </summary>
		/// <returns>
		/// <code>true</code> if an instance is referenced by at least one object and <code>false</code> otherwise.
		/// </returns>
		UTILITY_API bool IsReferenced() const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The number of references being maintained at specific moment.
		/// </summary>
		int m_refCount;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class ReferenceCounter */

} /* end namespace Utility */

#endif /* __UTILITY_REFERENCE_COUNTER_H__ */