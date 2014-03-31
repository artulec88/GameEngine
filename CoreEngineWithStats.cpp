#include "StdAfx.h"
#include "CoreEngine.h"
#include "Input.h"

#include "Utility\Log.h"
#include "Utility\Config.h"

#include <ctime>

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

long countStats1 = 0;
double timeSum1 = 0.0;
long countStats2 = 0;
double timeSum2 = 0.0;
long countStats3 = 0;
double timeSum3 = 0.0;

void CoreEngine::Stop()
{
	if (!isRunning)
	{
		stdlog(Warning, LOGPLACE, "The core engine instance is not running");
		return;
	}
	
	/* ==================== printing stats begin ==================== */
	stdlog(Debug, LOGPLACE, "The first region was processed %d times, which took exactly %.4f [ms]. The average time is %.4f [us]", countStats1, timeSum1, 1000.0 * timeSum1 / countStats1);
	stdlog(Debug, LOGPLACE, "The second region was processed %d times, which took exactly %.4f [ms]. The average time is %.4f [us]", countStats2, timeSum2, 1000.0 * timeSum2 / countStats2);
	stdlog(Debug, LOGPLACE, "The third region was processed %d times, which took exactly %.4f [ms]. The average time is %.4f [us]", countStats3, timeSum3, 1000.0 * timeSum3 / countStats3);
	/* ==================== printing stats end ==================== */
	
	
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
		clock_t begin = clock();
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
		clock_t end = clock();
		countStats1++;
		timeSum1 += static_cast<double>(1000.0 * (end - begin)) / CLOCKS_PER_SEC; // in [ms]

		begin = clock();
		while (unprocessingTime > frameTime)
		{
			isRenderRequired = true;
			if (renderer->IsCloseRequested())
			{
				Stop();
				return;
			}
			PollEvents();
			game->Input(frameTime);
			//Input::Update();
			game->Update(frameTime);
			unprocessingTime -= frameTime;
		}
		end = clock();
		countStats2++;
		timeSum2 += static_cast<double>(1000.0 * (end - begin)) / CLOCKS_PER_SEC; // in [ms]
		
		begin = clock();
		if (isRenderRequired)
		{
			//this->renderer->ClearScreen();
			//this->renderer->Render();
			//Shader* shader = this->game->GetShader();
			//if (shader == NULL)
			//{
			//	stdlog(Error, LOGPLACE, "Shader instance is NULL");
			//}
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
		end = clock();
		countStats3++;
		timeSum3 += static_cast<double>(1000.0 * (end - begin)) / CLOCKS_PER_SEC; // in [ms]
	}
}

unsigned int CoreEngine::NextCamera()
{
	ASSERT(renderer != NULL);
	if (this->renderer == NULL)
	{
		stdlog(Critical, LOGPLACE, "Renderer is not yet initialized");
		exit(EXIT_FAILURE);
	}
	return renderer->NextCamera();
}

unsigned int CoreEngine::PrevCamera()
{
	ASSERT(renderer != NULL);
	if (renderer == NULL)
	{
		stdlog(Critical, LOGPLACE, "Renderer is not yet initialized");
		exit(EXIT_FAILURE);
	}
	return renderer->PrevCamera();
}

void CoreEngine::PollEvents()
{
	glfwPollEvents();
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
