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

unsigned int testNumber = 0;

void ReportError(const std::string& reportStr)
{
	stdlog(Error, LOGPLACE, "Test #%d FAILED. %s", testNumber, reportStr.c_str());
}

void TestReport(bool error, const std::string& reportErrorStr)
{
	testNumber++;
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
	bool angleTestEnabled = true;
	bool vectorTestEnabled = true;
	bool matrixTestEnabled = true;
	bool quaternionTestEnabled = true;
	bool otherTestEnabled = true;
	
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

	Matrix4D identityMatrix1 = Matrix4D::Identity();
	Matrix4D identityMatrix2 = Matrix4D::Identity();

	/* ==================== ANGLE TEST #1 begin ==================== */
	if (angleTestEnabled)
	{
		Angle angle1(90.0);
		Angle angle2(90.0, true);
		Angle angle3(static_cast<Real>(M_PI / 2.0), false); // angle1 == angle2 == angle3
		Angle angle4(static_cast<Real>(M_PI / 2.0), true); // angle4 is different than angle1, angle2, angle3
		TestReport(angle1 != angle2, "The comparison operators of the Angle class are incorrect. angle1 != angle2");
		TestReport(angle1 != angle3, "The comparison operators of the Angle class are incorrect. angle1 != angle3");
		TestReport(angle2 != angle3, "The comparison operators of the Angle class are incorrect. angle2 != angle3");
		TestReport(angle1 == angle4, "The comparison operators of the Angle class are incorrect. angle1 == angle4");
		TestReport(angle2 == angle4, "The comparison operators of the Angle class are incorrect. angle2 == angle4");
		TestReport(angle3 == angle4, "The comparison operators of the Angle class are incorrect. angle3 == angle4");
	}
	/* ==================== ANGLE TEST #1 end ==================== */

	/* ==================== MATRIX TEST #1 begin ==================== */
	if (matrixTestEnabled)
	{
		Matrix4D matrix1 = Matrix4D::Identity();
		TestReport(! matrix1.IsIdentity(), "The function Matrix::IsIdentity() failed.");
		
		outerBegin = clock();
		for (unsigned int i = 0; i < outerLoops; ++i)
		{
			for (unsigned int j = 0; j < innerLoops; ++j)
			{
				Matrix4D matrix2 = Matrix4D::Identity();
			}
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (CLOCKS_PER_SEC * innerLoops * outerLoops);
		TimeReport("Average time for identity matrix creation:\t", elapsedSecs);
	}
	/* ==================== MATRIX TEST #1 end ==================== */

	/* ==================== MATRIX TEST #2 begin ==================== */
	if (matrixTestEnabled)
	{
		outerBegin = clock();
		for (unsigned int i = 0; i < outerLoops; ++i)
		{
			for (unsigned int j = 0; j < innerLoops; ++j)
			{
				Matrix4D result = identityMatrix1 * identityMatrix2;
				ASSERT(result == identityMatrix1 * identityMatrix2);
			}
		}
		outerEnd = clock();
		elapsedSecs = static_cast<double>((outerEnd - outerBegin)) / (CLOCKS_PER_SEC * innerLoops * outerLoops);
		TimeReport("Average time for identity matrices multiplication:\t", elapsedSecs);
	}
	/* ==================== MATRIX TEST #2 end ==================== */

	return 0;
}
