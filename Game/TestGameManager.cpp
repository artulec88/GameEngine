#include "TestGameManager.h"
#include "MenuGameState.h"
#include "IntroGameState.h"
#include "PlayGameState.h"
#include "PlayMenuGameState.h"
#include "LoadGameState.h"

#include "Engine\CoreEngine.h"
#include "Rendering\Camera.h"
#include "Engine\MeshRendererComponent.h"
#include "Engine\PhysicsComponent.h"
#include "Engine\LookAtComponent.h"
#include "Engine\GravityComponent.h"
#include "Engine\ParticleGeneratorComponent.h"
#include "Rendering\Color.h"
#include "Engine\Builder.h"
#include "Engine\BuilderDirector.h"

#include "Math\FloatingPoint.h"
#include "Math\Quaternion.h"
//#include "Math\Vector.h"

#include "Physics\PhysicsObject.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

#include <sstream>

using namespace Game;
using namespace Utility;
using namespace Math;

TestGameManager::TestGameManager() :
	GameManager(),
	RESOURCES_TO_LOAD(26),
	CAMERA_HEIGHT_UPDATE_INTERVAL(GET_CONFIG_VALUE("cameraHeightUpdateInterval", 0.01f)),
	m_resourcesLoaded(0),
	m_introGameState(NULL),
	m_menuGameState(NULL),
	m_loadGameState(NULL),
	m_playGameState(NULL),
	m_playMainMenuGameState(NULL),
	m_quitGameCommand(),
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
	terrainSpecularIntensity(GET_CONFIG_VALUE("defaultSpecularIntensity", 1.0f)),
	terrainSpecularPower(GET_CONFIG_VALUE("defaultSpecularPower", 8.0f)),
	terrainDisplacementScale(GET_CONFIG_VALUE("defaultDisplacementScale", 0.02f)),
	terrainDisplacementOffset(GET_CONFIG_VALUE("defaultDisplacementOffset", -0.5f)),
#endif
	HUMAN_NODES_COUNT(2),
	humanNodes(NULL),
	pointLightCount(GET_CONFIG_VALUE("pointLightsCount", 1)),
	spotLightCount(GET_CONFIG_VALUE("spotLightsCount", 1)),
	cameraCount(GET_CONFIG_VALUE("cameraCount", 3)),
	cameraNodes(NULL),
	m_heightMapCalculationEnabled(GET_CONFIG_VALUE("heightmapCalculationEnabled", true))
#ifdef CALCULATE_GAME_STATS
	, m_classStats(STATS_STORAGE.GetClassStats("TestGameManager"))
#endif
{
	DEBUG_LOG("TestGame is being constructed");

	m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::START, &m_startGameCommand);
	m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::QUIT, &m_quitGameCommand);
	m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::RESUME, &m_resumeGameCommand);
	m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::SAVE, &m_saveGameCommand);
	m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::LOAD, &m_loadGameCommand);
	m_gameCommandFactory.CreateCommand(Engine::GameCommandTypes::SHOW_INTRO, &m_showIntroGameCommand);

	//AddStaticEffects();
	//AddSmoothEffects();
	//AddBlinkEffects();

	// TODO: Intro should only be the first game state if the game starts for the first time. In all other cases the main menu should be the initial game state.
	//m_gameStateManager->Push(GetIntroGameState());
	m_gameStateManager->Push(GetMainMenuGameState());
	srand((unsigned int)time(NULL));
}


TestGameManager::~TestGameManager(void)
{
	SAFE_DELETE_JUST_TABLE(humanNodes);
	SAFE_DELETE_JUST_TABLE(cameraNodes);
	SAFE_DELETE(m_introGameState); // TODO: Removing the game states here will lead to errors because GameStateManager's destructor does the same thing
	SAFE_DELETE(m_menuGameState);
	SAFE_DELETE(m_loadGameState);
	SAFE_DELETE(m_playGameState);
	SAFE_DELETE(m_playMainMenuGameState);
}

