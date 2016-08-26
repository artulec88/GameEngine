#include "TestGameManager.h"
#include "Def.h"
#include "MenuGameState.h"
#include "IntroGameState.h"
#include "PlayGameState.h"
#include "PlayMenuGameState.h"
#include "LoadGameState.h"

#include "Engine\GameNodeBuilder.h"
#include "Engine\CoreEngine.h"
#include "Engine\BillboardRendererComponent.h"
#include "Engine\MeshRendererComponent.h"
#include "Engine\PhysicsComponent.h"
#include "Engine\LookAtComponent.h"
#include "Engine\GravityComponent.h"
#include "Engine\ParticleGeneratorComponent.h"

#include "Rendering\Color.h"
#include "Rendering\Camera.h"

#include "Math\FloatingPoint.h"
#include "Math\Quaternion.h"
#include "Math\HeightsGenerator.h"
#include "Math\RandomGeneratorFactory.h"
//#include "Math\Vector.h"

#include "Physics\PhysicsObject.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\BuilderDirector.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

#include <sstream>

Game::TestGameManager::TestGameManager() :
	GameManager(),
	RESOURCES_TO_LOAD(26),
	CAMERA_HEIGHT_UPDATE_INTERVAL(GET_CONFIG_VALUE_GAME("cameraHeightUpdateInterval", 0.01f)),
	m_resourcesLoaded(0),
	m_introGameState(nullptr),
	m_menuGameState(nullptr),
	m_loadGameState(nullptr),
	m_playGameState(nullptr),
	m_playMainMenuGameState(nullptr),
	m_startGameCommand(),
	m_showIntroGameCommand(),
	m_resumeGameCommand(),
	m_saveGameCommand(),
	m_loadGameCommand(),
	m_terrainMesh(NULL),
	m_timeToUpdateCameraHeight(REAL_ZERO),
	m_boxNode(NULL),
#ifdef ANT_TWEAK_BAR_ENABLED 
	terrainMaterial(NULL),
	boxMaterial(NULL),
	terrainSpecularIntensity(GET_CONFIG_VALUE_GAME("defaultSpecularIntensity", 1.0f)),
	terrainSpecularPower(GET_CONFIG_VALUE_GAME("defaultSpecularPower", 8.0f)),
	terrainDisplacementScale(GET_CONFIG_VALUE_GAME("defaultDisplacementScale", 0.02f)),
	terrainDisplacementOffset(GET_CONFIG_VALUE_GAME("defaultDisplacementOffset", -0.5f)),
#endif
	HUMAN_NODES_COUNT(2),
	humanNodes(NULL),
	cameraCount(GET_CONFIG_VALUE_GAME("cameraCount", 3)),
	cameraNodes(NULL),
	m_heightMapCalculationEnabled(GET_CONFIG_VALUE_GAME("heightmapCalculationEnabled", true))
#ifdef PROFILING_GAME_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("TestGameManager"))
#endif
{
	DEBUG_LOG_GAME("TestGame is being constructed");

	//m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::START, &m_startGameCommand);
	//m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::QUIT, &m_quitGameCommand);
	//m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::RESUME, &m_resumeGameCommand);
	//m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::SAVE, &m_saveGameCommand);
	//m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::LOAD, &m_loadGameCommand);
	//m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::SHOW_INTRO, &m_showIntroGameCommand);

	//AddStaticEffects();
	//AddSmoothEffects();
	//AddBlinkEffects();

	m_actionsToGameCommandsMap.insert(std::make_pair(Engine::Actions::START_GAME, &m_startGameCommand));
	m_actionsToGameCommandsMap.insert(std::make_pair(Engine::Actions::QUIT_GAME, &m_quitGameCommand));

	// TODO: Intro should only be the first game state if the game starts for the first time. In all other cases the main menu should be the initial game state.
	//m_gameStateManager->Push(GetIntroGameState());
	m_gameStateManager->Push(GetMainMenuGameState());
}


Game::TestGameManager::~TestGameManager(void)
{
	SAFE_DELETE_JUST_TABLE(humanNodes);
	SAFE_DELETE_JUST_TABLE(cameraNodes);
}

//void Game::TestGameManager::AddStaticEffects()
//{
//	const int staticSingleValueEffectsCount = GET_CONFIG_VALUE_GAME("staticValueEffectsCount", 4);
//	m_singleValueStaticEffects.reserve(staticSingleValueEffectsCount);
//	for (int i = 0; i < staticSingleValueEffectsCount; ++i)
//	{
//		std::stringstream ss("");
//		ss << (i + 1);
//		m_singleValueStaticEffects.emplace_back(GET_CONFIG_VALUE_GAME("staticValue_" + ss.str(), 0.2f));
//		m_effectFactory.CreateEffect(Rendering::Effects::STATIC, &m_singleValueStaticEffects.back());
//	}
//	const int staticVec2DEffectsCount = GET_CONFIG_VALUE_GAME("staticVec2DEffectsCount", 4);
//	m_vec2DStaticEffects.reserve(staticVec2DEffectsCount);
//	for (int i = 0; i < staticVec2DEffectsCount; ++i)
//	{
//		std::stringstream ss("");
//		ss << (i + 1);
//		m_vec2DStaticEffects.emplace_back(Math::Vector2D(GET_CONFIG_VALUE_GAME("staticVector2D_X_" + ss.str(), 1.0f),
//			GET_CONFIG_VALUE_GAME("staticVector2D_Y_" + ss.str(), 1.0f)));
//		m_effectFactory.CreateEffect(Rendering::Effects::STATIC, &m_vec2DStaticEffects.back());
//	}
//	const int staticVec3DEffectsCount = GET_CONFIG_VALUE_GAME("staticVec3DEffectsCount", 4);
//	m_vec3DStaticEffects.reserve(staticVec3DEffectsCount);
//	for (int i = 0; i < staticVec3DEffectsCount; ++i)
//	{
//		std::stringstream ss("");
//		ss << (i + 1);
//		m_vec3DStaticEffects.emplace_back(Math::Vector3D(GET_CONFIG_VALUE_GAME("staticVector3D_X_" + ss.str(), 1.0f),
//			GET_CONFIG_VALUE_GAME("staticVector3D_Y_" + ss.str(), 1.0f), GET_CONFIG_VALUE_GAME("staticVector3D_Z_" + ss.str(), 1.0f)));
//		m_effectFactory.CreateEffect(Rendering::Effects::STATIC, &m_vec3DStaticEffects.back());
//	}
//}

