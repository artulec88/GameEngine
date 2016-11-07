#include "StdAfx.h"
#include "CoreEngine.h"

#include "Audio\AudioEngineFactory.h"

#include "Math\Math.h"
#include "Math\FloatingPoint.h"

#include "Utility\ILogger.h"
#include "Utility\Config.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <thread>
#include <GLFW\glfw3.h>

/**
 * See http://stackoverflow.com/questions/546997/use-ifdefs-and-define-to-optionally-turn-a-function-call-into-a-comment
 */
#ifndef PROFILING_RENDERING_MODULE_ENABLED
#undef START_TIMER
#undef RESET_TIMER
#undef STOP_TIMER
#define START_TIMER(timerID)
#define RESET_TIMER(timerID)
#define STOP_TIMER(timerID, countStats, minMaxTime, timeSum)
#endif

int numberOfAllocs1 = 0; int numberOfDeallocs1 = 0;
int numberOfAllocs2 = 0; int numberOfDeallocs2 = 0;
int numberOfAllocs3 = 0; int numberOfDeallocs3 = 0;
int numberOfAllocs4 = 0; int numberOfDeallocs4 = 0;

void* operator new(std::size_t size) throw(std::bad_alloc)
{
	++numberOfAllocs1;
void* p = malloc(size);
if (!p)
{
	throw std::bad_alloc();
}
return p;
}

void* operator new[](std::size_t size) throw(std::bad_alloc)
{
	++numberOfAllocs2;
	void *p = malloc(size);
	if (!p)
	{
		throw std::bad_alloc();
	}
	return p;
}

void* operator new[](std::size_t size, const std::nothrow_t&) throw()
{
	++numberOfAllocs3;
	return malloc(size);
}

void* operator new(std::size_t size, const std::nothrow_t&) throw()
{
	++numberOfAllocs4;
return malloc(size);
}


void operator delete(void* ptr) throw() { ++numberOfDeallocs1; free(ptr); }
void operator delete(void* ptr, const std::nothrow_t&) throw() { ++numberOfDeallocs2; free(ptr); }
void operator delete[](void* ptr) throw() { ++numberOfDeallocs3; free(ptr); }
void operator delete[](void* ptr, const std::nothrow_t&) throw() { ++numberOfDeallocs4; free(ptr); }


Engine::CoreEngine* Engine::CoreEngine::s_coreEngine = nullptr;

/* static */ Engine::CoreEngine* Engine::CoreEngine::GetCoreEngine()
{
	return s_coreEngine;
}

/* static */ void Engine::CoreEngine::ErrorCallback(int errorCode, const char* description)
{
	GetCoreEngine()->ErrorCallbackEvent(errorCode, description);
}

/* static */ void Engine::CoreEngine::WindowCloseEventCallback(GLFWwindow* window)
{
	GetCoreEngine()->CloseWindowEvent(window);
}

/* static */ void Engine::CoreEngine::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwWindowSize(width, height))
	{
		GetCoreEngine()->WindowResizeEvent(window, width, height);
	}
#else
	GetCoreEngine()->WindowResizeEvent(window, width, height);
#endif
}

/* static */ void Engine::CoreEngine::KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	DEBUG_LOG_ENGINE("Key event callback (key = ", key, "; scancode = ", scancode, "; action = ", action, "; mods = ", mods);
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventKeyGLFW(key, action))  // send event to AntTweakBar
	{
		// event has not been handled by AntTweakBar
		GetCoreEngine()->KeyEvent(window, key, scancode, action, mods);
	}
#else
	GetCoreEngine()->KeyEvent(window, key, scancode, action, mods);
#endif
}

/* static */ void Engine::CoreEngine::MouseEventCallback(GLFWwindow* window, int button, int action, int mods)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventMouseButtonGLFW(button, action))
	{
		// event has not been handled by AntTweakBar
		GetCoreEngine()->MouseButtonEvent(window, button, action, mods);
	}
#else
	GetCoreEngine()->MouseButtonEvent(window, button, action, mods);
#endif
}

/* static */ void Engine::CoreEngine::MousePosCallback(GLFWwindow* window, double xPos, double yPos)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventMousePosGLFW(static_cast<int>(xPos), static_cast<int>(yPos)))
	{
		// event has not been handled by AntTweakBar
		GetCoreEngine()->MousePosEvent(window, xPos, yPos);
	}
#else
	GetCoreEngine()->MousePosEvent(window, xPos, yPos);
#endif
	//GetCoreEngine()->CentralizeCursor();
}

/* static */ void Engine::CoreEngine::ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventMouseWheelGLFW(static_cast<int>(yOffset))) // TODO: Check if yOffset here is ok
	{
		// event has not been handled by AntTweakBar
		GetCoreEngine()->ScrollEvent(window, xOffset, yOffset);
	}
#else
	GetCoreEngine()->ScrollEvent(window, xOffset, yOffset);
#endif
}

