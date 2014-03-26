#include "StdAfx.h"
#include "Renderer.h"
#include "Game.h"
#include "BasicShader.h"
#include "PhongShader.h"
#include "Utility\Config.h"
#include "Utility\Log.h"
#include "Utility\FileNotFoundException.h"

#include <sstream>

// Include GLM- a library for 3D mathematics
//#include <glm/glm.hpp>

using namespace Rendering;
using namespace Utility;
using namespace Math;

/* static */ const int Renderer::MAX_NUMBER_OF_CAMERAS = 10;

Renderer::Renderer(int width, int height, std::string title, unsigned short camerasCount /* = 3 */) :
	camerasCount(camerasCount),
	currentCameraIndex(0),
	cameras(NULL),
	window(NULL),
	vao(0),
	isFullscreen(false),
	isMouseEnabled(true),
	ambientLight(0.1f, 0.1f, 0.1f)
{
	stdlog(Debug, LOGPLACE, "Creating Renderer instance started");
	stdlog(Notice, LOGPLACE, "OpenGL version = %s", GetOpenGLVersion().c_str());
	
	Init(width, height, title);

	ASSERT(camerasCount > 0);
	if (camerasCount > MAX_NUMBER_OF_CAMERAS)
	{
		stdlog(Warning, LOGPLACE, "Requested too many cameras (%d, when %d is the maximum).", camerasCount, MAX_NUMBER_OF_CAMERAS);
		camerasCount = MAX_NUMBER_OF_CAMERAS;
	}

	cameras = new Camera [camerasCount];
	std::string tempStr = "camera";
	for (int i = 0; i < camerasCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		Real xPos = Config::Get("cameraPos_x_" + ss.str(), Camera::defaultCamera.GetPos().GetX());
		Real yPos = Config::Get("cameraPos_y_" + ss.str(), Camera::defaultCamera.GetPos().GetY());
		Real zPos = Config::Get("cameraPos_z_" + ss.str(), Camera::defaultCamera.GetPos().GetZ());
		Real xForward = Config::Get("cameraForward_x_" + ss.str(), Camera::defaultCamera.GetForward().GetX());
		Real yForward = Config::Get("cameraForward_y_" + ss.str(), Camera::defaultCamera.GetForward().GetY());
		Real zForward = Config::Get("cameraForward_z_" + ss.str(), Camera::defaultCamera.GetForward().GetZ());
		Real xUp = Config::Get("cameraUp_x_" + ss.str(), Camera::defaultCamera.GetUp().GetX());
		Real yUp = Config::Get("cameraUp_y_" + ss.str(), Camera::defaultCamera.GetUp().GetY());
		Real zUp = Config::Get("cameraUp_z_" + ss.str(), Camera::defaultCamera.GetUp().GetZ());
		Vector3D cameraPos = Math::Vector3D(xPos, yPos, zPos);
		Vector3D cameraForward = Math::Vector3D(xForward, yForward, zForward);
		Vector3D cameraUp = Math::Vector3D(xUp, yUp, zUp);

		Real FoV = Config::Get("cameraFoV_" + ss.str(), Camera::defaultFoV);
		Real aspectRatio = Config::Get("cameraAspectRatio_" + ss.str(), Camera::defaultAspectRatio);
		Real zNearPlane = Config::Get("cameraNearPlane_" + ss.str(), Camera::defaultNearPlane);
		Real zFarPlane = Config::Get("cameraFarPlane_" + ss.str(), Camera::defaultFarPlane);
		cameras[i] = Camera(cameraPos, cameraForward, cameraUp, FoV, aspectRatio, zNearPlane, zFarPlane);
	}

	stdlog(Delocust, LOGPLACE, "Creating Renderer instance finished");
}


Renderer::~Renderer(void)
{
	stdlog(Debug, LOGPLACE, "Destroying Renderer instance");
	
	glDeleteVertexArrays(1, &vao);

	if (cameras != NULL)
	{
		delete [] cameras;
		cameras = NULL;
	}

	glfwTerminate();
}

