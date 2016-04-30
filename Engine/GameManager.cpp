#include "stdafx.h"
#include "GameManager.h"
#include "GameStateManager.h"
#include "CoreEngine.h"

#include "Rendering\Renderer.h"
#include "Rendering\Attenuation.h"
#include "Rendering\PointLight.h"
#include "Rendering\Texture.h"

#include "Math\Math.h"
#include "Math\Vector.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"
#include "Utility\Time.h"

#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBar\include\AntTweakBar.h"
#endif

#include <GLFW\glfw3.h>
#include <fstream>

using namespace Utility;
using namespace Math;

Engine::GameManager* Engine::GameManager::s_gameManager = NULL;

/* static */ Engine::GameManager* Engine::GameManager::GetGameManager()
{
	return s_gameManager;
}

/* static */ void Engine::GameManager::Load(void* arg)
{
	Engine::GameManager* gameManager = static_cast<Engine::GameManager*>(arg);
	if (gameManager == NULL)
	{
		EMERGENCY_LOG("Cannot load the game. Passed parameter is not a game manager object.");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(CoreEngine::GetCoreEngine()->GetThreadWindow());
	Engine::CoreEngine::GetCoreEngine()->InitGlew(); // glew init
	gameManager->Load();
}

Engine::GameManager::GameManager() :
	m_rootGameNode(),
	m_terrainNode(NULL),
	m_skyboxNode(NULL),
	m_waterNode(NULL),
	m_gameStateManager(NULL),
	m_isGameLoaded(false),
	m_skyboxAngle(REAL_ZERO, Math::Unit::RADIAN),
	m_skyboxAngleStep(GET_CONFIG_VALUE("skyboxAngleStep", 0.02f), Math::Unit::RADIAN),
	m_gameCommandFactory()//,
	//m_effectFactory()
{
	INFO_LOG("Game manager construction started");
	//rootGameNode = new GameNode();
	//CHECK_CONDITION_EXIT(rootGameNode != NULL, Critical, "Root game node construction failed.");

	if (Engine::GameManager::s_gameManager != NULL)
	{
		ERROR_LOG("Constructor called when a singleton instance of CoreEngine class has already been created");
		SAFE_DELETE(Engine::GameManager::s_gameManager);
	}
	m_gameStateManager = new Engine::DefaultGameStateManager();

	Engine::GameManager::s_gameManager = this;
	DEBUG_LOG("Game manager construction finished");
}


Engine::GameManager::~GameManager(void)
{
	INFO_LOG("Game manager destruction finished");
	//SAFE_DELETE(m_rootGameNode);
	SAFE_DELETE(m_gameStateManager);
	SAFE_DELETE(m_terrainNode);
	SAFE_DELETE(m_skyboxNode);
	DEBUG_LOG("Game manager destruction finished");
}

void Engine::GameManager::WindowResizeEvent(int width, int height)
{
	NOTICE_LOG("Window resize event (width = %d, height = %d)", width, height);
	// TODO: Check if core engine finds out about the resizing of the window.
}

void Engine::GameManager::CloseWindowEvent()
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
void Engine::GameManager::KeyEvent(int key, int scancode, int action, int mods)
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

void Engine::GameManager::MouseButtonEvent(int button, int action, int mods)
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

void Engine::GameManager::MousePosEvent(double xPos, double yPos)
{
	DEBUG_LOG("Mouse position event x=%.2f, y=%.2f", xPos, yPos);
}

void Engine::GameManager::ScrollEvent(double xOffset, double yOffset)
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

void Engine::GameManager::AddToSceneRoot(GameNode* child)
{
	m_rootGameNode.AddChild(child);
}

void Engine::GameManager::AddWaterNode(GameNode* waterNode)
{
	m_waterNode = waterNode;
}

void Engine::GameManager::AddBillboardsRenderer(GameNode* billboardsRenderer)
{
	//CoreEngine::GetCoreEngine()->AddBillboardNode(billboardNode);
	// TODO: Check conditions
	m_billboardsRenderers.push_back(billboardsRenderer);
}

void Engine::GameManager::AddText(const Rendering::Text::GuiText& guiText)
{
	//FontMap::const_iterator textItr = m_texts.find(guiText.GetFont());
	//if (textItr == m_texts.end())
	//{
	//}
	//std::vector<Rendering::Text::GuiText>::const_iterator guiTextItr = std::find(m_texts[guiText.GetFont()].begin(), m_texts[guiText.GetFont()].end(), guiText);
	//m_texts[guiText.GetFont()].push_back(guiText); // TODO: What about duplicates?
	//m_texts.insert(std::pair<const Rendering::Text::Font*, std::vector<Rendering::Text::GuiText>>(guiText.GetFont(), std::vector<Rendering::Text::GuiText>()));
	//m_texts[guiText.GetFont()].push_back(guiText);
}

void Engine::GameManager::AddTerrainNode(GameNode* terrainNode)
{
	//CoreEngine::GetCoreEngine()->AddTerrainNode(terrainNode);
	m_terrainNode = terrainNode;
}

void Engine::GameManager::AddSkyboxNode(GameNode* skyboxNode)
{
	//CoreEngine::GetCoreEngine()->AddSkyboxNode(skyboxNode);
	m_skyboxNode = skyboxNode;
}

void Engine::GameManager::AddParticleGenerator(ParticleGenerator* particleGenerator)
{
	m_particleGenerators.push_back(particleGenerator);
}

void Engine::GameManager::Render(Rendering::Renderer* renderer) const
{
	m_gameStateManager->Render(NULL, renderer);
}

void Engine::GameManager::SetTransition(Engine::GameStateTransitioning::GameStateTransition* gameStateTransition)
{
	m_gameStateManager->SetTransition(gameStateTransition);
}

void Engine::GameManager::PerformStateTransition()
{
	m_gameStateManager->PerformStateTransition();
}

void Engine::GameManager::PopState()
{
	m_gameStateManager->Pop();
}

void Engine::GameManager::RequestGameQuit() const
{
	// TODO: Ask the user to save the game if the inGameState can be found in the game states stack
	//glfwSetWindowShouldClose(window, GL_TRUE); // Doesn't work, because we don't have a window

	CoreEngine::GetCoreEngine()->RequestWindowClose();
}

#ifdef ANT_TWEAK_BAR_ENABLED
void Engine::GameManager::InitializeTweakBars()
{
}
#endif