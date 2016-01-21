#pragma once

#include "Rendering.h"
#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"

namespace Rendering
{

struct ShadowCameraTransform
{
	ShadowCameraTransform(Math::Vector3D& position, Math::Quaternion& rotation) :
		m_pos(position),
		m_rot(rotation)
	{
	}
	
	Math::Vector3D m_pos;
	Math::Quaternion m_rot;
};

class ShadowInfo
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
	inline Math::Matrix4D GetProjection() const { return m_projectionMatrix; }
	inline Math::Real GetShadowSoftness() const { return m_shadowSoftness; }
	inline Math::Real GetMinVariance() const { return m_minVariance; }
	inline Math::Real GetLightBleedingReductionAmount() const { return m_lightBleedingReductionAmount; }
	inline bool IsFlipFacesEnabled() const { return m_flipFacesEnabled; }
	inline int GetShadowMapSizeAsPowerOf2() const { return m_shadowMapSizeAsPowerOf2; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Matrix4D m_projectionMatrix;
	bool m_flipFacesEnabled;
	int m_shadowMapSizeAsPowerOf2;
	Math::Real m_shadowSoftness;
	Math::Real m_lightBleedingReductionAmount;
	Math::Real m_minVariance;
/* ==================== Non-static member variables end ==================== */
}; /* end class ShadowInfo */

} /* end namespace Rendering */
