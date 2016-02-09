#include "PlayGameState.h"
#include "Rendering\GameManager.h"
#include "Utility\ILogger.h"
#include "PlayMenuGameState.h"
#include "Rendering\CoreEngine.h"
#include "Rendering\Shader.h"
#include "Rendering\GameNode.h"

#include "Math\FloatingPoint.h"

#include "tinythread.h"

using namespace Game;

PlayGameState::PlayGameState(void) :
	GameState(),
	m_isMouseLocked(false)
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
	Rendering::GameManager* gameManager = Rendering::GameManager::GetGameManager();
	CHECK_CONDITION(gameManager->IsGameLoaded(), Utility::Error, "PLAY game state has been placed in the game state manager before loading the game.");
#ifdef ANT_TWEAK_BAR_ENABLED
	gameManager->InitializeTweakBars();
	Rendering::CoreEngine::GetCoreEngine()->GetRenderer()->InitializeTweakBars();
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
	switch (action)
	{
	case GLFW_PRESS:
		m_isMouseLocked = ! m_isMouseLocked;
		if (m_isMouseLocked)
		{
			Rendering::CoreEngine::GetCoreEngine()->CentralizeCursor();
		}
		INFO_LOG("Mouse button pressed: button=%d\t mods=%d", button, mods);
		break;
	case GLFW_RELEASE:
		INFO_LOG("Mouse button released: button=%d\t mods=%d", button, mods);
		break;
	default:
		WARNING_LOG("Unknown action performed with the mouse. Button=%d\t action=%d\t mods=%d", button, action, mods);
	}
	STOP_PROFILING;
}

void PlayGameState::MousePosEvent(double xPos, double yPos)
{
	START_PROFILING;
	DEBUG_LOG("Cursor position = (%.2f, %.2f)", xPos, yPos);
	if (!m_isMouseLocked)
	{
		STOP_PROFILING;
		return;
	}

	int width = Rendering::CoreEngine::GetCoreEngine()->GetWindowWidth();
	int height = Rendering::CoreEngine::GetCoreEngine()->GetWindowHeight();
	Math::Vector2D centerPosition(static_cast<Math::Real>(width) / 2, static_cast<Math::Real>(height) / 2);
	Math::Vector2D deltaPosition(static_cast<Math::Real>(xPos), static_cast<Math::Real>(yPos));
	deltaPosition -= centerPosition;
	
	bool rotX = ! Math::AlmostEqual(deltaPosition.GetX(), REAL_ZERO);
	bool rotY = ! Math::AlmostEqual(deltaPosition.GetY(), REAL_ZERO);

	if (rotX || rotY)
	{
		Rendering::Transform& transform = Rendering::CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera().GetTransform();
		const Math::Real sensitivity = static_cast<Math::Real>(Rendering::CameraBase::GetSensitivity());
		if (rotX)
		{
			transform.Rotate(Math::Vector3D(0, 1, 0), Math::Angle(deltaPosition.GetX() * sensitivity));
		}
		if (rotY)
		{
			transform.Rotate(transform.GetRot().GetRight(), Math::Angle(deltaPosition.GetY() * sensitivity));
		}
		Rendering::CoreEngine::GetCoreEngine()->CentralizeCursor();
	}
	STOP_PROFILING;
}

bool forward = false;
bool backward = false;
bool left = false;
bool right = false;
bool up = false;
bool down = false;
void PlayGameState::KeyEvent(int key, int scancode, int action, int mods)
{
	START_PROFILING;
	//CHECK_CONDITION_RETURN(camera != NULL, Utility::Error, "Camera instance is not initialized");
	//if (camera == NULL)
	//{
	//	return;
	//}

	// TODO: Set delta to correct value
	//unsigned int currentCameraIndex = CoreEngine::GetCoreEngine()->GetCurrentCameraIndex();
	//Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
	//const Math::Real sensitivity = static_cast<Math::Real>(CameraBase::GetSensitivity());
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
		{
			Rendering::GameManager::GetGameManager()->SetTransition(new Rendering::GameStateTransitioning::GameStateTransition(new PlayMenuGameState(), Rendering::GameStateTransitioning::PUSH, Rendering::GameStateModality::EXCLUSIVE));
		}
		break;
	case GLFW_KEY_C:
		//DEBUG_LOG("transform.GetPos() = %s;\t transform.GetRot().GetForward() = %s", transform.GetPos().ToString().c_str(), transform.GetRot().GetForward().ToString().c_str());
		break;
	case GLFW_KEY_W:
		forward = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//DEBUG_LOG("Forward = %d", forward);

		//transform.SetPos(transform.GetPos() + (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_S:
		backward = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//DEBUG_LOG("Backward = %d", backward);

		//direction -= transform.GetRot().GetForward().Normalized();
		//transform.SetPos(transform.GetPos() - (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_A:
		left = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//DEBUG_LOG("Left = %d", left);

		//direction -= transform.GetRot().GetRight().Normalized();
		//transform.SetPos(transform.GetPos() - (transform.GetRot().GetRight() * sensitivity));
		break;
	case GLFW_KEY_D:
		right = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//DEBUG_LOG("Right = %d", right);

		//direction += transform.GetRot().GetRight().Normalized();
		//transform.SetPos(transform.GetPos() + (transform.GetRot().GetRight() * sensitivity));
		break;
	case GLFW_KEY_SPACE: // move up
		up = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//direction += transform.GetRot().GetUp().Normalized();
		//transform.SetPos(transform.GetPos() + (transform.GetRot().GetUp() * sensitivity));
		break;
	case GLFW_KEY_LEFT_CONTROL: // move down
		down = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//direction -= transform.GetRot().GetUp().Normalized();
		//transform.SetPos(transform.GetPos() - (transform.GetRot().GetUp() * sensitivity));
		break;
	case GLFW_KEY_UP: // rotation around X axis
		//transform.Rotate(transform.GetRot().GetRight(), Angle(-sensitivity));
		break;
	case GLFW_KEY_DOWN: // rotation around X axis
		//transform.Rotate(transform.GetRot().GetRight(), Angle(sensitivity));
		break;
	case GLFW_KEY_LEFT: // rotation around Y axis
		//transform.Rotate(transform.GetTransformedRot().GetUp() /*CameraBase::yAxis*/, Angle(-sensitivity));
		break;
	case GLFW_KEY_RIGHT: // rotation around Y axis
		//transform.Rotate(transform.GetTransformedRot().GetUp() /*CameraBase::yAxis*/, Angle(sensitivity));
		break;
	case GLFW_KEY_N: // next camera
		//if (action == GLFW_PRESS)
		//{
		//	CoreEngine::GetCoreEngine()->NextCamera();
		//}
		break;
	case GLFW_KEY_P: // prev camera
		//if (action == GLFW_PRESS)
		//{
		//	CoreEngine::GetCoreEngine()->PrevCamera();
		//}
		break;
	}
	STOP_PROFILING;
}

