#include "TestGame.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\Camera.h"
#include "Rendering\MeshRenderer.h"
#include "Rendering\LookAtComponent.h"

#include "Math\FloatingPoint.h"
//#include "Math\Vector.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\Log.h"
#include "Utility\Config.h"

#include <sstream>

using namespace Utility;
using namespace Math;
using namespace Rendering;

TestGame::TestGame() :
	Game(),
	humanCount(22),
	humanNodes(NULL),
	directionalLightNode(NULL),
	pointLightCount(1),
	pointLightNodes(NULL),
	spotLightCount(1),
	spotLightNodes(NULL),
	cameraCount(4),
	currentCameraIndex(0)
{
	stdlog(Debug, LOGPLACE, "TestGame is being constructed");
}


TestGame::~TestGame(void)
{
	SAFE_DELETE_JUST_TABLE(humanNodes);
	SAFE_DELETE_JUST_TABLE(pointLightNodes);
	SAFE_DELETE_JUST_TABLE(spotLightNodes);
	SAFE_DELETE_JUST_TABLE(cameraNodes);
}

void TestGame::Init()
{
	stdlog(Notice, LOGPLACE, "Initalizing test game");

	Game::Init();

	GameNode* boxNode = new GameNode();
	boxNode->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\box2.obj"),
		new Material(
			new Texture("..\\Textures\\crateBox2.jpg", GL_TEXTURE_2D, GL_LINEAR), 2, 32)));
	boxNode->GetTransform().SetTranslation(12.0f, 3.5f, 9.0f);
	AddToSceneRoot(boxNode);

	// TODO: Do not use hard-coded values
	Math::Real specularIntensity = 1;
	Math::Real specularPower = 32;
	
	Math::Real fov = GET_CONFIG_VALUE("FieldOfView", "FieldOfViewDefault", 70.0f);
	Math::Real zNear = GET_CONFIG_VALUE("zNearClippingPlane", "zNearClippingPlaneDefault", 0.1f);
	Math::Real zFar = GET_CONFIG_VALUE("zFarClippingPlane", "zFarClippingPlaneDefault", 1000.0f);

	GameNode* planeNode = new GameNode();
	planeNode->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\plane3.obj"),
		new Material(
			new Texture("..\\Textures\\bricks2.jpg", GL_TEXTURE_2D, GL_LINEAR), specularIntensity, specularPower,
			new Texture("..\\Textures\\bricks2_normal.jpg", GL_TEXTURE_2D, GL_LINEAR))));
	planeNode->GetTransform().SetTranslation(0.0f, -1.0f, 5.0f);
	planeNode->GetTransform().SetScale(Math::Vector3D(4.0f, 4.0f, 4.0f));

	GameNode* monkeyNode1 = new GameNode();
	monkeyNode1->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\monkey3.obj"),
		new Material(
			new Texture("..\\Textures\\bricks.jpg", GL_TEXTURE_2D, GL_LINEAR), 1, 8)));
			//new Texture("..\\Textures\\bricks_normal.jpg"))));
	monkeyNode1->GetTransform().SetTranslation(15.0f, 3.75f, 4.0f);
	//monkeyNode1->GetTransform().SetRotation(Quaternion(Vector3D(0, 1, 0), Angle(-45)));
	//stdlog(Info, LOGPLACE, "MonkeyNode1 has ID=%d", monkeyNode1->GetID());
	//monkeyNode1->AddComponent(new LookAtComponent());
	AddToSceneRoot(monkeyNode1);

	GameNode* monkeyNode2 = new GameNode();
	monkeyNode2->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\monkey3.obj"),
		new Material(
			new Texture("..\\Textures\\bricks.jpg", GL_TEXTURE_2D, GL_LINEAR), 2, 32)));
			//new Texture("..\\Textures\\bricks_normal.jpg"))));
	monkeyNode2->GetTransform().SetTranslation(5.0, 3.0, 15.0);
	//monkeyNode2->AddComponent(new LookAtComponent());
	AddToSceneRoot(monkeyNode2);

	humanNodes = new GameNode* [humanCount];
	for (int i = 0; i < humanCount; ++i)
	{
		humanNodes[i] = new GameNode();
		humanNodes[i]->AddComponent(new MeshRenderer(new Mesh("..\\Models\\BodyMesh.obj"), new Material(new Texture("..\\Textures\\HumanSkin.jpg", GL_TEXTURE_2D, GL_LINEAR), 2, 32)));
		humanNodes[i]->GetTransform().SetTranslation(static_cast<Real>(rand() % 20), 0.7f, static_cast<Real>(rand() % 20));
		AddToSceneRoot(humanNodes[i]);
	}

	directionalLightNode = new GameNode();
	directionalLightNode->AddComponent(new DirectionalLight(Math::Vector3D(1.0f, 1.0f, 1.0f), 0.8f));
	directionalLightNode->GetTransform().SetRotation(Quaternion(Vector3D(1.0f, 1.0f, 0.0f), Angle(45.0f)));
	AddToSceneRoot(directionalLightNode);

	srand((unsigned int)time(NULL));
	pointLightNodes = new GameNode* [pointLightCount];
	for (int i = 0; i < pointLightCount; ++i)
	{
		pointLightNodes[i] = new GameNode();
		pointLightNodes[i]->AddComponent(new PointLight(Math::Vector3D(0.0f, 1.0f, 0.0f), 0.8f, Attenuation(0.0f, 0.0f, 1.0f)));
		pointLightNodes[i]->GetTransform().SetTranslation(static_cast<Real>(rand() % 5), 0.5f, static_cast<Real>(rand() % 5));
		//std::cout << i << ")" << pointLightNode[i]->GetTransform().GetPos().ToString() << std::endl;
		//AddToSceneRoot(pointLightNodes[i]);
	}

	spotLightNodes = new GameNode* [spotLightCount];
	for (int i = 0; i < spotLightCount; ++i)
	{
		spotLightNodes[i] = new GameNode();
		spotLightNodes[i]->AddComponent(new SpotLight(Math::Vector3D(0.0f, 0.0f, 1.0f), 15.8f, Attenuation(0.0f, 0.1f, 0.0f), 0.7f));
		spotLightNodes[i]->GetTransform().SetTranslation(static_cast<Real>(rand() % 5 - 2), static_cast<Real>(abs(rand() % 5 - 3)), static_cast<Real>((rand() % 5) - 2));
		spotLightNodes[i]->GetTransform().SetRotation(Quaternion(Vector3D(0.2f, 1.0f, 0.0f), Angle(90.0f)));
		//AddToSceneRoot(spotLightNodes[i]);
	}

	AddToSceneRoot(planeNode);

	GameNode* testMesh1 = new GameNode();
	testMesh1->GetTransform().SetTranslation(-2.0f, 2.5f, 2.0f);
	testMesh1->GetTransform().SetScale(Math::Vector3D(0.5f, 0.5f, 0.5f));
	GameNode* testMesh2 = new GameNode();
	testMesh2->GetTransform().SetTranslation(-12.0f, 0.5f, -3.0f);
	testMesh2->GetTransform().SetScale(Math::Vector3D(0.5f, 0.5f, 0.5f));
	testMesh1->AddComponent(new MeshRenderer(new Mesh("..\\Models\\plane.obj"), new Material(new Texture("..\\Textures\\chessboard.jpg", GL_TEXTURE_2D, GL_LINEAR), specularIntensity, specularPower)));
	testMesh2->AddComponent(new MeshRenderer(new Mesh("..\\Models\\plane2.obj"), new Material(new Texture("..\\Textures\\chessboard2.jpg", GL_TEXTURE_2D, GL_LINEAR), specularIntensity, specularPower)));
	testMesh1->AddChild(testMesh2);

	AddToSceneRoot(testMesh1);

	cameraNodes = new GameNode* [cameraCount];
	for (int i = 0; i < cameraCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		std::string cameraIndexStr = ss.str();

		cameraNodes[i] = new GameNode();
		Math::Real xPos = Config::Get("cameraPos_x_" + cameraIndexStr, Camera::defaultCameraPos.GetX());
		Math::Real yPos = Config::Get("cameraPos_y_" + cameraIndexStr, Camera::defaultCameraPos.GetY());
		Math::Real zPos = Config::Get("cameraPos_z_" + cameraIndexStr, Camera::defaultCameraPos.GetZ());
		cameraNodes[i]->GetTransform().SetTranslation(xPos, yPos, zPos);
		
		// TODO: Set forward and up vector for the camera
		//Quaternion rot();
		//cameraNodes[i]->GetTransform().SetRotation(rot);

		Angle fov(Config::Get("cameraFoV_" + cameraIndexStr, Camera::defaultFoV), true);
		Math::Real aspectRatio = Config::Get("cameraAspectRatio_" + cameraIndexStr, Camera::defaultAspectRatio);
		Math::Real zNearPlane = Config::Get("cameraNearPlane_" + cameraIndexStr, Camera::defaultNearPlane);
		Math::Real zFarPlane = Config::Get("cameraFarPlane_" + cameraIndexStr, Camera::defaultFarPlane);
		cameraNodes[i]->AddComponent(new Camera(fov, aspectRatio, zNearPlane, zFarPlane));
		//testMesh2->AddChild(cameraNodes[i]);
		AddToSceneRoot(cameraNodes[i]);
	}
}

