#include "TestGameManager.h"
#include "MenuGameState.h"
//#include "PlayGameState.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\Camera.h"
#include "Rendering\MeshRenderer.h"
#include "Rendering\LookAtComponent.h"
#include "Rendering\Color.h"
#include "Rendering\LightBuilder.h"
#include "Rendering\LightBuilderDirector.h"

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
	RESOURCES_TO_LOAD(32),
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
	ASSERT(!m_isGameLoaded);
	if (m_isGameLoaded)
	{
		ERROR_LOG("Loading the game will not be performed. The game has already been loaded.");
	}

	//Material bricks(new Texture("..\\Textures\\bricks.jpg"), specularIntensity, specularPower, Texture("..\\Textures\\bricks_normal.jpg"), Texture("..\\Textures\\bricks_disp.png"), 0.03f, -0.5f);
	//Material bricks2("bricks2_material", Texture("..\\Textures\\bricks2.jpg"), 0.0f, 0, Texture("..\\Textures\\bricks2_normal.jpg"), Texture("..\\Textures\\bricks2_disp.jpg"), 0.04f, -1.0f);
	//Material humanMaterial("human_material", Texture("..\\Textures\\HumanSkin.jpg"), 2, 32);

	m_terrainNode = new GameNode();
	m_terrainMesh = new TerrainMesh("..\\Models\\" + GET_CONFIG_VALUE_STR("terrainModel", "terrain02.obj"));
#ifndef ANT_TWEAK_BAR_ENABLED
	Math::Real terrainSpecularIntensity = GET_CONFIG_VALUE("defaultSpecularIntensity", 1.0f);
	Math::Real terrainSpecularPower = GET_CONFIG_VALUE("defaultSpecularPower", 8.0f);
	Math::Real terrainDisplacementScale = GET_CONFIG_VALUE("defaultDisplacementScale", 0.02f);
	Math::Real terrainDisplacementOffset = GET_CONFIG_VALUE("defaultDisplacementOffset", -0.5f);
#endif
	terrainMaterial = new Material(new Texture("..\\Textures\\" + GET_CONFIG_VALUE_STR("terrainDiffuseTexture", "grass2.jpg")), terrainSpecularIntensity, terrainSpecularPower,
		new Texture("..\\Textures\\" + GET_CONFIG_VALUE_STR("terrainNormalMap", "grass_normal.jpg")),
		new Texture("..\\Textures\\" + GET_CONFIG_VALUE_STR("terrainDisplacementMap", "grass_disp.jpg")), terrainDisplacementScale, terrainDisplacementOffset);
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	terrainMaterial->SetAdditionalTexture(new Texture("..\\Textures\\" + GET_CONFIG_VALUE_STR("terrainDiffuseTexture2", "rocks2.jpg")), "diffuse2");
	//terrainMaterial->SetAdditionalTexture(new Texture("..\\Textures\\" + GET_CONFIG_VALUE_STR("terrainMap", "terrainMap.jpg")), "terrainMap");
	m_resourcesLoaded += 1; // TODO: Consider creating some prettier solution. This is ugly
	m_terrainNode->AddComponent(new MeshRenderer(m_terrainMesh, terrainMaterial));
	//m_terrainNode->GetTransform().SetPos(0.0f, 0.0f, 5.0f);
	m_terrainNode->GetTransform().SetScale(20.0f);
	m_terrainMesh->TransformPositions(m_terrainNode->GetTransform().GetTransformation());
	//AddToSceneRoot(m_terrainNode); // Terrain node uses special shaders, so we don't actually add it to the game scene hierarchy. Instead we just register it for the renderer to use it.
	RegisterTerrainNode(m_terrainNode);

	boxNode = new GameNode();
#ifdef ANT_TWEAK_BAR_ENABLED
	boxMaterial = new Material(new Texture("..\\Textures\\crateBox2.jpg"), 1.0f, 2.0f);
	boxNode->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\SimpleCrate\\CrateModel.obj"), boxMaterial));
	m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
