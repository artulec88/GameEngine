#include "GameNodeBuilder.h"
#include "TextureIDs.h"

#include "Engine\CoreEngine.h"

#include "Utility\IConfig.h"

/* ==================== SkyboxBuilder implementation begin ==================== */
Game::SkyboxBuilder::SkyboxBuilder(Engine::GameManager* gameManager, Engine::GameNode* skyboxNode) :
	Utility::Builder<Engine::GameNode>(skyboxNode),
	m_gameManager(gameManager),
	m_scale(5.0f),
	m_meshRendererComponent(NULL),
	m_constantRotationComponent(NULL)
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