//
//void Game::CleanUp()
//{
//	stdlog(Info, LOGPLACE, "The game is being cleaned up");
//}

void TestGame::Input(Math::Real delta)
{
	GetRootGameNode().InputAll(delta);
}

// TODO: Remove in the future
Math::Real temp = 0.0;
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;
bool up = false;
bool down = false;
Math::Vector3D velocity;
Math::Real maxSpeed = 0.02f;
bool isMouseLocked = false;

void TestGame::Update(Math::Real delta)
{
	//stdlog(Delocust, LOGPLACE, "Game is being updated");
	
	//planeObject->GetTransform().SetTranslation(0.0, -1.0, 5.0);

	//rootGameNode->GetTransform().SetTranslation(0.0, -1.0, 5.0);
	GetRootGameNode().UpdateAll(delta);

	temp += delta;
	if (temp > 20.0 * Math::M_PI)
	{
		temp = 0.0;
	}

	for (int i = 0; i < pointLightCount; ++i)
	{
		Transform& t = pointLightNodes[i]->GetTransform();
		//std::cout << i << ")" << t.GetPos().ToString() << std::endl;
		t.SetTranslation(t.GetPos() + (Math::Vector3D(sin(temp) / 1000, 0.0, sin(temp) / 1000)));
		//std::cout << i << ")" << t.GetPos().ToString() << std::endl;
	}

	for (int i = 0; i < spotLightCount; ++i)
	{
		Transform& t = spotLightNodes[i]->GetTransform();
		t.SetTranslation(Math::Vector3D(1.0f, 0.0f /*5.0f * abs(cos(temp))*/, 5.0f + 10.0f * abs(sin(temp))));
	}

	Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
	const Real sensitivity = static_cast<Real>(Camera::sensitivity);
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
	velocity += acceleration * delta * sensitivity * 0.01f;
	const Real step = 0.1f;
	const Real approachedValue = 0.0f; // must be ZERO!
	if (AlmostEqual(acceleration.GetX(), approachedValue))
	{
		velocity.ApproachX(step, approachedValue);
	}
	if (AlmostEqual(acceleration.GetY(), approachedValue))
	{
		velocity.ApproachY(step, approachedValue);
	}
	if (AlmostEqual(acceleration.GetZ(), approachedValue))
	{
		velocity.ApproachZ(step, approachedValue);
	}
	velocity.Threshold(maxSpeed);
	//velocity += acceleration * delta;
	//velocity -= slowDownVec;
	//stdlog(Debug, LOGPLACE, "Acceleration = %s\t Velocity = %s", acceleration.ToString().c_str(), velocity.ToString().c_str());

	transform.SetTranslation(transform.GetPos() + velocity);
}

