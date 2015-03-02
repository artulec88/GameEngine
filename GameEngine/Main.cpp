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

#include "TestGame.h"
#include "TestGame2.h"

#ifdef _DEBUG
#include <vld.h> // This is going to cause huge (!!!) performance issues, but is very helpful in detecting memory leaks in the application
#endif

using namespace std;
using namespace Math;
using namespace Utility;
using namespace Rendering;

void PrintHelp()
{
	cout << "usage: GameEngine.exe [OPTIONS]" << endl;
	cout << "\t-config ConfigFilePath" << endl;
	cout << endl;
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
	//stdlog.Fill(Command->Get("-log", ""), Debug);
	IConfig::GetConfig().LoadFromFile(ICommand::GetCommand().Get("-config", "..\\Config\\Config.cfg"));
	const std::string logLevel = GET_CONFIG_VALUE_STR("LoggingLevel", "Info");
	ILogger::GetLogger().Fill(logLevel, Debug);
	Camera::InitializeCameraSensitivity();
	ICommand::DeleteCommand();
	/* ==================== Reading settings and parameters end ==================== */

	/* ==================== Creating game instance and run ==================== */
	Game* game = new GameNamespace::TestGame();
	std::string windowTitle = GET_CONFIG_VALUE_STR("windowTitle", "Default window title");
	CoreEngine engine(GET_CONFIG_VALUE("windowWidth", 800), GET_CONFIG_VALUE("windowHeight", 600),
		windowTitle.c_str(), GET_CONFIG_VALUE("FPScap", 30), game);
	engine.Start();
	return EXIT_SUCCESS;
}