//void TestGameManager::AddStaticEffects()
//{
//	const int staticSingleValueEffectsCount = GET_CONFIG_VALUE("staticValueEffectsCount", 4);
//	m_singleValueStaticEffects.reserve(staticSingleValueEffectsCount);
//	for (int i = 0; i < staticSingleValueEffectsCount; ++i)
//	{
//		std::stringstream ss("");
//		ss << (i + 1);
//		m_singleValueStaticEffects.emplace_back(GET_CONFIG_VALUE("staticValue_" + ss.str(), 0.2f));
//		m_effectFactory.CreateEffect(Rendering::Effects::STATIC, &m_singleValueStaticEffects.back());
//	}
//	const int staticVec2DEffectsCount = GET_CONFIG_VALUE("staticVec2DEffectsCount", 4);
//	m_vec2DStaticEffects.reserve(staticVec2DEffectsCount);
//	for (int i = 0; i < staticVec2DEffectsCount; ++i)
//	{
//		std::stringstream ss("");
//		ss << (i + 1);
//		m_vec2DStaticEffects.emplace_back(Math::Vector2D(GET_CONFIG_VALUE("staticVector2D_X_" + ss.str(), 1.0f),
//			GET_CONFIG_VALUE("staticVector2D_Y_" + ss.str(), 1.0f)));
//		m_effectFactory.CreateEffect(Rendering::Effects::STATIC, &m_vec2DStaticEffects.back());
//	}
//	const int staticVec3DEffectsCount = GET_CONFIG_VALUE("staticVec3DEffectsCount", 4);
//	m_vec3DStaticEffects.reserve(staticVec3DEffectsCount);
//	for (int i = 0; i < staticVec3DEffectsCount; ++i)
//	{
//		std::stringstream ss("");
//		ss << (i + 1);
//		m_vec3DStaticEffects.emplace_back(Math::Vector3D(GET_CONFIG_VALUE("staticVector3D_X_" + ss.str(), 1.0f),
//			GET_CONFIG_VALUE("staticVector3D_Y_" + ss.str(), 1.0f), GET_CONFIG_VALUE("staticVector3D_Z_" + ss.str(), 1.0f)));
//		m_effectFactory.CreateEffect(Rendering::Effects::STATIC, &m_vec3DStaticEffects.back());
//	}
//}

//void TestGameManager::AddSmoothEffects()
//{
//	const int smoothSingleValueVariantsCount = GET_CONFIG_VALUE("smoothValueEffectsCount", 4);
//	m_singleValueSmoothEffects.reserve(smoothSingleValueVariantsCount);
//	for (int variant = 0; variant < smoothSingleValueVariantsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		size_t smoothValuesCount = GET_CONFIG_VALUE("smoothValuesCount_" + ssVariant.str(), 2);
//		bool isSmoothEffectGoingBackAndForth = GET_CONFIG_VALUE("smoothValuesIsGoingBackAndForth_" + ssVariant.str(), true);
//		std::vector<Math::Real> values;
//		std::vector<Math::Real> times;
//		values.reserve(smoothValuesCount);
//		times.reserve(smoothValuesCount);
//		for (size_t j = 0; j < smoothValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE("smoothValue_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			times.push_back(GET_CONFIG_VALUE("smoothValueEffectTime_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//		m_singleValueSmoothEffects.emplace_back(&values[0], &times[0], smoothValuesCount, isSmoothEffectGoingBackAndForth);
//		m_effectFactory.CreateEffect(Rendering::Effects::SMOOTH, &m_singleValueSmoothEffects.back());
//	}
//	const int smoothVec2DEffectsCount = GET_CONFIG_VALUE("smoothVec2DEffectsCount", 4);
//	m_vec2DSmoothEffects.reserve(smoothVec2DEffectsCount);
//	for (int variant = 0; variant < smoothVec2DEffectsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		size_t smoothValuesCount = GET_CONFIG_VALUE("smoothVec2DCount_" + ssVariant.str(), 2);
//		bool isSmoothEffectGoingBackAndForth = GET_CONFIG_VALUE("smoothVec2DIsGoingBackAndForth_" + ssVariant.str(), true);
//		std::vector<Math::Vector2D> values;
//		std::vector<Math::Real> times;
//		values.reserve(smoothValuesCount);
//		times.reserve(smoothValuesCount);
//		for (size_t j = 0; j < smoothValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE("smoothVec2D_X_" + ssVariant.str() + "_" + ss.str(), 1.0f), GET_CONFIG_VALUE("smoothVec2D_Y_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			times.push_back(GET_CONFIG_VALUE("smoothVec2DEffectTime_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//
//		m_vec2DSmoothEffects.emplace_back(&values[0], &times[0], smoothValuesCount, isSmoothEffectGoingBackAndForth);
//		m_effectFactory.CreateEffect(Rendering::Effects::SMOOTH, &m_vec2DSmoothEffects.back());
//	}
//	const int smoothVec3DEffectsCount = GET_CONFIG_VALUE("smoothVec3DEffectsCount", 4);
//	m_vec3DSmoothEffects.reserve(smoothVec3DEffectsCount);
//	for (int variant = 0; variant < smoothVec3DEffectsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		bool isSmoothEffectGoingBackAndForth = GET_CONFIG_VALUE("smoothVec3DIsGoingBackAndForth_" + ssVariant.str(), true);
//		size_t smoothValuesCount = GET_CONFIG_VALUE("smoothVec3DCount_" + ssVariant.str(), 2);
//		std::vector<Math::Vector3D> values;
//		std::vector<Math::Real> times;
//		values.reserve(smoothValuesCount);
//		times.reserve(smoothValuesCount);
//		for (size_t j = 0; j < smoothValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE("smoothVec3D_X_" + ssVariant.str() + "_" + ss.str(), 1.0f), GET_CONFIG_VALUE("smoothVec3D_Y_" + ssVariant.str() + "_" + ss.str(), 1.0f),
//				GET_CONFIG_VALUE("smoothVec3D_Z_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			times.push_back(GET_CONFIG_VALUE("smoothVec3DEffectTime_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//
//		m_vec3DSmoothEffects.emplace_back(&values[0], &times[0], smoothValuesCount, isSmoothEffectGoingBackAndForth);
//		m_effectFactory.CreateEffect(Rendering::Effects::SMOOTH, &m_vec3DSmoothEffects.back());
//	}
//}

