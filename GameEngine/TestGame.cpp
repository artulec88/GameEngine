#include "TestGame.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\Camera.h"
#include "Rendering\MeshRenderer.h"
//#include "Rendering\DirectionalLight.h"
//#include "Rendering\PointLight.h"
//#include "Rendering\SpotLight.h"

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
	pointLightNode(NULL),
	spotLightCount(1),
	spotLightNode(NULL),
	cameraCount(4),
	currentCameraIndex(0)
{
	stdlog(Debug, LOGPLACE, "TestGame is being constructed");
}


TestGame::~TestGame(void)
{
	//for (int i = 0; i < humanCount; ++i)
	//{
	//	delete humanNodes[i];
	//	humanNodes[i] = NULL;
	//}
	//delete [] humanNodes;
	//humanNodes = NULL;

	//delete directionalLightNode;

	//for (int i = 0; i < pointLightCount; ++i)
	//{
	//	delete pointLightNode[i];
	//	pointLightNode[i] = NULL;
	//}
	//delete [] pointLightNode;
	//pointLightNode = NULL;

	//for (int i = 0; i < spotLightCount; ++i)
	//{
	//	delete spotLightNode[i];
	//	spotLightNode[i] = NULL;
	//}
	//delete [] spotLightNode;
	//spotLightNode = NULL;

	//for (int i = 0; i < cameraCount; ++i)
	//{
	//	delete cameraNodes[i];
	//	cameraNodes[i] = NULL;
	//}
	//delete [] cameraNodes;
	//cameraNodes = NULL;
}

