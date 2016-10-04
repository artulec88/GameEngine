#include "CameraBuilder.h"

#include "Utility\IConfig.h"

/* ==================== CameraBuilder implementation begin ==================== */
Game::CameraBuilder::CameraBuilder(Engine::GameManager* gameManager, Rendering::BaseCamera* camera) :
	Utility::Builder<Rendering::BaseCamera>(camera),
	m_gameManager(gameManager),
	M_DEFAULT_CAMERA_POS(GET_CONFIG_VALUE_GAME("defaultCameraPosX", 0.0f), GET_CONFIG_VALUE_GAME("defaultCameraPosY", 0.0f), GET_CONFIG_VALUE_GAME("defaultCameraPosZ", 0.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_X(GET_CONFIG_VALUE_GAME("defaultCameraAngleX", -45.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_Y(GET_CONFIG_VALUE_GAME("defaultCameraAngleY", 0.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_Z(GET_CONFIG_VALUE_GAME("defaultCameraAngleZ", 0.0f)),
	M_DEFAULT_CAMERA_FIELD_OF_VIEW(GET_CONFIG_VALUE_GAME("defaultCameraFoV", 70.0f)),
	M_DEFAULT_CAMERA_ASPECT_RATIO(GET_CONFIG_VALUE_GAME("defaultCameraAspectRatio", static_cast<Math::Real>(800) / 600)),
	M_DEFAULT_CAMERA_NEAR_PLANE(GET_CONFIG_VALUE_GAME("defaultCameraNearPlane", 0.1f)),
	M_DEFAULT_CAMERA_FAR_PLANE(GET_CONFIG_VALUE_GAME("defaultCameraFarPlane", 1000.0f)),
	M_DEFAULT_CAMERA_SENSITIVITY(GET_CONFIG_VALUE_GAME("defaultCameraSensitivity", 0.005f)),
	//M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY(GET_CONFIG_VALUE_GAME("defaultCameraFollowEntityInitialDistance", 0.25f)),
	//M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED(GET_CONFIG_VALUE_GAME("defaultCameraFollowAngleAroundEntitySpeed", 0.24f)),
	//M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED(GET_CONFIG_VALUE_GAME("defaultCameraFollowPitchRotationSpeed", 0.1f)),
	//M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE(GET_CONFIG_VALUE_GAME("defaultCameraFollowInitialPitchAngle", 30.0f)),
	m_cameraIndex(0),
	m_cameraIndexStr("0"),
	m_pos(REAL_ZERO, REAL_ZERO, REAL_ZERO),
	m_rot(),
	m_fov(M_DEFAULT_CAMERA_FIELD_OF_VIEW),
	m_aspectRatio(M_DEFAULT_CAMERA_ASPECT_RATIO),
	m_nearPlane(M_DEFAULT_CAMERA_NEAR_PLANE),
	m_farPlane(M_DEFAULT_CAMERA_FAR_PLANE),
	m_sensitivity(M_DEFAULT_CAMERA_SENSITIVITY)
{
}

Game::CameraBuilder::~CameraBuilder()
{
}

void Game::CameraBuilder::SetCameraIndex(int cameraIndex)
{
	m_cameraIndex = cameraIndex;
	std::stringstream ss("");
	ss << (m_cameraIndex + 1);
	m_cameraIndexStr = ss.str();
}

//void Game::CameraBuilder::SetEntityToFollow(Engine::GameNode* gameNodeToFollow)
//{
//	m_gameNodeToFollow = gameNodeToFollow;
//}

void Game::CameraBuilder::BuildPart1()
{
	// Setting position
	m_pos.Set(GET_CONFIG_VALUE_GAME("cameraPosX_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetX()),
		GET_CONFIG_VALUE_GAME("cameraPosY_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetY()),
		GET_CONFIG_VALUE_GAME("cameraPosZ_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetZ()));

	// Setting rotation
	Math::Angle angleX(GET_CONFIG_VALUE_GAME("cameraAngleX_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_X.Get(Math::Unit::DEGREE)));
	Math::Angle angleY(GET_CONFIG_VALUE_GAME("cameraAngleY_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Y.Get(Math::Unit::DEGREE)));
	Math::Angle angleZ(GET_CONFIG_VALUE_GAME("cameraAngleZ_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Z.Get(Math::Unit::DEGREE)));
	DELOCUST_LOG_ENGINE("angleX=", angleX.ToString(), ", angleY=", angleY.ToString(), ", angleZ=", angleZ.ToString());
	m_rot = Math::Quaternion(Math::Matrix4D(angleX, angleY, angleZ));
}

void Game::CameraBuilder::BuildPart2()
{
	// Setting camera parameters
	m_fov.Set(GET_CONFIG_VALUE_GAME("cameraFoV_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FIELD_OF_VIEW.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
	m_aspectRatio = GET_CONFIG_VALUE_GAME("cameraAspectRatio_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ASPECT_RATIO);
	m_nearPlane = GET_CONFIG_VALUE_GAME("cameraNearPlane_" + m_cameraIndexStr, M_DEFAULT_CAMERA_NEAR_PLANE);
	m_farPlane = GET_CONFIG_VALUE_GAME("cameraFarPlane_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FAR_PLANE);
	m_sensitivity = GET_CONFIG_VALUE_GAME("cameraSensitivity_" + m_cameraIndexStr, M_DEFAULT_CAMERA_SENSITIVITY);

	//Math::Real initialDistanceFromEntity = GET_CONFIG_VALUE_GAME("cameraFollowEntityInitialDistance_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY);
	//Math::Real angleAroundEntitySpeed = GET_CONFIG_VALUE_GAME("cameraFollowAngleAroundEntitySpeed_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED);
	//Math::Real pitchRotationSpeed = GET_CONFIG_VALUE_GAME("cameraFollowPitchRotationSpeed_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED);
	//Math::Angle initialPitchAngle(GET_CONFIG_VALUE_GAME("cameraFollowInitialPitchAngle_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE.Get(Math::Unit::DEGREE)));

	//Engine::CameraComponent* cameraComponent = new Engine::CameraFollowComponent(camera, m_gameNodeToFollow, initialDistanceFromEntity, angleAroundEntitySpeed, pitchRotationSpeed, initialPitchAngle);
	////Engine::CameraComponent* cameraComponent = new Engine::CameraMoveComponent(camera);
	//m_object->AddComponent(cameraComponent);
}

void Game::CameraBuilder::BuildPart3()
{
	m_object->SetPos(m_pos);
	m_object->SetRot(m_rot);
	m_object->SetSensitivity(m_sensitivity);
	m_object->SetProjection(Math::Matrix4D(m_fov, m_aspectRatio, m_nearPlane, m_farPlane));
}

#ifdef BUILD_MESH_RENDERER
void Game::CameraBuilder::BuildMeshRenderer()
{
	// Rendering a small box around camera node position to let the user see the camera
	//m_gameNode->AddComponent(new MeshRenderer(
	//	new Mesh("Camera.obj"),
	//	new Material(m_gameManager->GetTextureFactory().GetTexture(TextureIDs::CAMERA), 1.0f, 8.0f)));
	//m_gameNode->GetTransform().SetScale(0.1f); /* TODO: Don't use hard-coded values! Ever! */
}
#endif

/* ==================== CameraBuilder implementation end ==================== */