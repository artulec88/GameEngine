#include "stdafx.h"
#include "GameManager.h"
#include "GameStateManager.h"
#include "CoreEngine.h"
#include "Vertex.h"
#include "Input.h"
#include "Renderer.h"
#include "Attenuation.h"
#include "PointLight.h"
#include "Texture.h"

#include "Math\Math.h"
#include "Math\Vector.h"

#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
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

GameManager::GameManager() :
	m_rootGameNode(),
	m_gameStateManager(NULL)
{
	LOG(Info, LOGPLACE, "Game manager construction started");
	//rootGameNode = new GameNode();
	//ASSERT(rootGameNode != NULL);
	//if (rootGameNode == NULL)
	//{
	//	LOG(Critical, LOGPLACE, "Root game node construction failed");
	//	exit(EXIT_FAILURE);
	//}

	if (GameManager::s_gameManager != NULL)
	{
		LOG(Error, LOGPLACE, "Constructor called when a singleton instance of CoreEngine class has already been created");
		SAFE_DELETE(GameManager::s_gameManager);
	}
	m_gameStateManager = new DefaultGameStateManager();

	GameManager::s_gameManager = this;
	LOG(Debug, LOGPLACE, "Game manager construction finished");
}


GameManager::~GameManager(void)
{
	LOG(Info, LOGPLACE, "Game manager destruction finished");
	//SAFE_DELETE(m_rootGameNode);
	SAFE_DELETE(m_gameStateManager);
	LOG(Debug, LOGPLACE, "Game manager destruction finished");
}

void GameManager::SetEngine(CoreEngine* coreEngine)
{
	//if (rootGameNode == NULL)
	//{
	//	LOG(Critical, LOGPLACE, "Cannot set core engine. Root game node is NULL.");
	//	exit(EXIT_FAILURE);
	//}
	m_rootGameNode.SetEngine(coreEngine);
}

void GameManager::Init()
{
	//if (rootGameNode == NULL)
	//{
	//	LOG(Critical, LOGPLACE, "Root game node is not initialized");
	//	exit(EXIT_FAILURE);
	//}
	//rootGameNode->Init();
}

/* static */ void GameManager::WindowCloseEventCallback(GLFWwindow* window)
{
	GetGameManager()->CloseWindowEvent(window);
}

/* static */ void GameManager::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if ( !TwWindowSize(width, height) )
	{
		GetGameManager()->WindowResizeEvent(window, width, height);
	}
#else
	GetGameManager()->CloseWindowEvent(window);
#endif
}

/* static */ void GameManager::KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if( !TwEventKeyGLFW(key, action) )  // send event to AntTweakBar
    {
		// event has not been handled by AntTweakBar
		GetGameManager()->KeyEvent(window, key, scancode, action, mods);
    }
#else
	GetGameManager()->KeyEvent(window, key, scancode, action, mods);
#endif
}

/* static */ void GameManager::MouseEventCallback(GLFWwindow* window, int button, int action, int mods)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if ( !TwEventMouseButtonGLFW(button, action) )
	{
		// event has not been handled by AntTweakBar
		GetGameManager()->MouseButtonEvent(window, button, action, mods);
	}
#else
	GetGameManager()->MouseButtonEvent(window, button, action, mods);
#endif
}

/* static */ void GameManager::MousePosCallback(GLFWwindow* window, double xPos, double yPos)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventMousePosGLFW(static_cast<int>(xPos), static_cast<int>(yPos)))
	{
		// event has not been handled by AntTweakBar
		GetGameManager()->MousePosEvent(window, xPos, yPos);
	}
#else
	GetGameManager()->MousePosEvent(window, xPos, yPos);
#endif
}

/* static */ void GameManager::ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if ( !TwEventMouseWheelGLFW(static_cast<int>(yOffset)) ) // TODO: Check if yOffset here is ok
	{
		// event has not been handled by AntTweakBar
		GetGameManager()->ScrollEvent(window, xOffset, yOffset);
	}
#else
	GetGameManager()->ScrollEvent(window, xOffset, yOffset);
#endif
}

void GameManager::WindowResizeEvent(GLFWwindow* window, int width, int height)
{
	LOG(Notice, LOGPLACE, "Window resize event (width = %d, height = %d)", width, height);
}

void GameManager::CloseWindowEvent(GLFWwindow* window)
{
	LOG(Notice, LOGPLACE, "Close window event");
	glfwSetWindowShouldClose(window, GL_TRUE);
}

/**
 * @param key the keyboard key that was pressed or released
 * @param scancode the system-specific scancode of the key
 * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
 * @param mods Bit field describing which modifier keys were held down
 */
void GameManager::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	LOG(Delocust, LOGPLACE, "Key event with key = %d", key);

	//Input::UpdateKey(window, key, scancode, action, mods);
	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
}

void GameManager::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	LOG(Delocust, LOGPLACE, "Mouse event: button=%d\t action=%d\t mods=%d", button, action, mods);

	/**
	 * GLFW_MOUSE_BUTTON_1 = left mouse button
	 * GLFW_MOUSE_BUTTON_2 = right mouse button
	 * GLFW_MOUSE_BUTTON_3 = middle mouse button
	 */

	switch (action)
	{
	case GLFW_PRESS:
		LOG(Debug, LOGPLACE, "Mouse button pressed: button=%d\t mods=%d", button, mods);
		break;
	case GLFW_RELEASE:
		LOG(Debug, LOGPLACE, "Mouse button released: button=%d\t mods=%d", button, mods);
		break;
	default:
		LOG(Warning, LOGPLACE, "Unknown action performed with the mouse");
	}
}

void GameManager::MousePosEvent(GLFWwindow* window, double xPos, double yPos)
{

}

void GameManager::ScrollEvent(GLFWwindow* window, double xOffset, double yOffset)
{
	LOG(Debug, LOGPLACE, "Scroll event: xOffset=%.3f\t yOffset=%.3f", xOffset, yOffset);
}

//GameNode& GameManager::GetRootGameNode() const
//{
//	ASSERT(rootGameNode != NULL);
//	if (rootGameNode == NULL)
//	{
//		LOG(Emergency, LOGPLACE, "Root game node is NULL.");
//		exit(EXIT_FAILURE);
//	//	// TODO: throw another exception in the future
//	//	throw FileNotFoundException();
//	}
//
//	return *rootGameNode;
//}

//Shader* GameManager::GetShader() const
//{
//	ASSERT(shader != NULL);
//	if (shader == NULL)
//	{
//		LOG(Error, LOGPLACE, "Shader is NULL");
//	}
//	return shader;
//}

void GameManager::AddToSceneRoot(GameNode* child)
{
	m_rootGameNode.AddChild(child);
}

void GameManager::Render(Renderer* renderer)
{
	m_gameStateManager->Render(renderer, GetRootGameNode());
}

#ifdef ANT_TWEAK_BAR_ENABLED
void GameManager::InitializeTweakBars()
{
}
#endif