#include "stdafx.h"
#include "Game.h"
#include "CoreEngine.h"
#include "Vertex.h"
#include "Input.h"
//#include "PhongShader.h"
#include "Attenuation.h"
#include "PointLight.h"
#include "Texture.h"

#include "Math\Math.h"
#include "Math\Vector.h"

#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\Log.h"
#include "Utility\Time.h"

#include <fstream>

using namespace Utility;
using namespace Math;
using namespace Rendering;

Game* Game::game = NULL;

/* static */ Game* Game::GetGame()
{
	return game;
}

Game::Game()
{
	stdlog(Debug, LOGPLACE, "Game construction started");
	rootGameNode = new GameNode();
	ASSERT(rootGameNode != NULL);
	if (rootGameNode == NULL)
	{
		stdlog(Critical, LOGPLACE, "Root game node construction failed");
		exit(EXIT_FAILURE);
	}

	if (game != NULL)
	{
		stdlog(Error, LOGPLACE, "Constructor called when a singleton instance of MainApp class has already been created");
		delete game;
		game = NULL;
	}
	game = this;
	stdlog(Debug, LOGPLACE, "Game construction finished");
}


Game::~Game(void)
{
	if (rootGameNode != NULL)
	{
		delete rootGameNode;
		rootGameNode = NULL;
	}
	//if (shader != NULL)
	//{
	//	delete shader;
	//	shader = NULL;
	//}
}

// TODO: Implement this function or consider deleting it
void Game::InitializeCameras()
{
}

/* static */ void Game::WindowCloseEventCallback(GLFWwindow* window)
{
	GetGame()->CloseWindowEvent(window);
}

/* static */ void Game::KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GetGame()->KeyEvent(window, key, scancode, action, mods);
}

/* static */ void Game::MouseEventCallback(GLFWwindow* window, int button, int action, int mods)
{
	GetGame()->MouseEvent(window, button, action, mods);
}

/* static */ void Game::ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	GetGame()->ScrollEvent(window, xOffset, yOffset);
}

void Game::CloseWindowEvent(GLFWwindow* window)
{
	stdlog(Notice, LOGPLACE, "Close window event");
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
	stdlog(Delocust, LOGPLACE, "Key event with key = %d", key);

	//Input::UpdateKey(window, key, scancode, action, mods);
	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
}

void Game::MouseEvent(GLFWwindow* window, int button, int action, int mods)
{
	stdlog(Delocust, LOGPLACE, "Mouse event: button=%d\t action=%d\t mods=%d", button, action, mods);

	/**
	 * GLFW_MOUSE_BUTTON_1 = left mouse button
	 * GLFW_MOUSE_BUTTON_2 = right mouse button
	 * GLFW_MOUSE_BUTTON_3 = middle mouse button
	 */

	switch (action)
	{
	case GLFW_PRESS:
		stdlog(Debug, LOGPLACE, "Mouse button pressed: button=%d\t mods=%d", button, mods);
		break;
	case GLFW_RELEASE:
		stdlog(Debug, LOGPLACE, "Mouse button released: button=%d\t mods=%d", button, mods);
		break;
	default:
		stdlog(Warning, LOGPLACE, "Unknown action performed with the mouse");
	}
}

void Game::ScrollEvent(GLFWwindow* window, double xOffset, double yOffset)
{
	stdlog(Debug, LOGPLACE, "Scroll event: xOffset=%.3f\t yOffset=%.3f", xOffset, yOffset);
}

GameNode& Game::GetRootGameNode() const
{
	ASSERT(rootGameNode != NULL);
	if (rootGameNode == NULL)
	{
		stdlog(Emergency, LOGPLACE, "Root game node is NULL.");
		exit(EXIT_FAILURE);
	//	// TODO: throw another exception in the future
	//	throw FileNotFoundException();
	}

	return *rootGameNode;
}

//Shader* Game::GetShader() const
//{
//	ASSERT(shader != NULL);
//	if (shader == NULL)
//	{
//		stdlog(Error, LOGPLACE, "Shader is NULL");
//	}
//	return shader;
//}