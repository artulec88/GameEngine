#include "PlayGameState.h"
#include "Rendering\GameManager.h"
#include "Utility\ILogger.h"
#include "PlayMenuGameState.h"
#include "Rendering\CoreEngine.h"
#include "Rendering\GameNode.h"

#include "tinythread.h"

using namespace Game;
using namespace Rendering;

PlayGameState::PlayGameState(void) :
	GameState()//,
	//m_rootGameNode(GameManager::GetGameManager()->GetRootGameNode())
{
}

PlayGameState::~PlayGameState(void)
{
}

void fun(void* arg)
{
	//GameManager* gameManager = GameManager::GetGameManager();
	//if (!gameManager->IsGameLoaded())
	//{
	//	gameManager->Load();
	//	gameManager->InitializeTweakBars();
	//}
	tthread::this_thread::sleep_for(tthread::chrono::milliseconds(15000));
	LOG(Utility::Critical, LOGPLACE, "This is fun");
}

void PlayGameState::Entered()
{
	LOG(Utility::Info, LOGPLACE, "PLAY game state has been placed in the game state manager");
	//tthread::thread t(GameManager::Load, GameManager::GetGameManager());
	GameManager* gameManager = GameManager::GetGameManager();
	if (!gameManager->IsGameLoaded())
	{
		gameManager->Load();
#ifdef ANT_TWEAK_BAR_ENABLED
		gameManager->InitializeTweakBars();
#endif
	}

#ifdef CALCULATE_STATS
	CoreEngine::GetCoreEngine()->StartSamplingSpf();
#endif
	//t.join();
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
	
	//renderer->Render(m_rootGameNode);

	renderer->Render(GameManager::GetGameManager()->GetRootGameNode());
}

void PlayGameState::Update(Math::Real elapsedTime)
{
	//LOG(Utility::Debug, LOGPLACE, "PLAY game state updating");

	//m_rootGameNode.UpdateAll(elapsedTime);
	GameManager::GetGameManager()->GetRootGameNode().UpdateAll(elapsedTime);


	//rootGameNode->GetTransform().SetPos(0.0, -1.0, 5.0);

	//temp += delta;
	//if (temp > 20.0 * M_PI)
	//{
	//	temp = 0.0;
	//}

	//for (int i = 0; i < pointLightCount; ++i)
	//{
	//	if (pointLightNodes[i] == NULL)
	//	{
	//		LOG(Error, LOGPLACE, "Point light node #%d is NULL while the number of created point lights should be equal to %d.", i, pointLightCount);
	//		continue;
	//	}
	//	Transform& t = pointLightNodes[i]->GetTransform();
	//	t.SetPos(t.GetPos() + (Vector3D(sin(temp) / 1000, cos(temp) / 2000, cos(temp) / 1000)));
	//}

	////for (int i = 0; i < spotLightCount; ++i)
	////{
	////	if (spotLightNodes[i] == NULL)
	////	{
	////		LOG(Error, LOGPLACE, "Spot light node #%d is NULL while the number of created spot lights should be equal to %d.", i, spotLightCount);
	////		continue;
	////	}
	////	Transform& t = spotLightNodes[i]->GetTransform();
	////	t.SetPos(t.GetPos() + (Vector3D(sin(temp) / 1000, sin(temp) / 2000, cos(temp) / 1000)));
	////}

	//unsigned int currentCameraIndex = CoreEngine::GetCoreEngine()->GetCurrentCameraIndex();
	//Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
	//
	///* ==================== Adjusting camera vertical position begin ==================== */
	////const int NUMBER_OF_TEST_ITERATIONS = 1000;
	////const Math::Real lowX = -30.0f;
	////const Math::Real highX = 30.0f;
	////const Math::Real lowZ = -30.0f;
	////const Math::Real highZ = 30.0f;
	////clock_t begin = clock();
	////for (int i = 0; i < NUMBER_OF_TEST_ITERATIONS; ++i)
	////{
	////	Math::Real x = lowX + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (highX - lowX));
	////	Math::Real z = lowZ + static_cast<Math::Real>(rand()) /  static_cast<Math::Real>(RAND_MAX / (highZ - lowZ));
	////	Math::Real height = planeMesh->GetHeightAt(Math::Vector2D(x, z));
	////}
	////clock_t end = clock();
	////LOG(Info, LOGPLACE, "Camera's height calculation took %.2f [us]", (1000000.0 * static_cast<double>(end - begin) / (CLOCKS_PER_SEC)) / NUMBER_OF_TEST_ITERATIONS);

	//timeToUpdateCameraHeight += delta;
	//if ((heightMapCalculationEnabled) && (timeToUpdateCameraHeight > CAMERA_HEIGHT_UPDATE_INTERVAL))
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
