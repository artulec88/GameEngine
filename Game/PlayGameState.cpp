#include "PlayGameState.h"
#include "PlayMenuGameState.h"
#include "LightBuilder.h"
#include "TextureIDs.h"
#include "GameNodeBuilder.h"
#include "CameraBuilder.h"

#include "Engine\GameManager.h"
#include "Engine\CoreEngine.h"
#include "Engine\MeshRendererComponent.h"
#include "Engine\BillboardRendererComponent.h"
#include "Engine\PhysicsComponent.h"
#include "Engine\GravityComponent.h"
#include "Engine\GameNode.h"

#include "Rendering\ParticlesSystem.h"
#include "Rendering\Shader.h"

#include "Math\FloatingPoint.h"

#include "Utility\BuilderDirector.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

Game::PlayGameState::PlayGameState(Engine::GameManager* gameManager, const std::string& inputMappingContextName) :
	GameState(inputMappingContextName),
	m_terrainNode(),
	m_terrainMesh(NULL),
	m_terrainMaterial(NULL),
	m_waterNode(),
	m_skyboxNode(),
	m_playerNode(),
	m_currentCameraIndex(-1),
	m_isMouseLocked(false),
	m_gameManager(gameManager),
	m_mousePicker(),
	m_clockSpeed(GET_CONFIG_VALUE_GAME("clockSpeed", REAL_ONE)),
	m_inGameDateTime(GET_CONFIG_VALUE_GAME("inGameYear", 2016), GET_CONFIG_VALUE_GAME("inGameMonth", 5),
		GET_CONFIG_VALUE_GAME("inGameDay", 22), GET_CONFIG_VALUE_GAME("inGameHour", 9), GET_CONFIG_VALUE_GAME("inGameMinute", 30), GET_CONFIG_VALUE_GAME("inGameSecond", 30)),
#ifdef DRAW_GAME_TIME
	m_inGameTimeGuiButton("9:00:00", gameManager->GetFont(Rendering::Text::FontTypes::CANDARA), GET_CONFIG_VALUE_GAME("fontSizeInGameTime", 2.5f), NULL,
		Math::Vector2D(GET_CONFIG_VALUE_GAME("screenPositionInGameTimeX", 0.0f), GET_CONFIG_VALUE_GAME("screenPositionInGameTimeY", 0.0f)), GET_CONFIG_VALUE_GAME("maxLineLengthInGameTime", 0.5f),
		Rendering::Color(GET_CONFIG_VALUE_GAME("colorInGameTimeRed", 1.0f), GET_CONFIG_VALUE_GAME("colorInGameTimeGreen", 0.0f), GET_CONFIG_VALUE_GAME("colorInGameTimeBlue", 0.0f)),
		Rendering::Color(GET_CONFIG_VALUE_GAME("outlineColorInGameTimeRed", 0.0f), GET_CONFIG_VALUE_GAME("outlineColorInGameTimeGreen", 1.0f), GET_CONFIG_VALUE_GAME("outlineColorInGameTimeBlue", 0.0f)),
		Math::Vector2D(GET_CONFIG_VALUE_GAME("offsetInGameTimeX", 0.005f), GET_CONFIG_VALUE_GAME("offsetInGameTimeY", 0.005f)), GET_CONFIG_VALUE_GAME("isCenteredInGameTime", false),
		GET_CONFIG_VALUE_GAME("characterWidthInGameTime", 0.5f), GET_CONFIG_VALUE_GAME("characterEdgeTransitionWidthInGameTime", 0.1f), GET_CONFIG_VALUE_GAME("borderWidthInGameTime", 0.4f),
		GET_CONFIG_VALUE_GAME("borderEdgeTransitionWidthInGameTime", 0.1f)),
#endif
	m_dayNightMixFactor(REAL_ZERO),
	m_ambientDaytimeColor(GET_CONFIG_VALUE_GAME("ambientDaytimeColorRed", 0.2f),
		GET_CONFIG_VALUE_GAME("ambientDaytimeColorGreen", 0.2f),
		GET_CONFIG_VALUE_GAME("ambientDaytimeColorBlue", 0.2f)),
	m_ambientSunNearHorizonColor(GET_CONFIG_VALUE_GAME("ambientSunNearHorizonColorRed", 0.1f),
		GET_CONFIG_VALUE_GAME("ambientSunNearHorizonColorGreen", 0.1f),
		GET_CONFIG_VALUE_GAME("ambientSunNearHorizonColorBlue", 0.1f)),
	m_ambientNighttimeColor(GET_CONFIG_VALUE_GAME("ambientNighttimeColorRed", 0.02f),
		GET_CONFIG_VALUE_GAME("ambientNighttimeColorGreen", 0.02f),
		GET_CONFIG_VALUE_GAME("ambientNighttimeColorBlue", 0.02f)),
	m_ambientLightColor(m_ambientDaytimeColor),
	m_directionalLightsCount(0),
	m_directionalLights(),
	m_spotLights(),
	m_pointLights(),
	m_previousMousePos(REAL_ZERO, REAL_ZERO),
	m_mousePos(REAL_ZERO, REAL_ZERO),
	m_mousePosChanged(false)
#ifdef PROFILING_GAME_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("PlayGameState"))
#endif
{
	DEBUG_LOG_GAME("Play game state created");
}

Game::PlayGameState::~PlayGameState(void)
{
}

