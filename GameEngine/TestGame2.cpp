#include "TestGame2.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\PhongShader.h"

//#include "Math\Vector.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\Log.h"

using namespace Utility;
using namespace Math;
using namespace Rendering;

TestGame2::TestGame2() :
	Game()
{
	stdlog(Debug, LOGPLACE, "TestGame2 construction started");
	stdlog(Debug, LOGPLACE, "TestGame2 construction finished");
}


TestGame2::~TestGame2(void)
{
	if (mesh != NULL)
	{
		delete mesh;
		mesh = NULL;
	}
	if (shader != NULL)
	{
		delete shader;
		shader = NULL;
	}
	if (transform != NULL)
	{
		delete transform;
		transform = NULL;
	}
	if (material != NULL)
	{
		delete material;
		material = NULL;
	}
	if (pointLights != NULL)
	{
		delete [] pointLights;
		pointLights = NULL;
	}
	if (spotLights != NULL)
	{
		delete [] spotLights;
		spotLights = NULL;
	}
}

void TestGame2::Init()
{
	stdlog(Notice, LOGPLACE, "Initalizing test game");

	if (rootGameNode == NULL)
	{
		stdlog(Critical, LOGPLACE, "Root game node is not initialized");
		exit(EXIT_FAILURE);
	}
	//rootGameNode->Init();
	
	//Vertex vertex1(Vector3D(-1.0, -1.0, 0.5773), Vector2D(0.0, 1.0));
	//Vertex vertex2(Vector3D(0.0, -1.0, -1.15475), Vector2D(0.5, 0.0));
	//Vertex vertex3(Vector3D(1.0, -1.0, 0.5773), Vector2D(1.0, 0.0));
	//Vertex vertex4(Vector3D(0.0, 1.0, 0.0), Vector2D(0.5, 1.0));
	//Vertex vertices[] = {vertex1, vertex2, vertex3, vertex4};
	//unsigned short indices[] = {0, 3, 1,
	//						  1, 3, 2,
	//						  2, 3, 0,
	//						  1, 2, 0};

	Math::Real fieldDepth = 10.0;
	Math::Real fieldWidth = 10.0;
	Vertex vertex1(Vector3D(-fieldWidth, 0.0, -fieldDepth), Vector2D(0.0, 0.0));
	Vertex vertex2(Vector3D(-fieldWidth, 0.0, 3 * fieldDepth), Vector2D(0.0, 1.0));
	Vertex vertex3(Vector3D(3 * fieldWidth, 0.0, -fieldDepth), Vector2D(1.0, 0.0));
	Vertex vertex4(Vector3D(3 * fieldWidth, 0.0, 3 * fieldDepth), Vector2D(1.0, 1.0));
	Vertex vertices[] = {vertex1, vertex2, vertex3, vertex4};
	unsigned short indices[] = {0, 1, 2,
							  2, 1, 3};
	mesh = new Mesh(vertices, 4, indices, 6, true);
	//mesh = new Mesh("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Models\\box.obj");

	Math::Vector3D materialColor = Math::Vector3D(1.0, 1.0, 1.0);
	Math::Real specularIntensity = 1.0;
	Math::Real specularPower = 8.0;
	//material = new Material(NULL, materialColor, specularIntensity, specularPower);
	material = new Material(new Texture("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Textures\\chessboard2.jpg", GL_TEXTURE_2D, GL_LINEAR),
		materialColor, specularIntensity, specularPower);

	PhongShader::SetAmbientLight(Math::Vector3D(0.02, 0.02, 0.02));
	PhongShader::SetDirectionalLight(DirectionalLight(Math::Vector3D(1.0, 1.0, 1.0), 0.5, Math::Vector3D(1.0, 1.0, 1.0)));
	
	//PointLight pLight1(Math::Vector3D(1.0, 1.0, 1.0), 1.8, Attenuation(0.7, 0.7, 1.0), Math::Vector3D(-2.0, 0, 3.0));
	//PointLight pLight2(Math::Vector3D(0.0, 0.0, 1.0), 0.8, Attenuation(0.7, 0.7, 1.0), Math::Vector3D(2.0, 0.0, 7.0));
	//PointLight pLight3(Math::Vector3D(0.0, 0.0, 1.0), 0.8, Attenuation(0.0, 0.0, 1.0), Math::Vector3D(2.0, 0, 7.0));
	//PointLight pLight4(Math::Vector3D(0.0, 0.0, 1.0), 0.8, Attenuation(0.0, 0.0, 1.0), Math::Vector3D(2.0, 0, 7.0));
	pointLights = new PointLight [2];
	pointLights[0] = PointLight(Math::Vector3D(1.0, 0.5, 0.0), 0.8, Attenuation(1.0, 0.0, 1.0), Math::Vector3D(-2.0, 0, 5.0), 6.0);
	pointLights[1] = PointLight(Math::Vector3D(0.0, 0.5, 1.0), 0.8, Attenuation(3.0, 1.0, 0.0), Math::Vector3D(2.0, 0.0, 7.0), 12.0);
	PhongShader::SetPointLights(pointLights, 2);

	//SpotLight sLight1(Math::Vector3D(1.0, 1.0, 1.0), 1.8, Attenuation(0.7, 0.7, 1.0), Math::Vector3D(-2.0, 0, 3.0));
	//SpotLight sLight2(Math::Vector3D(0.0, 0.0, 1.0), 0.8, Attenuation(0.7, 0.7, 1.0), Math::Vector3D(2.0, 0.0, 7.0));
	//SpotLight sLight3(Math::Vector3D(0.0, 0.0, 1.0), 0.8, Attenuation(0.0, 0.0, 1.0), Math::Vector3D(2.0, 0, 7.0));
	//SpotLight sLight4(Math::Vector3D(0.0, 0.0, 1.0), 0.8, Attenuation(0.0, 0.0, 1.0), Math::Vector3D(2.0, 0, 7.0));
	spotLights = new SpotLight [1];
	spotLights[0] = SpotLight(Math::Vector3D(0.0, 1.0f, 1.0f), 0.8f, Attenuation(0.0f, 1.0f, 0.0f), Math::Vector3D(-2.0, 0, 5.0), 30.0, Math::Vector3D(1.0f, 1.0f, 1.0f), 0.7f);
	//spotLights[1] = SpotLight(Math::Vector3D(0.0, 0.5, 1.0), 0.8, Attenuation(3.0, 1.0, 0.0), Math::Vector3D(2.0, 0.0, 7.0), 12.0, new Math::Vector3D(1.0f, 1.0f, 1.0f), 0.7f);
	PhongShader::SetSpotLights(spotLights, 1);
	
	shader = new PhongShader();
	if (shader == NULL)
	{
		stdlog(Critical, LOGPLACE, "Shader has not been initialized correctly");
		exit(INVALID_VALUE);
	}

	transform = new Transform();

	//GLuint programID = LoadShaders("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\BasicVertexShader.vshader",
	//	"C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\BasicFragmentShader.fshader");
	//glUseProgram(programID);
}

