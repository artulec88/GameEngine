#include "StdAfx.h"
#include "CoreEngine.h"
#include "tinythread.h"

#include "Audio\AudioEngineFactory.h"

#include "Math\Math.h"
#include "Math\FloatingPoint.h"

#include "Utility\ILogger.h"
#include "Utility\Config.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <GLFW\glfw3.h>

/**
 * See http://stackoverflow.com/questions/546997/use-ifdefs-and-define-to-optionally-turn-a-function-call-into-a-comment
 */
#ifndef CALCULATE_RENDERING_STATS
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


Engine::CoreEngine* Engine::CoreEngine::s_coreEngine = NULL;

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

Engine::CoreEngine::CoreEngine(int width, int height, const char* title, int maxFrameRate, const std::string& shadersDirectory /* = "..\\Shaders\\" */,
	const std::string& modelsDirectory /* = "..\\Models\\" */, const std::string& texturesDirectory /* = "..\\Textures\\" */,
	const std::string& fontsDirectory /* = "..\\Fonts\\" */, const std::string& audioDirectory /* = "..\\Sounds\\" */) :
	m_window(NULL),
	m_threadWindow(NULL),
	m_isRunning(false),
	m_windowWidth(width),
	m_windowHeight(height),
	m_windowTitle(title),
	m_frameTime(1.0f / maxFrameRate),
	m_game(NULL),
	m_audioEngine(nullptr),
	m_physicsEngine(NULL),
	m_renderer(NULL),
	LATITUDE(GET_CONFIG_VALUE_ENGINE("latitude", 52.0f)),
	LONGITUDE(GET_CONFIG_VALUE_ENGINE("longitude", -16.0f)),
	TROPIC_OF_CANCER_SINUS(0.39794863131f),
	SECONDS_PER_MINUTE(60),
	SECONDS_PER_HOUR(3600),
	SECONDS_PER_DAY(86400),
	DAYS_PER_YEAR(365),
	m_dayNumber(GET_CONFIG_VALUE_ENGINE("startingDayNumber", 172)),
	m_timeOfDay(GET_CONFIG_VALUE_ENGINE("startingTimeOfDay", REAL_ZERO)),
	m_daytime(Utility::Timing::NIGHT),
	m_sunElevation(0.0f),
	m_sunAzimuth(0.0f),
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
		{ GLFW_MOUSE_BUTTON_LEFT, Input::RawInputKeys::MOUSE_KEY_LEFT }, { GLFW_MOUSE_BUTTON_MIDDLE, Input::RawInputKeys::MOUSE_KEY_MIDDLE }, { GLFW_MOUSE_BUTTON_RIGHT, Input::RawInputKeys::MOUSE_KEY_RIGHT } }),
	m_inputMapping(GET_CONFIG_VALUE_STR_ENGINE("inputContextsListFileName", "ContextsList.txt"))
#ifdef CALCULATE_RENDERING_STATS
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
	m_timer(),
	m_renderingRequiredCount(0),
	m_renderingNotRequiredCount(0),
	m_isSamplingSpf(true),
	m_classStats(STATS_STORAGE.GetClassStats("CoreEngine")),
	m_stats(),
