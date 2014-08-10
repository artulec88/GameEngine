#include "Rendering.h"
#include "Math\Matrix.h"

namespace Rendering
{

class RENDERING_API ShadowInfo
{
/* ==================== Non-static member variables begin ==================== */
private:
	Math::Matrix4D projectionMatrix;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ShadowInfo(const Math::Matrix4D& projection);
	virtual ~ShadowInfo();
private:
	ShadowInfo(const ShadowInfo& shadowInfo) {}
	void operator=(const ShadowInfo& shadowInfo) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline Math::Matrix4D GetProjection() const { return projectionMatrix; }
/* ==================== Non-static member functions end ==================== */
}; /* end class ShadowInfo */

} /* end namespace Rendering */