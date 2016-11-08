#include "Def.h"

#include "Rendering\CameraBuilder.h"
#include "Rendering\LightBuilder.h"
#include "Rendering\ShaderFactory.h"
#include "Rendering\ParticlesSystemBuilder.h"
#include "Rendering\Renderer.h"

#include "Math\StatisticsStorage.h"
#include "Math\RandomGeneratorFactory.h"

#include "Utility\ICommandLineMapper.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"
#include "Utility\Time.h"

#include <GLFW\glfw3.h>
#include <ctime>
#include <string>
//#include <thread>
//#include <xmmintrin.h>
//#include <iostream>
//#include <fstream>

using namespace Rendering;
using namespace Math;
using namespace Utility;
using namespace std;

const std::string MODULE_NAME = "MathTest";
const Math::Random::RandomGenerator& g_randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE);
GLFWwindow* window = nullptr;
GLFWwindow* threadWindow = nullptr;
std::unique_ptr<Renderer> renderer = nullptr;

const std::string modelsDirectory = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Models\\";
const std::string shadersDirectory = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Shaders\\";
const std::string texturesDirectory = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Textures\\";
unsigned int testNumber = 0;
bool meshTestEnabled = true;
bool textureTestEnabled = true;
bool cameraBuilderTestEnabled = true;
bool lightBuilderTestEnabled = true;
bool particlesSystemBuilderTestEnabled = true;
bool otherTestsEnabled = true;

void WindowCloseEventCallback(GLFWwindow* window)
{
	NOTICE_LOG_RENDERING_TEST("Window close event callback");
	glfwSetWindowShouldClose(window, GL_TRUE);
}

void WindowResizeEvent(GLFWwindow* window, int width, int height)
{
	renderer->SetWindowWidth(width);
	renderer->SetWindowHeight(height);
}

void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwWindowSize(width, height))
	{
		WindowResizeEvent(window, width, height);
	}
#else
	WindowResizeEvent(window, width, height);
#endif
}

void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	INFO_LOG_RENDERING_TEST("Key event callback (key = ", key, "; scancode = ", scancode, "; action = ", action, "; mods = ", mods);
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventKeyGLFW(key, action))  // send event to AntTweakBar
	{
		// event has not been handled by AntTweakBar
		KeyEvent(window, key, scancode, action, mods);
	}
#else
	KeyEvent(window, key, scancode, action, mods);
#endif
}

void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
}

void MouseEventCallback(GLFWwindow* window, int button, int action, int mods)
{
	INFO_LOG_RENDERING_TEST("Mouse event callback");
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventMouseButtonGLFW(button, action))
	{
		// event has not been handled by AntTweakBar
		MouseButtonEvent(window, button, action, mods);
	}
#else
	MouseButtonEvent(window, button, action, mods);
#endif
}

void MousePosEvent(GLFWwindow* window, double xPos, double yPos)
{
}

void MousePosCallback(GLFWwindow* window, double xPos, double yPos)
{
	DEBUG_LOG_RENDERING_TEST("Mouse event callback");
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventMousePosGLFW(static_cast<int>(xPos), static_cast<int>(yPos)))
	{
		// event has not been handled by AntTweakBar
		MousePosEvent(window, xPos, yPos);
	}
#else
	MousePosEvent(window, xPos, yPos);
#endif
	//GetCoreEngine()->CentralizeCursor();
}

void ScrollEvent(GLFWwindow* window, double xOffset, double yOffset)
{

}

void ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	INFO_LOG_RENDERING_TEST("Scroll event callback");
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventMouseWheelGLFW(static_cast<int>(yOffset))) // TODO: Check if yOffset here is ok
	{
		// event has not been handled by AntTweakBar
		ScrollEvent(window, xOffset, yOffset);
	}
#else
	ScrollEvent(window, xOffset, yOffset);
#endif
}

void ErrorCallback(int errorCode, const char* description)
{
	switch (errorCode)
	{
	case GLFW_NOT_INITIALIZED:
		ERROR_LOG_RENDERING_TEST("GLFW has not been initialized. Error description: ", description);
		break;
	case GLFW_NO_CURRENT_CONTEXT:
		ERROR_LOG_RENDERING_TEST("No context is current for this thread. Error description: ", description);
		break;
	case GLFW_INVALID_ENUM:
		ERROR_LOG_RENDERING_TEST("One of the arguments to the function was an invalid enum value. Error description: ", description);
		break;
	case GLFW_INVALID_VALUE:
		ERROR_LOG_RENDERING_TEST("One of the arguments to the function was an invalid value. Error description: ", description);
		break;
	case GLFW_OUT_OF_MEMORY:
		ERROR_LOG_RENDERING_TEST("A memory allocation failed. Error description: ", description);
		break;
	case GLFW_API_UNAVAILABLE:
		ERROR_LOG_RENDERING_TEST("GLFW could not find support for the requested client API on the system. Error description: ", description);
		break;
	case GLFW_VERSION_UNAVAILABLE:
		ERROR_LOG_RENDERING_TEST("The requested OpenGL or OpenGL ES version is not available. Error description: ", description);
		break;
	case GLFW_PLATFORM_ERROR:
		ERROR_LOG_RENDERING_TEST("A platform-specific error occurred that does not match any of the more specific categories. Error description: ", description);
		break;
	case GLFW_FORMAT_UNAVAILABLE:
		ERROR_LOG_RENDERING_TEST("The requested format is not supported or available. Error description: ", description);
		break;
	default:
		ERROR_LOG_RENDERING_TEST("Unknown GLFW error event occurred with code ", errorCode, " and message: Error description: ", description);
	}
	exit(EXIT_FAILURE);
}