void Game::PlayGameState::Entered()
{
	START_PROFILING_GAME(true, "");

	AddShaders();
	AddTerrainNode();
	AddWaterNodes();
	AddSkyboxNode();
	AddPlayerNode();
	AddBillboards(GET_CONFIG_VALUE_GAME("billboardsTreeCount_1", 10), new Rendering::Material(m_gameManager->AddTexture(TextureIDs::BILLBOARD_TREE_1,
		GET_CONFIG_VALUE_STR_GAME("billboardTreeTexture_1", "Tree1.png")), 1.0f, 8.0f, m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_NORMAL_MAP),
		m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP)));
	AddBillboards(GET_CONFIG_VALUE_GAME("billboardsTreeCount_2", 10), new Rendering::Material(m_gameManager->AddTexture(TextureIDs::BILLBOARD_TREE_2,
		GET_CONFIG_VALUE_STR_GAME("billboardTreeTexture_2", "Tree2.png")), 1.0f, 8.0f, m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_NORMAL_MAP),
		m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP)));
	AddBillboards(GET_CONFIG_VALUE_GAME("billboardsTreeCount_3", 10), new Rendering::Material(m_gameManager->AddTexture(TextureIDs::BILLBOARD_TREE_3,
		GET_CONFIG_VALUE_STR_GAME("billboardTreeTexture_3", "Tree3.png")), 1.0f, 8.0f, m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_NORMAL_MAP),
		m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP)));
	AddCameras();
	AddLights(); // Adding all kinds of light (directional, point, spot)
	
	//const Math::Random::RandomGenerator& randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE);
	//const int treeCount = 30;
	//for (int i = 0; i < treeCount; ++i)
	//{
	//	Engine::GameNode* treeNode = new Engine::GameNode();
	//	Math::Real x = randomGenerator.NextFloat(0.0f, 30.0f);
	//	Math::Real z = randomGenerator.NextFloat(0.0f, 20.0f);
	//	Math::Real y = 0.0f;
	//	treeNode->GetTransform().SetPos(x, y, z);
	//	treeNode->GetTransform().SetRot(Math::Quaternion(Math::Matrix4D(Math::Angle(0.0f), Math::Angle(randomGenerator.NextFloat(0.0f, 180.0f)), Math::Angle(0.0f))));
	//	treeNode->GetTransform().SetScale(0.01f);
	//	//treeNode->SetPhysicsObject(new Physics::PhysicsObject(treeNode->GetTransform(), 1282.0f, Math::Vector3D(0.0f, 0.0f, 0.0f)));
	//	treeNode->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("lowPolyTree.obj"), new Rendering::Material(m_textureFactory.CreateTexture(TextureIDs::TREE, "lowPolyTree.png"), 1.0f, 8.0f, m_textureFactory.GetTexture(Rendering::TextureIDs::DEFAULT_NORMAL_MAP), m_textureFactory.GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP))));
	//	//treeNode->AddComponent(new Engine::GravityComponent(m_terrainMesh));
	//	AddToSceneRoot(treeNode);
	//}

	//const int boulderCount = 30;
	//for (int i = 0; i < boulderCount; ++i)
	//{
	//	Engine::GameNode* boulderNode = new Engine::GameNode();
	//	Math::Real x = randomGenerator.NextFloat(0.0f, 100.0f);
	//	Math::Real z = randomGenerator.NextFloat(0.0f, 100.0f);
	//	Math::Real y = 0.0f;
	//	boulderNode->GetTransform().SetPos(x, y, z);
	//	boulderNode->GetTransform().SetRot(Math::Quaternion(Math::Matrix4D(Math::Angle(0.0f), Math::Angle(randomGenerator.NextFloat(0.0f, 180.0f)), Math::Angle(0.0f))));
	//	boulderNode->GetTransform().SetScale(0.01f);
	//	//boulderNode->SetPhysicsObject(new Physics::PhysicsObject(boulderNode->GetTransform(), 1282.0f, Math::Vector3D(0.0f, 0.0f, 0.0f)));
	//	boulderNode->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("boulder.obj"),
	//		new Rendering::Material(m_textureFactory.CreateTexture(TextureIDs::BOULDER, "boulder.png"), 0.01f, 22.0f, m_textureFactory.CreateTexture(TextureIDs::BOULDER_NORMAL_MAP, "boulderNormal.png"), m_textureFactory.GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP))));
	//	//boulderNode->AddComponent(new Engine::GravityComponent(m_terrainMesh));
	//	AddToSceneRoot(boulderNode);
	//}

	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("PLAY game state has been placed in the game state manager");
	//tthread::thread t(GameManager::Load, GameManager::GetGameManager());
	CHECK_CONDITION_GAME(m_gameManager->IsGameLoaded(), Utility::Logging::ERR, "PLAY game state has been placed in the game state manager before loading the game.");
#ifdef ANT_TWEAK_BAR_ENABLED
	Engine::CoreEngine::GetCoreEngine()->InitializeGameTweakBars();
#endif

#ifdef CALCULATE_STATS
	CoreEngine::GetCoreEngine()->StartSamplingSpf();
#endif
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::Leaving()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("PLAY game state is about to be removed from the game state manager");
#ifdef CALCULATE_STATS
	Rendering::CoreEngine::GetCoreEngine()->StopSamplingSpf();
#endif
}

void Game::PlayGameState::Obscuring()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of PLAY game state");
}

void Game::PlayGameState::Revealed()
{
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("PLAY game state has become the topmost game state in the game state manager's stack");
}

