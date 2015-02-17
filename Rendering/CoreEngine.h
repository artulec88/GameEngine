#pragma once

#include "Rendering.h"
#include "Renderer.h"
#include "TextRenderer.h"
#include "Game.h"

#include "Math\Math.h"
#include "Math\Angle.h"

#include "Utility\Time.h"

#include <string>

//#ifdef _DEBUG
#define COUNT_FPS
//#endif

namespace Rendering
{

class RENDERING_API CoreEngine
{
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
	CoreEngine(int width, int height, const char* title, int maxFrameRate, Game* game);
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
	Rendering::Daytime GetCurrentDaytime(Math::Real& daytimeTransitionFactor) const;
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
	const Math::Real SECONDS_PER_MINUTE; // the number of seconds during one minute
	const Math::Real SECONDS_PER_HOUR; // the number of seconds during one hour
	const Math::Real SECONDS_PER_DAY; // the number of seconds during one day
	Math::Real m_timeOfDay;
	Rendering::Daytime m_daytime;
	Math::Angle m_sunElevation;
	Math::Angle m_sunAzimuth;
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
	Game* m_game;
	Renderer* m_renderer;
	TextRenderer* m_fpsTextRenderer;
/* ==================== Non-static member variables end ==================== */
}; /* end class CoreEngine */

} /* end namespace Rendering */