//void Game::TestGameManager::AddSmoothEffects()
//{
//	const int smoothSingleValueVariantsCount = GET_CONFIG_VALUE_GAME("smoothValueEffectsCount", 4);
//	m_singleValueSmoothEffects.reserve(smoothSingleValueVariantsCount);
//	for (int variant = 0; variant < smoothSingleValueVariantsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		size_t smoothValuesCount = GET_CONFIG_VALUE_GAME("smoothValuesCount_" + ssVariant.str(), 2);
//		bool isSmoothEffectGoingBackAndForth = GET_CONFIG_VALUE_GAME("smoothValuesIsGoingBackAndForth_" + ssVariant.str(), true);
//		std::vector<Math::Real> values;
//		std::vector<Math::Real> times;
//		values.reserve(smoothValuesCount);
//		times.reserve(smoothValuesCount);
//		for (size_t j = 0; j < smoothValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE_GAME("smoothValue_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			times.push_back(GET_CONFIG_VALUE_GAME("smoothValueEffectTime_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//		m_singleValueSmoothEffects.emplace_back(&values[0], &times[0], smoothValuesCount, isSmoothEffectGoingBackAndForth);
//		m_effectFactory.CreateEffect(Rendering::Effects::SMOOTH, &m_singleValueSmoothEffects.back());
//	}
//	const int smoothVec2DEffectsCount = GET_CONFIG_VALUE_GAME("smoothVec2DEffectsCount", 4);
//	m_vec2DSmoothEffects.reserve(smoothVec2DEffectsCount);
//	for (int variant = 0; variant < smoothVec2DEffectsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		size_t smoothValuesCount = GET_CONFIG_VALUE_GAME("smoothVec2DCount_" + ssVariant.str(), 2);
//		bool isSmoothEffectGoingBackAndForth = GET_CONFIG_VALUE_GAME("smoothVec2DIsGoingBackAndForth_" + ssVariant.str(), true);
//		std::vector<Math::Vector2D> values;
//		std::vector<Math::Real> times;
//		values.reserve(smoothValuesCount);
//		times.reserve(smoothValuesCount);
//		for (size_t j = 0; j < smoothValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE_GAME("smoothVec2D_X_" + ssVariant.str() + "_" + ss.str(), 1.0f), GET_CONFIG_VALUE_GAME("smoothVec2D_Y_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			times.push_back(GET_CONFIG_VALUE_GAME("smoothVec2DEffectTime_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//
//		m_vec2DSmoothEffects.emplace_back(&values[0], &times[0], smoothValuesCount, isSmoothEffectGoingBackAndForth);
//		m_effectFactory.CreateEffect(Rendering::Effects::SMOOTH, &m_vec2DSmoothEffects.back());
//	}
//	const int smoothVec3DEffectsCount = GET_CONFIG_VALUE_GAME("smoothVec3DEffectsCount", 4);
//	m_vec3DSmoothEffects.reserve(smoothVec3DEffectsCount);
//	for (int variant = 0; variant < smoothVec3DEffectsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		bool isSmoothEffectGoingBackAndForth = GET_CONFIG_VALUE_GAME("smoothVec3DIsGoingBackAndForth_" + ssVariant.str(), true);
//		size_t smoothValuesCount = GET_CONFIG_VALUE_GAME("smoothVec3DCount_" + ssVariant.str(), 2);
//		std::vector<Math::Vector3D> values;
//		std::vector<Math::Real> times;
//		values.reserve(smoothValuesCount);
//		times.reserve(smoothValuesCount);
//		for (size_t j = 0; j < smoothValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE_GAME("smoothVec3D_X_" + ssVariant.str() + "_" + ss.str(), 1.0f), GET_CONFIG_VALUE_GAME("smoothVec3D_Y_" + ssVariant.str() + "_" + ss.str(), 1.0f),
//				GET_CONFIG_VALUE_GAME("smoothVec3D_Z_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			times.push_back(GET_CONFIG_VALUE_GAME("smoothVec3DEffectTime_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//
//		m_vec3DSmoothEffects.emplace_back(&values[0], &times[0], smoothValuesCount, isSmoothEffectGoingBackAndForth);
//		m_effectFactory.CreateEffect(Rendering::Effects::SMOOTH, &m_vec3DSmoothEffects.back());
//	}
//}