//void TestGameManager::AddBlinkEffects()
//{
//	const int blinkSingleValueVariantsCount = GET_CONFIG_VALUE("blinkValueEffectsCount", 4);
//	m_singleValueBlinkEffects.reserve(blinkSingleValueVariantsCount);
//	for (int variant = 0; variant < blinkSingleValueVariantsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		size_t blinkValuesCount = GET_CONFIG_VALUE("blinkValuesCount_" + ssVariant.str(), 2);
//		std::vector<Math::Real> values;
//		std::vector<Math::Real> durations;
//		values.reserve(blinkValuesCount);
//		durations.reserve(blinkValuesCount);
//		for (size_t j = 0; j < blinkValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE("blinkValue_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			durations.push_back(GET_CONFIG_VALUE("blinkValueEffectDuration_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//		m_singleValueBlinkEffects.emplace_back(&values[0], &durations[0], blinkValuesCount);
//		m_effectFactory.CreateEffect(Rendering::Effects::BLINK, &m_singleValueBlinkEffects.back());
//	}
//	const int blinkVec2DEffectsCount = GET_CONFIG_VALUE("blinkVec2DEffectsCount", 4);
//	m_vec2DBlinkEffects.reserve(blinkVec2DEffectsCount);
//	for (int variant = 0; variant < blinkVec2DEffectsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		size_t blinkValuesCount = GET_CONFIG_VALUE("blinkVec2DCount_" + ssVariant.str(), 2);
//		std::vector<Math::Vector2D> values;
//		std::vector<Math::Real> durations;
//		values.reserve(blinkValuesCount);
//		durations.reserve(blinkValuesCount);
//		for (size_t j = 0; j < blinkValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE("blinkVec2D_X_" + ssVariant.str() + "_" + ss.str(), 1.0f), GET_CONFIG_VALUE("blinkVec2D_Y_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			durations.push_back(GET_CONFIG_VALUE("blinkVec2DEffectDuration_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//
//		m_vec2DBlinkEffects.emplace_back(&values[0], &durations[0], blinkValuesCount);
//		m_effectFactory.CreateEffect(Rendering::Effects::BLINK, &m_vec2DBlinkEffects.back());
//	}
//	const int blinkVec3DEffectsCount = GET_CONFIG_VALUE("blinkVec3DEffectsCount", 4);
//	m_vec3DBlinkEffects.reserve(blinkVec3DEffectsCount);
//	for (int variant = 0; variant < blinkVec3DEffectsCount; ++variant)
//	{
//		std::stringstream ssVariant("");
//		ssVariant << (variant + 1);
//		size_t blinkValuesCount = GET_CONFIG_VALUE("blinkVec3DCount_" + ssVariant.str(), 2);
//		std::vector<Math::Vector3D> values;
//		std::vector<Math::Real> durations;
//		values.reserve(blinkValuesCount);
//		durations.reserve(blinkValuesCount);
//		for (size_t j = 0; j < blinkValuesCount; ++j)
//		{
//			std::stringstream ss("");
//			ss << (j + 1);
//			values.emplace_back(GET_CONFIG_VALUE("blinkVec3D_X_" + ssVariant.str() + "_" + ss.str(), 1.0f), GET_CONFIG_VALUE("blinkVec3D_Y_" + ssVariant.str() + "_" + ss.str(), 1.0f),
//				GET_CONFIG_VALUE("blinkVec3D_Z_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//			durations.push_back(GET_CONFIG_VALUE("blinkVec3DEffectDuration_" + ssVariant.str() + "_" + ss.str(), 1.0f));
//		}
//
//		m_vec3DBlinkEffects.emplace_back(&values[0], &durations[0], blinkValuesCount);
//		m_effectFactory.CreateEffect(Rendering::Effects::BLINK, &m_vec3DBlinkEffects.back());
//	}
//}

