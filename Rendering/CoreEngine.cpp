#include "StdAfx.h"
#include "CoreEngine.h"
#include "Input.h"
#include "tinythread.h"

#include "Utility\ILogger.h"
#include "Utility\Config.h"

#include "Math\FloatingPoint.h"

#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
//#include <GLFW\glfw3.h>

using namespace Rendering;
using namespace Utility;
using namespace std;

CoreEngine* CoreEngine::s_coreEngine = NULL;

/* static */ CoreEngine* CoreEngine::GetCoreEngine()
{
	return s_coreEngine;
}

CoreEngine::CoreEngine(int width, int height, const char* title, int maxFrameRate, GameManager* game) :
	m_isRunning(false),
	m_windowWidth(width),
	m_windowHeight(height),
	m_windowTitle(title),
	m_frameTime(1.0f / maxFrameRate),
	SECONDS_PER_MINUTE(60),
	SECONDS_PER_HOUR(3600),
	SECONDS_PER_DAY(86400),
	m_timeOfDay(GET_CONFIG_VALUE("startingTimeOfDay", REAL_ZERO)),
	M_FIRST_ELEVATION_LEVEL(GET_CONFIG_VALUE("sunlightFirstElevationLevel", -REAL_ONE)),
	M_SECOND_ELEVATION_LEVEL(GET_CONFIG_VALUE("sunlightSecondElevationLevel", REAL_ZERO)),
	M_THIRD_ELEVATION_LEVEL(GET_CONFIG_VALUE("sunlightThirdElevationLevel", REAL_ONE)),
	m_clockSpeed(GET_CONFIG_VALUE("clockSpeed", REAL_ONE)),
	m_game(game),
	m_renderer(NULL),
	m_fpsTextRenderer(NULL),
	m_renderingRequiredCount(0),
	m_renderingNotRequiredCount(0)
{
	// TODO: Fix singleton initialization
	LOG(Debug, LOGPLACE, "Main application construction started");
	if (s_coreEngine != NULL)
	{
		LOG(Error, LOGPLACE, "Constructor called when a singleton instance of MainApp class has already been created");
		SAFE_DELETE(s_coreEngine);
	}
	s_coreEngine = this;
	
	m_game->SetEngine(this);

	CreateRenderer(width, height, title);

	m_fpsTextRenderer = new TextRenderer(new Texture("..\\Textures\\Holstein.tga", GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, true, GL_COLOR_ATTACHMENT0), 16.0f /* TODO: Configurable font size */);

	//while (m_timeOfDay > SECONDS_PER_DAY)
	//{
	//	m_timeOfDay -= static_cast<int>(m_timeOfDay) % SECONDS_PER_DAY;
	//}
	// return value within range [0.0; SECONDS_PER_DAY) (see http://www.cplusplus.com/reference/cmath/fmod/)
	m_timeOfDay = fmod(m_timeOfDay, SECONDS_PER_DAY);
	if (m_timeOfDay < REAL_ZERO)
	{
		m_timeOfDay = GetCurrentLocalTime();
	}

	LOG(Debug, LOGPLACE, "Main application construction finished");
}


CoreEngine::~CoreEngine(void)
{
	LOG(Debug, LOGPLACE, "Core engine destruction started");

	// TODO: Expand this with additional resources deallocation
	SAFE_DELETE(m_game);
	SAFE_DELETE(m_renderer);
	SAFE_DELETE(m_fpsTextRenderer);

	LOG(Notice, LOGPLACE, "Core engine destruction finished");
	ILogger::GetLogger().ResetConsoleColor();
	std::cout << "Bye!" << std::endl;
}

void CoreEngine::CreateRenderer(int width, int height, const std::string& title)
{
	GLFWwindow* window = Rendering::InitGraphics(width, height, title);
	m_renderer = new Renderer(window);

	if (m_renderer == NULL)
	{
		LOG(Critical, LOGPLACE, "Failed to create a Renderer");
		exit(EXIT_FAILURE);
	}

	ASSERT(m_renderer != NULL);
}

void CoreEngine::Start()
{
	if (m_isRunning)
	{
		LOG(Warning, LOGPLACE, "The core engine instance is already running");
		return;
	}
	LOG(Notice, LOGPLACE, "The core engine starts");
	Run();
}

