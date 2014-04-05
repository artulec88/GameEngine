#include "TestGame.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\Camera.h"
#include "Rendering\MeshRenderer.h"
//#include "Rendering\DirectionalLight.h"
//#include "Rendering\PointLight.h"
//#include "Rendering\SpotLight.h"

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
	directionalLightNode(NULL),
	pointLightNode(NULL),
	spotLightNode(NULL),
	cameraCount(4),
	currentCameraIndex(0)
{
	stdlog(Debug, LOGPLACE, "TestGame is being constructed");
}


TestGame::~TestGame(void)
{
	//if (camera != NULL)
	//{
	//	delete camera;
	//	camera = NULL;
	//}
	//if (planeObject != NULL)
	//{
	//	delete planeObject;
	//	planeObject = NULL;
	//}
	//if (shader != NULL)
	//{
	//	delete shader;
	//	shader = NULL;
	//}
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
	GameNode* humanNode = new GameNode();
	humanNode->AddComponent(new MeshRenderer(humanMesh, new Material(new Texture("..\\Textures\\chessboard2.jpg", GL_TEXTURE_2D, GL_LINEAR), 2, 32)));
	humanNode->GetTransform().SetTranslation(3.0, 0.5, 3.0);
	AddToSceneRoot(humanNode);

	directionalLightNode = new GameNode();
	directionalLightNode->AddComponent(new DirectionalLight(Math::Vector3D(1.0, 1.0, 1.0), 0.8));
	directionalLightNode->GetTransform().SetRotation(Quaternion(Vector3D(1, 0, 0), Angle(-45)));

	srand((unsigned int)time(NULL));
	pointLightNode = new GameNode* [1];
	for (int i = 0; i < 1; ++i)
	{
		pointLightNode[i] = new GameNode();
		pointLightNode[i]->AddComponent(new PointLight(Math::Vector3D(0.0, 1.0, 0.0), 0.8, Attenuation(0.0, 0.0, 1.0)));
		pointLightNode[i]->GetTransform().SetTranslation(rand() % 5, 0.5, rand() % 5);
		//std::cout << i << ")" << pointLightNode[i]->GetTransform().GetPos().ToString() << std::endl;
		AddToSceneRoot(pointLightNode[i]);
	}

	spotLightNode = new GameNode* [1];
	for (int i = 0; i < 1; ++i)
	{
		spotLightNode[i] = new GameNode();
		spotLightNode[i]->AddComponent(new SpotLight(Math::Vector3D(1.0, 1.0f, 1.0f), 0.8f, Attenuation(0.0f, 0.5f, 0.0f), 0.7f));
		spotLightNode[i]->GetTransform().SetTranslation(rand() % 5 - 2, abs(rand() % 5 - 3), (rand() % 5) - 2);
		spotLightNode[i]->GetTransform().SetRotation(Quaternion(Vector3D(0, 1, 0), Angle(90)));
		AddToSceneRoot(spotLightNode[i]);
	}

	AddToSceneRoot(planeNode);
	AddToSceneRoot(directionalLightNode);

	GameNode* testMesh1 = new GameNode();
	testMesh1->GetTransform().SetTranslation(2.0, 2.0, 2.0);
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

	for (int i = 0; i < 1; ++i)
	{
		Transform& t = pointLightNode[i]->GetTransform();
		//std::cout << i << ")" << t.GetPos().ToString() << std::endl;
		t.SetTranslation(t.GetPos() + (Math::Vector3D(sin(temp) / 1000, 0.0, sin(temp) / 1000)));
		//std::cout << i << ")" << t.GetPos().ToString() << std::endl;
	}

	//spotLightNode->GetTransform().SetTranslation(1.0, 10.0 * abs(cos(static_cast<Math::Real>(rand() % 90) / 11)), 5.0 + 10.0 * abs(sin(temp)));
	for (int i = 0; i < 1; ++i)
	{
		Transform& t = spotLightNode[i]->GetTransform();
		t.SetTranslation(Math::Vector3D(1.0, 5.0 * abs(cos(temp)), 5.0 + 10.0 * abs(sin(temp))));
	}

	//stdlog(Delocust, LOGPLACE, "Transform = \n%s", transform->GetTransformation().ToString().c_str());
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
		transform.SetTranslation(transform.GetPos() + (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_S:
		transform.SetTranslation(transform.GetPos() - (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_A:
		transform.SetTranslation(transform.GetPos() - (transform.GetRot().GetRight() * sensitivity));
		break;
	case GLFW_KEY_D:
		transform.SetTranslation(transform.GetPos() + (transform.GetRot().GetRight() * sensitivity));
		break;
	case GLFW_KEY_SPACE: // move up
		transform.SetTranslation(transform.GetPos() + (transform.GetRot().GetUp() * sensitivity));
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