#else
	boxNode->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\SimpleCrate\\CrateModel.obj"), new Material(new Texture("..\\Textures\\crateBox2.jpg"), 1.0f, 2.0f)));
#endif
	boxNode->GetTransform().SetPos(12.0f, 3.5f, 9.0f);
	boxNode->GetTransform().SetScale(0.05f);
	AddToSceneRoot(boxNode);

	////Vector3D rayEndPosition = boxNode->GetTransform().GetTransformedPos() + boxNode->GetTransform().GetTransformedRot().GetForward() * 100.0f;
	////Vertex vertices [] = { Vertex(boxNode->GetTransform().GetTransformedPos()), Vertex(rayEndPosition) };
	////int indices [] = { 0, 1 };
	////boxNode->AddComponent(new MeshRenderer(
	////	new Mesh(vertices, 2, indices, 2, true, GL_LINES),
	////	new Material(new Texture("..\\Textures\\DirectionalLight.png"))));

	GameNode* boxNode2 = new GameNode();
	boxNode2->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\cube.obj"),
		new Material(new Texture("..\\Textures\\bricks2.jpg"), 1.0f, 8.0f,
			new Texture("..\\Textures\\bricks2_normal.jpg"),
			new Texture("..\\Textures\\bricks2_disp.jpg"), 0.04f, -1.0f)));
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	boxNode2->GetTransform().SetPos(8.0f, 0.5f, 5.0f);
	boxNode2->GetTransform().Rotate(Quaternion(Vector3D(0.0f, 1.0f, 0.0f), Angle(30.0f)));
	boxNode2->GetTransform().SetScale(0.5f);
	AddToSceneRoot(boxNode2);

	GameNode* testMesh1 = new GameNode();
	testMesh1->GetTransform().SetPos(-2.0f, 2.5f, 2.0f);
	testMesh1->GetTransform().SetScale(1.5f);
	GameNode* testMesh2 = new GameNode();
	testMesh2->GetTransform().SetPos(9.0f, 0.0f, 0.0f);
	//testMesh2->GetTransform().SetScale(1.5f);
	testMesh2->GetTransform().SetRot(Quaternion(Matrix4D(Angle(90.0f), Angle(90.0f), Angle(0.0f))));
	testMesh1->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\plane.obj"),
		new Material(new Texture("..\\Textures\\bricks2.jpg"), 0.0f, 0, new Texture("..\\Textures\\bricks2_normal.jpg"), new Texture("..\\Textures\\bricks2_disp.jpg"), 0.04f, -1.0f)));
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	testMesh2->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\plane.obj"),
		new Material(new Texture("..\\Textures\\bricks2.jpg"), 0.0f, 0)));
	AddToSceneRoot(testMesh1);
	//AddToSceneRoot(testMesh2);
	testMesh1->AddChild(testMesh2);

	GameNode* monkeyNode1 = new GameNode();
	monkeyNode1->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\monkey4.obj"),
		new Material(new Texture("..\\Textures\\chessboard3.jpg", GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR), 1.0f, 8.0f)));
	monkeyNode1->AddComponent(new LookAtComponent());
	m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	monkeyNode1->GetTransform().SetPos(-5.0f, 3.75f, 4.0f);
	//monkeyNode1->GetTransform().SetRotation(Quaternion(Vector3D(0, 1, 0), Angle(-45)));
	//stdlog(Info, LOGPLACE, "MonkeyNode1 has ID=%d", monkeyNode1->GetID());
	//monkeyNode1->AddComponent(new LookAtComponent());
	AddToSceneRoot(monkeyNode1);

	GameNode* monkeyNode2 = new GameNode();
	monkeyNode2->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\monkey3.obj"),
		new Material(new Texture("..\\Textures\\bricks.jpg"), 2.0f, 32.0f)));
	m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	monkeyNode2->GetTransform().SetPos(5.0, 3.0, 15.0);
	//monkeyNode2->AddComponent(new LookAtComponent());
	AddToSceneRoot(monkeyNode2);

	GameNode* earthNode = new GameNode();
	earthNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\earth.obj"), new Material(new Texture("..\\Textures\\earth.jpg"))));
	m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	earthNode->GetTransform().SetPos(12.0f, 2.0f, -2.0f);
	AddToSceneRoot(earthNode);

	GameNode* sphereNode = new GameNode();
	sphereNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\sphere.obj"), new Material(new Texture("..\\Textures\\rust.jpg"))));
	m_resourcesLoaded += 2;
	sphereNode->GetTransform().SetPos(-3.0f, 1.5f, -3.0f);
	AddToSceneRoot(sphereNode);

	GameNode* sphereNode2 = new GameNode();
	sphereNode2->AddComponent(new MeshRenderer(new Mesh("..\\Models\\sphere.obj"), new Material(new Texture("..\\Textures\\rust.jpg"))));
	m_resourcesLoaded += 2;
	sphereNode2->GetTransform().SetPos(-3.0f, 2.0f, 3.0f);
	AddToSceneRoot(sphereNode2);

	GameNode* sphereNode3 = new GameNode();
	sphereNode3->AddComponent(new MeshRenderer(new Mesh("..\\Models\\sphere.obj"), new Material(new Texture("..\\Textures\\rust.jpg"))));
	m_resourcesLoaded += 2;
	sphereNode3->GetTransform().SetPos(3.0f, 2.5f, -3.0f);
	AddToSceneRoot(sphereNode3);

	GameNode* sphereNode4 = new GameNode();
	sphereNode4->AddComponent(new MeshRenderer(new Mesh("..\\Models\\sphere.obj"), new Material(new Texture("..\\Textures\\rust.jpg"))));
	m_resourcesLoaded += 2;
	sphereNode4->GetTransform().SetPos(3.0f, 3.0f, 3.0f);
	AddToSceneRoot(sphereNode4);

	srand((unsigned int)time(NULL));

	humanNodes = new GameNode* [HUMAN_NODES_COUNT];
	for (int i = 0; i < HUMAN_NODES_COUNT; ++i)
	{
		humanNodes[i] = new GameNode();
		humanNodes[i]->AddComponent(new MeshRenderer(new Mesh("..\\Models\\BodyMesh.obj"), new Material(new Texture("..\\Textures\\HumanSkin.jpg"), 2.0f, 32.0f)));
		humanNodes[i]->GetTransform().SetPos(static_cast<Real>(rand() % 20), 1.7f, static_cast<Real>(rand() % 20));
		AddToSceneRoot(humanNodes[i]);
	}
	m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly

	//castleNode = new GameNode();
	//castleNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\castle.obj"), new Material(new Texture("..\\Textures\\HumanSkin.jpg"), 0.5f, 8.0f)));
	//castleNode->GetTransform().SetPos(static_cast<Real>(rand() % 50), 1.0f, static_cast<Real>(rand() % 50));
	//AddToSceneRoot(castleNode);

	GameNode* teapotNode = new GameNode();
	teapotNode->GetTransform().SetPos(3.0f, 4.0f, REAL_ONE);
	teapotNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\teapot.obj"), new Material(new Texture("..\\Textures\\chessboard3.jpg"))));
	m_resourcesLoaded += 1; // TODO: Consider creating some prettier solution. This is ugly
	AddToSceneRoot(teapotNode);

	AddLights(); // Adding all kinds of light (directional, point, spot)

	AddCameras(); // Adding cameras

	m_isGameLoaded = true;
	ASSERT(m_isGameLoaded);
	if (!m_isGameLoaded)
	{
		CRITICAL_LOG("The game has not been loaded properly");
	}
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
	INFO_LOG("Directional lights enabled");

	Rendering::DirectionalLightBuilder directionalLightBuilder;
	Rendering::LightBuilderDirector lightBuilderDirector(directionalLightBuilder);
	lightBuilderDirector.Construct();
	GameNode* directionalLightNode = directionalLightBuilder.GetLightNode();
	AddToSceneRoot(directionalLightNode);
}

