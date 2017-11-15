#include "stdafx.h"
#include "BaseCamera.h"

Rendering::BaseCamera::BaseCamera(const math::Matrix4D& projectionMatrix, math::Real sensitivity) :
	m_projection(projectionMatrix),
	m_sensitivity(sensitivity),
	m_isActive(false)
#ifdef ANT_TWEAK_BAR_ENABLED
	, m_prevFov(REAL_ZERO),
	m_fov(REAL_ZERO),
	m_prevAspectRatio(REAL_ZERO),
	m_aspectRatio(REAL_ZERO),
	m_prevNearPlane(REAL_ZERO),
	m_nearPlane(REAL_ZERO),
	m_prevFarPlane(REAL_ZERO),
	m_farPlane(REAL_ZERO)
#endif
{
}

Rendering::BaseCamera::BaseCamera(const math::Angle& FoV, math::Real aspectRatio, math::Real zNearPlane, math::Real zFarPlane, math::Real sensitivity) :
	m_projection(FoV, aspectRatio, zNearPlane, zFarPlane),
	m_sensitivity(sensitivity),
	m_isActive(false)
#ifdef ANT_TWEAK_BAR_ENABLED
	, m_prevFov(REAL_ZERO),
	m_fov(REAL_ZERO),
	m_prevAspectRatio(REAL_ZERO),
	m_aspectRatio(REAL_ZERO),
	m_prevNearPlane(REAL_ZERO),
	m_nearPlane(REAL_ZERO),
	m_prevFarPlane(REAL_ZERO),
	m_farPlane(REAL_ZERO)
#endif
{
}

Rendering::BaseCamera::~BaseCamera(void)
{
}

math::Matrix4D Rendering::BaseCamera::GetViewMatrix() const
{
	// TODO: Check which one is the fastest: SOLUTION #1, SOLUTION #2, etc
	/* ==================== SOLUTION #1 begin ==================== */
	// return GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix() * GetTransform().GetTransformedPos().Negated();
	/* ==================== SOLUTION #1 end ==================== */

	//Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	/* ==================== SOLUTION #2 begin ==================== */
	/* ==================== SOLUTION #2 end ==================== */
	/* ==================== SOLUTION #3 begin ==================== */
	// math::Matrix4D cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	/* ==================== SOLUTION #3 end ==================== */

	return GetTransform().GetRot().Conjugate().ToRotationMatrix() * math::Matrix4D(GetTransform().GetPos().Negated()); // FIXME: Check matrix multiplication
}

math::Matrix4D Rendering::BaseCamera::GetViewProjection() const
{
	// This function is performed quiet often. Maybe we could, instead of multiplying three matrices (projection, rotation, translation),
	// just remember the result in some member variable and reuse it. Of course, we would have to perform the multiplication again if any of these matrices were changed.
	// It would probably lead to better performance.

	// TODO: Check which one is the fastest: SOLUTION #1, SOLUTION #2, etc
	/* ==================== SOLUTION #1 begin ==================== */
	// return m_projection * GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix() * GetTransform().GetTransformedPos().Negated();
	/* ==================== SOLUTION #1 end ==================== */

	/*math::Matrix4D cameraTranslation(m_transform.GetTransformedPos().Negate());*/
	//Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	/* ==================== SOLUTION #2 begin ==================== */
	/* ==================== SOLUTION #2 end ==================== */
	/* ==================== SOLUTION #3 begin ==================== */
	// math::Matrix4D cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	/* ==================== SOLUTION #3 end ==================== */

	return m_projection * GetTransform().GetRot().Conjugate().ToRotationMatrix() * math::Matrix4D(GetTransform().GetPos().Negated()); // FIXME: Check matrix multiplication
	//return m_projection * m_viewMatrix;
}