//void Game::TestGameManager::AddBlinkEffects()
//{
//	const int blinkSingleValueVariantsCount = GET_CONFIG_VALUE_GAME("blinkValueEffectsCount", 4);
//	m_singleValueBlinkEffects.reserve(blinkSingleValueVariantsCount);
//	for (int variant = 0; variant < blinkSingleValueVariantsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		size_t blinkValuesCount = GET_CONFIG_VALUE_GAME("blinkValuesCount_" + ssVariant.str(), 2);
//		std::vector<Math::Real> values;
//		std::vector<Math::Real> durations;
//		values.reserve(blinkValuesCount);
//		durations.reserve(blinkValuesCount);
//		for (size_t j = 0; j < blinkValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE_GAME("blinkValue_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			durations.push_back(GET_CONFIG_VALUE_GAME("blinkValueEffectDuration_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//		m_singleValueBlinkEffects.emplace_back(&values[0], &durations[0], blinkValuesCount);
//		m_effectFactory.CreateEffect(Rendering::Effects::BLINK, &m_singleValueBlinkEffects.back());
//	}
//	const int blinkVec2DEffectsCount = GET_CONFIG_VALUE_GAME("blinkVec2DEffectsCount", 4);
//	m_vec2DBlinkEffects.reserve(blinkVec2DEffectsCount);
//	for (int variant = 0; variant < blinkVec2DEffectsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		size_t blinkValuesCount = GET_CONFIG_VALUE_GAME("blinkVec2DCount_" + ssVariant.str(), 2);
//		std::vector<Math::Vector2D> values;
//		std::vector<Math::Real> durations;
//		values.reserve(blinkValuesCount);
//		durations.reserve(blinkValuesCount);
//		for (size_t j = 0; j < blinkValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE_GAME("blinkVec2D_X_" + ssVariant.str() + "_" + ss.str(), 1.0f), GET_CONFIG_VALUE_GAME("blinkVec2D_Y_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			durations.push_back(GET_CONFIG_VALUE_GAME("blinkVec2DEffectDuration_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//
//		m_vec2DBlinkEffects.emplace_back(&values[0], &durations[0], blinkValuesCount);
//		m_effectFactory.CreateEffect(Rendering::Effects::BLINK, &m_vec2DBlinkEffects.back());
//	}
//	const int blinkVec3DEffectsCount = GET_CONFIG_VALUE_GAME("blinkVec3DEffectsCount", 4);
//	m_vec3DBlinkEffects.reserve(blinkVec3DEffectsCount);
//	for (int variant = 0; variant < blinkVec3DEffectsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		size_t blinkValuesCount = GET_CONFIG_VALUE_GAME("blinkVec3DCount_" + ssVariant.str(), 2);
//		std::vector<Math::Vector3D> values;
//		std::vector<Math::Real> durations;
//		values.reserve(blinkValuesCount);
//		durations.reserve(blinkValuesCount);
//		for (size_t j = 0; j < blinkValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE_GAME("blinkVec3D_X_" + ssVariant.str() + "_" + ss.str(), 1.0f), GET_CONFIG_VALUE_GAME("blinkVec3D_Y_" + ssVariant.str() + "_" + ss.str(), 1.0f),
//				GET_CONFIG_VALUE_GAME("blinkVec3D_Z_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			durations.push_back(GET_CONFIG_VALUE_GAME("blinkVec3DEffectDuration_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//
//		m_vec3DBlinkEffects.emplace_back(&values[0], &durations[0], blinkValuesCount);
//		m_effectFactory.CreateEffect(Rendering::Effects::BLINK, &m_vec3DBlinkEffects.back());
//	}
//}

Math::Real Game::TestGameManager::GetLoadingProgress() const
{
	if (m_resourcesLoaded > RESOURCES_TO_LOAD)
	{
		WARNING_LOG_GAME("Resources loaded (", m_resourcesLoaded, ") exceeds the total number of expected resources (", RESOURCES_TO_LOAD, ")");
		return REAL_ONE;
	}
	return static_cast<Math::Real>(m_resourcesLoaded) / RESOURCES_TO_LOAD;
}

Engine::GameState* Game::TestGameManager::GetLoadGameState()
{
	if (m_loadGameState == nullptr)
	{
		m_loadGameState = std::make_unique<LoadGameState>(this, "LoadInputContext");
	}
	return m_loadGameState.get();
}

Engine::GameState* Game::TestGameManager::GetIntroGameState()
{
	if (m_introGameState == nullptr)
	{
		m_introGameState = std::make_unique<IntroGameState>(this, "IntroInputContext");
	}
	return m_introGameState.get();
}

Engine::GameState* Game::TestGameManager::GetMainMenuGameState()
{
	if (m_menuGameState == nullptr)
	{
		//m_mainMenuFont(GET_CONFIG_VALUE_GAME("mainMenuFontTextureAtlas", "cambria.png"), GET_CONFIG_VALUE_STR_GAME("mainMenuFontMetaData", "cambria.fnt")),
		m_menuGameState = std::make_unique<MenuGameState>(this, "MainMenuInputContext",
			m_fontFactory.GetFont(Rendering::Text::FontTypes::CANDARA), GET_CONFIG_VALUE_GAME("mainMenuFontSize", 16.0f));
	}
	return m_menuGameState.get();
}

