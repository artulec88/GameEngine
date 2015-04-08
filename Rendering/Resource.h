#ifndef __RENDERING_RESOURCE_H__
#define __RENDERING_RESOURCE_H__

#include "Rendering.h"
#include <string>

namespace Rendering
{

class Resource
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Resource(const std::string& filename);
	virtual ~Resource(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Load() = 0; // Loads the resource
	virtual void Release() = 0; // Releases the resource

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

} /* end namespace Rendering */

#endif // __RENDERING_RESOURCE_H__
