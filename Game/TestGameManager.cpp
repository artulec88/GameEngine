#include "TestGameManager.h"
#include "Def.h"
#include "TextureIDs.h"
#include "MenuGameState.h"
#include "IntroGameState.h"
#include "PlayGameState.h"
#include "PlayMenuGameState.h"
#include "LoadGameState.h"
#include "GameNodeBuilder.h"

#include "Engine\CoreEngine.h"
#include "Engine\BillboardRendererComponent.h"
#include "Engine\MeshRendererComponent.h"
#include "Engine\PhysicsComponent.h"
#include "Engine\LookAtComponent.h"
#include "Engine\GravityComponent.h"
#include "Engine\ParticlesSystemComponent.h"

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
	CAMERA_HEIGHT_UPDATE_INTERVAL(GET_CONFIG_VALUE_GAME("defaultCameraHeightUpdateInterval", 0.01f)),
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
	m_timeToUpdateCameraHeight(REAL_ZERO),
	m_boxNode(NULL),
	HUMAN_NODES_COUNT(2),
	humanNodes(NULL),
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
			GetFont(Rendering::Text::FontTypes::CANDARA), GET_CONFIG_VALUE_GAME("mainMenuFontSize", 16.0f));
	}
	return m_menuGameState.get();
}

Engine::GameState* Game::TestGameManager::GetPlayGameState()
{
	if (m_playGameState == nullptr)
	{
		DEBUG_LOG_GAME("Creating play game state");
		m_playGameState = std::make_unique<PlayGameState>(this, "PlayInputContext");
		DEBUG_LOG_GAME("Play game state created");
	}
	return m_playGameState.get();
}

Engine::GameState* Game::TestGameManager::GetPlayMainMenuGameState()
{
	if (m_playMainMenuGameState == nullptr)
	{
		m_playMainMenuGameState = std::make_unique<PlayMenuGameState>(this, "PlayMainMenuInputContext",
			GetFont(Rendering::Text::FontTypes::CANDARA), GET_CONFIG_VALUE_GAME("mainMenuFontSize", 16.0f));
	}
	return m_playMainMenuGameState.get();
}

void Game::TestGameManager::Load()
{
	NOTICE_LOG_GAME("Initalizing test game");
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_ALWAYS_GAME(!m_isGameLoaded, Utility::Logging::ERR, "Loading the game run into a problem. The game has already been loaded.");

	//Engine::GameNode* testMesh1 = new Engine::GameNode();
	//testMesh1->GetTransform().SetPos(-2.0f, 2.0f, 2.0f);
	//testMesh1->GetTransform().SetRot(Math::Quaternion(REAL_ZERO, sqrtf(2.0f) / 2, sqrtf(2.0f) / 2, REAL_ZERO));
	//testMesh1->GetTransform().SetScale(0.1f);
	//Engine::GameNode* testMesh2 = new Engine::GameNode();
	//testMesh2->GetTransform().SetPos(9.0f, 0.0f, 0.0f);
	////testMesh2->GetTransform().SetScale(1.5f);
	//testMesh2->GetTransform().SetRot(Math::Quaternion(Math::Matrix4D(Math::Angle(90.0f), Math::Angle(90.0f), Math::Angle(0.0f))));
	//testMesh1->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("plane.obj"), new Rendering::Material(m_textureFactory.CreateTexture(TextureIDs::BRICKS, "bricks2.jpg"), 0.0f, 0, m_textureFactory.CreateTexture(TextureIDs::BRICKS_NORMAL_MAP, "bricks2_normal.jpg"), m_textureFactory.CreateTexture(TextureIDs::BRICKS_DISPLACEMENT_MAP, "bricks2_disp.jpg"), 0.04f, -1.0f)));
	//m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	//testMesh2->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("plane.obj"), new Rendering::Material(m_textureFactory.GetTexture(TextureIDs::BRICKS), 0.0f, 0, m_textureFactory.GetTexture(Rendering::TextureIDs::DEFAULT_NORMAL_MAP), m_textureFactory.GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP))));
	//AddToSceneRoot(testMesh1);
	////AddToSceneRoot(testMesh2);
	//testMesh1->AddChild(testMesh2);
	//Engine::GameNode* testMesh3 = new Engine::GameNode();
	//testMesh3->GetTransform().SetPos(-1.0f, 0.5f, 1.0f);
	//testMesh3->GetTransform().SetRot(Math::Matrix4D(Math::Angle(0.0f), Math::Angle(0.0f), Math::Angle(-180.0f)));
	//testMesh3->GetTransform().SetScale(0.25f);
	//testMesh3->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("plane.obj"), new Rendering::Material(m_textureFactory.GetTexture(TextureIDs::BRICKS), 0.0f, 0, m_textureFactory.GetTexture(TextureIDs::BRICKS_NORMAL_MAP), m_textureFactory.GetTexture(TextureIDs::BRICKS_DISPLACEMENT_MAP), 0.04f, -1.0f)));;
	//AddToSceneRoot(testMesh3);

	const Math::Random::RandomGenerator& randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE);

	//Engine::GameNode* monkeyNode1 = new Engine::GameNode();
	//monkeyNode1->AddComponent(new Engine::MeshRendererComponent(
	//	new Rendering::Mesh("monkey3.obj"),
	//	new Rendering::Material(m_textureFactory.CreateTexture(TextureIDs::CHESSBOARD, "chessboard3.jpg"), 1.0f, 8.0f, m_textureFactory.GetTexture(Rendering::TextureIDs::DEFAULT_NORMAL_MAP), m_textureFactory.GetTexture(Rendering::TextureIDs::DEFAULT_DISPLACEMENT_MAP))));
	////monkeyNode1->AddComponent(new Engine::LookAtComponent());
	//m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	//monkeyNode1->GetTransform().SetPos(10.0f, -4.75f, 4.0f);
	//monkeyNode1->GetTransform().SetScale(0.1f);
	////monkeyNode1->GetTransform().SetRotation(Quaternion(Vector3D(0, 1, 0), Angle(-45)));
	////monkeyNode1->AddComponent(new LookAtComponent());
	////monkeyNode1->AddComponent(new Engine::ParticleGeneratorComponent(this, new Rendering::ParticleTexture(GET_CONFIG_VALUE_STR_GAME("particleGeneratorTexture", "particleFire.png"), GET_CONFIG_VALUE_GAME("particleGeneratorTextureRowsCount", 4), GET_CONFIG_VALUE_GAME("particleGeneratorTextureIsAdditive", true)),
	////	GET_CONFIG_VALUE_GAME("particleGeneratorParticlesPerSecondCount", 1000),
	////	GET_CONFIG_VALUE_GAME("particleGeneratorParticlesLifeSpanLimit", 0.8f),
	////	GET_CONFIG_VALUE_GAME("particleGeneratorParticlesSpeed", 0.02f),
	////	GET_CONFIG_VALUE_GAME("particleGeneratorParticlesGravityComplient", 0.3f),
	////	Math::Angle(GET_CONFIG_VALUE_GAME("particleGeneratorParticlesRotation", REAL_ZERO)),
	////	GET_CONFIG_VALUE_GAME("particleGeneratorParticlesScale", 0.005f)));
	//AddToSceneRoot(monkeyNode1);

	//GameNode* monkeyNode2 = new GameNode();
	//monkeyNode2->AddComponent(new MeshRenderer(
	//	new Mesh("monkey3.obj"),
	//	new Material(m_textureFactory.GetTexture(TextureIDs::BRICKS), 2.0f, 32.0f)));
	//m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	//monkeyNode2->GetTransform().SetPos(5.0, 3.0, 15.0);
	////monkeyNode2->AddComponent(new LookAtComponent());
	//AddToSceneRoot(monkeyNode2);

	//humanNodes = new GameNode* [HUMAN_NODES_COUNT];
	//for (int i = 0; i < HUMAN_NODES_COUNT; ++i)
	//{
	//	humanNodes[i] = new GameNode();
	//	humanNodes[i]->AddComponent(new MeshRenderer(new Mesh("BodyMesh.obj"), new Material(m_textureFactory.GetTexture(TextureIDs::HUMAN), 2.0f, 32.0f)));
	//	humanNodes[i]->GetTransform().SetPos(randomGenerator.NextFloat(0.0f, 20.0f), 1.7f, randomGenerator.NextFloat(0.0f, 20.0f));
	//	AddToSceneRoot(humanNodes[i]);
	//}
	//m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly

	m_isGameLoaded = true;
	CHECK_CONDITION_ALWAYS_GAME(m_isGameLoaded, Utility::Logging::CRITICAL, "The game has not been loaded properly.");
	STOP_PROFILING_GAME("");
	NOTICE_LOG_GAME("Initalizing test game finished");
}