Engine::CoreEngine::CoreEngine(bool fullscreenEnabled, int width, int height, const char* title, const std::string& shadersDirectory /* = "..\\Shaders\\" */,
	const std::string& modelsDirectory /* = "..\\Models\\" */, const std::string& texturesDirectory /* = "..\\Textures\\" */,
	const std::string& fontsDirectory /* = "..\\Fonts\\" */, const std::string& audioDirectory /* = "..\\Sounds\\" */) :
	m_window(NULL),
	m_threadWindow(NULL),
	m_isRunning(false),
	m_windowWidth(width),
	m_windowHeight(height),
	m_windowTitle(title),
	m_frameTime(1.0f / GET_CONFIG_VALUE_ENGINE("maxFPS", 500.0f)),
	m_game(NULL),
	m_audioEngine(nullptr),
	m_physicsEngine(NULL),
	m_renderer(nullptr),
	m_shadersDirectory(shadersDirectory),
	m_modelsDirectory(modelsDirectory),
	m_texturesDirectory(texturesDirectory),
	m_fontsDirectory(fontsDirectory),
	m_audioDirectory(audioDirectory),
	m_glfwKeysToRawInputKeysMap({ {GLFW_KEY_A, Input::RawInputKeys::KEY_A}, { GLFW_KEY_B, Input::RawInputKeys::KEY_B },
		{ GLFW_KEY_C, Input::RawInputKeys::KEY_C }, { GLFW_KEY_D, Input::RawInputKeys::KEY_D }, { GLFW_KEY_E, Input::RawInputKeys::KEY_E }, { GLFW_KEY_F, Input::RawInputKeys::KEY_F },
		{ GLFW_KEY_G, Input::RawInputKeys::KEY_G }, { GLFW_KEY_H, Input::RawInputKeys::KEY_H }, { GLFW_KEY_I, Input::RawInputKeys::KEY_I }, { GLFW_KEY_J, Input::RawInputKeys::KEY_J },
		{ GLFW_KEY_K, Input::RawInputKeys::KEY_K }, { GLFW_KEY_L, Input::RawInputKeys::KEY_L }, { GLFW_KEY_M, Input::RawInputKeys::KEY_M }, { GLFW_KEY_N, Input::RawInputKeys::KEY_N },
		{ GLFW_KEY_O, Input::RawInputKeys::KEY_O }, { GLFW_KEY_P, Input::RawInputKeys::KEY_P }, { GLFW_KEY_Q, Input::RawInputKeys::KEY_Q }, { GLFW_KEY_R, Input::RawInputKeys::KEY_R },
		{ GLFW_KEY_S, Input::RawInputKeys::KEY_S }, { GLFW_KEY_T, Input::RawInputKeys::KEY_T }, { GLFW_KEY_U, Input::RawInputKeys::KEY_U }, { GLFW_KEY_V, Input::RawInputKeys::KEY_V },
		{ GLFW_KEY_W, Input::RawInputKeys::KEY_W }, { GLFW_KEY_X, Input::RawInputKeys::KEY_X }, { GLFW_KEY_Y, Input::RawInputKeys::KEY_Y }, { GLFW_KEY_Z, Input::RawInputKeys::KEY_Z },
		{ GLFW_KEY_UP, Input::RawInputKeys::KEY_UP }, { GLFW_KEY_DOWN, Input::RawInputKeys::KEY_DOWN }, { GLFW_KEY_ESCAPE, Input::RawInputKeys::KEY_ESCAPE }, { GLFW_KEY_ENTER, Input::RawInputKeys::KEY_ENTER },
		{ GLFW_MOUSE_BUTTON_LEFT, Input::RawInputKeys::MOUSE_KEY_LEFT }, { GLFW_MOUSE_BUTTON_MIDDLE, Input::RawInputKeys::MOUSE_KEY_MIDDLE }, { GLFW_MOUSE_BUTTON_RIGHT, Input::RawInputKeys::MOUSE_KEY_RIGHT },
		{ GLFW_KEY_SPACE, Input::RawInputKeys::KEY_SPACE }, { GLFW_KEY_LEFT_CONTROL, Input::RawInputKeys::KEY_LEFT_CONTROL }, { GLFW_KEY_LEFT_ALT, Input::RawInputKeys::KEY_LEFT_ALT },
		{ GLFW_KEY_LEFT_SHIFT, Input::RawInputKeys::KEY_LEFT_SHIFT },{ GLFW_KEY_RIGHT_CONTROL, Input::RawInputKeys::KEY_RIGHT_CONTROL },{ GLFW_KEY_RIGHT_ALT, Input::RawInputKeys::KEY_RIGHT_ALT },
		{ GLFW_KEY_RIGHT_SHIFT, Input::RawInputKeys::KEY_RIGHT_SHIFT } }),
	m_inputMapping("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Config\\" + GET_CONFIG_VALUE_STR_ENGINE("inputContextsListFileName", "ContextsList.txt"))
#ifdef PROFILING_ENGINE_MODULE_ENABLED
	, m_countStats1(0),
	m_timeSum1(REAL_ZERO),
	m_countStats2(0),
	m_timeSum2(REAL_ZERO),
	m_countStats2_1(0),
	m_timeSum2_1(REAL_ZERO),
	m_countStats2_2(0),
	m_timeSum2_2(REAL_ZERO),
	m_countStats2_3(0),
	m_timeSum2_3(REAL_ZERO),
	m_countStats3(0),
	m_timeSum3(REAL_ZERO),
	m_renderingRequiredCount(0),
	m_renderingNotRequiredCount(0),
	m_isSamplingSpf(true),
	m_classStats(STATS_STORAGE.GetClassStats("CoreEngine")),
	m_stats()
