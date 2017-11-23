#ifndef __RENDERING_RESOURCE_H__
#define __RENDERING_RESOURCE_H__

#include "Rendering.h"
#include <string>

namespace rendering
{
	// TODO: This class belongs in the Utility library.
	class Resource
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Resource constructor. </summary>
		/// <param name="filename"> The file name. </param>
		explicit Resource(const std::string& filename);
		virtual ~Resource();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary>
		/// Loads the resource.
		/// </summary>
		virtual void Load() = 0;
		
		/// <summary>
		/// Releases the resource.
		/// </summary>
		virtual void Release() = 0;

		bool IsLoaded() const { return m_isLoaded; }

#ifdef ANT_TWEAK_BAR_ENABLED
		virtual void InitializeTweakBars() = 0;
#endif
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		bool m_isLoaded;
		int m_guid; // globally unique ID (hash value for a given filename)
		std::string m_filename;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Resource */

} /* end namespace rendering */

#endif // __RENDERING_RESOURCE_H__
