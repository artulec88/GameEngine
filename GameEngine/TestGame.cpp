#include "TestGame.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\Camera.h"
#include "Rendering\MeshRenderer.h"
#include "Rendering\LookAtComponent.h"

#include "Math\FloatingPoint.h"
#include "Math\Quaternion.h"
//#include "Math\Vector.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\Log.h"
#include "Utility\Config.h"

#include <sstream>

using namespace Utility;
using namespace Math;
using namespace Rendering;

#define RENDER_LIGHT_MESHES

TestGame::TestGame() :
	Game(),
	humanCount(2),
	humanNodes(NULL),
	directionalLightNode(NULL),
	pointLightCount(Config::Get("pointLightCount", 1)),
	pointLightNodes(NULL),
	spotLightCount(Config::Get("spotLightCount", 1)),
	spotLightNodes(NULL),
	cameraCount(Config::Get("cameraCount", 3)),
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
		new Mesh("..\\Models\\SimpleCrate\\CrateModel.obj"),
		new Material(
			new Texture("..\\Textures\\crateBox2.jpg", GL_TEXTURE_2D, GL_LINEAR), 1.0f, 2.0f)));
	boxNode->GetTransform().SetTranslation(12.0f, 3.5f, 9.0f);
	boxNode->GetTransform().SetScale(Vector3D(0.05f, 0.05f, 0.05f));
	AddToSceneRoot(boxNode);

	// TODO: Do not use hard-coded values
	Math::Real specularIntensity = Config::Get("defaultSpecularIntensity", 1.0f);
	Math::Real specularPower = Config::Get("defaultSpecularPower", 8.0f);

	GameNode* planeNode = new GameNode();
	planeNode->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\plane3.obj"),
		new Material(
			new Texture("..\\Textures\\bricks.jpg", GL_TEXTURE_2D, GL_LINEAR), specularIntensity, specularPower,
			new Texture("..\\Textures\\bricks_normal.jpg", GL_TEXTURE_2D, GL_LINEAR),
			new Texture("..\\Textures\\bricks_disp.png", GL_TEXTURE_2D, GL_LINEAR), 0.03f, -0.25f)));
	planeNode->GetTransform().SetTranslation(0.0f, -1.0f, 5.0f);
	planeNode->GetTransform().SetScale(Math::Vector3D(4.0f, 4.0f, 4.0f));

	AddToSceneRoot(planeNode);

	GameNode* testMesh1 = new GameNode();
	testMesh1->GetTransform().SetTranslation(-2.0f, 2.5f, 2.0f);
	testMesh1->GetTransform().SetScale(Math::Vector3D(0.5f, 0.5f, 0.5f));
	GameNode* testMesh2 = new GameNode();
	testMesh2->GetTransform().SetTranslation(9.0f, 0.0f, 0.0f);
	//testMesh2->GetTransform().SetScale(Math::Vector3D(0.5f, 0.5f, 0.5f));
	testMesh1->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\plane.obj"),
		new Material(
			new Texture("..\\Textures\\bricks2.jpg", GL_TEXTURE_2D, GL_LINEAR), specularIntensity, specularPower,
			new Texture("..\\Textures\\bricks2_normal.jpg", GL_TEXTURE_2D, GL_LINEAR),
			new Texture("..\\Textures\\bricks2_disp.jpg", GL_TEXTURE_2D, GL_LINEAR), 0.03f, -1.0f)));
	testMesh2->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\plane.obj"),
		new Material(
			new Texture("..\\Textures\\bricks2.jpg", GL_TEXTURE_2D, GL_LINEAR), specularIntensity, specularPower)));
	AddToSceneRoot(testMesh1);
	//AddToSceneRoot(testMesh2);
	testMesh1->AddChild(testMesh2);

	GameNode* monkeyNode1 = new GameNode();
	monkeyNode1->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\monkey3.obj"),
		new Material(
			new Texture("..\\Textures\\chessboard.jpg", GL_TEXTURE_2D, GL_LINEAR), 1, 8)));
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

	srand((unsigned int)time(NULL));

	/* ==================== Adding directional light begin ==================== */
	bool directionalLightEnabled = Config::Get("directionalLightEnabled", true);
	if (directionalLightEnabled)
	{
		stdlog(Notice, LOGPLACE, "Directional light enabled");
		directionalLightNode = new GameNode();

		const Math::Vector3D defaultDirectionalLightPos(Config::Get("defaultDirectionalLightPosX", 0.0), Config::Get("defaultDirectionalLightPosY", 0.0), Config::Get("defaultDirectionalLightPosZ", 0.0));
		const Math::Vector3D defaultDirectionalLightColor(Config::Get("defaultDirectionalLightColorRed", 1.0), Config::Get("defaultDirectionalLightColorGreen", 1.0), Config::Get("defaultDirectionalLightColorBlue", 1.0));
		const Math::Real defaultDirectionalLightIntensity(Config::Get("defaultDirectionalLightIntensity", 1.0));
		const Angle defaultDirectionalLightRotationX(Config::Get("defaultDirectionalLightAngleX", -45.0f));
		const Angle defaultDirectionalLightRotationY(Config::Get("defaultDirectionalLightAngleY", 0.0f));
		const Angle defaultDirectionalLightRotationZ(Config::Get("defaultDirectionalLightAngleZ", 0.0f));

		Math::Real xPos = Config::Get("directionalLightPosX", defaultDirectionalLightPos.GetX());
		Math::Real yPos = Config::Get("directionalLightPosY", defaultDirectionalLightPos.GetY());
		Math::Real zPos = Config::Get("directionalLightPosZ", defaultDirectionalLightPos.GetZ());
		directionalLightNode->GetTransform().SetTranslation(xPos, yPos, zPos);

		Angle angleX(Config::Get("directionalLightAngleX", defaultDirectionalLightRotationX.GetAngleInDegrees()));
		Angle angleY(Config::Get("directionalLightAngleY", defaultDirectionalLightRotationY.GetAngleInDegrees()));
		Angle angleZ(Config::Get("directionalLightAngleZ", defaultDirectionalLightRotationZ.GetAngleInDegrees()));
		Matrix4D rotMatrix = Matrix4D::Rotation(angleX, angleY, angleZ);
		Quaternion rot(rotMatrix);
		directionalLightNode->GetTransform().SetRotation(rot);
		//directionalLightNode->GetTransform().SetRotation(Quaternion(Vector3D(1.0f, 1.0f, 0.0f), Angle(45.0f)));

		Math::Real red = Config::Get("directionalLightColorRed", defaultDirectionalLightColor.GetX() /* Red */);
		Math::Real green = Config::Get("directionalLightColorGreen", defaultDirectionalLightColor.GetY() /* Green */);
		Math::Real blue = Config::Get("directionalLightColorBlue", defaultDirectionalLightColor.GetZ() /* Blue */);
		Math::Vector3D color(red, green, blue);
		
		Math::Real intensity = Config::Get("directionalLightIntensity", defaultDirectionalLightIntensity);

		directionalLightNode->AddComponent(new DirectionalLight(color, intensity));
		AddToSceneRoot(directionalLightNode);

		// Rendering a small box around point light node position to let the user see the source
#ifdef RENDER_LIGHT_MESHES
		directionalLightNode->AddComponent(new MeshRenderer(
			new Mesh("..\\Models\\DirectionalLight.obj"),
			new Material(new Texture("..\\Textures\\DirectionalLight.png"), 1, 8)));
		directionalLightNode->GetTransform().SetScale(Math::Vector3D(0.1f, 0.1f, 0.1f));
#endif
	}
	/* ==================== Adding directional light end ==================== */

	if (pointLightCount > 0)
	{
		stdlog(Notice, LOGPLACE, "Creating %d point lights", pointLightCount);
		AddPointLights();
	}
	if (spotLightCount > 0)
	{
		stdlog(Notice, LOGPLACE, "Creating %d spot lights", spotLightCount);
		AddSpotLights();
	}
	if (cameraCount > 0)
	{
		stdlog(Notice, LOGPLACE, "Creating %d cameras", cameraCount);
		AddCameras();
	}
	else
	{
		stdlog(Error, LOGPLACE, "No cameras defined.");
		exit(EXIT_FAILURE);
	}
}

