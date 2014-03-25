#pragma once

#include "Rendering.h"
#include "Renderer.h"
#include "Game.h"

#include "Math\Math.h"

#include "Utility\Time.h"

#include <string>

#ifdef _DEBUG
#define COUNT_FPS
#endif

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
	std::string windowTitle;
	Math::Real frameTime;
	Game* game;
	Renderer* renderer;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	CoreEngine(int width, int height, const std::string& title, int maxFrameRate, Game* game);
	virtual ~CoreEngine(void);
private: // disable copy constructor
	CoreEngine(const CoreEngine& app);
	void operator=(const CoreEngine& app);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Start();
	void Stop();
	void NextCamera();
	void PrevCamera();

	
	int GetWindowWidth() const { return this->windowWidth; };
	int GetWindowHeight() const { return this->windowHeight; };
	virtual bool IsCloseRequested() const;
	virtual Math::Real GetTime() const;
	virtual void ClearScreen() const;
protected:
	void CreateRenderer();
	virtual void Run();
	void PollEvents();
/* ==================== Non-static member functions end ==================== */
}; /* end class CoreEngine */

} /* end namespace Rendering */

