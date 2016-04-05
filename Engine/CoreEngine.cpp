#include "StdAfx.h"
#include "CoreEngine.h"
#include "tinythread.h"

#include "Utility\ILogger.h"
#include "Utility\Config.h"

#include "Math\FloatingPoint.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
//#include <GLFW\glfw3.h>

using namespace Engine;
using namespace Utility;
using namespace std;

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

CoreEngine* CoreEngine::s_coreEngine = NULL;

/* static */ CoreEngine* CoreEngine::GetCoreEngine()
{
	return s_coreEngine;
}

/* static */ void CoreEngine::ErrorCallback(int errorCode, const char* description)
{
	GetCoreEngine()->ErrorCallbackEvent(errorCode, description);
}

/* static */ void CoreEngine::WindowCloseEventCallback(GLFWwindow* window)
{
	GetCoreEngine()->CloseWindowEvent(window);
}

/* static */ void CoreEngine::WindowResizeCallback(GLFWwindow* window, int width, int height)
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

/* static */ void CoreEngine::KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	DEBUG_LOG("Key event callback (key = %d; scancode = %d; action = %d; mods = %d", key, scancode, action, mods);
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

/* static */ void CoreEngine::MouseEventCallback(GLFWwindow* window, int button, int action, int mods)
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

/* static */ void CoreEngine::MousePosCallback(GLFWwindow* window, double xPos, double yPos)
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

/* static */ void CoreEngine::ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset)
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

CoreEngine::CoreEngine(int width, int height, const char* title, int maxFrameRate, GameManager& game, const std::string& shadersDirectory /* = "..\\Shaders\\" */,
	const std::string& modelsDirectory /* = "..\\Models\\" */, const std::string& texturesDirectory /* = "..\\Textures\\" */,
	const std::string& fontsDirectory /* = "..\\Fonts\\" */) :
	m_window(NULL),
	m_threadWindow(NULL),
	m_isRunning(false),
	m_windowWidth(width),
	m_windowHeight(height),
	m_windowTitle(title),
	m_frameTime(1.0f / maxFrameRate),
	m_game(game),
	m_audioEngine(NULL),
	m_physicsEngine(NULL),
	m_renderer(NULL),
	LATITUDE(GET_CONFIG_VALUE("latitude", 52.0f)),
	LONGITUDE(GET_CONFIG_VALUE("longitude", -16.0f)),
	TROPIC_OF_CANCER_SINUS(0.39794863131f),
	SECONDS_PER_MINUTE(60),
	SECONDS_PER_HOUR(3600),
	SECONDS_PER_DAY(86400),
	DAYS_PER_YEAR(365),
	m_dayNumber(GET_CONFIG_VALUE("startingDayNumber", 172)),
	m_timeOfDay(GET_CONFIG_VALUE("startingTimeOfDay", REAL_ZERO)),
	m_shadersDirectory(shadersDirectory),
	m_modelsDirectory(modelsDirectory),
	m_texturesDirectory(texturesDirectory),
	m_fontsDirectory(fontsDirectory)
#ifdef CALCULATE_RENDERING_STATS
	,m_countStats1(0),
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
	M_FIRST_ELEVATION_LEVEL(GET_CONFIG_VALUE("sunlightFirstElevationLevel", -REAL_ONE)),
	M_SECOND_ELEVATION_LEVEL(GET_CONFIG_VALUE("sunlightSecondElevationLevel", REAL_ZERO)),
	M_THIRD_ELEVATION_LEVEL(GET_CONFIG_VALUE("sunlightThirdElevationLevel", REAL_ONE)),
	m_clockSpeed(GET_CONFIG_VALUE("clockSpeed", REAL_ONE))
{
	NOTICE_LOG("Main application construction started");
#ifdef CALCULATE_RENDERING_STATS
	m_timer.Start();
#endif
	START_PROFILING;

	if (s_coreEngine != NULL)
	{
		ERROR_LOG("Constructor called when a singleton instance of MainApp class has already been created");
		SAFE_DELETE(s_coreEngine);
	}
	s_coreEngine = this;

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

	NOTICE_LOG("Main application construction finished");
}


