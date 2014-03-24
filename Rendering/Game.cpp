#include "stdafx.h"
#include "Game.h"
#include "CoreEngine.h"
#include "Vertex.h"
#include "BasicShader.h"
#include "PhongShader.h"
#include "Attenuation.h"
#include "PointLight.h"
#include "Texture.h"

#include "Math\Math.h"
#include "Math\Vector.h"

#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\Log.h"
#include "Utility\Time.h"

#include <fstream>

using namespace Utility;
using namespace Math;
using namespace Rendering;

Game* Game::game = NULL;

/* static */ Game* Game::GetGame()
{
	return game;
}

Game::Game()
{
	rootGameNode = GameNode();

	stdlog(Debug, LOGPLACE, "Game construction started");
	if (game != NULL)
	{
		stdlog(Error, LOGPLACE, "Constructor called when a singleton instance of MainApp class has already been created");
		delete game;
		game = NULL;
	}
	game = this;
	stdlog(Debug, LOGPLACE, "Game construction finished");
}


Game::~Game(void)
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
	if (camera != NULL)
	{
		delete camera;
		camera = NULL;
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

void Game::Init()
{
	stdlog(Delocust, LOGPLACE, "Initalizing game");

	rootGameNode.Init();
	
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

	camera = new Camera();
	
	Transform::SetCamera(*camera);
	transform = new Transform();

	//GLuint programID = LoadShaders("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\BasicVertexShader.vshader",
	//	"C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\BasicFragmentShader.fshader");
	//glUseProgram(programID);
}

//
//void Game::CleanUp()
//{
//	stdlog(Info, LOGPLACE, "The game is being cleaned up");
//}

void Game::Input()
{
	stdlog(Delocust, LOGPLACE, "Input checked");
}

void Game::Update()
{
	//stdlog(Delocust, LOGPLACE, "Game is being updated");
	
	Math::Real temp = static_cast<Math::Real>(glfwGetTime());
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
		if (camera != NULL)
		{
			spotLights[0].SetPosition(camera->GetPos());
			spotLights[0].SetDirection(camera->GetForward());
		}
		else
		{
			stdlog(Warning, LOGPLACE, "Camera is not initialized");
		}
	}

	//stdlog(Delocust, LOGPLACE, "Transform = \n%s", transform->GetTransformation().ToString().c_str());
}

void Game::Render()
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

	if (shader != NULL)
	{
		shader->Bind();
		// TODO: Remember to set the uniform after binding the shader
		if (material == NULL)
		{
			stdlog(Emergency, LOGPLACE, "Material is NULL");
			exit(EXIT_FAILURE);
		}
		shader->UpdateUniforms(transform->GetTransformation(), transform->GetProjectedTransformation(), *material);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // TODO: Remove in the future

	if (mesh != NULL)
	{
		mesh->Draw();
	}
}

/* static */ void Game::WindowCloseCallback(GLFWwindow* window)
{
	GetGame()->CloseWindowEvent(window);
}

/* static */ void Game::KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GetGame()->KeyEvent(window, key, scancode, action, mods);
}

void Game::CloseWindowEvent(GLFWwindow* window)
{
	stdlog(Notice, LOGPLACE, "Close window event");
	glfwSetWindowShouldClose(window, GL_TRUE);
	CoreEngine::GetCoreEngine()->Stop();
}

/**
 * @param key the keyboard key that was pressed or released
 * @param scancode the system-specific scancode of the key
 * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
 * @param mods Bit field describing which modifier keys were held down
 */
void Game::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	stdlog(Delocust, LOGPLACE, "Key event with key = %d", key);
	if (key == GLFW_KEY_ESCAPE)
	{
		CoreEngine::GetCoreEngine()->Stop();
		return;
	}

	ASSERT(camera != NULL);
	if (camera == NULL)
	{
		stdlog(Error, LOGPLACE, "Camera instance is not initialized");
		return;
	}

	// TODO: Set delta to correct value
	camera->Input(key, 0.02);
}

///* inline */ GameNode& Game::GetRootGameNode() const
//{
//	//ASSERT(rootGameNode != NULL);
//	//if (rootGameNode == NULL)
//	//{
//	//	stdlog(Emergency, LOGPLACE, "Root game node is NULL.");
//	//	// TODO: throw another exception in the future
//	//	throw FileNotFoundException();
//	//}
//
//	return rootGameNode;
//}

GLuint Game::LoadShaders(const char* vertexFilePath, const char* fragmentFilePath)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertexFilePath, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertexFilePath);
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragmentFilePath, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertexFilePath);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragmentFilePath);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}
	
	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}