void TestGame::AddPointLights()
{
	if (pointLightCount < 1)
	{
		return;
	}

	const Math::Vector3D defaultPointLightPos(Config::Get("defaultPointLightPosX", 0.0), Config::Get("defaultPointLightPosY", 0.0), Config::Get("defaultPointLightPosZ", 0.0));
	const Angle defaultPointLightRotationX(Config::Get("defaultPointLightAngleX", -45.0f));
	const Angle defaultPointLightRotationY(Config::Get("defaultPointLightAngleY", 0.0f));
	const Angle defaultPointLightRotationZ(Config::Get("defaultPointLightAngleZ", 0.0f));
	const Math::Vector3D defaultPointLightColor(Config::Get("defaultPointLightColorRed", 0.0), Config::Get("defaultPointLightColorGreen", 0.0), Config::Get("defaultPointLightColorBlue", 1.0));
	const Math::Real defaultPointLightIntensity(Config::Get("defaultPointLightIntensity", 10.0));
	const Attenuation defaultPointLightAttenuation(Config::Get("defaultPointLightAttenuationConstant", 0.0), Config::Get("defaultPointLightAttenuationLinear", 0.1), Config::Get("defaultPointLightAttenuationExponent", 0.0));
	pointLightNodes = new GameNode* [pointLightCount];
	for (int i = 0; i < pointLightCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		std::string pointLightIndexStr = ss.str();

		pointLightNodes[i] = new GameNode();
		
		Math::Real xPos = Config::Get("pointLightPosX_" + pointLightIndexStr, defaultPointLightPos.GetX());
		Math::Real yPos = Config::Get("pointLightPosY_" + pointLightIndexStr, defaultPointLightPos.GetY());
		Math::Real zPos = Config::Get("pointLightPosZ_" + pointLightIndexStr, defaultPointLightPos.GetZ());
		pointLightNodes[i]->GetTransform().SetTranslation(xPos, yPos, zPos);
		
		Angle angleX(Config::Get("pointLightAngleX_" + pointLightIndexStr, defaultPointLightRotationX.GetAngleInDegrees()));
		Angle angleY(Config::Get("pointLightAngleY_" + pointLightIndexStr, defaultPointLightRotationY.GetAngleInDegrees()));
		Angle angleZ(Config::Get("pointLightAngleZ_" + pointLightIndexStr, defaultPointLightRotationZ.GetAngleInDegrees()));
		Matrix4D rotMatrix = Matrix4D::Rotation(angleX, angleY, angleZ);
		Quaternion rot(rotMatrix);
		pointLightNodes[i]->GetTransform().SetRotation(rot);

		Math::Real red = Config::Get("pointLightColorRed_" + pointLightIndexStr, defaultPointLightColor.GetX() /* Red */);
		Math::Real green = Config::Get("pointLightColorGreen_" + pointLightIndexStr, defaultPointLightColor.GetY() /* Green */);
		Math::Real blue = Config::Get("pointLightColorBlue_" + pointLightIndexStr, defaultPointLightColor.GetZ() /* Blue */);
		Math::Vector3D color(red, green, blue);
		
		Math::Real intensity = Config::Get("pointLightIntensity_" + pointLightIndexStr, defaultPointLightIntensity);
		
		Math::Real constant = Config::Get("pointLightAttenuationConstant_" + pointLightIndexStr, defaultPointLightAttenuation.GetConstant());
		Math::Real linear = Config::Get("pointLightAttenuationLinear_" + pointLightIndexStr, defaultPointLightAttenuation.GetLinear());
		Math::Real exponent = Config::Get("pointLightAttenuationExponent_" + pointLightIndexStr, defaultPointLightAttenuation.GetExponent());
		Attenuation attenuation(constant, linear, exponent);

		pointLightNodes[i]->AddComponent(new PointLight(color, intensity, attenuation));
		
		// Rendering a small box around point light node position to let the user see the source
#ifdef RENDER_LIGHT_MESHES
		pointLightNodes[i]->AddComponent(new MeshRenderer(
			new Mesh("..\\Models\\PointLight.obj"),
			new Material(new Texture("..\\Textures\\PointLight.png"), 1, 8)));
		pointLightNodes[i]->GetTransform().SetScale(Math::Vector3D(0.1f, 0.1f, 0.1f));
#endif
		
		AddToSceneRoot(pointLightNodes[i]);
	}
}

