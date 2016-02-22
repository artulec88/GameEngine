#ifndef __ENGINE_CORE_ENGINE_H__
#define __ENGINE_CORE_ENGINE_H__

#include "Engine.h"
#include "GameManager.h"
#include "GameStateManager.h"
#include "QuitGameCommand.h"


#include "Rendering\Renderer.h"
#include "Rendering\TextRenderer.h"

#include "Math\Math.h"
#include "Math\Angle.h"

#include "Utility\Time.h"

#include <string>

#ifdef CALCULATE_RENDERING_STATS
#include "Math\Statistics.h"
#include "Math\IStatisticsStorage.h"
#include "Math\UtmostTimeSamples.h"
#include <vector>
#endif

#undef STOP_TIMER
#define STOP_TIMER(timerID, countStats, minMaxTime, timeSum) do { if (true) StopTimer(timerID, countStats, minMaxTime, timeSum); } while (0)

// TODO: Consider replacing some of even all of below #defines with CoreEngine class member variables.
// As member variables they could be easily added to the CoreEngine tweak bar and modified in runtime.
//#ifdef _DEBUG

#define COUNT_FPS
#ifdef COUNT_FPS
#define DRAW_FPS
#endif

#define DRAW_GAME_TIME

//#endif

namespace Engine
{

class CoreEngine
{
/* ==================== Static variables and functions begin ==================== */
protected:
	ENGINE_API static CoreEngine* s_coreEngine;
public:
	ENGINE_API static CoreEngine* GetCoreEngine();
	static void ErrorCallback(int errorCode, const char* description);
	static void WindowCloseEventCallback(GLFWwindow* window);
	static void WindowResizeCallback(GLFWwindow* window, int width, int height);
	static void KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void CharEventCallback(GLFWwindow* window, unsigned int codepoint);
	static void MouseEventCallback(GLFWwindow* window, int button, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
	static void ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset);
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ENGINE_API CoreEngine(int width, int height, const char* title, int maxFrameRate, GameManager& game,
		const std::string& shadersDirectory = "..\\Shaders\\", const std::string& modelsDirectory = "..\\Models\\",
		const std::string& texturesDirectory = "..\\Textures\\");
	ENGINE_API ~CoreEngine(void);
private: // disable copy constructor
	CoreEngine(const CoreEngine& coreEngine);
	void operator=(const CoreEngine& coreEngine);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	ENGINE_API void Start();
	ENGINE_API void Stop();
	inline void RequestWindowClose() const
	{
		glfwSetWindowShouldClose(m_window, GL_TRUE);
	}
	void InitGlew();
	GLFWwindow* GetThreadWindow() const
	{
		return m_threadWindow;
	}

	size_t GetCurrentCameraIndex() const;
	size_t NextCamera() const;
	size_t PrevCamera() const;
	Rendering::Renderer* GetRenderer() const { return m_renderer; }

	void WindowResizeEvent(GLFWwindow* window, int width, int height);
	/// <summary>
	/// Error callback.
	/// </summary>
	/// <param name="errorCode"> The error code. See http://www.glfw.org/docs/latest/group__errors.html. </param>
	/// <param name="description"> The description of the error. </param>
	void ErrorCallbackEvent(int errorCode, const char* description);
	void CloseWindowEvent(GLFWwindow* window);
	
	/// <summary>
	/// Main entry for the key event handling.
	/// </summary>
	/// <param name="key">The keyboard key that was pressed or released.</param>
	/// <param name="scancode">The system-specific scancode of the key.</param>
	/// <param name="action">GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.</param>
	/// <param name="mods">Bit field describing which modifier keys were held down when key event was dispatched.</param>
	void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	void MousePosEvent(GLFWwindow* window, double xPos, double yPos);
	void ScrollEvent(GLFWwindow* window, double xOffset, double yOffset);
	
	int GetWindowWidth() const { return m_windowWidth; };
	int GetWindowHeight() const { return m_windowHeight; };
	
	void SetCursorPos(Math::Real xPos, Math::Real yPos);
	ENGINE_API void CentralizeCursor();

	Math::Real GetTime() const;
	void ClearScreen() const;
	Math::Real GetCurrentInGameTime() const { return m_timeOfDay; }
	Math::Angle GetSunElevation() const { return m_sunElevation; }
	Math::Angle GetSunAzimuth() const { return m_sunAzimuth; }
	Math::Real GetClockSpeed() const { return m_clockSpeed; }
	ENGINE_API Utility::Timing::Daytime GetCurrentDaytime(Math::Real& daytimeTransitionFactor) const;
	void ConvertTimeOfDay(int& inGameHours, int& inGameMinutes, int& inGameSeconds) const;
	void ConvertTimeOfDay(Math::Real timeOfDay, int& inGameHours, int& inGameMinutes, int& inGameSeconds) const;

