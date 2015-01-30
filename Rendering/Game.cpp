#include "stdafx.h"
#include "Game.h"
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

Game* Game::s_game = NULL;

/* static */ Game* Game::GetGame()
{
	return s_game;
}

Game::Game()
{
	LOG(Debug, LOGPLACE, "Game construction started");
	//rootGameNode = new GameNode();
	//ASSERT(rootGameNode != NULL);
	//if (rootGameNode == NULL)
	//{
	//	LOG(Critical, LOGPLACE, "Root game node construction failed");
	//	exit(EXIT_FAILURE);
	//}

	if (Game::s_game != NULL)
	{
		LOG(Error, LOGPLACE, "Constructor called when a singleton instance of MainApp class has already been created");
		SAFE_DELETE(Game::s_game);
	}
	Game::s_game = this;
	LOG(Debug, LOGPLACE, "Game construction finished");
}


Game::~Game(void)
{
	//SAFE_DELETE(rootGameNode);
	//SAFE_DELETE(shader);
}

void Game::SetEngine(CoreEngine* coreEngine)
{
	//if (rootGameNode == NULL)
	//{
	//	LOG(Critical, LOGPLACE, "Cannot set core engine. Root game node is NULL.");
	//	exit(EXIT_FAILURE);
	//}
	m_rootGameNode.SetEngine(coreEngine);
}

void Game::Init()
{
	//if (rootGameNode == NULL)
	//{
	//	LOG(Critical, LOGPLACE, "Root game node is not initialized");
	//	exit(EXIT_FAILURE);
	//}
	//rootGameNode->Init();
}

/* static */ void Game::WindowCloseEventCallback(GLFWwindow* window)
{
	GetGame()->CloseWindowEvent(window);
}

/* static */ void Game::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if ( !TwWindowSize(width, height) )
	{
		GetGame()->WindowResizeEvent(window, width, height);
	}
#else
	GetGame()->CloseWindowEvent(window);
#endif
}

/* static */ void Game::KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if( !TwEventKeyGLFW(key, action) )  // send event to AntTweakBar
    {
		// event has not been handled by AntTweakBar
		GetGame()->KeyEvent(window, key, scancode, action, mods);
    }
#else
	GetGame()->KeyEvent(window, key, scancode, action, mods);
#endif
}

/* static */ void Game::MouseEventCallback(GLFWwindow* window, int button, int action, int mods)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if ( !TwEventMouseButtonGLFW(button, action) )
	{
		// event has not been handled by AntTweakBar
		GetGame()->MouseButtonEvent(window, button, action, mods);
	}
#else
	GetGame()->MouseButtonEvent(window, button, action, mods);
#endif
}

/* static */ void Game::MousePosCallback(GLFWwindow* window, double xPos, double yPos)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventMousePosGLFW(static_cast<int>(xPos), static_cast<int>(yPos)))
	{
		// event has not been handled by AntTweakBar
		GetGame()->MousePosEvent(window, xPos, yPos);
	}
#else
	GetGame()->MousePosEvent(window, xPos, yPos);
#endif
}

/* static */ void Game::ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if ( !TwEventMouseWheelGLFW(static_cast<int>(yOffset)) ) // TODO: Check if yOffset here is ok
	{
		// event has not been handled by AntTweakBar
		GetGame()->ScrollEvent(window, xOffset, yOffset);
	}
#else
	GetGame()->ScrollEvent(window, xOffset, yOffset);
#endif
}

void Game::WindowResizeEvent(GLFWwindow* window, int width, int height)
{
	LOG(Notice, LOGPLACE, "Window resize event (width = %d, height = %d)", width, height);
}

void Game::CloseWindowEvent(GLFWwindow* window)
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
void Game::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	LOG(Delocust, LOGPLACE, "Key event with key = %d", key);

	//Input::UpdateKey(window, key, scancode, action, mods);
	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
}

void Game::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
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

void Game::MousePosEvent(GLFWwindow* window, double xPos, double yPos)
{

}

void Game::ScrollEvent(GLFWwindow* window, double xOffset, double yOffset)
{
	LOG(Debug, LOGPLACE, "Scroll event: xOffset=%.3f\t yOffset=%.3f", xOffset, yOffset);
}

//GameNode& Game::GetRootGameNode() const
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

//Shader* Game::GetShader() const
//{
//	ASSERT(shader != NULL);
//	if (shader == NULL)
//	{
//		LOG(Error, LOGPLACE, "Shader is NULL");
//	}
//	return shader;
//}

void Game::AddToSceneRoot(GameNode* child)
{
	m_rootGameNode.AddChild(child);
}

void Game::Render(Renderer* renderer)
{
	if (renderer == NULL)
	{
		LOG(Critical, LOGPLACE, "Rendering engine is NULL");
		exit(EXIT_FAILURE);
	}
	renderer->Render(m_rootGameNode);
}

#ifdef ANT_TWEAK_BAR_ENABLED
void Game::InitializeTweakBars()
{
}
#endif