void Game::PlayGameState::AddShaders()
{
	m_gameManager->AddShader(Engine::ShaderTypes::AMBIENT, GET_CONFIG_VALUE_STR_GAME("ambientLightShader", "forward-ambient.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::AMBIENT_TERRAIN, GET_CONFIG_VALUE_STR_GAME("ambientLightTerrainShader", "forward-ambient-terrain.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::AMBIENT_FOG_LINEAR_PLANE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogLinearPlaneBasedShader", "forward-ambient-fog-linear-plane-based.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogExponentialPlaneBasedShader", "forward-ambient-fog-exponential-plane-based.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::AMBIENT_FOG_LINEAR_RANGE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogLinearRangeBasedShader", "forward-ambient-fog-linear-range-based.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogExponentialRangeBasedShader", "forward-ambient-fog-exponential-range-based.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogLinearPlaneBasedTerrainShader", "forward-ambient-fog-linear-plane-based-terrain.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogExponentialPlaneBasedTerrainShader", "forward-ambient-fog-exponential-plane-based-terrain.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogLinearRangeBasedTerrainShader", "forward-ambient-fog-linear-range-based-terrain.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogExponentialRangeBasedTerrainShader", "forward-ambient-fog-exponential-range-based-terrain.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::SHADOW_MAP, GET_CONFIG_VALUE_STR_GAME("shadowMapShader", "ShadowMapGenerator.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::SHADOW_MAP_CUBE, GET_CONFIG_VALUE_STR_GAME("cubeShadowMapShader", "CubeShadowMapGenerator.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::SKYBOX, GET_CONFIG_VALUE_STR_GAME("skyboxShader", "skybox-shader.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::SKYBOX_PROCEDURAL, GET_CONFIG_VALUE_STR_GAME("skyboxProceduralShader", "skybox-procedural-shader.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::WATER, GET_CONFIG_VALUE_STR_GAME("waterShader", "water-shader.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::WATER_NO_DIRECTIONAL_LIGHT, GET_CONFIG_VALUE_STR_GAME("waterNoDirectionalLightShader", "water-no-directional-light-shader.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::BILLBOARD, GET_CONFIG_VALUE_STR_GAME("billboardShader", "billboard-shader.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::DIRECTIONAL_LIGHT, GET_CONFIG_VALUE_STR_GAME("directionalLightShader", "forward-directional.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN, GET_CONFIG_VALUE_STR_GAME("directionalLightTerrainShader", "forward-directional-terrain.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::DIRECTIONAL_LIGHT_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("directionalLightNoShadowShader", "forward-directional-no-shadows.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("directionalLightNoShadowTerrainShader", "forward-directional-terrain-no-shadows.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::POINT_LIGHT, GET_CONFIG_VALUE_STR_GAME("pointLightShader", "forward-point.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::POINT_LIGHT_TERRAIN, GET_CONFIG_VALUE_STR_GAME("pointLightTerrainShader", "forward-point-terrain.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::POINT_LIGHT_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("pointLightNoShadowShader", "forward-point-no-shadows.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::POINT_LIGHT_TERRAIN_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("pointLightNoShadowTerrainShader", "forward-point-terrain-no-shadows.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::SPOT_LIGHT, GET_CONFIG_VALUE_STR_GAME("spotLightShader", "forward-spot.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::SPOT_LIGHT_TERRAIN, GET_CONFIG_VALUE_STR_GAME("spotLightTerrainShader", "forward-spot-terrain.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::SPOT_LIGHT_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("spotLightNoShadowShader", "forward-spot-no-shadows.glsl"));
	m_gameManager->AddShader(Engine::ShaderTypes::SPOT_LIGHT_TERRAIN_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("spotLightNoShadowTerrainShader", "forward-spot-terrain-no-shadows.glsl"));
}

void Game::PlayGameState::AddTerrainNode()
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Adding terrain node");
	m_terrainMesh = new Rendering::TerrainMesh(0, 0, Math::HeightsGenerator(0, 0, GET_CONFIG_VALUE_GAME("terrainVertexCount", 128), GET_CONFIG_VALUE_GAME("terrainHeightGeneratorAmplitude", 70.0f),
		GET_CONFIG_VALUE_GAME("terrainHeightGeneratorOctavesCount", 3), GET_CONFIG_VALUE_GAME("terrainHeightGeneratorRoughness", 0.3f)), GET_CONFIG_VALUE_GAME("terrainVertexCount", 128));
	m_terrainMaterial = new Rendering::Material(m_gameManager->AddTexture(TextureIDs::TERRAIN_DIFFUSE, GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture", "grass4.jpg")), GET_CONFIG_VALUE_GAME("defaultSpecularIntensity", 1.0f),
		GET_CONFIG_VALUE_GAME("defaultSpecularPower", 8.0f), m_gameManager->AddTexture(TextureIDs::TERRAIN_NORMAL_MAP, GET_CONFIG_VALUE_STR_GAME("terrainNormalMap", "grass_normal.jpg")),
		m_gameManager->AddTexture(TextureIDs::TERRAIN_DISPLACEMENT_MAP, GET_CONFIG_VALUE_STR_GAME("terrainDisplacementMap", "grass_disp.jpg")),
		GET_CONFIG_VALUE_GAME("defaultDisplacementScale", 0.02f), GET_CONFIG_VALUE_GAME("defaultDisplacementOffset", -0.5f));

	//m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	m_terrainMaterial->SetAdditionalTexture(m_gameManager->AddTexture(TextureIDs::TERRAIN_BLEND_MAP, GET_CONFIG_VALUE_STR_GAME("terrainBlendMap", "terrainBlendMap.png")), "blendMap");
	m_terrainMaterial->SetAdditionalTexture(m_gameManager->AddTexture(TextureIDs::TERRAIN_DIFFUSE_2, GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture2", "rocks2.jpg")), "diffuse2");
	m_terrainMaterial->SetAdditionalTexture(m_gameManager->AddTexture(TextureIDs::TERRAIN_DIFFUSE_3, GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture3", "mud.png")), "diffuse3");
	m_terrainMaterial->SetAdditionalTexture(m_gameManager->AddTexture(TextureIDs::TERRAIN_DIFFUSE_4, GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture4", "path.png")), "diffuse4");
	//m_resourcesLoaded += 1; // TODO: Consider creating some prettier solution. This is ugly
	m_terrainNode.AddComponent(new Engine::MeshRendererComponent(m_terrainMesh, m_terrainMaterial));
	//m_terrainNode->GetTransform().SetPos(0.0f, 0.0f, 5.0f);
	//m_terrainNode->GetTransform().SetScale(20.0f);
	//m_terrainMesh->Initialize();
	m_terrainMesh->TransformPositions(m_terrainNode.GetTransform().GetTransformation());
	//AddToSceneRoot(m_terrainNode); // Terrain node uses special shaders, so we don't actually add it to the game scene hierarchy. Instead we just register it for the renderer to use it.
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::AddWaterNodes()
{
	// It seems we have a problem with sharing resources. If I use the plane.obj (which I use in other entities) then we'll have problems with rendering (e.g. disappearing billboards).
	// If I change it to myPlane.obj which is not used in other entities the errors seem to be gone.
	m_waterNode.AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("myPlane.obj"), NULL /* The NULL material fixes the problem with rendering both billboards and water nodes simultaneously. TODO: But why / how? */));
	//m_resourcesLoaded += 2;
	m_waterNode.GetTransform().SetPos(GET_CONFIG_VALUE_GAME("waterNodePosX", -18.0f), GET_CONFIG_VALUE_GAME("waterNodePosY", 0.0f), GET_CONFIG_VALUE_GAME("waterNodePosZ", -12.0f));
	m_waterNode.GetTransform().SetScale(0.2f);
}

void Game::PlayGameState::AddSkyboxNode()
{
	START_PROFILING_GAME(true, "");

	DEBUG_LOG_GAME("Creating a skybox");

	SkyboxBuilder skyboxBuilder(m_gameManager, &m_skyboxNode);
	Utility::BuilderDirector<Engine::GameNode> skyboxBuilderDirector(skyboxBuilder);
	skyboxBuilderDirector.Construct();
	NOTICE_LOG_GAME("The skybox has been created");
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::AddPlayerNode()
{
	const Math::Real playerPositionX = GET_CONFIG_VALUE_GAME("playerPosition_X", 11.2f);
	const Math::Real playerPositionZ = GET_CONFIG_VALUE_GAME("playerPosition_Z", 1.95f);
	const Math::Real playerPositionY = 1.82f; // m_terrainMesh->GetHeightAt(Math::Vector2D(playerPositionX, playerPositionZ));
	m_playerNode.GetTransform().SetPos(playerPositionX, playerPositionY, playerPositionZ);
	m_playerNode.GetTransform().SetScale(0.0005f);
	m_playerNode.CreatePhysicsObject(122.0f, Math::Vector3D(0.0f, 0.0f, 0.0f));
	m_playerNode.AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("mike\\Mike.obj"), new Rendering::Material(m_gameManager->AddTexture(TextureIDs::PLAYER, "mike_d.tga"), 1.0f, 8.0f, m_gameManager->AddTexture(TextureIDs::PLAYER_NORMAL_MAP, "mike_n.tga"), m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP))));
	m_playerNode.AddComponent(new Engine::PhysicsComponent(2555.5f, 2855.2f)); //, 0.26f, 5.0f, Math::Angle(152.0f, Math::Unit::DEGREE), 0.015f, 0.0002f));
	m_playerNode.AddComponent(new Engine::GravityComponent(m_terrainMesh));
	//Rendering::Particles::ParticlesSystem particlesSystem = CreateParticlesSystem(ParticleEffects::FOUNTAIN);
	//playerNode->AddComponent(new Engine::ParticlesSystemComponent(this, particlesSystem));
	//m_resourcesLoaded += 2;
	m_rootGameNode.AddChild(&m_playerNode);
}

void Game::PlayGameState::AddBillboards(unsigned int billboardsCount, Rendering::Material* billboardsMaterial)
{
	const Math::Random::RandomGenerator& randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE);
	Math::Real angle = 0.0f;
	std::vector<Math::Real> billboardsModelMatrices;
	billboardsModelMatrices.reserve(billboardsCount * MATRIX_SIZE * MATRIX_SIZE);
	for (int i = 0; i < billboardsCount; ++i)
	{
		Math::Real x = randomGenerator.NextFloat(0.0f, 150.0f);
		Math::Real z = randomGenerator.NextFloat(0.0f, 150.0f);
		Math::Real y = 0.0f;

		Math::Transform billboardTransform(Math::Vector3D(x, y, z), Math::Quaternion(Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Angle(angle)), 0.5f);
		//angle += 15.0f;
		Math::Matrix4D billboardModelMatrix = billboardTransform.GetTransformation();

		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(0, 0));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(0, 1));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(0, 2));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(0, 3));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(1, 0));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(1, 1));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(1, 2));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(1, 3));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(2, 0));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(2, 1));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(2, 2));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(2, 3));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(3, 0));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(3, 1));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(3, 2));
		billboardsModelMatrices.push_back(billboardModelMatrix.GetElement(3, 3));
	}
	Engine::GameNode billboardsNode;
	billboardsNode.AddComponent(new Engine::BillboardsRendererComponent(new Rendering::BillboardMesh(&billboardsModelMatrices[0], billboardsCount, MATRIX_SIZE * MATRIX_SIZE), billboardsMaterial));
	m_billboardsNodes.push_back(std::move(billboardsNode));
}

