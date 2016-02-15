#include "TestGameManager.h"
#include "MenuGameState.h"
//#include "PlayGameState.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\Camera.h"
#include "Rendering\MeshRenderer.h"
#include "Rendering\MoveComponent.h"
#include "Rendering\LookAtComponent.h"
#include "Rendering\Color.h"
#include "Rendering\Builder.h"
#include "Rendering\BuilderDirector.h"

#include "Math\FloatingPoint.h"
#include "Math\Quaternion.h"
//#include "Math\Vector.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

#include <sstream>

using namespace Game;
using namespace Utility;
using namespace Math;
using namespace Rendering;

TestGameManager::TestGameManager() :
	GameManager(),
	RESOURCES_TO_LOAD(26),
	CAMERA_HEIGHT_UPDATE_INTERVAL(GET_CONFIG_VALUE("cameraHeightUpdateInterval", 0.01f)),
	m_resourcesLoaded(0),
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
	,m_classStats(STATS_STORAGE.GetClassStats("TestGameManager"))
#endif
{
	DEBUG_LOG("TestGame is being constructed");

	m_gameStateManager->Push(new MenuGameState());
	//m_gameStateManager->Push(new InGameState());
	srand((unsigned int)time(NULL));
}


TestGameManager::~TestGameManager(void)
{
	SAFE_DELETE_JUST_TABLE(humanNodes);
	SAFE_DELETE_JUST_TABLE(cameraNodes);
}

Math::Real TestGameManager::GetLoadingProgress() const
{
	if (m_resourcesLoaded > RESOURCES_TO_LOAD)
	{
		WARNING_LOG("Resources loaded (%d) exceeds the total number of expected resources (%d)", m_resourcesLoaded, RESOURCES_TO_LOAD);
		return REAL_ONE;
	}
	return static_cast<Math::Real>(m_resourcesLoaded) / RESOURCES_TO_LOAD;
}