#endif
{
	START_PROFILING_ENGINE(true, "");
	NOTICE_LOG_ENGINE("Main application construction started");
	STATS_STORAGE.StartTimer();

	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(s_coreEngine == NULL, Utility::Logging::ERR, "Constructor called when a singleton instance of MainApp class has already been created");
	s_coreEngine = this;

	CreateAudioEngine();
	CreatePhysicsEngine();
	CreateRenderer(fullscreenEnabled, width, height, title, Rendering::Aliasing::NONE /* TODO: Get anti-aliasing method from Rendering configuration file. */);

	NOTICE_LOG_ENGINE("Main application construction finished");
	STOP_PROFILING_ENGINE("");
}


	Engine::CoreEngine::~CoreEngine(void)
{
	DEBUG_LOG_ENGINE("Core engine destruction started");

	/* ==================== Printing stats begin ==================== */
#ifdef PROFILING_RENDERING_MODULE_ENABLED
	INFO_LOG_ENGINE("The region #1 (Time calculating) was processed ", m_countStats1, " times, which took exactly", m_timeSum1, " [us]. The average time=", m_timeSum1 / m_countStats1, " [us]. ", m_minMaxTime1.ToString());
	INFO_LOG_ENGINE("The region #2 was processed ", m_countStats2, " times, which took exactly ", m_timeSum2, " [us]. The average time=", m_timeSum2 / m_countStats2, "[us]. ", m_minMaxTime2.ToString());
	INFO_LOG_ENGINE("\t The region #2_1 (Polling events) was processed ", m_countStats2_1, " times, which took exactly ", m_timeSum2_1, " [us]. The average time=", m_timeSum2_1 / m_countStats2_1, " [us]. ", m_minMaxTime2_1.ToString());
	INFO_LOG_ENGINE("\t The region #2_2 (Game input processing) was processed ", m_countStats2_2, " times, which took exactly ", m_timeSum2_2, " [us]. The average time=", m_timeSum2_2 / m_countStats2_2, " [us]. ", m_minMaxTime2_2.ToString());
	INFO_LOG_ENGINE("\t The region #2_3 (Game updating) was processed ", m_countStats2_3, " times, which took exactly ", m_timeSum2_3, " [us]. The average time=", m_timeSum2_3 / m_countStats2_3, " [us]. ", m_minMaxTime2_3.ToString());
	INFO_LOG_ENGINE("The region #3 (Rendering) was processed ", m_countStats3, " times, which took exactly ", m_timeSum3, " [us]. The average time=", m_timeSum3 / m_countStats3, " [us]. ", m_minMaxTime3.ToString());
	INFO_LOG_ENGINE("Rendering step performed ", m_renderingRequiredCount, " times.");
	INFO_LOG_ENGINE("Rendering step omitted ", m_renderingNotRequiredCount, " times.");

	STATS_STORAGE.StopTimer();
	STATS_STORAGE.PrintReport();

	//Math::Real minSpf, maxSpf, stdDevSpf;
	Math::Real meanSpf = m_stats.CalculateMean(Math::Statistics::SPF);
	Math::Real medianSpf = m_stats.CalculateMedian(Math::Statistics::SPF);
	INFO_LOG_ENGINE("SPF (Seconds Per Frame) statistics during gameplay:\nSamples =\t", m_stats.Size(), "\nAverage SPF =\t", meanSpf, " [ms]\nMedian SPF =\t", medianSpf, " [ms]");
	//INFO_LOG_ENGINE("SPF (Seconds Per Frame) statistics during gameplay:\nSamples =\t", m_stats.Size(), "\nAverage SPF =\t", meanSpf, " [ms]");
#endif
	/* ==================== Printing stats end ==================== */

	CRITICAL_LOG_ENGINE("Number of allocs: ", numberOfAllocs1, " ", numberOfAllocs2, " ", numberOfAllocs3, " ", numberOfAllocs4,
		"\t", numberOfDeallocs1, " ", numberOfDeallocs2, " ", numberOfDeallocs3, " ", numberOfDeallocs4);

	// TODO: Expand this with additional resources deallocation
	// SAFE_DELETE(m_game);
	SAFE_DELETE(m_physicsEngine);
	glfwTerminate(); // Terminate GLFW
	NOTICE_LOG_ENGINE("Core engine destruction finished");

	Utility::Logging::ILogger::GetLogger("Engine").ResetConsoleColor();
	std::cout << "Bye!" << std::endl;
}

void Engine::CoreEngine::CreateAudioEngine()
{
	Audio::AudioEngineFactory audioEngineFactory(m_audioDirectory);
	m_audioEngine = audioEngineFactory.CreateAudioEngine(static_cast<Audio::AudioEngineTypes::AudioEngineType>(GET_CONFIG_VALUE_AUDIO("audioEngineType", static_cast<int>(Audio::AudioEngineTypes::FMOD))));
	CHECK_CONDITION_EXIT_ENGINE(m_audioEngine != NULL, Utility::Logging::CRITICAL, "Failed to create an audio engine.");
}

void Engine::CoreEngine::CreatePhysicsEngine()
{
	m_physicsEngine = new Physics::PhysicsEngine();

	CHECK_CONDITION_EXIT_ENGINE(m_physicsEngine != NULL, Utility::Logging::CRITICAL, "Failed to create a physics engine.");
}

void Engine::CoreEngine::CreateRenderer(bool fullscreenEnabled, int width, int height, const std::string& title, Rendering::Aliasing::AntiAliasingMethod antiAliasingMethod)
{
	START_PROFILING_ENGINE(true, "");
	InitGraphics(fullscreenEnabled, width, height, title, antiAliasingMethod);
	Rendering::InitGraphics(width, height, antiAliasingMethod);

	glfwSetErrorCallback(&CoreEngine::ErrorCallback);
	//DEBUG_LOG_ENGINE("Thread window address: ", threadWindow);
	NOTICE_LOG_ENGINE("Creating Renderer instance started");
	m_renderer = std::make_unique<Rendering::Renderer>(width, height, m_modelsDirectory, m_texturesDirectory, antiAliasingMethod);
	NOTICE_LOG_ENGINE("Creating Renderer instance finished");

	CHECK_CONDITION_EXIT_ENGINE(m_renderer != NULL, Utility::Logging::CRITICAL, "Failed to create a renderer.");
	STOP_PROFILING_ENGINE("");
}

