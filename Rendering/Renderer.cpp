#include "StdAfx.h"
#include "Renderer.h"
#include "Game.h"
#include "Utility\Config.h"
#include "Utility\Log.h"
#include "Utility\FileNotFoundException.h"

// Include GLM- a library for 3D mathematics
//#include <glm/glm.hpp>

using namespace Rendering;
using namespace Utility;

Renderer::Renderer(int width, int height, std::string title) :
	window(NULL),
	vao(0),
	isFullscreen(false),
	isMouseEnabled(true)
{
	stdlog(Debug, LOGPLACE, "Creating Renderer instance started");
	stdlog(Notice, LOGPLACE, "OpenGL version = %s", GetOpenGLVersion().c_str());
	Init(width, height, title);
	stdlog(Delocust, LOGPLACE, "Creating Renderer instance finished");
}


Renderer::~Renderer(void)
{
	stdlog(Debug, LOGPLACE, "Destroying Renderer instance");
	
	glDeleteVertexArrays(1, &vao);

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
	glClearColor(Config::Get("ClearColorRed", 0.0f), Config::Get("ClearColorGreen", 0.0f), Config::Get("ClearColorBlue", 0.0f), Config::Get("ClearColorAlpha", 0.0f));

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

void Renderer::Render(GameNode& gameNode, Shader* shader)
{
	// TODO: Expand with Stencil buffer once it is used
	ClearScreen();

	gameNode.Render(shader);
}

void Renderer::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Renderer::ClearScreen() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::string Renderer::GetOpenGLVersion()
{
	// TODO: Fix this function
	//glGetString(GL_VERSION);
	return "OpenGL version: ";
}