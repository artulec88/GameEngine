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

Rendering::Camera::Camera() :
	BaseCamera(Math::Matrix4D::IDENTITY_MATRIX),
	m_pos(REAL_ZERO, REAL_ZERO, REAL_ZERO),
	m_rot(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE)
{
}

Rendering::Camera::Camera(const Math::Vector3D& position, const Math::Quaternion& rotation, const Math::Matrix4D& projectionMatrix, Math::Real sensitivity) :
	BaseCamera(projectionMatrix, sensitivity),
	m_pos(position),
	m_rot(rotation)
{
}

Rendering::Camera::Camera(const Math::Vector3D& position, const Math::Quaternion& rotation, const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity) :
	BaseCamera(FoV, aspectRatio, zNearPlane, zFarPlane, sensitivity),
	m_pos(position),
	m_rot(rotation)
{
}

Rendering::Camera::~Camera()
{
}
