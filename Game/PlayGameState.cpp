#include "PlayGameState.h"
#include "Engine\GameManager.h"
#include "Utility\ILogger.h"
#include "PlayMenuGameState.h"
#include "Engine\CoreEngine.h"
#include "Rendering\Shader.h"
#include "Engine\GameNode.h"

#include "Math\FloatingPoint.h"

#include "tinythread.h"

using namespace Game;

PlayGameState::PlayGameState(Engine::GameManager* gameManager) :
	GameState(),
	m_isMouseLocked(false),
	m_gameManager(gameManager)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("PlayGameState"))
#endif
{
}

PlayGameState::~PlayGameState(void)
{
}

void PlayGameState::Entered()
{
	START_PROFILING;
	INFO_LOG("PLAY game state has been placed in the game state manager");
	//tthread::thread t(GameManager::Load, GameManager::GetGameManager());
	CHECK_CONDITION(m_gameManager->IsGameLoaded(), Utility::Error, "PLAY game state has been placed in the game state manager before loading the game.");
#ifdef ANT_TWEAK_BAR_ENABLED
	m_gameManager->InitializeTweakBars();
	Engine::CoreEngine::GetCoreEngine()->GetRenderer()->InitializeTweakBars();
#endif

#ifdef CALCULATE_STATS
	CoreEngine::GetCoreEngine()->StartSamplingSpf();
#endif
	STOP_PROFILING;
}

void PlayGameState::Leaving()
{
	INFO_LOG("PLAY game state is about to be removed from the game state manager");
#ifdef CALCULATE_STATS
	Rendering::CoreEngine::GetCoreEngine()->StopSamplingSpf();
#endif
}

void PlayGameState::Obscuring()
{
	INFO_LOG("Another game state is about to stack on top of PLAY game state");
}

void PlayGameState::Revealed()
{
	INFO_LOG("PLAY game state has become the topmost game state in the game state manager's stack");
}

void PlayGameState::MouseButtonEvent(int button, int action, int mods)
{
	START_PROFILING;
	m_gameManager->GetRootGameNode().MouseButtonEvent(button, action, mods);
	//switch (action)
	//{
	//case GLFW_PRESS:
	//	m_isMouseLocked = ! m_isMouseLocked;
	//	if (m_isMouseLocked)
	//	{
	//		Rendering::CoreEngine::GetCoreEngine()->CentralizeCursor();
	//	}
	//	INFO_LOG("Mouse button pressed: button=%d\t mods=%d", button, mods);
	//	break;
	//case GLFW_RELEASE:
	//	INFO_LOG("Mouse button released: button=%d\t mods=%d", button, mods);
	//	break;
	//default:
	//	WARNING_LOG("Unknown action performed with the mouse. Button=%d\t action=%d\t mods=%d", button, action, mods);
	//}
	STOP_PROFILING;
}

void PlayGameState::MousePosEvent(double xPos, double yPos)
{
	START_PROFILING;
	DEBUG_LOG("Cursor position = (%.2f, %.2f)", xPos, yPos);
	m_gameManager->GetRootGameNode().MousePosEvent(xPos, yPos);
	//if (!m_isMouseLocked)
	//{
	//	STOP_PROFILING;
	//	return;
	//}

	//int width = Rendering::CoreEngine::GetCoreEngine()->GetWindowWidth();
	//int height = Rendering::CoreEngine::GetCoreEngine()->GetWindowHeight();
	//Math::Vector2D centerPosition(static_cast<Math::Real>(width) / 2, static_cast<Math::Real>(height) / 2);
	//Math::Vector2D deltaPosition(static_cast<Math::Real>(xPos), static_cast<Math::Real>(yPos));
	//deltaPosition -= centerPosition;
	//
	//bool rotX = ! Math::AlmostEqual(deltaPosition.GetX(), REAL_ZERO);
	//bool rotY = ! Math::AlmostEqual(deltaPosition.GetY(), REAL_ZERO);

	//if (rotX || rotY)
	//{
	//	Rendering::Transform& transform = Rendering::CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera().GetTransform();
	//	const Math::Real sensitivity = static_cast<Math::Real>(Rendering::CameraBase::GetSensitivity());
	//	if (rotX)
	//	{
	//		transform.Rotate(Math::Vector3D(0, 1, 0), Math::Angle(deltaPosition.GetX() * sensitivity));
	//	}
	//	if (rotY)
	//	{
	//		transform.Rotate(transform.GetRot().GetRight(), Math::Angle(deltaPosition.GetY() * sensitivity));
	//	}
	//	Rendering::CoreEngine::GetCoreEngine()->CentralizeCursor();
	//}
	STOP_PROFILING;
}

void PlayGameState::ScrollEvent(double xOffset, double yOffset)
{
	START_PROFILING;
	m_gameManager->GetRootGameNode().ScrollEvent(xOffset, yOffset);
	STOP_PROFILING;
}

void PlayGameState::KeyEvent(int key, int scancode, int action, int mods)
{
	START_PROFILING;
	m_gameManager->GetRootGameNode().KeyEvent(key, scancode, action, mods);
	STOP_PROFILING;
}

void PlayGameState::Render(Rendering::Shader* shader, Rendering::Renderer* renderer) const
{
	START_PROFILING;
	CHECK_CONDITION_EXIT(renderer != NULL, Utility::Critical, "Cannot render the game. The rendering engine is NULL.");
	DEBUG_LOG("PLAY game state rendering");

	Math::Real daytimeTransitionFactor;
	Utility::Timing::Daytime daytime = Engine::CoreEngine::GetCoreEngine()->GetCurrentDaytime(daytimeTransitionFactor);
	renderer->AdjustAmbientLightAccordingToCurrentTime(daytime, daytimeTransitionFactor);

	renderer->InitRenderScene();
	
	m_gameManager->GetRootGameNode().Render(renderer->GetAmbientShader(), renderer);
	//renderer->Render(Engine::GameManager::GetGameManager()->GetRootGameNode());


	STOP_PROFILING;
}

void PlayGameState::Update(Math::Real elapsedTime)
{
	START_PROFILING;
	DEBUG_LOG("PLAY game state updating");
	Engine::GameManager::GetGameManager()->GetRootGameNode().Update(elapsedTime);
	STOP_PROFILING;
}
