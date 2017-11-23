#include "stdafx.h"
#include "CameraBuilder.h"

#include "Utility/IConfig.h"

/* ==================== CameraBuilder implementation begin ==================== */
rendering::CameraBuilder::CameraBuilder() :
	utility::Builder<Camera>(),
	M_DEFAULT_CAMERA_POS_X(GET_CONFIG_VALUE_RENDERING("defaultCameraPosX", 0.0f)),
	M_DEFAULT_CAMERA_POS_Y(GET_CONFIG_VALUE_RENDERING("defaultCameraPosY", 0.0f)),
	M_DEFAULT_CAMERA_POS_Z(GET_CONFIG_VALUE_RENDERING("defaultCameraPosZ", 0.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_X(GET_CONFIG_VALUE_RENDERING("defaultCameraAngleX", -45.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_Y(GET_CONFIG_VALUE_RENDERING("defaultCameraAngleY", 0.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_Z(GET_CONFIG_VALUE_RENDERING("defaultCameraAngleZ", 0.0f)),
	M_DEFAULT_CAMERA_NEAR_PLANE(GET_CONFIG_VALUE_RENDERING("defaultCameraNearPlane", 0.1f)),
	M_DEFAULT_CAMERA_FAR_PLANE(GET_CONFIG_VALUE_RENDERING("defaultCameraFarPlane", 1000.0f)),
	M_DEFAULT_CAMERA_SENSITIVITY(GET_CONFIG_VALUE_RENDERING("defaultCameraSensitivity", 0.005f)),
	m_pos(M_DEFAULT_CAMERA_POS_X, M_DEFAULT_CAMERA_POS_Y, M_DEFAULT_CAMERA_POS_Z),
	m_rot(math::Matrix4D(M_DEFAULT_CAMERA_ROTATION_ANGLE_X, M_DEFAULT_CAMERA_ROTATION_ANGLE_Y, M_DEFAULT_CAMERA_ROTATION_ANGLE_Z)),
	m_nearPlane(M_DEFAULT_CAMERA_NEAR_PLANE),
	m_farPlane(M_DEFAULT_CAMERA_FAR_PLANE),
	m_projectionMatrix(),
	m_sensitivity(M_DEFAULT_CAMERA_SENSITIVITY)
{
}

rendering::CameraBuilder::~CameraBuilder()
{
}

//void Rendering::CameraBuilder::BuildPart1()
//{
//	// Setting position
//	m_pos.Set(GET_CONFIG_VALUE_RENDERING("cameraPosX_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetX()),
//		GET_CONFIG_VALUE_RENDERING("cameraPosY_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetY()),
//		GET_CONFIG_VALUE_RENDERING("cameraPosZ_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetZ()));
//
//	// Setting rotation
//	math::Angle angleX(GET_CONFIG_VALUE_RENDERING("cameraAngleX_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_X.Get(math::units::DEGREE)));
//	math::Angle angleY(GET_CONFIG_VALUE_RENDERING("cameraAngleY_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Y.Get(math::units::DEGREE)));
//	math::Angle angleZ(GET_CONFIG_VALUE_RENDERING("cameraAngleZ_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Z.Get(math::units::DEGREE)));
//	DELOCUST_LOG_ENGINE("angleX=", angleX, ", angleY=", angleY, ", angleZ=", angleZ);
//	m_rot = math::Quaternion(math::Matrix4D(angleX, angleY, angleZ));
//}

//void Rendering::CameraBuilder::BuildPart2()
//{
//	// Setting camera parameters
//	m_fov.Set(GET_CONFIG_VALUE_RENDERING("cameraFoV_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FIELD_OF_VIEW.Get(math::units::DEGREE)), math::units::DEGREE);
//	m_aspectRatio = GET_CONFIG_VALUE_RENDERING("cameraAspectRatio_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ASPECT_RATIO);
//	m_nearPlane = GET_CONFIG_VALUE_RENDERING("cameraNearPlane_" + m_cameraIndexStr, M_DEFAULT_CAMERA_NEAR_PLANE);
//	m_farPlane = GET_CONFIG_VALUE_RENDERING("cameraFarPlane_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FAR_PLANE);
//	m_sensitivity = GET_CONFIG_VALUE_RENDERING("cameraSensitivity_" + m_cameraIndexStr, M_DEFAULT_CAMERA_SENSITIVITY);
//
//	//math::Real initialDistanceFromEntity = GET_CONFIG_VALUE_RENDERING("cameraFollowEntityInitialDistance_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY);
//	//math::Real angleAroundEntitySpeed = GET_CONFIG_VALUE_RENDERING("cameraFollowAngleAroundEntitySpeed_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED);
//	//math::Real pitchRotationSpeed = GET_CONFIG_VALUE_RENDERING("cameraFollowPitchRotationSpeed_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED);
//	//math::Angle initialPitchAngle(GET_CONFIG_VALUE_RENDERING("cameraFollowInitialPitchAngle_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE.Get(math::units::DEGREE)));
//
//	//Engine::CameraComponent* cameraComponent = new Engine::CameraFollowComponent(camera, m_gameNodeToFollow, initialDistanceFromEntity, angleAroundEntitySpeed, pitchRotationSpeed, initialPitchAngle);
//	////Engine::CameraComponent* cameraComponent = new Engine::CameraMoveComponent(camera);
//	//m_object->AddComponent(cameraComponent);
//}

//void Rendering::CameraBuilder::BuildPart3()
//{
//	m_object->GetTransform().SetPos(m_pos);
//	m_object->GetTransform().SetRot(m_rot);
//	m_object->SetSensitivity(m_sensitivity);
//	m_object->SetProjection(math::Matrix4D(m_fov, m_aspectRatio, m_nearPlane, m_farPlane));
//}
/* ==================== CameraBuilder implementation end ==================== */

/* ==================== OrthoCameraBuilder implementation begin ==================== */
rendering::OrthoCameraBuilder::OrthoCameraBuilder() :
	CameraBuilder(),
	M_DEFAULT_CAMERA_LEFT_PLANE(GET_CONFIG_VALUE_RENDERING("defaultCameraLeftPlane", -100.0f)),
	M_DEFAULT_CAMERA_RIGHT_PLANE(GET_CONFIG_VALUE_RENDERING("defaultCameraRightPlane", 100.0f)),
	M_DEFAULT_CAMERA_BOTTOM_PLANE(GET_CONFIG_VALUE_RENDERING("defaultCameraBottomPlane", -100.0f)),
	M_DEFAULT_CAMERA_TOP_PLANE(GET_CONFIG_VALUE_RENDERING("defaultCameraTopPlane", 100.0f)),
	m_left(M_DEFAULT_CAMERA_LEFT_PLANE),
	m_right(M_DEFAULT_CAMERA_RIGHT_PLANE),
	m_bottom(M_DEFAULT_CAMERA_BOTTOM_PLANE),
	m_top(M_DEFAULT_CAMERA_TOP_PLANE)
{
}

rendering::OrthoCameraBuilder::~OrthoCameraBuilder(void)
{
}

void rendering::OrthoCameraBuilder::BuildProjectionMatrix()
{
	m_projectionMatrix.SetOrthogonalProjection(m_left, m_right, m_bottom, m_top, m_nearPlane, m_farPlane);
}
/* ==================== OrthoCameraBuilder implementation end ==================== */

/* ==================== PerspectiveCameraBuilder implementation begin ==================== */
rendering::PerspectiveCameraBuilder::PerspectiveCameraBuilder() :
	CameraBuilder(),
	M_DEFAULT_CAMERA_FIELD_OF_VIEW(GET_CONFIG_VALUE_RENDERING("defaultCameraFoV", 70.0f)),
	M_DEFAULT_CAMERA_ASPECT_RATIO(GET_CONFIG_VALUE_RENDERING("defaultCameraAspectRatio", static_cast<math::Real>(800) / 600)),
	m_fov(M_DEFAULT_CAMERA_FIELD_OF_VIEW),
	m_aspectRatio(M_DEFAULT_CAMERA_ASPECT_RATIO)
{
}

rendering::PerspectiveCameraBuilder::~PerspectiveCameraBuilder(void)
{
}

void rendering::PerspectiveCameraBuilder::BuildProjectionMatrix()
{
	m_projectionMatrix.SetPerspectiveProjection(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}
/* ==================== PerspectiveCameraBuilder implementation end ==================== */