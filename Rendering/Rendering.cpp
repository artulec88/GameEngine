#include "stdafx.h"
#include "Rendering.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

Rendering::AntiAliasingMethod Rendering::antiAliasingMethod = Rendering::AntiAliasingMethod::FXAA;

GLFWwindow* Rendering::InitGraphics(int width, int height, const std::string& title)
{
	LOG(Utility::Info, LOGPLACE, "Initializing graphics started");
	GLFWwindow* window = InitGlfw(width, height, title);
	InitGlew();

	glFrontFace(GL_CW); // every face I draw in clockwise order is a front face
	glCullFace(GL_BACK); // cull the back face
	glEnable(GL_CULL_FACE); // culling faces enabled. Cull triangles which normal is not towards the camera
	glEnable(GL_DEPTH_TEST); // to enable depth tests
	glEnable(GL_DEPTH_CLAMP); // prevents the camera to clip through the mesh

	switch (Rendering::antiAliasingMethod)
	{
	case NONE:
		glDisable(GL_MULTISAMPLE); // disable multisampling
		break;
	case FXAA:
		glDisable(GL_MULTISAMPLE); // disable multisampling
		break;
	case MSAA:
		glEnable(GL_MULTISAMPLE);
		break;
	default:
		LOG(Utility::Warning, LOGPLACE, "Unknown anti-aliasing algorithm chosen");
		glEnable(GL_MULTISAMPLE);
	}

	//glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_FRAMEBUFFER_SRGB); // Essentialy gives free gamma correction for better contrast. TODO: Test it!

	LOG(Utility::Info, LOGPLACE, "Initializing graphics finished successfully");
	return window;
}

GLFWwindow* Rendering::InitGlfw(int width, int height, const std::string& title)
{
	LOG(Utility::Debug, LOGPLACE, "Initializing GLFW started");
	if( !glfwInit() )
	{
		LOG(Utility::Critical, LOGPLACE, "Failed to initalize GLFW");
		exit(EXIT_FAILURE);
		// throw FileNotFoundException(); // TODO: throw another exception in the future
	}
	int antiAliasingSamples = GET_CONFIG_VALUE("antiAliasingSamples", 4); /* 4x anti-aliasing by default */
	switch (Rendering::antiAliasingMethod)
	{
	case NONE:
		/**
		 * TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		 * Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		 */
		glfwWindowHint(GLFW_SAMPLES, 0);
		LOG(Utility::Notice, LOGPLACE, "No anti-aliasing algorithm chosen");
		break;
	case FXAA:
		/**
		 * TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		 * Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		 */
		glfwWindowHint(GLFW_SAMPLES, 0);
		LOG(Utility::Notice, LOGPLACE, "FXAA anti-aliasing algorithm chosen");
		break;
	case MSAA:
		glfwWindowHint(GLFW_SAMPLES, antiAliasingSamples);
		LOG(Utility::Notice, LOGPLACE, "%dxMSAA anti-aliasing algorithm chosen", antiAliasingSamples);
		break;
	default:
		LOG(Utility::Warning, LOGPLACE, "Unknown anti-aliasing algorithm chosen. Default %dxMSAA algorithm chosen", antiAliasingSamples);
		glfwWindowHint(GLFW_SAMPLES, antiAliasingSamples);
	}
	glfwWindowHint(GLFW_VERSION_MAJOR, 3); // TODO: Do not hard-code any values
	glfwWindowHint(GLFW_VERSION_MINOR, 3); // TODO: Do not hard-code any values
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // So that glBegin / glEnd etc. work
#else
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL /* glfwGetPrimaryMonitor()- for fullscreen */, NULL); // Open a window and create its OpenGL context
	if (window == NULL)
	{
		LOG(Utility::Critical, LOGPLACE, "Failed to open GLFW window.  If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we can capture the escape key being pressed below
	glfwSetCursorPos(window, width / 2, height / 2); // Set cursor position to the middle point
	//glfwSwapInterval(1);
	glfwSetTime(0.0);
	LOG(Utility::Debug, LOGPLACE, "Initializing GLFW finished successfully");
	return window;
}

void Rendering::InitGlew()
{
	LOG(Utility::Info, LOGPLACE, "Initializing GLEW started");
	glewExperimental = true; // Needed in core profile
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		LOG(Utility::Error, LOGPLACE, "Error while initializing GLEW: %s", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	if (GLEW_VERSION_2_0)
	{
		LOG(Utility::Info, LOGPLACE, "OpenGL 2.0 supported");
	}
	else
	{
		LOG(Utility::Info, LOGPLACE, "OpenGL 2.0 NOT supported");
		exit(EXIT_FAILURE);
	}

	LOG(Utility::Notice, LOGPLACE, "Using GLEW version %s", glewGetString(GLEW_VERSION));
}

/**
 * See the link: https://www.opengl.org/wiki/GLAPI/glGetError
 */
void Rendering::CheckErrorCode(const char* functionName, const char* comment)
{
	GLenum errCode = glGetError();
	if (errCode == GL_NO_ERROR)
	{
		return;
	}
	switch (errCode)
	{
	case GL_INVALID_ENUM:
		LOG(Utility::Error, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_INVALID_ENUM", functionName, comment);
		break;
	case GL_INVALID_VALUE:
		LOG(Utility::Error, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_INVALID_VALUE", functionName, comment);
		break;
	case GL_INVALID_OPERATION:
		LOG(Utility::Error, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_INVALID_OPERATION", functionName, comment);
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		LOG(Utility::Error, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_INVALID_FRAMEBUFFER_OPERATION", functionName, comment);
		break;
	case GL_OUT_OF_MEMORY:
		LOG(Utility::Critical, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_OUT_OF_MEMORY", functionName, comment);
		exit(EXIT_FAILURE);
		break;
	case GL_STACK_UNDERFLOW:
		LOG(Utility::Critical, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_STACK_UNDERFLOW", functionName, comment);
		exit(EXIT_FAILURE);
		break;
	case GL_STACK_OVERFLOW:
		LOG(Utility::Critical, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_STACK_OVERFLOW", functionName, comment);
		exit(EXIT_FAILURE);
		break;
	default:
		LOG(Utility::Critical, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = %d", functionName, comment, errCode);
		exit(EXIT_FAILURE);
	}
}