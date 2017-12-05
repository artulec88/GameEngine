#include "GameNodeBuilder.h"
#include "TextureIDs.h"
#include "MeshIDs.h"

#include "Engine/CameraComponent.h"
#include "Engine/CameraBehavior.h"
#include "Engine/MeshRendererComponent.h"

#include "Utility/IConfig.h"

/* ==================== GameNodeBuilder implementation begin ==================== */
game::GameNodeBuilder::GameNodeBuilder(engine::GameManager* gameManager) :
	Builder<engine::GameNode>(),
	m_gameManager(gameManager),
	m_gameNode()
{
}

game::GameNodeBuilder::~GameNodeBuilder()
{
}
/* ==================== GameNodeBuilder implementation end ==================== */

/* ==================== CameraNodeBuilder implementation begin ==================== */
game::CameraNodeBuilder::CameraNodeBuilder(engine::GameManager* gameManager, rendering::Camera* camera) :
	GameNodeBuilder(gameManager),
	m_defaultCameraBehaviorType(static_cast<engine::camera_behavior_types::CameraBehaviorType>(
		GET_CONFIG_VALUE_GAME("defaultCameraBehaviorType", static_cast<int>(engine::camera_behavior_types::STATIC)))),
	m_defaultCameraFollowInitialDistanceFromEntity(GET_CONFIG_VALUE_GAME("defaultCameraFollowEntityInitialDistance", 0.25f)),
	m_defaultCameraFollowAngleAroundEntitySpeed(GET_CONFIG_VALUE_GAME("defaultCameraFollowAngleAroundEntitySpeed", 0.24f)),
	m_defaultCameraFollowPitchRotationSpeed(GET_CONFIG_VALUE_GAME("defaultCameraFollowPitchRotationSpeed", 0.1f)),
	m_defaultCameraFollowInitialPitchAngle(GET_CONFIG_VALUE_GAME("defaultCameraFollowInitialPitchAngle", 30.0f)),
	m_camera(camera),
	m_cameraBehaviorType(m_defaultCameraBehaviorType),
	m_gameNodeToFollow(nullptr)
{
}

game::CameraNodeBuilder::~CameraNodeBuilder()
{
}

void game::CameraNodeBuilder::BuildTransform()
{
	//for (std::size_t i = 0; i < m_camerasCount; ++i)
	//{
	//	const std::string indexStr = std::to_string(i + 1);
	//	// Setting position
	//	m_positions[i].Set(GET_CONFIG_VALUE_GAME("cameraPosX_" + indexStr, M_DEFAULT_CAMERA_POS.GetX()),
	//		GET_CONFIG_VALUE_GAME("cameraPosY_" + indexStr, M_DEFAULT_CAMERA_POS.GetY()),
	//		GET_CONFIG_VALUE_GAME("cameraPosZ_" + indexStr, M_DEFAULT_CAMERA_POS.GetZ()));

	//	// Setting rotation
	//	math::Angle angleX(GET_CONFIG_VALUE_GAME("cameraAngleX_" + indexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_X.Get(math::units::DEGREE)));
	//	math::Angle angleY(GET_CONFIG_VALUE_GAME("cameraAngleY_" + indexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Y.Get(math::units::DEGREE)));
	//	math::Angle angleZ(GET_CONFIG_VALUE_GAME("cameraAngleZ_" + indexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Z.Get(math::units::DEGREE)));
	//	DELOCUST_LOG_ENGINE("angleX=", angleX, ", angleY=", angleY, ", angleZ=", angleZ);
	//	m_rotations[i] = math::Quaternion(math::Matrix4D(angleX, angleY, angleZ));

	//	// Setting camera type
	//	Engine::camera_behavior_types::CameraBehaviorType cameraBehaviorType = ConvertToCameraBehaviorType(GET_CONFIG_VALUE_STR_GAME("cameraType" + indexStr, "Static"));
	//	switch (cameraBehaviorType)
	//	{
	//	case engine::camera_behavior_types::STATIC:
	//		m_cameraBehaviors[i] = new Engine::StaticCameraBehavior();
	//		break;
	//	case engine::camera_behavior_types::ROTATION_ONLY:
	//		m_cameraBehaviors[i] = new Engine::RotationCameraBehavior();
	//		break;
	//	case engine::camera_behavior_types::MOVEMENT_ONLY:
	//		m_cameraBehaviors[i] = new Engine::MovementCameraBehavior();
	//		break;
	//	case engine::camera_behavior_types::FOLLOW_ENTITY:
	//		m_cameraBehaviors[i] = new Engine::StaticCameraBehavior(); // TODO: Create appropriate camera behavior for the behavior type
	//		break;
	//	case engine::camera_behavior_types::FOLLOW_ENTITY_WITH_ROTATION:
	//		m_cameraBehaviors[i] = new Engine::StaticCameraBehavior(); // TODO: Create appropriate camera behavior for the behavior type
	//		break;
	//	default:
	//		WARNING_LOG_GAME("Unknown camera behavior type ", cameraBehaviorType, ". Using STATIC as default.");
	//		m_cameraBehaviors[i] = new Engine::StaticCameraBehavior(); // TODO: Create appropriate camera behavior for the behavior type
	//		break;
	//	}

	//	// Setting camera parameters
	//	m_fovs[i].Set(GET_CONFIG_VALUE_GAME("cameraFoV_" + indexStr, M_DEFAULT_CAMERA_FIELD_OF_VIEW.Get(math::units::DEGREE)), math::units::DEGREE);
	//	m_aspectRatios[i] = GET_CONFIG_VALUE_GAME("cameraAspectRatio_" + indexStr, M_DEFAULT_CAMERA_ASPECT_RATIO);
	//	m_nearPlanes[i] = GET_CONFIG_VALUE_GAME("cameraNearPlane_" + indexStr, M_DEFAULT_CAMERA_NEAR_PLANE);
	//	m_farPlanes[i] = GET_CONFIG_VALUE_GAME("cameraFarPlane_" + indexStr, M_DEFAULT_CAMERA_FAR_PLANE);
	//	m_sensitivities[i] = GET_CONFIG_VALUE_GAME("cameraSensitivity_" + indexStr, M_DEFAULT_CAMERA_SENSITIVITY);
	//}

	////m_object->GetTransform().SetPos(m_camera->GetPos());
	////m_object->GetTransform().SetRot(m_camera->GetRot());
}

