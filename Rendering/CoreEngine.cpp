#include "StdAfx.h"
#include "CoreEngine.h"
#include "Input.h"
#include "tinythread.h"

#include "Utility\ILogger.h"
#include "Utility\Config.h"

#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
//#include <GLFW\glfw3.h>

using namespace Rendering;
using namespace Utility;
using namespace std;

CoreEngine* CoreEngine::coreEngine = NULL;

/* static */ CoreEngine* CoreEngine::GetCoreEngine()
{
	return coreEngine;
}

CoreEngine::CoreEngine(int width, int height, const char* title, int maxFrameRate, Game* game) :
	isRunning(false),
	windowWidth(width),
	windowHeight(height),
	windowTitle(title),
	frameTime(1.0f / maxFrameRate),
	game(game),
	renderer(NULL),
	fpsTextRenderer(NULL)
{
	// TODO: Fix singleton initialization
	LOG(Debug, LOGPLACE, "Main application construction started");
	if (coreEngine != NULL)
	{
		LOG(Error, LOGPLACE, "Constructor called when a singleton instance of MainApp class has already been created");
		delete coreEngine;
		coreEngine = NULL;
	}
	coreEngine = this;
	
	game->SetEngine(this);

	CreateRenderer(width, height, title);

	fpsTextRenderer = new TextRenderer(new Texture("..\\Textures\\Holstein.tga", GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, true, GL_COLOR_ATTACHMENT0), 16.0f);
	
	LOG(Debug, LOGPLACE, "Main application construction finished");
}


CoreEngine::~CoreEngine(void)
{
	LOG(Debug, LOGPLACE, "Core engine destruction started");

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
	SAFE_DELETE(fpsTextRenderer);

	LOG(Notice, LOGPLACE, "Core engine destruction finished");
	ILogger::GetLogger().ResetConsoleColor();
	std::cout << "Bye!" << std::endl;
}

void CoreEngine::CreateRenderer(int width, int height, const std::string& title)
{
	GLFWwindow* window = Rendering::InitGraphics(width, height, title);
	this->renderer = new Renderer(window);

	if (this->renderer == NULL)
	{
		LOG(Critical, LOGPLACE, "Failed to create a Renderer");
		exit(-1);
	}

	ASSERT(this->renderer != NULL);
}

