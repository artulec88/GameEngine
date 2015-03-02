#include "InGameState.h"
#include "Utility\ILogger.h"

using namespace GameNamespace;

InGameState::InGameState(void) :
	Rendering::GameState()
{
}

InGameState::~InGameState(void)
{
}

void InGameState::Entered()
{
}

void InGameState::Leaving()
{
}

void InGameState::Obscuring()
{
}

void InGameState::Revealed()
{
}

void InGameState::Input(Math::Real elapsedTime, Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "IN-GAME game state input processing");
	gameNode.InputAll(elapsedTime);
}

void InGameState::Render(Rendering::Renderer* renderer, const Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "IN-GAME game state rendering");
	LOG(Utility::Debug, LOGPLACE, "Rendering IN-GAME STATE");

	if (renderer == NULL)
	{
		LOG(Utility::Critical, LOGPLACE, "Rendering engine is NULL");
		exit(EXIT_FAILURE);
	}
	renderer->Render(gameNode);
}

void InGameState::Update(Math::Real elapsedTime, Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "IN-GAME game state updating");
	//stdlog(Delocust, LOGPLACE, "Game is being updated");
	
	//planeObject->GetTransform().SetPos(0.0, -1.0, 5.0);

	//rootGameNode->GetTransform().SetPos(0.0, -1.0, 5.0);
	gameNode.UpdateAll(elapsedTime);

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