void Game::PlayGameState::AddCameras()
{
	START_PROFILING_GAME(true, "");
	const int cameraCount = GET_CONFIG_VALUE_GAME("cameraCount", 3);
	CHECK_CONDITION_EXIT_ALWAYS_GAME(cameraCount >= 1, Utility::Logging::CRITICAL, "No cameras defined in the rendering engine.");

	DEBUG_LOG_GAME("Creating ", cameraCount, " camera(-s)");

	Rendering::Camera camera;
	CameraBuilder cameraBuilder(m_gameManager, &camera);
	Utility::BuilderDirector<Rendering::Camera> cameraBuilderDirector(cameraBuilder);
	for (int i = 0; i < cameraCount; ++i)
	{
		cameraBuilder.SetCameraIndex(i);
		//cameraBuilder.SetEntityToFollow(entityToFollow);
		cameraBuilderDirector.Construct();
		if (m_cameras.empty())
		{
			camera.Activate();
			m_currentCameraIndex = 0;
		}
		m_cameras.push_back(camera);
		CRITICAL_LOG_GAME("You forgot to add camera node to scene root node.!!!");
		CRITICAL_LOG_GAME("There are no camera components in the application anymore!!!");
		//AddToSceneRoot(cameraNode);
	}
	NOTICE_LOG_GAME(cameraCount, " camera(-s) created");
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::AddLights()
{
	START_PROFILING_GAME(true, "");
	AddDirectionalLight(); // Adding directional light (if enabled)
	AddPointLights();
	AddSpotLights();
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::AddDirectionalLight()
{
	// TODO: For now we only check if directionalLightsCount is zero or not.
	// In the future there might be many directional lights enabled (?)
	int directionalLightsCount = GET_CONFIG_VALUE_GAME("directionalLightsCount", 1);
	if (directionalLightsCount == 0)
	{
		NOTICE_LOG_GAME("Directional lights disabled");
		return;
	}
	NOTICE_LOG_GAME("Directional lights enabled");

	Rendering::Lighting::DirectionalLight directionalLight;
	DirectionalLightBuilder directionalLightBuilder(m_gameManager->GetShaderFactory(), m_gameManager->GetTextureFactory(), &directionalLight);
	Utility::BuilderDirector<Rendering::Lighting::DirectionalLight> lightBuilderDirector(directionalLightBuilder);
	lightBuilderDirector.Construct();
	INFO_LOG_RENDERING("Directional light with intensity = ", directionalLight.GetIntensity(), " is being added to directional / spot lights vector");
	++m_directionalLightsCount;
	m_directionalLights.push_back(std::move(directionalLight));
	//m_lights.push_back(directionalLight);
}

void Game::PlayGameState::AddPointLights()
{
	const int pointLightsCount = GET_CONFIG_VALUE_GAME("pointLightsCount", 1);
	if (pointLightsCount > 0)
	{
		DEBUG_LOG_GAME("Creating ", pointLightsCount, " point lights");
		Rendering::Lighting::PointLight pointLight;
		PointLightBuilder pointLightBuilder(m_gameManager->GetShaderFactory(), m_gameManager->GetTextureFactory(), &pointLight);
		Utility::BuilderDirector<Rendering::Lighting::PointLight> lightBuilderDirector(pointLightBuilder);
		for (int i = 0; i < pointLightsCount; ++i)
		{
			pointLightBuilder.SetLightIndex(i);
			lightBuilderDirector.Construct();
			m_pointLights.push_back(std::move(pointLight));
			//m_lights.push_back(pointLight);
		}
		NOTICE_LOG_GAME(pointLightsCount, " point lights created");
	}
	else
	{
		NOTICE_LOG_GAME("Point lights disabled");
	}
}

void Game::PlayGameState::AddSpotLights()
{
	const int spotLightsCount = GET_CONFIG_VALUE_GAME("spotLightsCount", 1);
	if (spotLightsCount > 0)
	{
		DEBUG_LOG_GAME("Creating ", spotLightsCount, " spot lights");
		Rendering::Lighting::SpotLight spotLight;
		SpotLightBuilder spotLightBuilder(m_gameManager->GetShaderFactory(), m_gameManager->GetTextureFactory(), &spotLight);
		Utility::BuilderDirector<Rendering::Lighting::SpotLight> lightBuilderDirector(spotLightBuilder);
		for (int i = 0; i < spotLightsCount; ++i)
		{
			spotLightBuilder.SetLightIndex(i);
			lightBuilderDirector.Construct();
			m_spotLights.push_back(std::move(spotLight));
			//m_lights.push_back(spotLight);
		}
		NOTICE_LOG_GAME(spotLightsCount, " spot lights created");
	}
	else
	{
		NOTICE_LOG_GAME("Spot lights disabled");
	}
}

void Game::PlayGameState::Handle(Engine::Actions::Action action)
{
	START_PROFILING_GAME(true, "");
	switch (action)
	{
	case Engine::Actions::SHOW_PLAY_MENU:
		m_gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(m_gameManager->GetPlayMainMenuGameState(), Engine::GameStateTransitioning::PUSH, Engine::GameStateModality::EXCLUSIVE));
		break;
	case Engine::Actions::MOVE_CAMERA_UP:
		m_cameras[m_currentCameraIndex].GetPos().SetY(m_cameras[m_currentCameraIndex].GetPos().GetY() + 0.05f);
		//CRITICAL_LOG_GAME("Moving up... Current position: " + m_cameras[m_currentCameraIndex].GetPos().ToString());
		break;
	case Engine::Actions::MOVE_CAMERA_DOWN:
		m_cameras[m_currentCameraIndex].GetPos().SetY(m_cameras[m_currentCameraIndex].GetPos().GetY() - 0.05f);
		//CRITICAL_LOG_GAME("Moving down... Current position: " + m_cameras[m_currentCameraIndex].GetPos().ToString());
		break;
	case Engine::Actions::MOVE_CAMERA_LEFT:
		m_cameras[m_currentCameraIndex].GetPos().SetX(m_cameras[m_currentCameraIndex].GetPos().GetX() - 0.05f);
		break;
	case Engine::Actions::MOVE_CAMERA_RIGHT:
		m_cameras[m_currentCameraIndex].GetPos().SetX(m_cameras[m_currentCameraIndex].GetPos().GetX() + 0.05f);
		break;
	case Engine::Actions::MOVE_CAMERA_FORWARD:
		m_cameras[m_currentCameraIndex].GetPos().SetZ(m_cameras[m_currentCameraIndex].GetPos().GetZ() + 0.05f);
		break;
	case Engine::Actions::MOVE_CAMERA_BACKWARD:
		m_cameras[m_currentCameraIndex].GetPos().SetZ(m_cameras[m_currentCameraIndex].GetPos().GetZ() - 0.05f);
		break;
	default:
		INFO_LOG_GAME("Action ", action, " is not supported by the PLAY game state.");
	}
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::Handle(Engine::States::State state)
{
	//DELOCUST_LOG_GAME("Handling the state ", state);
	switch (state)
	{
	case Engine::States::MOUSE_KEY_LEFT_PRESSED:
		DEBUG_LOG_GAME("Mouse left key pressed");
		break;
	case Engine::States::MOUSE_KEY_MIDDLE_PRESSED:
		DEBUG_LOG_GAME("Mouse middle key pressed");
		break;
	case Engine::States::MOUSE_KEY_RIGHT_PRESSED:
		DEBUG_LOG_GAME("Mouse right key pressed");
		break;
	default:
		DEBUG_LOG_GAME("The state ", state, " is not supported by the MenuGameState");
		break;
	}
}

void Game::PlayGameState::Handle(Engine::Ranges::Range range, Math::Real value)
{
	switch (range)
	{
	case Engine::Ranges::AXIS_X:
		m_previousMousePos.SetX(m_mousePos.GetX());
		m_mousePos.SetX(value);
		m_mousePosChanged = true;
		DEBUG_LOG_GAME("Mouse pos = ", m_mousePos.ToString());
		break;
	case Engine::Ranges::AXIS_Y:
		m_previousMousePos.SetY(m_mousePos.GetY());
		m_mousePos.SetY(value);
		DEBUG_LOG_GAME("Mouse pos = ", m_mousePos.ToString());
		break;
	default:
		DEBUG_LOG_GAME("The range ", range, " is not supported by the PlayGameState");
		break;
	}
}

void Game::PlayGameState::MouseButtonEvent(int button, int action, int mods)
{
	START_PROFILING_GAME(true, "");
	m_rootGameNode.MouseButtonEvent(button, action, mods);
	//switch (action)
	//{
	//case GLFW_PRESS:
	//	m_isMouseLocked = ! m_isMouseLocked;
	//	if (m_isMouseLocked)
	//	{
	//		Rendering::CoreEngine::GetCoreEngine()->CentralizeCursor();
	//	}
	//	INFO_LOG_GAME("Mouse button pressed: button=", button, "\t mods=", mods);
	//	break;
	//case GLFW_RELEASE:
	//	INFO_LOG_GAME("Mouse button released: button=", button, "\t mods=", mods);
	//	break;
	//default:
	//	WARNING_LOG_GAME("Unknown action performed with the mouse. Button=", button, "\t action=", action, "\t mods=", mods);
	//}
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::MousePosEvent(double xPos, double yPos)
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Cursor position = (", xPos, ", ", yPos, ")");

	//const Rendering::Camera& currentCamera = Engine::CoreEngine::GetCoreEngine()->GetCurrentCamera();
	//m_mousePicker.CalculateCurrentRay(xPos, yPos, currentCamera.GetProjection(), currentCamera.GetViewMatrix());

	//m_rootGameNode.MousePosEvent(xPos, yPos);


	//if (!m_isMouseLocked)
	//{
	//	STOP_PROFILING_GAME("");
	//	return;
	//}

	//int width = Rendering::CoreEngine::GetCoreEngine()->GetWindowWidth();
	//int height = Rendering::CoreEngine::GetCoreEngine()->GetWindowHeight();
	//Math::Vector2D centerPosition(static_cast<Math::Real>(width) / 2, static_cast<Math::Real>(height) / 2);
	//Math::Vector2D deltaPosition(static_cast<Math::Real>(xPos), static_cast<Math::Real>(yPos));
	//deltaPosition -= centerPosition;
	//
	//bool rotX = ! Math::AlmostEqual(deltaPosition.GetX(), REAL_ZERO);
	//bool rotY = ! Math::AlmostEqual(deltaPosition.GetY(), REAL_ZERO);

	//if (rotX || rotY)
	//{
	//	Rendering::Transform& transform = Rendering::CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera().GetTransform();
	//	const Math::Real sensitivity = static_cast<Math::Real>(Rendering::Camera::GetSensitivity());
	//	if (rotX)
	//	{
	//		transform.Rotate(Math::Vector3D(0, 1, 0), Math::Angle(deltaPosition.GetX() * sensitivity));
	//	}
	//	if (rotY)
	//	{
	//		transform.Rotate(transform.GetRot().GetRight(), Math::Angle(deltaPosition.GetY() * sensitivity));
	//	}
	//	Rendering::CoreEngine::GetCoreEngine()->CentralizeCursor();
	//}
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::ScrollEvent(double xOffset, double yOffset)
{
	START_PROFILING_GAME(true, "");
	m_rootGameNode.ScrollEvent(xOffset, yOffset);
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::Render(Rendering::Renderer* renderer) const
{
	// TODO: Updating the state of the rendering engine (e.g. the values of some of its member variables)
	// in this function is not good. This should be done in the Update function (or maybe not?).
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_EXIT_GAME(renderer != NULL, Utility::Logging::CRITICAL, "Cannot render the game. The rendering engine is NULL.");
	DEBUG_LOG_GAME("PLAY game state rendering");

	renderer->InitRenderScene(m_ambientLightColor, m_dayNightMixFactor);
	renderer->SetCurrentCamera(&m_cameras[m_currentCameraIndex]);

	RenderWaterTextures(renderer);

	renderer->BindDisplayTexture();
	renderer->ClearScreen();

	RenderSceneWithAmbientLight(renderer);
	//m_rootGameNode.Render(shader, renderer);
	//RenderSceneWithPointLights(renderer); // Point light rendering
	RenderSceneWithDirectionalAndSpotLights(renderer); // Directional and spot light rendering

	RenderWaterNodes(renderer);

	RenderBillboardNodes(renderer);

	RenderSkybox(renderer);

	RenderParticles(renderer);

#ifdef DEBUG_RENDERING_ENABLED
	renderer->RenderDebugNodes(m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::GUI));
#endif

	renderer->FinalizeRenderScene((renderer->GetAntiAliasingMethod() == Rendering::Aliasing::FXAA) ?
		m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::FILTER_FXAA) :
		m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::FILTER_NULL));

#ifdef DRAW_GAME_TIME
	m_inGameTimeGuiButton.SetText(m_inGameDateTime.ToString());
	renderer->RenderGuiControl(m_inGameTimeGuiButton, m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::TEXT));
#endif

	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderSceneWithAmbientLight(Rendering::Renderer* renderer) const
{
	const Rendering::Shader* ambientShader = GetAmbientShader(renderer->GetFogInfo());
	renderer->BindShader(ambientShader);
	renderer->UpdateRendererUniforms(ambientShader);
	m_rootGameNode.Render(ambientShader, renderer);
	CHECK_CONDITION_GAME(m_gameManager->GetTerrainNode() != NULL, Utility::Logging::ERR, "Cannot render terrain. There are no terrain nodes registered.");
	const Rendering::Shader* ambientTerrainShader = GetAmbientTerrainShader(renderer->GetFogInfo());
	renderer->BindShader(ambientTerrainShader);
	renderer->UpdateRendererUniforms(ambientTerrainShader);
	m_terrainNode.Render(ambientTerrainShader, renderer);
}

void Game::PlayGameState::RenderSceneWithPointLights(Rendering::Renderer* renderer) const
{
	if (!Rendering::Lighting::PointLight::ArePointLightsEnabled())
	{
		DEBUG_LOG_GAME("All point lights are disabled");
		return;
	}

	for (size_t i = 0; i < m_pointLights.size(); ++i)
	{
		const Rendering::Lighting::PointLight* currentPointLight = renderer->SetCurrentPointLight(&(m_pointLights[i]));
		if (currentPointLight->IsEnabled())
		{
			DEBUG_LOG_GAME("Point light at index ", i, " is disabled");
			continue;
		}
		m_rootGameNode.Render(currentPointLight->GetShader(), renderer);
		m_terrainNode.Render(currentPointLight->GetTerrainShader(), renderer);
	}
}

void Game::PlayGameState::RenderSceneWithDirectionalAndSpotLights(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	for (auto lightItr = m_directionalLights.begin(); lightItr != m_directionalLights.end(); ++lightItr)
	{
		if (lightItr->IsEnabled())
		{
			const Rendering::Lighting::BaseLight* currentLight = renderer->SetCurrentLight(&(*lightItr));
			if (renderer->InitShadowMap())
			{
				// Render scene using shadow mapping shader
				const Rendering::Shader* shadowMapShader = m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::SHADOW_MAP);
				renderer->BindShader(shadowMapShader);
				renderer->UpdateRendererUniforms(shadowMapShader);
				m_rootGameNode.Render(shadowMapShader, renderer);
				m_terrainNode.Render(shadowMapShader, renderer); // TODO: Probably unnecessary
				renderer->FinalizeShadowMapRendering(m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::FILTER_GAUSSIAN_BLUR));
			}

			renderer->InitLightRendering();
			// TODO: Render scene with light is not ready. Check the function Renderer::RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode, bool isCastingShadowsEnabled /* = true */).
			renderer->BindShader(currentLight->GetShader());
			renderer->UpdateRendererUniforms(currentLight->GetShader());
			m_rootGameNode.Render(currentLight->GetShader(), renderer);
			renderer->BindShader(currentLight->GetTerrainShader());
			renderer->UpdateRendererUniforms(currentLight->GetTerrainShader());
			m_terrainNode.Render(currentLight->GetTerrainShader(), renderer);
			renderer->FinalizeLightRendering();
		}
	}
	for (auto lightItr = m_spotLights.begin(); lightItr != m_spotLights.end(); ++lightItr)
	{
		if (lightItr->IsEnabled())
		{
			const Rendering::Lighting::BaseLight* currentLight = renderer->SetCurrentLight(&(*lightItr));
			if (renderer->InitShadowMap())
			{
				// Render scene using shadow mapping shader
				const Rendering::Shader* shadowMapShader = m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::SHADOW_MAP);
				renderer->BindShader(shadowMapShader);
				renderer->UpdateRendererUniforms(shadowMapShader);
				m_rootGameNode.Render(shadowMapShader, renderer);
				m_terrainNode.Render(shadowMapShader, renderer); // TODO: Probably unnecessary
				renderer->FinalizeShadowMapRendering(m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::FILTER_GAUSSIAN_BLUR));
			}

			renderer->InitLightRendering();
			// TODO: Render scene with light is not ready. Check the function Renderer::RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode, bool isCastingShadowsEnabled /* = true */).
			renderer->BindShader(currentLight->GetShader());
			renderer->UpdateRendererUniforms(currentLight->GetShader());
			m_rootGameNode.Render(currentLight->GetShader(), renderer);
			renderer->BindShader(currentLight->GetTerrainShader());
			renderer->UpdateRendererUniforms(currentLight->GetTerrainShader());
			m_terrainNode.Render(currentLight->GetTerrainShader(), renderer);
			renderer->FinalizeLightRendering();
		}
	}
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderSkybox(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Skybox rendering started");

	//if (m_fogEnabled)
	//{
	//	STOP_PROFILING_GAME("");
	//	return;
	//}

	//glDisable(GL_DEPTH_TEST);
	renderer->SetCullFaceFront();
	/**
	 * By default (GL_LESS) we tell OpenGL that an incoming fragment wins the depth test if its Z value is less than the stored one.
	 * However, in the case of a skybox the Z value is always the far Z. The far Z is clipped when the depth test function is set to "less than".
	 * To make it part of the scene we change the depth function to "less than or equal".
	 */
	renderer->SetDepthFuncLessOrEqual();
	const Rendering::Shader* skyboxShader = m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::SKYBOX);
	renderer->BindShader(skyboxShader);
	renderer->UpdateRendererUniforms(skyboxShader);
	m_skyboxNode.Render(skyboxShader, renderer);
	renderer->SetDepthFuncDefault();
	renderer->SetCullFaceDefault();
	//glEnable(GL_DEPTH_TEST);
	//Rendering::CheckErrorCode("Renderer::Render", "Rendering skybox");

	DEBUG_LOG_GAME("Skybox rendering finished");
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderWaterTextures(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != NULL, Utility::Logging::DEBUG, "There are no water nodes registered in the rendering engine");
	// TODO: For now we only support one water node (you can see that in the "distance" calculation). In the future there might be more.

	RenderWaterReflectionTexture(renderer);
	RenderWaterRefractionTexture(renderer);

	renderer->DisableClippingPlanes();
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderWaterReflectionTexture(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != NULL, Utility::Logging::DEBUG, "There are no water nodes registered in the rendering engine");

	// TODO: The camera should be accessible from the game manager. It shouldn't be necessary to access them via rendering engine.
	Rendering::Camera reflectionCamera(m_cameras[m_currentCameraIndex]);
	const Math::Real cameraHeight = reflectionCamera.GetPos().GetY();
	Math::Real distance = 2.0f * (cameraHeight - m_waterNode.GetTransform().GetTransformedPos().GetY());
	reflectionCamera.GetPos().SetY(cameraHeight - distance); // TODO: use m_altCamera instead of the main camera.
	reflectionCamera.GetRot().InvertPitch();

	renderer->EnableWaterReflectionClippingPlane(-m_waterNode.GetTransform().GetTransformedPos().GetY() + 0.1f /* we add 0.1f to remove some glitches on the water surface */);
	renderer->BindWaterReflectionTexture();
	renderer->ClearScreen(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE);

	renderer->SetDepthTest(false);
	renderer->SetCurrentCamera(&reflectionCamera);
	RenderSkybox(renderer);
	RenderSceneWithAmbientLight(renderer);
	renderer->SetCurrentCamera(&m_cameras[m_currentCameraIndex]);

	//RenderSceneWithPointLights(renderer);
	//for (std::vector<Rendering::Lighting::BaseLight*>::iterator lightItr = m_directionalAndSpotLights.begin(); lightItr != m_directionalAndSpotLights.end(); ++lightItr)
	//{
	//	m_currentLight = (*lightItr);
	//	if (!m_currentLight->IsEnabled())
	//	{
	//		continue;
	//	}
	//	RenderSceneWithLight(m_currentLight, gameNode, false);
	//}
	//SetVector3D("inverseFilterTextureSize", Vector3D(REAL_ONE / m_waterReflectionTexture->GetWidth(), REAL_ONE / m_waterReflectionTexture->GetHeight(), REAL_ZERO));

	renderer->SetDepthTest(true);

	//if (Rendering::antiAliasingMethod == Rendering::Aliasing::FXAA)
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_FXAA), m_waterReflectionTexture, NULL);
	//}
	//else
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_NULL), m_waterReflectionTexture, NULL);
	//}

	//BindAsRenderTarget();

	//m_cameras[m_currentCameraIndex].GetPos().SetY(cameraHeight); // TODO: use m_altCamera instead of the main camera.
	//m_cameras[m_currentCameraIndex].GetRot().InvertPitch();

	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderWaterRefractionTexture(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != NULL, Utility::Logging::DEBUG, "There are no water nodes registered in the rendering engine");

	renderer->EnableWaterRefractionClippingPlane(m_waterNode.GetTransform().GetTransformedPos().GetY());
	renderer->BindWaterRefractionTexture();
	renderer->ClearScreen(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE);

	//glDisable(GL_DEPTH_TEST);
	RenderSkybox(renderer);
	RenderSceneWithAmbientLight(renderer);

	//RenderSceneWithPointLights(renderer);
	//for (std::vector<Lighting::BaseLight*>::iterator lightItr = m_directionalAndSpotLights.begin(); lightItr != m_directionalAndSpotLights.end(); ++lightItr)
	//{
	//	m_currentLight = (*lightItr);
	//	if (!m_currentLight->IsEnabled())
	//	{
	//		continue;
	//	}
	//
	//	RenderSceneWithLight(m_currentLight, gameNode, false);
	//}
	//SetVector3D("inverseFilterTextureSize", Vector3D(REAL_ONE / m_waterReflectionTexture->GetWidth(), REAL_ONE / m_waterReflectionTexture->GetHeight(), REAL_ZERO));

	//glEnable(GL_DEPTH_TEST);

	//if (Rendering::antiAliasingMethod == Rendering::Aliasing::FXAA)
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_FXAA), m_waterReflectionTexture, NULL);
	//}
	//else
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_NULL), m_waterReflectionTexture, NULL);
	//}

	//BindAsRenderTarget();

	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderWaterNodes(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	// TODO: Add some condition here that will prevent any water nodes rendering if there are no water nodes available.
	//CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_waterNode != NULL, Utility::Logging::DEBUG, "There are no water nodes registered in the rendering engine");
	renderer->InitWaterNodesRendering();

	// TODO: In the future there might be more than one water node.
	//for (std::vector<GameNode*>::const_iterator waterNodeItr = m_waterNodes.begin(); waterNodeItr != m_waterNodes.end(); ++waterNodeItr)
	//{
	//	if (m_waterLightReflectionEnabled)
	//	{
	//		(*waterNodeItr)->Render(waterShader, this);
	//	}
	//	else
	//	{
	//		(*waterNodeItr)->Render(waterNoDirectionalLightShader, this);
	//	}
	//}
	const Rendering::Shader* waterShader = GetWaterShader(renderer);
	renderer->BindShader(waterShader);
	renderer->UpdateRendererUniforms(waterShader);
	m_waterNode.Render(waterShader, renderer);
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderBillboardNodes(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Rendering billboards started");
	//renderer->SetDepthTest(false);
	renderer->SetBlendingEnabled(true);
	//GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
	// GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE,
	// GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR, GL_SRC1_ALPHA, and GL_ONE_MINUS_SRC1_ALPHA
	renderer->SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	const Rendering::Shader* billboardShader = m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::BILLBOARD);
	renderer->BindShader(billboardShader);
	renderer->UpdateRendererUniforms(billboardShader);
	for (auto billboardsNodeItr = m_billboardsNodes.begin(); billboardsNodeItr != m_billboardsNodes.end(); ++billboardsNodeItr)
	{
		billboardsNodeItr->Render(billboardShader, renderer);
	}
	//renderer->SetDepthTest(true);
	renderer->SetBlendingEnabled(false);
	//renderer->SetBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderParticles(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Rendering particles started");
	const Rendering::Shader* particlesShader = m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::PARTICLES);
	renderer->BindShader(particlesShader);
	renderer->UpdateRendererUniforms(particlesShader);
	for (auto particleSystemItr = m_gameManager->GetParticlesSystems().begin(); particleSystemItr != m_gameManager->GetParticlesSystems().end(); ++particleSystemItr)
	{
		//if (!(*particleSystemItr)->GetTexture()->IsAdditive())
		//{
			//(*particleSystemItr)->SortParticles(renderer->GetCurrentCamera().GetPos());
		//}
		renderer->RenderParticles(particlesShader, *(*particleSystemItr));
	}
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::Update(Math::Real elapsedTime)
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("PLAY game state updating");
	m_rootGameNode.Update(elapsedTime);
	m_skyboxNode.Update(elapsedTime);
	m_skyboxNode.GetTransform().SetPos(m_cameras[m_currentCameraIndex].GetPos()); // TODO: Instead, skyboxNode should have a simple component that does exactly that!

	//EMERGENCY_LOG_GAME("Elapsed time: ", elapsedTime * 1000.0f, " [ms]");
	m_inGameDateTime += Utility::Timing::TimeSpan(elapsedTime * m_clockSpeed * 1000000.0f, Utility::Timing::MICROSECOND);
	//CRITICAL_LOG_GAME("Clock speed = ", m_clockSpeed, ". ElapsedTime = ", elapsedTime, ". In-game time: ", m_inGameDateTime.ToString());

	//AdjustAmbientLightAccordingToCurrentTime();

	CalculateSunElevationAndAzimuth();
	DEBUG_LOG_GAME("PLAY game state updated");
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::CalculateSunElevationAndAzimuth()
{
	//const int timeGMTdifference = 1;

	//const Math::Angle b(0.9863014f * (m_inGameDateTime->GetDayInYear() - 81)); // 0,98630136986301369863013698630137 = 360 / 365
	//const Math::Real bSin = b.Sin();
	//const Math::Real bCos = b.Cos();

	//const Math::Real equationOfTime = 19.74f * bSin * bCos - 7.53f * bCos - 1.5f * bSin; // EoT
	//const Math::Real declinationSin = TROPIC_OF_CANCER_SINUS * bSin;
	//const Math::Angle declinationAngle(asin(declinationSin), Math::Unit::RADIAN);
	////DEBUG_LOG_ENGINE("Declination in degrees = ", declinationAngle.GetAngleInDegrees());

	//const Math::Real timeCorrectionInSeconds = 60.0f * (4.0f * (m_longitude.GetAngleInDegrees() - 15.0f * timeGMTdifference) + equationOfTime);
	//const Math::Real localSolarTime = m_inGameDateTime->GetDayTime() + timeCorrectionInSeconds;
	////DEBUG_LOG_ENGINE("Time correction in seconds = ", timeCorrectionInSeconds);
	////DEBUG_LOG_ENGINE("Local time = ", m_timeOfDay, "\tLocal solar time = ", localSolarTime);

	//const Math::Angle hourAngle(15.0f * (localSolarTime - 12 * Utility::Timing::DateTime::SECONDS_PER_HOUR) / Utility::Timing::DateTime::SECONDS_PER_HOUR);
	////DEBUG_LOG_ENGINE("Hour angle = ", hourAngle.GetAngleInDegrees());

	//const Math::Real sunElevationSin = declinationSin * m_latitude.Sin() + declinationAngle.Cos() * m_latitude.Cos() * hourAngle.Cos();
	//m_sunElevation.SetAngleInRadians(asin(sunElevationSin));
	////DEBUG_LOG_ENGINE("Sun elevation = ", m_sunElevation.GetAngleInDegrees());

	//const Math::Real sunAzimuthCos = ((declinationSin * m_latitude.Cos()) - (declinationAngle.Cos() * m_latitude.Sin() * hourAngle.Cos())) / m_sunElevation.Cos();
	//m_sunAzimuth.SetAngleInRadians(acos(sunAzimuthCos));
	//bool isAfternoon = (localSolarTime > 12.0f * Utility::Timing::DateTime::SECONDS_PER_HOUR) || (hourAngle.GetAngleInDegrees() > REAL_ZERO);
	//if (isAfternoon)
	//{
	//	m_sunAzimuth.SetAngleInDegrees(360.0f - m_sunAzimuth.GetAngleInDegrees());
	//}
	//DEBUG_LOG_ENGINE("Sun azimuth = ", m_sunAzimuth.GetAngleInDegrees());
}

