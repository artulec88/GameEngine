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
	// TODO: Create testing function
	/* Testing begin */
	//float value1 = -3.1415f;
	//double value2 = 3.1415f;
	//cout << "value1 = " << value1 << "; Abs(value1) = " << Absolute(value1) << endl;
	//cout << "value2 = " << value2 << "; Abs(value2) = " << Absolute(value2) << endl;
	//cout << "ToDeg(value1) = " << ToDeg(value1) << endl;
	//cout << "ToDeg(value2) = " << ToDeg(value2) << endl;
	//cout << "ToRad(value1) = " << ToRad(value1) << endl;
	//cout << "ToRad(value2) = " << ToRad(value2) << endl;

	//Vector2D vec2D(4.3, 15.6);
	//cout << vec2D.ToString() << endl;

	//Vector3D vec3D(4.3, 15.6, 17.0);
	//cout << vec3D.ToString() << endl;

	//Vector3D vec3D_2(vec3D);
	//cout << vec3D_2.ToString() << endl;

	//Vertex vertex(vec3D, vec2D, vec3D_2);
	//cout << "Vertex.ToString() = " << vertex.ToString() << endl;

	//ASSERT(value1 > 0.0f);
	/* Testing end */

	//Quaternion q(0.3, -0.8, 0.4, 1.0);
	//cout << q.GetForward().ToString() << endl;

	//Angle angleX(0.0f);
	//Angle angleY(0.0f);
	//Angle angleZ(180.0f);
	//Quaternion qX(Vector3D(1, 0, 0), angleX);
	//Quaternion qY(Vector3D(0, 1, 0), angleY);
	//Quaternion qZ(Vector3D(0, 0, 1), angleZ);
	//Quaternion r = qX * qY * qZ;
	//cout << qX.ToString() << endl;
	//cout << qY.ToString() << endl;
	//cout << qZ.ToString() << endl;
	//cout << r.ToString() << endl;
	//return 0;

	CommandLine commandLine(argc, argv);
	if (commandLine.IsPresent("-help"))
	{
		PrintHelp();
		system("pause");
		return 0;
	}
	stdlog.Fill(commandLine.Get("-log", ""), Debug);
	Config::LoadFromFile(commandLine.Get("-config", "..\\Config\\Config.cfg"));

	//Renderer renderer;
	Game* game = new TestGame();
	CoreEngine engine(Config::Get("windowWidth", 800), Config::Get("windowHeight", 600),
		Config::Get<string>("windowTitle", "Default window title"), Config::Get("FPS_cap", 30), game);
	engine.Start();

	//system("pause");
	return 0;
}