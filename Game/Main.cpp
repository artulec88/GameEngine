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
#include "Engine\CoreEngine.h"
#include "Rendering\Camera.h"
//#include "Rendering\Vertex.h"
///* RENDERING end */

#include "TestGameManager.h"
#include "Def.h"

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

void ReadSettingsAndParameters(int argc, char* argv[], std::string* shaderDirectory, std::string* modelsDirectory, std::string* texturesDirectory, std::string* fontsDirectory, std::string* audioDirectory)
{
	std::unique_ptr<ICommand> command = ICommand::CreateCommand(argc, argv);
	if (command->IsPresent("-help"))
	{
		PrintHelp();
		system("pause");
		exit(EXIT_SUCCESS);
	}
	IConfig::CreateConfig("Audio", command->Get("-configAudio", "..\\Config\\ConfigAudio.cfg"));
	IConfig::CreateConfig("Physics", command->Get("-configPhysics", "..\\Config\\ConfigPhysics.cfg"));
	IConfig::CreateConfig("Rendering", command->Get("-configRendering", "..\\Config\\ConfigRendering.cfg"));
	IConfig::CreateConfig("Engine", command->Get("-configEngine", "..\\Config\\ConfigEngine.cfg"));
	IConfig::CreateConfig("Game", command->Get("-configGame", "..\\Config\\ConfigGame.cfg"));
	IConfig::CreateConfig("Math", command->Get("-configMath", "..\\Config\\ConfigMath.cfg"));
	IConfig::CreateConfig("Utility", command->Get("-configUtility", "..\\Config\\ConfigUtility.cfg"));

	/* ==================== Initializing logger begin ==================== */
	std::string loggingLevel = "Info";
	if (command->IsPresent("-log"))
	{
		loggingLevel = command->Get("-log", "Info");
	}
	else
	{
		loggingLevel = GET_CONFIG_VALUE_STR_GAME("LoggingLevel", "Info");
	}
	ILogger::GetLogger("Game").Fill(loggingLevel, Info);
	/* ==================== Initializing logger end ==================== */

	// Initializing shader directory
	*shaderDirectory = (command->IsPresent("-shaders")) ? command->Get("-shaders", "..\\Shaders\\") : GET_CONFIG_VALUE_STR_GAME("shadersDirectory", "..\\Shaders\\");

	// Initializing mesh / models directory
	*modelsDirectory = (command->IsPresent("-models")) ? command->Get("-models", "..\\Models\\") : GET_CONFIG_VALUE_STR_GAME("modelsDirectory", "..\\Models\\");

	// Initializing textures directory
	*texturesDirectory = (command->IsPresent("-textures")) ? command->Get("-textures", "..\\Textures\\") : GET_CONFIG_VALUE_STR_GAME("texturesDirectory", "..\\Textures\\");

	// Initializing fonts directory end
	*fontsDirectory = (command->IsPresent("-fonts")) ? command->Get("-fonts", "..\\Fonts\\") : GET_CONFIG_VALUE_STR_GAME("fontsDirectory", "..\\Fonts\\");

	// Initializing audio directory
	*audioDirectory = (command->IsPresent("-audio")) ? command->Get("-audio", "..\\Sounds\\") : GET_CONFIG_VALUE_STR_GAME("audioDirectory", "..\\Sounds\\");
}

// One has to remember that if we call some method foo(const std::string& text) as follows:
// foo("bla") then it will most likely not allocate new memory for the string "bla". However if the string were a little longer, e.g. if we call foo("veryLongString")
// then most likely it will allocate new memory for the string "veryLongString". This will cause "new" operator to be invoked.

int main(int argc, char* argv[])
{
	std::string shaderDirectory, modelsDirectory, texturesDirectory, fontsDirectory, audioDirectory;
	ReadSettingsAndParameters(argc, argv, &shaderDirectory, &modelsDirectory, &texturesDirectory, &fontsDirectory, &audioDirectory);
	/* ==================== Create game instance and run ==================== */
	std::string windowTitle = GET_CONFIG_VALUE_STR_GAME("windowTitle", "Default window title");
	Engine::CoreEngine engine(GET_CONFIG_VALUE_GAME("windowWidth", 1024), GET_CONFIG_VALUE_GAME("windowHeight", 600),
		windowTitle.c_str(), GET_CONFIG_VALUE_GAME("FPScap", 200), shaderDirectory, modelsDirectory, texturesDirectory, fontsDirectory, audioDirectory);
	Game::TestGameManager game;
	engine.Start(&game);
	return EXIT_SUCCESS;
}