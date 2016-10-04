#include "GameNodeBuilder.h"
#include "TextureIDs.h"

#include "Engine\CameraComponent.h"
#include "Engine\CameraBehavior.h"

#include "Utility\IConfig.h"

/* ==================== GameNodeBuilder implementation begin ==================== */
Game::GameNodeBuilder::GameNodeBuilder(Engine::GameManager* gameManager, Engine::GameNode* gameNode) :
	Utility::Builder<Engine::GameNode>(gameNode),
	m_gameManager(gameManager)
{
}

Game::GameNodeBuilder::~GameNodeBuilder()
{
}
/* ==================== GameNodeBuilder implementation end ==================== */

/* ==================== CameraNodeBuilder implementation begin ==================== */
Game::CameraNodeBuilder::CameraNodeBuilder(Engine::GameManager* gameManager, Engine::GameNode* cameraNode) :
	GameNodeBuilder(gameManager, cameraNode),
	M_DEFAULT_CAMERA_POS(GET_CONFIG_VALUE_GAME("defaultCameraPosX", 0.0f), GET_CONFIG_VALUE_GAME("defaultCameraPosY", 0.0f), GET_CONFIG_VALUE_GAME("defaultCameraPosZ", 0.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_X(GET_CONFIG_VALUE_GAME("defaultCameraAngleX", -45.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_Y(GET_CONFIG_VALUE_GAME("defaultCameraAngleY", 0.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_Z(GET_CONFIG_VALUE_GAME("defaultCameraAngleZ", 0.0f)),
	M_DEFAULT_CAMERA_FIELD_OF_VIEW(GET_CONFIG_VALUE_GAME("defaultCameraFoV", 70.0f)),
	M_DEFAULT_CAMERA_ASPECT_RATIO(GET_CONFIG_VALUE_GAME("defaultCameraAspectRatio", static_cast<Math::Real>(800) / 600)),
	M_DEFAULT_CAMERA_NEAR_PLANE(GET_CONFIG_VALUE_GAME("defaultCameraNearPlane", 0.1f)),
	M_DEFAULT_CAMERA_FAR_PLANE(GET_CONFIG_VALUE_GAME("defaultCameraFarPlane", 1000.0f)),
	M_DEFAULT_CAMERA_SENSITIVITY(GET_CONFIG_VALUE_GAME("defaultCameraSensitivity", 0.005f)),
	M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY(GET_CONFIG_VALUE_GAME("defaultCameraFollowEntityInitialDistance", 0.25f)),
	M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED(GET_CONFIG_VALUE_GAME("defaultCameraFollowAngleAroundEntitySpeed", 0.24f)),
	M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED(GET_CONFIG_VALUE_GAME("defaultCameraFollowPitchRotationSpeed", 0.1f)),
	M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE(GET_CONFIG_VALUE_GAME("defaultCameraFollowInitialPitchAngle", 30.0f)),
	m_camerasCount(GET_CONFIG_VALUE_GAME("camerasCount", 3)),
	m_positions(m_camerasCount),
	m_rotations(m_camerasCount),
	m_cameraBehaviors(m_camerasCount),
	m_fovs(m_camerasCount),
	m_aspectRatios(m_camerasCount),
	m_nearPlanes(m_camerasCount),
	m_farPlanes(m_camerasCount),
	m_sensitivities(m_camerasCount),
	m_gameNodeToFollow(NULL)
{
	CHECK_CONDITION_EXIT_ALWAYS_GAME(m_camerasCount >= 1, Utility::Logging::CRITICAL, "No cameras defined in the rendering engine.");
	DEBUG_LOG_GAME("Creating ", m_camerasCount, " camera(-s)");
}

Game::CameraNodeBuilder::~CameraNodeBuilder()
{
}

void Game::CameraNodeBuilder::BuildPart1()
{
	for (std::size_t i = 0; i < m_camerasCount; ++i)
	{
		const std::string indexStr = std::to_string(i + 1);
		// Setting position
		m_positions[i].Set(GET_CONFIG_VALUE_GAME("cameraPosX_" + indexStr, M_DEFAULT_CAMERA_POS.GetX()),
			GET_CONFIG_VALUE_GAME("cameraPosY_" + indexStr, M_DEFAULT_CAMERA_POS.GetY()),
			GET_CONFIG_VALUE_GAME("cameraPosZ_" + indexStr, M_DEFAULT_CAMERA_POS.GetZ()));

		// Setting rotation
		Math::Angle angleX(GET_CONFIG_VALUE_GAME("cameraAngleX_" + indexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_X.Get(Math::Unit::DEGREE)));
		Math::Angle angleY(GET_CONFIG_VALUE_GAME("cameraAngleY_" + indexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Y.Get(Math::Unit::DEGREE)));
		Math::Angle angleZ(GET_CONFIG_VALUE_GAME("cameraAngleZ_" + indexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Z.Get(Math::Unit::DEGREE)));
		DELOCUST_LOG_ENGINE("angleX=", angleX.ToString(), ", angleY=", angleY.ToString(), ", angleZ=", angleZ.ToString());
		m_rotations[i] = Math::Quaternion(Math::Matrix4D(angleX, angleY, angleZ));

		// Setting camera type
		Engine::CameraBehaviorTypes::CameraBehaviorType cameraBehaviorType = ConvertToCameraBehaviorType(GET_CONFIG_VALUE_STR_GAME("cameraType" + indexStr, "Static"));
		switch (cameraBehaviorType)
		{
		case Engine::CameraBehaviorTypes::STATIC:
			m_cameraBehaviors[i] = new Engine::StaticCameraBehavior();
			break;
		case Engine::CameraBehaviorTypes::ROTATION_ONLY:
			m_cameraBehaviors[i] = new Engine::RotationCameraBehavior();
			break;
		case Engine::CameraBehaviorTypes::MOVEMENT_ONLY:
			m_cameraBehaviors[i] = new Engine::MovementCameraBehavior();
			break;
		case Engine::CameraBehaviorTypes::FOLLOW_ENTITY:
			m_cameraBehaviors[i] = new Engine::StaticCameraBehavior(); // TODO: Create appropriate camera behavior for the behavior type
			break;
		case Engine::CameraBehaviorTypes::FOLLOW_ENTITY_WITH_ROTATION:
			m_cameraBehaviors[i] = new Engine::StaticCameraBehavior(); // TODO: Create appropriate camera behavior for the behavior type
			break;
		default:
			WARNING_LOG_GAME("Unknown camera behavior type ", cameraBehaviorType, ". Using STATIC as default.");
			m_cameraBehaviors[i] = new Engine::StaticCameraBehavior(); // TODO: Create appropriate camera behavior for the behavior type
			break;
		}

		// Setting camera parameters
		m_fovs[i].Set(GET_CONFIG_VALUE_GAME("cameraFoV_" + indexStr, M_DEFAULT_CAMERA_FIELD_OF_VIEW.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
		m_aspectRatios[i] = GET_CONFIG_VALUE_GAME("cameraAspectRatio_" + indexStr, M_DEFAULT_CAMERA_ASPECT_RATIO);
		m_nearPlanes[i] = GET_CONFIG_VALUE_GAME("cameraNearPlane_" + indexStr, M_DEFAULT_CAMERA_NEAR_PLANE);
		m_farPlanes[i] = GET_CONFIG_VALUE_GAME("cameraFarPlane_" + indexStr, M_DEFAULT_CAMERA_FAR_PLANE);
		m_sensitivities[i] = GET_CONFIG_VALUE_GAME("cameraSensitivity_" + indexStr, M_DEFAULT_CAMERA_SENSITIVITY);
	}

	//m_object->GetTransform().SetPos(m_camera->GetPos());
	//m_object->GetTransform().SetRot(m_camera->GetRot());
}

void Game::CameraNodeBuilder::BuildPart2()
{
	//Math::Real initialDistanceFromEntity = GET_CONFIG_VALUE_GAME("cameraFollowEntityInitialDistance_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowEntityInitialDistance", 0.25f));
	//Math::Real angleAroundEntitySpeed = GET_CONFIG_VALUE_GAME("cameraFollowAngleAroundEntitySpeed_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowAngleAroundEntitySpeed", 0.24f));
	//Math::Real pitchRotationSpeed = GET_CONFIG_VALUE_GAME("cameraFollowPitchRotationSpeed_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowPitchRotationSpeed", 0.1f));
	//Math::Angle initialPitchAngle(GET_CONFIG_VALUE_GAME("cameraFollowInitialPitchAngle_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowInitialPitchAngle", 30.0f)));

	//Engine::CameraComponent* cameraComponent = new Engine::CameraComponent(
	//for (std::size_t i = 0; i < m_camerasCount; ++i)
	//{
	//	camerasComponent->AddCamera(Rendering::Camera(m_positions[i], m_rotations[i], m_fovs[i], m_aspectRatios[i], m_nearPlanes[i], m_farPlanes[i], m_sensitivities[i]), m_cameraBehaviors[i]);
	//}
	//if (m_gameNodeToFollow != NULL)
	//{
	//	cameraComponent = new Engine::CameraFollowComponent(m_camera, m_gameNodeToFollow, initialDistanceFromEntity, angleAroundEntitySpeed, pitchRotationSpeed, initialPitchAngle);
	//}
	//else
	//{
	//	cameraComponent = new Engine::CameraMoveComponent(m_camera);
	//}
	//m_object->AddComponent(camerasComponent);
}

void Game::CameraNodeBuilder::BuildPart3()
{
}

#ifdef BUILD_MESH_RENDERER
void Game::CameraNodeBuilder::BuildMeshRenderer()
{
}
#endif

/* ==================== SkyboxBuilder implementation end ==================== */

/* ==================== SkyboxBuilder implementation begin ==================== */
Game::SkyboxBuilder::SkyboxBuilder(Engine::GameManager* gameManager, Engine::GameNode* skyboxNode) :
	GameNodeBuilder(gameManager, skyboxNode),
	m_gameManager(gameManager),
	m_scale(5.0f),
	m_meshRendererComponent(NULL)
{
}

Game::SkyboxBuilder::~SkyboxBuilder()
{
}

void Game::SkyboxBuilder::BuildPart1()
{
	m_scale = GET_CONFIG_VALUE_GAME("skyboxScale", 5.0f);

	std::string cubeMapDayDirectory = GET_CONFIG_VALUE_STR_GAME("skyboxDayDirectory", "SkyboxDebug");
	std::string cubeMapNightDirectory = GET_CONFIG_VALUE_STR_GAME("skyboxNightDirectory", "SkyboxDebug");
	const Rendering::Texture* skyboxTextureDay = m_gameManager->AddCubeTexture(TextureIDs::SKYBOX_DAY, cubeMapDayDirectory);
	CHECK_CONDITION_EXIT_GAME(skyboxTextureDay != NULL, Utility::Logging::ERR, "Skybox day texture \"", cubeMapTextureDirectory, "\" is NULL");
	const Rendering::Texture* skyboxTextureNight = m_gameManager->AddCubeTexture(TextureIDs::SKYBOX_NIGHT, cubeMapNightDirectory);
	CHECK_CONDITION_EXIT_GAME(skyboxTextureNight != NULL, Utility::Logging::ERR, "Skybox night texture \"", cubeMapNightDirectory, "\" is NULL");

	//SetTexture("cubeMapDay", m_skyboxTextureDay);
	//SetTexture("cubeMapNight", m_skyboxTextureNight);

	Rendering::Material* skyboxMaterial = new Rendering::Material(skyboxTextureDay, "cubeMapDay");
	skyboxMaterial->SetAdditionalTexture(skyboxTextureNight, "cubeMapNight");

	m_meshRendererComponent = new Engine::MeshRendererComponent(new Rendering::Mesh(GET_CONFIG_VALUE_STR_GAME("skyboxModel", "cube.obj")), skyboxMaterial);
}

void Game::SkyboxBuilder::BuildPart2()
{
	m_object->GetTransform().SetPos(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	m_object->GetTransform().SetScale(m_scale);

	m_object->AddComponent(m_meshRendererComponent);
	m_object->AddComponent(new Engine::ConstantRotationComponent(Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Math::Angle(GET_CONFIG_VALUE_GAME("skyboxRotationSpeed", 0.00005f))));
}

#ifdef BUILD_MESH_RENDERER
void Game::SkyboxBuilder::BuildMeshRenderer()
{
}
#endif

/* ==================== SkyboxBuilder implementation end ==================== */