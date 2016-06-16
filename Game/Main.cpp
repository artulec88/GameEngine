/* UTILITY includes begin */
#include "Utility\ILogger.h"
#include "Utility\ICommand.h"
#include "Utility\IConfig.h"
/* UTILITY includes end */

/* ENGINE includes begin */
#include "Engine\CoreEngine.h"
/* ENGINE includes end */

/* GAME includes begin */
#include "TestGameManager.h"
#include "Def.h"
/* GAME includes end */

#ifdef _DEBUG
//#include <vld.h> // This is going to cause huge (!!!) performance issues, but is very helpful in detecting memory leaks in the application
#endif

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
	std::unique_ptr<Utility::ICommand> command = Utility::ICommand::CreateCommand(argc, argv);
	if (command->IsPresent("-help"))
	{
		PrintHelp();
		system("pause");
		exit(EXIT_SUCCESS);
	}
	Utility::IConfig::CreateConfig("Audio", command->Get("-configAudio", "..\\Config\\ConfigAudio.cfg"));
	Utility::IConfig::CreateConfig("Physics", command->Get("-configPhysics", "..\\Config\\ConfigPhysics.cfg"));
	Utility::IConfig::CreateConfig("Rendering", command->Get("-configRendering", "..\\Config\\ConfigRendering.cfg"));
	Utility::IConfig::CreateConfig("Engine", command->Get("-configEngine", "..\\Config\\ConfigEngine.cfg"));
	Utility::IConfig::CreateConfig("Game", command->Get("-configGame", "..\\Config\\ConfigGame.cfg"));
	Utility::IConfig::CreateConfig("Math", command->Get("-configMath", "..\\Config\\ConfigMath.cfg"));
	Utility::IConfig::CreateConfig("Utility", command->Get("-configUtility", "..\\Config\\ConfigUtility.cfg"));

	// Initializing logger
	Utility::ILogger::GetLogger("Game").Fill((command->IsPresent("-log")) ? command->Get("-log", "Info") : GET_CONFIG_VALUE_STR_GAME("LoggingLevel", "Info"), Utility::Info);

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
	Engine::CoreEngine engine(GET_CONFIG_VALUE_GAME("windowWidth", 1024), GET_CONFIG_VALUE_GAME("windowHeight", 600),
		GET_CONFIG_VALUE_STR_GAME("windowTitle", "Default window title").c_str(), GET_CONFIG_VALUE_GAME("FPScap", 200),
		shaderDirectory, modelsDirectory, texturesDirectory, fontsDirectory, audioDirectory);
	Game::TestGameManager game;
	engine.Start(&game);
	return EXIT_SUCCESS;
}