#define MIN_MAX_STATS_COUNT 3

struct minMaxTime
{
	double minTime[MIN_MAX_STATS_COUNT];
	double maxTime[MIN_MAX_STATS_COUNT];
	void Init()
	{
		for (int i = 0; i < MIN_MAX_STATS_COUNT; ++i)
		{
			minTime[i] = 99999999.9;
			maxTime[i] = 0.0;
		}
	}
	void ProcessTime(double elapsedTime)
	{
		bool maxFound = false;
		bool minFound = false;
		for (int i = 0; i < MIN_MAX_STATS_COUNT; ++i)
		{
			if ( (minFound) && (maxFound) )
				return;
			if ( (!minFound) && (elapsedTime < minTime[i]) )
			{
				for (int j = i; j < MIN_MAX_STATS_COUNT - 1; ++j)
				{
					minTime[j + 1] = minTime[j];
				}
				minTime[i] = elapsedTime;
				minFound = true;
			}
			if ( (!maxFound) && (elapsedTime > maxTime[i]) )
			{
				for (int j = i; j < MIN_MAX_STATS_COUNT - 1; ++j)
				{
					maxTime[j + 1] = maxTime[j];
				}
				maxTime[i] = elapsedTime;
				maxFound = true;
			}
		}
	}
	std::string ToString()
	{
		std::stringstream ss("");
		ss << "minTimes = { ";
		for (int i = 0; i < MIN_MAX_STATS_COUNT; ++i)
		{
			ss << std::setprecision(2) << minTime[i] << "[us]; ";
		}
		ss << " } maxTimes = { ";
		for (int i = 0; i < MIN_MAX_STATS_COUNT; ++i)
		{
			ss << std::setprecision(2) << maxTime[i] << "[us]; ";
		}
		ss << " }";
		return ss.str();
	}
};

long countStats1 = 0;
minMaxTime minMaxTime1;
double timeSum1 = 0.0;

long countStats2 = 0;
minMaxTime minMaxTime2;
double timeSum2 = 0.0;

long countStats2_1 = 0;
minMaxTime minMaxTime2_1;
double timeSum2_1 = 0.0;

long countStats2_2 = 0;
minMaxTime minMaxTime2_2;
double timeSum2_2 = 0.0;

long countStats2_3 = 0;
minMaxTime minMaxTime2_3;
double timeSum2_3 = 0.0;

long countStats3 = 0;
minMaxTime minMaxTime3;
double timeSum3 = 0.0;