Engine::GameState* Game::TestGameManager::GetPlayGameState()
{
	if (m_playGameState == nullptr)
	{
		m_playGameState = std::make_unique<PlayGameState>(this, "PlayInputContext");
	}
	return m_playGameState.get();
}

Engine::GameState* Game::TestGameManager::GetPlayMainMenuGameState()
{
	if (m_playMainMenuGameState == nullptr)
	{
		m_playMainMenuGameState = std::make_unique<PlayMenuGameState>(this, "PlayMainMenuInputContext",
			m_fontFactory.GetFont(Rendering::Text::FontTypes::CANDARA), GET_CONFIG_VALUE_GAME("mainMenuFontSize", 16.0f));
	}
	return m_playMainMenuGameState.get();
}

void Game::TestGameManager::Load()
{
	NOTICE_LOG_GAME("Initalizing test game");
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_ALWAYS_GAME(!m_isGameLoaded, Utility::Logging::ERR, "Loading the game run into a problem. The game has already been loaded.");

	//Material bricks(new Texture("bricks.jpg"), specularIntensity, specularPower, Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f);
	//Material bricks2("bricks2_material", Texture("bricks2.jpg"), 0.0f, 0, Texture("bricks2_normal.jpg"), Texture("bricks2_disp.jpg"), 0.04f, -1.0f);
	//Material humanMaterial("human_material", Texture("HumanSkin.jpg"), 2, 32);

	m_terrainNode = new Engine::GameNode();
	//m_terrainMesh = new Rendering::TerrainMesh(GET_CONFIG_VALUE_STR_GAME("terrainModel", "terrain02.obj"));
	//m_terrainMesh = new Rendering::TerrainMesh(REAL_ZERO, REAL_ZERO, GET_CONFIG_VALUE_STR_GAME("terrainHeightMap", "terrainHeightMap.png"));
	const int terrainVertexCount = GET_CONFIG_VALUE_GAME("terrainVertexCount", 128);
	Math::HeightsGenerator heightsGenerator(0, 0, terrainVertexCount, GET_CONFIG_VALUE_GAME("terrainHeightGeneratorAmplitude", 70.0f),
		GET_CONFIG_VALUE_GAME("terrainHeightGeneratorOctavesCount", 3), GET_CONFIG_VALUE_GAME("terrainHeightGeneratorRoughness", 0.3f));
	m_terrainMesh = new Rendering::TerrainMesh(0, 0, heightsGenerator, terrainVertexCount);
#ifndef ANT_TWEAK_BAR_ENABLED
	Math::Real terrainSpecularIntensity = GET_CONFIG_VALUE_GAME("defaultSpecularIntensity", 1.0f);
	Math::Real terrainSpecularPower = GET_CONFIG_VALUE_GAME("defaultSpecularPower", 8.0f);
	Math::Real terrainDisplacementScale = GET_CONFIG_VALUE_GAME("defaultDisplacementScale", 0.02f);
	Math::Real terrainDisplacementOffset = GET_CONFIG_VALUE_GAME("defaultDisplacementOffset", -0.5f);
	Rendering::Material* terrainMaterial = new Material(new Texture(GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture", "grass2.jpg")), terrainSpecularIntensity, terrainSpecularPower,
		new Texture(GET_CONFIG_VALUE_STR_GAME("terrainNormalMap", "grass_normal.jpg")),
		new Texture(GET_CONFIG_VALUE_STR_GAME("terrainDisplacementMap", "grass_disp.jpg")), terrainDisplacementScale, terrainDisplacementOffset);
#else
	terrainMaterial = new Rendering::Material(new Rendering::Texture(GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture", "grass4.jpg")), terrainSpecularIntensity, terrainSpecularPower,
		new Rendering::Texture(GET_CONFIG_VALUE_STR_GAME("terrainNormalMap", "grass_normal.jpg")),
		new Rendering::Texture(GET_CONFIG_VALUE_STR_GAME("terrainDisplacementMap", "grass_disp.jpg")), terrainDisplacementScale, terrainDisplacementOffset);
	//terrainMaterial = new Rendering::Material(m_textureFactory.CreateTexture(TextureTypes::TERRAIN_DIFFUSE_1, GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture", "grass4.jpg")),
	//	terrainSpecularIntensity, terrainSpecularPower,
	//	m_textureFactory.CreateTexture(TextureTypes::TERRAIN_NORMAL_MAP, GET_CONFIG_VALUE_STR_GAME("terrainNormalMap", "grass_normal.jpg")),
	//	m_textureFactory.CreateTexture(TextureTypes::TERRAIN_DISPLACEMENT_MAP, GET_CONFIG_VALUE_STR_GAME("terrainDisplacementMap", "grass_disp.jpg")),
	//	terrainDisplacementScale, terrainDisplacementOffset);
#endif
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	terrainMaterial->SetAdditionalTexture(new Rendering::Texture(GET_CONFIG_VALUE_STR_GAME("terrainBlendMap", "terrainBlendMap.png"), GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, GL_CLAMP_TO_EDGE), "blendMap");
	terrainMaterial->SetAdditionalTexture(new Rendering::Texture(GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture2", "rocks2.jpg")), "diffuse2");
	terrainMaterial->SetAdditionalTexture(new Rendering::Texture(GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture3", "mud.png")), "diffuse3");
	terrainMaterial->SetAdditionalTexture(new Rendering::Texture(GET_CONFIG_VALUE_STR_GAME("terrainDiffuseTexture4", "path.png")), "diffuse4");
	//terrainMaterial->SetAdditionalTexture(new Texture(GET_CONFIG_VALUE_STR_GAME("terrainMap", "terrainMap.jpg")), "terrainMap");
	m_resourcesLoaded += 1; // TODO: Consider creating some prettier solution. This is ugly
	m_terrainNode->AddComponent(new Engine::MeshRendererComponent(m_terrainMesh, terrainMaterial));
	//m_terrainNode->GetTransform().SetPos(0.0f, 0.0f, 5.0f);
	//m_terrainNode->GetTransform().SetScale(20.0f);
	//m_terrainMesh->Initialize();
	m_terrainMesh->TransformPositions(m_terrainNode->GetTransform().GetTransformation());
	//AddToSceneRoot(m_terrainNode); // Terrain node uses special shaders, so we don't actually add it to the game scene hierarchy. Instead we just register it for the renderer to use it.
	AddTerrainNode(m_terrainNode);

	////Vector3D rayEndPosition = boxNode->GetTransform().GetTransformedPos() + boxNode->GetTransform().GetTransformedRot().GetForward() * 100.0f;
	////Vertex vertices [] = { Vertex(boxNode->GetTransform().GetTransformedPos()), Vertex(rayEndPosition) };
	////int indices [] = { 0, 1 };
	////boxNode->AddComponent(new MeshRenderer(
	////	new Mesh(vertices, 2, indices, 2, true, GL_LINES),
	////	new Material(new Texture("DirectionalLight.png"))));

	Engine::GameNode* testMesh1 = new Engine::GameNode();
	testMesh1->GetTransform().SetPos(-2.0f, m_terrainMesh->GetHeightAt(Math::Vector2D(-2.0f, 2.0f)), 2.0f);
	testMesh1->GetTransform().SetRot(Math::Quaternion(REAL_ZERO, sqrtf(2.0f) / 2, sqrtf(2.0f) / 2, REAL_ZERO));
	testMesh1->GetTransform().SetScale(0.1f);
	Engine::GameNode* testMesh2 = new Engine::GameNode();
	testMesh2->GetTransform().SetPos(9.0f, 0.0f, 0.0f);
	//testMesh2->GetTransform().SetScale(1.5f);
	testMesh2->GetTransform().SetRot(Math::Quaternion(Math::Matrix4D(Math::Angle(90.0f), Math::Angle(90.0f), Math::Angle(0.0f))));
	testMesh1->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("plane.obj"), new Rendering::Material(new Rendering::Texture("bricks2.jpg"), 0.0f, 0, new Rendering::Texture("bricks2_normal.jpg"), new Rendering::Texture("bricks2_disp.jpg"), 0.04f, -1.0f)));
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	testMesh2->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("plane.obj"), new Rendering::Material(new Rendering::Texture("bricks2.jpg"), 0.0f, 0)));
	AddToSceneRoot(testMesh1);
	//AddToSceneRoot(testMesh2);
	testMesh1->AddChild(testMesh2);
	Engine::GameNode* testMesh3 = new Engine::GameNode();
	testMesh3->GetTransform().SetPos(-1.0f, 0.5f, 1.0f);
	testMesh3->GetTransform().SetRot(Math::Matrix4D(Math::Angle(0.0f), Math::Angle(0.0f), Math::Angle(-180.0f)));
	testMesh3->GetTransform().SetScale(0.25f);
	testMesh3->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("plane.obj"), new Rendering::Material(new Rendering::Texture("bricks2.jpg"), 0.0f, 0, new Rendering::Texture("bricks2_normal.jpg"), new Rendering::Texture("bricks2_disp.jpg"), 0.04f, -1.0f)));;
	AddToSceneRoot(testMesh3);

	const Math::Random::RandomGenerator& randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE);

	const int treeCount = 30;
	for (int i = 0; i < treeCount; ++i)
	{
		Engine::GameNode* treeNode = new Engine::GameNode();
		Math::Real x = randomGenerator.NextFloat(0.0f, 30.0f);
		Math::Real z = randomGenerator.NextFloat(0.0f, 20.0f);
		Math::Real y = m_terrainMesh->GetHeightAt(Math::Vector2D(x, z));
		treeNode->GetTransform().SetPos(x, y, z);
		treeNode->GetTransform().SetRot(Math::Quaternion(Math::Matrix4D(Math::Angle(0.0f), Math::Angle(randomGenerator.NextFloat(0.0f, 180.0f)), Math::Angle(0.0f))));
		treeNode->GetTransform().SetScale(0.01f);
		//treeNode->SetPhysicsObject(new Physics::PhysicsObject(treeNode->GetTransform(), 1282.0f, Math::Vector3D(0.0f, 0.0f, 0.0f)));
		treeNode->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("lowPolyTree.obj"), new Rendering::Material(new Rendering::Texture("lowPolyTree.png"))));
		//treeNode->AddComponent(new Engine::GravityComponent(m_terrainMesh));
		AddToSceneRoot(treeNode);
	}

	const int boulderCount = 30;
	for (int i = 0; i < boulderCount; ++i)
	{
		Engine::GameNode* boulderNode = new Engine::GameNode();
		Math::Real x = randomGenerator.NextFloat(0.0f, 100.0f);
		Math::Real z = randomGenerator.NextFloat(0.0f, 100.0f);
		Math::Real y = m_terrainMesh->GetHeightAt(Math::Vector2D(x, z));
		boulderNode->GetTransform().SetPos(x, y, z);
		boulderNode->GetTransform().SetRot(Math::Quaternion(Math::Matrix4D(Math::Angle(0.0f), Math::Angle(randomGenerator.NextFloat(0.0f, 180.0f)), Math::Angle(0.0f))));
		boulderNode->GetTransform().SetScale(0.01f);
		//boulderNode->SetPhysicsObject(new Physics::PhysicsObject(boulderNode->GetTransform(), 1282.0f, Math::Vector3D(0.0f, 0.0f, 0.0f)));
		boulderNode->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("boulder.obj"),
			new Rendering::Material(new Rendering::Texture("boulder.png", GL_TEXTURE_2D, GL_LINEAR), 0.01f, 22.0f, new Rendering::Texture("boulderNormal.png"))));
		//boulderNode->AddComponent(new Engine::GravityComponent(m_terrainMesh));
		AddToSceneRoot(boulderNode);
	}

	Engine::GameNode* monkeyNode1 = new Engine::GameNode();
	monkeyNode1->AddComponent(new Engine::MeshRendererComponent(
		new Rendering::Mesh("monkey3.obj"),
		new Rendering::Material(new Rendering::Texture("chessboard3.jpg"))));
	//monkeyNode1->AddComponent(new Engine::LookAtComponent());
	m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	monkeyNode1->GetTransform().SetPos(10.0f, -4.75f, 4.0f);
	monkeyNode1->GetTransform().SetScale(0.1f);
	//monkeyNode1->GetTransform().SetRotation(Quaternion(Vector3D(0, 1, 0), Angle(-45)));
	//monkeyNode1->AddComponent(new LookAtComponent());
	//monkeyNode1->AddComponent(new Engine::ParticleGeneratorComponent(this, new Rendering::ParticleTexture(GET_CONFIG_VALUE_STR_GAME("particleGeneratorTexture", "particleFire.png"), GET_CONFIG_VALUE_GAME("particleGeneratorTextureRowsCount", 4), GET_CONFIG_VALUE_GAME("particleGeneratorTextureIsAdditive", true)),
	//	GET_CONFIG_VALUE_GAME("particleGeneratorParticlesPerSecondCount", 1000),
	//	GET_CONFIG_VALUE_GAME("particleGeneratorParticlesLifeSpanLimit", 0.8f),
	//	GET_CONFIG_VALUE_GAME("particleGeneratorParticlesSpeed", 0.02f),
	//	GET_CONFIG_VALUE_GAME("particleGeneratorParticlesGravityComplient", 0.3f),
	//	Math::Angle(GET_CONFIG_VALUE_GAME("particleGeneratorParticlesRotation", REAL_ZERO)),
	//	GET_CONFIG_VALUE_GAME("particleGeneratorParticlesScale", 0.005f)));
	AddToSceneRoot(monkeyNode1);

	//GameNode* monkeyNode2 = new GameNode();
	//monkeyNode2->AddComponent(new MeshRenderer(
	//	new Mesh("monkey3.obj"),
	//	new Material(new Texture("bricks.jpg"), 2.0f, 32.0f)));
	//m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	//monkeyNode2->GetTransform().SetPos(5.0, 3.0, 15.0);
	////monkeyNode2->AddComponent(new LookAtComponent());
	//AddToSceneRoot(monkeyNode2);

	Engine::GameNode* waterNode = new Engine::GameNode();
	// It seems we have a problem with sharing resources. If I use the plane.obj (which I use in other entities) then we'll have problems with rendering (e.g. disappearing billboards).
	// If I change it to myPlane.obj which is not used in other entities the errors seem to be gone.
	waterNode->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("myPlane.obj"), NULL /* The NULL material fixes the problem with rendering both billboards and water nodes simultaneously. TODO: But why / how? */));
	m_resourcesLoaded += 2;
	waterNode->GetTransform().SetPos(GET_CONFIG_VALUE_GAME("waterNodePosX", -18.0f), GET_CONFIG_VALUE_GAME("waterNodePosY", 0.0f), GET_CONFIG_VALUE_GAME("waterNodePosZ", -12.0f));
	waterNode->GetTransform().SetScale(0.2f);
	AddWaterNode(waterNode);

	AddBillboards(GET_CONFIG_VALUE_GAME("billboardsTreeCount_1", 10), new Rendering::Material(new Rendering::Texture(GET_CONFIG_VALUE_STR_GAME("billboardTreeTexture_1", "Tree1.png"))));
	AddBillboards(GET_CONFIG_VALUE_GAME("billboardsTreeCount_2", 10), new Rendering::Material(new Rendering::Texture(GET_CONFIG_VALUE_STR_GAME("billboardTreeTexture_2", "Tree2.png"))));
	AddBillboards(GET_CONFIG_VALUE_GAME("billboardsTreeCount_3", 10), new Rendering::Material(new Rendering::Texture(GET_CONFIG_VALUE_STR_GAME("billboardTreeTexture_3", "Tree3.png"))));

	//humanNodes = new GameNode* [HUMAN_NODES_COUNT];
	//for (int i = 0; i < HUMAN_NODES_COUNT; ++i)
	//{
	//	humanNodes[i] = new GameNode();
	//	humanNodes[i]->AddComponent(new MeshRenderer(new Mesh("BodyMesh.obj"), new Material(new Texture("HumanSkin.jpg"), 2.0f, 32.0f)));
	//	humanNodes[i]->GetTransform().SetPos(randomGenerator.NextFloat(0.0f, 20.0f), 1.7f, randomGenerator.NextFloat(0.0f, 20.0f));
	//	AddToSceneRoot(humanNodes[i]);
	//}
	//m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly

	//castleNode = new GameNode();
	//castleNode->AddComponent(new MeshRenderer(new Mesh("castle.obj"), new Material(new Texture("HumanSkin.jpg"), 0.5f, 8.0f)));
	//castleNode->GetTransform().SetPos(randomGenerator.NextFloat(0.0f, 30.0f), 1.0f, randomGenerator.NextFloat(0.0f, 30.0f));
	//AddToSceneRoot(castleNode);

	Engine::GameNode* playerNode = new Engine::GameNode();
	const Math::Real playerPositionX = GET_CONFIG_VALUE_GAME("playerPosition_X", 11.2f);
	const Math::Real playerPositionZ = GET_CONFIG_VALUE_GAME("playerPosition_Z", 1.95f);
	const Math::Real playerPositionY = 0.02f; // m_terrainMesh->GetHeightAt(Math::Vector2D(playerPositionX, playerPositionZ));
	playerNode->GetTransform().SetPos(playerPositionX, playerPositionY, playerPositionZ);
	playerNode->GetTransform().SetScale(0.0005f);
	playerNode->CreatePhysicsObject(122.0f, Math::Vector3D(0.0f, 0.0f, 0.0f));
	playerNode->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("mike\\Mike.obj"), new Rendering::Material(new Rendering::Texture("mike_d.tga"), 1.0f, 8.0f, new Rendering::Texture("mike_n.tga"))));
	playerNode->AddComponent(new Engine::PhysicsComponent(2555.5f, 2855.2f)); //, 0.26f, 5.0f, Math::Angle(152.0f, Math::Unit::DEGREE), 0.015f, 0.0002f));
	playerNode->AddComponent(new Engine::GravityComponent(m_terrainMesh));
	Rendering::ParticleTexture* particleTexture = new Rendering::ParticleTexture(GET_CONFIG_VALUE_STR_GAME("particleGeneratorTexture", "particleFire.png"),
		GET_CONFIG_VALUE_GAME("particleGeneratorTextureRowsCount", 4), GET_CONFIG_VALUE_GAME("particleGeneratorTextureIsAdditive", true));
	playerNode->AddComponent(new Engine::ParticleGeneratorComponent(GameManager::GetGameManager(), new Rendering::ParticleTexture(GET_CONFIG_VALUE_STR_GAME("particleGeneratorTexture", "particleFire.png"), GET_CONFIG_VALUE_GAME("particleGeneratorTextureRowsCount", 4), GET_CONFIG_VALUE_GAME("particleGeneratorTextureIsAdditive", true)),
		GET_CONFIG_VALUE_GAME("particleGeneratorParticlesPerSecondCount", 1000), GET_CONFIG_VALUE_GAME("particleGeneratorParticlesLifeSpanLimit", 0.8f), GET_CONFIG_VALUE_GAME("particleGeneratorParticlesSpeed", 0.02f),
		GET_CONFIG_VALUE_GAME("particleGeneratorParticlesGravityComplient", 0.3f), Math::Angle(GET_CONFIG_VALUE_GAME("particleGeneratorParticlesRotation", REAL_ZERO)), GET_CONFIG_VALUE_GAME("particleGeneratorParticlesScale", 0.005f)));
	m_resourcesLoaded += 2;
	AddToSceneRoot(playerNode);

	AddCameras(playerNode); // Adding cameras

	AddSkybox(); // Adding skybox

	m_isGameLoaded = true;
	CHECK_CONDITION_ALWAYS_GAME(m_isGameLoaded, Utility::Logging::CRITICAL, "The game has not been loaded properly.");
	STOP_PROFILING_GAME("");
	NOTICE_LOG_GAME("Initalizing test game finished");
}