void Engine::CoreEngine::InitGraphics(bool fullscreenEnabled, int width, int height, const std::string& title, Rendering::Aliasing::AntiAliasingMethod antiAliasingMethod)
{
	InitGlfw(fullscreenEnabled, width, height, title, antiAliasingMethod);
	InitGlew();
	SetCallbacks();
}

void Engine::CoreEngine::InitGlfw(bool fullscreenEnabled, int width, int height, const std::string& title, Rendering::Aliasing::AntiAliasingMethod antiAliasingMethod)
{
	DEBUG_LOG_ENGINE("Initializing GLFW started");
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(glfwInit(), Utility::Logging::CRITICAL, "Failed to initialize GLFW.");

	const int antiAliasingSamples = GET_CONFIG_VALUE_ENGINE("antiAliasingSamples", 4); // TODO: This parameter belongs in the Rendering module. The config value should also be retrieved from the rendering configuration file.
	switch (antiAliasingMethod)
	{
	case Rendering::Aliasing::NONE:
		/**
		* TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		* Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		*/
		glfwWindowHint(GLFW_SAMPLES, 0);
		INFO_LOG_ENGINE("No anti-aliasing algorithm chosen");
		break;
	case Rendering::Aliasing::FXAA:
		/**
		* TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		* Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		*/
		glfwWindowHint(GLFW_SAMPLES, 0);
		INFO_LOG_ENGINE("FXAA anti-aliasing algorithm chosen");
		break;
	case Rendering::Aliasing::MSAA:
		glfwWindowHint(GLFW_SAMPLES, antiAliasingSamples);
		INFO_LOG_ENGINE(antiAliasingSamples, "xMSAA anti-aliasing algorithm chosen");
		break;
	default:
		WARNING_LOG_ENGINE("Unknown anti-aliasing algorithm chosen. Default ", antiAliasingSamples, "xMSAA algorithm chosen");
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
	m_window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL); // Open a window and create its OpenGL context
	if (m_window == NULL)
	{
		CRITICAL_LOG_ENGINE("Failed to create GLFW main window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	m_threadWindow = glfwCreateWindow(1, 1, "Thread Window", NULL, m_window);
	if (m_threadWindow == NULL)
	{
		CRITICAL_LOG_ENGINE("Failed to create GLFW thread window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we can capture the escape key being pressed below
	glfwSetCursorPos(m_window, width / 2, height / 2); // Set cursor position to the middle point
													 //glfwSwapInterval(1);
	glfwSetTime(REAL_ZERO);
	DEBUG_LOG_ENGINE("Initializing GLFW finished successfully");
}

void Engine::CoreEngine::InitGlew()
{
	INFO_LOG_ENGINE("Initializing GLEW started");
	glewExperimental = true; // Needed in core profile
	GLenum err = glewInit();

	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(GLEW_OK == err, Utility::Logging::EMERGENCY, "Error while initializing GLEW: ", glewGetErrorString(err));
	if (GLEW_VERSION_2_0)
	{
		DEBUG_LOG_ENGINE("OpenGL 2.0 supported");
	}
	else
	{
		ERROR_LOG_ENGINE("Initializing GLEW failed. OpenGL 2.0 NOT supported");
		exit(EXIT_FAILURE);
	}

	INFO_LOG_ENGINE("Using GLEW version ", glewGetString(GLEW_VERSION));
	//CheckErrorCode(__FUNCTION__, "Initializing GLEW");
}

void Engine::CoreEngine::SetCallbacks()
{
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(m_window != NULL, Utility::Logging::CRITICAL, "Setting GLFW callbacks failed. The window is NULL.");
	glfwSetWindowCloseCallback(m_window, &CoreEngine::WindowCloseEventCallback);
	glfwSetWindowSizeCallback(m_window, &CoreEngine::WindowResizeCallback);
	glfwSetKeyCallback(m_window, &CoreEngine::KeyEventCallback);
	//glfwSetCharCallback(m_window, &CoreEngine::CharEventCallback);
	//glfwSetMousePosCallback(m_window, &CoreEngine::MouseMotionCallback);
	glfwSetCursorPosCallback(m_window, &CoreEngine::MousePosCallback);
	glfwSetMouseButtonCallback(m_window, &CoreEngine::MouseEventCallback);
	glfwSetScrollCallback(m_window, &CoreEngine::ScrollEventCallback);
}

void Engine::CoreEngine::Start(GameManager* gameManager)
{
	START_PROFILING_ENGINE(true, "");
	m_game = gameManager;
	CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(!m_isRunning, Utility::Logging::WARNING, "The order to start core engine ignored. Core engine instance is already running.");
	NOTICE_LOG_ENGINE("The core engine started");

	Run();
	STOP_PROFILING_ENGINE("");
}

void Engine::CoreEngine::Stop()
{
	START_PROFILING_ENGINE(true, "");
	CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(m_isRunning, Utility::Logging::WARNING, "The order to stop core engine ignored. The core engine instance is not running.");

	m_isRunning = false;
	RequestWindowClose();
	CHECK_CONDITION_ENGINE(!m_isRunning, Utility::Logging::WARNING, "Stopping the core engine is not possible as it is simply not running at the moment.");
	NOTICE_LOG_ENGINE("The core engine has stopped");

	// Just for checking whether time calculation is performed correctly
	//LARGE_INTEGER frequency; // ticks per second
	//QueryPerformanceFrequency(&frequency); // get ticks per second;
	//LARGE_INTEGER t1, t2;
	//QueryPerformanceCounter(&t1);
	//tthread::this_thread::sleep_for(tthread::chrono::seconds(1));
	//QueryPerformanceCounter(&t2);
	//double elapsedTime = static_cast<double>(static_cast<Math::Real>(1000000.0f) * (t2.QuadPart - t1.QuadPart)) / frequency.QuadPart; // in [us]
	//INFO_LOG_ENGINE("Elapsed time = ", elapsedTime, " [us]");
	STOP_PROFILING_ENGINE("");
}

void Engine::CoreEngine::Run()
{
	START_PROFILING_ENGINE(true, "");
	const int THREAD_SLEEP_TIME = GET_CONFIG_VALUE_ENGINE("threadSleepTime", 10);

#ifdef DRAW_FPS
	//Math::Vector3D fpsColors[] = { Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector3D(0.0f, 0.0f, 1.0f) };
	//Math::Real fpsTimes[] = { 0.0f, 2.0f, 5.0f };
	//Math::Vector3D inGameTimeColors[] = { Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector3D(0.0f, 0.0f, 1.0f) };
	//Math::Real inGameTimeTimes[] = { 0.0f, 1.0f, 5.5f };
	// TODO: In the future the FPS and in-game time GUI controls should be a simple GuiTextBoxControls instead of GuiButtonControl.
	Rendering::Controls::GuiButtonControl fpsGuiButton("text", m_game->GetFont(Rendering::Text::FontTypes::CANDARA), GET_CONFIG_VALUE_ENGINE("fontSizeFPS", 2.5f), NULL,
		Math::Vector2D(GET_CONFIG_VALUE_ENGINE("screenPositionFPSX", 0.0f), GET_CONFIG_VALUE_ENGINE("screenPositionFPSY", 0.0f)), Math::Angle(GET_CONFIG_VALUE_ENGINE("screenRotationFPS", 0.0f)),
		Math::Vector2D(GET_CONFIG_VALUE_ENGINE("screenScaleFPSX", 1.0f), GET_CONFIG_VALUE_ENGINE("screenScaleFPSY", 1.0f)),
		GET_CONFIG_VALUE_ENGINE("maxLineLengthFPS", 0.5f), Rendering::Color(GET_CONFIG_VALUE_ENGINE("colorFPSRed", 1.0f), GET_CONFIG_VALUE_ENGINE("colorFPSGreen", 0.0f), GET_CONFIG_VALUE_ENGINE("colorFPSBlue", 0.0f)),
		Rendering::Color(GET_CONFIG_VALUE_ENGINE("outlineColorFPSRed", 0.0f), GET_CONFIG_VALUE_ENGINE("outlineColorFPSGreen", 1.0f), GET_CONFIG_VALUE_ENGINE("outlineColorFPSBlue", 0.0f)),
		Math::Vector2D(GET_CONFIG_VALUE_ENGINE("offsetFPSX", 0.005f), GET_CONFIG_VALUE_ENGINE("offsetFPSY", 0.005f)), GET_CONFIG_VALUE_ENGINE("isCenteredFPS", false),
		GET_CONFIG_VALUE_ENGINE("characterWidthFPS", 0.5f), GET_CONFIG_VALUE_ENGINE("characterEdgeTransitionWidthFPS", 0.1f), GET_CONFIG_VALUE_ENGINE("borderWidthFPS", 0.4f),
		GET_CONFIG_VALUE_ENGINE("borderEdgeTransitionWidthFPS", 0.1f));
#endif

	CHECK_CONDITION_ENGINE(!m_isRunning, Utility::Logging::WARNING, "According to the core engine the game is already running.");

#ifdef ANT_TWEAK_BAR_ENABLED
	Rendering::InitializeTweakBars();
	InitializeTweakBars();
#endif

	m_isRunning = true;

#ifdef COUNT_FPS
	Math::Real fpsSample = static_cast<Math::Real>(GET_CONFIG_VALUE_ENGINE("FPSsample", REAL_ONE)); // represents the time after which FPS value is calculated and logged
	int framesCount = 0;
	Math::Real frameTimeCounter = REAL_ZERO;
	int fps = 0;
	Math::Real spf = REAL_ZERO;
#endif

	m_audioEngine->LoadSong("520387_Horizon_short.mp3");
	m_audioEngine->PlaySong("520387_Horizon_short.mp3");
	//m_audioEngine->play2D("520387_Horizon_short.mp3".c_str(), true);

	Math::Real unprocessingTime = REAL_ZERO; // used to cap the FPS when it gets too high
	Math::Real previousTime = GetTime();

#ifdef PROFILING_RENDERING_MODULE_ENABLED
	//LARGE_INTEGER t1, t2, innerT1, innerT2; // ticks
#endif
	while (m_isRunning)
	{
		//CRITICAL_LOG_ENGINE("START");
		/* ==================== REGION #1 begin ====================*/
		START_TIMER(timer);
		bool isRenderRequired = false;

		// flCurrentTime will be lying around from last frame. It's now the previous time.
		Math::Real currentTime = GetTime();
		Math::Real passedTime = currentTime - previousTime;
		DELOCUST_LOG_ENGINE("Passed time: ", passedTime * 1000.0f, " [ms]");

		previousTime = currentTime;

		// TODO: If unprocessing time is big then each frame it will only get bigger and bigger each frame
		// FPS will plummet, as a result.
		unprocessingTime += passedTime;

#ifdef COUNT_FPS
		frameTimeCounter += passedTime;
		// Counting FPS and logging
		if (frameTimeCounter >= fpsSample)
		{
			fps = static_cast<int>(framesCount / fpsSample); // Frames Per Second
			spf = 1000 * frameTimeCounter / framesCount; // Seconds Per Frame
#ifdef PROFILING_RENDERING_MODULE_ENABLED
			if (m_isSamplingSpf)
			{
				m_stats.Push(Math::Statistics::SPF, spf);
			}
#endif
			//DEBUG_LOG_ENGINE("FPS = ", fps, "\t Average time per frame = ", spf, " [ms]");
			framesCount = 0;
			frameTimeCounter = REAL_ZERO;
		}
#endif
		STOP_TIMER(timer, m_countStats1, m_minMaxTime1, m_timeSum1);
		/* ==================== REGION #1 end ====================*/

		/* ==================== REGION #2 begin ====================*/
		RESET_TIMER(timer);
		while (unprocessingTime > m_frameTime)
		{
			//previousTime = GetTime();
			isRenderRequired = true;
			/**
			 *TODO: The function IsCloseRequested() is called thousand times before actually returning true and closing the application.
			 * Instead we should only check it from time to time or maybe only in some specific game states (e.g. PlayMenuGameState)
			 */
			if (glfwWindowShouldClose(m_window) != 0)
			{
				STOP_PROFILING_ENGINE("");
				return;
			}
			/* ==================== REGION #2_1 begin ====================*/
			START_TIMER(innerTimer);
			PollEvents();
			STOP_TIMER(innerTimer, m_countStats2_1, m_minMaxTime2_1, m_timeSum2_1);
			/* ==================== REGION #2_1 end ====================*/

			/* ==================== REGION #2_2 begin ====================*/
			//RESET_TIMER(innerTimer);
			m_game->Input(m_inputMapping.GetMappedInput());
			m_inputMapping.ClearActions();
			m_inputMapping.ClearRanges();
			//STOP_TIMER(innerTimer, m_countStats2_2, m_minMaxTime2_2, m_timeSum2_2);
			RESET_TIMER(innerTimer);
			m_game->Update(m_frameTime);
#ifdef DRAW_FPS
			fpsGuiButton.Update(m_frameTime);
#endif
			STOP_TIMER(innerTimer, m_countStats2_2, m_minMaxTime2_2, m_timeSum2_2);
			/* ==================== REGION #2_2 end ====================*/

			/* ==================== REGION #2_3 begin ====================*/
			RESET_TIMER(innerTimer);
			m_physicsEngine->Simulate(m_frameTime);
			STOP_TIMER(innerTimer, m_countStats2_3, m_minMaxTime2_3, m_timeSum2_3);
			/* ==================== REGION #2_3 end ====================*/

			/* ==================== REGION #2_4 begin ====================*/
			//RESET_TIMER(innerTimer);
			m_audioEngine->Update(m_frameTime);
			//STOP_TIMER(innerTimer, m_countStats2_4, m_minMaxTime2_4, m_timeSum2_4);
			/* ==================== REGION #2_4 end ====================*/

			/* ==================== Switching the game state if necessary begin ==================== */
			m_game->PerformStateTransition();
			/* ==================== Switching the game state if necessary end ==================== */

#ifdef ANT_TWEAK_BAR_ENABLED
			Rendering::CheckChangesAndUpdateGLState();
#endif

			unprocessingTime -= m_frameTime;
		}
		STOP_TIMER(timer, m_countStats2, m_minMaxTime2, m_timeSum2);
		/* ==================== REGION #2 end ====================*/

		/* ==================== REGION #3 begin ====================*/
		RESET_TIMER(timer);
		if (isRenderRequired)
		{
			//m_renderer->SetReal("timeOfDay", m_timeOfDay);
			m_game->Render(m_renderer.get());
#ifdef COUNT_FPS
			++framesCount;

#ifdef DRAW_FPS
			//ERROR_LOG_ENGINE("1: ", numberOfAllocs1, " ", numberOfAllocs2, " ", numberOfAllocs3, " ", numberOfAllocs4, "\t",
			//	numberOfDeallocs1, " ", numberOfDeallocs2, " ", numberOfDeallocs3, " ", numberOfDeallocs4);
			std::stringstream ss;
			ss << "FPS = " << fps << " SPF[ms] = " << std::setprecision(4) << spf; // TODO: This allocates memory which seemes unneccessary.
			//ERROR_LOG_ENGINE("2: ", numberOfAllocs1, " ", numberOfAllocs2, " ", numberOfAllocs3, " ", numberOfAllocs4, "\t",
			//	numberOfDeallocs1, " ", numberOfDeallocs2, " ", numberOfDeallocs3, " ", numberOfDeallocs4);
			fpsGuiButton.SetText(ss.str());
			m_renderer->RenderGuiControl(fpsGuiButton, m_game->GetShaderFactory().GetShader(Rendering::ShaderIDs::GUI));
			//ERROR_LOG_ENGINE("3: ", numberOfAllocs1, " ", numberOfAllocs2, " ", numberOfAllocs3, " ", numberOfAllocs4, "\t",
			//	numberOfDeallocs1, " ", numberOfDeallocs2, " ", numberOfDeallocs3, " ", numberOfDeallocs4);
#endif
#endif
#ifdef ANT_TWEAK_BAR_ENABLED
			TwDraw();
#endif
			glfwSwapBuffers(m_window);
#ifdef PROFILING_RENDERING_MODULE_ENABLED
			++m_renderingRequiredCount;
#endif
	}
		else
		{
			//INFO_LOG_ENGINE("Rendering is not required. Moving on...");
			std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_SLEEP_TIME)); // Sleep for some time to prevent the thread from constant looping
#ifdef PROFILING_RENDERING_MODULE_ENABLED
			++m_renderingNotRequiredCount;
#endif
		}
		STOP_TIMER(timer, m_countStats3, m_minMaxTime3, m_timeSum3);
		/* ==================== REGION #3 end ====================*/
		//CRITICAL_LOG_ENGINE("STOP");
}
}

void Engine::CoreEngine::WindowResizeEvent(GLFWwindow* window, int width, int height)
{
	m_renderer->SetWindowWidth(width);
	m_renderer->SetWindowHeight(height);
	m_game->WindowResizeEvent(width, height);
}

void Engine::CoreEngine::ErrorCallbackEvent(int errorCode, const char* description)
{
	switch (errorCode)
	{
	case GLFW_NOT_INITIALIZED:
		ERROR_LOG_ENGINE("GLFW has not been initialized. Error description: ", description);
		break;
	case GLFW_NO_CURRENT_CONTEXT:
		ERROR_LOG_ENGINE("No context is current for this thread. Error description: ", description);
		break;
	case GLFW_INVALID_ENUM:
		ERROR_LOG_ENGINE("One of the arguments to the function was an invalid enum value. Error description: ", description);
		break;
	case GLFW_INVALID_VALUE:
		ERROR_LOG_ENGINE("One of the arguments to the function was an invalid value. Error description: ", description);
		break;
	case GLFW_OUT_OF_MEMORY:
		ERROR_LOG_ENGINE("A memory allocation failed. Error description: ", description);
		break;
	case GLFW_API_UNAVAILABLE:
		ERROR_LOG_ENGINE("GLFW could not find support for the requested client API on the system. Error description: ", description);
		break;
	case GLFW_VERSION_UNAVAILABLE:
		ERROR_LOG_ENGINE("The requested OpenGL or OpenGL ES version is not available. Error description: ", description);
		break;
	case GLFW_PLATFORM_ERROR:
		ERROR_LOG_ENGINE("A platform-specific error occurred that does not match any of the more specific categories. Error description: ", description);
		break;
	case GLFW_FORMAT_UNAVAILABLE:
		ERROR_LOG_ENGINE("The requested format is not supported or available. Error description: ", description);
		break;
	default:
		ERROR_LOG_ENGINE("Unknown GLFW error event occurred with code ", errorCode, " and message: Error description: ", description);
	}
	exit(EXIT_FAILURE);
}

void Engine::CoreEngine::CloseWindowEvent(GLFWwindow* window)
{
	m_game->CloseWindowEvent();
}

void Engine::CoreEngine::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::map<int, Input::RawInputKeys::RawInputKey>::const_iterator rawInputKeyItr = m_glfwKeysToRawInputKeysMap.find(key);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(rawInputKeyItr != m_glfwKeysToRawInputKeysMap.end(), Utility::Logging::ERR, "Key ", key, " not found in the map.");
	m_inputMapping.SetRawButtonState(rawInputKeyItr->second, action != GLFW_RELEASE, action == GLFW_REPEAT);
	//m_game->KeyEvent(key, scancode, action, mods);
}

void Engine::CoreEngine::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	// TODO: The action can either be GLFW_PRESS or GLFW_RELEASE, so inputMapping cannot perform e.g. drag & dropping using mouse. Improve it.
	DELOCUST_LOG_ENGINE("Mouse button event: button=", button, "\t action=", action, "\t mods=", mods);

	std::map<int, Input::RawInputKeys::RawInputKey>::const_iterator rawInputKeyItr = m_glfwKeysToRawInputKeysMap.find(button);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_ENGINE(rawInputKeyItr != m_glfwKeysToRawInputKeysMap.end(), Utility::Logging::ERR, "Button ", button, " not found in the map.");
	m_inputMapping.SetRawButtonState(rawInputKeyItr->second, action == GLFW_PRESS, true /* TODO: mouseButtonEvent will never have action equal to GLFW_REPEAT. */);
	//m_game->MouseButtonEvent(button, action, mods);
}