void CoreEngine::Stop()
{
	if (!m_isRunning)
	{
		LOG(Warning, LOGPLACE, "The core engine instance is not running");
		return;
	}
	
	m_isRunning = false;
	ASSERT(!m_isRunning);
	LOG(Notice, LOGPLACE, "The core engine has stopped");
	
	/* ==================== Printing stats begin ==================== */
	LOG(Info, LOGPLACE, "The region #1 (Time calculating) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats1, timeSum1, timeSum1 / countStats1, minMaxTime1.ToString().c_str());
	LOG(Info, LOGPLACE, "The region #2 was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats2, timeSum2, timeSum2 / countStats2, minMaxTime2.ToString().c_str());
	LOG(Info, LOGPLACE, "\t The region #2_1 (Polling events) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats2_1, timeSum2_1, timeSum2_1 / countStats2_1, minMaxTime2_1.ToString().c_str());
	LOG(Info, LOGPLACE, "\t The region #2_2 (Game input processing) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats2_2, timeSum2_2, timeSum2_2 / countStats2_2, minMaxTime2_2.ToString().c_str());
	LOG(Info, LOGPLACE, "\t The region #2_3 (Game updating) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats2_3, timeSum2_3, timeSum2_3 / countStats2_3, minMaxTime2_3.ToString().c_str());
	LOG(Info, LOGPLACE, "The region #3 (Rendering) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats3, timeSum3, timeSum3 / countStats3, minMaxTime3.ToString().c_str());
	LOG(Info, LOGPLACE, "Rendering step performed %d times", m_renderingRequiredCount);
	LOG(Info, LOGPLACE, "Rendering step omitted %d times", m_renderingNotRequiredCount);
	/* ==================== Printing stats end ==================== */
}

void CoreEngine::Run()
{
	const int THREAD_SLEEP_TIME = GET_CONFIG_VALUE("threadSleepTime", 10);
	const Math::Real ONE_MILLION = static_cast<Math::Real>(1000000.0);

	minMaxTime1.Init();
	minMaxTime2.Init();
	minMaxTime2_1.Init();
	minMaxTime2_2.Init();
	minMaxTime2_3.Init();
	minMaxTime3.Init();
	
	LOG(Notice, LOGPLACE, "The game started running");
	ASSERT(!isRunning);

	m_game->Init();
#ifdef ANT_TWEAK_BAR_ENABLED
	Rendering::InitializeTweakBars();
	m_renderer->InitializeTweakBars();
	m_game->InitializeTweakBars();
	InitializeTweakBars();
#endif

	m_isRunning = true;

#ifdef COUNT_FPS
	Math::Real fpsSample = static_cast<Math::Real>(GET_CONFIG_VALUE("FPSsample", REAL_ONE)); // represents the time after which FPS value is calculated and logged
	int framesCount = 0;
	Math::Real frameTimeCounter = REAL_ZERO;
	int fps;
	Math::Real spf;
#endif

	Math::Real unprocessingTime = REAL_ZERO; // used to cap the FPS when it gets too high
	Math::Real previousTime = GetTime();
	int inGameHours, inGameMinutes, inGameSeconds;

	LARGE_INTEGER frequency; // ticks per second
	QueryPerformanceFrequency(&frequency); // get ticks per second;
	LARGE_INTEGER t1, t2, innerT1, innerT2; // ticks

	while (m_isRunning)

	{
		/* ==================== REGION #1 begin ====================*/
		QueryPerformanceCounter(&t1); // start timer
		bool isRenderRequired = false;

		// flCurrentTime will be lying around from last frame. It's now the previous time.
		Math::Real currentTime = GetTime();
		Math::Real passedTime = currentTime - previousTime;
		
		if (m_game->IsInGameTimeCalculationEnabled())
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
			LOG(Debug, LOGPLACE, "FPS = %5d\t Average time per frame = %.3f [ms]", fps, spf);
			framesCount = 0;
			frameTimeCounter = REAL_ZERO;
		}
#endif
		QueryPerformanceCounter(&t2); // stop timer
		countStats1++;
		double elapsedTime = static_cast<double>(ONE_MILLION * (t2.QuadPart - t1.QuadPart)) / frequency.QuadPart; // in [us]
		minMaxTime1.ProcessTime(elapsedTime);
		timeSum1 += elapsedTime;
		/* ==================== REGION #1 end ====================*/

		/* ==================== REGION #2 begin ====================*/
		QueryPerformanceCounter(&t1); // start timer
		while (unprocessingTime > m_frameTime)
		{
			//previousTime = GetTime();
			isRenderRequired = true;
			if (m_renderer->IsCloseRequested())
			{
				Stop();
				return;
			}
			/* ==================== REGION #2_1 begin ====================*/
			QueryPerformanceCounter(&innerT1);
			PollEvents();
			QueryPerformanceCounter(&innerT2);
			countStats2_1++;
			elapsedTime = static_cast<double>(ONE_MILLION * (innerT2.QuadPart - innerT1.QuadPart)) / frequency.QuadPart; // in [us]
			minMaxTime2_1.ProcessTime(elapsedTime);
			timeSum2_1 += elapsedTime;
			/* ==================== REGION #2_1 end ====================*/
			
			/* ==================== REGION #2_2 begin ====================*/
			QueryPerformanceCounter(&innerT1);
			m_game->Input(m_frameTime);
			QueryPerformanceCounter(&innerT2);
			countStats2_2++;
			elapsedTime = static_cast<double>(ONE_MILLION * (innerT2.QuadPart - innerT1.QuadPart)) / frequency.QuadPart; // in [us]
			minMaxTime2_2.ProcessTime(elapsedTime);
			timeSum2_2 += elapsedTime;
			/* ==================== REGION #2_2 end ====================*/
			
			//Input::Update();
			
			/* ==================== REGION #2_3 begin ====================*/
			QueryPerformanceCounter(&innerT1);
			m_game->Update(m_frameTime);
			QueryPerformanceCounter(&innerT2);
			countStats2_3++;
			elapsedTime = static_cast<double>(ONE_MILLION * (innerT2.QuadPart - innerT1.QuadPart)) / frequency.QuadPart; // in [us]
			minMaxTime2_3.ProcessTime(elapsedTime);
			timeSum2_3 += elapsedTime;
			/* ==================== REGION #2_3 end ====================*/

#ifdef ANT_TWEAK_BAR_ENABLED
			Rendering::CheckChangesAndUpdateGLState();
#endif
			
			unprocessingTime -= m_frameTime;
		}
		QueryPerformanceCounter(&t2); // start timer
		countStats2++;
		elapsedTime = static_cast<double>(ONE_MILLION * (t2.QuadPart - t1.QuadPart)) / frequency.QuadPart; // in [us]
		minMaxTime2.ProcessTime(elapsedTime);
		timeSum2 += elapsedTime; // in [ms]
		/* ==================== REGION #2 end ====================*/
		
		/* ==================== REGION #3 begin ====================*/
		QueryPerformanceCounter(&t1);
		if (isRenderRequired)
		{
			//m_renderer->ClearScreen();
			//m_renderer->Render();
			//Shader* shader = m_game->GetShader();
			//if (shader == NULL)
			//{
			//	LOG(Error, LOGPLACE, "Shader instance is NULL");
			//}
			//m_renderer->Render(m_game->GetRootGameNode());
			m_renderer->SetReal("timeOfDay", m_timeOfDay);
			m_game->Render(m_renderer);
#ifdef COUNT_FPS
			++framesCount;
			
			std::stringstream ss;
			ss << "FPS = " << fps << " SPF[ms] = " << std::setprecision(4) << spf;
			m_fpsTextRenderer->DrawString(0, 570, ss.str(), m_renderer);
			
			if (m_game->IsInGameTimeCalculationEnabled())
			{
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
#ifdef ANT_TWEAK_BAR_ENABLED
			TwDraw();
#endif
			m_renderer->SwapBuffers();
			++m_renderingRequiredCount;
		}
		else
		{
			//LOG(Info, LOGPLACE, "Rendering is not required. Moving on...");
			// TODO: Sleep for 1ms to prevent the thread from constant looping
			//this_thread::sleep_for(chrono::milliseconds(100));
			tthread::this_thread::sleep_for(tthread::chrono::milliseconds(THREAD_SLEEP_TIME));
			++m_renderingNotRequiredCount;
		}
		QueryPerformanceCounter(&t2);
		countStats3++;
		elapsedTime = static_cast<double>(ONE_MILLION * (t2.QuadPart - t1.QuadPart)) / frequency.QuadPart; // in [us]
		minMaxTime3.ProcessTime(elapsedTime);
		timeSum3 += elapsedTime; // in [ms]
		/* ==================== REGION #3 end ====================*/
	}
}

void CoreEngine::ConvertTimeOfDay(int& inGameHours, int& inGameMinutes, int& inGameSeconds) const
{
	ConvertTimeOfDay(m_timeOfDay, inGameHours, inGameMinutes, inGameSeconds);
}

void CoreEngine::ConvertTimeOfDay(Math::Real timeOfDay, int& inGameHours, int& inGameMinutes, int& inGameSeconds) const
{
	inGameHours = static_cast<int>(timeOfDay) / SECONDS_PER_HOUR;
	Math::Real temp = fmod(m_timeOfDay, SECONDS_PER_HOUR);
	inGameMinutes = static_cast<int>(temp) / SECONDS_PER_MINUTE;
	inGameSeconds = static_cast<int>(fmod(temp, SECONDS_PER_MINUTE));
}

unsigned int CoreEngine::GetCurrentCameraIndex() const
{
	ASSERT(m_renderer != NULL);
	if (m_renderer == NULL)
	{
		LOG(Critical, LOGPLACE, "Renderer is not yet initialized");
		exit(EXIT_FAILURE);
	}
	return m_renderer->GetCurrentCameraIndex();
}

unsigned int CoreEngine::NextCamera() const
{
	ASSERT(m_renderer != NULL);
	if (m_renderer == NULL)
	{
		LOG(Critical, LOGPLACE, "Renderer is not yet initialized");
		exit(EXIT_FAILURE);
	}
	return m_renderer->NextCamera();
}

unsigned int CoreEngine::PrevCamera() const
{
	ASSERT(m_renderer != NULL);
	if (m_renderer == NULL)
	{
		LOG(Critical, LOGPLACE, "Renderer is not yet initialized");
		exit(EXIT_FAILURE);
	}
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
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	Math::Real result = SECONDS_PER_HOUR * timeinfo->tm_hour + SECONDS_PER_MINUTE * timeinfo->tm_min + timeinfo->tm_sec;
	if (result > SECONDS_PER_DAY)
	{
		LOG(Utility::Error, LOGPLACE, "Incorrect local time");
		result = REAL_ZERO;
	}
	return result;
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
		LOG(Critical, LOGPLACE, "Cannot set cursor position. The rendering engine is NULL.");
		return;
	}
	m_renderer->SetCursorPos(xPos, yPos);
}

void CoreEngine::CalculateSunElevationAndAzimuth()
{
	const int dayNumber = GET_CONFIG_VALUE("startingDayNumber", 172);
	const Math::Angle LATITUDE(GET_CONFIG_VALUE("latitude", 52.0f));
	const Math::Angle LONGITUDE(GET_CONFIG_VALUE("longitude", -16.0f));
	const int timeGMTdifference = 1;
	
	const Math::Angle b((360.0f / 365.0f) * (dayNumber -81));
	const Math::Angle doubleB(b.GetAngleInRadians() * 2.0f, Math::Unit::RADIAN);
	const Math::Angle tropicOfCancer(23.45f);

	const Math::Real equationOfTime = 9.87f * doubleB.Sin() - 7.53f * b.Cos() - 1.5f * b.Sin(); // EoT
	const Math::Real declinationSin = tropicOfCancer.Sin() * b.Sin();
	const Math::Angle declinationAngle(asin(declinationSin), Math::Unit::RADIAN);
	LOG(Utility::Debug, LOGPLACE, "Declination in degrees = %.5f", declinationAngle.GetAngleInDegrees());

	const Math::Real timeCorrectionInSeconds = 60.0f * (4.0f * (LONGITUDE.GetAngleInDegrees() - 15.0f * timeGMTdifference) + equationOfTime);
	const Math::Real localSolarTime = m_timeOfDay + timeCorrectionInSeconds;
	LOG(Utility::Debug, LOGPLACE, "Time correction in seconds = %.5f", timeCorrectionInSeconds);
	LOG(Utility::Debug, LOGPLACE, "Local time = %.5f\tLocal solar time = %.5f", m_timeOfDay, localSolarTime);
	
	const Math::Angle hourAngle(15.0f * (localSolarTime - 12 * SECONDS_PER_HOUR) / SECONDS_PER_HOUR);
	LOG(Utility::Debug, LOGPLACE, "Hour angle = %.5f", hourAngle.GetAngleInDegrees());

	const Math::Real sunElevationSin = declinationSin * LATITUDE.Sin() + declinationAngle.Cos() * LATITUDE.Cos() * hourAngle.Cos();
	m_sunElevation.SetAngleInRadians(asin(sunElevationSin));
	LOG(Utility::Debug, LOGPLACE, "Sun elevation = %.5f", m_sunElevation.GetAngleInDegrees());

	const Math::Real sunAzimuthCos = ((declinationSin * LATITUDE.Cos()) - (declinationAngle.Cos() * LATITUDE.Sin() * hourAngle.Cos())) / m_sunElevation.Cos();
	m_sunAzimuth.SetAngleInRadians(acos(sunAzimuthCos));
	bool isAfternoon = (localSolarTime > 12.0f * SECONDS_PER_HOUR) || (hourAngle.GetAngleInDegrees() > REAL_ZERO);
	if (isAfternoon)
	{
		m_sunAzimuth.SetAngleInDegrees(360.0f - m_sunAzimuth.GetAngleInDegrees());
	}

	Daytime prevDaytime = m_daytime;
	if (m_sunElevation < M_FIRST_ELEVATION_LEVEL)
	{
		m_daytime = Rendering::NIGHT;
	}
	else if (m_sunElevation < M_SECOND_ELEVATION_LEVEL)
	{
		m_daytime = (isAfternoon) ? Rendering::AFTER_DUSK : Rendering::BEFORE_DAWN;
	}
	else if (m_sunElevation < M_THIRD_ELEVATION_LEVEL)
	{
		m_daytime = (isAfternoon) ? Rendering::SUNSET : Rendering::SUNRISE;
	}
	else
	{
		m_daytime = Rendering::DAY;
	}
	if (prevDaytime != m_daytime)
	{
		LOG(Utility::Info, LOGPLACE, "%.2f, %.2f, %.2f\t m_daytime = %d at %.1f", M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees(), M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees(),
			M_THIRD_ELEVATION_LEVEL.GetAngleInDegrees(), m_daytime, m_timeOfDay);
	}


	LOG(Utility::Debug, LOGPLACE, "Sun azimuth = %.5f", m_sunAzimuth.GetAngleInDegrees());
}

Rendering::Daytime CoreEngine::GetCurrentDaytime(Math::Real& daytimeTransitionFactor) const
{
	switch (m_daytime)
	{
	case Rendering::NIGHT:
	case Rendering::DAY:
		daytimeTransitionFactor = REAL_ZERO;
		break;
	case Rendering::BEFORE_DAWN:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	case Rendering::SUNRISE:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_THIRD_ELEVATION_LEVEL.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	case Rendering::SUNSET:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_THIRD_ELEVATION_LEVEL.GetAngleInDegrees() - M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	case Rendering::AFTER_DUSK:
		daytimeTransitionFactor = (m_sunElevation.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees()) /
			(M_SECOND_ELEVATION_LEVEL.GetAngleInDegrees() - M_FIRST_ELEVATION_LEVEL.GetAngleInDegrees());
		break;
	default:
		LOG(Utility::Error, LOGPLACE, "Incorrect daytime");
	}
	return m_daytime;
}

#ifdef ANT_TWEAK_BAR_ENABLED
void CoreEngine::InitializeTweakBars()
{
	TwBar* coreEnginePropertiesBar = TwNewBar("CoreEnginePropertiesBar");
	TwAddVarRW(coreEnginePropertiesBar, "windowWidth", TW_TYPE_INT32, &m_windowWidth, " label='Window width' ");
	TwAddVarRW(coreEnginePropertiesBar, "windowHeight", TW_TYPE_INT32, &m_windowHeight, " label='Window height' ");
	TwAddVarRO(coreEnginePropertiesBar, "frameTime", TW_TYPE_REAL, &m_frameTime, " label='Frame time' ");
	TwAddVarRW(coreEnginePropertiesBar, "clockSpeed", TW_TYPE_REAL, &m_clockSpeed, " label='Clock speed' ");
	TwAddVarRW(coreEnginePropertiesBar, "timeOfDay", TW_TYPE_REAL, &m_timeOfDay, " label='Time of day' ");
	
	TwEnumVal daytimeEV[] = { { Rendering::NIGHT, "Night" }, { Rendering::BEFORE_DAWN, "Before dawn" }, { Rendering::SUNRISE, "Sunrise" },
		{ Rendering::DAY, "Day" }, { Rendering::SUNSET, "Sunset" }, { Rendering::AFTER_DUSK, "After dusk" }};
	TwType daytimeType = TwDefineEnum("Daytime", daytimeEV, 6);
	TwAddVarRW(coreEnginePropertiesBar, "daytime", daytimeType, &m_daytime, " label='Daytime' ");
	
	
	TwAddVarRW(coreEnginePropertiesBar, "sunElevation", angleType, &m_sunElevation, " label='Sun elevation' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunAzimuth", angleType, &m_sunAzimuth, " label='Sun azimuth' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunFirstElevationLevel", angleType, &M_FIRST_ELEVATION_LEVEL, " label='First elevation level' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunSecondElevationLevel", angleType, &M_SECOND_ELEVATION_LEVEL, " label='Second elevation level' ");
	TwAddVarRW(coreEnginePropertiesBar, "sunThirdElevationLevel", angleType, &M_THIRD_ELEVATION_LEVEL, " label='Third elevation level' ");
	//TwSetParam(coreEnginePropertiesBar, NULL, "visible", TW_PARAM_CSTRING, 1, "false"); // Hide the bar at startup
}
#endif