void TestGame::Init()
{
	stdlog(Notice, LOGPLACE, "Initalizing test game");

	Game::Init();

	Math::Real fieldDepth = 10.0;
	Math::Real fieldWidth = 10.0;
	Vertex vertex1_1(Vector3D(-fieldWidth, 0.0, -fieldDepth), Vector2D(0.0, 0.0));
	Vertex vertex1_2(Vector3D(-fieldWidth, 0.0, 3 * fieldDepth), Vector2D(0.0, 1.0));
	Vertex vertex1_3(Vector3D(3 * fieldWidth, 0.0, -fieldDepth), Vector2D(1.0, 0.0));
	Vertex vertex1_4(Vector3D(3 * fieldWidth, 0.0, 3 * fieldDepth), Vector2D(1.0, 1.0));
	Vertex vertices1[] = {vertex1_1, vertex1_2, vertex1_3, vertex1_4};
	unsigned short indices1[] = {0, 1, 2,
							  2, 1, 3};
	Mesh* mesh1 = new Mesh(vertices1, 4, indices1, 6, true);

	Vertex vertex2_1(Vector3D(-fieldWidth / 10.0, 0.0, -fieldDepth / 10.0), Vector2D(0.0, 0.0));
	Vertex vertex2_2(Vector3D(-fieldWidth / 10.0, 0.0, 3 * fieldDepth / 10.0), Vector2D(0.0, 1.0));
	Vertex vertex2_3(Vector3D(3 * fieldWidth / 10.0, 0.0, -fieldDepth / 10.0), Vector2D(1.0, 0.0));
	Vertex vertex2_4(Vector3D(3 * fieldWidth / 10.0, 0.0, 3 * fieldDepth / 10.0), Vector2D(1.0, 1.0));
	Vertex vertices2[] = {vertex2_1, vertex2_2, vertex2_3, vertex2_4};
	unsigned short indices2[] = {0, 1, 2,
							  2, 1, 3};

	GameNode* boxNode = new GameNode();
	//Mesh* boxMesh = new Mesh("..\\Models\\box.obj");
	Mesh* boxMesh = new Mesh("..\\Models\\box.obj");
	boxNode->AddComponent(new MeshRenderer(boxMesh, new Material(new Texture("..\\Textures\\crateBox.jpg", GL_TEXTURE_2D, GL_LINEAR), 2, 32)));
	boxNode->GetTransform().SetTranslation(5.0, 0.5, 6.0);
	AddToSceneRoot(boxNode);

	// TODO: Do not use hard-coded values
	Math::Real specularIntensity = 1.0;
	Math::Real specularPower = 32.0;
	//material = new Material(NULL, materialColor, specularIntensity, specularPower);
	Material* material = new Material(new Texture("..\\Textures\\bricks3.jpg", GL_TEXTURE_2D, GL_LINEAR),
		specularIntensity, specularPower);
	
	Math::Real fov = GET_CONFIG_VALUE("FieldOfView", "FieldOfViewDefault", 70.0);
	Math::Real zNear = GET_CONFIG_VALUE("zNearClippingPlane", "zNearClippingPlaneDefault", 0.1);
	Math::Real zFar = GET_CONFIG_VALUE("zFarClippingPlane", "zFarClippingPlaneDefault", 0.1);

	GameNode* planeNode = new GameNode();
	planeNode->AddComponent(new MeshRenderer(mesh1, material));
	planeNode->GetTransform().SetTranslation(0.0, -1.0, 5.0);

	Mesh* monkeyMesh1 = new Mesh("..\\Models\\monkey.obj");
	GameNode* monkeyNode = new GameNode();
	monkeyNode->AddComponent(new MeshRenderer(monkeyMesh1, new Material(new Texture("..\\Textures\\crateBox.jpg", GL_TEXTURE_2D, GL_LINEAR), 1, 8)));
	monkeyNode->GetTransform().SetTranslation(5.0, 1.0, 15.0);
	AddToSceneRoot(monkeyNode);
	
	Mesh* monkeyMesh2 = new Mesh("..\\Models\\monkey.obj"); // this texture should already be loaded
	GameNode* monkeyNode2 = new GameNode();
	monkeyNode2->AddComponent(new MeshRenderer(monkeyMesh2, new Material(new Texture("..\\Textures\\chessboard2.jpg", GL_TEXTURE_2D, GL_LINEAR), 2, 32)));
	monkeyNode2->GetTransform().SetTranslation(5.0, 3.0, 15.0);
	AddToSceneRoot(monkeyNode2);

	Mesh* humanMesh = new Mesh("..\\Models\\BodyMesh.obj");
	humanNodes = new GameNode* [humanCount];
	for (int i = 0; i < humanCount; ++i)
	{
		humanNodes[i] = new GameNode();
		humanNodes[i]->AddComponent(new MeshRenderer(humanMesh, new Material(new Texture("..\\Textures\\HumanSkin.jpg", GL_TEXTURE_2D, GL_LINEAR), 2, 32)));
		humanNodes[i]->GetTransform().SetTranslation(rand() % 20, 0.65, rand() % 20);
		AddToSceneRoot(humanNodes[i]);
	}

	directionalLightNode = new GameNode();
	directionalLightNode->AddComponent(new DirectionalLight(Math::Vector3D(1.0, 1.0, 1.0), 0.8));
	directionalLightNode->GetTransform().SetRotation(Quaternion(Vector3D(1, 0, 0), Angle(-45)));

	srand((unsigned int)time(NULL));
	pointLightNode = new GameNode* [pointLightCount];
	for (int i = 0; i < pointLightCount; ++i)
	{
		pointLightNode[i] = new GameNode();
		pointLightNode[i]->AddComponent(new PointLight(Math::Vector3D(0.0, 1.0, 0.0), 0.8, Attenuation(0.0, 0.0, 1.0)));
		pointLightNode[i]->GetTransform().SetTranslation(rand() % 5, 0.5, rand() % 5);
		//std::cout << i << ")" << pointLightNode[i]->GetTransform().GetPos().ToString() << std::endl;
		AddToSceneRoot(pointLightNode[i]);
	}

	spotLightNode = new GameNode* [spotLightCount];
	for (int i = 0; i < spotLightCount; ++i)
	{
		spotLightNode[i] = new GameNode();
		spotLightNode[i]->AddComponent(new SpotLight(Math::Vector3D(0.0, 0.0f, 1.0f), 5.8f, Attenuation(0.0f, 0.1f, 0.0f), 0.7f));
		spotLightNode[i]->GetTransform().SetTranslation(rand() % 5 - 2, abs(rand() % 5 - 3), (rand() % 5) - 2);
		spotLightNode[i]->GetTransform().SetRotation(Quaternion(Vector3D(0.2, 1, 0), Angle(90)));
		AddToSceneRoot(spotLightNode[i]);
	}

	AddToSceneRoot(planeNode);
	AddToSceneRoot(directionalLightNode);

	GameNode* testMesh1 = new GameNode();
	testMesh1->GetTransform().SetTranslation(2.0, 2.5, 2.0);
	GameNode* testMesh2 = new GameNode();
	testMesh2->GetTransform().SetTranslation(2.0, 0.0, 5.0);
	testMesh1->AddComponent(new MeshRenderer(new Mesh(vertices2, 4, indices2, 6, true), new Material(new Texture("..\\Textures\\chessboard.jpg", GL_TEXTURE_2D, GL_LINEAR), specularIntensity, specularPower)));
	testMesh2->AddComponent(new MeshRenderer(new Mesh(vertices2, 4, indices2, 6, true), new Material(new Texture("..\\Textures\\chessboard2.jpg", GL_TEXTURE_2D, GL_LINEAR), specularIntensity, specularPower)));
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
		testMesh2->AddChild(cameraNodes[i]);
	}

	//const int camerasCount = GET_CONFIG_VALUE("CamerasCount", "CamerasCountDefault", 5);
	//std::string tempStr = "camera";
	//for (int i = 0; i < camerasCount; ++i)
	//{
	//	std::stringstream ss("");
	//	ss << (i + 1);
	//	std::string cameraIndexStr = ss.str();
	//	Real xPos = Config::Get("cameraPos_x_" + cameraIndexStr, Camera::defaultCamera.GetPos().GetX());
	//	Real yPos = Config::Get("cameraPos_y_" + cameraIndexStr, Camera::defaultCamera.GetPos().GetY());
	//	Real zPos = Config::Get("cameraPos_z_" + cameraIndexStr, Camera::defaultCamera.GetPos().GetZ());
	//	Real xForward = Config::Get("cameraForward_x_" + cameraIndexStr, Camera::defaultCamera.GetForward().GetX());
	//	Real yForward = Config::Get("cameraForward_y_" + cameraIndexStr, Camera::defaultCamera.GetForward().GetY());
	//	Real zForward = Config::Get("cameraForward_z_" + cameraIndexStr, Camera::defaultCamera.GetForward().GetZ());
	//	Real xUp = Config::Get("cameraUp_x_" + cameraIndexStr, Camera::defaultCamera.GetUp().GetX());
	//	Real yUp = Config::Get("cameraUp_y_" + cameraIndexStr, Camera::defaultCamera.GetUp().GetY());
	//	Real zUp = Config::Get("cameraUp_z_" + cameraIndexStr, Camera::defaultCamera.GetUp().GetZ());
	//	Vector3D cameraPos = Math::Vector3D(xPos, yPos, zPos);
	//	Vector3D cameraForward = Math::Vector3D(xForward, yForward, zForward);
	//	Vector3D cameraUp = Math::Vector3D(xUp, yUp, zUp);

	//	Real FoV = Config::Get("cameraFoV_" + ss.str(), Camera::defaultFoV);
	//	Real aspectRatio = Config::Get("cameraAspectRatio_" + ss.str(), Camera::defaultAspectRatio);
	//	Real zNearPlane = Config::Get("cameraNearPlane_" + ss.str(), Camera::defaultNearPlane);
	//	Real zFarPlane = Config::Get("cameraFarPlane_" + ss.str(), Camera::defaultFarPlane);
	//	cameras.push_back(new Camera(cameraPos, cameraForward, cameraUp, FoV, aspectRatio, zNearPlane, zFarPlane));
	//}
}