void PlayGameState::Input(Math::Real elapsedTime)
{
	START_PROFILING;
	DEBUG_LOG("PLAY game state input processing");
	Rendering::GameManager::GetGameManager()->GetRootGameNode().InputAll(elapsedTime);
	//m_rootGameNode.InputAll(elapsedTime);
	STOP_PROFILING;
}

void PlayGameState::Render(Rendering::Shader* shader, Rendering::Renderer* renderer)
{
	START_PROFILING;
	CHECK_CONDITION_EXIT(renderer != NULL, Utility::Critical, "Cannot render the game. The rendering engine is NULL.");
	DEBUG_LOG("PLAY game state rendering");

	renderer->Render(Rendering::GameManager::GetGameManager()->GetRootGameNode());
	STOP_PROFILING;
}

Math::Vector3D velocity;
Math::Real maxSpeed = 1.0f;
void PlayGameState::Update(Math::Real elapsedTime)
{
	START_PROFILING;
	DEBUG_LOG("PLAY game state updating");

	//m_rootGameNode.UpdateAll(elapsedTime);
	Rendering::GameManager::GetGameManager()->GetRootGameNode().UpdateAll(elapsedTime);

	Rendering::Transform& transform = Rendering::CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera().GetTransform();
	//unsigned int currentCameraIndex = CoreEngine::GetCoreEngine()->GetCurrentCameraIndex();
	//Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
	//
	///* ==================== Adjusting camera vertical position begin ==================== */
	//timeToUpdateCameraHeight += elapsedTime;
	//if ((m_heightMapCalculationEnabled) && (timeToUpdateCameraHeight > CAMERA_HEIGHT_UPDATE_INTERVAL))
	//{
	//	Math::Real height = planeMesh->GetHeightAt(transform.GetPos().GetXZ());
	//	transform.GetPos().SetY(height);
	//	timeToUpdateCameraHeight = REAL_ZERO;
	//}
	///* ==================== Adjusting camera vertical position begin ==================== */

	const Math::Real sensitivity = static_cast< Math::Real>(Rendering::Camera::GetSensitivity());
	Math::Vector3D acceleration;
	if (forward)
	{
		acceleration += transform.GetRot().GetForward().Normalized();
	}
	if (backward)
	{
		acceleration -= transform.GetRot().GetForward().Normalized();
	}
	if (left)
	{
		acceleration -= transform.GetRot().GetRight().Normalized();
	}
	if (right)
	{
		acceleration += transform.GetRot().GetRight().Normalized();
	}
	if (up)
	{
		acceleration += transform.GetRot().GetUp().Normalized();
	}
	if (down)
	{
		acceleration -= transform.GetRot().GetUp().Normalized();
	}
	velocity += acceleration * elapsedTime * sensitivity * 0.01f;
	const Math::Real step = 0.1f;
	const Math::Real approachedValue = 0.0f; // must be ZERO!
	if (Math::AlmostEqual(acceleration.GetX(), approachedValue))
	{
		velocity.ApproachX(step, approachedValue);
	}
	if (Math::AlmostEqual(acceleration.GetY(), approachedValue))
	{
		velocity.ApproachY(step, approachedValue);
	}
	if (Math::AlmostEqual(acceleration.GetZ(), approachedValue))
	{
		velocity.ApproachZ(step, approachedValue);
	}
	velocity.Threshold(maxSpeed);
	//velocity += acceleration * delta;
	//velocity -= slowDownVec;
	DEBUG_LOG("Acceleration = %s\tVelocity = %s", acceleration.ToString().c_str(), velocity.ToString().c_str());
	transform.SetPos(transform.GetPos() + velocity);
	STOP_PROFILING;
}
