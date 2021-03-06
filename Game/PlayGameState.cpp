#include "PlayGameState.h"
#include "PlayMenuGameState.h"
#include "MeshIDs.h"
#include "TextureIDs.h"
#include "GameNodeBuilder.h"

#include "Engine/GameManager.h"
#include "Engine/CoreEngine.h"
#include "Engine/MeshRendererComponent.h"
#include "Engine/BillboardRendererComponent.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/GravityComponent.h"
#include "Engine/GameNode.h"

#include "Rendering/CameraBuilder.h"
#include "Rendering/LightBuilder.h"
#include "Rendering/MeshIDs.h"

#include "Utility/BuilderDirector.h"
#include "Utility/ILogger.h"
#include "Utility/IConfig.h"

game::PlayGameState::PlayGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName) :
	GameState(inputMappingContextName),
	m_rootGameNode(),
	m_terrainNode(),
	m_terrain(nullptr),
	m_terrainMaterial(nullptr),
	m_waterNode(),
	m_skyboxNode(),
	m_playerNode(),
	m_nodes(),
	m_currentCameraIndex(-1),
	m_camerasNode(),
	m_isMouseLocked(false),
	m_gameManager(gameManager),
	m_mousePicker(),
	m_clockSpeed(GET_CONFIG_VALUE_GAME("clockSpeed", REAL_ONE)),
	m_inGameDateTime(GET_CONFIG_VALUE_GAME("inGameYear", 2016), GET_CONFIG_VALUE_GAME("inGameMonth", 5),
		GET_CONFIG_VALUE_GAME("inGameDay", 22), GET_CONFIG_VALUE_GAME("inGameHour", 9), GET_CONFIG_VALUE_GAME("inGameMinute", 30), GET_CONFIG_VALUE_GAME("inGameSecond", 30)),