//
//void Game::CleanUp()
//{
//	stdlog(Info, LOGPLACE, "The game is being cleaned up");
//}

void TestGame::Input(Math::Real delta)
{
	GetRootGameNode().Input(delta);
}

// TODO: Remove in the future
Math::Real temp = 0.0;
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;
Math::Vector3D velocity;
Math::Real maxSpeed = 0.02;
bool isMouseLocked = false;

void TestGame::Update(Math::Real delta)
{
	//stdlog(Delocust, LOGPLACE, "Game is being updated");
	
	//planeObject->GetTransform().SetTranslation(0.0, -1.0, 5.0);

	//rootGameNode->GetTransform().SetTranslation(0.0, -1.0, 5.0);
	GetRootGameNode().Update(delta);

	temp += delta;
	if (temp > 20.0 * Math::M_PI)
	{
		temp = 0.0;
	}

	for (int i = 0; i < pointLightCount; ++i)
	{
		Transform& t = pointLightNode[i]->GetTransform();
		//std::cout << i << ")" << t.GetPos().ToString() << std::endl;
		t.SetTranslation(t.GetPos() + (Math::Vector3D(sin(temp) / 1000, 0.0, sin(temp) / 1000)));
		//std::cout << i << ")" << t.GetPos().ToString() << std::endl;
	}

	//spotLightNode->GetTransform().SetTranslation(1.0, 10.0 * abs(cos(static_cast<Math::Real>(rand() % 90) / 11)), 5.0 + 10.0 * abs(sin(temp)));
	for (int i = 0; i < spotLightCount; ++i)
	{
		Transform& t = spotLightNode[i]->GetTransform();
		t.SetTranslation(Math::Vector3D(1.0, 0.0 /*5.0 * abs(cos(temp))*/, 5.0 + 10.0 * abs(sin(temp))));
	}

	//stdlog(Delocust, LOGPLACE, "Transform = \n%s", transform->GetTransformation().ToString().c_str());

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
	velocity += acceleration * delta * sensitivity * 0.01;
	if (AlmostEqual(acceleration.GetX(), static_cast<Real>(0.0)))
	{
		velocity.ApproachX(0.1, 0.0);
	}
	if (AlmostEqual(acceleration.GetY(), static_cast<Real>(0.0)))
	{
		velocity.ApproachY(0.1, 0.0);
	}
	if (AlmostEqual(acceleration.GetZ(), static_cast<Real>(0.0)))
	{
		velocity.ApproachZ(0.1, 0.0);
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
		//direction += transform.GetRot().GetUp().Normalized();
		//transform.SetTranslation(transform.GetPos() + (transform.GetRot().GetUp() * sensitivity));
		break;
	case GLFW_KEY_LEFT_CONTROL: // move down
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
	
	bool rotX = ! AlmostEqual(deltaPosition.GetX(), static_cast<Real>(0.0));
	bool rotY = ! AlmostEqual(deltaPosition.GetY(), static_cast<Real>(0.0));

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