Math::Real TestGameManager::GetLoadingProgress() const
{
	if (m_resourcesLoaded > RESOURCES_TO_LOAD)
	{
		WARNING_LOG("Resources loaded (%d) exceeds the total number of expected resources (%d)", m_resourcesLoaded, RESOURCES_TO_LOAD);
		return REAL_ONE;
	}
	return static_cast<Math::Real>(m_resourcesLoaded) / RESOURCES_TO_LOAD;
}

Engine::GameState* TestGameManager::GetLoadGameState()
{
	if (m_loadGameState == NULL)
	{
		m_loadGameState = new LoadGameState();
	}
	return m_loadGameState;
}

Engine::GameState* TestGameManager::GetIntroGameState()
{
	if (m_introGameState == NULL)
	{
		m_introGameState = new IntroGameState();
	}
	return m_introGameState;
}

Engine::GameState* TestGameManager::GetMainMenuGameState()
{
	if (m_menuGameState == NULL)
	{
		m_menuGameState = new MenuGameState();
	}
	return m_menuGameState;
}

Engine::GameState* TestGameManager::GetPlayGameState()
{
	if (m_playGameState == NULL)
	{
		m_playGameState = new PlayGameState(this);
	}
	return m_playGameState;
}

Engine::GameState* TestGameManager::GetPlayMainMenuGameState()
{
	if (m_playMainMenuGameState == NULL)
	{
		m_playMainMenuGameState = new PlayMenuGameState();
	}
	return m_playMainMenuGameState;
}

