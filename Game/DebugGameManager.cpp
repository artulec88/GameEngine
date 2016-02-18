#include "DebugGameManager.h"
#include "MenuGameState.h"
//#include "PlayGameState.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\Camera.h"
#include "Rendering\MeshRenderer.h"
#include "Rendering\LookAtComponent.h"
#include "Rendering\Color.h"
#include "Rendering\Builder.h"
#include "Rendering\BuilderDirector.h"

#include "Math\FloatingPoint.h"
#include "Math\Quaternion.h"

#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

#include <sstream>

using namespace Game;
using namespace Utility;
using namespace Math;
using namespace Rendering;

DebugGameManager::DebugGameManager() :
	GameManager(),
	M_RESOURCES_TO_LOAD(1),
	m_resourcesLoaded(0),
	m_cameraCount(GET_CONFIG_VALUE("cameraCount", 3)),
	m_cameraNodes(NULL)
#ifdef CALCULATE_GAME_STATS
	, m_classStats(STATS_STORAGE.GetClassStats("DebugGameManager"))
#endif
{
	DEBUG_LOG("DebugGame is being constructed");

	m_gameStateManager->Push(new MenuGameState());
	//m_gameStateManager->Push(new InGameState());
}


DebugGameManager::~DebugGameManager(void)
{
	SAFE_DELETE_JUST_TABLE(m_cameraNodes);
}

Math::Real DebugGameManager::GetLoadingProgress() const
{
	if (m_resourcesLoaded > M_RESOURCES_TO_LOAD)
	{
		WARNING_LOG("Resources loaded (%d) exceeds the total number of expected resources (%d)", m_resourcesLoaded, M_RESOURCES_TO_LOAD);
		return REAL_ONE;
	}
	return static_cast<Math::Real>(m_resourcesLoaded) / M_RESOURCES_TO_LOAD;
}

void DebugGameManager::Load()
{
	NOTICE_LOG("Initalizing debug game");
	START_PROFILING;
	CHECK_CONDITION_ALWAYS(!m_isGameLoaded, Utility::Error, "Loading the game run into a problem. The game has already been loaded.");

	GameNode* triangleNode = new GameNode();
	Material* triangleMaterial = new Material(new Texture("crateBox2.jpg"), 1.0f, 2.0f);
	//Vertex vertices[] = { Vertex(Vector3D(-1, -1, 0)), Vertex(Vector3D(0, 1, 0)), Vertex(Vector3D(1, -1, 0)) };
	//int indices[] = { 0, 1, 2, 0 };
	//Mesh* triangleMesh = new Mesh(vertices, 3, indices, 4);
	//triangleNode->AddComponent(new MeshRenderer(triangleMesh, triangleMaterial));
	triangleNode->GetTransform().SetPos(12.0f, 3.5f, 9.0f);
	triangleNode->GetTransform().SetScale(0.05f);
	AddToSceneRoot(triangleNode);

	m_resourcesLoaded += 1; // TODO: Consider creating some prettier solution. This is ugly

	AddCameras(triangleNode); // Adding cameras

	m_isGameLoaded = true;
	CHECK_CONDITION_ALWAYS(m_isGameLoaded, Utility::Critical, "The game has not been loaded properly.");
	STOP_PROFILING;
	NOTICE_LOG("Initalizing debug game finished");
}

void DebugGameManager::AddCameras(Rendering::GameNode* entityToFollow)
{
	START_PROFILING;
	int cameraCount = 3;
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

void DebugGameManager::Update(Real delta)
{
	START_PROFILING;
	m_gameStateManager->Update(delta);
	STOP_PROFILING;
}

void DebugGameManager::WindowResizeEvent(int width, int height)
{
	GameManager::WindowResizeEvent(width, height);
}

/**
* @param key the keyboard key that was pressed or released
* @param scancode the system-specific scancode of the key
* @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
* @param mods Bit field describing which modifier keys were held down
*/
void DebugGameManager::KeyEvent(int key, int scancode, int action, int mods)
{
	GameManager::KeyEvent(key, scancode, action, mods);
	m_gameStateManager->KeyEvent(key, scancode, action, mods);
}

/**
* GLFW_MOUSE_BUTTON_1 = left mouse button
* GLFW_MOUSE_BUTTON_2 = right mouse button
* GLFW_MOUSE_BUTTON_3 = middle mouse button
*/
void DebugGameManager::MouseButtonEvent(int button, int action, int mods)
{
	// TODO: Pass the event to the Input function in the current game state.
	// TODO: Create additional functions for mouse, keyboard events (see IInputable class)
	// I would expect here something as follows:
	// currentGameState->MouseInput(...)

	// DEBUG_LOG("Mouse event: button=%d\t action=%d\t mods=%d", button, action, mods);

	//GameManager::MouseButtonEvent(window, button, action, mods);
	m_gameStateManager->MouseButtonEvent(button, action, mods);
}

void DebugGameManager::MousePosEvent(double xPos, double yPos)
{
	m_gameStateManager->MousePosEvent(xPos, yPos);
}

#ifdef ANT_TWEAK_BAR_ENABLED
void DebugGameManager::InitializeTweakBars()
{
#ifdef GAME_PROPERTIES_TWEAK_BAR
	if (!m_isGameLoaded)
	{
		WARNING_LOG("Cannot initialize game's tweak bars. The game has not been loaded yet.");
		return;
	}
	INFO_LOG("Initializing game's tweak bars");
	// TODO: GAME_PROPERTIES_TWEAK_BAR gives some errors. Investigate why and fix that!

	TwBar* testGamePropertiesBar = TwNewBar("DebugGamePropertiesBar");

	TwSetParam(testGamePropertiesBar, NULL, "visible", TW_PARAM_CSTRING, 1, "true"); // Hide the bar at startup
	INFO_LOG("Initializing game's tweak bars finished");
#endif
}
#endif
