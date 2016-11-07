#include "Def.h"

#include "Rendering\CameraBuilder.h"
#include "Rendering\LightBuilder.h"
#include "Rendering\ShaderFactory.h"
#include "Rendering\ParticlesSystemBuilder.h"

#include "Math\StatisticsStorage.h"
#include "Math\RandomGeneratorFactory.h"

#include "Utility\ICommandLineMapper.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"
#include "Utility\Time.h"

#include <ctime>
#include <string>
//#include <thread>
//#include <xmmintrin.h>
//#include <iostream>
//#include <fstream>

using namespace Rendering;
using namespace Math;
using namespace Utility;
using namespace std;

const std::string MODULE_NAME = "MathTest";
const Math::Random::RandomGenerator& g_randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE);

unsigned int testNumber = 0;
bool meshTestEnabled = true;
bool textureTestEnabled = true;
bool cameraBuilderTestEnabled = true;
bool lightBuilderTestEnabled = true;
bool particlesSystemBuilderTestEnabled = true;
bool otherTestsEnabled = true;

void ReportError(const std::string& reportStr)
{
	ERROR_LOG_RENDERING_TEST("Test #", testNumber, " FAILED. ", reportStr);
}

void TestReport(bool statusCode /* false if error */, const std::string& reportErrorStr)
{
	testNumber++;
	if (statusCode == false)
	{
		ReportError(reportErrorStr);
	}
	INFO_LOG_RENDERING_TEST("Test #", testNumber, " passed");
}

void TimeReport(const std::string& reportStr, Timing::Timer& timer, Timing::TimeUnit timeUnit, const unsigned int NUMBER_OF_ITERATIONS = 1)
{
	//CHECK_CONDITION_EXIT_ALWAYS_MATH_TEST(!timer.IsRunning(), Logging::ERR, "Timer is still running");
	double elapsedTime = static_cast<double>(timer.GetDuration(timeUnit));
	elapsedTime /= NUMBER_OF_ITERATIONS;
	//timeSpan.AdjustUnitToValue();
	NOTICE_LOG_RENDERING_TEST(reportStr, ":\t", elapsedTime, " ", Timing::DateTime::ConvertTimeUnitToString(timeUnit));
}

void CameraBuilderTest()
{
	if (!cameraBuilderTestEnabled)
	{
		return;
	}

	OrthoCameraBuilder orthoCameraBuilder;
	BuilderDirector<Camera> cameraBuilderDirector(&orthoCameraBuilder);
	Rendering::Camera camera = cameraBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(camera);
	camera = orthoCameraBuilder.SetPos(4.0f, 4.0f, 0.0f).SetNearPlane(0.1f).SetFarPlane(10000.0f).Get();
	NOTICE_LOG_RENDERING_TEST(camera);

	PerspectiveCameraBuilder persepectiveCameraBuilder;
	persepectiveCameraBuilder.SetPos(3.0f, 15.0f, -3.0f);
	cameraBuilderDirector.SetBuilder(&persepectiveCameraBuilder);
	camera = cameraBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(camera);
}

void LightBuilderTest()
{
	if (!lightBuilderTestEnabled)
	{
		return;
	}

	//ShaderFactory shaderFactory("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Shaders\\");

	//DirectionalLightBuilder directionalLightBuilder(shaderFactory);
	//BuilderDirector<Lighting::DirectionalLight> directionalLightBuilderDirector(&directionalLightBuilder);
}

void ParticlesSystemBuilderTest()
{
	if (!particlesSystemBuilderTestEnabled)
	{
		return;
	}

	Particles::ParticlesSystemBuilder particlesSystemBuilder;
	BuilderDirector<Particles::ParticlesSystem> particlesSystemBuilderDirector(&particlesSystemBuilder);
	Particles::ParticlesSystem particlesSystem = particlesSystemBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(particlesSystem);

	particlesSystemBuilder.SetMaxCount(10).SetAttributesMask(Particles::Attributes::POSITION | Particles::Attributes::COLOR).SetTextureID(TextureIDs::INVALID).SetShaderID(ShaderIDs::PARTICLES_COLORS);
	particlesSystem = particlesSystemBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(particlesSystem);
}

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	std::unique_ptr<ICommandLineMapper> commandLineMapper = ICommandLineMapper::CreateCommandLineMapper(argc, argv);
	//if (commandLineMapper.IsPresent("-help"))
	//{
	//	PrintHelp();
	//	system("pause");
	//	return 0;
	//}
	Logging::ILogger::GetLogger(MODULE_NAME).Fill(commandLineMapper->Get("-log", ""), Logging::INFO);
	Logging::ILogger::GetLogger("Rendering").Fill(commandLineMapper->Get("-logRendering", "Info"), Logging::INFO);
	Logging::ILogger::GetLogger("Math").Fill(commandLineMapper->Get("-logMath", "Info"), Logging::INFO);
	Logging::ILogger::GetLogger("Utility").Fill(commandLineMapper->Get("-logUtility", "Info"), Logging::INFO);

	IConfig::CreateConfig("Rendering", commandLineMapper->Get("-configRendering", "..\\..\\Config\\ConfigRendering.cfg"));

	STATS_STORAGE.StartTimer();

	//MeshTest();
	//TextureTest();
	CameraBuilderTest();
	LightBuilderTest();
	ParticlesSystemBuilderTest();
	//OtherTests();

	STATS_STORAGE.StopTimer();
	STATS_STORAGE.PrintReport();

	Logging::ILogger::GetLogger(MODULE_NAME).ResetConsoleColor();
	std::cout << "Bye!" << std::endl;

	return 0;
}