void TestGameManager::Load()
{
	NOTICE_LOG("Initalizing test game");
	START_PROFILING;
	CHECK_CONDITION_ALWAYS(!m_isGameLoaded, Utility::Error, "Loading the game run into a problem. The game has already been loaded.");

	//Material bricks(new Texture("bricks.jpg"), specularIntensity, specularPower, Texture("bricks_normal.jpg"), Texture("bricks_disp.png"), 0.03f, -0.5f);
	//Material bricks2("bricks2_material", Texture("bricks2.jpg"), 0.0f, 0, Texture("bricks2_normal.jpg"), Texture("bricks2_disp.jpg"), 0.04f, -1.0f);
	//Material humanMaterial("human_material", Texture("HumanSkin.jpg"), 2, 32);

	m_terrainNode = new GameNode();
	//m_terrainMesh = new TerrainMesh(REAL_ZERO, REAL_ZERO);
	m_terrainMesh = new TerrainMesh(GET_CONFIG_VALUE_STR("terrainModel", "terrain02.obj"));
#ifndef ANT_TWEAK_BAR_ENABLED
	Math::Real terrainSpecularIntensity = GET_CONFIG_VALUE("defaultSpecularIntensity", 1.0f);
	Math::Real terrainSpecularPower = GET_CONFIG_VALUE("defaultSpecularPower", 8.0f);
	Math::Real terrainDisplacementScale = GET_CONFIG_VALUE("defaultDisplacementScale", 0.02f);
	Math::Real terrainDisplacementOffset = GET_CONFIG_VALUE("defaultDisplacementOffset", -0.5f);
	Rendering::Material* terrainMaterial = new Material(new Texture(GET_CONFIG_VALUE_STR("terrainDiffuseTexture", "grass2.jpg")), terrainSpecularIntensity, terrainSpecularPower,
		new Texture(GET_CONFIG_VALUE_STR("terrainNormalMap", "grass_normal.jpg")),
		new Texture(GET_CONFIG_VALUE_STR("terrainDisplacementMap", "grass_disp.jpg")), terrainDisplacementScale, terrainDisplacementOffset);
#else
	terrainMaterial = new Material(new Texture(GET_CONFIG_VALUE_STR("terrainDiffuseTexture", "grass4.jpg")), terrainSpecularIntensity, terrainSpecularPower,
		new Texture(GET_CONFIG_VALUE_STR("terrainNormalMap", "grass_normal.jpg")),
		new Texture(GET_CONFIG_VALUE_STR("terrainDisplacementMap", "grass_disp.jpg")), terrainDisplacementScale, terrainDisplacementOffset);
#endif
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	terrainMaterial->SetAdditionalTexture(new Texture(GET_CONFIG_VALUE_STR("terrainBlendMap", "terrainBlendMap.png")), "blendMap");
	terrainMaterial->SetAdditionalTexture(new Texture(GET_CONFIG_VALUE_STR("terrainDiffuseTexture2", "rocks2.jpg")), "diffuse2");
	terrainMaterial->SetAdditionalTexture(new Texture(GET_CONFIG_VALUE_STR("terrainDiffuseTexture3", "mud.png")), "diffuse3");
	terrainMaterial->SetAdditionalTexture(new Texture(GET_CONFIG_VALUE_STR("terrainDiffuseTexture4", "path.png")), "diffuse4");
	//terrainMaterial->SetAdditionalTexture(new Texture(GET_CONFIG_VALUE_STR("terrainMap", "terrainMap.jpg")), "terrainMap");
	m_resourcesLoaded += 1; // TODO: Consider creating some prettier solution. This is ugly
	m_terrainNode->AddComponent(new MeshRenderer(m_terrainMesh, terrainMaterial));
	//m_terrainNode->GetTransform().SetPos(0.0f, 0.0f, 5.0f);
	//m_terrainNode->GetTransform().SetScale(20.0f);
	m_terrainMesh->TransformPositions(m_terrainNode->GetTransform().GetTransformation());
	//AddToSceneRoot(m_terrainNode); // Terrain node uses special shaders, so we don't actually add it to the game scene hierarchy. Instead we just register it for the renderer to use it.
	AddTerrainNode(m_terrainNode);

	////Vector3D rayEndPosition = boxNode->GetTransform().GetTransformedPos() + boxNode->GetTransform().GetTransformedRot().GetForward() * 100.0f;
	////Vertex vertices [] = { Vertex(boxNode->GetTransform().GetTransformedPos()), Vertex(rayEndPosition) };
	////int indices [] = { 0, 1 };
	////boxNode->AddComponent(new MeshRenderer(
	////	new Mesh(vertices, 2, indices, 2, true, GL_LINES),
	////	new Material(new Texture("DirectionalLight.png"))));

	GameNode* testMesh1 = new GameNode();
	testMesh1->GetTransform().SetPos(-2.0f, m_terrainMesh->GetHeightAt(Math::Vector2D(-2.0f, 2.0f)), 2.0f);
	testMesh1->GetTransform().SetRot(Quaternion(REAL_ZERO, sqrtf(2.0f) / 2, sqrtf(2.0f) / 2, REAL_ZERO));
	testMesh1->GetTransform().SetScale(0.1f);
	GameNode* testMesh2 = new GameNode();
	testMesh2->GetTransform().SetPos(9.0f, 0.0f, 0.0f);
	//testMesh2->GetTransform().SetScale(1.5f);
	testMesh2->GetTransform().SetRot(Quaternion(Matrix4D(Angle(90.0f), Angle(90.0f), Angle(0.0f))));
	testMesh1->AddComponent(new MeshRenderer(
		new Mesh("plane.obj"),
		new Material(new Texture("bricks2.jpg"), 0.0f, 0, new Texture("bricks2_normal.jpg"), new Texture("bricks2_disp.jpg"), 0.04f, -1.0f)));
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	testMesh2->AddComponent(new MeshRenderer(
		new Mesh("plane.obj"),
		new Material(new Texture("bricks2.jpg"), 0.0f, 0)));
	AddToSceneRoot(testMesh1);
	//AddToSceneRoot(testMesh2);
	testMesh1->AddChild(testMesh2);

	//GameNode* monkeyNode1 = new GameNode();
	//monkeyNode1->AddComponent(new MeshRenderer(
	//	new Mesh("monkey4.obj"),
	//	new Material(new Texture("chessboard3.jpg", GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR), 1.0f, 8.0f)));
	//monkeyNode1->AddComponent(new LookAtComponent());
	//m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	//monkeyNode1->GetTransform().SetPos(-5.0f, 3.75f, 4.0f);
	////monkeyNode1->GetTransform().SetRotation(Quaternion(Vector3D(0, 1, 0), Angle(-45)));
	////INFO_LOG("MonkeyNode1 has ID=%d", monkeyNode1->GetID());
	////monkeyNode1->AddComponent(new LookAtComponent());
	//AddToSceneRoot(monkeyNode1);

	//GameNode* monkeyNode2 = new GameNode();
	//monkeyNode2->AddComponent(new MeshRenderer(
	//	new Mesh("monkey3.obj"),
	//	new Material(new Texture("bricks.jpg"), 2.0f, 32.0f)));
	//m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	//monkeyNode2->GetTransform().SetPos(5.0, 3.0, 15.0);
	////monkeyNode2->AddComponent(new LookAtComponent());
	//AddToSceneRoot(monkeyNode2);

	GameNode* waterNode = new GameNode();
	// It seems we have a problem with sharing resources. If I use the plane.obj (which I use in other entities) then we'll have problems with rendering (e.g. disappearing billboards).
	// If I change it to myPlane.obj which is not used in other entities the errors seem to be gone.
	waterNode->AddComponent(new MeshRenderer(new Mesh("myPlane.obj"), NULL /* The NULL material fixes the problem with rendering both billboards and water nodes simultaneously. TODO: But why / how? */));
	m_resourcesLoaded += 2;
	waterNode->GetTransform().SetPos(GET_CONFIG_VALUE("waterNodePosX", -18.0f), GET_CONFIG_VALUE("waterNodePosY", 0.0f), GET_CONFIG_VALUE("waterNodePosZ", -12.0f));
	waterNode->GetTransform().SetScale(3.0f);
	AddWaterNode(waterNode);

	for (int i = 0; i < 20; ++i)
	{
		GameNode* billboardNode = new GameNode();
		Math::Real x = static_cast<Real>(rand() % 50) - 25.0f;
		Math::Real z = static_cast<Real>(rand() % 50) - 25.0f;
		Math::Real y = m_terrainMesh->GetHeightAt(Math::Vector2D(x, z));
		billboardNode->AddComponent(new MeshRenderer(new BillboardMesh(Math::Vector3D(x, y, z)), new Material(new Texture(GET_CONFIG_VALUE_STR("billboardTreeTexture", "Tree1.png")))));
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

	GameNode* playerNode = new GameNode();
	const Math::Real playerPositionX = 3.0f;
	const Math::Real playerPositionZ = 1.0f;
	const Math::Real playerPositionY = m_terrainMesh->GetHeightAt(Math::Vector2D(playerPositionX, playerPositionZ));
	playerNode->GetTransform().SetPos(playerPositionX, playerPositionY, playerPositionZ);
	playerNode->AddComponent(new MeshRenderer(new Mesh("person.obj"), new Material(new Texture("player.png"))));
	playerNode->AddComponent(new MoveComponent(0.26f, Math::Angle(152.0f, Math::Unit::DEGREE), 0.015f, 0.0002f));
	playerNode->GetTransform().SetScale(0.005f);
	m_resourcesLoaded += 2;
	AddToSceneRoot(playerNode);

	AddLights(); // Adding all kinds of light (directional, point, spot)

	AddCameras(playerNode); // Adding cameras

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

	Rendering::DirectionalLightBuilder directionalLightBuilder;
	Rendering::BuilderDirector lightBuilderDirector(directionalLightBuilder);
	lightBuilderDirector.Construct();
	GameNode* directionalLightNode = directionalLightBuilder.GetGameNode();
	AddToSceneRoot(directionalLightNode);
}

void TestGameManager::AddPointLights()
{
	if (pointLightCount < 1)
	{
		return;
	}

	Rendering::PointLightBuilder pointLightBuilder;
	Rendering::BuilderDirector lightBuilderDirector(pointLightBuilder);
	for (int i = 0; i < pointLightCount; ++i)
	{
		pointLightBuilder.SetLightIndex(i);
		lightBuilderDirector.Construct();
		GameNode* pointLightNode = pointLightBuilder.GetGameNode();
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

	Rendering::SpotLightBuilder spotLightBuilder;
	Rendering::BuilderDirector lightBuilderDirector(spotLightBuilder);
	for (int i = 0; i < spotLightCount; ++i)
	{
		spotLightBuilder.SetLightIndex(i);
		lightBuilderDirector.Construct();
		GameNode* spotLightNode = spotLightBuilder.GetGameNode();
		AddToSceneRoot(spotLightNode);
	}
}

void TestGameManager::AddCameras(GameNode* entityToFollow)
{
	START_PROFILING;
	CHECK_CONDITION_EXIT_ALWAYS(cameraCount >= 1, Utility::Critical, "No cameras defined in the rendering engine.");
	
	NOTICE_LOG("Creating %d camera(-s)...", cameraCount);

	Rendering::CameraBuilder cameraBuilder;
	Rendering::BuilderDirector cameraBuilderDirector(cameraBuilder);
	for (int i = 0; i < cameraCount; ++i)
	{
		cameraBuilder.SetCameraIndex(i);
		cameraBuilder.SetEntityToFollow(entityToFollow);
		cameraBuilderDirector.Construct();
		GameNode* cameraNode = cameraBuilder.GetGameNode();
		AddToSceneRoot(cameraNode);
	}
	DEBUG_LOG("%d camera(-s) created", cameraCount);
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
