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

using namespace Rendering;
using namespace Utility;

CameraBase::CameraBase(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity) :
	m_projection(projectionMatrix),
	m_sensitivity(sensitivity),
	m_isActive(false)
{
}

CameraBase::CameraBase(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity) :
	m_projection(FoV, aspectRatio, zNearPlane, zFarPlane),
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


CameraBase::~CameraBase(void)
{
}

Math::Matrix4D CameraBase::GetViewMatrix() const
{
	// TODO: Check which one is the fastest: SOLUTION #1, SOLUTION #2, etc
	/* ==================== SOLUTION #1 begin ==================== */
	// return GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix() * GetTransform().GetTransformedPos().Negated();
	/* ==================== SOLUTION #1 end ==================== */

	Math::Matrix4D cameraTranslation(GetTransform().GetTransformedPos().Negate());
	//Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	/* ==================== SOLUTION #2 begin ==================== */
	Math::Matrix4D cameraRotation(GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix());
	/* ==================== SOLUTION #2 end ==================== */
	/* ==================== SOLUTION #3 begin ==================== */
	// Math::Matrix4D cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	/* ==================== SOLUTION #3 end ==================== */

	return cameraRotation * cameraTranslation; // FIXME: Check matrix multiplication
}

Math::Matrix4D CameraBase::GetViewProjection() const
{
	// This function is performed quiet often. Maybe we could, instead of multiplying three matrices (projection, rotation, translation),
	// just remember the result in some member variable and reuse it. Of course, we would have to perform the multiplication again if any of these matrices were changed.
	// It would probably lead to better performance.

	// TODO: Check which one is the fastest: SOLUTION #1, SOLUTION #2, etc
	/* ==================== SOLUTION #1 begin ==================== */
	// return m_projection * GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix() * GetTransform().GetTransformedPos().Negated();
	/* ==================== SOLUTION #1 end ==================== */

	Math::Matrix4D cameraTranslation(GetTransform().GetTransformedPos().Negate());
	//Matrix4D cameraRotation = GetTransform().GetRot().ToRotationMatrix();
	/* ==================== SOLUTION #2 begin ==================== */
	Math::Matrix4D cameraRotation(GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix());
	/* ==================== SOLUTION #2 end ==================== */
	/* ==================== SOLUTION #3 begin ==================== */
	// Math::Matrix4D cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	/* ==================== SOLUTION #3 end ==================== */

	return m_projection * cameraRotation * cameraTranslation; // FIXME: Check matrix multiplication
}


Camera::Camera(const Math::Matrix4D& projectionMatrix, const Math::Transform& transform, Math::Real sensitivity) :
	CameraBase(projectionMatrix, sensitivity),
	m_transform(transform)
{
}

Camera::Camera(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, const Math::Transform& transform, Math::Real sensitivity) :
	CameraBase(FoV, aspectRatio, zNearPlane, zFarPlane, sensitivity),
	m_transform(transform)
{
}


Camera::~Camera(void)
{
}

