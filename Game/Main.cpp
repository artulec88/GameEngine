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
//#include <vld.h> // This is going to cause huge (!!!) performance issues, but is very helpful in detecting memory leaks in the application
#endif

using namespace Math;
using namespace Utility;
using namespace Rendering;

void PrintHelp()
{
	std::cout << "usage: GameEngine.exe [OPTIONS]" << std::endl;
	std::cout << "\t-log LoggingLevel" << std::endl;
	std::cout << "\t-config ConfigFilePath" << std::endl;
	std::cout << "\t-textures TexturesDirectory" << std::endl;
	std::cout << "\t-shaders ShadersDirectory" << std::endl;
	std::cout << "\t-models ModelsDirectory" << std::endl;
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

	/* ==================== Initializing logger begin ==================== */
	std::string loggingLevel = "Info";
	if (ICommand::GetCommand().IsPresent("-log"))
	{
		loggingLevel = ICommand::GetCommand().Get("-log", "Info");
	}
	else
	{
		loggingLevel = GET_CONFIG_VALUE_STR("LoggingLevel", "Info");
	}
	ILogger::GetLogger().Fill(loggingLevel, Info);
	/* ==================== Initializing logger end ==================== */

	/* ==================== Initializing shader directory begin ==================== */
	std::string shaderDirectory = "..\\Shaders\\";
	if (ICommand::GetCommand().IsPresent("-shaders"))
	{
		shaderDirectory = ICommand::GetCommand().Get("-shaders", "..\\Shaders\\");
	}
	else
	{
		shaderDirectory = GET_CONFIG_VALUE_STR("shadersDirectory", "..\\Shaders\\");
	}
	/* ==================== Initializing shader directory end ==================== */

	/* ==================== Initializing mesh / models directory begin ==================== */
	std::string modelsDirectory = "..\\Models\\";
	if (ICommand::GetCommand().IsPresent("-models"))
	{
		modelsDirectory = ICommand::GetCommand().Get("-models", "..\\Models\\");
	}
	else
	{
		modelsDirectory = GET_CONFIG_VALUE_STR("modelsDirectory", "..\\Models\\");
	}
	/* ==================== Initializing mesh / models directory end ==================== */

	/* ==================== Initializing textures directory begin ==================== */
	std::string texturesDirectory = "..\\Textures\\";
	if (ICommand::GetCommand().IsPresent("-textures"))
	{
		texturesDirectory = ICommand::GetCommand().Get("-textures", "..\\Textures\\");
	}
	else
	{
		texturesDirectory = GET_CONFIG_VALUE_STR("texturesDirectory", "..\\Textures\\");
	}
	/* ==================== Initializing textures directory end ==================== */

	ICommand::DeleteCommand();
	/* ==================== Reading settings and parameters end ==================== */

	/* ==================== Creating game instance and run ==================== */
	Game::TestGameManager game;
	std::string windowTitle = GET_CONFIG_VALUE_STR("windowTitle", "Default window title");
	CoreEngine engine(GET_CONFIG_VALUE("windowWidth", 1024), GET_CONFIG_VALUE("windowHeight", 600),
		windowTitle.c_str(), GET_CONFIG_VALUE("FPScap", 200), game, shaderDirectory, modelsDirectory, texturesDirectory);
	engine.Start();
	return EXIT_SUCCESS;
}