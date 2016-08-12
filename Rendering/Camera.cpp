#include "StdAfx.h"
#include "Camera.h"
#include "Renderer.h"

#include "Math\Quaternion.h"
#ifdef ANT_TWEAK_BAR_ENABLED
#include "Math\FloatingPoint.h"
#endif

#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

#include <sstream>

Rendering::Camera::Camera(const Math::Vector3D& position, const Math::Quaternion& rotation, const Math::Matrix4D& projectionMatrix, Math::Real sensitivity) :
	m_pos(position),
	m_rot(rotation),
	m_projection(projectionMatrix),
	//m_viewMatrix(m_transform.GetTransformedRot().Conjugate().ToRotationMatrix() * Math::Matrix4D(m_transform.GetTransformedPos().Negate())),
	m_sensitivity(sensitivity),
	m_isActive(false)
#ifdef ANT_TWEAK_BAR_ENABLED
	, m_prevFov(0.0f),
	m_fov(0.0f),
	m_prevAspectRatio(0.0f),
	m_aspectRatio(0.0f),
	m_prevNearPlane(0.0f),
	m_nearPlane(0.0f),
	m_prevFarPlane(0.0f),
	m_farPlane(0.0f)
#endif
{
}

Rendering::Camera::Camera(const Math::Vector3D& position, const Math::Quaternion& rotation, const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity) :
	m_pos(position),
	m_rot(rotation),
	m_projection(FoV, aspectRatio, zNearPlane, zFarPlane),
	//m_viewMatrix(m_transform.GetTransformedRot().Conjugate().ToRotationMatrix() * Math::Matrix4D(m_transform.GetTransformedPos().Negate())),
	m_sensitivity(sensitivity),
	m_isActive(false)
#ifdef ANT_TWEAK_BAR_ENABLED
	,m_prevFov(FoV),
	m_fov(FoV),
	m_prevAspectRatio(aspectRatio),
	m_aspectRatio(aspectRatio),
	m_prevNearPlane(zNearPlane),
	m_nearPlane(zNearPlane),
	m_prevFarPlane(zFarPlane),
	m_farPlane(zFarPlane)
#endif
{
}

Rendering::Camera::~Camera()
{
}

Math::Matrix4D Rendering::Camera::GetViewMatrix() const
{
	// TODO: Check which one is the fastest: SOLUTION #1, SOLUTION #2, etc
	/* ==================== SOLUTION #1 begin ==================== */
	// return GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix() * GetTransform().GetTransformedPos().Negated();
	/* ==================== SOLUTION #1 end ==================== */

	//Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	/* ==================== SOLUTION #2 begin ==================== */
	/* ==================== SOLUTION #2 end ==================== */
	/* ==================== SOLUTION #3 begin ==================== */
	// Math::Matrix4D cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	/* ==================== SOLUTION #3 end ==================== */

	return m_rot.Conjugate().ToRotationMatrix() * Math::Matrix4D(m_pos.Negated()); // FIXME: Check matrix multiplication
}

Math::Matrix4D Rendering::Camera::GetViewProjection() const
{
	// This function is performed quiet often. Maybe we could, instead of multiplying three matrices (projection, rotation, translation),
	// just remember the result in some member variable and reuse it. Of course, we would have to perform the multiplication again if any of these matrices were changed.
	// It would probably lead to better performance.

	// TODO: Check which one is the fastest: SOLUTION #1, SOLUTION #2, etc
	/* ==================== SOLUTION #1 begin ==================== */
	// return m_projection * GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix() * GetTransform().GetTransformedPos().Negated();
	/* ==================== SOLUTION #1 end ==================== */

	/*Math::Matrix4D cameraTranslation(m_transform.GetTransformedPos().Negate());*/
	//Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	/* ==================== SOLUTION #2 begin ==================== */
	/* ==================== SOLUTION #2 end ==================== */
	/* ==================== SOLUTION #3 begin ==================== */
	// Math::Matrix4D cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	/* ==================== SOLUTION #3 end ==================== */

	return m_projection * m_rot.Conjugate().ToRotationMatrix() * Math::Matrix4D(m_pos.Negated()); // FIXME: Check matrix multiplication
	//return m_projection * m_viewMatrix;
}