/**
 * @param key the keyboard key that was pressed or released
 * @param scancode the system-specific scancode of the key
 * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
 * @param mods Bit field describing which modifier keys were held down
 */
void TestGame::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game::KeyEvent(window, key, scancode, action, mods);

	//ASSERT(camera != NULL);
	//if (camera == NULL)
	//{
	//	stdlog(Error, LOGPLACE, "Camera instance is not initialized");
	//	return;
	//}

	// TODO: Set delta to correct value
	Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
	const Real sensitivity = static_cast<Real>(Camera::sensitivity);
	switch (key)
	{
	case GLFW_KEY_W:
		forward = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//stdlog(Debug, LOGPLACE, "Forward = %d", forward);

		//transform.SetTranslation(transform.GetPos() + (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_S:
		backward = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//stdlog(Debug, LOGPLACE, "Backward = %d", backward);

		//direction -= transform.GetRot().GetForward().Normalized();
		//transform.SetTranslation(transform.GetPos() - (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_A:
		left = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//stdlog(Debug, LOGPLACE, "Left = %d", left);

		//direction -= transform.GetRot().GetRight().Normalized();
		//transform.SetTranslation(transform.GetPos() - (transform.GetRot().GetRight() * sensitivity));
		break;
	case GLFW_KEY_D:
		right = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//stdlog(Debug, LOGPLACE, "Right = %d", right);

		//direction += transform.GetRot().GetRight().Normalized();
		//transform.SetTranslation(transform.GetPos() + (transform.GetRot().GetRight() * sensitivity));
		break;
	case GLFW_KEY_SPACE: // move up
		up = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//direction += transform.GetRot().GetUp().Normalized();
		//transform.SetTranslation(transform.GetPos() + (transform.GetRot().GetUp() * sensitivity));
		break;
	case GLFW_KEY_LEFT_CONTROL: // move down
		down = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//direction -= transform.GetRot().GetUp().Normalized();
		//transform.SetTranslation(transform.GetPos() - (transform.GetRot().GetUp() * sensitivity));
		break;
	case GLFW_KEY_UP: // rotation around X axis
		transform.Rotate(transform.GetRot().GetRight(), Angle(-sensitivity));
		break;
	case GLFW_KEY_DOWN: // rotation around X axis
		transform.Rotate(transform.GetRot().GetRight(), Angle(sensitivity));
		break;
	case GLFW_KEY_LEFT: // rotation around Y axis
		transform.Rotate(/*transform.GetTransformedRot().GetUp()*/ Camera::yAxis, Angle(-sensitivity));
		break;
	case GLFW_KEY_RIGHT: // rotation around Y axis
		transform.Rotate(/*transform.GetTransformedRot().GetUp()*/ Camera::yAxis, Angle(sensitivity));
		break;
	case GLFW_KEY_N: // next camera
		if (action == GLFW_PRESS)
		{
			currentCameraIndex = CoreEngine::GetCoreEngine()->NextCamera();
		}
		break;
	case GLFW_KEY_P: // prev camera
		if (action == GLFW_PRESS)
		{
			currentCameraIndex = CoreEngine::GetCoreEngine()->PrevCamera();
		}
		break;
	}
}