// TODO: temporary code. Remove in the future.
//Rendering::Particles::ParticlesSystem Game::TestGameManager::CreateParticlesSystem(ParticleEffects::ParticleEffect particleEffect)
//{
	//ParticlesSystemBuilder particlesSystemBuilder(this, particleEffect);
	//Utility::BuilderDirector<Rendering::Particles::ParticlesSystem> particlesSystemBuilderDirector(particlesSystemBuilder);
	//particlesSystemBuilderDirector.Construct();
	//Rendering::Particles::ParticlesSystem* particlesSystem = particlesSystemBuilder.Get();
	//if (particlesSystem != NULL)
	//{
	//	INFO_LOG_RENDERING("Particles system for effect ", particleEffect, " has been created.");
	//}
	//return particlesSystem;

	//START_PROFILING_GAME(true, "");

	//ParticlesSystemBuilder particlesSystemBuilder(this, particleEffect);
	//Utility::BuilderDirector<Rendering::Particles::ParticlesSystem> particlesSystemBuilderDirector(particlesSystemBuilder);
	//Rendering::Particles::ParticlesSystem particlesSystem(particlesSystemBuilderDirector.Construct());
	//STOP_PROFILING_GAME("");
	//return particlesSystem;
//}

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

	//TwAddVarRO(testGamePropertiesBar, "temp1", TW_TYPE_INT32, &HUMAN_NODES_COUNT, " label='Human count' group='Terrain' ");
	//TwAddVarRO(testGamePropertiesBar, "temp2", TW_TYPE_INT32, &pointLightCount, " label='Human count' group='Box' ");

	//terrainMaterial->SetVector3D("Vec1", Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO));
	//terrainMaterial->InitializeTweakBar(testGamePropertiesBar, "Terrain");
	//boxMaterial->SetVector3D("Vec1", Math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ONE));
	//boxMaterial->InitializeTweakBar(testGamePropertiesBar, "Box");

	//TwRemoveVar(testGamePropertiesBar, "temp1");
	//TwRemoveVar(testGamePropertiesBar, "temp2");

	TwSetParam(testGamePropertiesBar, NULL, "visible", TW_PARAM_CSTRING, 1, "true"); // Hide the bar at startup
	INFO_LOG_GAME("Initializing game's tweak bars finished");
#endif
}
#endif