void TestGame::AddSpotLights()
{
	if (spotLightCount < 1)
	{
		return;
	}
	const Math::Vector3D defaultSpotLightPos(Config::Get("defaultSpotLightPosX", 0.0f), Config::Get("defaultSpotLightPosY", 0.0f), Config::Get("defaultSpotLightPosZ", 0.0f));
	const Angle defaultSpotLightRotationX(Config::Get("defaultSpotLightAngleX", -45.0f));
	const Angle defaultSpotLightRotationY(Config::Get("defaultSpotLightAngleY", 0.0f));
	const Angle defaultSpotLightRotationZ(Config::Get("defaultSpotLightAngleZ", 0.0f));
	const Math::Vector3D defaultSpotLightColor(Config::Get("defaultSpotLightColorRed", 0.0f), Config::Get("defaultSpotLightColorGreen", 0.0f), Config::Get("defaultSpotLightColorBlue", 1.0f));
	const Math::Real defaultSpotLightIntensity(Config::Get("defaultSpotLightIntensity", 4.0f));
	const Attenuation defaultSpotLightAttenuation(Config::Get("defaultSpotLightAttenuationConstant", 0.5f), Config::Get("defaultSpotLightAttenuationLinear", 0.1f), Config::Get("defaultSpotLightAttenuationExponent", 0.05f));
	const Math::Real defaultSpotLightCutoff(Config::Get("defaultSpotLightCutoff", 0.45f));
	spotLightNodes = new GameNode* [spotLightCount];
	for (int i = 0; i < spotLightCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		std::string spotLightIndexStr = ss.str();

		spotLightNodes[i] = new GameNode();
		
		Math::Real xPos = Config::Get("spotLightPosX_" + spotLightIndexStr, defaultSpotLightPos.GetX());
		Math::Real yPos = Config::Get("spotLightPosY_" + spotLightIndexStr, defaultSpotLightPos.GetY());
		Math::Real zPos = Config::Get("spotLightPosZ_" + spotLightIndexStr, defaultSpotLightPos.GetZ());
		spotLightNodes[i]->GetTransform().SetTranslation(xPos, yPos, zPos);
		
		Angle angleX(Config::Get("spotLightAngleX_" + spotLightIndexStr, defaultSpotLightRotationX.GetAngleInDegrees()));
		Angle angleY(Config::Get("spotLightAngleY_" + spotLightIndexStr, defaultSpotLightRotationY.GetAngleInDegrees()));
		Angle angleZ(Config::Get("spotLightAngleZ_" + spotLightIndexStr, defaultSpotLightRotationZ.GetAngleInDegrees()));
		Matrix4D rotMatrix = Matrix4D::Rotation(angleX, angleY, angleZ);
		Quaternion rot(rotMatrix);
		spotLightNodes[i]->GetTransform().SetRotation(rot);

		Math::Real red = Config::Get("spotLightColorRed_" + spotLightIndexStr, defaultSpotLightColor.GetX() /* Red */);
		Math::Real green = Config::Get("spotLightColorGreen_" + spotLightIndexStr, defaultSpotLightColor.GetY() /* Green */);
		Math::Real blue = Config::Get("spotLightColorBlue_" + spotLightIndexStr, defaultSpotLightColor.GetZ() /* Blue */);
		Math::Vector3D color(red, green, blue);
		
		Math::Real intensity = Config::Get("spotLightIntensity_" + spotLightIndexStr, defaultSpotLightIntensity);
		
		Math::Real constant = Config::Get("spotLightAttenuationConstant_" + spotLightIndexStr, defaultSpotLightAttenuation.GetConstant());
		Math::Real linear = Config::Get("spotLightAttenuationLinear_" + spotLightIndexStr, defaultSpotLightAttenuation.GetLinear());
		Math::Real exponent = Config::Get("spotLightAttenuationExponent_" + spotLightIndexStr, defaultSpotLightAttenuation.GetExponent());
		Attenuation attenuation(constant, linear, exponent);

		Math::Real cutoff = Config::Get("spotLightCutoff_" + spotLightIndexStr, defaultSpotLightCutoff);

		spotLightNodes[i]->AddComponent(new SpotLight(color, intensity, attenuation, cutoff));
		
		// Rendering a small box around spot light node position to let the user see the source
#ifdef RENDER_LIGHT_MESHES
		spotLightNodes[i]->AddComponent(new MeshRenderer(
			new Mesh("..\\Models\\SpotLight.obj"),
			new Material(new Texture("..\\Textures\\SpotLight.png"), 1, 8)));
		spotLightNodes[i]->GetTransform().SetScale(Math::Vector3D(0.1f, 0.1f, 0.1f));
#endif
		
		AddToSceneRoot(spotLightNodes[i]);
	}
}

