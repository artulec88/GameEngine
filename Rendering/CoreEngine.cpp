#include "StdAfx.h"
#include "CoreEngine.h"
#include "Input.h"
#include "tinythread.h"

#include "Utility\ILogger.h"
#include "Utility\Config.h"

#include "Math\FloatingPoint.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
//#include <GLFW\glfw3.h>

using namespace Rendering;
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
	const std::string& modelsDirectory /* = "..\\Models\\" */, const std::string& texturesDirectory /* = "..\\Textures\\" */) :
	m_isRunning(false),
	m_windowWidth(width),
	m_windowHeight(height),
	m_windowTitle(title),
	m_frameTime(1.0f / maxFrameRate),
	m_game(game),
	m_renderer(NULL),
	m_fpsTextRenderer(NULL),
	m_quitGameCommand(m_game),
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
#ifdef CALCULATE_RENDERING_STATS
	m_countStats1(0),
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

	CreateRenderer(width, height, title);

	m_fpsTextRenderer = new TextRenderer(new Texture("Holstein.tga", GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, true, GL_COLOR_ATTACHMENT0), 16.0f /* TODO: Configurable font size */);

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
	SAFE_DELETE(m_renderer);
	SAFE_DELETE(m_fpsTextRenderer);

	NOTICE_LOG("Core engine destruction finished");

	ILogger::GetLogger().ResetConsoleColor();
	std::cout << "Bye!" << std::endl;
}

void CoreEngine::CreateRenderer(int width, int height, const std::string& title)
{
	START_PROFILING;
	GLFWwindow* threadWindow = NULL;
	GLFWwindow* window = Rendering::InitGraphics(width, height, title, threadWindow);
	//DEBUG_LOG("Thread window address: %p", threadWindow);
	m_renderer = new Renderer(window, threadWindow);

	CHECK_CONDITION_EXIT(m_renderer != NULL, Utility::Critical, "Failed to create a renderer.");
	STOP_PROFILING;
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
	Stop();
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
				m_timeOfDay -= SECONDS_PER_DAY;
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
			if (m_renderer->IsCloseRequested())
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
			RESET_TIMER(innerTimer);
			m_game.Input(m_frameTime);
			STOP_TIMER(innerTimer, m_countStats2_2, m_minMaxTime2_2, m_timeSum2_2);
			/* ==================== REGION #2_2 end ====================*/
			
			//Input::Update();
			
			/* ==================== REGION #2_3 begin ====================*/
			RESET_TIMER(innerTimer);
			m_game.Update(m_frameTime);
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
			m_fpsTextRenderer->DrawString(0, 570, ss.str(), m_renderer);
#endif
#ifdef DRAW_GAME_TIME
			if (m_game.IsInGameTimeCalculationEnabled())
			{
#ifndef DRAW_FPS
				std::stringstream ss;
#endif
				ss.str(std::string());
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
				m_fpsTextRenderer->DrawString(0, 550, ss.str(), m_renderer);
			}
#endif
#endif
#ifdef ANT_TWEAK_BAR_ENABLED
			TwDraw();
#endif
			m_renderer->SwapBuffers();
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
	m_game.WindowResizeEvent(window, width, height);
}

void CoreEngine::CloseWindowEvent(GLFWwindow* window)
{
	m_game.CloseWindowEvent(window);
}

void CoreEngine::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	m_game.KeyEvent(window, key, scancode, action, mods);
}

void CoreEngine::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	m_game.MouseButtonEvent(window, button, action, mods);
}

void CoreEngine::MousePosEvent(GLFWwindow* window, double xPos, double yPos)
{
	m_game.MousePosEvent(window, xPos, yPos);
}

void CoreEngine::ScrollEvent(GLFWwindow* window, double xOffset, double yOffset)
{
	m_game.ScrollEvent(window, xOffset, yOffset);
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
	m_renderer->SetCursorPos(xPos, yPos);
}

