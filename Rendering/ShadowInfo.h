#pragma once

#include "Rendering.h"
#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"

namespace Rendering
{

struct ShadowCameraTransform
{
	Math::Vector3D pos;
	Math::Quaternion rot;
};

class RENDERING_API ShadowInfo
{
/* ==================== Constructors and destructors begin ==================== */
public:
	ShadowInfo(const Math::Matrix4D& projection, bool flipFacesEnabled,
		int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness = REAL_ONE,
		Math::Real lightBleedingReductionAmount = static_cast<Math::Real>(0.2f),
		Math::Real minVariance = static_cast<Math::Real>(0.00002f) );
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
	inline int GetShadowMapSizeAsPowerOf2() const { return shadowMapSizeAsPowerOf2; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Matrix4D projectionMatrix;
	bool flipFacesEnabled;
	int shadowMapSizeAsPowerOf2;
	Math::Real shadowSoftness;
	Math::Real lightBleedingReductionAmount;
	Math::Real minVariance;
/* ==================== Non-static member variables end ==================== */
}; /* end class ShadowInfo */

} /* end namespace Rendering */