void TestGameManager::Load()
{
	NOTICE_LOG("Initalizing test game");
	START_PROFILING;
	CHECK_CONDITION_ALWAYS(!m_isGameLoaded, Utility::Error, "Loading the game run into a problem. The game has already been loaded.");

	//Material bricks(new Texture("bricks.jpg"), specularIntensity, specularPower, Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f);
	//Material bricks2("bricks2_material", Texture("bricks2.jpg"), 0.0f, 0, Texture("bricks2_normal.jpg"), Texture("bricks2_disp.jpg"), 0.04f, -1.0f);
	//Material humanMaterial("human_material", Texture("HumanSkin.jpg"), 2, 32);

	m_terrainNode = new Engine::GameNode();
	m_terrainMesh = new Rendering::TerrainMesh(REAL_ZERO, REAL_ZERO, GET_CONFIG_VALUE_STR("terrainHeightMap", "terrainHeightMap.png"));
	//m_terrainMesh = new Rendering::TerrainMesh(GET_CONFIG_VALUE_STR("terrainModel", "terrain02.obj"));
#ifndef ANT_TWEAK_BAR_ENABLED
	Math::Real terrainSpecularIntensity = GET_CONFIG_VALUE("defaultSpecularIntensity", 1.0f);
	Math::Real terrainSpecularPower = GET_CONFIG_VALUE("defaultSpecularPower", 8.0f);
	Math::Real terrainDisplacementScale = GET_CONFIG_VALUE("defaultDisplacementScale", 0.02f);
	Math::Real terrainDisplacementOffset = GET_CONFIG_VALUE("defaultDisplacementOffset", -0.5f);
	Rendering::Material* terrainMaterial = new Material(new Texture(GET_CONFIG_VALUE_STR("terrainDiffuseTexture", "grass2.jpg")), terrainSpecularIntensity, terrainSpecularPower,
		new Texture(GET_CONFIG_VALUE_STR("terrainNormalMap", "grass_normal.jpg")),
		new Texture(GET_CONFIG_VALUE_STR("terrainDisplacementMap", "grass_disp.jpg")), terrainDisplacementScale, terrainDisplacementOffset);
#else
	terrainMaterial = new Rendering::Material(new Rendering::Texture(GET_CONFIG_VALUE_STR("terrainDiffuseTexture", "grass4.jpg")), terrainSpecularIntensity, terrainSpecularPower,
		new Rendering::Texture(GET_CONFIG_VALUE_STR("terrainNormalMap", "grass_normal.jpg")),
		new Rendering::Texture(GET_CONFIG_VALUE_STR("terrainDisplacementMap", "grass_disp.jpg")), terrainDisplacementScale, terrainDisplacementOffset);
#endif
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	terrainMaterial->SetAdditionalTexture(new Rendering::Texture(GET_CONFIG_VALUE_STR("terrainBlendMap", "terrainBlendMap.png")), "blendMap");
	terrainMaterial->SetAdditionalTexture(new Rendering::Texture(GET_CONFIG_VALUE_STR("terrainDiffuseTexture2", "rocks2.jpg")), "diffuse2");
	terrainMaterial->SetAdditionalTexture(new Rendering::Texture(GET_CONFIG_VALUE_STR("terrainDiffuseTexture3", "mud.png")), "diffuse3");
	terrainMaterial->SetAdditionalTexture(new Rendering::Texture(GET_CONFIG_VALUE_STR("terrainDiffuseTexture4", "path.png")), "diffuse4");
	//terrainMaterial->SetAdditionalTexture(new Texture(GET_CONFIG_VALUE_STR("terrainMap", "terrainMap.jpg")), "terrainMap");
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
	testMesh1->GetTransform().SetRot(Quaternion(REAL_ZERO, sqrtf(2.0f) / 2, sqrtf(2.0f) / 2, REAL_ZERO));
	testMesh1->GetTransform().SetScale(0.1f);
	Engine::GameNode* testMesh2 = new Engine::GameNode();
	testMesh2->GetTransform().SetPos(9.0f, 0.0f, 0.0f);
	//testMesh2->GetTransform().SetScale(1.5f);
	testMesh2->GetTransform().SetRot(Quaternion(Matrix4D(Angle(90.0f), Angle(90.0f), Angle(0.0f))));
	testMesh1->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("plane.obj"), new Rendering::Material(new Rendering::Texture("bricks2.jpg"), 0.0f, 0, new Rendering::Texture("bricks2_normal.jpg"), new Rendering::Texture("bricks2_disp.jpg"), 0.04f, -1.0f)));
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	testMesh2->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("plane.obj"), new Rendering::Material(new Rendering::Texture("bricks2.jpg"), 0.0f, 0)));
	AddToSceneRoot(testMesh1);
	//AddToSceneRoot(testMesh2);
	testMesh1->AddChild(testMesh2);
	Engine::GameNode* testMesh3 = new Engine::GameNode();
	testMesh3->GetTransform().SetPos(-1.0f, 0.5f, 1.0f);
	testMesh3->GetTransform().SetRot(Matrix4D(Angle(0.0f), Angle(0.0f), Angle(-180.0f)));
	testMesh3->GetTransform().SetScale(0.25f);
	testMesh3->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("plane.obj"), new Rendering::Material(new Rendering::Texture("bricks2.jpg"), 0.0f, 0, new Rendering::Texture("bricks2_normal.jpg"), new Rendering::Texture("bricks2_disp.jpg"), 0.04f, -1.0f)));;
	AddToSceneRoot(testMesh3);

	const int treeCount = 30;
	for (int i = 0; i < treeCount; ++i)
	{
		Engine::GameNode* treeNode = new Engine::GameNode();
		Math::Real x = (static_cast<Real>(rand() % 5000) / 2500.0f) * 50.0f;
		Math::Real z = (static_cast<Real>(rand() % 5000) / 2500.0f) * 50.0f;
		Math::Real y = m_terrainMesh->GetHeightAt(Math::Vector2D(x, z));
		treeNode->GetTransform().SetPos(x, y, z);
		treeNode->GetTransform().SetRot(Quaternion(Matrix4D(Angle(0.0f), Angle(rand() % 180), Angle(0.0f))));
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
		Math::Real x = (static_cast<Real>(rand() % 5000) / 2500.0f) * 50.0f;
		Math::Real z = (static_cast<Real>(rand() % 5000) / 2500.0f) * 50.0f;
		Math::Real y = m_terrainMesh->GetHeightAt(Math::Vector2D(x, z));
		boulderNode->GetTransform().SetPos(x, y, z);
		boulderNode->GetTransform().SetRot(Quaternion(Matrix4D(Angle(0.0f), Angle(rand() % 180), Angle(0.0f))));
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
	//INFO_LOG("MonkeyNode1 has ID=%d", monkeyNode1->GetID());
	//monkeyNode1->AddComponent(new LookAtComponent());
	//monkeyNode1->AddComponent(new Engine::ParticleGeneratorComponent(this, new Rendering::ParticleTexture(GET_CONFIG_VALUE_STR("particleGeneratorTexture", "particleFire.png"), GET_CONFIG_VALUE("particleGeneratorTextureRowsCount", 4), GET_CONFIG_VALUE("particleGeneratorTextureIsAdditive", true)),
	//	GET_CONFIG_VALUE("particleGeneratorParticlesPerSecondCount", 1000),
	//	GET_CONFIG_VALUE("particleGeneratorParticlesLifeSpanLimit", 0.8f),
	//	GET_CONFIG_VALUE("particleGeneratorParticlesSpeed", 0.02f),
	//	GET_CONFIG_VALUE("particleGeneratorParticlesGravityComplient", 0.3f),
	//	Math::Angle(GET_CONFIG_VALUE("particleGeneratorParticlesRotation", REAL_ZERO)),
	//	GET_CONFIG_VALUE("particleGeneratorParticlesScale", 0.005f)));
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
	waterNode->GetTransform().SetPos(GET_CONFIG_VALUE("waterNodePosX", -18.0f), GET_CONFIG_VALUE("waterNodePosY", 0.0f), GET_CONFIG_VALUE("waterNodePosZ", -12.0f));
	waterNode->GetTransform().SetScale(0.2f);
	AddWaterNode(waterNode);

	for (int i = 0; i < 20; ++i)
	{
		Engine::GameNode* billboardNode = new Engine::GameNode();
		Math::Real x = static_cast<Real>(rand() % 50) - 25.0f;
		Math::Real z = static_cast<Real>(rand() % 50) - 25.0f;
		Math::Real y = m_terrainMesh->GetHeightAt(Math::Vector2D(x, z));
		billboardNode->AddComponent(new Engine::MeshRendererComponent(new Rendering::BillboardMesh(Math::Vector3D(x, y, z)), new Rendering::Material(new Rendering::Texture(GET_CONFIG_VALUE_STR("billboardTreeTexture", "Tree1.png"), GL_TEXTURE_2D, GL_NEAREST))));
		// TODO: Scaling the billboards
		//billboardNode->GetTransform().SetScale(0.2f);
		AddBillboardNode(billboardNode);
	}

	//humanNodes = new GameNode* [HUMAN_NODES_COUNT];
	//for (int i = 0; i < HUMAN_NODES_COUNT; ++i)
	//{
	//	humanNodes[i] = new GameNode();
	//	humanNodes[i]->AddComponent(new MeshRenderer(new Mesh("BodyMesh.obj"), new Material(new Texture("HumanSkin.jpg"), 2.0f, 32.0f)));
	//	humanNodes[i]->GetTransform().SetPos(static_cast<Real>(rand() % 20), 1.7f, static_cast<Real>(rand() % 20));
	//	AddToSceneRoot(humanNodes[i]);
	//}
	//m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly

	//castleNode = new GameNode();
	//castleNode->AddComponent(new MeshRenderer(new Mesh("castle.obj"), new Material(new Texture("HumanSkin.jpg"), 0.5f, 8.0f)));
	//castleNode->GetTransform().SetPos(static_cast<Real>(rand() % 50), 1.0f, static_cast<Real>(rand() % 50));
	//AddToSceneRoot(castleNode);

	Engine::GameNode* playerNode = new Engine::GameNode();
	const Math::Real playerPositionX = 11.2f;
	const Math::Real playerPositionZ = 1.95f;
	const Math::Real playerPositionY = 0.02f; // m_terrainMesh->GetHeightAt(Math::Vector2D(playerPositionX, playerPositionZ));
	playerNode->GetTransform().SetPos(playerPositionX, playerPositionY, playerPositionZ);
	playerNode->GetTransform().SetScale(0.005f);
	playerNode->SetPhysicsObject(new Physics::PhysicsObject(playerNode->GetTransform(), 82.0f, Math::Vector3D(0.0f, 0.0f, 0.0f)));
	playerNode->AddComponent(new Engine::MeshRendererComponent(new Rendering::Mesh("person.obj"), new Rendering::Material(new Rendering::Texture("player.png", GL_TEXTURE_2D, GL_LINEAR))));
	playerNode->AddComponent(new Engine::PhysicsComponent(2555.5f, 2855.2f)); //, 0.26f, 5.0f, Math::Angle(152.0f, Math::Unit::DEGREE), 0.015f, 0.0002f));
	playerNode->AddComponent(new Engine::GravityComponent(m_terrainMesh));
	Rendering::ParticleTexture* particleTexture = new Rendering::ParticleTexture(GET_CONFIG_VALUE_STR("particleGeneratorTexture", "particleFire.png"),
		GET_CONFIG_VALUE("particleGeneratorTextureRowsCount", 4), GET_CONFIG_VALUE("particleGeneratorTextureIsAdditive", true));
	playerNode->AddComponent(new Engine::ParticleGeneratorComponent(GameManager::GetGameManager(), new Rendering::ParticleTexture(GET_CONFIG_VALUE_STR("particleGeneratorTexture", "particleFire.png"), GET_CONFIG_VALUE("particleGeneratorTextureRowsCount", 4), GET_CONFIG_VALUE("particleGeneratorTextureIsAdditive", true)),
		GET_CONFIG_VALUE("particleGeneratorParticlesPerSecondCount", 1000), GET_CONFIG_VALUE("particleGeneratorParticlesLifeSpanLimit", 0.8f), GET_CONFIG_VALUE("particleGeneratorParticlesSpeed", 0.02f),
		GET_CONFIG_VALUE("particleGeneratorParticlesGravityComplient", 0.3f), Math::Angle(GET_CONFIG_VALUE("particleGeneratorParticlesRotation", REAL_ZERO)), GET_CONFIG_VALUE("particleGeneratorParticlesScale", 0.005f)));
	m_resourcesLoaded += 2;
	AddToSceneRoot(playerNode);

	AddLights(); // Adding all kinds of light (directional, point, spot)

	AddCameras(playerNode); // Adding cameras

	AddSkybox(); // Adding skybox

	m_isGameLoaded = true;
	CHECK_CONDITION_ALWAYS(m_isGameLoaded, Utility::Critical, "The game has not been loaded properly.");
	STOP_PROFILING;
	NOTICE_LOG("Initalizing test game finished");
}

