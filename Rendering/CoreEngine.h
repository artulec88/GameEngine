#pragma once

#include "Rendering.h"
#include "Renderer.h"
#include "TextRenderer.h"
#include "GameManager.h"
#include "GameStateManager.h"

#include "Math\Math.h"
#include "Math\Angle.h"

#include "Utility\Time.h"

#include <string>

//#ifdef _DEBUG
#define COUNT_FPS
//#define CALCULATE_STATS
//#endif

namespace Rendering
{

class RENDERING_API CoreEngine
{
#ifdef CALCULATE_STATS
#define MIN_MAX_STATS_COUNT 3
struct MinMaxTime
{
	double m_minTime[MIN_MAX_STATS_COUNT];
	double m_maxTime[MIN_MAX_STATS_COUNT];
	
	void Init();
	void ProcessTime(double elapsedTime);
	std::string ToString();
};
#endif

/* ==================== Static variables begin ==================== */
protected:
	static CoreEngine* s_coreEngine;
/* ==================== Static variables end ==================== */

/* ==================== Static functions begin ==================== */
public:
	static CoreEngine* GetCoreEngine();
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	CoreEngine(int width, int height, const char* title, int maxFrameRate, GameManager& game);
	virtual ~CoreEngine(void);
private: // disable copy constructor
	CoreEngine(const CoreEngine& app);
	void operator=(const CoreEngine& app);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Start();
	void Stop();
	unsigned int GetCurrentCameraIndex() const;
	unsigned int NextCamera() const;
	unsigned int PrevCamera() const;
	Renderer* GetRenderer() const { return m_renderer; }

	
	int GetWindowWidth() const { return m_windowWidth; };
	int GetWindowHeight() const { return m_windowHeight; };
	void SetCursorPos(Math::Real xPos, Math::Real yPos);
	virtual Math::Real GetTime() const;
	virtual void ClearScreen() const;
	Math::Real GetCurrentInGameTime() const { return m_timeOfDay; }
	Math::Angle GetSunElevation() const { return m_sunElevation; }
	Math::Angle GetSunAzimuth() const { return m_sunAzimuth; }
	Rendering::GameTime::Daytime GetCurrentDaytime(Math::Real& daytimeTransitionFactor) const;
	void ConvertTimeOfDay(int& inGameHours, int& inGameMinutes, int& inGameSeconds) const;
	void ConvertTimeOfDay(Math::Real timeOfDay, int& inGameHours, int& inGameMinutes, int& inGameSeconds) const;
protected:
	void CreateRenderer(int width, int height, const std::string& title);
	virtual void Run();
	void PollEvents();
private:
	Math::Real GetCurrentLocalTime() const;
	/**
	 * See http://pveducation.org/pvcdrom/properties-of-sunlight/sun-position-calculator
	 */
	void CalculateSunElevationAndAzimuth();

#ifdef CALCULATE_STATS
	void StartTimer(LARGE_INTEGER& start) const { QueryPerformanceCounter(&start); }
	void StopTimer(LARGE_INTEGER& start, LARGE_INTEGER& end, LARGE_INTEGER frequency, long& countStats, MinMaxTime& minMaxTime, double& timeSum) const
	{
		static const Math::Real ONE_MILLION = static_cast<Math::Real>(1000000.0f);
		QueryPerformanceCounter(&end);
		++countStats;
		double elapsedTime = static_cast<double>(ONE_MILLION * (end.QuadPart - start.QuadPart)) / frequency.QuadPart; // in [us]
		minMaxTime.ProcessTime(elapsedTime);
		timeSum += elapsedTime;
	}
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
	void InitializeTweakBars();
#endif
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	bool m_isRunning;
	int m_windowWidth;
	int m_windowHeight;
	const char* m_windowTitle;
	const Math::Real m_frameTime;
	GameManager& m_game;
	Renderer* m_renderer;
	TextRenderer* m_fpsTextRenderer;

	const int SECONDS_PER_MINUTE; // the number of seconds during one minute
	const int SECONDS_PER_HOUR; // the number of seconds during one hour
	const int SECONDS_PER_DAY; // the number of seconds during one day
	const int DAYS_PER_YEAR; // the number of days during one year
	int m_dayNumber;
	Math::Real m_timeOfDay;
	Rendering::GameTime::Daytime m_daytime;
	Math::Angle m_sunElevation;
	Math::Angle m_sunAzimuth;

#ifdef CALCULATE_STATS
	long m_countStats1;
	MinMaxTime m_minMaxTime1;
	double m_timeSum1;
	
	long m_countStats2;
	MinMaxTime m_minMaxTime2;
	double m_timeSum2;
	
	long m_countStats2_1;
	MinMaxTime m_minMaxTime2_1;
	double m_timeSum2_1;
	
	long m_countStats2_2;
	MinMaxTime m_minMaxTime2_2;
	double m_timeSum2_2;
	
	long m_countStats2_3;
	MinMaxTime m_minMaxTime2_3;
	double m_timeSum2_3;
	
	long m_countStats3;
	MinMaxTime m_minMaxTime3;
	double m_timeSum3;

	mutable int m_renderingRequiredCount;
	mutable int m_renderingNotRequiredCount;
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
	Math::Angle M_FIRST_ELEVATION_LEVEL;
	Math::Angle M_SECOND_ELEVATION_LEVEL;
	Math::Angle M_THIRD_ELEVATION_LEVEL;
	Math::Real m_clockSpeed;
#else
	const Math::Angle M_FIRST_ELEVATION_LEVEL;
	const Math::Angle M_SECOND_ELEVATION_LEVEL;
	const Math::Angle M_THIRD_ELEVATION_LEVEL;
	const Math::Real m_clockSpeed;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class CoreEngine */

} /* end namespace Rendering */

