#include "Math\Angle.h"
#include "Math\FloatingPoint.h"
#include "Math\Matrix.h"
#include "Math\Quaternion.h"
#include "Math\Vector.h"

#include "Utility\ICommand.h"
#include "Utility\ILogger.h"
#include "Utility\Time.h"

#include <ctime>
#include <string>

using namespace Math;
using namespace Utility;
using namespace std;

unsigned int innerLoops = 200;
unsigned int outerLoops = 200;
clock_t outerBegin, outerEnd;
double elapsedSecs;

unsigned int testNumber = 0;
bool angleTestEnabled = true;
bool vectorTestEnabled = true;
bool matrixTestEnabled = true;
bool quaternionTestEnabled = true;
bool otherTestEnabled = true;

void ReportError(const std::string& reportStr)
{
	LOG(Error, LOGPLACE, "Test #%d FAILED. %s", testNumber, reportStr.c_str());
}

void TestReport(bool statusCode /* false if error */, const std::string& reportErrorStr)
{
	testNumber++;
	if (statusCode == false)
	{
		ReportError(reportErrorStr);
	}
	LOG(Info, LOGPLACE, "Test #%d passed", testNumber);
}

void TimeReport(const std::string& reportStr, double elapsedSecs)
{
	LOG(Notice, LOGPLACE, "%s:\t%.4f [us]", reportStr.c_str(), 1000000.0 * elapsedSecs);
}

void AngleTest()
{
	if (!angleTestEnabled)
	{
		return;
	}
	/* ==================== ANGLE TEST #1- comparing angles- begin ==================== */
	Angle angle1(90.0);
	Angle angle2(90.0, Angle::Unit::DEGREE);
	Angle angle3(M_PI / 2.0f, Angle::Unit::RADIAN); // angle1 == angle2 == angle3
	Angle angle4(M_PI / 2.0f, Angle::Unit::DEGREE); // angle4 is different than angle1, angle2, angle3
	TestReport(angle1 == angle2, "The comparison operators of the Angle class are incorrect. angle1 == angle2");
	TestReport(angle1 == angle3, "The comparison operators of the Angle class are incorrect. angle1 == angle3");
	TestReport(angle2 == angle3, "The comparison operators of the Angle class are incorrect. angle2 == angle3");
	TestReport(angle1 != angle4, "The comparison operators of the Angle class are incorrect. angle1 != angle4");
	TestReport(angle2 != angle4, "The comparison operators of the Angle class are incorrect. angle2 != angle4");
	TestReport(angle3 != angle4, "The comparison operators of the Angle class are incorrect. angle3 != angle4");
	Angle angle5(45.0f);
	Angle angle6(M_PI / 4.0f, Angle::Unit::RADIAN); // angle5 is equal to angle6
	TestReport(angle1 > angle5, "The comparison operators of the Angle class are incorrect. angle1 > angle5");
	TestReport(angle1 > angle6, "The comparison operators of the Angle class are incorrect. angle1 > angle6");
	TestReport(angle1 >= angle5, "The comparison operators of the Angle class are incorrect. angle1 >= angle5");
	TestReport(angle1 >= angle6, "The comparison operators of the Angle class are incorrect. angle1 >= angle6");
	TestReport(angle5 < angle1, "The comparison operators of the Angle class are incorrect. angle5 < angle1");
	TestReport(angle6 < angle1, "The comparison operators of the Angle class are incorrect. angle6 < angle1");
	TestReport(angle5 <= angle1, "The comparison operators of the Angle class are incorrect. angle5 <= angle1");
	TestReport(angle6 <= angle1, "The comparison operators of the Angle class are incorrect. angle6 <= angle1");
	TestReport(angle5 == angle6, "The comparison operators of the Angle class are incorrect. angle5 == angle6");
	TestReport(angle5 >= angle6, "The comparison operator of the Angle class are incorrect. angle5 >= angle6");
	TestReport(angle5 <= angle6, "The comparison operator of the Angle class are incorrect. angle5 <= angle6");
	TestReport(angle6 >= angle5, "The comparison operator of the Angle class are incorrect. angle6 >= angle5");
	TestReport(angle6 <= angle5, "The comparison operator of the Angle class are incorrect. angle6 <= angle5");
	/* ==================== ANGLE TEST #1- comparing angles- end ==================== */
}

void VectorTest()
{
	if (!vectorTestEnabled)
	{
		return;
	}
	/* ==================== VECTOR2D TEST #1- comparing vectors- begin ==================== */
	Vector2D vec1(90.0, 90.0);
	Vector2D vec2(45.0, 90.0);
	Vector2D vec3(90.0, 45.0);
	Vector2D vec4(90.0, 90.0);
	TestReport(vec1 == vec2, "The comparison operators of the Angle class are incorrect. angle1 == angle2");
	TestReport(vec1 == vec3, "The comparison operators of the Angle class are incorrect. angle1 == angle3");
	TestReport(vec2 == vec3, "The comparison operators of the Angle class are incorrect. angle2 == angle3");
	TestReport(vec1 != vec4, "The comparison operators of the Angle class are incorrect. angle1 != angle4");
	TestReport(vec2 != vec4, "The comparison operators of the Angle class are incorrect. angle2 != angle4");
	TestReport(vec3 != vec4, "The comparison operators of the Angle class are incorrect. angle3 != angle4");
	//Angle angle5(45.0f);
	//Angle angle6(M_PI / 4.0f, false); // angle5 is equal to angle6
	//TestReport(angle1 > angle5, "The comparison operators of the Angle class are incorrect. angle1 > angle5");
	//TestReport(angle1 > angle6, "The comparison operators of the Angle class are incorrect. angle1 > angle6");
	//TestReport(angle1 >= angle5, "The comparison operators of the Angle class are incorrect. angle1 >= angle5");
	//TestReport(angle1 >= angle6, "The comparison operators of the Angle class are incorrect. angle1 >= angle6");
	//TestReport(angle5 < angle1, "The comparison operators of the Angle class are incorrect. angle5 < angle1");
	//TestReport(angle6 < angle1, "The comparison operators of the Angle class are incorrect. angle6 < angle1");
	//TestReport(angle5 <= angle1, "The comparison operators of the Angle class are incorrect. angle5 <= angle1");
	//TestReport(angle6 <= angle1, "The comparison operators of the Angle class are incorrect. angle6 <= angle1");
	//TestReport(angle5 == angle6, "The comparison operators of the Angle class are incorrect. angle5 == angle6");
	//TestReport(angle5 >= angle6, "The comparison operator of the Angle class are incorrect. angle5 >= angle6");
	//TestReport(angle5 <= angle6, "The comparison operator of the Angle class are incorrect. angle5 <= angle6");
	//TestReport(angle6 >= angle5, "The comparison operator of the Angle class are incorrect. angle6 >= angle5");
	//TestReport(angle6 <= angle5, "The comparison operator of the Angle class are incorrect. angle6 <= angle5");
	/* ==================== VECTOR2D TEST #1- comparing vectors- end ==================== */
}

int main (int argc, char* argv[])
{
	ICommand::SetCommand(argc, argv);
	//if (Command.IsPresent("-help"))
	//{
	//	PrintHelp();
	//	system("pause");
	//	return 0;
	//}
	ILogger::GetLogger().Fill(ICommand::GetCommand().Get("-log", ""), Debug);

	Matrix4D identityMatrix1 = Matrix4D::Identity();
	Matrix4D identityMatrix2 = Matrix4D::Identity();

	AngleTest();
	VectorTest();

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
