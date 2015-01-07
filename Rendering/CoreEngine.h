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
	static CoreEngine* coreEngine;
/* ==================== Static variables end ==================== */

/* ==================== Static functions begin ==================== */
public:
	static CoreEngine* GetCoreEngine();
/* ==================== Static functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	bool isRunning;
	int windowWidth;
	int windowHeight;
	const char* windowTitle;
	const Math::Real frameTime;
	Game* game;
	Renderer* renderer;
	TextRenderer* fpsTextRenderer;
/* ==================== Non-static member variables end ==================== */

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
	Renderer* GetRenderer() const { return renderer; }

	
	int GetWindowWidth() const { return this->windowWidth; };
	int GetWindowHeight() const { return this->windowHeight; };
	void SetCursorPos(Math::Real xPos, Math::Real yPos) { this->renderer->SetCursorPos(xPos, yPos); }
	virtual Math::Real GetTime() const;
	virtual void ClearScreen() const;
protected:
	void CreateRenderer(int width, int height, const std::string& title);
	virtual void Run();
	void PollEvents();
/* ==================== Non-static member functions end ==================== */
}; /* end class CoreEngine */

} /* end namespace Rendering */