void CoreEngine::Start()
{
	if (isRunning)
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
	if (!isRunning)
	{
		LOG(Warning, LOGPLACE, "The core engine instance is not running");
		return;
	}
	
	isRunning = false;
	ASSERT(!isRunning);
	LOG(Notice, LOGPLACE, "The core engine has stopped");
	
	/* ==================== Printing stats begin ==================== */
	LOG(Info, LOGPLACE, "The region #1 (Time calculating) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats1, timeSum1, timeSum1 / countStats1, minMaxTime1.ToString().c_str());
	LOG(Info, LOGPLACE, "The region #2 was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats2, timeSum2, timeSum2 / countStats2, minMaxTime2.ToString().c_str());
	LOG(Info, LOGPLACE, "\t The region #2_1 (Polling events) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats2_1, timeSum2_1, timeSum2_1 / countStats2_1, minMaxTime2_1.ToString().c_str());
	LOG(Info, LOGPLACE, "\t The region #2_2 (Game input processing) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats2_2, timeSum2_2, timeSum2_2 / countStats2_2, minMaxTime2_2.ToString().c_str());
	LOG(Info, LOGPLACE, "\t The region #2_3 (Game updating) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats2_3, timeSum2_3, timeSum2_3 / countStats2_3, minMaxTime2_3.ToString().c_str());
	LOG(Info, LOGPLACE, "The region #3 (Rendering) was processed %d times, which took exactly %.2f [us]. The average time=%.2f [us]. %s", countStats3, timeSum3, timeSum3 / countStats3, minMaxTime3.ToString().c_str());
	/* ==================== Printing stats end ==================== */
}

void CoreEngine::Run()
{
	const Math::Real ONE_MILLION = static_cast<Math::Real>(1000000.0);

	minMaxTime1.Init();
	minMaxTime2.Init();
	minMaxTime2_1.Init();
	minMaxTime2_2.Init();
	minMaxTime2_3.Init();
	minMaxTime3.Init();
	
	LOG(Notice, LOGPLACE, "The game started running");
	ASSERT(!isRunning);

	game->Init();
#ifdef ANT_TWEAK_BAR_ENABLED
	Rendering::InitializeTweakBars();
	renderer->InitializeTweakBars();
	game->InitializeTweakBars();
#endif

	isRunning = true;

#ifdef COUNT_FPS
	Math::Real fpsSample = static_cast<Math::Real>(GET_CONFIG_VALUE("FPSsample", REAL_ONE)); // represents the time after which FPS value is calculated and logged
	int framesCount = 0;
	Math::Real frameTimeCounter = 0.0;
	int fps;
	Math::Real spf;
#endif

	Math::Real unprocessingTime = 0.0; // used to cap the FPS when it gets too high
	Math::Real previousTime = GetTime();

	LARGE_INTEGER frequency; // ticks per second
	QueryPerformanceFrequency(&frequency); // get ticks per second;
	LARGE_INTEGER t1, t2, innerT1, innerT2; // ticks

	while (isRunning)

	{
		/* ==================== REGION #1 begin ====================*/
		QueryPerformanceCounter(&t1); // start timer
		bool isRenderRequired = false;

		// flCurrentTime will be lying around from last frame. It's now the previous time.
		Math::Real currentTime = GetTime();
		Math::Real passedTime = currentTime - previousTime;

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
			LOG(Info, LOGPLACE, "FPS = %5d\t Average time per frame = %.3f [ms]", fps, spf);
			framesCount = 0;
			frameTimeCounter = 0.0;
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
		while (unprocessingTime > frameTime)
		{
			//previousTime = GetTime();
			isRenderRequired = true;
			if (renderer->IsCloseRequested())
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
			game->Input(frameTime);
			QueryPerformanceCounter(&innerT2);
			countStats2_2++;
			elapsedTime = static_cast<double>(ONE_MILLION * (innerT2.QuadPart - innerT1.QuadPart)) / frequency.QuadPart; // in [us]
			minMaxTime2_2.ProcessTime(elapsedTime);
			timeSum2_2 += elapsedTime;
			/* ==================== REGION #2_2 end ====================*/
			
			//Input::Update();
			
			/* ==================== REGION #2_3 begin ====================*/
			QueryPerformanceCounter(&innerT1);
			game->Update(frameTime);
			QueryPerformanceCounter(&innerT2);
			countStats2_3++;
			elapsedTime = static_cast<double>(ONE_MILLION * (innerT2.QuadPart - innerT1.QuadPart)) / frequency.QuadPart; // in [us]
			minMaxTime2_3.ProcessTime(elapsedTime);
			timeSum2_3 += elapsedTime;
			/* ==================== REGION #2_3 end ====================*/

#ifdef ANT_TWEAK_BAR_ENABLED
			Rendering::CheckChangesAndUpdateGLState();
#endif
			
			unprocessingTime -= frameTime;
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
			//this->renderer->ClearScreen();
			//this->renderer->Render();
			//Shader* shader = this->game->GetShader();
			//if (shader == NULL)
			//{
			//	LOG(Error, LOGPLACE, "Shader instance is NULL");
			//}
			//this->renderer->Render(this->game->GetRootGameNode());
			game->Render(renderer);
#ifdef COUNT_FPS
			++framesCount;
			//double time = glfwGetTime();
			std::stringstream ss;
			ss << "FPS = " << fps << " SPF[ms] = " << std::setprecision(4) << spf;
			fpsTextRenderer->DrawString(0, 570, ss.str(), renderer);
			//fpsTextRenderer->DrawString(static_cast<Math::Real>(windowWidth / 4), static_cast<Math::Real>(windowHeight / 2) + 100.0f, "Start", renderer, 64.0f);
			//fpsTextRenderer->DrawString(static_cast<Math::Real>(windowWidth / 4), static_cast<Math::Real>(windowHeight / 2), "Options", renderer, 64.0f);
			//fpsTextRenderer->DrawString(static_cast<Math::Real>(windowWidth / 4), static_cast<Math::Real>(windowHeight / 2) - 100.0f, "Exit", renderer, 64.0f);
#endif
#ifdef ANT_TWEAK_BAR_ENABLED
			glDisable(GL_DEPTH_TEST);
			TwDraw();
			glEnable(GL_DEPTH_TEST);
#endif
			this->renderer->SwapBuffers();
		}
		else
		{
			//LOG(Info, LOGPLACE, "Rendering is not required. Moving on...");
			// TODO: Sleep for 1ms to prevent the thread from constant looping
			//this_thread::sleep_for(chrono::milliseconds(100));
			tthread::this_thread::sleep_for(tthread::chrono::milliseconds(10));
		}
		QueryPerformanceCounter(&t2);
		countStats3++;
		elapsedTime = static_cast<double>(ONE_MILLION * (t2.QuadPart - t1.QuadPart)) / frequency.QuadPart; // in [us]
		minMaxTime3.ProcessTime(elapsedTime);
		timeSum3 += elapsedTime; // in [ms]
		/* ==================== REGION #3 end ====================*/
	}
}

unsigned int CoreEngine::GetCurrentCameraIndex() const
{
	ASSERT(renderer != NULL);
	if (this->renderer == NULL)
	{
		LOG(Critical, LOGPLACE, "Renderer is not yet initialized");
		exit(EXIT_FAILURE);
	}
	return renderer->GetCurrentCameraIndex();
}

unsigned int CoreEngine::NextCamera() const
{
	ASSERT(renderer != NULL);
	if (this->renderer == NULL)
	{
		LOG(Critical, LOGPLACE, "Renderer is not yet initialized");
		exit(EXIT_FAILURE);
	}
	return renderer->NextCamera();
}

unsigned int CoreEngine::PrevCamera() const
{
	ASSERT(renderer != NULL);
	if (renderer == NULL)
	{
		LOG(Critical, LOGPLACE, "Renderer is not yet initialized");
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