void InitGlfw(bool fullscreenEnabled, int width, int height, const std::string& title, Rendering::Aliasing::AntiAliasingMethod antiAliasingMethod)
{
	DEBUG_LOG_RENDERING_TEST("Initializing GLFW started");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(glfwInit(), Utility::Logging::CRITICAL, "Failed to initialize GLFW.");

	const int antiAliasingSamples = 4; // TODO: This parameter belongs in the Rendering module. The config value should also be retrieved from the rendering configuration file.
	switch (antiAliasingMethod)
	{
	case Rendering::Aliasing::NONE:
		/**
		* TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		* Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		*/
		glfwWindowHint(GLFW_SAMPLES, 0);
		INFO_LOG_RENDERING_TEST("No anti-aliasing algorithm chosen");
		break;
	case Rendering::Aliasing::FXAA:
		/**
		* TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		* Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		*/
		glfwWindowHint(GLFW_SAMPLES, 0);
		INFO_LOG_RENDERING_TEST("FXAA anti-aliasing algorithm chosen");
		break;
	case Rendering::Aliasing::MSAA:
		glfwWindowHint(GLFW_SAMPLES, antiAliasingSamples);
		INFO_LOG_RENDERING_TEST(antiAliasingSamples, "xMSAA anti-aliasing algorithm chosen");
		break;
	default:
		WARNING_LOG_RENDERING_TEST("Unknown anti-aliasing algorithm chosen. Default ", antiAliasingSamples, "xMSAA algorithm chosen");
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
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GL_TRUE);

	GLFWmonitor* monitor = NULL;
	if (fullscreenEnabled)
	{
		monitor = glfwGetPrimaryMonitor();
	}
	window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL); // Open a window and create its OpenGL context
	if (window == NULL)
	{
		CRITICAL_LOG_RENDERING_TEST("Failed to create GLFW main window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	threadWindow = glfwCreateWindow(1, 1, "Thread Window", NULL, window);
	if (threadWindow == NULL)
	{
		CRITICAL_LOG_RENDERING_TEST("Failed to create GLFW thread window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we can capture the escape key being pressed below
	glfwSetCursorPos(window, width / 2, height / 2); // Set cursor position to the middle point
													 //glfwSwapInterval(1);
	glfwSetTime(REAL_ZERO);
	DEBUG_LOG_RENDERING_TEST("Initializing GLFW finished successfully");
}

void InitGlew()
{
	INFO_LOG_RENDERING_TEST("Initializing GLEW started");
	glewExperimental = true; // Needed in core profile
	GLenum err = glewInit();

	CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(GLEW_OK == err, Utility::Logging::EMERGENCY, "Error while initializing GLEW: ", glewGetErrorString(err));
	if (GLEW_VERSION_2_0)
	{
		DEBUG_LOG_RENDERING_TEST("OpenGL 2.0 supported");
	}
	else
	{
		ERROR_LOG_RENDERING_TEST("Initializing GLEW failed. OpenGL 2.0 NOT supported");
		exit(EXIT_FAILURE);
	}

	INFO_LOG_RENDERING_TEST("Using GLEW version ", glewGetString(GLEW_VERSION));
	//CheckErrorCode(__FUNCTION__, "Initializing GLEW");
}

void SetCallbacks()
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(window != NULL, Utility::Logging::CRITICAL, "Setting GLFW callbacks failed. The window is NULL.");
	glfwSetWindowCloseCallback(window, &WindowCloseEventCallback);
	glfwSetWindowSizeCallback(window, &WindowResizeCallback);
	glfwSetKeyCallback(window, &KeyEventCallback);
	//glfwSetCharCallback(window, &CharEventCallback);
	//glfwSetMousePosCallback(window, &MouseMotionCallback);
	glfwSetCursorPosCallback(window, &MousePosCallback);
	glfwSetMouseButtonCallback(window, &MouseEventCallback);
	glfwSetScrollCallback(window, &ScrollEventCallback);
}

void CreateRenderer(bool fullscreenEnabled, int width, int height, const std::string& title, Rendering::Aliasing::AntiAliasingMethod antiAliasingMethod)
{
	InitGlfw(fullscreenEnabled, width, height, title, antiAliasingMethod);
	InitGlew();
	SetCallbacks();

	Rendering::InitGraphics(width, height, antiAliasingMethod);

	glfwSetErrorCallback(&ErrorCallback);
	//DEBUG_LOG_ENGINE("Thread window address: ", threadWindow);
	NOTICE_LOG_RENDERING_TEST("Creating Renderer instance started");
	renderer = std::make_unique<Rendering::Renderer>(width, height, modelsDirectory, texturesDirectory, antiAliasingMethod);
	NOTICE_LOG_RENDERING_TEST("Creating Renderer instance finished");

	CHECK_CONDITION_EXIT_RENDERING_TEST(renderer != NULL, Utility::Logging::CRITICAL, "Failed to create a renderer.");
}

void ReportError(const std::string& reportStr)
{
	ERROR_LOG_RENDERING_TEST("Test #", testNumber, " FAILED. ", reportStr);
}

void TestReport(bool statusCode /* false if error */, const std::string& reportErrorStr)
{
	testNumber++;
	if (statusCode == false)
	{
		ReportError(reportErrorStr);
	}
	INFO_LOG_RENDERING_TEST("Test #", testNumber, " passed");
}

void TimeReport(const std::string& reportStr, Timing::Timer& timer, Timing::TimeUnit timeUnit, const unsigned int NUMBER_OF_ITERATIONS = 1)
{
	//CHECK_CONDITION_EXIT_ALWAYS_MATH_TEST(!timer.IsRunning(), Logging::ERR, "Timer is still running");
	double elapsedTime = static_cast<double>(timer.GetDuration(timeUnit));
	elapsedTime /= NUMBER_OF_ITERATIONS;
	//timeSpan.AdjustUnitToValue();
	NOTICE_LOG_RENDERING_TEST(reportStr, ":\t", elapsedTime, " ", Timing::DateTime::ConvertTimeUnitToString(timeUnit));
}

void CameraBuilderTest()
{
	if (!cameraBuilderTestEnabled)
	{
		return;
	}

	OrthoCameraBuilder orthoCameraBuilder;
	BuilderDirector<Camera> cameraBuilderDirector(&orthoCameraBuilder);
	Rendering::Camera camera = cameraBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(camera);
	camera = orthoCameraBuilder.SetPos(4.0f, 4.0f, 0.0f).SetNearPlane(0.1f).SetFarPlane(10000.0f).Get();
	NOTICE_LOG_RENDERING_TEST(camera);

	PerspectiveCameraBuilder persepectiveCameraBuilder;
	persepectiveCameraBuilder.SetPos(3.0f, 15.0f, -3.0f);
	cameraBuilderDirector.SetBuilder(&persepectiveCameraBuilder);
	camera = cameraBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(camera);
}

void LightBuilderTest()
{
	if (!lightBuilderTestEnabled)
	{
		return;
	}

	ShaderFactory shaderFactory(shadersDirectory);

	DirectionalLightBuilder directionalLightBuilder(shaderFactory);
	BuilderDirector<Lighting::DirectionalLight> directionalLightBuilderDirector(&directionalLightBuilder);
}

void ParticlesSystemBuilderTest()
{
	if (!particlesSystemBuilderTestEnabled)
	{
		return;
	}

	Particles::ParticlesSystemBuilder particlesSystemBuilder;
	BuilderDirector<Particles::ParticlesSystem> particlesSystemBuilderDirector(&particlesSystemBuilder);
	Particles::ParticlesSystem particlesSystem = particlesSystemBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(particlesSystem);

	particlesSystemBuilder.SetMaxCount(10).SetAttributesMask(Particles::Attributes::POSITION | Particles::Attributes::COLOR).SetTextureID(TextureIDs::INVALID).SetShaderID(ShaderIDs::PARTICLES_COLORS);
	particlesSystem = particlesSystemBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(particlesSystem);
}

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	std::unique_ptr<ICommandLineMapper> commandLineMapper = ICommandLineMapper::CreateCommandLineMapper(argc, argv);
	//if (commandLineMapper.IsPresent("-help"))
	//{
	//	PrintHelp();
	//	system("pause");
	//	return 0;
	//}
	Logging::ILogger::GetLogger(MODULE_NAME).Fill(commandLineMapper->Get("-log", ""), Logging::INFO);
	Logging::ILogger::GetLogger("Rendering").Fill(commandLineMapper->Get("-logRendering", "Info"), Logging::INFO);
	Logging::ILogger::GetLogger("Math").Fill(commandLineMapper->Get("-logMath", "Info"), Logging::INFO);
	Logging::ILogger::GetLogger("Utility").Fill(commandLineMapper->Get("-logUtility", "Info"), Logging::INFO);

	IConfig::CreateConfig("Rendering", commandLineMapper->Get("-configRendering", "..\\..\\Config\\ConfigRendering.cfg"));

	STATS_STORAGE.StartTimer();

	CreateRenderer(false, 1600, 900, "3D rendering tests", Rendering::Aliasing::NONE);

	//MeshTest();
	//TextureTest();
	CameraBuilderTest();
	LightBuilderTest();
	ParticlesSystemBuilderTest();
	//OtherTests();

	STATS_STORAGE.StopTimer();
	STATS_STORAGE.PrintReport();

	Logging::ILogger::GetLogger(MODULE_NAME).ResetConsoleColor();
	std::cout << "Bye!" << std::endl;

	return 0;
}
