/* MATH begin */
#include "Math\Math.h"
#include "Math\Point.h"
/* MATH end */

/* UTILITY begin */
#include "Utility\CommandLine.h"
/* UTILITY end */

using namespace std;
using namespace Math;
using namespace Utility;

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
	float value1 = -3.1415f;
	double value2 = 3.1415f;
	cout << "value1 = " << value1 << "; Abs(value1) = " << Absolute(value1) << endl;
	cout << "value2 = " << value2 << "; Abs(value2) = " << Absolute(value2) << endl;
	cout << "ToDeg(value1) = " << ToDeg(value1) << endl;
	cout << "ToDeg(value2) = " << ToDeg(value2) << endl;
	cout << "ToRad(value1) = " << ToRad(value1) << endl;
	cout << "ToRad(value2) = " << ToRad(value2) << endl;

	Point p(4.3, 15.6);
	cout << p.ToString() << endl;
	/* Testing end */

	CommandLine commands(argc, argv);
	if (commands.IsPresent("-help"))
	{
		PrintHelp();
		system("pause");
		return 0;
	}
	//stdlog.Fill(commands.Get("-log", ""), Debug);
	//Config::LoadFromFile(commands.Get("-config", "C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\Robocup\\x64\\Debug\\Config.cfg"));

	system("pause");
	return 0;
}