//double lastXPos = 0.0;
//double lastYPos = 0.0;

void Engine::CoreEngine::MousePosEvent(GLFWwindow* window, double xPos, double yPos)
{
	DEBUG_LOG_ENGINE("Mouse position = (", xPos, ", ", yPos, ")");
	m_inputMapping.SetRawAxisValue(Engine::Input::RawInputAxes::RAW_INPUT_AXIS_MOUSE_X, xPos);
	m_inputMapping.SetRawAxisValue(Engine::Input::RawInputAxes::RAW_INPUT_AXIS_MOUSE_Y, yPos);
	//lastXPos = xPos;
	//lastYPos = yPos;
	//m_game->MousePosEvent(xPos, yPos);
}

void Engine::CoreEngine::ScrollEvent(GLFWwindow* window, double xOffset, double yOffset)
{
	m_game->ScrollEvent(xOffset, yOffset);
}

void Engine::CoreEngine::PollEvents()
{
	glfwPollEvents();
}

Math::Real Engine::CoreEngine::GetTime() const
{
	return static_cast<Math::Real>(glfwGetTime());
	//return Time(glfwGetTime());

	//return Time::Now();
}

void Engine::CoreEngine::ClearScreen() const
{
}

void Engine::CoreEngine::SetCursorPos(Math::Real xPos, Math::Real yPos)
{
	if (m_renderer == NULL)
	{
		CRITICAL_LOG_ENGINE("Cannot set cursor position. The rendering engine is NULL.");
		return;
	}
	glfwSetCursorPos(m_window, xPos, yPos);
}