#endif
	M_FIRST_ELEVATION_LEVEL(GET_CONFIG_VALUE_ENGINE("sunlightFirstElevationLevel", -REAL_ONE)),
	M_SECOND_ELEVATION_LEVEL(GET_CONFIG_VALUE_ENGINE("sunlightSecondElevationLevel", REAL_ZERO)),
	M_THIRD_ELEVATION_LEVEL(GET_CONFIG_VALUE_ENGINE("sunlightThirdElevationLevel", REAL_ONE)),
	m_clockSpeed(GET_CONFIG_VALUE_ENGINE("clockSpeed", REAL_ONE))
{
	Utility::Logging::ILogger::GetLogger("Engine").Fill(GET_CONFIG_VALUE_STR_ENGINE("LoggingLevel", "Info"), Utility::Logging::INFO); // Initializing engine logger
	Utility::Logging::ILogger::GetLogger("Math").Fill(GET_CONFIG_VALUE_STR_MATH("LoggingLevel", "Info"), Utility::Logging::INFO); // Initializing math logger
	Utility::Logging::ILogger::GetLogger("Utility").Fill(GET_CONFIG_VALUE_STR_UTILITY("LoggingLevel", "Info"), Utility::Logging::INFO); // Initializing utility logger

	NOTICE_LOG_ENGINE("Main application construction started");
#ifdef CALCULATE_RENDERING_STATS
	m_timer.Start();
#endif
	START_PROFILING;

	if (s_coreEngine != NULL)
	{
		ERROR_LOG_ENGINE("Constructor called when a singleton instance of MainApp class has already been created");
		SAFE_DELETE(s_coreEngine);
	}
	s_coreEngine = this;

	CreateAudioEngine();
	CreatePhysicsEngine();
	CreateRenderer(width, height, title);

	m_dayNumber = m_dayNumber % DAYS_PER_YEAR;
	m_timeOfDay = fmod(m_timeOfDay, static_cast<Math::Real>(SECONDS_PER_DAY)); // return value within range [0.0; SECONDS_PER_DAY) (see http://www.cplusplus.com/reference/cmath/fmod/)
	if (m_timeOfDay < REAL_ZERO)
	{
		m_timeOfDay = GetCurrentLocalTime();
	}
	CalculateSunElevationAndAzimuth();

	STOP_PROFILING;

	NOTICE_LOG_ENGINE("Main application construction finished");
}


	Engine::CoreEngine::~CoreEngine(void)
{
	DEBUG_LOG_ENGINE("Core engine destruction started");

	/* ==================== Printing stats begin ==================== */
#ifdef CALCULATE_RENDERING_STATS
	INFO_LOG_ENGINE("The region #1 (Time calculating) was processed ", m_countStats1, " times, which took exactly", m_timeSum1, " [us]. The average time=", m_timeSum1 / m_countStats1, " [us]. ", m_minMaxTime1.ToString());
	INFO_LOG_ENGINE("The region #2 was processed ", m_countStats2, " times, which took exactly ", m_timeSum2, " [us]. The average time=", m_timeSum2 / m_countStats2, "[us]. ", m_minMaxTime2.ToString());
	INFO_LOG_ENGINE("\t The region #2_1 (Polling events) was processed ", m_countStats2_1, " times, which took exactly ", m_timeSum2_1, " [us]. The average time=", m_timeSum2_1 / m_countStats2_1, " [us]. ", m_minMaxTime2_1.ToString());
	INFO_LOG_ENGINE("\t The region #2_2 (Game input processing) was processed ", m_countStats2_2, " times, which took exactly ", m_timeSum2_2, " [us]. The average time=", m_timeSum2_2 / m_countStats2_2, " [us]. ", m_minMaxTime2_2.ToString());
	INFO_LOG_ENGINE("\t The region #2_3 (Game updating) was processed ", m_countStats2_3, " times, which took exactly ", m_timeSum2_3, " [us]. The average time=", m_timeSum2_3 / m_countStats2_3, " [us]. ", m_minMaxTime2_3.ToString());
	INFO_LOG_ENGINE("The region #3 (Rendering) was processed ", m_countStats3, " times, which took exactly ", m_timeSum3, " [us]. The average time=", m_timeSum3 / m_countStats3, " [us]. ", m_minMaxTime3.ToString());
	INFO_LOG_ENGINE("Rendering step performed ", m_renderingRequiredCount, " times.");
	INFO_LOG_ENGINE("Rendering step omitted ", m_renderingNotRequiredCount, " times.");

	m_timer.Stop();
	STATS_STORAGE.PrintReport(m_timer.GetTimeSpan(Utility::Timing::SECOND));

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
	SAFE_DELETE(m_renderer);
	glfwTerminate(); // Terminate GLFW
	NOTICE_LOG_ENGINE("Core engine destruction finished");

	Utility::Logging::ILogger::GetLogger("Engine").ResetConsoleColor();
	std::cout << "Bye!" << std::endl;
}

void Engine::CoreEngine::CreateAudioEngine()
{
	Audio::AudioEngineFactory audioEngineFactory;
	m_audioEngine = audioEngineFactory.CreateAudioEngine(static_cast<Audio::AudioEngineTypes::AudioEngineType>(GET_CONFIG_VALUE_AUDIO("audioEngineType", static_cast<int>(Audio::AudioEngineTypes::FMOD))));
	CHECK_CONDITION_EXIT_ENGINE(m_audioEngine != NULL, Utility::CRITICAL, "Failed to create an audio engine.");
}

