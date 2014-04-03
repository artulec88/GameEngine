#include "TestGame.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\Camera.h"
//#include "Rendering\BasicShader.h"
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

	if (rootGameNode == NULL)
	{
		stdlog(Critical, LOGPLACE, "Root game node is not initialized");
		exit(EXIT_FAILURE);
	}
	//rootGameNode->Init();

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
	//mesh = new Mesh("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Models\\box.obj");

	Vertex vertex2_1(Vector3D(-fieldWidth / 10.0, 0.0, -fieldDepth / 10.0), Vector2D(0.0, 0.0));
	Vertex vertex2_2(Vector3D(-fieldWidth / 10.0, 0.0, 3 * fieldDepth / 10.0), Vector2D(0.0, 1.0));
	Vertex vertex2_3(Vector3D(3 * fieldWidth / 10.0, 0.0, -fieldDepth / 10.0), Vector2D(1.0, 0.0));
	Vertex vertex2_4(Vector3D(3 * fieldWidth / 10.0, 0.0, 3 * fieldDepth / 10.0), Vector2D(1.0, 1.0));
	Vertex vertices2[] = {vertex2_1, vertex2_2, vertex2_3, vertex2_4};
	unsigned short indices2[] = {0, 1, 2,
							  2, 1, 3};
	Mesh* mesh2 = new Mesh(vertices2, 4, indices2, 6, true);

	Math::Vector3D materialColor = Math::Vector3D(1.0, 1.0, 1.0);
	Math::Real specularIntensity = 1.0;
	Math::Real specularPower = 8.0;
	//material = new Material(NULL, materialColor, specularIntensity, specularPower);
	Material* material = new Material(new Texture("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Textures\\chessboard2.jpg", GL_TEXTURE_2D, GL_LINEAR),
		materialColor, specularIntensity, specularPower);

	MeshRenderer* meshRenderer = new MeshRenderer(mesh1, material);
	
	Math::Real fov = GET_CONFIG_VALUE("FieldOfView", "FieldOfViewDefault", 70.0);
	Math::Real zNear = GET_CONFIG_VALUE("zNearClippingPlane", "zNearClippingPlaneDefault", 0.1);
	Math::Real zFar = GET_CONFIG_VALUE("zFarClippingPlane", "zFarClippingPlaneDefault", 0.1);

	GameNode* planeNode = new GameNode();
	planeNode->AddComponent(meshRenderer);
	planeNode->GetTransform().SetTranslation(0.0, -1.0, 5.0);

	directionalLightNode = new GameNode();
	directionalLightNode->AddComponent(new DirectionalLight(Math::Vector3D(1.0, 1.0, 1.0), 0.8, Math::Vector3D(1.0, 1.0, 1.0)));

	srand((unsigned int)time(NULL));
	pointLightNode = new GameNode* [33];
	for (int i = 0; i < 33; ++i)
	{
		pointLightNode[i] = new GameNode();
		pointLightNode[i]->AddComponent(new PointLight(Math::Vector3D(0.0, 1.0, 0.0), 0.8, Attenuation(0.0, 0.0, 1.0)));
		pointLightNode[i]->GetTransform().SetTranslation(rand() % 5, 0.5, rand() % 5);
		std::cout << i << ")" << pointLightNode[i]->GetTransform().GetPos().ToString() << std::endl;
		rootGameNode->AddChild(pointLightNode[i]);
	}

	spotLightNode = new GameNode* [3];
	for (int i = 0; i < 3; ++i)
	{
		spotLightNode[i] = new GameNode();
		spotLightNode[i]->AddComponent(new SpotLight(Math::Vector3D(1.0, 1.0f, 1.0f), 0.8f, Attenuation(0.0f, 0.1f, 0.0f), 0.7f));
		spotLightNode[i]->GetTransform().SetTranslation(rand() % 5 - 2, abs(rand() % 5 - 3), (rand() % 5) - 2);
		rootGameNode->AddChild(spotLightNode[i]);
	}

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
		rootGameNode->AddChild(cameraNodes[i]);
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

	rootGameNode->AddChild(planeNode);
	//rootGameNode->AddChild(directionalLightNode);

	GameNode* testMesh1 = new GameNode();
	testMesh1->GetTransform().SetTranslation(2.0, 2.0, 2.0);
	GameNode* testMesh2 = new GameNode();
	testMesh2->GetTransform().SetTranslation(2.0, 0.0, 5.0);
	testMesh1->AddComponent(new MeshRenderer(mesh2, material));
	testMesh2->AddComponent(new MeshRenderer(mesh2, material));
	testMesh1->AddChild(testMesh2);

	rootGameNode->AddChild(testMesh1);
}

//
//void Game::CleanUp()
//{
//	stdlog(Info, LOGPLACE, "The game is being cleaned up");
//}

void TestGame::Input(Math::Real delta)
{
	rootGameNode->Input(delta);
}

// TODO: Remove in the future
Math::Real temp = 0.0;

void TestGame::Update(Math::Real delta)
{
	//stdlog(Delocust, LOGPLACE, "Game is being updated");
	
	//planeObject->GetTransform().SetTranslation(0.0, -1.0, 5.0);

	//rootGameNode->GetTransform().SetTranslation(0.0, -1.0, 5.0);
	rootGameNode->Update(delta);

	temp += delta;
	if (temp > 20.0 * Math::M_PI)
	{
		temp = 0.0;
	}

	for (int i = 0; i < 33; ++i)
	{
		Transform& t = pointLightNode[i]->GetTransform();
		//std::cout << i << ")" << t.GetPos().ToString() << std::endl;
		t.SetTranslation(t.GetPos() + (Math::Vector3D(sin(temp) / 1000, 0.0, sin(temp) / 1000)));
		//std::cout << i << ")" << t.GetPos().ToString() << std::endl;
	}

	//spotLightNode->GetTransform().SetTranslation(1.0, 10.0 * abs(cos(static_cast<Math::Real>(rand() % 90) / 11)), 5.0 + 10.0 * abs(sin(temp)));
	for (int i = 0; i < 3; ++i)
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
	case GLFW_KEY_UP: // rotation around X axis
		transform.SetRotation(transform.GetRot() * (Quaternion(transform.GetRot().GetRight(), Angle(sensitivity))).Normalized());
		//RotateX(Angle(-rotationAmount));
		break;
	case GLFW_KEY_SPACE: // move up
		transform.SetTranslation(transform.GetPos() + (transform.GetRot().GetUp() * sensitivity));
		break;
	case GLFW_KEY_DOWN: // rotation around X axis
		transform.SetRotation(transform.GetRot() * (Quaternion(transform.GetRot().GetRight(), Angle(-sensitivity))).Normalized());
		//RotateX(Angle(rotationAmount));
		break;
	case GLFW_KEY_LEFT: // rotation around Y axis
		transform.SetRotation(transform.GetRot() * (Quaternion(Camera::yAxis, Angle(sensitivity))));
		stdlog(Debug, LOGPLACE, "Transform.Pos = %s", transform.GetPos().ToString().c_str());
		stdlog(Debug, LOGPLACE, "Transform.Rot = %s", transform.GetRot().ToString().c_str());
		break;
	case GLFW_KEY_RIGHT: // rotation around Y axis
		transform.SetRotation(transform.GetRot() * (Quaternion(Camera::yAxis, Angle(-sensitivity))));
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