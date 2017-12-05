#include "StdAfx.h"
#include "Camera.h"

#include "Math/Quaternion.h"

rendering::Camera::Camera(const CameraBuilder& cameraBuilder) :
	m_projection(cameraBuilder.GetProjectionMatrix()),
	m_sensitivity(cameraBuilder.GetSensitivity()),
	m_isActive(false),
	m_pos(cameraBuilder.GetPos()),
	m_rot(cameraBuilder.GetRot())
{
}

rendering::Camera::~Camera()
{
}

math::Matrix4D rendering::Camera::GetViewMatrix() const
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

	return m_rot.Conjugate().ToRotationMatrix() * math::Matrix4D(m_pos.Negated()); // FIXME: Check matrix multiplication
}

math::Matrix4D rendering::Camera::GetViewProjection() const
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

	return m_projection * m_rot.Conjugate().ToRotationMatrix() * math::Matrix4D(m_pos.Negated()); // FIXME: Check matrix multiplication
	//return m_projection * m_viewMatrix;
}

void rendering::Camera::Rotate(const math::Vector3D& axis, const math::Angle& angle)
{
	Rotate(math::Quaternion(axis, angle));
}

void rendering::Camera::Rotate(const math::Quaternion& rot)
{
	//DEBUG_LOG_MATH("Rotating the camera by the quaternion ", rot);
	m_rot = (rot * m_rot).Normalized(); // FIXME: Check quaternion multiplication
}