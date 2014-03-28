#include "TestGame.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\BasicShader.h"
#include "Rendering\MeshRenderer.h"
//#include "Rendering\DirectionalLight.h"
//#include "Rendering\PointLight.h"
//#include "Rendering\SpotLight.h"

//#include "Math\Vector.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\Log.h"
#include "Utility\Config.h"

using namespace Utility;
using namespace Math;
using namespace Rendering;

TestGame::TestGame() :
	Game(),
	directionalLightObject(NULL),
	pointLightObject(NULL),
	spotLightObject(NULL)
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
	Vertex vertex1(Vector3D(-fieldWidth, 0.0, -fieldDepth), Vector2D(0.0, 0.0));
	Vertex vertex2(Vector3D(-fieldWidth, 0.0, 3 * fieldDepth), Vector2D(0.0, 1.0));
	Vertex vertex3(Vector3D(3 * fieldWidth, 0.0, -fieldDepth), Vector2D(1.0, 0.0));
	Vertex vertex4(Vector3D(3 * fieldWidth, 0.0, 3 * fieldDepth), Vector2D(1.0, 1.0));
	Vertex vertices[] = {vertex1, vertex2, vertex3, vertex4};
	unsigned short indices[] = {0, 1, 2,
							  2, 1, 3};
	Mesh* mesh = new Mesh(vertices, 4, indices, 6, true);
	//mesh = new Mesh("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Models\\box.obj");

	Math::Vector3D materialColor = Math::Vector3D(1.0, 1.0, 1.0);
	Math::Real specularIntensity = 1.0;
	Math::Real specularPower = 8.0;
	//material = new Material(NULL, materialColor, specularIntensity, specularPower);
	Material* material = new Material(new Texture("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Textures\\chessboard2.jpg", GL_TEXTURE_2D, GL_LINEAR),
		materialColor, specularIntensity, specularPower);

	MeshRenderer* meshRenderer = new MeshRenderer(mesh, material);
	
	Math::Real fov = GET_CONFIG_VALUE("FieldOfView", "FieldOfViewDefault", 70.0);
	Math::Real zNear = GET_CONFIG_VALUE("zNearClippingPlane", "zNearClippingPlaneDefault", 0.1);
	Math::Real zFar = GET_CONFIG_VALUE("zFarClippingPlane", "zFarClippingPlaneDefault", 0.1);

	GameNode* planeObject = new GameNode();
	planeObject->AddComponent(meshRenderer);
	planeObject->GetTransform().SetTranslation(0.0, -1.0, 5.0);

	directionalLightObject = new GameNode();
	directionalLightObject->AddComponent(new DirectionalLight(Math::Vector3D(1.0, 1.0, 1.0), 0.8, Math::Vector3D(1.0, 1.0, 1.0)));

	pointLightObject = new GameNode();
	pointLightObject->AddComponent(new PointLight(Math::Vector3D(0.0, 1.0, 0.0), 2.8, Attenuation(0.0, 0.0, 1.0)));

	spotLightObject = new GameNode();
	spotLightObject->AddComponent(new SpotLight(Math::Vector3D(1.0, 1.0f, 1.0f), 0.8f, Attenuation(0.0f, 0.1f, 0.0f), 0.7f));

	rootGameNode->AddChild(planeObject);
	//rootGameNode->AddChild(directionalLightObject);
	rootGameNode->AddChild(pointLightObject);
	rootGameNode->AddChild(spotLightObject);
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

	pointLightObject->GetTransform().SetTranslation(10.0 * abs(sin(temp)), 0.0, 7.0 + 10.0 * abs(cos(temp)));

	//spotLightObject->GetTransform().SetTranslation(1.0, 10.0 * abs(cos(static_cast<Math::Real>(rand() % 90) / 11)), 5.0 + 10.0 * abs(sin(temp)));
	spotLightObject->GetTransform().SetTranslation(1.0, -1.0 + 10.0 * abs(cos(temp)), 10.0 * abs(sin(temp)));

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
	//camera->Input(key, 0.02);
}