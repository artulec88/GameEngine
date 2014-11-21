#include "Rendering.h"
#include "Math\Matrix.h"

namespace Rendering
{

class RENDERING_API ShadowInfo
{
/* ==================== Non-static member variables begin ==================== */
private:
	Math::Matrix4D projectionMatrix;
	bool flipFacesEnabled;
	Math::Real shadowSoftness;
	Math::Real lightBleedingReductionAmount;
	Math::Real minVariance;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ShadowInfo(const Math::Matrix4D& projection, bool flipFacesEnabled, Math::Real shadowSoftness = static_cast<Real>(1.0f), Math::Real lightBleedingReductionAmount = static_cast<Real>(0.2f), Math::Real minVariance = static_cast<Real>(0.00002f) );
	virtual ~ShadowInfo();
private:
	ShadowInfo(const ShadowInfo& shadowInfo) {}
	void operator=(const ShadowInfo& shadowInfo) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline Math::Matrix4D GetProjection() const { return projectionMatrix; }
	inline Math::Real GetShadowSoftness() const { return shadowSoftness; }
	inline Math::Real GetMinVariance() const { return minVariance; }
	inline Math::Real GetLightBleedingReductionAmount() const { return lightBleedingReductionAmount; }
	inline bool IsFlipFacesEnabled() const { return flipFacesEnabled; }
/* ==================== Non-static member functions end ==================== */
}; /* end class ShadowInfo */

} /* end namespace Rendering */