//void Game::PlayGameState::AdjustAmbientLightAccordingToCurrentTime()
//{
//	START_PROFILING_GAME(true, "");
//	switch (dayTime)
//	{
//	case Utility::Timing::NIGHT:
//		dayNightMixFactor = REAL_ZERO;
//		m_ambientLightColor = m_ambientNighttimeColor;
//		break;
//	case Utility::Timing::BEFORE_DAWN:
//		dayNightMixFactor = REAL_ZERO;
//		m_ambientLightColor = m_ambientNighttimeColor.Lerp(m_ambientSunNearHorizonColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	case Utility::Timing::SUNRISE:
//		dayNightMixFactor = dayTimeTransitionFactor;
//		m_ambientLightColor = m_ambientSunNearHorizonColor.Lerp(m_ambientDaytimeColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	case Utility::Timing::DAY:
//		dayNightMixFactor = REAL_ONE;
//		m_ambientLightColor = m_ambientDaytimeColor;
//		break;
//	case Utility::Timing::SUNSET:
//		dayNightMixFactor = dayTimeTransitionFactor;
//		m_ambientLightColor = m_ambientSunNearHorizonColor.Lerp(m_ambientDaytimeColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	case Utility::Timing::AFTER_DUSK:
//		dayNightMixFactor = REAL_ZERO;
//		m_ambientLightColor = m_ambientNighttimeColor.Lerp(m_ambientSunNearHorizonColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	}
//	STOP_PROFILING_GAME("");
//}