void game::CameraNodeBuilder::BuildComponents()
{
	//math::Real initialDistanceFromEntity = GET_CONFIG_VALUE_GAME("cameraFollowEntityInitialDistance_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowEntityInitialDistance", 0.25f));
	//math::Real angleAroundEntitySpeed = GET_CONFIG_VALUE_GAME("cameraFollowAngleAroundEntitySpeed_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowAngleAroundEntitySpeed", 0.24f));
	//math::Real pitchRotationSpeed = GET_CONFIG_VALUE_GAME("cameraFollowPitchRotationSpeed_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowPitchRotationSpeed", 0.1f));
	//math::Angle initialPitchAngle(GET_CONFIG_VALUE_GAME("cameraFollowInitialPitchAngle_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowInitialPitchAngle", 30.0f)));

	//Engine::CameraComponent* cameraComponent = new Engine::CameraComponent(
	//for (std::size_t i = 0; i < m_camerasCount; ++i)
	//{
	//	camerasComponent->AddCamera(Rendering::Camera(m_positions[i], m_rotations[i], m_fovs[i], m_aspectRatios[i], m_nearPlanes[i], m_farPlanes[i], m_sensitivities[i]), m_cameraBehaviors[i]);
	//}
	//if (m_gameNodeToFollow != nullptr)
	//{
	//	cameraComponent = new Engine::CameraFollowComponent(m_camera, m_gameNodeToFollow, initialDistanceFromEntity, angleAroundEntitySpeed, pitchRotationSpeed, initialPitchAngle);
	//}
	//else
	//{
	//	cameraComponent = new Engine::CameraMoveComponent(m_camera);
	//}
	//m_object->AddComponent(camerasComponent);
}
/* ==================== CameraNodeBuilder implementation end ==================== */

/* ==================== SkyboxBuilder implementation begin ==================== */
game::SkyboxBuilder::SkyboxBuilder(engine::GameManager* gameManager) :
	GameNodeBuilder(gameManager),
	m_gameManager(gameManager),
	m_scale(GET_CONFIG_VALUE_GAME("skyboxScale", 5.0f))
{
}

game::SkyboxBuilder::~SkyboxBuilder()
{
}

void game::SkyboxBuilder::BuildTransform()
{
	m_gameNode.GetTransform().SetPos(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	m_gameNode.GetTransform().SetScale(m_scale);
}

void game::SkyboxBuilder::BuildComponents()
{
	const auto cubeMapDayDirectory = GET_CONFIG_VALUE_STR_GAME("skyboxDayDirectory", "SkyboxDebug");
	const auto cubeMapNightDirectory = GET_CONFIG_VALUE_STR_GAME("skyboxNightDirectory", "SkyboxDebug");
	const auto skyboxTextureDay = m_gameManager->AddCubeTexture(texture_ids::SKYBOX_DAY, cubeMapDayDirectory);
	CHECK_CONDITION_EXIT_GAME(skyboxTextureDay != nullptr, Utility::Logging::ERR, "Skybox day texture \"", cubeMapTextureDirectory, "\" is NULL");
	const auto skyboxTextureNight = m_gameManager->AddCubeTexture(texture_ids::SKYBOX_NIGHT, cubeMapNightDirectory);
	CHECK_CONDITION_EXIT_GAME(skyboxTextureNight != nullptr, Utility::Logging::ERR, "Skybox night texture \"", cubeMapNightDirectory, "\" is NULL");

	//SetTexture("cubeMapDay", m_skyboxTextureDay);
	//SetTexture("cubeMapNight", m_skyboxTextureNight);

	const auto skyboxMaterial = new rendering::Material(skyboxTextureDay, "cubeMapDay");
	skyboxMaterial->SetAdditionalTexture(skyboxTextureNight, "cubeMapNight");

	m_gameManager->AddMesh(mesh_ids::SKYBOX, GET_CONFIG_VALUE_STR_GAME("skyboxModel", "cube.obj"));
	m_gameNode.AddComponent(new engine::MeshRendererComponent(mesh_ids::SKYBOX, skyboxMaterial));

	m_gameNode.AddComponent(new engine::ConstantRotationComponent(math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), math::Angle(GET_CONFIG_VALUE_GAME("skyboxRotationSpeed", 0.00005f))));
}
/* ==================== SkyboxBuilder implementation end ==================== */