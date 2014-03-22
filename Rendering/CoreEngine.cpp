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

CoreEngine::CoreEngine(int maxFrameRate, Game* game) :
	isRunning(false),
	windowWidth(Config::Get("windowWidth", 800)),
	windowHeight(Config::Get("windowHeight", 600)),
	windowTitle(Config::Get<string>("windowTitle", "3D game")),
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

void CoreEngine::CreateRenderer(int width, int height, const std::string& title)
{
	this->renderer = new Renderer(width, height, title);

	if (this->renderer == NULL)
	{
		stdlog(Critical, LOGPLACE, "Failed to create a Renderer");
		exit(-1);
	}

	// TODO: Get this value from the config file
	Transform::SetProjection(70.0 /* fov */, static_cast<Math::Real>(width), static_cast<Math::Real>(height), 0.1, 1000.0);

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
	double fpsSample = Config::Get("FPS_sample", 1.0); // represents the time after which FPS value is calculated and logged
	int framesCount = 0;
	double frameTimeCounter = 0.0;
#endif

	double unprocessingTime = 0.0; // used to cap the FPS when it gets too high
	double previousTime = glfwGetTime();

	while (isRunning)
	{
		bool isRenderRequired = false;

		// flCurrentTime will be lying around from last frame. It's now the previous time.
		double currentTime = glfwGetTime();
		double passedTime = currentTime - previousTime;
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
			//this->renderer->Render();
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

Time CoreEngine::GetTime() const
{
	return Time(glfwGetTime());

	//return Time::Now();
}