#include "StdAfx.h"
#include "CoreEngine.h"
#include "Input.h"

#include "Utility\Log.h"
#include "Utility\Config.h"

using namespace Rendering;
using namespace Utility;
using namespace std;

CoreEngine* CoreEngine::coreEngine = NULL;

/* static */ CoreEngine* CoreEngine::GetCoreEngine()
{
	return coreEngine;
}

CoreEngine::CoreEngine(int width, int height, const std::string& title, int maxFrameRate, Game* game) :
	isRunning(false),
	windowWidth(width),
	windowHeight(height),
	windowTitle(title),
	frameTime(static_cast<Math::Real>(1.0) / maxFrameRate),
	game(game),
	renderer(NULL)
{
	stdlog(Debug, LOGPLACE, "Main application construction started");
	if (coreEngine != NULL)
	{
		stdlog(Error, LOGPLACE, "Constructor called when a singleton instance of MainApp class has already been created");
		delete coreEngine;
		coreEngine = NULL;
	}
	coreEngine = this;
	
	CreateRenderer();
	
	stdlog(Debug, LOGPLACE, "Main application construction finished");
}


CoreEngine::~CoreEngine(void)
{
	stdlog(Debug, LOGPLACE, "Core engine destruction started");

	// TODO: Expand this with additional resources deallocation
	if (this->game != NULL)
	{
		delete this->game;
		this->game = NULL;
	}
	if (this->renderer != NULL)
	{
		delete this->renderer;
		this->renderer = NULL;
	}

	stdlog(Debug, LOGPLACE, "Core engine destruction finished");
}

void CoreEngine::CreateRenderer()
{
	this->renderer = new Renderer(windowWidth, windowHeight, windowTitle);

	if (this->renderer == NULL)
	{
		stdlog(Critical, LOGPLACE, "Failed to create a Renderer");
		exit(-1);
	}

	ASSERT(this->renderer != NULL);
}

void CoreEngine::Start()
{
	if (isRunning)
	{
		stdlog(Warning, LOGPLACE, "The core engine instance is already running");
		return;
	}
	stdlog(Notice, LOGPLACE, "The core engine starts");
	Run();
}

void CoreEngine::Stop()
{
	if (!isRunning)
	{
		stdlog(Warning, LOGPLACE, "The core engine instance is not running");
		return;
	}
	
	isRunning = false;
	ASSERT(!isRunning);
	stdlog(Notice, LOGPLACE, "The core engine has stopped");
}

void CoreEngine::Run()
{
	stdlog(Notice, LOGPLACE, "The game started running");
	ASSERT(!isRunning);

	game->Init();

	isRunning = true;

#ifdef COUNT_FPS
	Math::Real fpsSample = static_cast<Math::Real>(Config::Get("FPS_sample", 1.0)); // represents the time after which FPS value is calculated and logged
	int framesCount = 0;
	Math::Real frameTimeCounter = 0.0;
#endif

	Math::Real unprocessingTime = 0.0; // used to cap the FPS when it gets too high
	Math::Real previousTime = GetTime();

	while (isRunning)
	{
		bool isRenderRequired = false;

		// flCurrentTime will be lying around from last frame. It's now the previous time.
		Math::Real currentTime = GetTime();
		Math::Real passedTime = currentTime - previousTime;

		previousTime = currentTime;
		unprocessingTime += passedTime;

#ifdef COUNT_FPS
		frameTimeCounter += passedTime;
		// Counting FPS and logging
		if (frameTimeCounter >= fpsSample)
		{
			int fps = framesCount / fpsSample; // Frames Per Second
			Math::Real spf = 1000 * frameTimeCounter / framesCount; // Seconds Per Frame
			stdlog(Debug, LOGPLACE, "FPS = %5d\t Average time per frame = %.3f [ms]", fps, spf);
			framesCount = 0;
			frameTimeCounter = 0.0;
		}
#endif

		while (unprocessingTime > frameTime)
		{
			isRenderRequired = true;
			if (IsCloseRequested())
			{
				Stop();
				return;
			}
			// TODO: Set delta
			PollEvents();
			game->Update();
			unprocessingTime -= frameTime;
		}
		if (isRenderRequired)
		{
			//this->renderer->ClearScreen();
			//this->renderer->Render();
			Shader* shader = this->game->GetShader();
			if (shader == NULL)
			{
				stdlog(Error, LOGPLACE, "Shader instance is NULL");
			}
			this->renderer->Render(this->game->GetRootGameNode());
			//game->Render();
			this->renderer->SwapBuffers();
#ifdef COUNT_FPS
			++framesCount;
#endif
		}
		else
		{
			//stdlog(Info, LOGPLACE, "Rendering is not required. Moving on...");
			// TODO: Sleep for 1ms to prevent the thread from constant looping
		}
	}
}

void CoreEngine::NextCamera()
{
	ASSERT(renderer != NULL);
	if (this->renderer == NULL)
	{
		stdlog(Error, LOGPLACE, "Renderer is not yet initialized");
		return;
	}
	renderer->NextCamera();
}

void CoreEngine::PrevCamera()
{
	ASSERT(renderer != NULL);
	if (renderer == NULL)
	{
		stdlog(Error, LOGPLACE, "Renderer is not yet initialized");
		return;
	}
	renderer->PrevCamera();
}

void CoreEngine::PollEvents()
{
	glfwPollEvents();
}

bool CoreEngine::IsCloseRequested() const
{
	// TODO: Create Input class and check whether the user pressed Escape or Alt+F4 combination. Return true if so and false otherwise.
	//return Input::IsLeftAltPressed(renderer.GetWindow());
	return false;
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