CoreEngine::~CoreEngine(void)
{
	DEBUG_LOG("Core engine destruction started");

	/* ==================== Printing stats begin ==================== */
#ifdef CALCULATE_RENDERING_STATS
	INFO_LOG("The region #1 (Time calculating) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", m_countStats1, m_timeSum1, m_timeSum1 / m_countStats1, m_minMaxTime1.ToString().c_str());
	INFO_LOG("The region #2 was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", m_countStats2, m_timeSum2, m_timeSum2 / m_countStats2, m_minMaxTime2.ToString().c_str());
	INFO_LOG("\t The region #2_1 (Polling events) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", m_countStats2_1, m_timeSum2_1, m_timeSum2_1 / m_countStats2_1, m_minMaxTime2_1.ToString().c_str());
	INFO_LOG("\t The region #2_2 (Game input processing) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", m_countStats2_2, m_timeSum2_2, m_timeSum2_2 / m_countStats2_2, m_minMaxTime2_2.ToString().c_str());
	INFO_LOG("\t The region #2_3 (Game updating) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", m_countStats2_3, m_timeSum2_3, m_timeSum2_3 / m_countStats2_3, m_minMaxTime2_3.ToString().c_str());
	INFO_LOG("The region #3 (Rendering) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", m_countStats3, m_timeSum3, m_timeSum3 / m_countStats3, m_minMaxTime3.ToString().c_str());
	INFO_LOG("Rendering step performed %d times", m_renderingRequiredCount);
	INFO_LOG("Rendering step omitted %d times", m_renderingNotRequiredCount);

	m_timer.Stop();
	STATS_STORAGE.PrintReport(m_timer.GetTimeSpan(Utility::Timing::SECOND));

	//Math::Real minSpf, maxSpf, stdDevSpf;
	Math::Real meanSpf = m_stats.CalculateMean(Math::Statistics::SPF);
	Math::Real medianSpf = m_stats.CalculateMedian(Math::Statistics::SPF);
	INFO_LOG("SPF (Seconds Per Frame) statistics during gameplay:\nSamples =\t%d\nAverage SPF =\t%.3f [ms]\nMedian SPF =\t%.3f [ms]", m_stats.Size(), meanSpf, medianSpf);
	//INFO_LOG("SPF (Seconds Per Frame) statistics during gameplay:\nSamples =\t%d\nAverage SPF =\t%.3f [ms]", m_stats.Size(), meanSpf);
#endif
	/* ==================== Printing stats end ==================== */	

	// TODO: Expand this with additional resources deallocation
	// SAFE_DELETE(m_game);
	SAFE_DELETE(m_audioEngine);
	SAFE_DELETE(m_physicsEngine);
	SAFE_DELETE(m_renderer);
	glfwTerminate(); // Terminate GLFW
	NOTICE_LOG("Core engine destruction finished");

	ILogger::GetLogger().ResetConsoleColor();
	std::cout << "Bye!" << std::endl;
}

void CoreEngine::CreateAudioEngine()
{
	m_audioEngine = new Audio::AudioEngine();
	CHECK_CONDITION_EXIT(m_audioEngine != NULL, Utility::Critical, "Failed to create an audio engine.");
}

void CoreEngine::CreatePhysicsEngine()
{
	m_physicsEngine = new Physics::PhysicsEngine();

	CHECK_CONDITION_EXIT(m_physicsEngine != NULL, Utility::Critical, "Failed to create a physics engine.");
}

void CoreEngine::CreateRenderer(int width, int height, const std::string& title)
{
	START_PROFILING;
	InitGraphics(width, height, title);
	Rendering::InitGraphics(width, height);

	glfwSetErrorCallback(&CoreEngine::ErrorCallback);
	//DEBUG_LOG("Thread window address: %p", threadWindow);
	m_renderer = new Rendering::Renderer(width, height);

	CHECK_CONDITION_EXIT(m_renderer != NULL, Utility::Critical, "Failed to create a renderer.");
	STOP_PROFILING;
}