void TestGameManager::AddPointLights()
{
	if (pointLightCount < 1)
	{
		return;
	}

	Rendering::PointLightBuilder pointLightBuilder;
	Rendering::LightBuilderDirector lightBuilderDirector(pointLightBuilder);
	for (int i = 0; i < pointLightCount; ++i)
	{
		pointLightBuilder.SetPointLightIndex(i);
		lightBuilderDirector.Construct();
		GameNode* pointLightNode = pointLightBuilder.GetLightNode();
		AddToSceneRoot(pointLightNode);
		
		//GameNode* bulbNode = new GameNode();
		//bulbNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\Bulb\\Bulb.obj") /* new Mesh("..\\Models\\PointLight.obj") */, new Material(new Texture("..\\Textures\\PointLight.png"), 1.0f, 8.0f)));
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
	Rendering::LightBuilderDirector lightBuilderDirector(spotLightBuilder);
	for (int i = 0; i < spotLightCount; ++i)
	{
		spotLightBuilder.SetSpotLightIndex(i);
		lightBuilderDirector.Construct();
		GameNode* spotLightNode = spotLightBuilder.GetLightNode();
		AddToSceneRoot(spotLightNode);
	}
}

void TestGameManager::AddCameras()
{
	START_PROFILING;
	if (cameraCount < 1)
	{
		ERROR_LOG("No cameras defined.");
		exit(EXIT_FAILURE);
	}
	
	NOTICE_LOG("Creating %d camera(-s)...", cameraCount);

	const Real defaultFoV = GET_CONFIG_VALUE("defaultCameraFoV", 70.0f);
	const Real defaultAspectRatio = GET_CONFIG_VALUE("defaultCameraAspectRatio", static_cast<Real>(800) / 600);
	const Real defaultNearPlane = GET_CONFIG_VALUE("defaultCameraNearPlane", 0.1f);
	const Real defaultFarPlane = GET_CONFIG_VALUE("defaultCameraFarPlane", 1000.0f);
	
	const Vector3D defaultCameraPos(GET_CONFIG_VALUE("defaultCameraPosX", 0.0f), GET_CONFIG_VALUE("defaultCameraPosY", 0.0f), GET_CONFIG_VALUE("defaultCameraPosZ", 0.0f));
	const Angle defaultCameraRotationX(GET_CONFIG_VALUE("defaultCameraAngleX", -45.0f));
	const Angle defaultCameraRotationY(GET_CONFIG_VALUE("defaultCameraAngleY", 0.0f));
	const Angle defaultCameraRotationZ(GET_CONFIG_VALUE("defaultCameraAngleZ", 0.0f));

	cameraNodes = new GameNode* [cameraCount];
	for (int i = 0; i < cameraCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		std::string cameraIndexStr = ss.str();

		cameraNodes[i] = new GameNode();
		Real xPos = GET_CONFIG_VALUE("cameraPosX_" + cameraIndexStr, defaultCameraPos.GetX());
		Real yPos = GET_CONFIG_VALUE("cameraPosY_" + cameraIndexStr, defaultCameraPos.GetY());
		Real zPos = GET_CONFIG_VALUE("cameraPosZ_" + cameraIndexStr, defaultCameraPos.GetZ());
		cameraNodes[i]->GetTransform().SetPos(xPos, yPos, zPos);
		
		Angle angleX(GET_CONFIG_VALUE("cameraAngleX_" + cameraIndexStr, defaultCameraRotationX.GetAngleInDegrees()));
		Angle angleY(GET_CONFIG_VALUE("cameraAngleY_" + cameraIndexStr, defaultCameraRotationY.GetAngleInDegrees()));
		Angle angleZ(GET_CONFIG_VALUE("cameraAngleZ_" + cameraIndexStr, defaultCameraRotationZ.GetAngleInDegrees()));
		Matrix4D rotMatrix(angleX, angleY, angleZ);
		DELOCUST_LOG("angleX=%.1f, angleY=%.1f, angleZ=%.1f", angleX.GetAngleInDegrees(), angleY.GetAngleInDegrees(), angleZ.GetAngleInDegrees());
		Quaternion rot(rotMatrix);
		Quaternion rot2(Vector3D(1, 0, 0), angleX);
		//DELOCUST_LOG("rotMatrix =\n%s\n rot =\n%s\n rot.ToRotationMatrix() =\n%s\n rot2.ToRotationMatrix() = \n%s",
		//	rotMatrix.ToString().c_str(),
		//	rot.ToString().c_str(),
		//	rot.ToRotationMatrix().ToString().c_str(),
		//	rot2.ToRotationMatrix().ToString().c_str());
		cameraNodes[i]->GetTransform().SetRot(rot);

		Angle fov(GET_CONFIG_VALUE("cameraFoV_" + cameraIndexStr, defaultFoV), Unit::DEGREE);
		Real aspectRatio = GET_CONFIG_VALUE("cameraAspectRatio_" + cameraIndexStr, defaultAspectRatio);
		Real zNearPlane = GET_CONFIG_VALUE("cameraNearPlane_" + cameraIndexStr, defaultNearPlane);
		Real zFarPlane = GET_CONFIG_VALUE("cameraFarPlane_" + cameraIndexStr, defaultFarPlane);
		cameraNodes[i]->AddComponent(new CameraComponent(fov, aspectRatio, zNearPlane, zFarPlane));
		//testMesh2->AddChild(cameraNodes[i]);
		AddToSceneRoot(cameraNodes[i]);
	}
	DEBUG_LOG("%d camera(-s) created", cameraCount);
	STOP_PROFILING;
}

//
//void Game::CleanUp()
//{
//	stdlog(Info, LOGPLACE, "The game is being cleaned up");
//}

void TestGameManager::Input(Real delta)
{
	START_PROFILING;
	m_gameStateManager->Input(delta);
	STOP_PROFILING;
}

// TODO: Remove in the future
Real temp = 0.0;
bool isMouseLocked = false;

void TestGameManager::Update(Real delta)
{
	START_PROFILING;
	m_gameStateManager->Update(delta);
	STOP_PROFILING;
}

void TestGameManager::WindowResizeEvent(GLFWwindow* window, int width, int height)
{
	GameManager::WindowResizeEvent(window, width, height);
}

/**
 * @param key the keyboard key that was pressed or released
 * @param scancode the system-specific scancode of the key
 * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
 * @param mods Bit field describing which modifier keys were held down
 */
void TestGameManager::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GameManager::KeyEvent(window, key, scancode, action, mods);
	m_gameStateManager->KeyEvent(key, scancode, action, mods);
}

/**
 * GLFW_MOUSE_BUTTON_1 = left mouse button
 * GLFW_MOUSE_BUTTON_2 = right mouse button
 * GLFW_MOUSE_BUTTON_3 = middle mouse button
 */
void TestGameManager::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	// TODO: Pass the event to the Input function in the current game state.
	// TODO: Create additional functions for mouse, keyboard events (see IInputable class)
	// I would expect here something as follows:
	// currentGameState->MouseInput(...)
	
	// DEBUG_LOG("Mouse event: button=%d\t action=%d\t mods=%d", button, action, mods);

	//GameManager::MouseButtonEvent(window, button, action, mods);
	m_gameStateManager->MouseButtonEvent(button, action, mods);
}

void TestGameManager::MousePosEvent(GLFWwindow* window, double xPos, double yPos)
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
