#include "Rendering.h"
#include "Math\Matrix.h"

namespace Rendering
{

class RENDERING_API ShadowInfo
{
/* ==================== Non-static member variables begin ==================== */
private:
	Math::Matrix4D projectionMatrix;
	Math::Real bias;
	bool flipFacesEnabled;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ShadowInfo(const Math::Matrix4D& projection, Math::Real bias, bool flipFacesEnabled);
	virtual ~ShadowInfo();
private:
	ShadowInfo(const ShadowInfo& shadowInfo) {}
	void operator=(const ShadowInfo& shadowInfo) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline Math::Matrix4D GetProjection() const { return projectionMatrix; }
	inline Math::Real GetBias() const { return bias; }
	inline bool IsFlipFacesEnabled() const { return flipFacesEnabled; }
/* ==================== Non-static member functions end ==================== */
}; /* end class ShadowInfo */

} /* end namespace Rendering */
