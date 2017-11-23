#include "StdAfx.h"
#include "Camera.h"

#include "Math/Quaternion.h"
#ifdef ANT_TWEAK_BAR_ENABLED
#include "Math/FloatingPoint.h"
#endif

#include "Utility/ILogger.h"
#include "Utility/IConfig.h"

rendering::Camera::Camera() :
	BaseCamera(math::Matrix4D::IDENTITY_MATRIX, 0.05f /* TODO: Don't ever use hard-coded values */),
	m_transform(REAL_ZERO, REAL_ZERO, REAL_ZERO, math::NO_ROTATION_QUATERNION, REAL_ONE)
{
}

rendering::Camera::Camera(const math::Vector3D& position, const math::Quaternion& rotation, const math::Matrix4D& projectionMatrix, math::Real sensitivity) :
	BaseCamera(projectionMatrix, sensitivity),
	m_transform(position, rotation, REAL_ONE)
{
}

rendering::Camera::Camera(const math::Vector3D& position, const math::Quaternion& rotation, const math::Angle& FoV, math::Real aspectRatio, math::Real zNearPlane, math::Real zFarPlane, math::Real sensitivity) :
	BaseCamera(FoV, aspectRatio, zNearPlane, zFarPlane, sensitivity),
	m_transform(position, rotation, REAL_ONE)
{
}

rendering::Camera::~Camera()
{
}