void TestGameManager::AddLights()
{
	START_PROFILING;
	AddDirectionalLight(); // Adding directional light (if enabled)
	if (pointLightCount > 0)
	{
		NOTICE_LOG("Creating %d point lights", pointLightCount);
		AddPointLights();
		DEBUG_LOG("%d point lights created", pointLightCount);
	}
	else
	{
		NOTICE_LOG("Point lights disabled");
	}
	if (spotLightCount > 0)
	{
		NOTICE_LOG("Creating %d spot lights...", spotLightCount);
		AddSpotLights();
		DEBUG_LOG("%d spot lights created", spotLightCount);
	}
	else
	{
		NOTICE_LOG("Spot lights disabled");
	}
	STOP_PROFILING;
}

void TestGameManager::AddDirectionalLight()
{
	// TODO: For now we only check if directionalLightsCount is zero or not.
	// In the future there might be many directional lights enabled (?)
	int directionalLightsCount = GET_CONFIG_VALUE("directionalLightsCount", 1);
	if (directionalLightsCount == 0)
	{
		NOTICE_LOG("Directional lights disabled");
		return;
	}
	NOTICE_LOG("Directional lights enabled");

	Engine::DirectionalLightBuilder directionalLightBuilder;
	Engine::BuilderDirector lightBuilderDirector(directionalLightBuilder);
	lightBuilderDirector.Construct();
	Engine::GameNode* directionalLightNode = directionalLightBuilder.GetGameNode();
	AddToSceneRoot(directionalLightNode);
}

