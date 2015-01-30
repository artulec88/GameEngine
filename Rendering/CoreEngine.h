#pragma once

#include "Rendering.h"
#include "Renderer.h"
#include "TextRenderer.h"
#include "Game.h"

#include "Math\Math.h"

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
protected:
	void CreateRenderer(int width, int height, const std::string& title);
	virtual void Run();
	void PollEvents();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	bool m_isRunning;
	int m_windowWidth;
	int m_windowHeight;
	const char* m_windowTitle;
	const Math::Real m_frameTime;
	Game* m_game;
	Renderer* m_renderer;
	TextRenderer* m_fpsTextRenderer;
/* ==================== Non-static member variables end ==================== */
}; /* end class CoreEngine */

} /* end namespace Rendering */

