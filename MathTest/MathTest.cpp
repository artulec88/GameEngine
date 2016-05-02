#include "Math\Angle.h"
#include "Math\FloatingPoint.h"
#include "Math\Matrix.h"
#include "Math\Quaternion.h"
#include "Math\Vector.h"
#include "Math\ISort.h"
#include "Math\KDTree.h"
#include "Math\RandomGeneratorFactory.h"

#include "Utility\ICommand.h"
#include "Utility\ILogger.h"
#include "Utility\Time.h"

#include "StatsTest.h"
#include "AngleTestGroup.h"
#include "VectorTestGroup.h"
#include "QuaternionTestGroup.h"
#include "MatrixTestGroup.h"

#include <ctime>
#include <string>
//#include <iostream>
//#include <fstream>

using namespace Math;
using namespace Utility;
using namespace std;

unsigned int innerLoops = 10000;
unsigned int outerLoops = 10000;
double elapsedTime;

unsigned int testNumber = 0;
bool angleTestEnabled = false;
bool vectorTestEnabled = false;
bool matrixTestEnabled = true;
bool quaternionTestEnabled = true;
bool sortingTestEnabled = false;
bool kdTreeTestEnabled = false;
bool statsTestEnabled = false;
bool otherTestEnabled = true;

const Math::Random::RandomGenerator& g_randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE);

void ReportError(const std::string& reportStr)
{
	ERROR_LOG("Test #%d FAILED. %s", testNumber, reportStr.c_str());
}

void TestReport(bool statusCode /* false if error */, const std::string& reportErrorStr)
{
	testNumber++;
	if (statusCode == false)
	{
		ReportError(reportErrorStr);
	}
	INFO_LOG("Test #%d passed", testNumber);
}

void TimeReport(const std::string& reportStr, Timing::Timer& timer, Timing::TimeUnit timeUnit, const int NUMBER_OF_ITERATIONS = 1)
{
	CHECK_CONDITION_EXIT_ALWAYS(!timer.IsRunning(), Error, "Timer is still running");
	Timing::TimeSpan timeSpan = timer.GetTimeSpan(timeUnit);
	timeSpan /= NUMBER_OF_ITERATIONS;
	//timeSpan.AdjustUnitToValue();
	NOTICE_LOG("%s:\t%s", reportStr.c_str(), timeSpan.ToString().c_str());
}

void AngleTest()
{
	if (!angleTestEnabled)
	{
		return;
	}

	MathTest::AngleTestGroup angleTests;
	const Angle angle1(90.0);
	const Angle angle2(90.0, Unit::DEGREE);
	const Angle angle3(PI / 2.0f, Unit::RADIAN);
	const Angle angle4(PI / 2.0f, Unit::DEGREE);
	const Angle angle5(45.0f);
	const Angle angle6(PI / 4.0f, Unit::RADIAN);
	angleTests.AddTest(new MathTest::AngleTestCompare(angle1, angle2, true, false, false));
	angleTests.AddTest(new MathTest::AngleTestCompare(angle1, angle3, true, false, false));
	angleTests.AddTest(new MathTest::AngleTestCompare(angle2, angle3, true, false, false));
	angleTests.AddTest(new MathTest::AngleTestCompare(angle1, angle4, false, false, true));
	angleTests.AddTest(new MathTest::AngleTestCompare(angle4, angle1, false, true, false));
	angleTests.AddTest(new MathTest::AngleTestCompare(angle2, angle4, false, false, true));
	angleTests.AddTest(new MathTest::AngleTestCompare(angle4, angle2, false, true, false));
	angleTests.AddTest(new MathTest::AngleTestCompare(angle3, angle4, false, false, true));
	angleTests.AddTest(new MathTest::AngleTestCompare(angle4, angle3, false, true, false));
	angleTests.AddTest(new MathTest::AngleTestCompare(angle5, angle6, true, false, false));

	angleTests.StartTests();
}