void TestGameManager::AddPointLights()
{
	if (pointLightCount < 1)
	{
		return;
	}

	Engine::PointLightBuilder pointLightBuilder;
	Engine::BuilderDirector lightBuilderDirector(pointLightBuilder);
	for (int i = 0; i < pointLightCount; ++i)
	{
		pointLightBuilder.SetLightIndex(i);
		lightBuilderDirector.Construct();
		Engine::GameNode* pointLightNode = pointLightBuilder.GetGameNode();
		AddToSceneRoot(pointLightNode);

		//GameNode* bulbNode = new GameNode();
		//bulbNode->AddComponent(new MeshRenderer(new Mesh("Bulb\\Bulb.obj") /* new Mesh("PointLight.obj") */, new Material(new Texture("PointLight.png"), 1.0f, 8.0f)));
		//bulbNode->GetTransform().SetPos(REAL_ZERO, REAL_ONE, REAL_ZERO);
		//bulbNode->GetTransform().SetScale(5.0f);
		//pointLightNode->AddChild(bulbNode);
	}
}

void TestGameManager::AddSpotLights()
{
	if (spotLightCount < 1)
	{
		return;
	}

	Engine::SpotLightBuilder spotLightBuilder;
	Engine::BuilderDirector lightBuilderDirector(spotLightBuilder);
	for (int i = 0; i < spotLightCount; ++i)
	{
		spotLightBuilder.SetLightIndex(i);
		lightBuilderDirector.Construct();
		Engine::GameNode* spotLightNode = spotLightBuilder.GetGameNode();
		AddToSceneRoot(spotLightNode);
	}
}

void TestGameManager::AddCameras(Engine::GameNode* entityToFollow)
{
	START_PROFILING;
	CHECK_CONDITION_EXIT_ALWAYS(cameraCount >= 1, Utility::Critical, "No cameras defined in the rendering engine.");

	NOTICE_LOG("Creating %d camera(-s)...", cameraCount);

	Engine::CameraBuilder cameraBuilder;
	Engine::BuilderDirector cameraBuilderDirector(cameraBuilder);
	for (int i = 0; i < cameraCount; ++i)
	{
		cameraBuilder.SetCameraIndex(i);
		cameraBuilder.SetEntityToFollow(entityToFollow);
		cameraBuilderDirector.Construct();
		Engine::GameNode* cameraNode = cameraBuilder.GetGameNode();
		AddToSceneRoot(cameraNode);
	}
	DEBUG_LOG("%d camera(-s) created", cameraCount);
	STOP_PROFILING;
}

