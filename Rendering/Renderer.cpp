#include "StdAfx.h"
#include "Renderer.h"
#include "Game.h"
//#include "PhongShader.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
//#include "ForwardAmbientShader.h"
//#include "ForwardDirectionalShader.h"
#include "Utility\Config.h"
#include "Utility\Log.h"
#include "Utility\FileNotFoundException.h"

#include <sstream>

// Include GLM- a library for 3D mathematics
//#include <glm/glm.hpp>

using namespace Rendering;
using namespace Utility;
using namespace Math;

Renderer::Renderer(int width, int height, std::string title) :
	window(NULL),
	vao(0),
	isFullscreen(false),
	isMouseEnabled(true),
	ambientLight(GET_CONFIG_VALUE("ambientLight_x", "ambientLight_xDefault", 0.02f),
		GET_CONFIG_VALUE("ambientLight_y", "ambientLight_yDefault", 0.02f),
		GET_CONFIG_VALUE("ambientLight_z", "ambientLight_zDefault", 0.02f)),
	currentLight(NULL),
	currentCameraIndex(0),
	currentCamera(NULL),
	defaultShader(NULL)
{
	stdlog(Debug, LOGPLACE, "Creating Renderer instance started");
	stdlog(Notice, LOGPLACE, "OpenGL version = %s", GetOpenGLVersion().c_str());
	
	Init(width, height, title);

	samplerMap.insert(std::pair<std::string, unsigned int>("diffuse", 0));
	samplerMap.insert(std::pair<std::string, unsigned int>("normalMap", 1));
	samplerMap.insert(std::pair<std::string, unsigned int>("displacementMap", 2));
	AddVector3D("ambientIntensity", ambientLight);

	stdlog(Delocust, LOGPLACE, "Creating Renderer instance finished");
}


Renderer::~Renderer(void)
{
	stdlog(Debug, LOGPLACE, "Destroying rendering engine");
	
	//glDeleteVertexArrays(1, &vao);

	//if (cameras != NULL)
	//{
	//	delete [] cameras;
	//	cameras = NULL;
	//}
	//if (currentLight != NULL)
	//{
	//	delete currentLight;
	//	currentLight = NULL;
	//}
	// TODO: Deallocating the lights member variable
	// TODO: Deallocating the cameras member variable

	glfwTerminate();
	stdlog(Debug, LOGPLACE, "Rendering engine destroyed");
}

void Renderer::Init(int width, int height, std::string title)
{
	stdlog(Info, LOGPLACE, "Initializing Renderer started");
	// Initialise GLFW
	if( !glfwInit() )
	{
		stdlog(Critical, LOGPLACE, "Failed to initalize GLFW");
		exit(EXIT_FAILURE);
		// TODO: throw another exception in the future
		//throw FileNotFoundException();
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // TODO: Do not hard-code any values
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // TODO: Do not hard-code any values
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // TODO: Do not hard-code any values
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // So that glBegin / glEnd etc. work
#else
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	//window = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL); // fullscreen
	if (window == NULL)
	{
		stdlog(Critical, LOGPLACE, "Failed to open GLFW window.  If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowCloseCallback(window, &Game::WindowCloseEventCallback);
	//glfwSetWindowSizeCallback(window, Game::WindowResizeCallback);
	glfwSetKeyCallback(window, &Game::KeyEventCallback);
	//glfwSetCharCallback(window, Game::CharEventCallback);
	//glfwSetMousePosCallback(window, Game::MouseMotionCallback);
	glfwSetCursorPosCallback(window, &Game::MousePosCallback);
	glfwSetMouseButtonCallback(window, &Game::MouseEventCallback);
	glfwSetScrollCallback(window, &Game::ScrollEventCallback);

	glfwMakeContextCurrent(window);

	InitGlew();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, width / 2, height / 2);

	//glfwSwapInterval(1);
	glfwSetTime(0.0);

	InitGraphics();

	defaultShader = new Shader("ForwardAmbient");

	stdlog(Notice, LOGPLACE, "Using OpenGL version %s", GetOpenGLVersion().c_str());
}

void Renderer::InitGraphics()
{
	glClearColor(GET_CONFIG_VALUE("ClearColorRed", "ClearColorRedDefault", 0.0f), GET_CONFIG_VALUE("ClearColorGreen", "ClearColorGreenDefault", 0.0f), GET_CONFIG_VALUE("ClearColorBlue", "ClearColorBlueDefault", 0.0f), GET_CONFIG_VALUE("ClearColorAlpha", "ClearColorAlphaDefault", 0.0f));

	//glFrontFace(GL_CW); // every face I draw in clockwise order is a front face
	//glCullFace(GL_BACK); // cull the back face
	//glEnable(GL_CULL_FACE); // culling faces enabled. Cull triangles which normal is not towards the camera
	glEnable(GL_DEPTH_TEST); // to enable depth tests
	glEnable(GL_DEPTH_CLAMP); // prevents the camera to clip through the mesh

	//glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one

	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_FRAMEBUFFER_SRGB); // Essentialy gives free gamma correction for better contrast. TODO: Test it!

	//ASSERT(!vao);
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	//ASSERT(!framebuffer);
	//glGenFramebuffers(1, &framebuffer);
}

void Renderer::InitGlew() const
{
	stdlog(Info, LOGPLACE, "Initializing GLEW...");
	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		stdlog(Error, LOGPLACE, "Error while initializing GLEW: %s", glewGetErrorString(err));
		return;
	}

	if (GLEW_VERSION_2_0)
	{
		stdlog(Info, LOGPLACE, "OpenGL 2.0 supported");
	}
	else
	{
		stdlog(Info, LOGPLACE, "OpenGL 2.0 NOT supported");
		exit(EXIT_FAILURE);
	}

	stdlog(Notice, LOGPLACE, "Using GLEW version %s", glewGetString(GLEW_VERSION));
}