void TestGame2::Input()
{
	stdlog(Delocust, LOGPLACE, "Input checked");
}

void TestGame2::Update()
{
	//stdlog(Delocust, LOGPLACE, "Game is being updated");
	
	Math::Real temp = static_cast<Math::Real>(CoreEngine::GetCoreEngine()->GetTime());
	Math::Real sinTemp = static_cast<Math::Real>(sin(temp));
	Math::Real cosTemp = static_cast<Math::Real>(cos(temp));
	//transform->SetTranslation(sin(temp), 0.0, 2.0);
	//transform->SetRotation(0.0, sin(temp) * 180, 0.0);
	//transform->SetScale(0.5 * sin(temp));

	if (transform != NULL)
	{
		transform->SetTranslation(0.0, -1.0, 5.0);
	}
	if (pointLights != NULL)
	{
		pointLights[0].SetPosition(Math::Vector3D(3.0, 0.0, 8.0 * (sinTemp + 0.5) + 10.0));
		pointLights[1].SetPosition(Math::Vector3D(17.0, 0.0, 8.0 * (cosTemp + 0.5) + 10.0));
	}
	if (spotLights != NULL)
	{
		//if (camera != NULL)
		//{
		//	spotLights[0].SetPosition(camera->GetPos());
		//	spotLights[0].SetDirection(camera->GetForward());
		//}
		//else
		//{
		//	stdlog(Warning, LOGPLACE, "Camera is not initialized");
		//}
	}

	//stdlog(Delocust, LOGPLACE, "Transform = \n%s", transform->GetTransformation().ToString().c_str());
}

void TestGame2::Render()
{
	//stdlog(Debug, LOGPLACE, "One single frame is being rendered");
	if (shader == NULL)
	{
		stdlog(Warning, LOGPLACE, "Shader instance is NULL");
	}
	if (mesh == NULL)
	{
		stdlog(Warning, LOGPLACE, "Mesh instance is NULL");
	}

	//if (shader != NULL)
	//{
	//	shader->Bind();
	//	// TODO: Remember to set the uniform after binding the shader
	//	if (material == NULL)
	//	{
	//		stdlog(Emergency, LOGPLACE, "Material is NULL");
	//		exit(EXIT_FAILURE);
	//	}
	//	shader->UpdateUniforms(transform, *material, CoreEngine::GetCoreEngine()->GetRenderer());
	//}

	if (mesh != NULL)
	{
		mesh->Draw();
	}
}

/**
 * @param key the keyboard key that was pressed or released
 * @param scancode the system-specific scancode of the key
 * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
 * @param mods Bit field describing which modifier keys were held down
 */
void TestGame2::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
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