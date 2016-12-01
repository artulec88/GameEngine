#include "StdAfx.h"
#include "Camera.h"

#include "Math\Quaternion.h"
#ifdef ANT_TWEAK_BAR_ENABLED
#include "Math\FloatingPoint.h"
#endif

#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

#include <sstream>

Rendering::Camera::Camera() :
	BaseCamera(Math::Matrix4D::IDENTITY_MATRIX, 0.05f /* TODO: Don't ever use hard-coded values */),
	m_transform(REAL_ZERO, REAL_ZERO, REAL_ZERO, Math::NO_ROTATION_QUATERNION, REAL_ONE)
{
}

Rendering::Camera::Camera(const Math::Vector3D& position, const Math::Quaternion& rotation, const Math::Matrix4D& projectionMatrix, Math::Real sensitivity) :
	BaseCamera(projectionMatrix, sensitivity),
	m_transform(position, rotation, REAL_ONE)
{
}

Rendering::Camera::Camera(const Math::Vector3D& position, const Math::Quaternion& rotation, const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity) :
	BaseCamera(FoV, aspectRatio, zNearPlane, zFarPlane, sensitivity),
	m_transform(position, rotation, REAL_ONE)
{
}

Rendering::Camera::~Camera()
{
}
