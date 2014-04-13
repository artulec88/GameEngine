/* MATH begin */
#include "Math\Math.h"
#include "Math\Vector.h"
#include "Math\Angle.h"
#include "Math\Quaternion.h"
/* MATH end */

/* UTILITY begin */
#include "Utility\CommandLine.h"
#include "Utility\Config.h"
#include "Utility\Log.h"
/* UTILITY end */

///* RENDERING begin */
//#include "Rendering\Renderer.h"
#include "Rendering\CoreEngine.h"
#include "Rendering\Vertex.h"
///* RENDERING end */

#include "TestGame.h"
#include "TestGame2.h"

using namespace std;
using namespace Math;
using namespace Utility;
using namespace Rendering;

void PrintHelp()
{
	cout << "usage: GameEngine.exe [OPTIONS]" << endl;
	cout << "\t-log [Critical, Emergency, Error, Warning, Notice, Info, Debug, Delocust]" << endl;
	cout << "\t-config ConfigFilePath" << endl;
	cout << endl;
}

int main (int argc, char* argv[])
{
	/* ==================== Reading settings and parameters begin ==================== */
	CommandLine* commandLine = new CommandLine(argc, argv);
	if (commandLine->IsPresent("-help"))
	{
		PrintHelp();
		system("pause");
		return 0;
	}
	stdlog.Fill(commandLine->Get("-log", ""), Debug);
	Config::LoadFromFile(commandLine->Get("-config", "..\\Config\\Config.cfg"));
	SAFE_DELETE(commandLine);
	/* ==================== Reading settings and parameters end ==================== */

	/* ==================== Creating game instance and run ==================== */
	Game* game = new TestGame();
	CoreEngine engine(GET_CONFIG_VALUE("windowWidth", "windowWidth_default", 800), GET_CONFIG_VALUE("windowHeight", "windowHeight_default", 600),
		GET_CONFIG_VALUE_STR("windowTitle", "windowTitle_default", "Default window title"), GET_CONFIG_VALUE("FPScap", "FPScap_default", 30), game);
	engine.Start();
	return 0;
}