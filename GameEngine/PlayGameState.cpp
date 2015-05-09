#include "PlayGameState.h"
#include "Rendering\GameManager.h"
#include "Utility\ILogger.h"
#include "PlayMenuGameState.h"
#include "Rendering\CoreEngine.h"
#include "Rendering\GameNode.h"

#include "Math\FloatingPoint.h"

#include "tinythread.h"

using namespace Game;
using namespace Rendering;

PlayGameState::PlayGameState(void) :
	GameState(),
	m_isMouseLocked(false)
{
}

PlayGameState::~PlayGameState(void)
{
}

void PlayGameState::Entered()
{
	LOG(Utility::Info, LOGPLACE, "PLAY game state has been placed in the game state manager");
	//tthread::thread t(GameManager::Load, GameManager::GetGameManager());
	GameManager* gameManager = GameManager::GetGameManager();
	ASSERT(gameManager->IsGameLoaded());
	if (!gameManager->IsGameLoaded())
	{
		LOG(Utility::Warning, LOGPLACE, "PLAY game state has been placed in the game state manager before loading the game.");
		gameManager->Load();
	}
#ifdef ANT_TWEAK_BAR_ENABLED
	gameManager->InitializeTweakBars();
	CoreEngine::GetCoreEngine()->GetRenderer()->InitializeTweakBars();
#endif

#ifdef CALCULATE_STATS
	CoreEngine::GetCoreEngine()->StartSamplingSpf();
#endif
}

void PlayGameState::Leaving()
{
	LOG(Utility::Info, LOGPLACE, "PLAY game state is about to be removed from the game state manager");
#ifdef CALCULATE_STATS
	CoreEngine::GetCoreEngine()->StopSamplingSpf();
#endif
}

void PlayGameState::Obscuring()
{
	LOG(Utility::Info, LOGPLACE, "Another game state is about to stack on top of PLAY game state");
}

void PlayGameState::Revealed()
{
	LOG(Utility::Info, LOGPLACE, "PLAY game state has become the topmost game state in the game state manager's stack");
}

void PlayGameState::MouseButtonEvent(int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		m_isMouseLocked = ! m_isMouseLocked;
		if (m_isMouseLocked)
		{
			CoreEngine::GetCoreEngine()->CentralizeCursor();
		}
		LOG(Utility::Info, LOGPLACE, "Mouse button pressed: button=%d\t mods=%d", button, mods);
		break;
	case GLFW_RELEASE:
		LOG(Utility::Info, LOGPLACE, "Mouse button released: button=%d\t mods=%d", button, mods);
		break;
	default:
		LOG(Utility::Warning, LOGPLACE, "Unknown action performed with the mouse. Button=%d\t action=%d\t mods=%d", button, action, mods);
	}
}

void PlayGameState::MousePosEvent(double xPos, double yPos)
{
	// TODO: Mouse pos event should be handled by the game state manager
	if (!m_isMouseLocked)
	{
		return;
	}
	//stdlog(Debug, LOGPLACE, "Cursor position = %.2f, %.2f", xPos, yPos);

	int width = CoreEngine::GetCoreEngine()->GetWindowWidth();
	int height = CoreEngine::GetCoreEngine()->GetWindowHeight();
	Math::Vector2D centerPosition(static_cast<Math::Real>(width) / 2, static_cast<Math::Real>(height) / 2);
	Math::Vector2D deltaPosition(static_cast<Math::Real>(xPos), static_cast<Math::Real>(yPos));
	deltaPosition -= centerPosition;
	
	bool rotX = ! Math::AlmostEqual(deltaPosition.GetX(), REAL_ZERO);
	bool rotY = ! Math::AlmostEqual(deltaPosition.GetY(), REAL_ZERO);

	if (rotX || rotY)
	{
		Transform& transform = CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera().GetTransform();
		const Math::Real sensitivity = static_cast<Math::Real>(CameraBase::GetSensitivity());
		if (rotX)
		{
			transform.Rotate(Math::Vector3D(0, 1, 0), Math::Angle(deltaPosition.GetX() * sensitivity));
		}
		if (rotY)
		{
			transform.Rotate(transform.GetRot().GetRight(), Math::Angle(deltaPosition.GetY() * sensitivity));
		}
		CoreEngine::GetCoreEngine()->CentralizeCursor();
	}
}