void Game::TestGameManager::AddBillboards(unsigned int billboardsCount, Rendering::Material* billboardsMaterial)
{
	const Math::Random::RandomGenerator& randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE);
	Math::Real angle = 0.0f;
	std::vector<Math::Real> billboardsModelMatrices;
	billboardsModelMatrices.reserve(billboardsCount * MATRIX_SIZE * MATRIX_SIZE);
	for (int i = 0; i < billboardsCount; ++i)
	{
		Math::Real x = randomGenerator.NextFloat(0.0f, 150.0f);
		Math::Real z = randomGenerator.NextFloat(0.0f, 150.0f);
		Math::Real y = m_terrainMesh->GetHeightAt(Math::Vector2D(x, z));

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
	Engine::GameNode* billboardsRenderer = new Engine::GameNode();
	billboardsRenderer->AddComponent(new Engine::BillboardsRendererComponent(new Rendering::BillboardMesh(&billboardsModelMatrices[0], billboardsCount, MATRIX_SIZE * MATRIX_SIZE), billboardsMaterial));
	AddBillboardsRenderer(billboardsRenderer);
}

void Game::TestGameManager::AddCameras(Engine::GameNode* entityToFollow)
{
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_EXIT_ALWAYS_GAME(cameraCount >= 1, Utility::Logging::CRITICAL, "No cameras defined in the rendering engine.");

	DEBUG_LOG_GAME("Creating ", cameraCount, " camera(-s)");

	Engine::CameraBuilder cameraBuilder(this);
	Utility::BuilderDirector<Engine::GameNode> cameraBuilderDirector(cameraBuilder);
	for (int i = 0; i < cameraCount; ++i)
	{
		cameraBuilder.SetCameraIndex(i);
		cameraBuilder.SetEntityToFollow(entityToFollow);
		cameraBuilderDirector.Construct();
		Engine::GameNode* cameraNode = cameraBuilder.Get();
		AddToSceneRoot(cameraNode);
	}
	NOTICE_LOG_GAME(cameraCount, " camera(-s) created");
	STOP_PROFILING_GAME("");
}