void CoreEngine::InitGraphics(int width, int height, const std::string& title)
{
	InitGlfw(width, height, title);
	InitGlew();
	SetCallbacks();
}

void CoreEngine::InitGlfw(int width, int height, const std::string& title)
{
	DEBUG_LOG("Initializing GLFW started");
	CHECK_CONDITION_EXIT_ALWAYS(glfwInit(), Utility::Critical, "Failed to initialize GLFW.");

	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	m_threadWindow = glfwCreateWindow(1, 1, "Thread Window", NULL, NULL);
	if (m_threadWindow == NULL)
	{
		CRITICAL_LOG("Failed to create GLFW thread window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	int antiAliasingSamples = GET_CONFIG_VALUE("antiAliasingSamples", 4); /* 4x anti-aliasing by default */
	Rendering::Aliasing::AntiAliasingMethod antiAliasingMethod = Rendering::Aliasing::NONE;
	switch (antiAliasingMethod)
	{
	case Rendering::Aliasing::NONE:
		/**
		* TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		* Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		*/
		glfwWindowHint(GLFW_SAMPLES, 0);
		INFO_LOG("No anti-aliasing algorithm chosen");
		break;
	case Rendering::Aliasing::FXAA:
		/**
		* TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		* Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		*/
		glfwWindowHint(GLFW_SAMPLES, 0);
		INFO_LOG("FXAA anti-aliasing algorithm chosen");
		break;
	case Rendering::Aliasing::MSAA:
		glfwWindowHint(GLFW_SAMPLES, antiAliasingSamples);
		INFO_LOG("%dxMSAA anti-aliasing algorithm chosen", antiAliasingSamples);
		break;
	default:
		WARNING_LOG("Unknown anti-aliasing algorithm chosen. Default %dxMSAA algorithm chosen", antiAliasingSamples);
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
	bool fullscreenEnabled = GET_CONFIG_VALUE("fullscreenEnabled", false);
	if (fullscreenEnabled)
	{
		monitor = glfwGetPrimaryMonitor();
	}
	m_window = glfwCreateWindow(width, height, title.c_str(), monitor, m_threadWindow); // Open a window and create its OpenGL context
	if (m_window == NULL)
	{
		CRITICAL_LOG("Failed to create GLFW main window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we can capture the escape key being pressed below
	glfwSetCursorPos(m_window, width / 2, height / 2); // Set cursor position to the middle point
													 //glfwSwapInterval(1);
	glfwSetTime(REAL_ZERO);
	DEBUG_LOG("Initializing GLFW finished successfully");
}

void CoreEngine::InitGlew()
{
	INFO_LOG("Initializing GLEW started");
	glewExperimental = true; // Needed in core profile
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		ERROR_LOG("Error while initializing GLEW: %s", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	if (GLEW_VERSION_2_0)
	{
		DEBUG_LOG("OpenGL 2.0 supported");
	}
	else
	{
		ERROR_LOG("Initializing GLEW failed. OpenGL 2.0 NOT supported");
		exit(EXIT_FAILURE);
	}

	INFO_LOG("Using GLEW version %s", glewGetString(GLEW_VERSION));
	//CheckErrorCode(__FUNCTION__, "Initializing GLEW");
}

void CoreEngine::SetCallbacks()
{
	CHECK_CONDITION_EXIT_ALWAYS(m_window != NULL, Critical, "Setting GLFW callbacks failed. The window is NULL.");
	glfwSetWindowCloseCallback(m_window, &CoreEngine::WindowCloseEventCallback);
	glfwSetWindowSizeCallback(m_window, &CoreEngine::WindowResizeCallback);
	glfwSetKeyCallback(m_window, &CoreEngine::KeyEventCallback);
	//glfwSetCharCallback(m_window, &CoreEngine::CharEventCallback);
	//glfwSetMousePosCallback(m_window, &CoreEngine::MouseMotionCallback);
	glfwSetCursorPosCallback(m_window, &CoreEngine::MousePosCallback);
	glfwSetMouseButtonCallback(m_window, &CoreEngine::MouseEventCallback);
	glfwSetScrollCallback(m_window, &CoreEngine::ScrollEventCallback);
}

void CoreEngine::Start()
{
	START_PROFILING;
	if (m_isRunning)
	{
		WARNING_LOG("The core engine instance is already running");
		return;
	}
	NOTICE_LOG("The core engine started");

	Run();
	STOP_PROFILING;
}

void CoreEngine::Stop()
{
	START_PROFILING;
	if (!m_isRunning)
	{
		WARNING_LOG("The core engine instance is not running");
		return;
	}
	
	m_isRunning = false;
	RequestWindowClose();
	CHECK_CONDITION(!m_isRunning, Utility::Warning, "Stopping the core engine is not possible as it is simply not running at the moment.");
	NOTICE_LOG("The core engine has stopped");

	// Just for checking whether time calculation is performed correctly
	//LARGE_INTEGER frequency; // ticks per second
	//QueryPerformanceFrequency(&frequency); // get ticks per second;
	//LARGE_INTEGER t1, t2;
	//QueryPerformanceCounter(&t1);
	//tthread::this_thread::sleep_for(tthread::chrono::seconds(1));
	//QueryPerformanceCounter(&t2);
	//double elapsedTime = static_cast<double>(static_cast<Math::Real>(1000000.0f) * (t2.QuadPart - t1.QuadPart)) / frequency.QuadPart; // in [us]
	//INFO_LOG("Elapsed time = %f [us]", elapsedTime);
	STOP_PROFILING;
}

void CoreEngine::Run()
{
	START_PROFILING;
	const Rendering::Text::Font font(GET_CONFIG_VALUE_STR("fontTextureAtlas", "segoe.png"), GET_CONFIG_VALUE_STR("fontMetaData", "segoe.fnt"));
	const int THREAD_SLEEP_TIME = GET_CONFIG_VALUE("threadSleepTime", 10);
	
	CHECK_CONDITION(!m_isRunning, Utility::Warning, "According to the core engine the game is already running.");

#ifdef ANT_TWEAK_BAR_ENABLED
	Rendering::InitializeTweakBars();
	InitializeTweakBars();
#endif

	m_isRunning = true;

#ifdef COUNT_FPS
	Math::Real fpsSample = static_cast<Math::Real>(GET_CONFIG_VALUE("FPSsample", REAL_ONE)); // represents the time after which FPS value is calculated and logged
	int framesCount = 0;
	Math::Real frameTimeCounter = REAL_ZERO;
	int fps = 0;
	Math::Real spf = REAL_ZERO;
#endif

	Math::Real unprocessingTime = REAL_ZERO; // used to cap the FPS when it gets too high
	Math::Real previousTime = GetTime();
	int inGameHours, inGameMinutes, inGameSeconds;

#ifdef CALCULATE_RENDERING_STATS
	//LARGE_INTEGER t1, t2, innerT1, innerT2; // ticks
#endif
	while (m_isRunning)
	{
		/* ==================== REGION #1 begin ====================*/
		START_TIMER(timer);
		bool isRenderRequired = false;

		// flCurrentTime will be lying around from last frame. It's now the previous time.
		Math::Real currentTime = GetTime();
		Math::Real passedTime = currentTime - previousTime;
		
		if (m_game.IsInGameTimeCalculationEnabled())
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
			DEBUG_LOG("FPS = %5d\t Average time per frame = %.3f [ms]", fps, spf);
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
			//m_game.Input(m_frameTime);
			//STOP_TIMER(innerTimer, m_countStats2_2, m_minMaxTime2_2, m_timeSum2_2);
			RESET_TIMER(innerTimer);
			m_game.Update(m_frameTime);
			STOP_TIMER(innerTimer, m_countStats2_2, m_minMaxTime2_2, m_timeSum2_2);
			/* ==================== REGION #2_2 end ====================*/
			
			//Input::Update();
			
			/* ==================== REGION #2_3 begin ====================*/
			RESET_TIMER(innerTimer);
			m_physicsEngine->Simulate(m_frameTime);
			STOP_TIMER(innerTimer, m_countStats2_3, m_minMaxTime2_3, m_timeSum2_3);
			/* ==================== REGION #2_3 end ====================*/

			/* ==================== Switching the game state if necessary begin ==================== */
			m_game.PerformStateTransition();
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
			m_game.Render(m_renderer);
#ifdef COUNT_FPS
			++framesCount;

#ifdef DRAW_FPS
			std::stringstream ss;
			ss << "FPS = " << fps << " SPF[ms] = " << std::setprecision(4) << spf;
			//m_renderer->RenderText(0, 570, ss.str(), 16.0f /* TODO: Never use hard-coded values! */);
			Rendering::Text::GuiText guiText("This is a GUI text", &font, 8.0f, Math::Vector2D(0.0f, 0.0f), 1.0f, false);
			//exit(EXIT_FAILURE);
			m_renderer->RenderText(guiText);
#endif
#ifdef DRAW_GAME_TIME
			if (m_game.IsInGameTimeCalculationEnabled())
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
				m_renderer->RenderText(0, 550, ss.str(), 16.0f /* TODO: Never use hard-coded values! */);
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
			//INFO_LOG("Rendering is not required. Moving on...");
			// TODO: Sleep for 1ms to prevent the thread from constant looping
			//this_thread::sleep_for(chrono::milliseconds(100));
			tthread::this_thread::sleep_for(tthread::chrono::milliseconds(THREAD_SLEEP_TIME));
#ifdef CALCULATE_RENDERING_STATS
			++m_renderingNotRequiredCount;
#endif
		}
		STOP_TIMER(timer, m_countStats3, m_minMaxTime3, m_timeSum3);
		/* ==================== REGION #3 end ====================*/
	}
}

void CoreEngine::WindowResizeEvent(GLFWwindow* window, int width, int height)
{
	m_renderer->SetWindowWidth(width);
	m_renderer->SetWindowHeight(height);
	m_game.WindowResizeEvent(width, height);
}

void CoreEngine::ErrorCallbackEvent(int errorCode, const char* description)
{
	switch (errorCode)
	{
	case GLFW_NOT_INITIALIZED:
		ERROR_LOG("GLFW has not been initialized. Error description: %s", description);
		break;
	case GLFW_NO_CURRENT_CONTEXT:
		ERROR_LOG("No context is current for this thread. Error description: %s", description);
		break;
	case GLFW_INVALID_ENUM:
		ERROR_LOG("One of the arguments to the function was an invalid enum value. Error description: %s", description);
		break;
	case GLFW_INVALID_VALUE:
		ERROR_LOG("One of the arguments to the function was an invalid value. Error description: %s", description);
		break;
	case GLFW_OUT_OF_MEMORY:
		ERROR_LOG("A memory allocation failed. Error description: %s", description);
		break;
	case GLFW_API_UNAVAILABLE:
		ERROR_LOG("GLFW could not find support for the requested client API on the system. Error description: %s", description);
		break;
	case GLFW_VERSION_UNAVAILABLE:
		ERROR_LOG("The requested OpenGL or OpenGL ES version is not available. Error description: %s", description);
		break;
	case GLFW_PLATFORM_ERROR:
		ERROR_LOG("A platform-specific error occurred that does not match any of the more specific categories. Error description: %s", description);
		break;
	case GLFW_FORMAT_UNAVAILABLE:
		ERROR_LOG("The requested format is not supported or available. Error description: %s", description);
		break;
	default:
		ERROR_LOG("Unknown GLFW error event occurred with code %d and message: Error description: %s", errorCode, description);
	}
	exit(EXIT_FAILURE);
}

void CoreEngine::CloseWindowEvent(GLFWwindow* window)
{
	m_game.CloseWindowEvent();
}

void CoreEngine::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	m_game.KeyEvent(key, scancode, action, mods);
}

void CoreEngine::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	m_game.MouseButtonEvent(button, action, mods);
}

void CoreEngine::MousePosEvent(GLFWwindow* window, double xPos, double yPos)
{
	m_game.MousePosEvent(xPos, yPos);
}

void CoreEngine::ScrollEvent(GLFWwindow* window, double xOffset, double yOffset)
{
	m_game.ScrollEvent(xOffset, yOffset);
}

void CoreEngine::ConvertTimeOfDay(int& inGameHours, int& inGameMinutes, int& inGameSeconds) const
{
	ConvertTimeOfDay(m_timeOfDay, inGameHours, inGameMinutes, inGameSeconds);
}

void CoreEngine::ConvertTimeOfDay(Math::Real timeOfDay, int& inGameHours, int& inGameMinutes, int& inGameSeconds) const
{
	inGameHours = static_cast<int>(timeOfDay) / SECONDS_PER_HOUR;
	Math::Real temp = fmod(m_timeOfDay, static_cast<Math::Real>(SECONDS_PER_HOUR));
	inGameMinutes = static_cast<int>(temp) / SECONDS_PER_MINUTE;
	inGameSeconds = static_cast<int>(fmod(temp, static_cast<Math::Real>(SECONDS_PER_MINUTE)));
}

size_t CoreEngine::GetCurrentCameraIndex() const
{
	CHECK_CONDITION_EXIT(m_renderer != NULL, Critical, "Cannot get the current camera index. The renderer does not exist.");
	return m_renderer->GetCurrentCameraIndex();
}

size_t CoreEngine::NextCamera() const
{
	CHECK_CONDITION_EXIT(m_renderer != NULL, Critical, "Cannot move to the next camera. The renderer does not exist.");
	return m_renderer->NextCamera();
}

size_t CoreEngine::PrevCamera() const
{
	CHECK_CONDITION_EXIT(m_renderer != NULL, Critical, "Cannot move to the previous camera. The renderer does not exist.");
	return m_renderer->PrevCamera();
}

void CoreEngine::PollEvents()
{
	glfwPollEvents();
}

/**
 * See http://www.cplusplus.com/reference/ctime/localtime/
 * http://www.cplusplus.com/reference/ctime/strftime/
 */
Math::Real CoreEngine::GetCurrentLocalTime() const
{
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	int result = SECONDS_PER_HOUR * timeinfo.tm_hour + SECONDS_PER_MINUTE * timeinfo.tm_min + timeinfo.tm_sec;
	if (result > SECONDS_PER_DAY)
	{
		ERROR_LOG("Incorrect local time");
		// result = REAL_ZERO;
		result -= SECONDS_PER_DAY;
	}
	return static_cast<Math::Real>(result);
}

Math::Real CoreEngine::GetTime() const
{
	return static_cast<Math::Real>(glfwGetTime());
	//return Time(glfwGetTime());

	//return Time::Now();
}

void CoreEngine::ClearScreen() const
{
}

void CoreEngine::SetCursorPos(Math::Real xPos, Math::Real yPos)
{
	if (m_renderer == NULL)
	{
		CRITICAL_LOG("Cannot set cursor position. The rendering engine is NULL.");
		return;
	}
	glfwSetCursorPos(m_window, xPos, yPos);
}

void CoreEngine::CentralizeCursor()
{
	if (m_renderer == NULL)
	{
		CRITICAL_LOG("Cannot set cursor position. The rendering engine is NULL.");
		return;
	}
	glfwSetCursorPos(m_window, static_cast<Math::Real>(m_windowWidth) / 2, static_cast<Math::Real>(m_windowHeight) / 2);
}

void CoreEngine::CalculateSunElevationAndAzimuth()
{
	const int timeGMTdifference = 1;
	
	const Math::Angle b(0.9863014f * (m_dayNumber - 81)); // 0,98630136986301369863013698630137 = 360 / 365
	const Math::Real bSin = b.Sin();
	const Math::Real bCos = b.Cos();

	const Math::Real equationOfTime = 19.74f * bSin * bCos - 7.53f * bCos - 1.5f * bSin; // EoT
	const Math::Real declinationSin = TROPIC_OF_CANCER_SINUS * bSin;
	const Math::Angle declinationAngle(asin(declinationSin), Math::Unit::RADIAN);
	//DEBUG_LOG("Declination in degrees = %.5f", declinationAngle.GetAngleInDegrees());

	const Math::Real timeCorrectionInSeconds = 60.0f * (4.0f * (LONGITUDE.GetAngleInDegrees() - 15.0f * timeGMTdifference) + equationOfTime);
	const Math::Real localSolarTime = m_timeOfDay + timeCorrectionInSeconds;
	//DEBUG_LOG("Time correction in seconds = %.5f", timeCorrectionInSeconds);
	//DEBUG_LOG("Local time = %.5f\tLocal solar time = %.5f", m_timeOfDay, localSolarTime);
	
	const Math::Angle hourAngle(15.0f * (localSolarTime - 12 * SECONDS_PER_HOUR) / SECONDS_PER_HOUR);
	//DEBUG_LOG("Hour angle = %.5f", hourAngle.GetAngleInDegrees());

	const Math::Real sunElevationSin = declinationSin * LATITUDE.Sin() + declinationAngle.Cos() * LATITUDE.Cos() * hourAngle.Cos();
	m_sunElevation.SetAngleInRadians(asin(sunElevationSin));
	//DEBUG_LOG("Sun elevation = %.5f", m_sunElevation.GetAngleInDegrees());

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
	//	INFO_LOG("Daytime = %d at in-game time clock %.1f", m_daytime, m_timeOfDay);
	//}
	//DEBUG_LOG("Sun azimuth = %.5f", m_sunAzimuth.GetAngleInDegrees());
}

Utility::Timing::Daytime CoreEngine::GetCurrentDaytime(Math::Real& daytimeTransitionFactor) const
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
		ERROR_LOG("Incorrect daytime %d", m_daytime);
	}
	return m_daytime;
}

void CoreEngine::AddLight(Rendering::Lighting::BaseLight* light)
{
	m_renderer->AddLight(light);
}

void CoreEngine::AddCamera(Rendering::CameraBase* camera)
{
	m_renderer->AddCamera(camera);
	//m_game.AddCamera(camera);
}

void CoreEngine::AddSkyboxNode(GameNode* skyboxNode)
{
	m_game.AddSkyboxNode(skyboxNode);
	//m_renderer->AddSkyboxNode(skyboxNode);
}

void CoreEngine::AddWaterNode(GameNode* waterNode)
{
	//m_game.AddWaterNode(waterNode);
	//m_renderer->AddWaterNode(waterNode);
}

void CoreEngine::AddTerrainNode(GameNode* terrainNode)
{
	m_game.AddTerrainNode(terrainNode);
	//m_renderer->AddTerrainNode(terrainNode);
}

void CoreEngine::AddBillboardNode(GameNode* billboardNode)
{
	//m_renderer->AddBillboardNode(billboardNode);
}

void CoreEngine::AddPhysicsObject(Physics::PhysicsObject* physicsObject)
{
	m_physicsEngine->AddPhysicsObject(physicsObject);
}

#ifdef ANT_TWEAK_BAR_ENABLED
void CoreEngine::InitializeTweakBars()
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
		{ Utility::Timing::DAY, "Day" }, { Utility::Timing::SUNSET, "Sunset" }, { Utility::Timing::AFTER_DUSK, "After dusk" }};
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

void CoreEngine::InitializeGameTweakBars()
{
	m_game.InitializeTweakBars();
	m_renderer->InitializeTweakBars();
}
#endif

//#ifdef CALCULATE_RENDERING_STATS
//Math::Real CoreEngine::CalculateAverageSpf(Math::Real& minSpf, Math::Real& maxSpf, Math::Real& stdDev) const
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