void Renderer::Render(GameNode& gameNode)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0 /*framebuffer*/);

	// TODO: Expand with Stencil buffer once it is used
	ClearScreen();

	currentCamera = cameras[currentCameraIndex];
	//currentCamera->Activate();

	// Ambient rendering
	gameNode.RenderAll(defaultShader, this);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both wages equal to 1
	glDepthMask(GL_FALSE); // Disable writing to the depth buffer (Z-buffer). We are after the ambient rendering pass, so we do not need to write to Z-buffer anymore
	glDepthFunc(GL_EQUAL); // CRITICAL FOR PERFORMANCE SAKE! This will allow calculating the light only for the pixel which will be seen in the final rendered image

	// TODO: Perform any other lighting calculations here
	for (std::vector<BaseLight*>::iterator lightItr = lights.begin(); lightItr != lights.end(); ++lightItr)
	{
		currentLight = (*lightItr);
		// TODO: Get a shader instance from the currentLight object.
		gameNode.RenderAll(currentLight->GetShader(), this);
	}

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
}

void Renderer::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Renderer::ClearScreen() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline Camera& Renderer::GetCurrentCamera()
{
	if (currentCamera == NULL)
	{
		stdlog(Emergency, LOGPLACE, "Current camera is NULL");
		exit(EXIT_FAILURE);
	}
	return *currentCamera;
}

unsigned int Renderer::NextCamera()
{
	if (currentCameraIndex == cameras.size() - 1)
	{
		currentCameraIndex = -1;
	}
	return SetCurrentCamera(currentCameraIndex + 1);
}

unsigned int Renderer::PrevCamera()
{
	if (currentCameraIndex == 0)
	{
		currentCameraIndex = cameras.size();
	}
	return SetCurrentCamera(currentCameraIndex - 1);
}

unsigned int Renderer::SetCurrentCamera(unsigned int cameraIndex)
{
	//currentCamera->Deactivate();
	ASSERT((cameraIndex >= 0) && (cameraIndex < cameras.size()));
	if ( (cameraIndex < 0) || (cameraIndex >= cameras.size()) )
	{
		stdlog(Error, LOGPLACE, "Incorrect current camera index. Passed %d when the correct range is (%d, %d).", cameraIndex, 0, cameras.size());
		stdlog(Notice, LOGPLACE, "Setting current camera index to 1");
		this->currentCameraIndex = 0;
	}
	else
	{
		this->currentCameraIndex = cameraIndex;
	}
	stdlog(Notice, LOGPLACE, "Switched to camera #%d", this->currentCameraIndex + 1);
	stdlog(Debug, LOGPLACE, "%s", cameras[this->currentCameraIndex]->ToString().c_str());
	//cameras[this->currentCameraIndex]->Activate();
	return this->currentCameraIndex;
}

inline void Renderer::AddLight(BaseLight* light)
{
	lights.push_back(light);
}

inline void Renderer::AddCamera(Camera* camera)
{
	cameras.push_back(camera);
}

std::string Renderer::GetOpenGLVersion()
{
	// TODO: Fix this function
	//const GLubyte* strVersion = glGetString(GL_VERSION);
	//return std::string((char*)strVersion);
	return "OpenGL version: ";
}

bool Renderer::IsCloseRequested() const
{
	return glfwWindowShouldClose(window);
}

void Renderer::UpdateUniformStruct(const Transform& transform, const Material& material, Shader* shader, const std::string& uniformName, const std::string& uniformType)
{
	//throw uniformType + " is not supported by the rendering engine";
	stdlog(Error, LOGPLACE, "Uniform name \"%s\" of type \"%s\" is not supported by the rendering engine", uniformName, uniformType);
}

void Renderer::BindAsRenderTarget()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}