void Game::TestGameManager::AddSkybox() const
{
	START_PROFILING_GAME(true, "");

	DEBUG_LOG_GAME("Creating a skybox");

	Engine::SkyboxBuilder skyboxBuilder;
	Utility::BuilderDirector<Engine::GameNode> skyboxBuilderDirector(skyboxBuilder);
	skyboxBuilderDirector.Construct();
	//GameNode* skyboxNode = skyboxBuilder.Get();
	NOTICE_LOG_GAME("The skybox has been created");
	STOP_PROFILING_GAME("");
}

//
//void Game::CleanUp()
//{
//	stdlog(INFO, LOGPLACE, "The game is being cleaned up");
//}

void Game::TestGameManager::Update(Math::Real delta)
{
	START_PROFILING_GAME(true, "");
	//for (std::vector<Engine::ParticleGenerator>::iterator particleGeneratorItr = m_particleGenerators.begin(); particleGeneratorItr != m_particleGenerators.end(); ++particleGeneratorItr)
	//{
	//	particleGeneratorItr->Update(delta);
	//}
	m_gameStateManager->Update(delta);
	STOP_PROFILING_GAME("");
}

void Game::TestGameManager::WindowResizeEvent(int width, int height)
{
	GameManager::WindowResizeEvent(width, height);
}

