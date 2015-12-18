/* MATH begin */
#include "Math\Math.h"
#include "Math\Vector.h"
#include "Math\Angle.h"
#include "Math\Quaternion.h"
/* MATH end */

/* UTILITY begin */
#include "Utility\ILogger.h"
#include "Utility\ICommand.h"
#include "Utility\IConfig.h"
/* UTILITY end */

///* RENDERING begin */
//#include "Rendering\Renderer.h"
#include "Rendering\CoreEngine.h"
#include "Rendering\Camera.h"
//#include "Rendering\Vertex.h"
///* RENDERING end */

#include "TestGameManager.h"

#ifdef _DEBUG
#include <vld.h> // This is going to cause huge (!!!) performance issues, but is very helpful in detecting memory leaks in the application
#endif

using namespace Math;
using namespace Utility;
using namespace Rendering;

void PrintHelp()
{
	std::cout << "usage: GameEngine.exe [OPTIONS]" << std::endl;
	std::cout << "\t-log LoggingLevel" << std::endl;
	std::cout << "\t-config ConfigFilePath" << std::endl;
	std::cout << std::endl;
}

int main (int argc, char* argv[])
{
	/* ==================== Reading settings and parameters begin ==================== */
	ICommand::SetCommand(argc, argv);
	if (ICommand::GetCommand().IsPresent("-help"))
	{
		PrintHelp();
		system("pause");
		return EXIT_SUCCESS;
	}
	IConfig::GetConfig().LoadFromFile(ICommand::GetCommand().Get("-config", "..\\Config\\Config.cfg"));
	const std::string logLevel = GET_CONFIG_VALUE_STR("LoggingLevel", "Info");
	ILogger::GetLogger().Fill(logLevel, Debug);
	CameraBase::InitializeCameraSensitivity();
	ICommand::DeleteCommand();
	/* ==================== Reading settings and parameters end ==================== */

	/* ==================== Creating game instance and run ==================== */
	Game::TestGameManager game;
	std::string windowTitle = GET_CONFIG_VALUE_STR("windowTitle", "Default window title");
	CoreEngine engine(GET_CONFIG_VALUE("windowWidth", 1024), GET_CONFIG_VALUE("windowHeight", 600),
		windowTitle.c_str(), GET_CONFIG_VALUE("FPScap", 200), game);
	engine.Start();
	return EXIT_SUCCESS;
}