void Engine::CoreEngine::CreatePhysicsEngine()
{
	m_physicsEngine = new Physics::PhysicsEngine();

	CHECK_CONDITION_EXIT_ENGINE(m_physicsEngine != NULL, Utility::CRITICAL, "Failed to create a physics engine.");
}

void Engine::CoreEngine::CreateRenderer(int width, int height, const std::string& title)
{
	START_PROFILING;
	InitGraphics(width, height, title);
	Rendering::InitGraphics(width, height);

	glfwSetErrorCallback(&CoreEngine::ErrorCallback);
	//DEBUG_LOG_ENGINE("Thread window address: ", threadWindow);
	m_renderer = new Rendering::Renderer(width, height);

	CHECK_CONDITION_EXIT_ENGINE(m_renderer != NULL, Utility::CRITICAL, "Failed to create a renderer.");
	STOP_PROFILING;
}

void Engine::CoreEngine::InitGraphics(int width, int height, const std::string& title)
{
	InitGlfw(width, height, title);
	InitGlew();
	SetCallbacks();
}

void Engine::CoreEngine::InitGlfw(int width, int height, const std::string& title)
{
	DEBUG_LOG_ENGINE("Initializing GLFW started");
	CHECK_CONDITION_EXIT_ALWAYS_ENGINE(glfwInit(), Utility::Logging::CRITICAL, "Failed to initialize GLFW.");

	int antiAliasingSamples = GET_CONFIG_VALUE_ENGINE("antiAliasingSamples", 4); /* 4x anti-aliasing by default */
	Rendering::Aliasing::AntiAliasingMethod antiAliasingMethod = Rendering::Aliasing::NONE;
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
	bool fullscreenEnabled = GET_CONFIG_VALUE_ENGINE("fullscreenEnabled", false);
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

	if (GLEW_OK != err)
	{
		ERROR_LOG_ENGINE("Error while initializing GLEW: ", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
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
	START_PROFILING;
	m_game = gameManager;
	if (m_isRunning)
	{
		WARNING_LOG_ENGINE("The core engine instance is already running");
		return;
	}
	NOTICE_LOG_ENGINE("The core engine started");

	Run();
	STOP_PROFILING;
}

void Engine::CoreEngine::Stop()
{
	START_PROFILING;
	if (!m_isRunning)
	{
		WARNING_LOG_ENGINE("The core engine instance is not running");
		return;
	}

	m_isRunning = false;
	RequestWindowClose();
	CHECK_CONDITION_ENGINE(!m_isRunning, Utility::WARNING, "Stopping the core engine is not possible as it is simply not running at the moment.");
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
	STOP_PROFILING;
}

void Engine::CoreEngine::Run()
{
	START_PROFILING;
	const int THREAD_SLEEP_TIME = GET_CONFIG_VALUE_ENGINE("threadSleepTime", 10);

#ifdef DRAW_FPS
	const Rendering::Text::Font font(GET_CONFIG_VALUE_STR_ENGINE("fontTextureAtlas", "segoe.png"), GET_CONFIG_VALUE_STR_ENGINE("fontMetaData", "segoe.fnt"));
	Math::Vector3D fpsColors[] = { Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector3D(0.0f, 0.0f, 1.0f) };
	Math::Real fpsTimes[] = { 0.0f, 2.0f, 5.0f };
	Math::Vector3D inGameTimeColors[] = { Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector3D(0.0f, 0.0f, 1.0f) };
	Math::Real inGameTimeTimes[] = { 0.0f, 1.0f, 5.5f };
	Rendering::Controls::GuiTextControl fpsGuiText("", &font, GET_CONFIG_VALUE_ENGINE("fontSizeFPS", 2.5f),
		Math::Vector2D(GET_CONFIG_VALUE_ENGINE("screenPositionFPSX", 0.0f), GET_CONFIG_VALUE_ENGINE("screenPositionFPSY", 0.0f)),
		GET_CONFIG_VALUE_ENGINE("maxLineLengthFPS", 0.5f), Math::Vector3D(GET_CONFIG_VALUE_ENGINE("colorFPSRed", 1.0f), GET_CONFIG_VALUE_ENGINE("colorFPSGreen", 0.0f), GET_CONFIG_VALUE_ENGINE("colorFPSBlue", 0.0f)),
		Math::Vector3D(GET_CONFIG_VALUE_ENGINE("outlineColorFPSRed", 0.0f), GET_CONFIG_VALUE_ENGINE("outlineColorFPSGreen", 1.0f), GET_CONFIG_VALUE_ENGINE("outlineColorFPSBlue", 0.0f)),
		Math::Vector2D(GET_CONFIG_VALUE_ENGINE("offsetFPSX", 0.005f), GET_CONFIG_VALUE_ENGINE("offsetFPSY", 0.005f)), GET_CONFIG_VALUE_ENGINE("isCenteredFPS", false),
		GET_CONFIG_VALUE_ENGINE("characterWidthFPS", 0.5f), GET_CONFIG_VALUE_ENGINE("characterEdgeTransitionWidthFPS", 0.1f), GET_CONFIG_VALUE_ENGINE("borderWidthFPS", 0.4f),
		GET_CONFIG_VALUE_ENGINE("borderEdgeTransitionWidthFPS", 0.1f));
	Rendering::Controls::GuiTextControl inGameTimeGuiText("", &font, GET_CONFIG_VALUE_ENGINE("fontSizeInGameTime", 2.5f),
		Math::Vector2D(GET_CONFIG_VALUE_ENGINE("screenPositionInGameTimeX", 0.0f), GET_CONFIG_VALUE_ENGINE("screenPositionInGameTimeY", 0.0f)), GET_CONFIG_VALUE_ENGINE("maxLineLengthInGameTime", 0.5f),
		Math::Vector3D(GET_CONFIG_VALUE_ENGINE("colorInGameTimeRed", 1.0f), GET_CONFIG_VALUE_ENGINE("colorInGameTimeGreen", 0.0f), GET_CONFIG_VALUE_ENGINE("colorInGameTimeBlue", 0.0f)),
		Math::Vector3D(GET_CONFIG_VALUE_ENGINE("outlineColorInGameTimeRed", 0.0f), GET_CONFIG_VALUE_ENGINE("outlineColorInGameTimeGreen", 1.0f), GET_CONFIG_VALUE_ENGINE("outlineColorInGameTimeBlue", 0.0f)),
		Math::Vector2D(GET_CONFIG_VALUE_ENGINE("offsetInGameTimeX", 0.005f), GET_CONFIG_VALUE_ENGINE("offsetInGameTimeY", 0.005f)), GET_CONFIG_VALUE_ENGINE("isCenteredInGameTime", false),
		GET_CONFIG_VALUE_ENGINE("characterWidthInGameTime", 0.5f), GET_CONFIG_VALUE_ENGINE("characterEdgeTransitionWidthInGameTime", 0.1f), GET_CONFIG_VALUE_ENGINE("borderWidthInGameTime", 0.4f),
		GET_CONFIG_VALUE_ENGINE("borderEdgeTransitionWidthInGameTime", 0.1f));
#endif

	CHECK_CONDITION_ENGINE(!m_isRunning, Utility::WARNING, "According to the core engine the game is already running.");

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

	m_audioEngine->LoadSong(m_audioDirectory + "520387_Horizon_short.mp3");
	m_audioEngine->PlaySong(m_audioDirectory + "520387_Horizon_short.mp3");
	//m_audioEngine->play2D((m_audioDirectory + "520387_Horizon_short.mp3").c_str(), true);

	Math::Real unprocessingTime = REAL_ZERO; // used to cap the FPS when it gets too high
	Math::Real previousTime = GetTime();
	int inGameHours, inGameMinutes, inGameSeconds;

#ifdef CALCULATE_RENDERING_STATS
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

		if (m_game->IsInGameTimeCalculationEnabled())
		{
			m_timeOfDay += (passedTime * m_clockSpeed); // adjusting in-game time
			if (m_timeOfDay > SECONDS_PER_DAY)
			{
				m_timeOfDay -= SECONDS_PER_DAY; // this will not work if m_clockSpeed > SECONDS_PER_DAY. The time of day will increase until going out of range [0; SECONDS_PER_DAY).
			}
			CalculateSunElevationAndAzimuth(); // adjusting sun elevation and azimuth based on current in-game time
			ConvertTimeOfDay(inGameHours, inGameMinutes, inGameSeconds);
		}

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
#ifdef CALCULATE_RENDERING_STATS
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
				STOP_PROFILING;
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
			fpsGuiText.Update(m_frameTime);
			inGameTimeGuiText.Update(m_frameTime);
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
			m_game->Render(m_renderer);
#ifdef COUNT_FPS
			++framesCount;

#ifdef DRAW_FPS
			//ERROR_LOG_ENGINE("1: ", numberOfAllocs1, " ", numberOfAllocs2, " ", numberOfAllocs3, " ", numberOfAllocs4, "\t",
			//	numberOfDeallocs1, " ", numberOfDeallocs2, " ", numberOfDeallocs3, " ", numberOfDeallocs4);
			std::stringstream ss;
			ss << "FPS = " << fps << " SPF[ms] = " << std::setprecision(4) << spf; // TODO: This allocates memory which seemes unneccessary.
			//ERROR_LOG_ENGINE("2: ", numberOfAllocs1, " ", numberOfAllocs2, " ", numberOfAllocs3, " ", numberOfAllocs4, "\t",
			//	numberOfDeallocs1, " ", numberOfDeallocs2, " ", numberOfDeallocs3, " ", numberOfDeallocs4);
			fpsGuiText.SetText(ss.str());
			m_renderer->RenderGuiControl(fpsGuiText);
			//ERROR_LOG_ENGINE("3: ", numberOfAllocs1, " ", numberOfAllocs2, " ", numberOfAllocs3, " ", numberOfAllocs4, "\t",
			//	numberOfDeallocs1, " ", numberOfDeallocs2, " ", numberOfDeallocs3, " ", numberOfDeallocs4);
#endif
#ifdef DRAW_GAME_TIME
			if (m_game->IsInGameTimeCalculationEnabled())
			{
#ifndef DRAW_FPS
				std::stringstream ss;
#endif
				ss.str("");
				// TODO: Leading zeros (setfill('0') << setw(5))
				if (inGameHours < 10) { ss << "Time: 0"; }
				else { ss << "Time: "; }
				if (inGameMinutes < 10) { ss << inGameHours << ":0" << inGameMinutes; }
				else { ss << inGameHours << ":" << inGameMinutes; }
				if (m_clockSpeed < 12.0f)
				{
					if (inGameSeconds < 10)
					{
						ss << ":0" << inGameSeconds;
					}
					else
					{
						ss << ":" << inGameSeconds;
					}
				}
				inGameTimeGuiText.SetText(ss.str());
				m_renderer->RenderGuiControl(inGameTimeGuiText);
		}
#endif
#endif
#ifdef ANT_TWEAK_BAR_ENABLED
			TwDraw();
#endif
			glfwSwapBuffers(m_window);
#ifdef CALCULATE_RENDERING_STATS
			++m_renderingRequiredCount;
#endif
	}
		else
		{
			//INFO_LOG_ENGINE("Rendering is not required. Moving on...");
			// TODO: Sleep for 1ms to prevent the thread from constant looping
			//this_thread::sleep_for(chrono::milliseconds(100));
			tthread::this_thread::sleep_for(tthread::chrono::milliseconds(THREAD_SLEEP_TIME));
#ifdef CALCULATE_RENDERING_STATS
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

void Engine::CoreEngine::ConvertTimeOfDay(int& inGameHours, int& inGameMinutes, int& inGameSeconds) const
{
	ConvertTimeOfDay(m_timeOfDay, inGameHours, inGameMinutes, inGameSeconds);
}

void Engine::CoreEngine::ConvertTimeOfDay(Math::Real timeOfDay, int& inGameHours, int& inGameMinutes, int& inGameSeconds) const
{
	inGameHours = static_cast<int>(timeOfDay) / SECONDS_PER_HOUR;
	Math::Real temp = fmod(m_timeOfDay, static_cast<Math::Real>(SECONDS_PER_HOUR));
	inGameMinutes = static_cast<int>(temp) / SECONDS_PER_MINUTE;
	inGameSeconds = static_cast<int>(fmod(temp, static_cast<Math::Real>(SECONDS_PER_MINUTE)));
}

size_t Engine::CoreEngine::GetCurrentCameraIndex() const
{
	CHECK_CONDITION_EXIT_ENGINE(m_renderer != NULL, CRITICAL, "Cannot get the current camera index. The renderer does not exist.");
	return m_renderer->GetCurrentCameraIndex();
}

size_t Engine::CoreEngine::NextCamera() const
{
	CHECK_CONDITION_EXIT_ENGINE(m_renderer != NULL, CRITICAL, "Cannot move to the next camera. The renderer does not exist.");
	return m_renderer->NextCamera();
}

size_t Engine::CoreEngine::PrevCamera() const
{
	CHECK_CONDITION_EXIT_ENGINE(m_renderer != NULL, CRITICAL, "Cannot move to the previous camera. The renderer does not exist.");
	return m_renderer->PrevCamera();
}

void Engine::CoreEngine::PollEvents()
{
	glfwPollEvents();
}

/**
 * See http://www.cplusplus.com/reference/ctime/localtime/
 * http://www.cplusplus.com/reference/ctime/strftime/
 */
Math::Real Engine::CoreEngine::GetCurrentLocalTime() const
{
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	int result = SECONDS_PER_HOUR * timeinfo.tm_hour + SECONDS_PER_MINUTE * timeinfo.tm_min + timeinfo.tm_sec;
	if (result > SECONDS_PER_DAY)
	{
		ERROR_LOG_ENGINE("Incorrect local time");
		// result = REAL_ZERO;
		result -= SECONDS_PER_DAY;
	}
	return static_cast<Math::Real>(result);
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
	if (m_renderer == NULL)
	{
		CRITICAL_LOG_ENGINE("Cannot set cursor position. The rendering engine is NULL.");
		return;
	}
	glfwSetCursorPos(m_window, static_cast<Math::Real>(m_windowWidth) / 2, static_cast<Math::Real>(m_windowHeight) / 2);
}

void Engine::CoreEngine::CalculateSunElevationAndAzimuth()
{
	const int timeGMTdifference = 1;

	const Math::Angle b(0.9863014f * (m_dayNumber - 81)); // 0,98630136986301369863013698630137 = 360 / 365
	const Math::Real bSin = b.Sin();
	const Math::Real bCos = b.Cos();

	const Math::Real equationOfTime = 19.74f * bSin * bCos - 7.53f * bCos - 1.5f * bSin; // EoT
	const Math::Real declinationSin = TROPIC_OF_CANCER_SINUS * bSin;
	const Math::Angle declinationAngle(asin(declinationSin), Math::Unit::RADIAN);
	//DEBUG_LOG_ENGINE("Declination in degrees = ", declinationAngle.GetAngleInDegrees());

	const Math::Real timeCorrectionInSeconds = 60.0f * (4.0f * (LONGITUDE.GetAngleInDegrees() - 15.0f * timeGMTdifference) + equationOfTime);
	const Math::Real localSolarTime = m_timeOfDay + timeCorrectionInSeconds;
	//DEBUG_LOG_ENGINE("Time correction in seconds = ", timeCorrectionInSeconds);
	//DEBUG_LOG_ENGINE("Local time = ", m_timeOfDay, "\tLocal solar time = ", localSolarTime);

	const Math::Angle hourAngle(15.0f * (localSolarTime - 12 * SECONDS_PER_HOUR) / SECONDS_PER_HOUR);
	//DEBUG_LOG_ENGINE("Hour angle = ", hourAngle.GetAngleInDegrees());

	const Math::Real sunElevationSin = declinationSin * LATITUDE.Sin() + declinationAngle.Cos() * LATITUDE.Cos() * hourAngle.Cos();
	m_sunElevation.SetAngleInRadians(asin(sunElevationSin));
	//DEBUG_LOG_ENGINE("Sun elevation = ", m_sunElevation.GetAngleInDegrees());

	const Math::Real sunAzimuthCos = ((declinationSin * LATITUDE.Cos()) - (declinationAngle.Cos() * LATITUDE.Sin() * hourAngle.Cos())) / m_sunElevation.Cos();
	m_sunAzimuth.SetAngleInRadians(acos(sunAzimuthCos));
	bool isAfternoon = (localSolarTime > 12.0f * SECONDS_PER_HOUR) || (hourAngle.GetAngleInDegrees() > REAL_ZERO);
	if (isAfternoon)
	{
		m_sunAzimuth.SetAngleInDegrees(360.0f - m_sunAzimuth.GetAngleInDegrees());
	}

	Utility::Timing::Daytime prevDaytime = m_daytime;
	if (m_sunElevation < M_FIRST_ELEVATION_LEVEL)
	{
		m_daytime = Utility::Timing::NIGHT;
	}
	else if (m_sunElevation < M_SECOND_ELEVATION_LEVEL)
	{
		m_daytime = (isAfternoon) ? Utility::Timing::AFTER_DUSK : Utility::Timing::BEFORE_DAWN;
	}
	else if (m_sunElevation < M_THIRD_ELEVATION_LEVEL)
	{
		m_daytime = (isAfternoon) ? Utility::Timing::SUNSET : Utility::Timing::SUNRISE;
	}
	else
	{
		m_daytime = Utility::Timing::DAY;
	}
	//if (prevDaytime != m_daytime)
	//{
	//	INFO_LOG_ENGINE("Daytime = ", m_daytime, " at in-game time clock = ", m_timeOfDay);
	//}
	//DEBUG_LOG_ENGINE("Sun azimuth = ", m_sunAzimuth.GetAngleInDegrees());
}

Utility::Timing::Daytime Engine::CoreEngine::GetCurrentDaytime(Math::Real& daytimeTransitionFactor) const
{
	switch (m_daytime)
	{
	case Utility::Timing::NIGHT:
		daytimeTransitionFactor = REAL_ZERO;
		break;
	case Utility::Timing::DAY:
		daytimeTransitionFactor = REAL_ZERO; // TODO: Check if this is correct
		break;
	case Utility::Timing::BEFORE_DAWN:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	case Utility::Timing::SUNRISE:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_THIRD_ELEVATION_LEVEL.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	case Utility::Timing::SUNSET:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_THIRD_ELEVATION_LEVEL.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	case Utility::Timing::AFTER_DUSK:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	default:
		ERROR_LOG_ENGINE("Incorrect daytime: ", m_daytime);
	}
	return m_daytime;
}

void Engine::CoreEngine::AddLight(Rendering::Lighting::BaseLight* light)
{
	m_renderer->AddLight(light);
}

void Engine::CoreEngine::AddCamera(Rendering::CameraBase* camera)
{
	m_renderer->AddCamera(camera);
	//m_game.AddCamera(camera);
}

void Engine::CoreEngine::AddSkyboxNode(GameNode* skyboxNode)
{
	m_game->AddSkyboxNode(skyboxNode);
	//m_renderer->AddSkyboxNode(skyboxNode);
}

void Engine::CoreEngine::AddWaterNode(GameNode* waterNode)
{
	//m_game.AddWaterNode(waterNode);
	//m_renderer->AddWaterNode(waterNode);
}

void Engine::CoreEngine::AddTerrainNode(GameNode* terrainNode)
{
	m_game->AddTerrainNode(terrainNode);
	//m_renderer->AddTerrainNode(terrainNode);
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
	TwAddVarRW(coreEnginePropertiesBar, "clockSpeed", TW_TYPE_REAL, &m_clockSpeed, " label='Clock speed' ");
	TwAddVarRW(coreEnginePropertiesBar, "timeOfDay", TW_TYPE_REAL, &m_timeOfDay, " label='Time of day' ");

	TwEnumVal daytimeEV[] = { { Utility::Timing::NIGHT, "Night" }, { Utility::Timing::BEFORE_DAWN, "Before dawn" }, { Utility::Timing::SUNRISE, "Sunrise" },
		{ Utility::Timing::DAY, "Day" }, { Utility::Timing::SUNSET, "Sunset" }, { Utility::Timing::AFTER_DUSK, "After dusk" } };
	TwType daytimeType = TwDefineEnum("Daytime", daytimeEV, 6);
	TwAddVarRW(coreEnginePropertiesBar, "daytime", daytimeType, &m_daytime, " label='Daytime' ");

	TwAddVarRW(coreEnginePropertiesBar, "sunElevation", Rendering::angleType, &m_sunElevation, " label='Sun elevation' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunAzimuth", Rendering::angleType, &m_sunAzimuth, " label='Sun azimuth' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunFirstElevationLevel", Rendering::angleType, &M_FIRST_ELEVATION_LEVEL, " label='First elevation level' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunSecondElevationLevel", Rendering::angleType, &M_SECOND_ELEVATION_LEVEL, " label='Second elevation level' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunThirdElevationLevel", Rendering::angleType, &M_THIRD_ELEVATION_LEVEL, " label='Third elevation level' ");

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

//#ifdef CALCULATE_RENDERING_STATS
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