void VectorTest()
{
	if (!vectorTestEnabled)
	{
		return;
	}
	MathTest::VectorTestGroup vectorTests;
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector2D>(Math::Vector2D(0.0f, 0.0f), Math::Vector2D(0.0f, 0.0f), true));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector2D>(Math::Vector2D(0.0f, 0.0f), Math::Vector2D(0.0f, 1.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector2D>(Math::Vector2D(0.0f, 0.0f), Math::Vector2D(1.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector2D>(Math::Vector2D(0.0f, 0.0f), Math::Vector2D(1.0f, 1.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 0.0f, 0.0f), true));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), Math::Vector3D(1.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), Math::Vector3D(1.0f, 1.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), true));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Math::Vector4D(0.0f, 1.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Math::Vector4D(1.0f, 0.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Math::Vector4D(1.0f, 1.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector2D>(Math::Vector2D(0.0f, 0.0f), 0.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector2D>(Math::Vector2D(1.0f, 0.0f), 1.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector2D>(Math::Vector2D(12.0f, -4.0f), 4.0f * sqrtf(10.0f)));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), 0.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector3D>(Math::Vector3D(1.0f, 0.0f, 0.0f), 1.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector3D>(Math::Vector3D(3.0f, 4.0f, 12.0f), 13.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), 0.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector4D>(Math::Vector4D(1.0f, 0.0f, 0.0f, 0.0f), 1.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector4D>(Math::Vector4D(1.0f, 2.0f, 5.0f, -3.0f), sqrtf(39.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector2D>(Math::Vector2D(0.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), Math::Vector2D(1.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector2D>(Math::Vector2D(0.0f, 0.0f), Math::Vector2D(1.0f, 0.0f), Math::Vector2D(1.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector2D>(Math::Vector2D(1.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), Math::Vector2D(2.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector3D>(Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(1.0f, 1.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), Math::Vector3D(1.0f, 0.0f, -1.0f), Math::Vector3D(1.0f, 0.0f, -1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector3D>(Math::Vector3D(1.0f, 1.0f, -3.0f), Math::Vector3D(1.0f, 0.0f, -2.0f), Math::Vector3D(2.0f, 1.0f, -5.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector4D>(Math::Vector4D(0.0f, 1.0f, 2.0f, 3.0f), Math::Vector4D(1.0f, 0.0f, -1.0f, 0.0f), Math::Vector4D(1.0f, 1.0f, 1.0f, 3.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, -3.0f, 4.0f), Math::Vector4D(1.0f, 0.0f, 2.0f, -3.0f), Math::Vector4D(1.0f, 0.0f, -1.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector4D>(Math::Vector4D(1.0f, 1.0f, 5.0f, 6.0f), Math::Vector4D(1.0f, 0.0f, 5.0f, 5.0f), Math::Vector4D(2.0f, 1.0f, 10.0f, 11.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector2D>(Math::Vector2D(0.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), Math::Vector2D(-1.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector2D>(Math::Vector2D(0.0f, 0.0f), Math::Vector2D(1.0f, 0.0f), Math::Vector2D(-1.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector2D>(Math::Vector2D(1.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), Math::Vector2D(0.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector3D>(Math::Vector3D(0.0f, 1.0f, 2.0f), Math::Vector3D(1.0f, 0.0f, 2.0f), Math::Vector3D(-1.0f, 1.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, -2.0f), Math::Vector3D(1.0f, 0.0f, -1.0f), Math::Vector3D(-1.0f, 0.0f, -1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector3D>(Math::Vector3D(1.0f, 1.0f, 3.0f), Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 3.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector4D>(Math::Vector4D(0.0f, 1.0f, 2.0f, -2.0f), Math::Vector4D(1.0f, 0.0f, -2.0f, -3.0f), Math::Vector4D(-1.0f, 1.0f, 4.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, -4.0f, -5.0f), Math::Vector4D(1.0f, 0.0f, -5.0f, 4.0f), Math::Vector4D(-1.0f, 0.0f, 1.0f, -9.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector4D>(Math::Vector4D(1.0f, 1.0f, -8.0f, 5.0f), Math::Vector4D(1.0f, 0.0f, 6.0f, -7.0f), Math::Vector4D(0.0f, 1.0f, -14.0f, 12.0f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector2D>(Math::Vector2D(0.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), 0.2f, Math::Vector2D(0.0f, 0.0f), Math::Vector2D(0.0f, 0.2f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector2D>(Math::Vector2D(0.0f, 0.0f), Math::Vector2D(1.0f, 0.0f), 0.5f, Math::Vector2D(0.0f, 0.0f), Math::Vector2D(0.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector2D>(Math::Vector2D(1.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), 0.75f, Math::Vector2D(1.0f, 0.0f), Math::Vector2D(0.75f, 0.75f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector3D>(Math::Vector3D(0.0f, 1.0f, 2.0f), Math::Vector3D(1.0f, 0.0f, 4.0f), 0.2f, Math::Vector3D(0.0f, 0.0f, 8.0f), Math::Vector3D(0.0f, 0.2f, 0.4f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, -1.0f), Math::Vector3D(1.0f, 0.0f, -5.0f), 0.5f, Math::Vector3D(0.0f, 0.0f, 5.0f), Math::Vector3D(0.0f, 0.0f, -0.5f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector3D>(Math::Vector3D(1.0f, 1.0f, 5.0f), Math::Vector3D(1.0f, 0.0f, 0.0f), 0.75f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.75f, 0.75f, 3.75f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector4D>(Math::Vector4D(0.0f, 1.0f, 2.0f, 4.0f), Math::Vector4D(1.0f, 0.0f, 4.0f, -6.0f), 0.2f, Math::Vector4D(0.0f, 0.0f, 8.0f, -24.0f), Math::Vector4D(0.0f, 0.2f, 0.4f, 0.8f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, -5.0f, -1.0f), Math::Vector4D(1.0f, 0.0f, 0.0f, 1.0f), 0.5f, Math::Vector4D(0.0f, 0.0f, 0.0f, -1.0f), Math::Vector4D(0.0f, 0.0f, -2.5f, -0.5f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector4D>(Math::Vector4D(1.0f, 1.0f, 0.0f, 2.0f), Math::Vector4D(1.0f, 0.0f, -2.0f, 5.0f), 0.75f, Math::Vector4D(1.0f, 0.0f, 0.0f, 10.0f), Math::Vector4D(0.75f, 0.75f, 0.0f, 1.5f)));
	vectorTests.AddTest(new MathTest::VectorTestDivideOperator<Math::Vector2D>(Math::Vector2D(8.0f, 1.0f), Math::Vector2D(1.0f, 2.0f), 0.2f, Math::Vector2D(8.0f, 0.5f), Math::Vector2D(40.0f, 5.0f)));
	vectorTests.AddTest(new MathTest::VectorTestDivideOperator<Math::Vector2D>(Math::Vector2D(0.0f, 0.0f), Math::Vector2D(1.0f, 5.0f), 0.5f, Math::Vector2D(0.0f, 0.0f), Math::Vector2D(0.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestDivideOperator<Math::Vector2D>(Math::Vector2D(1.0f, 10.0f), Math::Vector2D(1.0f, -2.0f), 0.75f, Math::Vector2D(1.0f, -5.0f), Math::Vector2D(4.0f / 3.0f, 40.0f / 3.0f)));
	vectorTests.AddTest(new MathTest::VectorTestDivideOperator<Math::Vector3D>(Math::Vector3D(0.0f, 1.0f, 2.0f), Math::Vector3D(1.0f, 2.0f, 4.0f), 0.2f, Math::Vector3D(0.0f, 0.5f, 0.5f), Math::Vector3D(0.0f, 5.0f, 10.0f)));
	vectorTests.AddTest(new MathTest::VectorTestDivideOperator<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, -1.0f), Math::Vector3D(1.0f, 10.0f, -5.0f), 0.5f, Math::Vector3D(0.0f, 0.0f, 0.2f), Math::Vector3D(0.0f, 0.0f, -2.0f)));
	vectorTests.AddTest(new MathTest::VectorTestDivideOperator<Math::Vector3D>(Math::Vector3D(1.0f, 1.0f, 5.0f), Math::Vector3D(1.0f, -1.0f, 2.0f), 2.0f, Math::Vector3D(1.0f, -1.0f, 2.5f), Math::Vector3D(0.5f, 0.5f, 2.5f)));
	vectorTests.AddTest(new MathTest::VectorTestDivideOperator<Math::Vector4D>(Math::Vector4D(0.0f, 1.0f, 2.0f, 4.0f), Math::Vector4D(1.0f, -2.0f, 4.0f, -16.0f), -2.0f, Math::Vector4D(0.0f, -0.5f, 0.5f, -0.25f), Math::Vector4D(0.0f, -0.5f, -1.0f, -2.0f)));
	vectorTests.AddTest(new MathTest::VectorTestDivideOperator<Math::Vector4D>(Math::Vector4D(70.0f, 100.0f, -5.0f, -1.0f), Math::Vector4D(1.0f, -5.0f, 5.0f, 1.0f), 0.5f, Math::Vector4D(70.0f, -20.0f, -1.0f, -1.0f), Math::Vector4D(140.0f, 200.0f, -10.0f, -2.0f)));
	vectorTests.AddTest(new MathTest::VectorTestDivideOperator<Math::Vector4D>(Math::Vector4D(21.0f, 144.0f, -3.0f, 2.0f), Math::Vector4D(3.0f, 12.0f, -2.0f, 5.0f), 3.0f, Math::Vector4D(7.0f, 12.0f, 1.5f, 0.4f), Math::Vector4D(7.0f, 48.0f, -1.0f, 2.0f / 3.0f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector2D>(Math::Vector2D(1.0f, 0.0f), Math::Vector2D(1.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector2D>(Math::Vector2D(2.0f, 0.0f), Math::Vector2D(1.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector2D>(Math::Vector2D(1.0f, 1.0f), Math::Vector2D(sqrtf(2.0f) / 2.0f, sqrtf(2.0f) / 2.0f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector2D>(Math::Vector2D(0.0f, 1.0f), Math::Vector2D(0.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector2D>(Math::Vector2D(5.0f, 5.0f), Math::Vector2D(sqrtf(2.0f) / 2.0f, sqrtf(2.0f) / 2.0f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector2D>(Math::Vector2D(3.0f, 4.0f), Math::Vector2D(0.6f, 0.8f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector3D>(Math::Vector3D(1.0f, 0.0f, 2.0f), Math::Vector3D(sqrtf(5.0f) / 5.0f, 0.0f, sqrtf(5.0f) / 2.5f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector3D>(Math::Vector3D(2.0f, 0.0f, 3.0f), Math::Vector3D(sqrtf(13.0f) / 6.5f, 0.0f, 3.0f * sqrtf(13.0f) / 13.0f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector3D>(Math::Vector3D(1.0f, -1.0f, 5.0f), Math::Vector3D(sqrtf(27.0f) / 27.0f, -sqrtf(27.0f) / 27.0f, 5.0f * sqrtf(27.0f) / 27.0f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector3D>(Math::Vector3D(0.0f, 1.0f, 2.0f), Math::Vector3D(0.0f, 1.0f / sqrtf(5.0f), 2.0f / sqrtf(5.0f))));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector3D>(Math::Vector3D(5.0f, -5.0f, 3.0f), Math::Vector3D(5.0f / sqrtf(59.0f), -5.0f / sqrtf(59.0f), 3.0f / sqrtf(59.0f))));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector3D>(Math::Vector3D(3.0f, 4.0f, 0.0f), Math::Vector3D(0.6f, 0.8f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector4D>(Math::Vector4D(1.0f, 0.0f, -3.0f, 0.0f), Math::Vector4D(1.0f / sqrtf(10.0f), 0.0f, -3.0f / sqrtf(10.0f), 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector4D>(Math::Vector4D(2.0f, 0.0f, 1.0f, 5.0f), Math::Vector4D(2.0f / sqrtf(30.0f), 0.0f, 1.0f / sqrtf(30.0f), 5.0f / sqrtf(30.0f))));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector4D>(Math::Vector4D(1.0f, 1.0f, -2.0f, 1.0f), Math::Vector4D(1.0f / sqrtf(7.0f), 1.0f / sqrtf(7.0f), -2.0f / sqrtf(7.0f), 1.0f / sqrtf(7.0f))));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector4D>(Math::Vector4D(0.0f, 1.0f, 3.0f, -2.0f), Math::Vector4D(0.0f, 1.0f / sqrtf(14.0f), 3.0f / sqrtf(14.0f), -2.0f / sqrtf(14.0f))));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector4D>(Math::Vector4D(5.0f, 5.0f, 0.0f, -1.0f), Math::Vector4D(5.0f / sqrtf(51.0f), 5.0f / sqrtf(51.0f), 0.0f, -1.0f / sqrtf(51.0f))));
	vectorTests.AddTest(new MathTest::VectorTestNormalize<Math::Vector4D>(Math::Vector4D(3.0f, 4.0f, 1.0f, -10.0f), Math::Vector4D(3.0f / sqrtf(126.0f), 4.0f / sqrtf(126.0f), 1.0f / sqrtf(126.0f), -10.0f / sqrtf(126.0f))));
	vectorTests.AddTest(new MathTest::Vector2DTestCross(Math::Vector2D(3.0f, 4.0f), Math::Vector2D(2.0f, 1.0f), -5.0f));
	vectorTests.AddTest(new MathTest::Vector2DTestCross(Math::Vector2D(5.0f, 4.0f), Math::Vector2D(2.0f, -1.0f), -13.0f));
	vectorTests.AddTest(new MathTest::Vector2DTestCross(Math::Vector2D(-3.0f, 2.0f), Math::Vector2D(-3.0f, 3.0f), -3.0f));
	vectorTests.AddTest(new MathTest::Vector2DTestCross(Math::Vector2D(0.0f, 4.0f), Math::Vector2D(4.0f, 1.0f), -16.0f));
	vectorTests.AddTest(new MathTest::Vector3DTestCross(Math::Vector3D(3.0f, 4.0f, 1.0f), Math::Vector3D(2.0f, 1.0f, 5.0f), Math::Vector3D(19.0f, -13.0f, -5.0f)));
	vectorTests.AddTest(new MathTest::Vector3DTestCross(Math::Vector3D(5.0f, 4.0f, -2.0f), Math::Vector3D(2.0f, -1.0f, 3.0f), Math::Vector3D(10.0f, -19.0f, -13.0f)));
	vectorTests.AddTest(new MathTest::Vector3DTestCross(Math::Vector3D(-3.0f, 2.0f, 0.0f), Math::Vector3D(-3.0f, 3.0f, 1.0f), Math::Vector3D(2.0f, 3.0f, -3.0f)));
	vectorTests.AddTest(new MathTest::Vector3DTestCross(Math::Vector3D(0.0f, 4.0f, -2.0f), Math::Vector3D(4.0f, 1.0f, -1.0f), Math::Vector3D(-2.0f, -8.0f, -16.0f)));

	//vectorTests.AddTest(new MathTest::Vector2DTestDot(Math::Vector2D(3.0f, 4.0f), Math::Vector2D(2.0f, 1.0f), 11.0f));
	//vectorTests.AddTest(new MathTest::Vector2DTestDot(Math::Vector2D(5.0f, 4.0f), Math::Vector2D(2.0f, -1.0f), 6.0f));
	//vectorTests.AddTest(new MathTest::Vector2DTestDot(Math::Vector2D(-3.0f, 2.0f), Math::Vector2D(-3.0f, 3.0f), 15.0f));
	//vectorTests.AddTest(new MathTest::Vector2DTestDot(Math::Vector2D(0.0f, 4.0f), Math::Vector2D(4.0f, 1.0f), 4.0f));
	//vectorTests.AddTest(new MathTest::Vector2DTestRotate(Math::Vector2D(0.0f, 4.0f), Math::Angle(45.0f), Math::Vector2D(-2.0f * sqrtf(2.0f), 2.0f * sqrtf(2.0f))));
	//vectorTests.AddTest(new MathTest::Vector2DTestRotate(Math::Vector2D(5.0f, 4.0f), Math::Angle(90.0f), Math::Vector2D(-4.0f, 5.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestRotate(Math::Vector2D(5.0f, 4.0f), Math::Angle(360.0f), Math::Vector2D(5.0f, 4.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestRotate(Math::Vector2D(-3.0f, 2.0f), Math::Angle(-45.0f), Math::Vector2D(-sqrtf(2.0f) / 2.0f, 2.5f * sqrtf(2.0f))));
	//vectorTests.AddTest(new MathTest::Vector2DTestRotate(Math::Vector2D(3.0f, 4.0f), Math::Angle(60.0f), Math::Vector2D((3.0f - 4.0f * sqrtf(3.0f)) / 2.0f, (4.0f + 3.0f * sqrtf(3.0f)) / 2.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(Math::Vector2D(0.0f, 4.0f), Math::Vector2D(1.0f, 2.0f), 0.2f, Math::Vector2D(0.2f, 3.6f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(Math::Vector2D(5.0f, 4.0f), Math::Vector2D(0.5f, 2.0f), 0.5f, Math::Vector2D(2.75f, 3.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(Math::Vector2D(5.0f, 4.0f), Math::Vector2D(0.5f, 2.0f), 1.0f, Math::Vector2D(0.5f, 2.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(Math::Vector2D(-3.0f, 2.0f), Math::Vector2D(0.5f, 2.0f), 0.25f, Math::Vector2D(-2.125f, 2.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(Math::Vector2D(3.0f, 4.0f), Math::Vector2D(0.5f, 2.0f), 0.75f, Math::Vector2D(1.125f, 2.5f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(Math::Vector2D(3.0f, 4.0f), Math::Vector2D(0.5f, 2.0f), 0.0f, Math::Vector2D(3.0f, 4.0f)));
	vectorTests.StartTests();
}

Quaternion RandomQuaternion()
{
	const Real MIN_COMPONENT = -5.0f;
	const Real MAX_COMPONENT = 5.0f;
	const Real x = g_randomGenerator.NextFloat(MIN_COMPONENT, MAX_COMPONENT);
	const Real y = g_randomGenerator.NextFloat(MIN_COMPONENT, MAX_COMPONENT);
	const Real z = g_randomGenerator.NextFloat(MIN_COMPONENT, MAX_COMPONENT);
	const Real w = g_randomGenerator.NextFloat(MIN_COMPONENT, MAX_COMPONENT);
	return Quaternion(x, y, z, w);
}

void QuaternionTest()
{
	if (!quaternionTestEnabled)
	{
		return;
	}
	MathTest::QuaternionTestGroup quaternionTests;
	quaternionTests.AddTest(new MathTest::QuaternionTestCompare(Quaternion(0.0f, 0.0f, 0.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.0f, 0.0f), false));
	quaternionTests.AddTest(new MathTest::QuaternionTestCompare(Quaternion(0.0f, 0.0f, 0.0f, 0.0f), Quaternion(1.0f, 1.0f, 1.0f, 1.0f), false));
	quaternionTests.AddTest(new MathTest::QuaternionTestCompare(Quaternion(0.0f, 0.0f, 0.0f, 0.0f), Quaternion(1.0f, 1.0f, 1.0f, 1.0f), false));
	quaternionTests.AddTest(new MathTest::QuaternionTestCompare(Quaternion(0.0f, 0.0f, 0.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.0f, 1.0f), true));
	quaternionTests.AddTest(new MathTest::QuaternionTestLength(Quaternion(1.0f, 2.0f, 3.0f, 4.0f), sqrtf(30.0f)));
	quaternionTests.AddTest(new MathTest::QuaternionTestLength(Quaternion(0.0f, 0.0f, 0.0f, 4.0f), 4.0f));
	quaternionTests.AddTest(new MathTest::QuaternionTestLength(Quaternion(3.0f, 4.0f, 0.0f, 0.0f), 5.0f));
	quaternionTests.AddTest(new MathTest::QuaternionTestConjugate(Quaternion(3.0f, 4.0f, 2.0f, 1.0f), Quaternion(-3.0, -4.0f, -2.0f, 1.0f)));
	quaternionTests.AddTest(new MathTest::QuaternionTestConjugate(Quaternion(1.0f, -5.0f, 3.0f, -7.0f), Quaternion(-1.0, 5.0f, -3.0f, -7.0f)));
	quaternionTests.AddTest(new MathTest::QuaternionTestAddOperator(Quaternion(3.0f, 0.0f, 1.0f, -2.0f), Quaternion(-7.0, 15.0f, -2.0f, -4.0f), Quaternion(-4.0f, 15.0f, -1.0f, -6.0f)));
	quaternionTests.AddTest(new MathTest::QuaternionTestAddOperator(Quaternion(2.0f, -8.0f, 13.0f, -9.0f), Quaternion(-2.0, -5.0f, -6.0f, -2.0f), Quaternion(0.0f, -13.0f, 7.0f, -11.0f)));
	quaternionTests.AddTest(new MathTest::QuaternionTestAddOperator(Quaternion(1.0f, 8.0f, 5.0f, -7.0f), Quaternion(-1.0, 5.0f, 5.0f, -1.0f), Quaternion(0.0f, 13.0f, 10.0f, -8.0f)));

	/* ==================== QUATERNION TEST #2- calculating rotation matrix- begin ==================== */
	INFO_LOG("Profiling quaternion to rotation matrix conversion functions begin");
	Quaternion q = RandomQuaternion();
	const int QUATERNION_TO_ROTATION_MATRIX_ITERATIONS = 5000000;
	Timing::Timer timer;
	timer.Start();
	for (int i = 0; i < QUATERNION_TO_ROTATION_MATRIX_ITERATIONS; ++i)
	{
		Quaternion q = RandomQuaternion();
		q.ToRotationMatrix();
	}
	timer.Stop();
	TimeReport("Average time for quaternion to rotation matrix conversion", timer, Timing::MICROSECOND, QUATERNION_TO_ROTATION_MATRIX_ITERATIONS);
	DEBUG_LOG("Profiling quaternion to rotation matrix conversion functions end.");
	/* ==================== QUATERNION TEST #2- calculating rotation matrix- end ==================== */

	/* ==================== QUATERNION TEST #3- calculating linear interpolation- begin ==================== */
	INFO_LOG("Profiling quaternion linear interpolation functions begin");
	const int QUATERNION_LINEAR_INTERPOLATION_ITERATIONS = 1000000;
	timer.Reset();
	for (int i = 0; i < QUATERNION_LINEAR_INTERPOLATION_ITERATIONS; ++i)
	{
		Quaternion q1 = RandomQuaternion();
		Quaternion q2 = RandomQuaternion();
		q1.Nlerp1(q2, g_randomGenerator.NextFloat(0.0f, 1.0f), false);
	}
	timer.Stop();
	TimeReport("Average time for quaternion linear interpolation function #1", timer, Timing::MICROSECOND, QUATERNION_LINEAR_INTERPOLATION_ITERATIONS);

	timer.Reset();
	for (int i = 0; i < QUATERNION_LINEAR_INTERPOLATION_ITERATIONS; ++i)
	{
		Quaternion q1 = RandomQuaternion();
		Quaternion q2 = RandomQuaternion();
		q1.Nlerp2(q2, g_randomGenerator.NextFloat(0.0f, 1.0f), false);
	}
	timer.Stop();
	TimeReport("Average time for quaternion linear interpolation function #2", timer, Timing::MICROSECOND, QUATERNION_LINEAR_INTERPOLATION_ITERATIONS);
	DEBUG_LOG("Profiling quaternion linear interpolation functions end.");
	/* ==================== QUATERNION TEST #3- calculating linear interpolation- end ==================== */
}

Angle RandomAngle()
{
	const Real MIN_ANGLE = REAL_ZERO;
	const Real MAX_ANGLE = 360.0f;
	Angle angle(g_randomGenerator.NextFloat(MIN_ANGLE, MAX_ANGLE), Unit::DEGREE);
	return angle;
}

Matrix4D RandomMatrix(Real min, Real max)
{
	if (min < max)
	{
		Real temp = min;
		min = max;
		max = temp;
	}
	Matrix4D matrix;
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			Real value = g_randomGenerator.NextFloat(min, max);
			matrix.SetElement(i, j, value);
		}
	}
	return matrix;
}

void MatrixTest()
{
	if (!matrixTestEnabled)
	{
		return;
	}
	MathTest::MatrixTestGroup matrixTests;

	Matrix4D matrix1;
	Matrix4D matrix2(REAL_ONE); // the scale matrix with scaleFactor = REAL_ONE is indeed an identity matrix
	Matrix4D matrix3(2.0f);
	Matrix4D matrix4(REAL_ZERO, REAL_ZERO, REAL_ZERO); // translation matrix with translation vector equal to zero is indeed an identity matrix
	Matrix4D matrix5(3.0f, -5.0f, 2.0f); // translation matrix
	Matrix4D matrix6(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f); // ortographic projection
	Matrix4D matrix7(Angle(30.0f), Angle(60.0f)); // rotation matrix
	Matrix4D matrix8(Angle(30.0f), Angle(60.0f), Angle(0.0f));
	Matrix4D matrix9(matrix6);
	Matrix4D matrix10(Vector3D(5.0f, 10.0f, 15.0f)); // translation matrix
	Matrix4D matrix11(5.0f, 10.0f, 15.0f); // translation matrix
	Matrix4D matrix12(Angle(75.0f), 1.333f, 0.1f, 1000.0f); // perspective projection
	Matrix4D matrix13(Vector2D(5.0f, -2.0f), Vector2D(0.5f, 1.2f));
	Matrix4D matrix14(Vector3D(1.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f)); // rotation matrix
	Matrix4D matrix15(Vector3D(1.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 2.0f, 5.0f)); // rotation matrix
	Matrix4D matrix16(1, 2, 3, 0, 5, 6, 7, 0, 9, 10, 11, 0, 13, 14, 15, 1);
	Matrix4D matrix17(-1, 2, -3, 4, -5, 6, -7, 8, -9, 10, -11, 12, -13, 14, -15, 16);
	Matrix4D matrix18(-38, 44, -50, 56, -98, 116, -134, 152, -158, 188, -218, 248, -231, 274, -317, 360);

	matrixTests.AddTest(new MathTest::MatrixTestCompare(matrix1, Matrix4D::IDENTITY_MATRIX, true));
	matrixTests.AddTest(new MathTest::MatrixTestCompare(Matrix4D::IDENTITY_MATRIX, matrix1, true));
	matrixTests.AddTest(new MathTest::MatrixTestCompare(matrix1, matrix2, true));
	matrixTests.AddTest(new MathTest::MatrixTestCompare(matrix2, matrix1, true));
	matrixTests.AddTest(new MathTest::MatrixTestCompare(matrix2, Matrix4D::IDENTITY_MATRIX, true));
	matrixTests.AddTest(new MathTest::MatrixTestCompare(Matrix4D::IDENTITY_MATRIX, matrix2, true));
	matrixTests.AddTest(new MathTest::MatrixTestCompare(matrix4, Matrix4D::IDENTITY_MATRIX, true));
	matrixTests.AddTest(new MathTest::MatrixTestCompare(matrix7, matrix8, true));
	matrixTests.AddTest(new MathTest::MatrixTestCompare(matrix7, Matrix4D::IDENTITY_MATRIX, false));
	matrixTests.AddTest(new MathTest::MatrixTestCompare(matrix9, matrix6, true));
	matrixTests.AddTest(new MathTest::MatrixTestCompare(matrix9, matrix9, true)); // comparing the matrix with itself
	matrixTests.AddTest(new MathTest::MatrixTestCompare(matrix10, matrix11, true));
	matrixTests.AddTest(new MathTest::MatrixTestCompare(matrix11, matrix10, true));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByMatrixOperator(Matrix4D::IDENTITY_MATRIX, Matrix4D::IDENTITY_MATRIX, Matrix4D::IDENTITY_MATRIX));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByMatrixOperator(Matrix4D::IDENTITY_MATRIX, matrix3, matrix3));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByMatrixOperator(Matrix4D::IDENTITY_MATRIX, matrix5, matrix5));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByMatrixOperator(matrix12, Matrix4D::IDENTITY_MATRIX, matrix12));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByMatrixOperator(matrix3, Matrix4D::IDENTITY_MATRIX, matrix3));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByMatrixOperator(matrix16, matrix17, matrix18));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix1, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix2, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix3, Vector3D(2, 3, 5), Vector3D(4, 6, 10)));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix4, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix5, Vector3D(2, 3, 5), Vector3D(5, -2, 7)));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix6, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix7, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix8, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix9, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix10, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix11, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix12, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix13, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix14, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix15, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix16, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix17, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	//matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix18, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));

	matrixTests.StartTests();

	Matrix4D identityMatrix1(Matrix4D::IDENTITY_MATRIX);
	Matrix4D identityMatrix2(Matrix4D::IDENTITY_MATRIX);

	/* ==================== MATRIX TEST #1 begin ==================== */
	const int NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS = 1000000;
	Matrix4D testMatrix1(Matrix4D::IDENTITY_MATRIX);
	TestReport(testMatrix1.IsIdentity(), "The function Matrix::IsIdentity() failed.");

	Timing::Timer timer;
	timer.Start();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS; ++i)
	{
		Matrix4D testMatrix2(Matrix4D::IDENTITY_MATRIX);
	}
	timer.Stop();
	TimeReport("Average time for identity matrix creation:\t", timer, Timing::MICROSECOND, NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS);
	/* ==================== MATRIX TEST #1 end ==================== */

	/* ==================== MATRIX TEST #2 begin ==================== */
	const int NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS = 2000000;
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS; ++i)
	{
		Matrix4D result = identityMatrix1 * identityMatrix2; // FIXME: Check matrix multiplication
		CHECK_CONDITION(result == identityMatrix1 * identityMatrix2, Utility::Error, "Identity matrix multiplication result is incorrect.");
	}
	timer.Stop();
	TimeReport("Average time for identity matrices multiplication:\t", timer, Timing::MICROSECOND, NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS);
	/* ==================== MATRIX TEST #2 end ==================== */

	/* ==================== MATRIX TEST #3 begin ==================== */
	const int NUMBER_OF_RANDOM_MATRIX_MULTIPLICATION_ITERATIONS = 2000000;
	Matrix4D mat1 = RandomMatrix(REAL_ZERO, REAL_ONE);
	Matrix4D mat2 = RandomMatrix(REAL_ZERO, REAL_ONE);
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_RANDOM_MATRIX_MULTIPLICATION_ITERATIONS; ++i)
	{
		Matrix4D result = mat1 * mat2; // FIXME: Check matrix multiplication
		//CHECK_CONDITION(result == mat1 * mat2, Utility::Error, "Random matrix multiplication result is incorrect.");
	}
	timer.Stop();
	TimeReport("Average time for random matrices multiplication:\t", timer, Timing::MICROSECOND, NUMBER_OF_RANDOM_MATRIX_MULTIPLICATION_ITERATIONS);
	/* ==================== MATRIX TEST #3 end ==================== */

	/* ==================== MATRIX TEST #4 begin ==================== */
	const int NUMBER_OF_ROTATION_EULER_ITERATIONS = 100000;
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_ROTATION_EULER_ITERATIONS; ++i)
	{
		Matrix4D result(RandomAngle(), RandomAngle(), RandomAngle());
	}
	timer.Stop();
	TimeReport("Average time for rotation Euler matrix calculation:\t", timer, Timing::MICROSECOND, NUMBER_OF_ROTATION_EULER_ITERATIONS);
	/* ==================== MATRIX TEST #4 end ==================== */
}

void SortTest()
{
	if (!sortingTestEnabled)
	{
		return;
	}

	ILogger::GetLogger().AddFile("sortingOutput.txt");
	//ofstream sortingOutputFile;
	//sortingOutputFile.open("sortingOutput.txt");

	NOTICE_LOG("Sorting test started");
	const int NUMBER_OF_VECTORS = 200000;
	const Real LOWER_BOUND_X = -20.0f;
	const Real HIGHER_BOUND_X = 20.0f;
	const Real LOWER_BOUND_Y = -20.0f;
	const Real HIGHER_BOUND_Y = 20.0f;	
	Vector2D* initialVectors = new Vector2D[NUMBER_OF_VECTORS];
	Vector2D* vectors = new Vector2D[NUMBER_OF_VECTORS];
	//initialVectors[0].SetX(REAL_ONE); initialVectors[5].SetY(REAL_ONE); // sumOfComponents = 2	sumOfAbsComponents = 2	sumOfSquaredComponents = 2
	//initialVectors[1].SetX(REAL_ONE); initialVectors[6].SetY(2.0f); // sumOfComponents = 3	sumOfAbsComponents = 3	sumOfSquaredComponents = 5
	//initialVectors[2].SetX(REAL_ONE); initialVectors[7].SetY(3.0f); // sumOfComponents = 4	sumOfAbsComponents = 4	sumOfSquaredComponents = 10
	//initialVectors[3].SetX(REAL_ONE); initialVectors[8].SetY(4.0f); // sumOfComponents = 5	sumOfAbsComponents = 5	sumOfSquaredComponents = 17
	//initialVectors[4].SetX(REAL_ONE); initialVectors[9].SetY(5.0f); // sumOfComponents = 6	sumOfAbsComponents = 6	sumOfSquaredComponents = 26
	//initialVectors[5].SetX(REAL_ZERO); initialVectors[0].SetY(REAL_ONE); // sumOfComponents = 1	sumOfAbsComponents = 1	sumOfSquaredComponents = 1
	//initialVectors[6].SetX(REAL_ZERO); initialVectors[1].SetY(2.0f); // sumOfComponents = 2	sumOfAbsComponents = 2	sumOfSquaredComponents = 4
	//initialVectors[7].SetX(REAL_ZERO); initialVectors[2].SetY(3.0f); // sumOfComponents = 3	sumOfAbsComponents = 3	sumOfSquaredComponents = 9
	//initialVectors[8].SetX(REAL_ZERO); initialVectors[3].SetY(4.0f); // sumOfComponents = 4	sumOfAbsComponents = 4	sumOfSquaredComponents = 16
	//initialVectors[9].SetX(REAL_ZERO); initialVectors[4].SetY(5.0f); // sumOfComponents = 5	sumOfAbsComponents = 5	sumOfSquaredComponents = 25
	//initialVectors[10].SetX(-1.0f); initialVectors[4].SetY(2.0f); // sumOfComponents = 1	sumOfAbsComponents = 3	sumOfSquaredComponents = 5
	//initialVectors[11].SetX(-2.0f); initialVectors[4].SetY(5.0f); // sumOfComponents = 3	sumOfAbsComponents = 7	sumOfSquaredComponents = 29
	for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
	{
		initialVectors[i].SetX(g_randomGenerator.NextFloat(LOWER_BOUND_X, HIGHER_BOUND_X));
		initialVectors[i].SetY(g_randomGenerator.NextFloat(LOWER_BOUND_Y, HIGHER_BOUND_Y));
		DEBUG_LOG("initialVectors[%d] = %s", i, initialVectors[i].ToString().c_str());
	}

	const int NUMBER_OF_SORTING_METHODS = 1; /* the number of sorting methods in the Math library we want to check (10 means we want to check all of them) */
	const int chosenSortingMethodIndices[] = { 3 }; // its length must match the value of NUMBER_OF_SORTING_METHODS variable
	Sorting::SortingAlgorithm sortingMethods[] = { Sorting::BUBBLE_SORT, Sorting::INSERTION_SORT, Sorting::SELECTION_SORT,
		Sorting::MERGE_SORT, Sorting::HEAP_SORT, Sorting::QUICK_SORT, Sorting::SHELL_SORT, Sorting::COMB_SORT,
		Sorting::COUNTING_SORT, Sorting::RADIX_SORT, Sorting::BUCKET_SORT };
	std::string sortingMethodsStr[] = { "Bubble sort", "Insertion sort", "Selection sort", "Merge sort",
		"Heap sort", "Quick sort", "Shell sort", "Comb sort", "Counting sort", "Radix sort", "Bucket sort" };

	const int NUMBER_OF_TIME_TESTS_ITERATIONS = 1;
	/**
	 * TODO: Instead of measuring the time needed to perform NUMBER_OF_TIME_TESTS_ITERATION sorts it should be more efficient to count the number of successful sorts within given period of time.
	 * If this time limit is passed then sorting is stopped no matter if it's finished or not.
	 */
	for (int sortingMethodIndex = 0; sortingMethodIndex < NUMBER_OF_SORTING_METHODS; ++sortingMethodIndex)
	{
		Sorting::ISort* sorter = Sorting::ISort::GetSortingObject(sortingMethods[chosenSortingMethodIndices[sortingMethodIndex]]);
		/* ==================== SORTING TEST #1- sorting Vector2D objects by X component ascending begin ==================== */
		bool sortingTestCasePassed = true;
		Sorting::SortingParametersChain sortingParameters(Sorting::COMPONENT_X, Sorting::ASCENDING);
		sortingParameters.AddChainLink(new Sorting::SortingParametersChain(Sorting::COMPONENT_Y, Sorting::DESCENDING));
		Timing::Timer timer;
		timer.Start();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by X component", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			bool firstCriterionEqual = AlmostEqual(vectors[i + 1].GetX(), vectors[i].GetX());
			if ((!firstCriterionEqual && vectors[i + 1].GetX() < vectors[i].GetX()) ||
				(firstCriterionEqual && (vectors[i + 1].GetY() > vectors[i].GetY())))
			{
				ERROR_LOG("%s in ASCENDING order by X component failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), vectors[i + 1].GetX(), i, vectors[i].ToString().c_str(), vectors[i].GetX());
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (ASCENDING order by X component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by X component failed.");
		/* ==================== SORTING TEST #1- sorting Vector2D objects by X component ascending end ==================== */

		/* ==================== SORTING TEST #2- sorting Vector2D objects by X component descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::COMPONENT_X);
		sortingParameters.SetSortingDirection(Sorting::DESCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by X component", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].GetX() > vectors[i].GetX()) || ((AlmostEqual(vectors[i + 1].GetX(), vectors[i].GetX())) && (vectors[i + 1].GetY() > vectors[i].GetY())))
			{
				ERROR_LOG("%s in DESCENDING order by X component failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), vectors[i + 1].GetX(), i, vectors[i].ToString().c_str(), vectors[i].GetX());
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (DESCENDING order by X component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by X component failed.");
		/* ==================== SORTING TEST #2- sorting Vector2D objects by X component descending end ==================== */

		/* ==================== SORTING TEST #3- sorting Vector2D objects by Y component ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.ResetChainLink();
		sortingParameters.SetSortingKey(Sorting::COMPONENT_Y);
		sortingParameters.SetSortingDirection(Sorting::ASCENDING);
		sortingParameters.AddChainLink(new Sorting::SortingParametersChain(Sorting::COMPONENT_X, Sorting::DESCENDING));
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by Y component", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].GetY() < vectors[i].GetY()) || ((AlmostEqual(vectors[i + 1].GetY(), vectors[i].GetY())) && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in ASCENDING order by Y component failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), vectors[i + 1].GetY(), i, vectors[i].ToString().c_str(), vectors[i].GetY());
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (ASCENDING order by Y component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by Y component failed.");
		/* ==================== SORTING TEST #3- sorting Vector2D objects by Y component ascending end ==================== */

		/* ==================== SORTING TEST #4- sorting Vector2D objects by Y component descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::COMPONENT_Y);
		sortingParameters.SetSortingDirection(Sorting::DESCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by Y component", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].GetY() > vectors[i].GetY()) || ((AlmostEqual(vectors[i + 1].GetY(), vectors[i].GetY())) && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in DESCENDING order by Y component failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), vectors[i + 1].GetY(), i, vectors[i].ToString().c_str(), vectors[i].GetY());
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (DESCENDING order by Y component):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by Y component failed.");
		/* ==================== SORTING TEST #4- sorting Vector2D objects by Y component descending end ==================== */

		/* ==================== SORTING TEST #5- sorting Vector2D objects by sum of components ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::ASCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfComponents1 = vectors[i].SumOfComponents();
			Math::Real sumOfComponents2 = vectors[i + 1].SumOfComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfComponents2, sumOfComponents1);
			if ((!firstCriterionEqual && sumOfComponents2 < sumOfComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in ASCENDING order by sum of components failed. Vectors[%d] (%s) (sorting key value = %.5f) should precede Vectors[%d] (%s) (sorting key value = %.5f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), sumOfComponents2, i, vectors[i].ToString().c_str(), sumOfComponents1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (ASCENDING order by sum of components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by sum of components failed.");
		/* ==================== SORTING TEST #5- sorting Vector2D objects by sum of components ascending end ==================== */

		/* ==================== SORTING TEST #6- sorting Vector2D objects by sum of components descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::DESCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfComponents1 = vectors[i].SumOfComponents();
			Math::Real sumOfComponents2 = vectors[i + 1].SumOfComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfComponents2, sumOfComponents1);
			if ((!firstCriterionEqual && sumOfComponents2 > sumOfComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in DESCENDING order by sum of components failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), sumOfComponents2, i, vectors[i].ToString().c_str(), sumOfComponents1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (DESCENDING order by sum of components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by sum of components failed.");
		/* ==================== SORTING TEST #6- sorting Vector2D objects by sum of components descending end ==================== */

		/* ==================== SORTING TEST #7- sorting Vector2D objects by sum of absolute components ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_ABSOLUTE_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::ASCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of absolute components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfAbsoluteComponents1 = vectors[i].SumOfAbsoluteComponents();
			Math::Real sumOfAbsoluteComponents2 = vectors[i + 1].SumOfAbsoluteComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfAbsoluteComponents2, sumOfAbsoluteComponents1);
			if ((!firstCriterionEqual && sumOfAbsoluteComponents2 < sumOfAbsoluteComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in ASCENDING order by sum of absolute components failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), sumOfAbsoluteComponents2, i, vectors[i].ToString().c_str(), sumOfAbsoluteComponents1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (ASCENDING order by sum of absolute components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by sum of absolute components failed.");
		/* ==================== SORTING TEST #7- sorting Vector2D objects by sum of absolute components ascending end ==================== */

		/* ==================== SORTING TEST #8- sorting Vector2D objects by sum of absolute components descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_ABSOLUTE_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::DESCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of absolute components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfAbsoluteComponents1 = vectors[i].SumOfAbsoluteComponents();
			Math::Real sumOfAbsoluteComponents2 = vectors[i + 1].SumOfAbsoluteComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfAbsoluteComponents2, sumOfAbsoluteComponents1);
			if ((!firstCriterionEqual && sumOfAbsoluteComponents2 > sumOfAbsoluteComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in DESCENDING order by sum of absolute components failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), sumOfAbsoluteComponents2, i, vectors[i].ToString().c_str(), sumOfAbsoluteComponents1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (DESCENDING order by sum of absolute components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by sum of absolute components failed.");
		/* ==================== SORTING TEST #8- sorting Vector2D objects by sum of absolute components descending end ==================== */

		/* ==================== SORTING TEST #9- sorting Vector2D objects by sum of squared components ascending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_SQUARED_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::ASCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of squared components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real lengthSquared1 = vectors[i].LengthSquared();
			Math::Real lengthSquared2 = vectors[i + 1].LengthSquared();
			bool firstCriterionEqual = AlmostEqual(lengthSquared2, lengthSquared1);
			if ((!firstCriterionEqual && lengthSquared2 < lengthSquared1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in ASCENDING order by sum of squared components failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), lengthSquared2, i, vectors[i].ToString().c_str(), lengthSquared1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (ASCENDING order by sum of squared components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in ASCENDING order by sum of squared components failed.");
		/* ==================== SORTING TEST #9- sorting Vector2D objects by sum of squared components ascending end ==================== */

		/* ==================== SORTING TEST #10- sorting Vector2D objects by sum of squared components descending begin ==================== */
		sortingTestCasePassed = true;
		sortingParameters.SetSortingKey(Sorting::SUM_OF_SQUARED_COMPONENTS);
		sortingParameters.SetSortingDirection(Sorting::DESCENDING);
		timer.Reset();
		for (int k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
		{
			for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
			{
				vectors[i] = initialVectors[i];
			}
			sorter->Sort(vectors, NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of squared components", timer, Timing::SECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real lengthSquared1 = vectors[i].LengthSquared();
			Math::Real lengthSquared2 = vectors[i + 1].LengthSquared();
			bool firstCriterionEqual = AlmostEqual(lengthSquared2, lengthSquared1);
			if ((!firstCriterionEqual && lengthSquared2 > lengthSquared1) ||
				(firstCriterionEqual && (vectors[i + 1].GetX() > vectors[i].GetX())))
			{
				ERROR_LOG("%s in DESCENDING order by sum of squared components failed. Vectors[%d] (%s) (sorting key value = %.4f) should precede Vectors[%d] (%s) (sorting key value = %.4f)",
					sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str(), i + 1, vectors[i + 1].ToString().c_str(), lengthSquared2, i, vectors[i].ToString().c_str(), lengthSquared1);
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG("%s results (DESCENDING order by sum of squared components):\n", sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]].c_str());
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG("Vectors[%d] = %s", i, vectors[i].ToString().c_str());
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by sum of squared components failed.");
		/* ==================== SORTING TEST #10- sorting Vector2D objects by sum of squared components descending end ==================== */
		SAFE_DELETE(sorter);
	}

	SAFE_DELETE_JUST_TABLE(initialVectors);
	SAFE_DELETE_JUST_TABLE(vectors);
	NOTICE_LOG("Sorting test finished");
}

void KDTreeTest()
{
	if (!kdTreeTestEnabled)
	{
		return;
	}

	NOTICE_LOG("K-d tree test started");
	const int NUMBER_OF_POSITIONS = 6;
	const int NUMBER_OF_SEARCH_POSITIONS = 10;
	const int NUMBER_OF_INTERPOLATED_NEAREST_POINTS = 1; // number of nearest points which will be used for calculating the final result
	const Real LOWER_BOUND_X = -20.0f; const Real HIGHER_BOUND_X = 20.0f;
	const Real LOWER_BOUND_Y = -20.0f; const Real HIGHER_BOUND_Y = 20.0f;
	const Real LOWER_BOUND_Z = -20.0f; const Real HIGHER_BOUND_Z = 20.0f;
	Vector3D* positions = new Vector3D[NUMBER_OF_POSITIONS];
	positions[0].SetX(2.0f); positions[0].SetY(3.0f); positions[0].SetZ(2.0f);
	positions[1].SetX(5.0f); positions[1].SetY(4.0f); positions[1].SetZ(4.0f);
	positions[2].SetX(9.0f); positions[2].SetY(6.0f); positions[2].SetZ(-2.0f);
	positions[3].SetX(4.0f); positions[3].SetY(7.0f); positions[3].SetZ(1.0f);
	positions[4].SetX(8.0f); positions[4].SetY(1.0f); positions[4].SetZ(12.0f);
	positions[5].SetX(7.0f); positions[5].SetY(2.0f); positions[5].SetZ(6.0f);
	//for (int i = 0; i < NUMBER_OF_POSITIONS; ++i)
	//{
	//	positions[i].SetX(g_randomGenerator.NextFloat(LOWER_BOUND_X, HIGHER_BOUND_X));
	//	positions[i].SetY(g_randomGenerator.NextFloat(LOWER_BOUND_Y, HIGHER_BOUND_Y));
	//	positions[i].SetZ(g_randomGenerator.NextFloat(LOWER_BOUND_Z, HIGHER_BOUND_Z));
	//	DEBUG_LOG("positions[%d] = %s", i, positions[i].ToString().c_str());
	//}

	KDTree* kdTree = new KDTree(positions, NUMBER_OF_POSITIONS, NUMBER_OF_INTERPOLATED_NEAREST_POINTS);

	//DEBUG_LOG("K-d tree structure:\n%s", kdTree->ToString().c_str());

	Math::Vector2D* searchPositions = new Math::Vector2D[NUMBER_OF_SEARCH_POSITIONS];
	searchPositions[0].SetX(2.0f); searchPositions[0].SetY(3.0f);
	searchPositions[1].SetX(5.0f); searchPositions[1].SetY(4.0f);
	searchPositions[2].SetX(9.0f); searchPositions[2].SetY(6.0f);
	searchPositions[3].SetX(4.0f); searchPositions[3].SetY(7.0f);
	searchPositions[4].SetX(8.0f); searchPositions[4].SetY(1.0f);
	searchPositions[5].SetX(7.0f); searchPositions[5].SetY(2.0f);
	searchPositions[6].SetX(6.0f); searchPositions[6].SetY(2.0f);
	searchPositions[7].SetX(7.0f); searchPositions[7].SetY(3.0f);
	searchPositions[8].SetX(8.0f); searchPositions[8].SetY(4.0f);
	searchPositions[9].SetX(9.0f); searchPositions[9].SetY(5.0f);
	for (int i = 0; i < NUMBER_OF_SEARCH_POSITIONS; ++i)
	{
		Real height = kdTree->SearchNearestValue(searchPositions[i]);
		//INFO_LOG("The nearest point for search point (%s) is (%s). The minimum distance equals %.2f and returned height is %.2f",
		//	searchPositions[i].ToString().c_str(), minDistancePositions[0].ToString().c_str(), minDistances[0], height);
		INFO_LOG("The final result for position (%s) in k-d tree search is %.3f", searchPositions[i].ToString().c_str(), height);
	}

	SAFE_DELETE(kdTree);
	SAFE_DELETE_JUST_TABLE(searchPositions);
	SAFE_DELETE_JUST_TABLE(positions);
}

void StatsTest()
{
	if (!statsTestEnabled)
	{
		return;
	}
	Timing::Timer timer;
	timer.Start();

	const int STATS_TEST_1_METHOD_1_INVOCATIONS_COUNT = 5;
	const int STATS_TEST_1_METHOD_2_INVOCATIONS_COUNT = 150;
	const int STATS_TEST_1_METHOD_3_INVOCATIONS_COUNT = 4;
	const int STATS_TEST_2_METHOD_1_INVOCATIONS_COUNT = 2;
	const int STATS_TEST_2_METHOD_2_INVOCATIONS_COUNT = 1000;
	const int STATS_TEST_2_METHOD_3_INVOCATIONS_COUNT = 8;

	MathTest::StatsTest1 statsTest1;
	for (int i = 0; i < STATS_TEST_1_METHOD_1_INVOCATIONS_COUNT; ++i)
	{
		float floatValue1 = statsTest1.Method1();
	}
	for (int i = 0; i < STATS_TEST_1_METHOD_2_INVOCATIONS_COUNT; ++i)
	{
		float floatValue2 = statsTest1.Method2();
	}
	for (int i = 0; i < STATS_TEST_1_METHOD_3_INVOCATIONS_COUNT; ++i)
	{
		float floatValue3 = statsTest1.Method3();
	}
	
	MathTest::StatsTest2 statsTest2;
	for (int i = 0; i < STATS_TEST_2_METHOD_1_INVOCATIONS_COUNT; ++i)
	{
		float floatValue1 = statsTest2.Method1();
	}
	for (int i = 0; i < STATS_TEST_2_METHOD_2_INVOCATIONS_COUNT; ++i)
	{
		float floatValue2 = statsTest2.Method2();
	}
	for (int i = 0; i < STATS_TEST_2_METHOD_3_INVOCATIONS_COUNT; ++i)
	{
		float floatValue3 = statsTest2.Method3();
	}

	timer.Stop();

	//double anotherTotalElapsedTime = CalculateElapsedTime(outerBegin, outerEnd, SECONDS);
	//std::cout << "Outer end = " << outerEnd << "[ms]. OuterBegin = " << outerBegin << "[ms]." << std::endl;
	//std::cout << "Total elapsed time = " << totalElapsedTime << "[s]. Another total elapsed time = " << anotherTotalElapsedTime << "[s]." << std::endl;
	STATS_STORAGE.PrintReport(timer.GetTimeSpan(Timing::SECOND));
}

int main (int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	ICommand::SetCommand(argc, argv);
	//if (Command.IsPresent("-help"))
	//{
	//	PrintHelp();
	//	system("pause");
	//	return 0;
	//}
	ILogger::GetLogger().Fill(ICommand::GetCommand().Get("-log", ""), Info);

	AngleTest();
	VectorTest();
	QuaternionTest();
	MatrixTest();
	SortTest();
	//SortTestTime();

	KDTreeTest();

	StatsTest();

	ILogger::GetLogger().ResetConsoleColor();
	std::cout << "Bye!" << std::endl;

	return 0;
}
