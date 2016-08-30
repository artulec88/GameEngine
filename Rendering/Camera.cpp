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

Rendering::Camera::Camera(const Camera& camera) :
	m_pos(camera.m_pos),
	m_rot(camera.m_rot),
	m_projection(camera.m_projection),
	//m_viewMatrix(m_transform.GetTransformedRot().Conjugate().ToRotationMatrix() * Math::Matrix4D(m_transform.GetTransformedPos().Negate())),
	m_sensitivity(camera.m_sensitivity),
	m_isActive(camera.m_isActive)
#ifdef ANT_TWEAK_BAR_ENABLED
	, m_prevFov(camera.m_prevFov),
	m_fov(camera.m_fov),
	m_prevAspectRatio(camera.m_prevAspectRatio),
	m_aspectRatio(camera.m_aspectRatio),
	m_prevNearPlane(camera.m_prevNearPlane),
	m_nearPlane(camera.m_nearPlane),
	m_prevFarPlane(camera.m_prevFarPlane),
	m_farPlane(camera.m_farPlane)
#endif
{
}

Rendering::Camera::Camera(Camera&& camera) :
	m_pos(std::move(camera.m_pos)),
	m_rot(std::move(camera.m_rot)),
	m_projection(std::move(camera.m_projection)),
	//m_viewMatrix(m_transform.GetTransformedRot().Conjugate().ToRotationMatrix() * Math::Matrix4D(m_transform.GetTransformedPos().Negate())),
	m_sensitivity(std::move(camera.m_sensitivity)),
	m_isActive(std::move(camera.m_isActive))
#ifdef ANT_TWEAK_BAR_ENABLED
	, m_prevFov(std::move(camera.m_prevFov)),
	m_fov(std::move(camera.m_fov)),
	m_prevAspectRatio(std::move(camera.m_prevAspectRatio)),
	m_aspectRatio(std::move(camera.m_aspectRatio)),
	m_prevNearPlane(std::move(camera.m_prevNearPlane)),
	m_nearPlane(std::move(camera.m_nearPlane)),
	m_prevFarPlane(std::move(camera.m_prevFarPlane)),
	m_farPlane(std::move(camera.m_farPlane))
#endif
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