bool forward = false;
bool backward = false;
bool left = false;
bool right = false;
bool up = false;
bool down = false;
void PlayGameState::KeyEvent(int key, int scancode, int action, int mods)
{
	//ASSERT(camera != NULL);
	//if (camera == NULL)
	//{
	//	LOG(Error, LOGPLACE, "Camera instance is not initialized");
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
			GameManager::GetGameManager()->SetTransition(new GameStateTransitioning::GameStateTransition(new PlayMenuGameState(), GameStateTransitioning::PUSH, GameStateModality::EXCLUSIVE));
		}
		break;
	case GLFW_KEY_C:
		//LOG(Utility::Debug, LOGPLACE, "transform.GetPos() = %s;\t transform.GetRot().GetForward() = %s", transform.GetPos().ToString().c_str(), transform.GetRot().GetForward().ToString().c_str());
		break;
	case GLFW_KEY_W:
		forward = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//LOG(Debug, LOGPLACE, "Forward = %d", forward);

		//transform.SetPos(transform.GetPos() + (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_S:
		backward = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//LOG(Debug, LOGPLACE, "Backward = %d", backward);

		//direction -= transform.GetRot().GetForward().Normalized();
		//transform.SetPos(transform.GetPos() - (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_A:
		left = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//LOG(Debug, LOGPLACE, "Left = %d", left);

		//direction -= transform.GetRot().GetRight().Normalized();
		//transform.SetPos(transform.GetPos() - (transform.GetRot().GetRight() * sensitivity));
		break;
	case GLFW_KEY_D:
		right = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//LOG(Debug, LOGPLACE, "Right = %d", right);

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
}

void PlayGameState::Input(Math::Real elapsedTime)
{
	//LOG(Utility::Debug, LOGPLACE, "PLAY game state input processing");
	GameManager::GetGameManager()->GetRootGameNode().InputAll(elapsedTime);
	//m_rootGameNode.InputAll(elapsedTime);
}

void PlayGameState::Render(Renderer* renderer)
{
	//LOG(Utility::Debug, LOGPLACE, "PLAY game state rendering");

	//if (renderer == NULL)
	//{
	//	LOG(Utility::Critical, LOGPLACE, "Rendering engine is NULL");
	//	exit(EXIT_FAILURE);
	//}
	renderer->Render(GameManager::GetGameManager()->GetRootGameNode());
}

void PlayGameState::Update(Math::Real elapsedTime)
{
	//LOG(Utility::Debug, LOGPLACE, "PLAY game state updating");

	//m_rootGameNode.UpdateAll(elapsedTime);
	GameManager::GetGameManager()->GetRootGameNode().UpdateAll(elapsedTime);

	//unsigned int currentCameraIndex = CoreEngine::GetCoreEngine()->GetCurrentCameraIndex();
	//Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
	//
	///* ==================== Adjusting camera vertical position begin ==================== */
	//timeToUpdateCameraHeight += delta;
	//if ((m_heightMapCalculationEnabled) && (timeToUpdateCameraHeight > CAMERA_HEIGHT_UPDATE_INTERVAL))
	//{
	//	Math::Real height = planeMesh->GetHeightAt(transform.GetPos().GetXZ());
	//	transform.GetPos().SetY(height);
	//	timeToUpdateCameraHeight = REAL_ZERO;
	//}
	///* ==================== Adjusting camera vertical position begin ==================== */

	//const Real sensitivity = static_cast<Real>(Camera::GetSensitivity());
	//Vector3D acceleration;
	//if (forward)
	//{
	//	acceleration += transform.GetRot().GetForward().Normalized();
	//}
	//if (backward)
	//{
	//	acceleration -= transform.GetRot().GetForward().Normalized();
	//}
	//if (left)
	//{
	//	acceleration -= transform.GetRot().GetRight().Normalized();
	//}
	//if (right)
	//{
	//	acceleration += transform.GetRot().GetRight().Normalized();
	//}
	//if (up)
	//{
	//	acceleration += transform.GetRot().GetUp().Normalized();
	//}
	//if (down)
	//{
	//	acceleration -= transform.GetRot().GetUp().Normalized();
	//}
	//velocity += acceleration * delta * sensitivity * 0.01f;
	//const Real step = 0.1f;
	//const Real approachedValue = 0.0f; // must be ZERO!
	//if (AlmostEqual(acceleration.GetX(), approachedValue))
	//{
	//	velocity.ApproachX(step, approachedValue);
	//}
	//if (AlmostEqual(acceleration.GetY(), approachedValue))
	//{
	//	velocity.ApproachY(step, approachedValue);
	//}
	//if (AlmostEqual(acceleration.GetZ(), approachedValue))
	//{
	//	velocity.ApproachZ(step, approachedValue);
	//}
	//velocity.Threshold(maxSpeed);
	////velocity += acceleration * delta;
	////velocity -= slowDownVec;
	////stdlog(Debug, LOGPLACE, "Acceleration = %s\t Velocity = %s", acceleration.ToString().c_str(), velocity.ToString().c_str());

	//transform.SetPos(transform.GetPos() + velocity);
}
