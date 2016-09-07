/* UTILITY includes begin */
#include "Utility\ILogger.h"
#include "Utility\ICommandLineMapper.h"
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
	std::cout << "\t-configAudio AudioConfigFilePath" << std::endl;
	std::cout << "\t-configPhysics PhysicsConfigFilePath" << std::endl;
	std::cout << "\t-configRendering RenderingConfigFilePath" << std::endl;
	std::cout << "\t-configEngine EngineConfigFilePath" << std::endl;
	std::cout << "\t-configGame GameConfigFilePath" << std::endl;
	std::cout << "\t-configMath MathConfigFilePath" << std::endl;
	std::cout << "\t-configUtility UtilityConfigFilePath" << std::endl;
	std::cout << "\t-textures TexturesDirectory" << std::endl;
	std::cout << "\t-shaders ShadersDirectory" << std::endl;
	std::cout << "\t-models ModelsDirectory" << std::endl;
	std::cout << std::endl;
}

void ReadSettingsAndParameters(int argc, char* argv[], std::string* shaderDirectory, std::string* modelsDirectory, std::string* texturesDirectory, std::string* fontsDirectory, std::string* audioDirectory)
{
	std::unique_ptr<Utility::ICommandLineMapper> commandLineMapper = Utility::ICommandLineMapper::CreateCommandLineMapper(argc, argv);
	if (commandLineMapper->IsPresent("-help"))
	{
		PrintHelp();
		system("pause");
		exit(EXIT_SUCCESS);
	}
	Utility::IConfig::CreateConfig("Audio", commandLineMapper->Get("-configAudio", "..\\Config\\ConfigAudio.cfg"));
	Utility::IConfig::CreateConfig("Physics", commandLineMapper->Get("-configPhysics", "..\\Config\\ConfigPhysics.cfg"));
	Utility::IConfig::CreateConfig("Rendering", commandLineMapper->Get("-configRendering", "..\\Config\\ConfigRendering.cfg"));
	Utility::IConfig::CreateConfig("Engine", commandLineMapper->Get("-configEngine", "..\\Config\\ConfigEngine.cfg"));
	Utility::IConfig::CreateConfig("Game", commandLineMapper->Get("-configGame", "..\\Config\\ConfigGame.cfg"));
	Utility::IConfig::CreateConfig("Math", commandLineMapper->Get("-configMath", "..\\Config\\ConfigMath.cfg"));
	Utility::IConfig::CreateConfig("Utility", commandLineMapper->Get("-configUtility", "..\\Config\\ConfigUtility.cfg"));

	Utility::Logging::ILogger::GetLogger("Audio").Fill((commandLineMapper->IsPresent("-logAudio")) ? commandLineMapper->Get("-logAudio", "Info") :
		GET_CONFIG_VALUE_STR_GAME("LoggingLevelAudio", "Info"), Utility::Logging::INFO); // Initializing audio logger
	Utility::Logging::ILogger::GetLogger("Physics").Fill((commandLineMapper->IsPresent("-logPhysics")) ? commandLineMapper->Get("-logPhysics", "Info") :
		GET_CONFIG_VALUE_STR_GAME("LoggingLevelPhysics", "Info"), Utility::Logging::INFO); // Initializing physics logger
	Utility::Logging::ILogger::GetLogger("Rendering").Fill((commandLineMapper->IsPresent("-logRendering")) ? commandLineMapper->Get("-logRendering", "Info") :
		GET_CONFIG_VALUE_STR_GAME("LoggingLevelRendering", "Info"), Utility::Logging::INFO); // Initializing rendering logger
	Utility::Logging::ILogger::GetLogger("Engine").Fill((commandLineMapper->IsPresent("-logEngine")) ? commandLineMapper->Get("-logEngine", "Info") :
		GET_CONFIG_VALUE_STR_GAME("LoggingLevelEngine", "Info"), Utility::Logging::INFO); // Initializing engine logger
	Utility::Logging::ILogger::GetLogger("Game").Fill((commandLineMapper->IsPresent("-logGame")) ? commandLineMapper->Get("-logGame", "Info") :
		GET_CONFIG_VALUE_STR_GAME("LoggingLevelGame", "Info"), Utility::Logging::INFO); // Initializing game logger
	Utility::Logging::ILogger::GetLogger("Math").Fill((commandLineMapper->IsPresent("-logMath")) ? commandLineMapper->Get("-logMath", "Info") :
		GET_CONFIG_VALUE_STR_GAME("LoggingLevelMath", "Info"), Utility::Logging::INFO); // Initializing math logger
	Utility::Logging::ILogger::GetLogger("Utility").Fill((commandLineMapper->IsPresent("-logUtility")) ? commandLineMapper->Get("-logUtility", "Info") :
		GET_CONFIG_VALUE_STR_GAME("LoggingLevelUtility", "Info"), Utility::Logging::INFO); // Initializing utility logger

	// Initializing shader directory
	*shaderDirectory = (commandLineMapper->IsPresent("-shaders")) ? commandLineMapper->Get("-shaders", "..\\Shaders\\") : GET_CONFIG_VALUE_STR_GAME("shadersDirectory", "..\\Shaders\\");

	// Initializing mesh / models directory
	*modelsDirectory = (commandLineMapper->IsPresent("-models")) ? commandLineMapper->Get("-models", "..\\Models\\") : GET_CONFIG_VALUE_STR_GAME("modelsDirectory", "..\\Models\\");

	// Initializing textures directory
	*texturesDirectory = (commandLineMapper->IsPresent("-textures")) ? commandLineMapper->Get("-textures", "..\\Textures\\") : GET_CONFIG_VALUE_STR_GAME("texturesDirectory", "..\\Textures\\");

	// Initializing fonts directory end
	*fontsDirectory = (commandLineMapper->IsPresent("-fonts")) ? commandLineMapper->Get("-fonts", "..\\Fonts\\") : GET_CONFIG_VALUE_STR_GAME("fontsDirectory", "..\\Fonts\\");

	// Initializing audio directory
	*audioDirectory = (commandLineMapper->IsPresent("-audio")) ? commandLineMapper->Get("-audio", "..\\Sounds\\") : GET_CONFIG_VALUE_STR_GAME("audioDirectory", "..\\Sounds\\");
}

// One has to remember that if we call some method foo(const std::string& text) as follows:
// foo("bla") then it will most likely not allocate new memory for the string "bla". However if the string were a little longer, e.g. if we call foo("veryLongString")
// then most likely it will allocate new memory for the string "veryLongString". This will cause "new" operator to be invoked.

int main(int argc, char* argv[])
{
	std::string shaderDirectory, modelsDirectory, texturesDirectory, fontsDirectory, audioDirectory;
	ReadSettingsAndParameters(argc, argv, &shaderDirectory, &modelsDirectory, &texturesDirectory, &fontsDirectory, &audioDirectory);

	/* ==================== Create game instance and run ==================== */
	Engine::CoreEngine engine(GET_CONFIG_VALUE_GAME("fullscreenEnabled", false), GET_CONFIG_VALUE_GAME("windowWidth", 1024), GET_CONFIG_VALUE_GAME("windowHeight", 600),
		GET_CONFIG_VALUE_STR_GAME("windowTitle", "Default window title").c_str(), shaderDirectory, modelsDirectory, texturesDirectory, fontsDirectory, audioDirectory);
	Game::TestGameManager game;
	engine.Start(&game);
	return EXIT_SUCCESS;
}