void Engine::CoreEngine::CentralizeCursor()
{
	SetCursorPos(static_cast<Math::Real>(m_windowWidth) / 2, static_cast<Math::Real>(m_windowHeight) / 2);
}

void Engine::CoreEngine::AddBillboardNode(GameNode* billboardNode)
{
	//m_renderer->AddBillboardNode(billboardNode);
}

void Engine::CoreEngine::AddPhysicsObject(Physics::PhysicsObject* physicsObject)
{
	m_physicsEngine->AddPhysicsObject(physicsObject);
}

void Engine::CoreEngine::PushInputContext(const std::string& inputContextName)
{
	m_inputMapping.PushContext(inputContextName);
}

void Engine::CoreEngine::PopInputContext()
{
	m_inputMapping.PopContext();
}

#ifdef ANT_TWEAK_BAR_ENABLED
void Engine::CoreEngine::InitializeTweakBars()
{
	Rendering::AntTweakBarTypes::InitializeTweakBarTypes();

	TwWindowSize(m_windowWidth, m_windowHeight);

	TwBar* coreEnginePropertiesBar = TwNewBar("CoreEnginePropertiesBar");
	TwAddVarRW(coreEnginePropertiesBar, "windowWidth", TW_TYPE_INT32, &m_windowWidth, " label='Window width' ");
	TwAddVarRW(coreEnginePropertiesBar, "windowHeight", TW_TYPE_INT32, &m_windowHeight, " label='Window height' ");
	TwAddVarRO(coreEnginePropertiesBar, "frameTime", TW_TYPE_REAL, &m_frameTime, " label='Frame time' ");
	//TwAddVarRW(coreEnginePropertiesBar, "clockSpeed", TW_TYPE_REAL, &m_clockSpeed, " label='Clock speed' ");
	//TwAddVarRW(coreEnginePropertiesBar, "timeOfDay", TW_TYPE_REAL, &m_timeOfDay, " label='Time of day' ");

	//TwEnumVal daytimeEV[] = { { Utility::Timing::NIGHT, "Night" }, { Utility::Timing::BEFORE_DAWN, "Before dawn" }, { Utility::Timing::SUNRISE, "Sunrise" },
	//	{ Utility::Timing::DAY, "Day" }, { Utility::Timing::SUNSET, "Sunset" }, { Utility::Timing::AFTER_DUSK, "After dusk" } };
	//TwType daytimeType = TwDefineEnum("Daytime", daytimeEV, 6);
	//TwAddVarRW(coreEnginePropertiesBar, "daytime", daytimeType, &m_daytime, " label='Daytime' ");

	//TwAddVarRW(coreEnginePropertiesBar, "sunElevation", Rendering::angleType, &m_sunElevation, " label='Sun elevation' ");
	//TwAddVarRW(coreEnginePropertiesBar, "sunAzimuth", Rendering::angleType, &m_sunAzimuth, " label='Sun azimuth' ");
	//TwAddVarRW(coreEnginePropertiesBar, "sunFirstElevationLevel", Rendering::angleType, &M_FIRST_ELEVATION_LEVEL, " label='First elevation level' ");
	//TwAddVarRW(coreEnginePropertiesBar, "sunSecondElevationLevel", Rendering::angleType, &M_SECOND_ELEVATION_LEVEL, " label='Second elevation level' ");
	//TwAddVarRW(coreEnginePropertiesBar, "sunThirdElevationLevel", Rendering::angleType, &M_THIRD_ELEVATION_LEVEL, " label='Third elevation level' ");

	TwDefine(" CoreEnginePropertiesBar refresh=0.5 ");
	//double refreshRate = 0.2;
	//TwSetParam(coreEnginePropertiesBar, NULL, "refresh", TW_PARAM_DOUBLE, 1, &refreshRate);

	//TwSetParam(coreEnginePropertiesBar, NULL, "visible", TW_PARAM_CSTRING, 1, "false"); // Hide the bar at startup
}