const Rendering::Shader* Game::PlayGameState::GetAmbientShader(const Rendering::FogEffect::FogInfo& fogInfo) const
{
	START_PROFILING_ENGINE(true, "");
	if (fogInfo.IsEnabled()) // if (fogInfo != NULL)
	{
		//DEBUG_LOG_RENDERING("Fog fall-off type: ", m_fogFallOffType, ". Fog distance calculation type: ", m_fogCalculationType);

		// TODO: A very ugly way. If we decide to add more fog fall off or calculation types then we will surely have a big problem in here.
		if (fogInfo.GetFallOffType() == Rendering::FogEffect::LINEAR)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_FOG_LINEAR_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_FOG_LINEAR_RANGE_BASED);
			}
		}
		else if (fogInfo.GetFallOffType() == Rendering::FogEffect::EXPONENTIAL)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED);
			}
		}
	}
	STOP_PROFILING_ENGINE("");
	return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT);
}

const Rendering::Shader* Game::PlayGameState::GetAmbientTerrainShader(const Rendering::FogEffect::FogInfo& fogInfo) const
{
	START_PROFILING_ENGINE(true, "");
	if (fogInfo.IsEnabled())
	{
		//DEBUG_LOG_RENDERING("Fog fall-off type: ", m_fogFallOffType, ". Fog distance calculation type: ", m_fogCalculationType);
		// TODO: A very ugly way. If we decide to add more fog fall off or calculation types then we will surely have a big problem in here.
		if (fogInfo.GetFallOffType() == Rendering::FogEffect::LINEAR)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED);
			}
		}
		else if (fogInfo.GetFallOffType() == Rendering::FogEffect::EXPONENTIAL)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED);
			}
		}
	}
	STOP_PROFILING_ENGINE("");
	return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_TERRAIN);
}