void Game::TestGameManager::ScrollEvent(double xOffset, double yOffset)
{
	GameManager::ScrollEvent(xOffset, yOffset);
	m_gameStateManager->ScrollEvent(xOffset, yOffset);
}

void Game::TestGameManager::MousePosEvent(double xPos, double yPos)
{
	m_gameStateManager->MousePosEvent(xPos, yPos);
}

#ifdef ANT_TWEAK_BAR_ENABLED
void Game::TestGameManager::InitializeTweakBars()
{
#ifdef GAME_PROPERTIES_TWEAK_BAR
	if (!m_isGameLoaded)
	{
		WARNING_LOG_GAME("Cannot initialize game's tweak bars. The game has not been loaded yet.");
		return;
	}
	INFO_LOG_GAME("Initializing game's tweak bars");
	// TODO: GAME_PROPERTIES_TWEAK_BAR gives some errors. Investigate why and fix that!

	TwBar* testGamePropertiesBar = TwNewBar("TestGamePropertiesBar");
	TwAddVarRW(testGamePropertiesBar, "heightMapCalculationEnabled", TW_TYPE_BOOLCPP, &m_heightMapCalculationEnabled, " label='Heightmap calculation enabled' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainSpecularIntensity", TW_TYPE_FLOAT, &terrainSpecularIntensity, " label='Terrain specular intensity' group='Terrain' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainSpecularPower", TW_TYPE_FLOAT, &terrainSpecularPower, " label='Terrain specular power' group='Terrain' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainDisplacementScale", TW_TYPE_FLOAT, &terrainDisplacementScale, " label='Terrain displacement scale' group='Terrain' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainDisplacementOffset", TW_TYPE_FLOAT, &terrainDisplacementOffset, " label='Terrain displacement offset' group='Terrain' ");
	if (terrainMaterial == NULL)
	{
		ERROR_LOG_GAME("Cannot add terrain material information to tweak bar. The terrain material is NULL.");
		return;
	}

	//TwAddVarRO(testGamePropertiesBar, "temp1", TW_TYPE_INT32, &HUMAN_NODES_COUNT, " label='Human count' group='Terrain' ");
	//TwAddVarRO(testGamePropertiesBar, "temp2", TW_TYPE_INT32, &pointLightCount, " label='Human count' group='Box' ");

	terrainMaterial->SetVector3D("Vec1", Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO));
	terrainMaterial->InitializeTweakBar(testGamePropertiesBar, "Terrain");
	//boxMaterial->SetVector3D("Vec1", Math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ONE));
	//boxMaterial->InitializeTweakBar(testGamePropertiesBar, "Box");

	//TwRemoveVar(testGamePropertiesBar, "temp1");
	//TwRemoveVar(testGamePropertiesBar, "temp2");

	TwSetParam(testGamePropertiesBar, NULL, "visible", TW_PARAM_CSTRING, 1, "true"); // Hide the bar at startup
	INFO_LOG_GAME("Initializing game's tweak bars finished");
#endif
}
#endif