void CoreEngine::CentralizeCursor()
{
	if (m_renderer == NULL)
	{
		CRITICAL_LOG("Cannot set cursor position. The rendering engine is NULL.");
		return;
	}
	m_renderer->SetCursorPos(static_cast<Math::Real>(m_windowWidth) / 2, static_cast<Math::Real>(m_windowHeight) / 2);
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

	GameTime::Daytime prevDaytime = m_daytime;
	if (m_sunElevation < M_FIRST_ELEVATION_LEVEL)
	{
		m_daytime = Rendering::GameTime::NIGHT;
	}
	else if (m_sunElevation < M_SECOND_ELEVATION_LEVEL)
	{
		m_daytime = (isAfternoon) ? Rendering::GameTime::AFTER_DUSK : Rendering::GameTime::BEFORE_DAWN;
	}
	else if (m_sunElevation < M_THIRD_ELEVATION_LEVEL)
	{
		m_daytime = (isAfternoon) ? Rendering::GameTime::SUNSET : Rendering::GameTime::SUNRISE;
	}
	else
	{
		m_daytime = Rendering::GameTime::DAY;
	}
	//if (prevDaytime != m_daytime)
	//{
	//	INFO_LOG("Daytime = %d at in-game time clock %.1f", m_daytime, m_timeOfDay);
	//}
	//DEBUG_LOG("Sun azimuth = %.5f", m_sunAzimuth.GetAngleInDegrees());
}

Rendering::GameTime::Daytime CoreEngine::GetCurrentDaytime(Math::Real& daytimeTransitionFactor) const
{
	switch (m_daytime)
	{
	case Rendering::GameTime::NIGHT:
	case Rendering::GameTime::DAY:
		daytimeTransitionFactor = REAL_ZERO;
		break;
	case Rendering::GameTime::BEFORE_DAWN:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	case Rendering::GameTime::SUNRISE:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_THIRD_ELEVATION_LEVEL.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	case Rendering::GameTime::SUNSET:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_THIRD_ELEVATION_LEVEL.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	case Rendering::GameTime::AFTER_DUSK:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	default:
		ERROR_LOG("Incorrect daytime %d", m_daytime);
	}
	return m_daytime;
}

#ifdef ANT_TWEAK_BAR_ENABLED
void CoreEngine::InitializeTweakBars()
{
	AntTweakBarTypes::InitializeTweakBarTypes();

	TwWindowSize(m_windowWidth, m_windowHeight);

	TwBar* coreEnginePropertiesBar = TwNewBar("CoreEnginePropertiesBar");
	TwAddVarRW(coreEnginePropertiesBar, "windowWidth", TW_TYPE_INT32, &m_windowWidth, " label='Window width' ");
	TwAddVarRW(coreEnginePropertiesBar, "windowHeight", TW_TYPE_INT32, &m_windowHeight, " label='Window height' ");
	TwAddVarRO(coreEnginePropertiesBar, "frameTime", TW_TYPE_REAL, &m_frameTime, " label='Frame time' ");
	TwAddVarRW(coreEnginePropertiesBar, "clockSpeed", TW_TYPE_REAL, &m_clockSpeed, " label='Clock speed' ");
	TwAddVarRW(coreEnginePropertiesBar, "timeOfDay", TW_TYPE_REAL, &m_timeOfDay, " label='Time of day' ");
	
	TwEnumVal daytimeEV[] = { { Rendering::GameTime::NIGHT, "Night" }, { Rendering::GameTime::BEFORE_DAWN, "Before dawn" }, { Rendering::GameTime::SUNRISE, "Sunrise" },
		{ Rendering::GameTime::DAY, "Day" }, { Rendering::GameTime::SUNSET, "Sunset" }, { Rendering::GameTime::AFTER_DUSK, "After dusk" }};
	TwType daytimeType = TwDefineEnum("Daytime", daytimeEV, 6);
	TwAddVarRW(coreEnginePropertiesBar, "daytime", daytimeType, &m_daytime, " label='Daytime' ");
	
	TwAddVarRW(coreEnginePropertiesBar, "sunElevation", angleType, &m_sunElevation, " label='Sun elevation' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunAzimuth", angleType, &m_sunAzimuth, " label='Sun azimuth' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunFirstElevationLevel", angleType, &M_FIRST_ELEVATION_LEVEL, " label='First elevation level' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunSecondElevationLevel", angleType, &M_SECOND_ELEVATION_LEVEL, " label='Second elevation level' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunThirdElevationLevel", angleType, &M_THIRD_ELEVATION_LEVEL, " label='Third elevation level' ");
	
	TwDefine(" CoreEnginePropertiesBar refresh=0.5 ");
	//double refreshRate = 0.2;
	//TwSetParam(coreEnginePropertiesBar, NULL, "refresh", TW_PARAM_DOUBLE, 1, &refreshRate);

	//TwSetParam(coreEnginePropertiesBar, NULL, "visible", TW_PARAM_CSTRING, 1, "false"); // Hide the bar at startup
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