unsigned int Game::PlayGameState::SetCurrentCamera(unsigned int cameraIndex)
{
	CHECK_CONDITION_RENDERING((cameraIndex >= 0) && (cameraIndex < m_cameras.size()), Utility::Logging::ERR, "Incorrect current camera index. Passed ",
		cameraIndex, " when the correct range is (", 0, ", ", m_cameras.size(), ").");
	m_cameras[m_currentCameraIndex].Deactivate();
	m_currentCameraIndex = cameraIndex;
	m_cameras[m_currentCameraIndex].Activate();
#ifndef ANT_TWEAK_BAR_ENABLED
	NOTICE_LOG_RENDERING("Switched to camera #", m_currentCameraIndex + 1);
	//DEBUG_LOG_RENDERING("Current camera parameters: ", m_cameras[m_currentCameraIndex]->ToString());
#endif
	return m_currentCameraIndex;
}

unsigned int Game::PlayGameState::NextCamera()
{
	if (m_currentCameraIndex == static_cast<int>(m_cameras.size()) - 1)
	{
		m_currentCameraIndex = -1;
	}
	return SetCurrentCamera(m_currentCameraIndex + 1);
}

unsigned int Game::PlayGameState::PrevCamera()
{
	if (m_currentCameraIndex == 0)
	{
		m_currentCameraIndex = m_cameras.size();
	}
	return SetCurrentCamera(m_currentCameraIndex - 1);
}