void TestGameManager::AddSkybox() const
{
	START_PROFILING;

	NOTICE_LOG("Creating a skybox...");

	Engine::SkyboxBuilder skyboxBuilder;
	Engine::BuilderDirector skyboxBuilderDirector(skyboxBuilder);
	skyboxBuilderDirector.Construct();
	//GameNode* skyboxNode = skyboxBuilder.GetGameNode();
	DEBUG_LOG("The skybox has been created");
	STOP_PROFILING;
}

//
//void Game::CleanUp()
//{
//	stdlog(Info, LOGPLACE, "The game is being cleaned up");
//}

// TODO: Remove in the future
Real temp = 0.0;
bool isMouseLocked = false;

void TestGameManager::Update(Real delta)
{
	START_PROFILING;
	//for (std::vector<Engine::ParticleGenerator>::iterator particleGeneratorItr = m_particleGenerators.begin(); particleGeneratorItr != m_particleGenerators.end(); ++particleGeneratorItr)
	//{
	//	particleGeneratorItr->Update(delta);
	//}
	m_gameStateManager->Update(delta);
	STOP_PROFILING;
}

void TestGameManager::WindowResizeEvent(int width, int height)
{
	GameManager::WindowResizeEvent(width, height);
}

/**
* @param key the keyboard key that was pressed or released
* @param scancode the system-specific scancode of the key
* @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
* @param mods Bit field describing which modifier keys were held down
*/
void TestGameManager::KeyEvent(int key, int scancode, int action, int mods)
{
	GameManager::KeyEvent(key, scancode, action, mods);
	m_gameStateManager->KeyEvent(key, scancode, action, mods);
}

void TestGameManager::ScrollEvent(double xOffset, double yOffset)
{
	GameManager::ScrollEvent(xOffset, yOffset);
	m_gameStateManager->ScrollEvent(xOffset, yOffset);
}

/**
* GLFW_MOUSE_BUTTON_1 = left mouse button
* GLFW_MOUSE_BUTTON_2 = right mouse button
* GLFW_MOUSE_BUTTON_3 = middle mouse button
*/
void TestGameManager::MouseButtonEvent(int button, int action, int mods)
{
	// TODO: Pass the event to the Input function in the current game state.
	// TODO: Create additional functions for mouse, keyboard events (see IInputable class)
	// I would expect here something as follows:
	// currentGameState->MouseInput(...)

	// DEBUG_LOG("Mouse event: button=%d\t action=%d\t mods=%d", button, action, mods);

	//GameManager::MouseButtonEvent(window, button, action, mods);
	m_gameStateManager->MouseButtonEvent(button, action, mods);
}

void TestGameManager::MousePosEvent(double xPos, double yPos)
{
	m_gameStateManager->MousePosEvent(xPos, yPos);
}

#ifdef ANT_TWEAK_BAR_ENABLED
void TestGameManager::InitializeTweakBars()
{
#ifdef GAME_PROPERTIES_TWEAK_BAR
	if (!m_isGameLoaded)
	{
		WARNING_LOG("Cannot initialize game's tweak bars. The game has not been loaded yet.");
		return;
	}
	INFO_LOG("Initializing game's tweak bars");
	// TODO: GAME_PROPERTIES_TWEAK_BAR gives some errors. Investigate why and fix that!

	TwBar* testGamePropertiesBar = TwNewBar("TestGamePropertiesBar");
	TwAddVarRW(testGamePropertiesBar, "heightMapCalculationEnabled", TW_TYPE_BOOLCPP, &m_heightMapCalculationEnabled, " label='Heightmap calculation enabled' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainSpecularIntensity", TW_TYPE_FLOAT, &terrainSpecularIntensity, " label='Terrain specular intensity' group='Terrain' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainSpecularPower", TW_TYPE_FLOAT, &terrainSpecularPower, " label='Terrain specular power' group='Terrain' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainDisplacementScale", TW_TYPE_FLOAT, &terrainDisplacementScale, " label='Terrain displacement scale' group='Terrain' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainDisplacementOffset", TW_TYPE_FLOAT, &terrainDisplacementOffset, " label='Terrain displacement offset' group='Terrain' ");
	if (terrainMaterial == NULL)
	{
		ERROR_LOG("Cannot add terrain material information to tweak bar. The terrain material is NULL.");
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
	INFO_LOG("Initializing game's tweak bars finished");
#endif
}
#endif
