#include "GameNodeBuilder.h"
#include "TextureIDs.h"

#include "Engine\CoreEngine.h"
#include "Engine\CameraComponent.h"
#include "Engine\MeshRendererComponent.h"
#include "Engine\ParticlesSystemComponent.h"
#include "Engine\ConstantRotationComponent.h"

#include "Utility\IConfig.h"

/* ==================== CameraBuilder implementation begin ==================== */
Game::CameraBuilder::CameraBuilder(Engine::GameManager* gameManager) :
	Utility::Builder<Engine::GameNode>(),
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
	M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY(GET_CONFIG_VALUE_GAME("defaultCameraFollowEntityInitialDistance", 0.25f)),
	M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED(GET_CONFIG_VALUE_GAME("defaultCameraFollowAngleAroundEntitySpeed", 0.24f)),
	M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED(GET_CONFIG_VALUE_GAME("defaultCameraFollowPitchRotationSpeed", 0.1f)),
	M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE(GET_CONFIG_VALUE_GAME("defaultCameraFollowInitialPitchAngle", 30.0f)),
	m_cameraIndex(0),
	m_cameraIndexStr("0")
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

void Game::CameraBuilder::SetEntityToFollow(Engine::GameNode* gameNodeToFollow)
{
	m_gameNodeToFollow = gameNodeToFollow;
}

void Game::CameraBuilder::BuildPart1()
{
	SetupCameraTransform();
}

void Game::CameraBuilder::BuildPart2()
{
	SetupCameraParams();
}

void Game::CameraBuilder::SetupCameraTransform()
{
	//m_object = std::make_unique<GameNode>();
	m_object = new Engine::GameNode();

	// Setting position
	Math::Real xPos = GET_CONFIG_VALUE_GAME("cameraPosX_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetX());
	Math::Real yPos = GET_CONFIG_VALUE_GAME("cameraPosY_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetY());
	Math::Real zPos = GET_CONFIG_VALUE_GAME("cameraPosZ_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetZ());
	m_object->GetTransform().SetPos(xPos, yPos, zPos);

	// Setting rotation
	Math::Angle angleX(GET_CONFIG_VALUE_GAME("cameraAngleX_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_X.Get(Math::Unit::DEGREE)));
	Math::Angle angleY(GET_CONFIG_VALUE_GAME("cameraAngleY_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Y.Get(Math::Unit::DEGREE)));
	Math::Angle angleZ(GET_CONFIG_VALUE_GAME("cameraAngleZ_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Z.Get(Math::Unit::DEGREE)));
	Math::Matrix4D rotMatrix(angleX, angleY, angleZ);
	DELOCUST_LOG_ENGINE("angleX=", angleX.ToString(), ", angleY=", angleY.ToString(), ", angleZ=", angleZ.ToString());
	Math::Quaternion rot(rotMatrix);
	m_object->GetTransform().SetRot(rot);
}

void Game::CameraBuilder::SetupCameraParams()
{
	// Setting camera parameters
	Math::Angle fov(GET_CONFIG_VALUE_GAME("cameraFoV_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FIELD_OF_VIEW.Get(Math::Unit::DEGREE)));
	Math::Real aspectRatio = GET_CONFIG_VALUE_GAME("cameraAspectRatio_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ASPECT_RATIO);
	Math::Real zNearPlane = GET_CONFIG_VALUE_GAME("cameraNearPlane_" + m_cameraIndexStr, M_DEFAULT_CAMERA_NEAR_PLANE);
	Math::Real zFarPlane = GET_CONFIG_VALUE_GAME("cameraFarPlane_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FAR_PLANE);
	Math::Real sensitivity = GET_CONFIG_VALUE_GAME("cameraSensitivity_" + m_cameraIndexStr, M_DEFAULT_CAMERA_SENSITIVITY);

	Math::Real initialDistanceFromEntity = GET_CONFIG_VALUE_GAME("cameraFollowEntityInitialDistance_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY);
	Math::Real angleAroundEntitySpeed = GET_CONFIG_VALUE_GAME("cameraFollowAngleAroundEntitySpeed_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED);
	Math::Real pitchRotationSpeed = GET_CONFIG_VALUE_GAME("cameraFollowPitchRotationSpeed_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED);
	Math::Angle initialPitchAngle(GET_CONFIG_VALUE_GAME("cameraFollowInitialPitchAngle_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE.Get(Math::Unit::DEGREE)));

	Rendering::Camera* camera = new Rendering::Camera(m_object->GetTransform().GetTransformedPos(), m_object->GetTransform().GetTransformedRot(), fov, aspectRatio, zNearPlane, zFarPlane, sensitivity);
	Engine::CameraComponent* cameraComponent = new Engine::CameraFollowComponent(camera, m_gameNodeToFollow, initialDistanceFromEntity, angleAroundEntitySpeed, pitchRotationSpeed, initialPitchAngle);
	//Engine::CameraComponent* cameraComponent = new Engine::CameraMoveComponent(camera);
	m_object->AddComponent(cameraComponent);
	m_gameManager->AddCamera(camera);
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


/* ==================== SkyboxBuilder implementation begin ==================== */
Game::SkyboxBuilder::SkyboxBuilder(Engine::GameManager* gameManager) :
	Utility::Builder<Engine::GameNode>(),
	m_gameManager(gameManager)
{
}

Game::SkyboxBuilder::~SkyboxBuilder()
{
}

void Game::SkyboxBuilder::BuildPart1()
{
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

	//m_object = std::make_unique<GameNode>();
	m_object = new Engine::GameNode();
	m_object->GetTransform().SetPos(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	m_object->GetTransform().SetScale(5.0f); /* TODO: Don't use hardcoded values! Ever! */
	m_object->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh(GET_CONFIG_VALUE_STR_GAME("skyboxModel", "cube.obj")), skyboxMaterial));
}

void Game::SkyboxBuilder::BuildPart2()
{
	m_object->AddComponent(new Engine::ConstantRotationComponent(Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Math::Angle(GET_CONFIG_VALUE_GAME("skyboxRotationSpeed", 0.00005f))));

	m_gameManager->AddSkyboxNode(m_object);
}

#ifdef BUILD_MESH_RENDERER
void Game::SkyboxBuilder::BuildMeshRenderer()
{
}
#endif
/* ==================== SkyboxBuilder implementation end ==================== */