void Engine::CoreEngine::InitializeGameTweakBars()
{
	m_game->InitializeTweakBars();
	m_renderer->InitializeTweakBars();
}
#endif

//#ifdef PROFILING_RENDERING_MODULE_ENABLED
//Math::Real Engine::CoreEngine::CalculateAverageSpf(Math::Real& minSpf, Math::Real& maxSpf, Math::Real& stdDev) const
//{
//	if (m_secondsPerFrameStats.empty())
//	{
//		minSpf = REAL_ZERO;
//		maxSpf = REAL_ZERO;
//		stdDev = REAL_ZERO;
//		return REAL_ZERO;
//	}
//
//	minSpf = REAL_MAX;
//	maxSpf = REAL_MIN;
//	Math::Real spfSum = REAL_ZERO;
//	for (std::vector<Math::Real>::const_iterator spfItr = m_secondsPerFrameStats.begin(); spfItr != m_secondsPerFrameStats.end(); ++spfItr)
//	{
//		Math::Real spf = *spfItr;
//		if (spf < minSpf)
//		{
//			minSpf = spf;
//		}
//		else if (spf > maxSpf)
//		{
//			maxSpf = spf;
//		}
//		spfSum += spf;
//	}
//
//	Math::Real average = spfSum / m_secondsPerFrameStats.size();
//
//	/* ==================== Calculating standard deviation begin ==================== */
//	stdDev = REAL_ZERO;
//	for (std::vector<Math::Real>::const_iterator spfItr = m_secondsPerFrameStats.begin(); spfItr != m_secondsPerFrameStats.end(); ++spfItr)
//	{
//		stdDev += (average - *spfItr) * (average - *spfItr);
//	}
//	stdDev = sqrt(stdDev / m_secondsPerFrameStats.size());
//	/* ==================== Calculating standard deviation end ==================== */
//
//
//	return average;
//}
//#endif