	std::string GetShadersDirectory() const { return m_shadersDirectory; }
	std::string GetModelsDirectory() const { return m_modelsDirectory; }
	std::string GetTexturesDirectory() const { return m_texturesDirectory; }

	void AddWaterNode(Rendering::GameNode* waterNode);
	void AddTerrainNode(Rendering::GameNode* terrainNode);
	void AddBillboardNode(Rendering::GameNode* billboardNode);
private:
	void CreateRenderer(int width, int height, const std::string& title);
	void Run();
	void PollEvents();
	Math::Real GetCurrentLocalTime() const;
	/**
	 * See http://pveducation.org/pvcdrom/properties-of-sunlight/sun-position-calculator
	 */
	void CalculateSunElevationAndAzimuth();

#ifdef CALCULATE_RENDERING_STATS
public:
	void StartSamplingSpf() const { m_isSamplingSpf = true; }
	void StopSamplingSpf() const { m_isSamplingSpf = false; }
private:
	void InitGraphics(int width, int height, const std::string& title);
	void InitGlfw(int width, int height, const std::string& title);
	void SetCallbacks();
	void StopTimer(Utility::Timing::Timer& timer, long& countStats, Math::Statistics::UtmostTimeSamples& minMaxTime, double& timeSum) const
	{
		if (timer.IsRunning())
		{
			timer.Stop();
		}
		++countStats;
		Utility::Timing::TimeSpan timeSpan = timer.GetTimeSpan(Utility::Timing::MICROSECOND);
		minMaxTime.ProcessTime(timeSpan);
		timeSum += timeSpan.GetValue();
	}
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
	void InitializeTweakBars();
#endif
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	GLFWwindow* m_window;
	GLFWwindow* m_threadWindow;
	bool m_isRunning;
	int m_windowWidth;
	int m_windowHeight;
	const char* m_windowTitle;
	const Math::Real m_frameTime;
	GameManager& m_game;
	Rendering::Renderer* m_renderer;
	Rendering::TextRenderer* m_fpsTextRenderer;

	QuitGameCommand m_quitGameCommand;

	const Math::Angle LATITUDE;
	const Math::Angle LONGITUDE;
	const Math::Real TROPIC_OF_CANCER_SINUS;
	const int SECONDS_PER_MINUTE; // the number of seconds during one minute
	const int SECONDS_PER_HOUR; // the number of seconds during one hour
	const int SECONDS_PER_DAY; // the number of seconds during one day
	const int DAYS_PER_YEAR; // the number of days during one year
	int m_dayNumber;
	Math::Real m_timeOfDay;
	Utility::Timing::Daytime m_daytime;
	Math::Angle m_sunElevation;
	Math::Angle m_sunAzimuth;

	/// <summary> Specifies where to look for the shader files. </summary>
	const std::string m_shadersDirectory;
	/// <summary> Specifies where to look for the model files. </summary>
	const std::string m_modelsDirectory;
	/// <summary> Specifies where to look for the texture files. </summary>
	const std::string m_texturesDirectory;

#ifdef CALCULATE_RENDERING_STATS
	long m_countStats1;
	Math::Statistics::UtmostTimeSamples m_minMaxTime1;
	double m_timeSum1;
	
	long m_countStats2;
	Math::Statistics::UtmostTimeSamples m_minMaxTime2;
	double m_timeSum2;
	
	long m_countStats2_1;
	Math::Statistics::UtmostTimeSamples m_minMaxTime2_1;
	double m_timeSum2_1;
	
	long m_countStats2_2;
	Math::Statistics::UtmostTimeSamples m_minMaxTime2_2;
	double m_timeSum2_2;
	
	long m_countStats2_3;
	Math::Statistics::UtmostTimeSamples m_minMaxTime2_3;
	double m_timeSum2_3;
	
	long m_countStats3;
	Math::Statistics::UtmostTimeSamples m_minMaxTime3;
	double m_timeSum3;

	Utility::Timing::Timer m_timer;

	mutable int m_renderingRequiredCount;
	mutable int m_renderingNotRequiredCount;
	mutable bool m_isSamplingSpf;
	Math::Statistics::ClassStats& m_classStats;
	mutable Math::Statistics::Stats<Math::Real> m_stats;
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
}; /* end class GameEngine */

} /* end namespace Engine */

#endif /* __ENGINE_CORE_ENGINE_H__ */