void TestGame::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	stdlog(Debug, LOGPLACE, "Mouse event: button=%d\t action=%d\t mods=%d", button, action, mods);

	/**
	 * GLFW_MOUSE_BUTTON_1 = left mouse button
	 * GLFW_MOUSE_BUTTON_2 = right mouse button
	 * GLFW_MOUSE_BUTTON_3 = middle mouse button
	 */

	switch (action)
	{
	case GLFW_PRESS:
		isMouseLocked = ! isMouseLocked;
		stdlog(Info, LOGPLACE, "Mouse is locked");
		stdlog(Debug, LOGPLACE, "Mouse button pressed: button=%d\t mods=%d", button, mods);
		break;
	case GLFW_RELEASE:
		stdlog(Debug, LOGPLACE, "Mouse button released: button=%d\t mods=%d", button, mods);
		break;
	default:
		stdlog(Warning, LOGPLACE, "Unknown action performed with the mouse");
	}
}

void TestGame::MousePosEvent(GLFWwindow* window, double xPos, double yPos)
{
	if (! isMouseLocked)
	{
		return;
	}
	//stdlog(Debug, LOGPLACE, "Cursor position = %.2f, %.2f", xPos, yPos);

	int width = CoreEngine::GetCoreEngine()->GetWindowWidth();
	int height = CoreEngine::GetCoreEngine()->GetWindowHeight();
	Vector2D centerPosition(static_cast<Real>(width) / 2, static_cast<Real>(height) / 2);
	Vector2D deltaPosition(static_cast<Real>(xPos), static_cast<Real>(yPos));
	deltaPosition -= centerPosition;
	
	bool rotX = ! AlmostEqual(deltaPosition.GetX(), 0.0f);
	bool rotY = ! AlmostEqual(deltaPosition.GetY(), 0.0f);

	if (rotX)
	{
		const Real sensitivity = static_cast<Real>(Camera::sensitivity);
		Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
		transform.Rotate(Vector3D(0, 1, 0), Angle(deltaPosition.GetX() * sensitivity));
	}
	if (rotY)
	{
		const Real sensitivity = static_cast<Real>(Camera::sensitivity);
		Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
		transform.Rotate(transform.GetRot().GetRight(), Angle(deltaPosition.GetY() * sensitivity));
	}

	if (rotX || rotY)
	{
		CoreEngine::GetCoreEngine()->SetCursorPos(centerPosition.GetX(), centerPosition.GetY());
	}
}