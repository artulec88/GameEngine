#include "stdafx.h"
#include "GameManager.h"
#include "GameStateManager.h"
#include "CoreEngine.h"
#include "Vertex.h"
#include "Renderer.h"
#include "Attenuation.h"
#include "PointLight.h"
#include "Texture.h"

#include "Math\Math.h"
#include "Math\Vector.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\ILogger.h"
#include "Utility\Time.h"

#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBar\include\AntTweakBar.h"
#endif

#include <fstream>

using namespace Utility;
using namespace Math;
using namespace Rendering;

GameManager* GameManager::s_gameManager = NULL;

/* static */ GameManager* GameManager::GetGameManager()
{
	return s_gameManager;
}

/* static */ void GameManager::Load(void* arg)
{
	GameManager* gameManager = static_cast<GameManager*>(arg);
	if (gameManager == NULL)
	{
		EMERGENCY_LOG("Cannot load the game. Passed parameter is not a game manager object.");
		exit(EXIT_FAILURE);
	}
	//CoreEngine::GetCoreEngine()->GetRenderer()
	glfwMakeContextCurrent(CoreEngine::GetCoreEngine()->GetRenderer()->GetThreadWindow());
	Rendering::InitGlew();
	// glew init
	gameManager->Load();
}

GameManager::GameManager() :
	m_rootGameNode(),
	m_terrainNode(NULL),
	m_gameStateManager(NULL),
	m_isGameLoaded(false)
{
	INFO_LOG("Game manager construction started");
	//rootGameNode = new GameNode();
	//CHECK_CONDITION_EXIT(rootGameNode != NULL, Critical, "Root game node construction failed.");

	if (GameManager::s_gameManager != NULL)
	{
		ERROR_LOG("Constructor called when a singleton instance of CoreEngine class has already been created");
		SAFE_DELETE(GameManager::s_gameManager);
	}
	m_gameStateManager = new DefaultGameStateManager();

	GameManager::s_gameManager = this;
	DEBUG_LOG("Game manager construction finished");
}


GameManager::~GameManager(void)
{
	INFO_LOG("Game manager destruction finished");
	//SAFE_DELETE(m_rootGameNode);
	SAFE_DELETE(m_gameStateManager);
	SAFE_DELETE(m_terrainNode);
	DEBUG_LOG("Game manager destruction finished");
}

void GameManager::WindowResizeEvent(int width, int height)
{
	NOTICE_LOG("Window resize event (width = %d, height = %d)", width, height);
	// TODO: Check if core engine finds out about the resizing of the window.
}

void GameManager::CloseWindowEvent()
{
	NOTICE_LOG("Close window event");
	CoreEngine::GetCoreEngine()->Stop();
}

/**
 * @param key the keyboard key that was pressed or released
 * @param scancode the system-specific scancode of the key
 * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
 * @param mods Bit field describing which modifier keys were held down
 */
void GameManager::KeyEvent(int key, int scancode, int action, int mods)
{
	DELOCUST_LOG("Key event with key = %d", key);

	//if (key == GLFW_KEY_ESCAPE)
	//{
	//	glfwSetWindowShouldClose(window, GL_TRUE);
	//	return;
	//}
	if ((key == GLFW_KEY_F3) && (mods & GLFW_MOD_ALT))
	{
		CoreEngine::GetCoreEngine()->Stop();
		return;
	}
}

void GameManager::MouseButtonEvent(int button, int action, int mods)
{
	DELOCUST_LOG("Mouse event: button=%d\t action=%d\t mods=%d", button, action, mods);

	/**
	 * GLFW_MOUSE_BUTTON_1 = left mouse button
	 * GLFW_MOUSE_BUTTON_2 = right mouse button
	 * GLFW_MOUSE_BUTTON_3 = middle mouse button
	 */

	switch (action)
	{
	case GLFW_PRESS:
		DEBUG_LOG("Mouse button pressed: button=%d\t mods=%d", button, mods);
		break;
	case GLFW_RELEASE:
		DEBUG_LOG("Mouse button released: button=%d\t mods=%d", button, mods);
		break;
	default:
		WARNING_LOG("Unknown action performed with the mouse");
	}
}

void GameManager::MousePosEvent(double xPos, double yPos)
{
	DEBUG_LOG("Mouse position event x=%.2f, y=%.2f", xPos, yPos);
}

void GameManager::ScrollEvent(double xOffset, double yOffset)
{
	DEBUG_LOG("Scroll event: xOffset=%.3f\t yOffset=%.3f", xOffset, yOffset);
}

//GameNode& GameManager::GetRootGameNode() const
//{
//	CHECK_CONDITION_EXIT(rootGameNode != NULL, Emergency, "Root game node is NULL.");
//	return *rootGameNode;
//}

//Shader* GameManager::GetShader() const
//{
//	CHECK_CONDITION(shader != NULL, Error, "Shader is NULL.");
//	return shader;
//}

void GameManager::AddToSceneRoot(GameNode* child)
{
	m_rootGameNode.AddChild(child);
}

void GameManager::AddWaterNode(GameNode* waterNode)
{
	CoreEngine::GetCoreEngine()->GetRenderer()->AddWaterNode(waterNode);
}

void GameManager::AddBillboardNode(GameNode* billboardNode)
{
	CoreEngine::GetCoreEngine()->GetRenderer()->AddBillboardNode(billboardNode);
}

void GameManager::AddTerrainNode(GameNode* terrainNode)
{
	CoreEngine::GetCoreEngine()->GetRenderer()->AddTerrainNode(terrainNode);
}

void GameManager::Render(Renderer* renderer) const
{
	m_gameStateManager->Render(NULL, renderer);
}

void GameManager::SetTransition(GameStateTransitioning::GameStateTransition* gameStateTransition)
{
	m_gameStateManager->SetTransition(gameStateTransition);
}

void GameManager::PerformStateTransition()
{
	m_gameStateManager->PerformStateTransition();
}

void GameManager::PopState()
{
	m_gameStateManager->Pop();
}

void GameManager::RequestGameQuit() const
{
	// TODO: Ask the user to save the game if the inGameState can be found in the game states stack
	//glfwSetWindowShouldClose(window, GL_TRUE); // Doesn't work, because we don't have a window

	CoreEngine::GetCoreEngine()->GetRenderer()->RequestWindowClose();
}

#ifdef ANT_TWEAK_BAR_ENABLED
void GameManager::InitializeTweakBars()
{
}
#endif