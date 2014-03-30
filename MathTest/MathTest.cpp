#include "Math\Angle.h"
#include "Math\FloatingPoint.h"
#include "Math\Matrix.h"
#include "Math\Quaternion.h"
#include "Math\Vector.h"

#include "Utility\CommandLine.h"
#include "Utility\Log.h"
#include "Utility\Time.h"

#include <ctime>
#include <string>

using namespace Math;
using namespace Utility;
using namespace std;

unsigned int testNumber = 1;

void ReportError(const std::string& reportStr)
{
	stdlog(Error, LOGPLACE, "Test #%d FAILED. %s", testNumber, reportStr.c_str());
}

void TestReport(bool error, const std::string& reportErrorStr)
{
	if (error == true)
	{
		ReportError(reportErrorStr);
	}
	stdlog(Info, LOGPLACE, "Test #%d passed", testNumber);
}

void TimeReport(const std::string& reportStr, double elapsedSecs)
{
	stdlog(Notice, LOGPLACE, "%s:\t%.4f [us]", reportStr.c_str(), 1000000.0 * elapsedSecs);
}

int main (int argc, char* argv[])
{
	unsigned int innerLoops = 200;
	unsigned int outerLoops = 200;
	clock_t outerBegin, outerEnd;
	double elapsedSecs;

	CommandLine commandLine(argc, argv);
	//if (commandLine.IsPresent("-help"))
	//{
	//	PrintHelp();
	//	system("pause");
	//	return 0;
	//}
	stdlog.Fill(commandLine.Get("-log", ""), Debug);

	/* ==================== MATRIX TEST #1 begin ==================== */
	Matrix4D matrix1 = Matrix4D::Identity();
	TestReport(! matrix1.IsIdentity(), "Matrix is not a identity matrix");
	
	outerBegin = clock();
	for (int i = 0; i < outerLoops; ++i)
	{
		for (int j = 0; j < innerLoops; ++j)
		{
			Matrix4D matrix2 = Matrix4D::Identity();
		}
	}
	outerEnd = clock();
	elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (CLOCKS_PER_SEC * innerLoops * outerLoops);
	TimeReport("Average time for identity matrix creation:\t", elapsedSecs);
	/* ==================== MATRIX TEST #1 end ==================== */

	return 0;
}