void Renderer::Init(int width, int height, std::string title)
{
	stdlog(Debug, LOGPLACE, "Initializing Renderer started");
	// Initialise GLFW
	if( !glfwInit() )
	{
		stdlog(Critical, LOGPLACE, "Failed to initalize GLFW");
		// TODO: throw another exception in the future
		throw FileNotFoundException();
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
	if (window == NULL)
	{
		stdlog(Critical, LOGPLACE, "Failed to open GLFW window.  If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowCloseCallback(window, &Game::WindowCloseCallback);
	//glfwSetWindowSizeCallback(window, Game::WindowResizeCallback);
	glfwSetKeyCallback(window, &Game::KeyEventCallback);
	//glfwSetCharCallback(window, Game::CharEventCallback);
	//glfwSetMousePosCallback(window, Game::MouseMotionCallback);
	//glfwSetMouseButtonCallback(window, Game::MouseInputCallback);

	glfwMakeContextCurrent(window);

	InitGlew();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, width / 2, height / 2);

	//glfwSwapInterval(1);
	glfwSetTime(0.0);

	InitGraphics();
}

void Renderer::InitGraphics()
{
	glClearColor(GET_CONFIG_VALUE("ClearColorRed", "ClearColorRedDefault", 0.0f), GET_CONFIG_VALUE("ClearColorGreen", "ClearColorGreenDefault", 0.0f), GET_CONFIG_VALUE("ClearColorBlue", "ClearColorBlueDefault", 0.0f), GET_CONFIG_VALUE("ClearColorAlpha", "ClearColorAlphaDefault", 0.0f));

	//glFrontFace(GL_CW); // everything face I draw in clockwise order is a front face
	//glCullFace(GL_BACK); // cull the back face
	//glEnable(GL_CULL_FACE); // culling faces enabled. Cull triangles which normal is not towards the camera
	glEnable(GL_DEPTH_TEST); // to enable depth tests
	glEnable(GL_DEPTH_CLAMP); // prevents the camera to clip through the mesh

	glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one

	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_FRAMEBUFFER_SRGB); // Essentialy gives free gamma correction for better contrast. TODO: Test it!

	ASSERT(!vao);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
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

	//if (GLEW_VERSION_2_0) {
	//	printf("OpenGL 2.0 supported\n");
	//} else {
	//	printf("OpenGL 2.0 NOT supported\n");
	//	exit(0);
	//}

	stdlog(Notice, LOGPLACE, "Using GLEW version %s", glewGetString(GLEW_VERSION));
}

void Renderer::Render(GameNode& gameNode)
{
	// TODO: Expand with Stencil buffer once it is used
	ClearScreen();

	// Ambient rendering
	//gameNode.Render(ForwardAmbient::GetInstance(), this);

	gameNode.Render(BasicShader::GetInstance(), this);
}

void Renderer::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Renderer::ClearScreen() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::NextCamera()
{
	SetCurrentCameraIndex((currentCameraIndex + 1) % camerasCount);
}

void Renderer::PrevCamera()
{
	SetCurrentCameraIndex((currentCameraIndex == 0) ? camerasCount - 1: currentCameraIndex - 1);
}

void Renderer::SetCurrentCameraIndex(int cameraIndex)
{
	ASSERT((cameraIndex >= 0) && (cameraIndex < camerasCount));
	if ( (cameraIndex < 0) || (cameraIndex >= camerasCount) )
	{
		stdlog(Error, LOGPLACE, "Incorrect camera index given. Camera index stays untouched.");
		return;
	}
	currentCameraIndex = cameraIndex;
	stdlog(Notice, LOGPLACE, "Switched to camera #%d", currentCameraIndex + 1);
	stdlog(Debug, LOGPLACE, "%s", cameras[currentCameraIndex].ToString().c_str());
}

std::string Renderer::GetOpenGLVersion()
{
	// TODO: Fix this function
	//glGetString(GL_VERSION);
	return "OpenGL version: ";
}