#include "Math\StatisticsStorage.h"

#include "Utility\ICommandLineMapper.h"
#include "Utility\ILogger.h"
#include "Utility\Time.h"

#include <ctime>
#include <string>
//#include <thread>
//#include <xmmintrin.h>
//#include <iostream>
//#include <fstream>

using namespace Math;
using namespace Utility;
using namespace std;

const std::string MODULE_NAME = "RenderingTest";

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

	STATS_STORAGE.StartTimer();

	//MeshTest();
	//TextureTest();
	//OtherTests();

	STATS_STORAGE.StopTimer();
	STATS_STORAGE.PrintReport();

	Logging::ILogger::GetLogger(MODULE_NAME).ResetConsoleColor();
	std::cout << "Bye!" << std::endl;

	return 0;
}