#ifdef DRAW_GAME_TIME
	m_inGameTimeGuiButton("9:00:00", gameManager->GetFont(rendering::text::font_ids::CANDARA), GET_CONFIG_VALUE_GAME("fontSizeInGameTime", 2.5f), nullptr,
		math::Vector2D(GET_CONFIG_VALUE_GAME("screenPositionInGameTimeX", 0.0f), GET_CONFIG_VALUE_GAME("screenPositionInGameTimeY", 0.0f)), math::Angle(GET_CONFIG_VALUE_GAME("screenRotationInGameTime", 0.0f)),
		math::Vector2D(GET_CONFIG_VALUE_GAME("screenScaleInGameTimeX", 1.0f), GET_CONFIG_VALUE_GAME("screenScaleInGameTimeY", 1.0f)), GET_CONFIG_VALUE_GAME("maxLineLengthInGameTime", 0.5f),
		rendering::Color(GET_CONFIG_VALUE_GAME("colorInGameTimeRed", 1.0f), GET_CONFIG_VALUE_GAME("colorInGameTimeGreen", 0.0f), GET_CONFIG_VALUE_GAME("colorInGameTimeBlue", 0.0f)),
		rendering::Color(GET_CONFIG_VALUE_GAME("outlineColorInGameTimeRed", 0.0f), GET_CONFIG_VALUE_GAME("outlineColorInGameTimeGreen", 1.0f), GET_CONFIG_VALUE_GAME("outlineColorInGameTimeBlue", 0.0f)),
		math::Vector2D(GET_CONFIG_VALUE_GAME("offsetInGameTimeX", 0.005f), GET_CONFIG_VALUE_GAME("offsetInGameTimeY", 0.005f)), GET_CONFIG_VALUE_GAME("isCenteredInGameTime", false),
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

game::PlayGameState::~PlayGameState()
{
	for (auto cameraItr = m_cameras.begin(); cameraItr != m_cameras.end(); ++cameraItr)
	{
		SAFE_DELETE(*cameraItr);
	}
}

void game::PlayGameState::Entered()
{
	START_PROFILING_GAME(true, "");

	AddShaders();
	//AddTerrainNode();
	AddWaterNodes();
	AddSkyboxNode();
	//AddPlayerNode();
	//AddBillboards(GET_CONFIG_VALUE_GAME("billboardsTreeCount_1", 10), new Rendering::Material(m_gameManager->AddTexture(TextureIDs::BILLBOARD_TREE_1,
	//	GET_CONFIG_VALUE_STR_GAME("billboardTreeTexture_1", "Tree1.png")), 1.0f, 8.0f, m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_NORMAL_MAP),
	//	m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP)));
	//AddBillboards(GET_CONFIG_VALUE_GAME("billboardsTreeCount_2", 10), new Rendering::Material(m_gameManager->AddTexture(TextureIDs::BILLBOARD_TREE_2,
	//	GET_CONFIG_VALUE_STR_GAME("billboardTreeTexture_2", "Tree2.png")), 1.0f, 8.0f, m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_NORMAL_MAP),
	//	m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP)));
	//AddBillboards(GET_CONFIG_VALUE_GAME("billboardsTreeCount_3", 10), new Rendering::Material(m_gameManager->AddTexture(TextureIDs::BILLBOARD_TREE_3,
	//	GET_CONFIG_VALUE_STR_GAME("billboardTreeTexture_3", "Tree3.png")), 1.0f, 8.0f, m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_NORMAL_MAP),
	//	m_gameManager->GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP)));
	AddCameras();
	AddLights(); // Adding all kinds of light (directional, point, spot)

	engine::GameNode testMesh1;
	testMesh1.GetTransform().SetPos(GET_CONFIG_VALUE_GAME("testMesh1PosX", 42.0f), GET_CONFIG_VALUE_GAME("testMesh1PosY", 1.0f), GET_CONFIG_VALUE_GAME("testMesh1PosZ", 40.0f));
	testMesh1.GetTransform().SetRot(math::Quaternion(REAL_ZERO, sqrtf(2.0f) / 2, sqrtf(2.0f) / 2, REAL_ZERO));
	testMesh1.GetTransform().SetScale(0.1f);
	testMesh1.AddComponent(new engine::MeshRendererComponent(rendering::mesh_ids::SIMPLE_PLANE,
		new rendering::Material(m_gameManager->AddTexture(texture_ids::BRICKS, "bricks2.jpg"), 0.0f, 0,
			m_gameManager->AddTexture(texture_ids::BRICKS_NORMAL_MAP, "bricks2_normal.jpg"),
			m_gameManager->AddTexture(texture_ids::BRICKS_DISPLACEMENT_MAP, "bricks2_disp.jpg"), 0.04f, -1.0f)));
	engine::GameNode testMesh2;
	testMesh2.GetTransform().SetPos(GET_CONFIG_VALUE_GAME("testMesh2PosX", 8.0f), GET_CONFIG_VALUE_GAME("testMesh2PosY", 1.0f), GET_CONFIG_VALUE_GAME("testMesh2PosZ", 0.0f));
	testMesh2.GetTransform().SetRot(math::Quaternion(math::Matrix4D(math::Angle(90.0f), math::Angle(90.0f), math::Angle(0.0f))));
	testMesh2.AddComponent(new engine::MeshRendererComponent(rendering::mesh_ids::SIMPLE_PLANE,
		new rendering::Material(m_gameManager->GetTexture(texture_ids::BRICKS), 0.0f, 0,
			m_gameManager->GetTexture(rendering::texture_ids::DEFAULT_NORMAL_MAP),
			m_gameManager->GetTexture(rendering::texture_ids::DEFAULT_DISPLACEMENT_MAP))));
	m_nodes.push_back(std::move(testMesh2));
	m_nodes.push_back(std::move(testMesh1));
	m_nodes.back().AddChild(&m_nodes[0]);
	m_rootGameNode.AddChild(&m_nodes[1]);
	//Engine::GameNode* testMesh3 = new Engine::GameNode();
	//testMesh3->GetTransform().SetPos(-1.0f, 0.5f, 1.0f);
	//testMesh3->GetTransform().SetRot(math::Matrix4D(math::Angle(0.0f), math::Angle(0.0f), math::Angle(-180.0f)));
	//testMesh3->GetTransform().SetScale(0.25f);
	//testMesh3->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("plane.obj"), new Rendering::Material(m_textureFactory.GetTexture(TextureIDs::BRICKS), 0.0f, 0, m_textureFactory.GetTexture(TextureIDs::BRICKS_NORMAL_MAP), m_textureFactory.GetTexture(TextureIDs::BRICKS_DISPLACEMENT_MAP), 0.04f, -1.0f)));;
	//AddToSceneRoot(testMesh3);

	//const math::random::RandomGenerator& randomGenerator = math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::Generators::SIMPLE);
	//const int treeCount = 30;
	//for (int i = 0; i < treeCount; ++i)
	//{
	//	Engine::GameNode* treeNode = new Engine::GameNode();
	//	math::Real x = randomGenerator.NextFloat(0.0f, 30.0f);
	//	math::Real z = randomGenerator.NextFloat(0.0f, 20.0f);
	//	math::Real y = 0.0f;
	//	treeNode->GetTransform().SetPos(x, y, z);
	//	treeNode->GetTransform().SetRot(math::Quaternion(math::Matrix4D(math::Angle(0.0f), math::Angle(randomGenerator.NextFloat(0.0f, 180.0f)), math::Angle(0.0f))));
	//	treeNode->GetTransform().SetScale(0.01f);
	//	//treeNode->SetPhysicsObject(new Physics::PhysicsObject(treeNode->GetTransform(), 1282.0f, math::Vector3D(0.0f, 0.0f, 0.0f)));
	//	treeNode->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("lowPolyTree.obj"), new Rendering::Material(m_textureFactory.CreateTexture(TextureIDs::TREE, "lowPolyTree.png"), 1.0f, 8.0f, m_textureFactory.GetTexture(Rendering::TextureIDs::DEFAULT_NORMAL_MAP), m_textureFactory.GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP))));
	//	//treeNode->AddComponent(new Engine::GravityComponent(m_terrainMesh));
	//	AddToSceneRoot(treeNode);
	//}

	//const int boulderCount = 30;
	//for (int i = 0; i < boulderCount; ++i)
	//{
	//	Engine::GameNode* boulderNode = new Engine::GameNode();
	//	math::Real x = randomGenerator.NextFloat(0.0f, 100.0f);
	//	math::Real z = randomGenerator.NextFloat(0.0f, 100.0f);
	//	math::Real y = 0.0f;
	//	boulderNode->GetTransform().SetPos(x, y, z);
	//	boulderNode->GetTransform().SetRot(math::Quaternion(math::Matrix4D(math::Angle(0.0f), math::Angle(randomGenerator.NextFloat(0.0f, 180.0f)), math::Angle(0.0f))));
	//	boulderNode->GetTransform().SetScale(0.01f);
	//	//boulderNode->SetPhysicsObject(new Physics::PhysicsObject(boulderNode->GetTransform(), 1282.0f, math::Vector3D(0.0f, 0.0f, 0.0f)));
	//	boulderNode->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("boulder.obj"),
	//		new Rendering::Material(m_textureFactory.CreateTexture(TextureIDs::BOULDER, "boulder.png"), 0.01f, 22.0f, m_textureFactory.CreateTexture(TextureIDs::BOULDER_NORMAL_MAP, "boulderNormal.png"), m_textureFactory.GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP))));
	//	//boulderNode->AddComponent(new Engine::GravityComponent(m_terrainMesh));
	//	AddToSceneRoot(boulderNode);
	//}

	engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("PLAY game state has been placed in the game state manager");
	//tthread::thread t(GameManager::Load, GameManager::GetGameManager());
	CHECK_CONDITION_GAME(m_gameManager->IsGameLoaded(), utility::logging::ERR, "PLAY game state has been placed in the game state manager before loading the game.");
#ifdef ANT_TWEAK_BAR_ENABLED
	engine::CoreEngine::GetCoreEngine()->InitializeGameTweakBars();
#endif

#ifdef CALCULATE_STATS
	CoreEngine::GetCoreEngine()->StartSamplingSpf();
#endif
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::Leaving()
{
	engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("PLAY game state is about to be removed from the game state manager");
#ifdef CALCULATE_STATS
	Rendering::CoreEngine::GetCoreEngine()->StopSamplingSpf();
#endif
}

void game::PlayGameState::Obscuring()
{
	engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of PLAY game state");
}

void game::PlayGameState::Revealed()
{
	engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("PLAY game state has become the topmost game state in the game state manager's stack");
}

void game::PlayGameState::AddShaders()
{
	//m_gameManager->AddShader(Rendering::shader_ids::AMBIENT_TERRAIN, GET_CONFIG_VALUE_STR_GAME("ambientLightTerrainShader", "forward-ambient-terrain.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::AMBIENT_FOG_LINEAR_PLANE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogLinearPlaneBasedShader", "forward-ambient-fog-linear-plane-based.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogExponentialPlaneBasedShader", "forward-ambient-fog-exponential-plane-based.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::AMBIENT_FOG_LINEAR_RANGE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogLinearRangeBasedShader", "forward-ambient-fog-linear-range-based.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogExponentialRangeBasedShader", "forward-ambient-fog-exponential-range-based.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogLinearPlaneBasedTerrainShader", "forward-ambient-fog-linear-plane-based-terrain.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogExponentialPlaneBasedTerrainShader", "forward-ambient-fog-exponential-plane-based-terrain.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogLinearRangeBasedTerrainShader", "forward-ambient-fog-linear-range-based-terrain.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED, GET_CONFIG_VALUE_STR_GAME("ambientLightFogExponentialRangeBasedTerrainShader", "forward-ambient-fog-exponential-range-based-terrain.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::SHADOW_MAP, GET_CONFIG_VALUE_STR_GAME("shadowMapShader", "ShadowMapGenerator.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::SHADOW_MAP_CUBE, GET_CONFIG_VALUE_STR_GAME("cubeShadowMapShader", "CubeShadowMapGenerator.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::SKYBOX, GET_CONFIG_VALUE_STR_GAME("skyboxShader", "skybox-shader.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::SKYBOX_PROCEDURAL, GET_CONFIG_VALUE_STR_GAME("skyboxProceduralShader", "skybox-procedural-shader.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::WATER, GET_CONFIG_VALUE_STR_GAME("waterShader", "water-shader.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::WATER_NO_DIRECTIONAL_LIGHT, GET_CONFIG_VALUE_STR_GAME("waterNoDirectionalLightShader", "water-no-directional-light-shader.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::BILLBOARD, GET_CONFIG_VALUE_STR_GAME("billboardShader", "billboard-shader.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::DIRECTIONAL_LIGHT, GET_CONFIG_VALUE_STR_GAME("directionalLightShader", "forward-directional.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::DIRECTIONAL_LIGHT_TERRAIN, GET_CONFIG_VALUE_STR_GAME("directionalLightTerrainShader", "forward-directional-terrain.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::DIRECTIONAL_LIGHT_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("directionalLightNoShadowShader", "forward-directional-no-shadows.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("directionalLightNoShadowTerrainShader", "forward-directional-terrain-no-shadows.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::POINT_LIGHT, GET_CONFIG_VALUE_STR_GAME("pointLightShader", "forward-point.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::POINT_LIGHT_TERRAIN, GET_CONFIG_VALUE_STR_GAME("pointLightTerrainShader", "forward-point-terrain.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::POINT_LIGHT_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("pointLightNoShadowShader", "forward-point-no-shadows.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::POINT_LIGHT_TERRAIN_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("pointLightNoShadowTerrainShader", "forward-point-terrain-no-shadows.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::SPOT_LIGHT, GET_CONFIG_VALUE_STR_GAME("spotLightShader", "forward-spot.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::SPOT_LIGHT_TERRAIN, GET_CONFIG_VALUE_STR_GAME("spotLightTerrainShader", "forward-spot-terrain.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::SPOT_LIGHT_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("spotLightNoShadowShader", "forward-spot-no-shadows.glsl"));
	//m_gameManager->AddShader(Rendering::shader_ids::SPOT_LIGHT_TERRAIN_NO_SHADOWS, GET_CONFIG_VALUE_STR_GAME("spotLightNoShadowTerrainShader", "forward-spot-terrain-no-shadows.glsl"));
}

void game::PlayGameState::AddTerrainNode()
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Adding terrain node");
	//m_terrainMesh = new Rendering::TerrainMesh(0, 0, math::HeightsGenerator(0, 0, GET_CONFIG_VALUE_GAME("terrainVertexCount", 128), GET_CONFIG_VALUE_GAME("terrainHeightGeneratorAmplitude", 70.0f),
	//	GET_CONFIG_VALUE_GAME("terrainHeightGeneratorOctavesCount", 3), GET_CONFIG_VALUE_GAME("terrainHeightGeneratorRoughness", 0.3f)), GET_CONFIG_VALUE_GAME("terrainVertexCount", 128));
	m_terrainMaterial = new rendering::Material(m_gameManager->AddTexture(texture_ids::TERRAIN_DIFFUSE, GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture", "grass4.jpg")), GET_CONFIG_VALUE_GAME("defaultSpecularIntensity", 1.0f),
		GET_CONFIG_VALUE_GAME("defaultSpecularPower", 8.0f), m_gameManager->AddTexture(texture_ids::TERRAIN_NORMAL_MAP, GET_CONFIG_VALUE_STR_GAME("terrainNormalMap", "grass_normal.jpg")),
		m_gameManager->AddTexture(texture_ids::TERRAIN_DISPLACEMENT_MAP, GET_CONFIG_VALUE_STR_GAME("terrainDisplacementMap", "grass_disp.jpg")),
		GET_CONFIG_VALUE_GAME("defaultDisplacementScale", 0.02f), GET_CONFIG_VALUE_GAME("defaultDisplacementOffset", -0.5f));

	//m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	m_terrainMaterial->SetAdditionalTexture(m_gameManager->AddTexture(texture_ids::TERRAIN_BLEND_MAP, GET_CONFIG_VALUE_STR_GAME("terrainBlendMap", "terrainBlendMap.png")), "blendMap");
	m_terrainMaterial->SetAdditionalTexture(m_gameManager->AddTexture(texture_ids::TERRAIN_DIFFUSE_2, GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture2", "rocks2.jpg")), "diffuse2");
	m_terrainMaterial->SetAdditionalTexture(m_gameManager->AddTexture(texture_ids::TERRAIN_DIFFUSE_3, GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture3", "mud.png")), "diffuse3");
	m_terrainMaterial->SetAdditionalTexture(m_gameManager->AddTexture(texture_ids::TERRAIN_DIFFUSE_4, GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture4", "path.png")), "diffuse4");
	//m_resourcesLoaded += 1; // TODO: Consider creating some prettier solution. This is ugly
	//m_terrainNode.AddComponent(new Engine::MeshRendererComponent(m_terrainMesh, m_terrainMaterial));
	//m_terrainNode->GetTransform().SetPos(0.0f, 0.0f, 5.0f);
	//m_terrainNode->GetTransform().SetScale(20.0f);
	//m_terrain->TransformPositions(m_terrainNode.GetTransform().GetTransformation());
	//AddToSceneRoot(m_terrainNode); // Terrain node uses special shaders, so we don't actually add it to the game scene hierarchy. Instead we just register it for the renderer to use it.
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::AddWaterNodes()
{
	// It seems we have a problem with sharing resources. If I use the plane.obj (which I use in other entities) then we'll have problems with rendering (e.g. disappearing billboards).
	// If I change it to myPlane.obj which is not used in other entities the errors seem to be gone.
	m_waterNode.AddComponent(new engine::MeshRendererComponent(rendering::mesh_ids::SIMPLE_PLANE,
		nullptr /* The NULL material fixes the problem with rendering both billboards and water nodes simultaneously. TODO: But why / how? */));
	//m_resourcesLoaded += 2;
	m_waterNode.GetTransform().SetPos(GET_CONFIG_VALUE_GAME("waterNodePosX", -18.0f), GET_CONFIG_VALUE_GAME("waterNodePosY", 0.0f), GET_CONFIG_VALUE_GAME("waterNodePosZ", -12.0f));
	m_waterNode.GetTransform().SetScale(0.2f);
}

void game::PlayGameState::AddSkyboxNode()
{
	START_PROFILING_GAME(true, "");

	DEBUG_LOG_GAME("Creating a skybox");

	SkyboxBuilder skyboxBuilder(m_gameManager);
	utility::BuilderDirector<engine::GameNode> skyboxBuilderDirector(&skyboxBuilder);
	m_skyboxNode = skyboxBuilderDirector.Construct();
	NOTICE_LOG_GAME("The skybox has been created");
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::AddPlayerNode()
{
	const auto playerPositionX = GET_CONFIG_VALUE_GAME("playerPosition_X", 11.2f);
	const auto playerPositionZ = GET_CONFIG_VALUE_GAME("playerPosition_Z", 1.95f);
	const auto playerPositionY = 1.82f; // m_terrainMesh->GetHeightAt(math::Vector2D(playerPositionX, playerPositionZ));
	m_playerNode.GetTransform().SetPos(playerPositionX, playerPositionY, playerPositionZ);
	m_playerNode.GetTransform().SetScale(0.0005f);
	m_playerNode.CreatePhysicsObject(122.0f, math::Vector3D(0.0f, 0.0f, 0.0f));
	m_gameManager->AddMesh(mesh_ids::PLAYER, GET_CONFIG_VALUE_STR_GAME("playerMesh", "mike\\Mike.obj"));
	m_playerNode.AddComponent(new engine::MeshRendererComponent(mesh_ids::PLAYER, new rendering::Material(m_gameManager->AddTexture(texture_ids::PLAYER, "mike_d.tga"), 1.0f, 8.0f, m_gameManager->AddTexture(texture_ids::PLAYER_NORMAL_MAP, "mike_n.tga"), m_gameManager->GetTexture(rendering::texture_ids::DEFAULT_DISPLACEMENT_MAP))));
	m_playerNode.AddComponent(new engine::PhysicsComponent(2555.5f, 2855.2f)); //, 0.26f, 5.0f, math::Angle(152.0f, math::units::DEGREE), 0.015f, 0.0002f));
	m_playerNode.AddComponent(new engine::GravityComponent(m_terrain));
	//Rendering::Particles::ParticlesSystem particlesSystem = CreateParticlesSystem(Particleeffects::FOUNTAIN);
	//playerNode->AddComponent(new Engine::ParticlesSystemComponent(this, particlesSystem));
	//m_resourcesLoaded += 2;
	m_rootGameNode.AddChild(&m_playerNode);
}

void game::PlayGameState::AddBillboards(unsigned int billboardsCount, rendering::Material* billboardsMaterial)
{
	const auto& randomGenerator = math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE);
	const auto angle = REAL_ZERO;
	std::vector<math::Real> billboardsModelMatrices;
	billboardsModelMatrices.reserve(billboardsCount * math::Matrix4D::SIZE * math::Matrix4D::SIZE);
	for (auto i = 0; i < billboardsCount; ++i)
	{
		const auto x = randomGenerator.NextFloat(0.0f, 150.0f);
		const auto z = randomGenerator.NextFloat(0.0f, 150.0f);
		const auto y = 0.0f;

		math::Transform billboardTransform(math::Vector3D(x, y, z), math::Quaternion(math::Vector3D(0.0f, 1.0f, 0.0f), math::Angle(angle)), 0.5f);
		//angle += 15.0f;
		const auto billboardModelMatrix = billboardTransform.GetTransformation();

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
	engine::GameNode billboardsNode;
	//m_gameManager->AddMesh(mesh_ids::BILLBOARD, new Rendering::BillboardMesh(&billboardsModelMatrices[0], billboardsCount, math::Matrix4D::SIZE * math::Matrix4D::SIZE));
	billboardsNode.AddComponent(new engine::BillboardsRendererComponent(mesh_ids::BILLBOARD, billboardsMaterial));
	m_billboardsNodes.push_back(std::move(billboardsNode));
}

void game::PlayGameState::AddCameras()
{
	START_PROFILING_GAME(true, "");

	// TODO: temporary code begin.
	//m_cameras.push_back(new rendering::Camera(math::Vector3D(50.0f, 0.5f, 49.95f), math::NO_ROTATION_QUATERNION, math::Angle(70.0f), 1.7f, 0.1f, 1000.0f, 5.026f));
	//m_currentCameraIndex = 0;
	// TODO: temporary code end.

	rendering::PerspectiveCameraBuilder cameraBuilder;
	utility::BuilderDirector<rendering::Camera> cameraBuilderDirector(&cameraBuilder);
	const auto camerasCount = GET_CONFIG_VALUE_GAME("camerasCount", 3);
	for (auto i = 0; i < camerasCount; ++i)
	{
		const auto indexStr = std::to_string(i + 1);
		if (HAS_CONFIG_VALUE_GAME("cameraPosX_" + indexStr))
		{
			cameraBuilder.SetPosX(GET_CONFIG_VALUE_GAME("cameraPosX_" + indexStr, 0.0f));
		}
		if (HAS_CONFIG_VALUE_GAME("cameraPosY_" + indexStr))
		{
			cameraBuilder.SetPosY(GET_CONFIG_VALUE_GAME("cameraPosY_" + indexStr, 0.0f));
		}
		if (HAS_CONFIG_VALUE_GAME("cameraPosZ_" + indexStr))
		{
			cameraBuilder.SetPosZ(GET_CONFIG_VALUE_GAME("cameraPosZ_" + indexStr, 0.0f));
		}
		rendering::Camera camera = cameraBuilderDirector.Construct();
	}

	//CameraNodeBuilder cameraNodeBuilder(m_gameManager, &camera);
	//Utility::BuilderDirector<Engine::GameNode> cameraNodeBuilderDirector(&cameraNodeBuilder);
	//Engine::GameNode cameraNode = cameraNodeBuilderDirector.Construct();
	//for (int i = 0; i < cameraCount; ++i)
	//{
	//	cameraBuilder.SetCameraIndex(i);
	//	cameraBuilderDirector.Construct();
	//	if (m_cameras.empty())
	//	{
	//		camera.Activate();
	//		m_currentCameraIndex = 0;
	//	}
	//	m_cameras.push_back(camera);
	//	CRITICAL_LOG_GAME("You forgot to add camera node to scene root node.!!!");
	//	CRITICAL_LOG_GAME("There are no camera components in the application anymore!!!");

	//	//cameraNodeBuilder.SetCameraIndex(i);
	//	//cameraNodeBuilder.SetGameNodeToFollow(&m_playerNode);
	//	//cameraNodeBuilderDirector.Construct();
	//	//m_rootGameNode.AddChild(&cameraNode);
	//}
	//NOTICE_LOG_GAME(cameraCount, " camera(-s) created");
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::AddLights()
{
	START_PROFILING_GAME(true, "");
	AddDirectionalLight(); // Adding directional light (if enabled)
	AddPointLights();
	AddSpotLights();
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::AddDirectionalLight()
{
	// TODO: For now we only check if directionalLightsCount is zero or not.
	// In the future there might be many directional lights enabled (?)
	const auto directionalLightsCount = GET_CONFIG_VALUE_GAME("directionalLightsCount", 1);
	if (directionalLightsCount == 0)
	{
		NOTICE_LOG_GAME("Directional lights disabled");
		return;
	}
	NOTICE_LOG_GAME("Directional lights enabled");

	rendering::lighting::DirectionalLightBuilder directionalLightBuilder;
	utility::BuilderDirector<rendering::lighting::DirectionalLight> lightBuilderDirector(&directionalLightBuilder);
	auto directionalLight = lightBuilderDirector.Construct();
	INFO_LOG_RENDERING("Directional light with intensity = ", directionalLight.GetIntensity(), " is being added to directional / spot lights vector");
	++m_directionalLightsCount;
	m_directionalLights.push_back(std::move(directionalLight));
	//m_lights.push_back(directionalLight);
}

void game::PlayGameState::AddPointLights()
{
	const auto pointLightsCount = GET_CONFIG_VALUE_GAME("pointLightsCount", 1);
	if (pointLightsCount > 0)
	{
		DEBUG_LOG_GAME("Creating ", pointLightsCount, " point lights");
		rendering::lighting::PointLightBuilder pointLightBuilder;
		utility::BuilderDirector<rendering::lighting::PointLight> lightBuilderDirector(&pointLightBuilder);
		for (auto i = 0; i < pointLightsCount; ++i)
		{
			auto pointLight = lightBuilderDirector.Construct();
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

void game::PlayGameState::AddSpotLights()
{
	const auto spotLightsCount = GET_CONFIG_VALUE_GAME("spotLightsCount", 1);
	if (spotLightsCount > 0)
	{
		DEBUG_LOG_GAME("Creating ", spotLightsCount, " spot lights");
		rendering::lighting::SpotLightBuilder spotLightBuilder;
		utility::BuilderDirector<rendering::lighting::SpotLight> lightBuilderDirector(&spotLightBuilder);
		for (auto i = 0; i < spotLightsCount; ++i)
		{
			m_spotLights.push_back(std::move(lightBuilderDirector.Construct()));
			//m_lights.push_back(spotLight);
		}
		NOTICE_LOG_GAME(spotLightsCount, " spot lights created");
	}
	else
	{
		NOTICE_LOG_GAME("Spot lights disabled");
	}
}

void game::PlayGameState::Handle(engine::actions::Action action)
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Handling action: ", action);
	switch (action)
	{
	case engine::actions::SHOW_PLAY_MENU:
		m_gameManager->SetTransition(new engine::game_state_transitioning::GameStateTransition(m_gameManager->GetPlayMainMenuGameState(), engine::game_state_transitioning::PUSH, engine::game_state_modality::EXCLUSIVE));
		break;
	case engine::actions::MOVE_CAMERA_UP:
		m_cameras[m_currentCameraIndex]->GetPos().y += 0.15f;
		CRITICAL_LOG_GAME("Moving up... Current position: ", m_cameras[m_currentCameraIndex]->GetPos());
		break;
	case engine::actions::MOVE_CAMERA_DOWN:
		m_cameras[m_currentCameraIndex]->GetPos().y -= 0.15f;
		CRITICAL_LOG_GAME("Moving down... Current position: ", m_cameras[m_currentCameraIndex]->GetPos());
		break;
	case engine::actions::MOVE_CAMERA_LEFT:
		m_cameras[m_currentCameraIndex]->GetPos().x -= 0.15f;
		CRITICAL_LOG_GAME("Moving left... Current position: ", m_cameras[m_currentCameraIndex]->GetPos());
		break;
	case engine::actions::MOVE_CAMERA_RIGHT:
		m_cameras[m_currentCameraIndex]->GetPos().x += 0.15f;
		CRITICAL_LOG_GAME("Moving right... Current position: ", m_cameras[m_currentCameraIndex]->GetPos());
		break;
	case engine::actions::MOVE_CAMERA_FORWARD:
		m_cameras[m_currentCameraIndex]->GetPos().z += 0.15f;
		CRITICAL_LOG_GAME("Moving forward... Current position: ", m_cameras[m_currentCameraIndex]->GetPos());
		break;
	case engine::actions::MOVE_CAMERA_BACKWARD:
		m_cameras[m_currentCameraIndex]->GetPos().z -= 0.15f;
		CRITICAL_LOG_GAME("Moving backward... Current position: ", m_cameras[m_currentCameraIndex]->GetPos());
		break;
	default:
		WARNING_LOG_GAME("Action ", action, " is not supported by the PLAY game state.");
	}
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::Handle(engine::states::State state)
{
	//DELOCUST_LOG_GAME("Handling the state ", state);
	switch (state)
	{
	case engine::states::MOUSE_KEY_LEFT_PRESSED:
		m_isMouseLocked = true;
		DEBUG_LOG_GAME("Mouse left key pressed");
		break;
	case engine::states::MOUSE_KEY_MIDDLE_PRESSED:
		DEBUG_LOG_GAME("Mouse middle key pressed");
		break;
	case engine::states::MOUSE_KEY_RIGHT_PRESSED:
		DEBUG_LOG_GAME("Mouse right key pressed");
		break;
	case engine::states::MOVE_CAMERA_UP:
		m_cameras[m_currentCameraIndex]->GetPos().y += 0.01f;
		//CRITICAL_LOG_GAME("Moving up... Current position: " + m_cameras[m_currentCameraIndex].GetPos().ToString());
		break;
	case engine::states::MOVE_CAMERA_DOWN:
		m_cameras[m_currentCameraIndex]->GetPos().y -= 0.01f;
		//CRITICAL_LOG_GAME("Moving down... Current position: " + m_cameras[m_currentCameraIndex].GetPos().ToString());
		break;
	case engine::states::MOVE_CAMERA_LEFT:
		m_cameras[m_currentCameraIndex]->GetPos().x -= 0.01f;
		break;
	case engine::states::MOVE_CAMERA_RIGHT:
		m_cameras[m_currentCameraIndex]->GetPos().x += 0.01f;
		break;
	case engine::states::MOVE_CAMERA_FORWARD:
		m_cameras[m_currentCameraIndex]->GetPos().z += 0.01f;
		break;
	case engine::states::MOVE_CAMERA_BACKWARD:
		m_cameras[m_currentCameraIndex]->GetPos().z -= 0.01f;
		break;
	default:
		DEBUG_LOG_GAME("The state ", state, " is not supported by the MenuGameState");
		break;
	}
}

void game::PlayGameState::Handle(engine::ranges::Range range, math::Real value)
{
	switch (range)
	{
	case engine::ranges::AXIS_X:
		m_previousMousePos.x = m_mousePos.x;
		m_mousePos.x = value;
		m_mousePosChanged = true;
		if (m_isMouseLocked)
		{
			m_cameras[m_currentCameraIndex]->Rotate(math::Vector3D(0, 1, 0),
				math::Angle(500.0f * (m_mousePos.x - m_previousMousePos.x) * m_cameras[m_currentCameraIndex]->GetSensitivity()));
			//m_gameManager->CentralizeCursor(); // TODO: Rotation ceases to work. But why?
			m_isMouseLocked = false;
		}
		DEBUG_LOG_GAME("Mouse pos = ", m_mousePos);
		break;
	case engine::ranges::AXIS_Y:
		m_previousMousePos.y = m_mousePos.y;
		m_mousePos.y = value;
		m_mousePosChanged = true;
		if (m_isMouseLocked)
		{
			m_cameras[m_currentCameraIndex]->Rotate(m_cameras[m_currentCameraIndex]->GetRot().GetRight(),
				math::Angle((m_mousePos.y - m_previousMousePos.y) * m_cameras[m_currentCameraIndex]->GetSensitivity()));
			//m_gameManager->CentralizeCursor(); // TODO: Rotation ceases to work. But why?
			m_isMouseLocked = false;
		}
		DEBUG_LOG_GAME("Mouse pos = ", m_mousePos);
		break;
	default:
		DEBUG_LOG_GAME("The range ", range, " is not supported by the PlayGameState");
		break;
	}
}

void game::PlayGameState::MouseButtonEvent(int button, int action, int mods)
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

void game::PlayGameState::MousePosEvent(double xPos, double yPos)
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
	//math::Vector2D centerPosition(static_cast<math::Real>(width) / 2, static_cast<math::Real>(height) / 2);
	//math::Vector2D deltaPosition(static_cast<math::Real>(xPos), static_cast<math::Real>(yPos));
	//deltaPosition -= centerPosition;
	//
	//bool rotX = ! math::AlmostEqual(deltaPosition.GetX(), REAL_ZERO);
	//bool rotY = ! math::AlmostEqual(deltaPosition.GetY(), REAL_ZERO);

	//if (rotX || rotY)
	//{
	//	Rendering::Transform& transform = Rendering::CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera().GetTransform();
	//	const math::Real sensitivity = static_cast<math::Real>(Rendering::Camera::GetSensitivity());
	//	if (rotX)
	//	{
	//		transform.Rotate(math::Vector3D(0, 1, 0), math::Angle(deltaPosition.GetX() * sensitivity));
	//	}
	//	if (rotY)
	//	{
	//		transform.Rotate(transform.GetRot().GetRight(), math::Angle(deltaPosition.GetY() * sensitivity));
	//	}
	//	Rendering::CoreEngine::GetCoreEngine()->CentralizeCursor();
	//}
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::ScrollEvent(double xOffset, double yOffset)
{
	START_PROFILING_GAME(true, "");
	m_rootGameNode.ScrollEvent(xOffset, yOffset);
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::Render(rendering::Renderer* renderer) const
{
	// TODO: Updating the state of the rendering engine (e.g. the values of some of its member variables)
	// in this function is not good. This should be done in the Update function (or maybe not?).
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_EXIT_GAME(renderer != nullptr, utility::logging::CRITICAL, "Cannot render the game. The rendering engine is NULL.");
	DEBUG_LOG_GAME("PLAY game state rendering");

	renderer->InitRenderScene(m_ambientLightColor, m_dayNightMixFactor);
	renderer->SetCurrentCamera(m_cameras[m_currentCameraIndex]);

	//RenderWaterTextures(renderer);

	renderer->BindDisplayTexture();
	renderer->ClearScreen();

	RenderSceneWithAmbientLight(renderer);
	//m_rootGameNode.Render(shader, renderer);
	//RenderSceneWithPointLights(renderer); // Point light rendering
	RenderSceneWithDirectionalAndSpotLights(renderer); // Directional and spot light rendering

	//RenderWaterNodes(renderer);

	RenderBillboardNodes(renderer);

	RenderSkybox(renderer);

	//RenderParticles(renderer);

#ifdef DEBUG_RENDERING_ENABLED
	renderer->RenderDebugGuiControls(m_gameManager->GetShader(rendering::shader_ids::GUI));
#endif

	renderer->FinalizeRenderScene(renderer->GetAntiAliasingMethod() == rendering::aliasing::FXAA ? rendering::shader_ids::FILTER_FXAA : rendering::shader_ids::FILTER_NULL);

#ifdef DRAW_GAME_TIME
	m_inGameTimeGuiButton.SetText(m_inGameDateTime.ToString());
	renderer->RenderGuiControl(m_inGameTimeGuiButton, rendering::shader_ids::TEXT);
#endif

	STOP_PROFILING_GAME("");
}

void game::PlayGameState::RenderSceneWithAmbientLight(rendering::Renderer* renderer) const
{
	const auto ambientShaderId = GetAmbientShaderId(renderer->GetFogInfo());
	renderer->BindShader(ambientShaderId);
	renderer->UpdateRendererUniforms(ambientShaderId);
	m_rootGameNode.Render(ambientShaderId, renderer);
	CHECK_CONDITION_GAME(m_gameManager->GetTerrainNode() != nullptr, utility::logging::ERR, "Cannot render terrain. There are no terrain nodes registered.");
	const auto ambientTerrainShaderId = GetAmbientTerrainShaderId(renderer->GetFogInfo());
	renderer->BindShader(ambientTerrainShaderId);
	renderer->UpdateRendererUniforms(ambientTerrainShaderId);
	m_terrainNode.Render(ambientTerrainShaderId, renderer);
}

void game::PlayGameState::RenderSceneWithPointLights(rendering::Renderer* renderer) const
{
	for (size_t i = 0; i < m_pointLights.size(); ++i)
	{
		const auto currentPointLight = renderer->SetCurrentPointLight(&m_pointLights[i]);
		if (currentPointLight->IsEnabled())
		{
			DEBUG_LOG_GAME("Point light at index ", i, " is disabled");
			continue;
		}
		m_rootGameNode.Render(currentPointLight->GetShaderId(), renderer);
		m_terrainNode.Render(currentPointLight->GetTerrainShaderId(), renderer);
	}
}

void game::PlayGameState::RenderSceneWithDirectionalAndSpotLights(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	for (auto lightItr = m_directionalLights.begin(); lightItr != m_directionalLights.end(); ++lightItr)
	{
		if (lightItr->IsEnabled())
		{
			const auto currentLight = renderer->SetCurrentLight(&*lightItr);
			if (renderer->InitShadowMap())
			{
				// Render scene using shadow mapping shader
				const auto shadowMapShaderId = rendering::shader_ids::SHADOW_MAP;
				renderer->BindShader(shadowMapShaderId);
				renderer->UpdateRendererUniforms(shadowMapShaderId);
				m_rootGameNode.Render(shadowMapShaderId, renderer);
				m_terrainNode.Render(shadowMapShaderId, renderer); // TODO: Probably unnecessary
				renderer->FinalizeShadowMapRendering(rendering::shader_ids::FILTER_GAUSSIAN_BLUR);
			}

			renderer->InitLightRendering();
			// TODO: Render scene with light is not ready. Check the function Renderer::RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode, bool isCastingShadowsEnabled /* = true */).
			renderer->BindShader(currentLight->GetShaderId());
			renderer->UpdateRendererUniforms(currentLight->GetShaderId());
			m_rootGameNode.Render(currentLight->GetShaderId(), renderer);
			renderer->BindShader(currentLight->GetTerrainShaderId());
			renderer->UpdateRendererUniforms(currentLight->GetTerrainShaderId());
			m_terrainNode.Render(currentLight->GetTerrainShaderId(), renderer);
			renderer->FinalizeLightRendering();
		}
	}
	for (auto lightItr = m_spotLights.begin(); lightItr != m_spotLights.end(); ++lightItr)
	{
		if (lightItr->IsEnabled())
		{
			const auto currentLight = renderer->SetCurrentLight(&*lightItr);
			if (renderer->InitShadowMap())
			{
				// Render scene using shadow mapping shader
				const auto shadowMapShaderId = rendering::shader_ids::SHADOW_MAP;
				renderer->BindShader(shadowMapShaderId);
				renderer->UpdateRendererUniforms(shadowMapShaderId);
				m_rootGameNode.Render(shadowMapShaderId, renderer);
				m_terrainNode.Render(shadowMapShaderId, renderer); // TODO: Probably unnecessary
				renderer->FinalizeShadowMapRendering(rendering::shader_ids::FILTER_GAUSSIAN_BLUR);
			}

			renderer->InitLightRendering();
			// TODO: Render scene with light is not ready. Check the function Renderer::RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode, bool isCastingShadowsEnabled /* = true */).
			renderer->BindShader(currentLight->GetShaderId());
			renderer->UpdateRendererUniforms(currentLight->GetShaderId());
			m_rootGameNode.Render(currentLight->GetShaderId(), renderer);
			renderer->BindShader(currentLight->GetTerrainShaderId());
			renderer->UpdateRendererUniforms(currentLight->GetTerrainShaderId());
			m_terrainNode.Render(currentLight->GetTerrainShaderId(), renderer);
			renderer->FinalizeLightRendering();
		}
	}
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::RenderSkybox(rendering::Renderer* renderer) const
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
	const auto skyboxShaderId = rendering::shader_ids::SKYBOX;
	renderer->BindShader(skyboxShaderId);
	renderer->UpdateRendererUniforms(skyboxShaderId);
	m_skyboxNode.Render(skyboxShaderId, renderer);
	renderer->SetDepthFuncDefault();
	renderer->SetCullFaceDefault();
	//glEnable(GL_DEPTH_TEST);
	//Rendering::CheckErrorCode("Renderer::Render", "Rendering skybox");

	DEBUG_LOG_GAME("Skybox rendering finished");
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::RenderWaterTextures(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != nullptr, utility::logging::DEBUG, "There are no water nodes registered in the rendering engine");
	// TODO: For now we only support one water node (you can see that in the "distance" calculation). In the future there might be more.

	RenderWaterReflectionTexture(renderer);
	RenderWaterRefractionTexture(renderer);

	renderer->DisableClippingPlanes();
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::RenderWaterReflectionTexture(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != nullptr, utility::logging::DEBUG, "There are no water nodes registered in the rendering engine");
	EMERGENCY_LOG_GAME("Rendering water reflection texture doesn't work now. Reflection camera is not properly setup.");
	return;

	// TODO: The camera should be accessible from the game manager. It shouldn't be necessary to access them via rendering engine.
	const auto& currentCameraCopy(renderer->GetCurrentCamera());
	//Rendering::BaseCamera reflectionCamera(currentCameraCopy);
	//const math::Real cameraHeight = reflectionCamera.GetPos().GetY();
	//math::Real distance = 2.0f * (cameraHeight - m_waterNode.GetTransform().GetTransformedPos().GetY());
	//reflectionCamera.GetPos().SetY(cameraHeight - distance); // TODO: Uncomment. use m_altCamera instead of the main camera.
	//reflectionCamera.GetRot().InvertPitch(); // TODO: Uncomment.

	renderer->EnableWaterReflectionClippingPlane(-m_waterNode.GetTransform().GetTransformedPos().y + 0.1f /* we add 0.1f to remove some glitches on the water surface */);
	renderer->BindWaterReflectionTexture();
	renderer->ClearScreen(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE);

	renderer->SetDepthTest(false);
	//renderer->SetCurrentCamera(&reflectionCamera);
	RenderSkybox(renderer);
	RenderSceneWithAmbientLight(renderer);
	//renderer->SetCurrentCamera(&currentCameraCopy);

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

	//if (Rendering::antiAliasingMethod == Rendering::aliasing::FXAA)
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_FXAA), m_waterReflectionTexture, nullptr);
	//}
	//else
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_NULL), m_waterReflectionTexture, nullptr);
	//}

	//BindAsRenderTarget();

	//m_cameras[m_currentCameraIndex].GetPos().SetY(cameraHeight); // TODO: use m_altCamera instead of the main camera.
	//m_cameras[m_currentCameraIndex].GetRot().InvertPitch();

	STOP_PROFILING_GAME("");
}

void game::PlayGameState::RenderWaterRefractionTexture(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != nullptr, utility::logging::DEBUG, "There are no water nodes registered in the rendering engine");

	renderer->EnableWaterRefractionClippingPlane(m_waterNode.GetTransform().GetTransformedPos().y);
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

	//if (Rendering::antiAliasingMethod == Rendering::aliasing::FXAA)
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_FXAA), m_waterReflectionTexture, nullptr);
	//}
	//else
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_NULL), m_waterReflectionTexture, nullptr);
	//}

	//BindAsRenderTarget();

	STOP_PROFILING_GAME("");
}

void game::PlayGameState::RenderWaterNodes(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	// TODO: Add some condition here that will prevent any water nodes rendering if there are no water nodes available.
	//CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_waterNode != nullptr, utility::logging::DEBUG, "There are no water nodes registered in the rendering engine");
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
	const auto waterShaderId = GetWaterShaderId(renderer);
	renderer->BindShader(waterShaderId);
	renderer->UpdateRendererUniforms(waterShaderId);
	m_waterNode.Render(waterShaderId, renderer);
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::RenderBillboardNodes(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Rendering billboards started");
	//renderer->SetDepthTest(false);
	renderer->SetBlendingEnabled(true);
	//GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
	// GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE,
	// GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR, GL_SRC1_ALPHA, and GL_ONE_MINUS_SRC1_ALPHA
	renderer->SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	const auto billboardShaderId = rendering::shader_ids::BILLBOARD;
	renderer->BindShader(billboardShaderId);
	renderer->UpdateRendererUniforms(billboardShaderId);
	for (auto billboardsNodeItr = m_billboardsNodes.begin(); billboardsNodeItr != m_billboardsNodes.end(); ++billboardsNodeItr)
	{
		billboardsNodeItr->Render(billboardShaderId, renderer);
	}
	//renderer->SetDepthTest(true);
	renderer->SetBlendingEnabled(false);
	//renderer->SetBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::RenderParticles(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Rendering particles started");
	for (auto particleSystemItr = m_gameManager->GetParticlesSystems().begin(); particleSystemItr != m_gameManager->GetParticlesSystems().end(); ++particleSystemItr)
	{
		//if (!(*particleSystemItr)->GetTexture()->IsAdditive())
		//{
			//(*particleSystemItr)->SortParticles(renderer->GetCurrentCamera().GetPos());
		//}
		renderer->RenderParticles(**particleSystemItr);
	}
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::Update(math::Real elapsedTime)
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("PLAY game state updating");
	m_rootGameNode.Update(elapsedTime);
	m_skyboxNode.Update(elapsedTime);
	m_skyboxNode.GetTransform().SetPos(m_cameras[m_currentCameraIndex]->GetPos()); // TODO: Instead, skyboxNode should have a simple component that does exactly that!

	//EMERGENCY_LOG_GAME("Elapsed time: ", elapsedTime * 1000.0f, " [ms]");
	m_inGameDateTime += utility::timing::TimeSpan(elapsedTime * m_clockSpeed * 1000000.0f, utility::timing::MICROSECOND);
	//CRITICAL_LOG_GAME("Clock speed = ", m_clockSpeed, ". ElapsedTime = ", elapsedTime, ". In-game time: ", m_inGameDateTime.ToString());

	//AdjustAmbientLightAccordingToCurrentTime();

	CalculateSunElevationAndAzimuth();
	DEBUG_LOG_GAME("PLAY game state updated");
	STOP_PROFILING_GAME("");
}

void game::PlayGameState::CalculateSunElevationAndAzimuth()
{
	//const int timeGMTdifference = 1;

	//const math::Angle b(0.9863014f * (m_inGameDateTime->GetDayInYear() - 81)); // 0,98630136986301369863013698630137 = 360 / 365
	//const math::Real bSin = b.Sin();
	//const math::Real bCos = b.Cos();

	//const math::Real equationOfTime = 19.74f * bSin * bCos - 7.53f * bCos - 1.5f * bSin; // EoT
	//const math::Real declinationSin = TROPIC_OF_CANCER_SINUS * bSin;
	//const math::Angle declinationAngle(asin(declinationSin), math::units::RADIAN);
	////DEBUG_LOG_ENGINE("Declination in degrees = ", declinationAngle.GetAngleInDegrees());

	//const math::Real timeCorrectionInSeconds = 60.0f * (4.0f * (m_longitude.GetAngleInDegrees() - 15.0f * timeGMTdifference) + equationOfTime);
	//const math::Real localSolarTime = m_inGameDateTime->GetDayTime() + timeCorrectionInSeconds;
	////DEBUG_LOG_ENGINE("Time correction in seconds = ", timeCorrectionInSeconds);
	////DEBUG_LOG_ENGINE("Local time = ", m_timeOfDay, "\tLocal solar time = ", localSolarTime);

	//const math::Angle hourAngle(15.0f * (localSolarTime - 12 * Utility::timing::DateTime::SECONDS_PER_HOUR) / Utility::timing::DateTime::SECONDS_PER_HOUR);
	////DEBUG_LOG_ENGINE("Hour angle = ", hourAngle.GetAngleInDegrees());

	//const math::Real sunElevationSin = declinationSin * m_latitude.Sin() + declinationAngle.Cos() * m_latitude.Cos() * hourAngle.Cos();
	//m_sunElevation.SetAngleInRadians(asin(sunElevationSin));
	////DEBUG_LOG_ENGINE("Sun elevation = ", m_sunElevation.GetAngleInDegrees());

	//const math::Real sunAzimuthCos = ((declinationSin * m_latitude.Cos()) - (declinationAngle.Cos() * m_latitude.Sin() * hourAngle.Cos())) / m_sunElevation.Cos();
	//m_sunAzimuth.SetAngleInRadians(acos(sunAzimuthCos));
	//bool isAfternoon = (localSolarTime > 12.0f * Utility::timing::DateTime::SECONDS_PER_HOUR) || (hourAngle.GetAngleInDegrees() > REAL_ZERO);
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
//	case Utility::timing::NIGHT:
//		dayNightMixFactor = REAL_ZERO;
//		m_ambientLightColor = m_ambientNighttimeColor;
//		break;
//	case Utility::timing::BEFORE_DAWN:
//		dayNightMixFactor = REAL_ZERO;
//		m_ambientLightColor = m_ambientNighttimeColor.Lerp(m_ambientSunNearHorizonColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	case Utility::timing::SUNRISE:
//		dayNightMixFactor = dayTimeTransitionFactor;
//		m_ambientLightColor = m_ambientSunNearHorizonColor.Lerp(m_ambientDaytimeColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	case Utility::timing::DAY:
//		dayNightMixFactor = REAL_ONE;
//		m_ambientLightColor = m_ambientDaytimeColor;
//		break;
//	case Utility::timing::SUNSET:
//		dayNightMixFactor = dayTimeTransitionFactor;
//		m_ambientLightColor = m_ambientSunNearHorizonColor.Lerp(m_ambientDaytimeColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	case Utility::timing::AFTER_DUSK:
//		dayNightMixFactor = REAL_ZERO;
//		m_ambientLightColor = m_ambientNighttimeColor.Lerp(m_ambientSunNearHorizonColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	}
//	STOP_PROFILING_GAME("");
//}

int game::PlayGameState::GetAmbientShaderId(const rendering::fog_effect::FogInfo& fogInfo) const
{
	START_PROFILING_ENGINE(true, "");
	auto shaderId = rendering::shader_ids::AMBIENT;
	if (fogInfo.IsEnabled())
	{
		//DEBUG_LOG_RENDERING("Fog fall-off type: ", m_fogFallOffType, ". Fog distance calculation type: ", m_fogCalculationType);
		// TODO: A very ugly way. If we decide to add more fog fall off or calculation types then we will surely have a big problem in here.
		switch (fogInfo.GetFallOffType())
		{
		case rendering::fog_effect::LINEAR:
			switch (fogInfo.GetCalculationType())
			{
			case rendering::fog_effect::PLANE_BASED:
				shaderId = rendering::shader_ids::AMBIENT_FOG_LINEAR_PLANE_BASED;
				break;
			case rendering::fog_effect::RANGE_BASED:
				shaderId = rendering::shader_ids::AMBIENT_FOG_LINEAR_RANGE_BASED;
				break;
			default:
				ERROR_LOG_GAME("Unknown fog calculation type specified: ", fogInfo.GetCalculationType());
			}
			break;
		case rendering::fog_effect::EXPONENTIAL:
			switch (fogInfo.GetCalculationType())
			{
			case rendering::fog_effect::PLANE_BASED:
				shaderId = rendering::shader_ids::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED;
				break;
			case rendering::fog_effect::RANGE_BASED:
				shaderId = rendering::shader_ids::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED;
			default:
				ERROR_LOG_GAME("Unknown fog calculation type specified: ", fogInfo.GetCalculationType());
			}
			break;
		default:
			ERROR_LOG_GAME("Unknown fog fall-off type specified: ", fogInfo.GetFallOffType());
		}
	}
	STOP_PROFILING_ENGINE("");
	return shaderId;
}

int game::PlayGameState::GetAmbientTerrainShaderId(const rendering::fog_effect::FogInfo& fogInfo) const
{
	START_PROFILING_ENGINE(true, "");
	auto shaderId = rendering::shader_ids::AMBIENT_TERRAIN;
	if (fogInfo.IsEnabled())
	{
		//DEBUG_LOG_RENDERING("Fog fall-off type: ", m_fogFallOffType, ". Fog distance calculation type: ", m_fogCalculationType);
		// TODO: A very ugly way. If we decide to add more fog fall off or calculation types then we will surely have a big problem in here.
		switch (fogInfo.GetFallOffType())
		{
		case rendering::fog_effect::LINEAR:
			switch (fogInfo.GetCalculationType())
			{
			case rendering::fog_effect::PLANE_BASED:
				shaderId = rendering::shader_ids::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED;
				break;
			case rendering::fog_effect::RANGE_BASED:
				shaderId = rendering::shader_ids::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED;
				break;
			default:
				ERROR_LOG_GAME("Unknown fog calculation type specified: ", fogInfo.GetCalculationType());
			}
			break;
		case rendering::fog_effect::EXPONENTIAL:
			switch (fogInfo.GetCalculationType())
			{
			case rendering::fog_effect::PLANE_BASED:
				shaderId = rendering::shader_ids::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED;
				break;
			case rendering::fog_effect::RANGE_BASED:
				shaderId = rendering::shader_ids::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED;
			default:
				ERROR_LOG_GAME("Unknown fog calculation type specified: ", fogInfo.GetCalculationType());
			}
			break;
		default:
			ERROR_LOG_GAME("Unknown fog fall-off type specified: ", fogInfo.GetFallOffType());
		}
	}
	STOP_PROFILING_ENGINE("");
	return shaderId;
}

unsigned int game::PlayGameState::SetCurrentCamera(unsigned int cameraIndex)
{
	CHECK_CONDITION_RENDERING((cameraIndex >= 0) && (cameraIndex < m_cameras.size()), utility::logging::ERR, "Incorrect current camera index. Passed ",
		cameraIndex, " when the correct range is (", 0, ", ", m_cameras.size(), ").");
	//m_cameras[m_currentCameraIndex].Deactivate();
	//m_currentCameraIndex = cameraIndex;
	//m_cameras[m_currentCameraIndex].Activate();
#ifndef ANT_TWEAK_BAR_ENABLED
	NOTICE_LOG_RENDERING("Switched to camera #", m_currentCameraIndex + 1);
	//DEBUG_LOG_RENDERING("Current camera parameters: ", m_cameras[m_currentCameraIndex]->ToString());
#endif
	//return m_currentCameraIndex;
	return 0;
}

unsigned int game::PlayGameState::NextCamera()
{
	//if (m_currentCameraIndex == static_cast<int>(m_cameras.size()) - 1)
	//{
	//	m_currentCameraIndex = -1;
	//}
	//return SetCurrentCamera(m_currentCameraIndex + 1);
	return 0;
}

unsigned int game::PlayGameState::PrevCamera()
{
	//if (m_currentCameraIndex == 0)
	//{
	//	m_currentCameraIndex = m_cameras.size();
	//}
	//return SetCurrentCamera(m_currentCameraIndex - 1);
	return 0;
}