void TestGame::AddCameras()
{
	if (cameraCount < 1)
	{
		stdlog(Error, LOGPLACE, "No cameras defined.");
		exit(EXIT_FAILURE);
	}

	const Real defaultFoV = Config::Get("defaultCameraFoV", 70.0);
	const Real defaultAspectRatio = Config::Get("defaultCameraAspectRatio", static_cast<Real>(800) / 600);
	const Real defaultNearPlane = Config::Get("defaultCameraNearPlane", 0.1);
	const Real defaultFarPlane = Config::Get("defaultCameraFarPlane", 1000.0);
	
	const Vector3D defaultCameraPos(Config::Get("defaultCameraPosX", 0.0f), Config::Get("defaultCameraPosY", 0.0), Config::Get("defaultCameraPosZ", 0.0));
	const Angle defaultCameraRotationX(Config::Get("defaultCameraAngleX", -45.0f));
	const Angle defaultCameraRotationY(Config::Get("defaultCameraAngleY", 0.0));
	const Angle defaultCameraRotationZ(Config::Get("defaultCameraAngleZ", 0.0));

	cameraNodes = new GameNode* [cameraCount];
	for (int i = 0; i < cameraCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		std::string cameraIndexStr = ss.str();

		cameraNodes[i] = new GameNode();
		Math::Real xPos = Config::Get("cameraPosX_" + cameraIndexStr, defaultCameraPos.GetX());
		Math::Real yPos = Config::Get("cameraPosY_" + cameraIndexStr, defaultCameraPos.GetY());
		Math::Real zPos = Config::Get("cameraPosZ_" + cameraIndexStr, defaultCameraPos.GetZ());
		cameraNodes[i]->GetTransform().SetTranslation(xPos, yPos, zPos);
		
		Angle angleX(Config::Get("cameraAngleX_" + cameraIndexStr, defaultCameraRotationX.GetAngleInDegrees()));
		Angle angleY(Config::Get("cameraAngleY_" + cameraIndexStr, defaultCameraRotationY.GetAngleInDegrees()));
		Angle angleZ(Config::Get("cameraAngleZ_" + cameraIndexStr, defaultCameraRotationZ.GetAngleInDegrees()));
		Matrix4D rotMatrix = Matrix4D::Rotation(angleX, angleY, angleZ);
		Quaternion rot(rotMatrix);
		cameraNodes[i]->GetTransform().SetRotation(rot);

		Angle fov(Config::Get("cameraFoV_" + cameraIndexStr, defaultFoV), true);
		Math::Real aspectRatio = Config::Get("cameraAspectRatio_" + cameraIndexStr, defaultAspectRatio);
		Math::Real zNearPlane = Config::Get("cameraNearPlane_" + cameraIndexStr, defaultNearPlane);
		Math::Real zFarPlane = Config::Get("cameraFarPlane_" + cameraIndexStr, defaultFarPlane);
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
		t.SetTranslation(t.GetPos() + (Math::Vector3D(sin(temp) / 1000, cos(temp) / 2000, cos(temp) / 1000)));
	}

	for (int i = 0; i < spotLightCount; ++i)
	{
		Transform& t = spotLightNodes[i]->GetTransform();
		t.SetTranslation(t.GetPos() + (Math::Vector3D(sin(temp) / 1000, sin(temp) / 2000, cos(temp) / 1000)));
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