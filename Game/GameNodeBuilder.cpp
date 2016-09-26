#include "GameNodeBuilder.h"
#include "TextureIDs.h"

#include "Engine\CameraComponent.h"

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
Game::CameraNodeBuilder::CameraNodeBuilder(Engine::GameManager* gameManager, Engine::GameNode* cameraNode, Rendering::Camera* camera) :
	GameNodeBuilder(gameManager, cameraNode),
	m_camera(camera),
	m_cameraIndex(0),
	m_cameraIndexStr("0"),
	m_gameNodeToFollow(NULL)
{
}

Game::CameraNodeBuilder::~CameraNodeBuilder()
{
}

void Game::CameraNodeBuilder::BuildPart1()
{
	m_object->GetTransform().SetPos(m_camera->GetPos());
	m_object->GetTransform().SetRot(m_camera->GetRot());
}

void Game::CameraNodeBuilder::BuildPart2()
{
	Math::Real initialDistanceFromEntity = GET_CONFIG_VALUE_GAME("cameraFollowEntityInitialDistance_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowEntityInitialDistance", 0.25f));
	Math::Real angleAroundEntitySpeed = GET_CONFIG_VALUE_GAME("cameraFollowAngleAroundEntitySpeed_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowAngleAroundEntitySpeed", 0.24f));
	Math::Real pitchRotationSpeed = GET_CONFIG_VALUE_GAME("cameraFollowPitchRotationSpeed_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowPitchRotationSpeed", 0.1f));
	Math::Angle initialPitchAngle(GET_CONFIG_VALUE_GAME("cameraFollowInitialPitchAngle_" + m_cameraIndexStr, GET_CONFIG_VALUE_GAME("defaultCameraFollowInitialPitchAngle", 30.0f)));

	Engine::CameraComponent* cameraComponent = NULL;
	if (m_gameNodeToFollow != NULL)
	{
		cameraComponent = new Engine::CameraFollowComponent(m_camera, m_gameNodeToFollow, initialDistanceFromEntity, angleAroundEntitySpeed, pitchRotationSpeed, initialPitchAngle);
	}
	else
	{
		cameraComponent = new Engine::CameraMoveComponent(m_camera);
	}
	m_object->AddComponent(cameraComponent);
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