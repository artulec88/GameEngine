#include "Math\Angle.h"
#include "Math\FloatingPoint.h"
#include "Math\Matrix.h"
#include "Math\NewMatrix.h"
#include "Math\Quaternion.h"
#include "Math\Vector.h"
#include "Math\ISort.h"
#include "Math\KDTree.h"
#include "Math\HeightsGenerator.h"
#include "Math\RandomGeneratorFactory.h"

#include "Utility\ICommandLineMapper.h"
#include "Utility\ILogger.h"
#include "Utility\Time.h"

#include "StatsTest.h"
#include "AngleTestGroup.h"
#include "VectorTestGroup.h"
#include "QuaternionTestGroup.h"
#include "MatrixTestGroup.h"
#include "TransformTestGroup.h"
#include "Def.h"

#include <ctime>
#include <string>
#include <thread>
#include <xmmintrin.h>
//#include <iostream>
//#include <fstream>

using namespace Math;
using namespace Utility;
using namespace std;

unsigned int innerLoops = 10000;
unsigned int outerLoops = 10000;
double elapsedTime;

unsigned int testNumber = 0;
bool angleTestEnabled = true;
bool vectorTestEnabled = true;
bool matrixTestEnabled = true;
bool transformTestEnabled = true;
bool quaternionTestEnabled = true;
bool sortingTestEnabled = true;
bool kdTreeTestEnabled = true;
bool statsTestEnabled = true;
bool heightsGeneratorTestsEnabled = true;
bool otherTestsEnabled = true;

const Math::Random::RandomGenerator& g_randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::GeneratorIDs::SIMPLE);
const std::string MODULE_NAME = "MathTest";

void ReportError(const std::string& reportStr)
{
	ERROR_LOG_MATH_TEST("Test #", testNumber, " FAILED. ", reportStr);
}

void TestReport(bool statusCode /* false if error */, const std::string& reportErrorStr)
{
	testNumber++;
	if (statusCode == false)
	{
		ReportError(reportErrorStr);
	}
	INFO_LOG_MATH_TEST("Test #", testNumber, " passed");
}

void TimeReport(const std::string& reportStr, Timing::Timer& timer, Timing::TimeUnit timeUnit, const unsigned int NUMBER_OF_ITERATIONS = 1)
{
	//CHECK_CONDITION_EXIT_ALWAYS_MATH_TEST(!timer.IsRunning(), Logging::ERR, "Timer is still running");
	double elapsedTime = static_cast<double>(timer.GetDuration(timeUnit));
	elapsedTime /= NUMBER_OF_ITERATIONS;
	//timeSpan.AdjustUnitToValue();
	NOTICE_LOG_MATH_TEST(reportStr, ":\t", elapsedTime, " ", Timing::DateTime::ConvertTimeUnitToString(timeUnit));
}

void TestAngleConstructionTime(const unsigned int NUMBER_OF_ITERATIONS)
{
	Timing::Timer timer;
	timer.Start();
	for (unsigned int i = 0; i < NUMBER_OF_ITERATIONS; ++i)
	{
		Angle angle(34.0f);
	}
	timer.Stop();
	TimeReport("Average time for angle creation:\t", timer, Timing::NANOSECOND, NUMBER_OF_ITERATIONS);
}

void TestAngleSinCalculationTime(const unsigned int NUMBER_OF_ITERATIONS)
{
	Timing::Timer timer;
	timer.Start();
	for (unsigned int i = 0; i < NUMBER_OF_ITERATIONS; ++i)
	{
		Angle angle(34.0f);
		angle.Sin();
	}
	timer.Stop();
	TimeReport("Average time for angle sinus calculation #1:\t", timer, Timing::NANOSECOND, NUMBER_OF_ITERATIONS);

	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_ITERATIONS; ++i)
	{
		Angle angle(34.0f);
		angle.FastSin1();
	}
	timer.Stop();
	TimeReport("Average time for angle sinus calculation #2:\t", timer, Timing::NANOSECOND, NUMBER_OF_ITERATIONS);

	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_ITERATIONS; ++i)
	{
		Angle angle(34.0f);
		angle.FastSin2();
	}
	timer.Stop();
	TimeReport("Average time for angle sinus calculation #3:\t", timer, Timing::NANOSECOND, NUMBER_OF_ITERATIONS);
}

void TestAngleCosCalculationTime(const unsigned int NUMBER_OF_ITERATIONS)
{
	Timing::Timer timer;
	timer.Start();
	for (unsigned int i = 0; i < NUMBER_OF_ITERATIONS; ++i)
	{
		Angle angle(34.0f);
		angle.Cos();
	}
	timer.Stop();
	TimeReport("Average time for angle cosinus calculation #1:\t", timer, Timing::NANOSECOND, NUMBER_OF_ITERATIONS);

	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_ITERATIONS; ++i)
	{
		Angle angle(34.0f);
		angle.FastCos1();
	}
	timer.Stop();
	TimeReport("Average time for angle cosinus calculation #2:\t", timer, Timing::NANOSECOND, NUMBER_OF_ITERATIONS);

	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_ITERATIONS; ++i)
	{
		Angle angle(34.0f);
		angle.FastCos2();
	}
	timer.Stop();
	TimeReport("Average time for angle cosinus calculation #3:\t", timer, Timing::NANOSECOND, NUMBER_OF_ITERATIONS);
}

void TestAngleTanCalculationTime(const unsigned int NUMBER_OF_ITERATIONS)
{
	Timing::Timer timer;
	timer.Start();
	for (unsigned int i = 0; i < NUMBER_OF_ITERATIONS; ++i)
	{
		Angle angle(34.0f);
		angle.Tan();
	}
	timer.Stop();
	TimeReport("Average time for angle tangent calculation #1:\t", timer, Timing::NANOSECOND, NUMBER_OF_ITERATIONS);

	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_ITERATIONS; ++i)
	{
		Angle angle(34.0f);
		angle.FastTan1();
	}
	timer.Stop();
	TimeReport("Average time for angle tangent calculation #2:\t", timer, Timing::NANOSECOND, NUMBER_OF_ITERATIONS);

	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_ITERATIONS; ++i)
	{
		Angle angle(34.0f);
		angle.FastTan2();
	}
	timer.Stop();
	TimeReport("Average time for angle tangent calculation #3:\t", timer, Timing::NANOSECOND, NUMBER_OF_ITERATIONS);
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
	angleTests.AddTest(new MathTest::AngleTestTrigonometry(angle1, REAL_ONE, REAL_ZERO, REAL_ZERO));
	angleTests.AddTest(new MathTest::AngleTestTrigonometry(angle2, REAL_ONE, REAL_ZERO, REAL_ZERO));
	angleTests.AddTest(new MathTest::AngleTestTrigonometry(angle3, REAL_ONE, REAL_ZERO, REAL_ZERO));
	angleTests.AddTest(new MathTest::AngleTestTrigonometry(angle4, 0.027412f, 0.999624f, 0.027422f));
	angleTests.AddTest(new MathTest::AngleTestTrigonometry(angle5, static_cast<Math::Real>(sqrt(2.0) / 2.0), static_cast<Math::Real>(sqrt(2.0) / 2.0), REAL_ONE));
	angleTests.AddTest(new MathTest::AngleTestTrigonometry(angle6, static_cast<Math::Real>(sqrt(2.0) / 2.0), static_cast<Math::Real>(sqrt(2.0) / 2.0), REAL_ONE));
	angleTests.AddTest(new MathTest::AngleTestOperators(angle1, angle2, 0.3f, Angle(180.0f), Angle(0.0f), Angle(27.0f), Angle(300.0f), Angle(27.0f), Angle(300.0f)));
	angleTests.AddTest(new MathTest::AngleTestOperators(angle1, angle3, 2.0f, Angle(180.0f), Angle(0.0f), Angle(180.0f), Angle(45.0f), Angle(180.0f), Angle(45.0f)));
	angleTests.AddTest(new MathTest::AngleTestOperators(angle1, angle4, 5.0f, Angle(91.5708f), Angle(88.4292037f), Angle(450.0f), Angle(18.0f), Angle(PI * 5.0f / 2.0f), Angle(PI / 10.0f)));
	angleTests.AddTest(new MathTest::AngleTestOperators(angle1, angle5, 0.5f, Angle(135.0f), Angle(45.0f), Angle(45.0f), Angle(180.0f), Angle(22.5f), Angle(90.0f)));
	angleTests.AddTest(new MathTest::AngleTestOperators(angle1, angle6, 0.3f, Angle(135.0f), Angle(45.0f), Angle(27.0f), Angle(300.0f), Angle(13.5f), Angle(150.0f)));
	angleTests.AddTest(new MathTest::AngleTestOperators(Angle(30.0f), Angle(60.0f), 3.0f, Angle(90.0f), Angle(-30.0f), Angle(90.0f), Angle(10.0f), Angle(180.0f), Angle(20.0f)));
	angleTests.AddTest(new MathTest::AngleTestOperators(Angle(40.0f), Angle(20.0f), 0.1f, Angle(60.0f), Angle(20.0f), Angle(4.0f), Angle(400.0f), Angle(2.0f), Angle(200.0f)));

	angleTests.StartTests();
	angleTests.StartTimeTests();

	TestAngleConstructionTime(100000);
	TestAngleSinCalculationTime(10000);
	TestAngleCosCalculationTime(10000);
	TestAngleTanCalculationTime(10000);
}

void VectorTest()
{
	if (!vectorTestEnabled)
	{
		return;
	}
	MathTest::VectorTestGroup vectorTests;
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector2D>(ZERO_VECTOR_2D, ZERO_VECTOR_2D, true));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector2D>(ZERO_VECTOR_2D, Math::Vector2D(0.0f, 1.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector2D>(ZERO_VECTOR_2D, Math::Vector2D(1.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector2D>(ZERO_VECTOR_2D, Math::Vector2D(1.0f, 1.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 0.0f, 0.0f), true));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), Math::Vector3D(1.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), Math::Vector3D(1.0f, 1.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), true));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Math::Vector4D(0.0f, 1.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Math::Vector4D(1.0f, 0.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestCompare<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), Math::Vector4D(1.0f, 1.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector2D>(ZERO_VECTOR_2D, 0.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector2D>(Math::Vector2D(1.0f, 0.0f), 1.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector2D>(Math::Vector2D(12.0f, -4.0f), 4.0f * sqrtf(10.0f)));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), 0.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector3D>(Math::Vector3D(1.0f, 0.0f, 0.0f), 1.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector3D>(Math::Vector3D(3.0f, 4.0f, 12.0f), 13.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), 0.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector4D>(Math::Vector4D(1.0f, 0.0f, 0.0f, 0.0f), 1.0f));
	vectorTests.AddTest(new MathTest::VectorTestLength<Math::Vector4D>(Math::Vector4D(1.0f, 2.0f, 5.0f, -3.0f), sqrtf(39.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector2D>(Math::Vector2D(0.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), Math::Vector2D(1.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector2D>(ZERO_VECTOR_2D, Math::Vector2D(1.0f, 0.0f), Math::Vector2D(1.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector2D>(Math::Vector2D(1.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), Math::Vector2D(2.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector3D>(Math::Vector3D(0.0f, 1.0f, 0.0f), Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(1.0f, 1.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, 0.0f), Math::Vector3D(1.0f, 0.0f, -1.0f), Math::Vector3D(1.0f, 0.0f, -1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector3D>(Math::Vector3D(1.0f, 1.0f, -3.0f), Math::Vector3D(1.0f, 0.0f, -2.0f), Math::Vector3D(2.0f, 1.0f, -5.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector4D>(Math::Vector4D(0.0f, 1.0f, 2.0f, 3.0f), Math::Vector4D(1.0f, 0.0f, -1.0f, 0.0f), Math::Vector4D(1.0f, 1.0f, 1.0f, 3.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, -3.0f, 4.0f), Math::Vector4D(1.0f, 0.0f, 2.0f, -3.0f), Math::Vector4D(1.0f, 0.0f, -1.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestAddOperator<Math::Vector4D>(Math::Vector4D(1.0f, 1.0f, 5.0f, 6.0f), Math::Vector4D(1.0f, 0.0f, 5.0f, 5.0f), Math::Vector4D(2.0f, 1.0f, 10.0f, 11.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector2D>(Math::Vector2D(0.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), Math::Vector2D(-1.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector2D>(ZERO_VECTOR_2D, Math::Vector2D(1.0f, 0.0f), Math::Vector2D(-1.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector2D>(Math::Vector2D(1.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), Math::Vector2D(0.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector3D>(Math::Vector3D(0.0f, 1.0f, 2.0f), Math::Vector3D(1.0f, 0.0f, 2.0f), Math::Vector3D(-1.0f, 1.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, -2.0f), Math::Vector3D(1.0f, 0.0f, -1.0f), Math::Vector3D(-1.0f, 0.0f, -1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector3D>(Math::Vector3D(1.0f, 1.0f, 3.0f), Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.0f, 1.0f, 3.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector4D>(Math::Vector4D(0.0f, 1.0f, 2.0f, -2.0f), Math::Vector4D(1.0f, 0.0f, -2.0f, -3.0f), Math::Vector4D(-1.0f, 1.0f, 4.0f, 1.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, -4.0f, -5.0f), Math::Vector4D(1.0f, 0.0f, -5.0f, 4.0f), Math::Vector4D(-1.0f, 0.0f, 1.0f, -9.0f)));
	vectorTests.AddTest(new MathTest::VectorTestSubtractOperator<Math::Vector4D>(Math::Vector4D(1.0f, 1.0f, -8.0f, 5.0f), Math::Vector4D(1.0f, 0.0f, 6.0f, -7.0f), Math::Vector4D(0.0f, 1.0f, -14.0f, 12.0f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector2D>(Math::Vector2D(0.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), 0.2f, ZERO_VECTOR_2D, Math::Vector2D(0.0f, 0.2f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector2D>(ZERO_VECTOR_2D, Math::Vector2D(1.0f, 0.0f), 0.5f, Math::Vector2D(0.0f, 0.0f), Math::Vector2D(0.0f, 0.0f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector2D>(Math::Vector2D(1.0f, 1.0f), Math::Vector2D(1.0f, 0.0f), 0.75f, Math::Vector2D(1.0f, 0.0f), Math::Vector2D(0.75f, 0.75f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector3D>(Math::Vector3D(0.0f, 1.0f, 2.0f), Math::Vector3D(1.0f, 0.0f, 4.0f), 0.2f, Math::Vector3D(0.0f, 0.0f, 8.0f), Math::Vector3D(0.0f, 0.2f, 0.4f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector3D>(Math::Vector3D(0.0f, 0.0f, -1.0f), Math::Vector3D(1.0f, 0.0f, -5.0f), 0.5f, Math::Vector3D(0.0f, 0.0f, 5.0f), Math::Vector3D(0.0f, 0.0f, -0.5f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector3D>(Math::Vector3D(1.0f, 1.0f, 5.0f), Math::Vector3D(1.0f, 0.0f, 0.0f), 0.75f, Math::Vector3D(1.0f, 0.0f, 0.0f), Math::Vector3D(0.75f, 0.75f, 3.75f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector4D>(Math::Vector4D(0.0f, 1.0f, 2.0f, 4.0f), Math::Vector4D(1.0f, 0.0f, 4.0f, -6.0f), 0.2f, Math::Vector4D(0.0f, 0.0f, 8.0f, -24.0f), Math::Vector4D(0.0f, 0.2f, 0.4f, 0.8f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector4D>(Math::Vector4D(0.0f, 0.0f, -5.0f, -1.0f), Math::Vector4D(1.0f, 0.0f, 0.0f, 1.0f), 0.5f, Math::Vector4D(0.0f, 0.0f, 0.0f, -1.0f), Math::Vector4D(0.0f, 0.0f, -2.5f, -0.5f)));
	vectorTests.AddTest(new MathTest::VectorTestMultiplyOperator<Math::Vector4D>(Math::Vector4D(1.0f, 1.0f, 0.0f, 2.0f), Math::Vector4D(1.0f, 0.0f, -2.0f, 5.0f), 0.75f, Math::Vector4D(1.0f, 0.0f, 0.0f, 10.0f), Math::Vector4D(0.75f, 0.75f, 0.0f, 1.5f)));
	vectorTests.AddTest(new MathTest::VectorTestDivideOperator<Math::Vector2D>(Math::Vector2D(8.0f, 1.0f), Math::Vector2D(1.0f, 2.0f), 0.2f, Math::Vector2D(8.0f, 0.5f), Math::Vector2D(40.0f, 5.0f)));
	vectorTests.AddTest(new MathTest::VectorTestDivideOperator<Math::Vector2D>(ZERO_VECTOR_2D, Math::Vector2D(1.0f, 5.0f), 0.5f, Math::Vector2D(0.0f, 0.0f), Math::Vector2D(0.0f, 0.0f)));
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
	vectorTests.StartTimeTests();
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

	quaternionTests.StartTests();
	quaternionTests.StartTimeTests();

	/* ==================== QUATERNION TEST #2- calculating rotation matrix- begin ==================== */
	INFO_LOG_MATH_TEST("Profiling quaternion to rotation matrix conversion functions begin");
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
	DEBUG_LOG_MATH_TEST("Profiling quaternion to rotation matrix conversion functions end.");
	/* ==================== QUATERNION TEST #2- calculating rotation matrix- end ==================== */

	/* ==================== QUATERNION TEST #3- calculating linear interpolation- begin ==================== */
	INFO_LOG_MATH_TEST("Profiling quaternion linear interpolation function begin");
	const int QUATERNION_LINEAR_INTERPOLATION_ITERATIONS = 1000000;
	timer.Reset();
	for (int i = 0; i < QUATERNION_LINEAR_INTERPOLATION_ITERATIONS; ++i)
	{
		Quaternion q1 = RandomQuaternion();
		Quaternion q2 = RandomQuaternion();
		q1.Nlerp(q2, g_randomGenerator.NextFloat(0.0f, 1.0f), false);
	}
	timer.Stop();
	TimeReport("Average time for quaternion linear interpolation function", timer, Timing::MICROSECOND, QUATERNION_LINEAR_INTERPOLATION_ITERATIONS);

	DEBUG_LOG_MATH_TEST("Profiling quaternion linear interpolation functions end.");
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
	if (min > max)
	{
		Real temp = min;
		min = max;
		max = temp;
	}
	Matrix4D matrix;
	for (int i = 0; i < Matrix4D::SIZE; ++i)
	{
		for (int j = 0; j < Matrix4D::SIZE; ++j)
		{
			Real value = g_randomGenerator.NextFloat(min, max);
			matrix.SetElement(i, j, value);
		}
	}
	return matrix;
}

void M4x4_SSE(const Math::Real* A, const Math::Real* B, Math::Real* C) {
	__m128 row1 = _mm_load_ps(&B[0]);
	__m128 row2 = _mm_load_ps(&B[4]);
	__m128 row3 = _mm_load_ps(&B[8]);
	__m128 row4 = _mm_load_ps(&B[12]);
	for (int i = 0; i < 4; i++) {
		__m128 brod1 = _mm_set1_ps(A[4 * i + 0]);
		__m128 brod2 = _mm_set1_ps(A[4 * i + 1]);
		__m128 brod3 = _mm_set1_ps(A[4 * i + 2]);
		__m128 brod4 = _mm_set1_ps(A[4 * i + 3]);
		__m128 row = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(brod1, row1),
				_mm_mul_ps(brod2, row2)),
			_mm_add_ps(
				_mm_mul_ps(brod3, row3),
				_mm_mul_ps(brod4, row4)));
		_mm_store_ps(&C[4 * i], row);
	}
}

void NewMatrixTest()
{
	//auto matrix = Math::Matrix<int, 100>::IDENTITY;
	//for (int test = 0; test < 4; ++test)
	//{
	//	switch (test)
	//	{
	//	case matrix[5][5] * matrix[5][6]:
	//		CRITICAL_LOG_MATH_TEST("Test == 0");
	//		break;
	//	case matrix[2][2]:
	//		CRITICAL_LOG_MATH_TEST("Test == 1");
	//		break;
	//	case matrix[2][2] + matrix[3][3]:
	//		CRITICAL_LOG_MATH_TEST("Test == 2");
	//		break;
	//	default:
	//		CRITICAL_LOG_MATH_TEST("Default");
	//		break;
	//	}
	//}

	auto identityMatrix1 = Math::Matrix<int, 60, 45, MatrixOrderings::ROW_MAJOR>::IDENTITY;
	auto identityMatrix2 = Math::Matrix<int, 45, 70, MatrixOrderings::ROW_MAJOR>::IDENTITY;

	/* ==================== MATRIX TEST #1 begin ==================== */
	constexpr int NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS = 1000000;
	Timing::Timer timer;
	timer.Start();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS; ++i)
	{
		auto m = Math::Matrix<int, 33, 44, MatrixOrderings::ROW_MAJOR>::IDENTITY;
	}
	timer.Stop();
	TimeReport("Average time for identity matrix creation:\t", timer, Timing::NANOSECOND, NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS);
	/* ==================== MATRIX TEST #1 end ==================== */

	/* ==================== MATRIX TEST #2 begin ==================== */
	constexpr int NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS = 2000000;
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS; ++i)
	{
		auto result = identityMatrix1 * identityMatrix2; // FIXME: Check matrix multiplication
		CHECK_CONDITION_MATH_TEST(result == identityMatrix1 * identityMatrix2, Utility::Logging::ERR, "Identity matrix multiplication result is incorrect.");
	}
	timer.Stop();
	TimeReport("Average time for identity matrices multiplication:\t", timer, Timing::NANOSECOND, NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS);
	/* ==================== MATRIX TEST #2 end ==================== */
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
	Matrix4D matrix13(Vector2D(5.0f, -2.0f), Math::Angle(0.0f), Vector2D(0.5f, 1.2f));
	Matrix4D matrix14(Vector3D(1.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f)); // rotation matrix
	Matrix4D matrix15(Vector3D(1.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 2.0f, 5.0f)); // rotation matrix
	Matrix4D matrix16(1, 2, 3, 0, 5, 6, 7, 0, 9, 10, 11, 0, 13, 14, 15, 1);
	Matrix4D matrix17(-1, 2, -3, 4, -5, 6, -7, 8, -9, 10, -11, 12, -13, 14, -15, 16);
	Matrix4D leftMultiplication(-38, 44, -50, 56, -98, 116, -134, 152, -158, 188, -218, 248, -231, 274, -317, 360);
	Matrix4D rightMultiplication(34, 36, 38, 4, 66, 68, 70, 8, 98, 100, 102, 12, 130, 132, 134, 16);

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
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByMatrixOperator(matrix16, matrix17, rightMultiplication));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByMatrixOperator(matrix17, matrix16, leftMultiplication));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix1, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix2, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix3, Vector3D(2, 3, 5), Vector3D(4, 6, 10)));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix4, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix5, Vector3D(2, 3, 5), Vector3D(5, -2, 7)));
	matrixTests.AddTest(new MathTest::MatrixTestMultiplyByVectorOperator(matrix6, Vector3D(2, 3, 5), Vector3D(0.2f, 0.3f, -1.102102102102102102102102f)));

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
	matrixTests.StartTimeTests();

	Matrix4D identityMatrix1(Matrix4D::IDENTITY_MATRIX);
	Matrix4D identityMatrix2(Matrix4D::IDENTITY_MATRIX);

	/* ==================== MATRIX TEST #1 begin ==================== */
	constexpr int NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS = 1000000;
	Matrix4D testMatrix1(Matrix4D::IDENTITY_MATRIX);
	TestReport(testMatrix1.IsIdentity(), "The function Matrix::IsIdentity() failed.");

	Timing::Timer timer;
	timer.Start();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS; ++i)
	{
		Matrix4D testMatrix2(Matrix4D::IDENTITY_MATRIX);
	}
	timer.Stop();
	TimeReport("Average time for identity matrix creation:\t", timer, Timing::NANOSECOND, NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS);
	/* ==================== MATRIX TEST #1 end ==================== */

	/* ==================== MATRIX TEST #2 begin ==================== */
	constexpr int NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS = 2000000;
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS; ++i)
	{
		Matrix4D result = identityMatrix1 * identityMatrix2; // FIXME: Check matrix multiplication
		CHECK_CONDITION_MATH_TEST(result == identityMatrix1 * identityMatrix2, Utility::Logging::ERR, "Identity matrix multiplication result is incorrect.");
	}
	timer.Stop();
	TimeReport("Average time for identity matrices multiplication:\t", timer, Timing::NANOSECOND, NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS);
	/* ==================== MATRIX TEST #2 end ==================== */

	/* ==================== MATRIX TEST #3 begin ==================== */
	constexpr int NUMBER_OF_RANDOM_MATRIX_MULTIPLICATION_ITERATIONS = 2000000;
	Matrix4D mat1 = RandomMatrix(REAL_ZERO, REAL_ONE);
	Matrix4D mat2 = RandomMatrix(REAL_ZERO, REAL_ONE);
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_RANDOM_MATRIX_MULTIPLICATION_ITERATIONS; ++i)
	{
		Matrix4D result = mat1 * mat2; // FIXME: Check matrix multiplication
		//CHECK_CONDITION_MATH_TEST(result == mat1 * mat2, Utility::ERR, "Random matrix multiplication result is incorrect.");
	}
	timer.Stop();
	TimeReport("Average time for random matrices multiplication:\t", timer, Timing::NANOSECOND, NUMBER_OF_RANDOM_MATRIX_MULTIPLICATION_ITERATIONS);
	/* ==================== MATRIX TEST #3 end ==================== */

	/* ==================== MATRIX TEST #4 begin ==================== */
	constexpr int NUMBER_OF_ROTATION_EULER_ITERATIONS = 100000;
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_ROTATION_EULER_ITERATIONS; ++i)
	{
		Matrix4D result(RandomAngle(), RandomAngle(), RandomAngle());
	}
	timer.Stop();
	TimeReport("Average time for rotation Euler matrix calculation:\t", timer, Timing::NANOSECOND, NUMBER_OF_ROTATION_EULER_ITERATIONS);
	/* ==================== MATRIX TEST #4 end ==================== */

	constexpr int NUMBER_OF_SSE_MATRIX_MULTIPLICATIONS = 1000000;
	std::array<float, 16> array1d_1 = { { 1.0f, 2.0f, 3.0f, 0.0f, 5.0f, 6.0f, 7.0f, 0.0f, 9.0f, 10.0f, 11.0f, 0.0f, 13.0f, 14.0f, 15.0f, 1.0f } };
	std::array<float, 16> array1d_2 = { { -1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f, -7.0f, 8.0f, -9.0f, 10.0f, -11.0f, 12.0f, -13.0f, 14.0f, -15.0f, 16.0f } };
	std::array<float, 16> leftArray1d = { { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } };
	const std::array<float, 16> leftArrayExpected1d = { { -38.0f, 44.0f, -50.0f, 56.0f, -98.0f, 116.0f, -134.0f, 152.0f, -158.0f, 188.0f, -218.0f, 248.0f, -231.0f, 274.0f, -317.0f, 360.0f } };
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_SSE_MATRIX_MULTIPLICATIONS; ++i)
	{
		M4x4_SSE(array1d_1.data(), array1d_2.data(), leftArray1d.data());
	}
	timer.Stop();
	//for (int i = 0; i < 16; ++i)
	//{
	//	cout << "array1[" << i << "] = " << array1d_1[i] << endl;
	//	cout << "array2[" << i << "] = " << array1d_2[i] << endl;
	//	cout << "leftArray[" << i << "] = " << leftArray1d[i] << endl;
	//}
	TimeReport("Average time for SSE 1D matrix multiplication:\t", timer, Timing::NANOSECOND, NUMBER_OF_SSE_MATRIX_MULTIPLICATIONS);

	std::array<std::array<float, 4>, 4> array2d_1{ {{{ 1.0f, 2.0f, 3.0f, 0.0f}}, {{5.0f, 6.0f, 7.0f, 0.0f}}, {{9.0f, 10.0f, 11.0f, 0.0f}}, {{13.0f, 14.0f, 15.0f, 1.0f}}} };
	std::array<std::array<float, 4>, 4> array2d_2{ { { { -1.0f, 2.0f, -3.0f, 4.0f } },{ { -5.0f, 6.0f, -7.0f, 8.0f } },{ { -9.0f, 10.0f, -11.0f, 12.0f } },{ { -13.0f, 14.0f, -15.0f, 16.0f } } } };
	std::array<std::array<float, 4>, 4> leftArray2d{ {{ { 0.0f, 0.0f, 0.0f, 0.0f }}, {{0.0f, 0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f, 0.0f } }} };
	const std::array<std::array<float, 4>, 4> leftArrayExpected2d{ { { { -38.0f, 44.0f, -50.0f, 56.0f}}, {{-98.0f, 116.0f, -134.0f, 152.0f}}, {{-158.0f, 188.0f, -218.0f, 248.0f}}, {{-231.0f, 274.0f, -317.0f, 360.0f } }} };
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_SSE_MATRIX_MULTIPLICATIONS; ++i)
	{
		M4x4_SSE(array2d_1[0].data(), array2d_2[0].data(), leftArray2d[0].data());
	}
	timer.Stop();
	//for (int i = 0; i < 4; ++i)
	//{
	//	for (int j = 0; j < 4; ++j)
	//	{
	//		cout << "array1[" << i << "][" << j << "] = " << array2d_1[i][j] << endl;
	//		cout << "array2[" << i << "][" << j << "] = " << array2d_2[i][j] << endl;
	//		cout << "leftArray[" << i << "][" << j << "] = " << leftArray2d[i][j] << endl;
	//	}
	//}
	TimeReport("Average time for SSE 2D matrix multiplication:\t", timer, Timing::NANOSECOND, NUMBER_OF_SSE_MATRIX_MULTIPLICATIONS);
}

void TransformTest()
{
	if (!transformTestEnabled)
	{
		return;
	}
	MathTest::TransformTestGroup transformTests;

	transformTests.AddTest(new MathTest::TransformTestCompare(Transform(), Transform(), true));
	transformTests.AddTest(new MathTest::TransformTestParent(Transform(Vector3D(1.0f, 2.0f, 3.0f), Math::NO_ROTATION_QUATERNION, 1.0f),
		Transform(Vector3D(1.0f, 2.0f, 3.0f), Math::NO_ROTATION_QUATERNION, 1.0f),
		Transform(Vector3D(2.0f, 4.0f, 6.0f), Math::NO_ROTATION_QUATERNION, 1.0f)));
	//transformTests.AddTest(new MathTest::TransformTestParent(Transform(Vector3D(1.0f, 2.0f, 3.0f), Quaternion(Vector3D(0.0f, 1.0f, 0.0f), Angle(50.0f)), 1.0f),
	//	Transform(Vector3D(1.0f, 2.0f, 3.0f), Quaternion(Vector3D(0.0f, 1.0f, 0.0f), Angle(0.0f)), 1.0f),
	//	Transform(Vector3D(2.0f, 4.0f, 6.0f), Quaternion(Vector3D(0.0f, 1.0f, 0.0f), Angle(50.0f)), 1.0f)));

	transformTests.StartTests();
	transformTests.StartTimeTests();
}

void SortTest()
{
	if (!sortingTestEnabled)
	{
		return;
	}

	Logging::ILogger::GetLogger(MODULE_NAME).AddStream("sortingOutput.txt");
	//ofstream sortingOutputFile;
	//sortingOutputFile.open("sortingOutput.txt");

	NOTICE_LOG_MATH_TEST("Sorting test started");
	constexpr int NUMBER_OF_VECTORS = 10000;
	const Real LOWER_BOUND_X = -20.0f;
	const Real HIGHER_BOUND_X = 20.0f;
	const Real LOWER_BOUND_Y = -20.0f;
	const Real HIGHER_BOUND_Y = 20.0f;
	std::array<Vector2D, NUMBER_OF_VECTORS> initialVectors;
	std::array<Vector2D, NUMBER_OF_VECTORS> vectors;
	//initialVectors[0].x = 1.0f; initialVectors[5].y = 1.0f; // sumOfComponents = 2		sumOfAbsComponents = 2	sumOfSquaredComponents = 2
	//initialVectors[1].x = -1.0f; initialVectors[6].y = 2.0f; // sumOfComponents = 1		sumOfAbsComponents = 3	sumOfSquaredComponents = 5
	//initialVectors[2].x = 2.0f; initialVectors[7].y = 3.0f; // sumOfComponents = 5		sumOfAbsComponents = 5	sumOfSquaredComponents = 13
	//initialVectors[3].x = -2.0f; initialVectors[8].y = 4.0f; // sumOfComponents = 2		sumOfAbsComponents = 6	sumOfSquaredComponents = 20
	//initialVectors[4].x = 3.0f; initialVectors[9].y = 5.0f; // sumOfComponents = 8		sumOfAbsComponents = 8	sumOfSquaredComponents = 34
	//initialVectors[5].x = -3.0f; initialVectors[0].y = 1.0f; // sumOfComponents = -2	sumOfAbsComponents = 4	sumOfSquaredComponents = 10
	//initialVectors[6].x = 4.0f; initialVectors[1].y = 2.0f; // sumOfComponents = 6		sumOfAbsComponents = 6	sumOfSquaredComponents = 20
	//initialVectors[7].x = -4.0f; initialVectors[2].y = 3.0f; // sumOfComponents = -1	sumOfAbsComponents = 7	sumOfSquaredComponents = 25
	//initialVectors[8].x  = 5.0f; initialVectors[3].y = 4.0f; // sumOfComponents = 9		sumOfAbsComponents = 9	sumOfSquaredComponents = 41
	//initialVectors[9].x = -5.0f; initialVectors[4].y = 5.0f; // sumOfComponents = 0		sumOfAbsComponents = 10	sumOfSquaredComponents = 50
	for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
	{
		initialVectors[i].x = g_randomGenerator.NextFloat(LOWER_BOUND_X, HIGHER_BOUND_X);
		initialVectors[i].y = g_randomGenerator.NextFloat(LOWER_BOUND_Y, HIGHER_BOUND_Y);
		DEBUG_LOG_MATH_TEST("initialVectors[", i, "] = ", initialVectors[i]);
	}

	constexpr int NUMBER_OF_SORTING_METHODS = 1; /* the number of sorting methods in the Math library we want to check (10 means we want to check all of them) */
	const std::array<int, NUMBER_OF_SORTING_METHODS> chosenSortingMethodIndices = { 5 }; // its length must match the value of NUMBER_OF_SORTING_METHODS variable
	const std::array<Sorting::SortingAlgorithms::SortingAlgorithm, Sorting::SortingAlgorithms::COUNT> sortingMethods = { Sorting::SortingAlgorithms::BUBBLE_SORT,
		Sorting::SortingAlgorithms::INSERTION_SORT, Sorting::SortingAlgorithms::SELECTION_SORT, Sorting::SortingAlgorithms::MERGE_SORT,
		Sorting::SortingAlgorithms::HEAP_SORT, Sorting::SortingAlgorithms::QUICK_SORT, Sorting::SortingAlgorithms::SHELL_SORT,
		Sorting::SortingAlgorithms::COMB_SORT, Sorting::SortingAlgorithms::COUNTING_SORT, Sorting::SortingAlgorithms::RADIX_SORT,
		Sorting::SortingAlgorithms::BUCKET_SORT };
	const std::array<std::string, Sorting::SortingAlgorithms::COUNT> sortingMethodsStr = { "Bubble sort", "Insertion sort", "Selection sort", "Merge sort",
		"Heap sort", "Quick sort", "Shell sort", "Comb sort", "Counting sort", "Radix sort", "Bucket sort" };

	constexpr int NUMBER_OF_TIME_TESTS_ITERATIONS = 20;
	/**
	 * TODO: Instead of measuring the time needed to perform NUMBER_OF_TIME_TESTS_ITERATION sorts it should be more efficient to count the number of successful sorts within given period of time.
	 * If this time limit is passed then sorting is stopped no matter if it's finished or not.
	 */
	for (int sortingMethodIndex = 0; sortingMethodIndex < NUMBER_OF_SORTING_METHODS; ++sortingMethodIndex)
	{
		std::unique_ptr<Sorting::ISort> sorter = Sorting::ISort::GetSortingObject(sortingMethods[chosenSortingMethodIndices[sortingMethodIndex]]);
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
			sorter->Sort(vectors.data(), NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by X component", timer, Timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			bool firstCriterionEqual = AlmostEqual(vectors[i + 1].x, vectors[i].x);
			if ((!firstCriterionEqual && vectors[i + 1].x < vectors[i].x) ||
				(firstCriterionEqual && (vectors[i + 1].y > vectors[i].y)))
			{
				ERROR_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " in ASCENDING order by X component failed. Vectors[", i + 1, "] (",
					vectors[i + 1], ") (sorting key value = ", vectors[i + 1].x, ") should precede Vectors[", i, "] (", vectors[i],
					") (sorting key value = ", vectors[i].x, ")");
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " results (ASCENDING order by X component):\n");
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG_MATH_TEST("Vectors[", i, "] = ", vectors[i]);
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
			sorter->Sort(vectors.data(), NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by X component", timer, Timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].x > vectors[i].x) || ((AlmostEqual(vectors[i + 1].x, vectors[i].x)) && (vectors[i + 1].y > vectors[i].y)))
			{
				ERROR_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " in DESCENDING order by X component failed. Vectors[", i + 1, "] (",
					vectors[i + 1], ") (sorting key value = ", vectors[i + 1].x, ") should precede Vectors[", i, "] (", vectors[i], ") (sorting key value = ", vectors[i].x, ")");
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " results (DESCENDING order by X component):\n");
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG_MATH_TEST("Vectors[", i, "] = ", vectors[i]);
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
			sorter->Sort(vectors.data(), NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by Y component", timer, Timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].y < vectors[i].y) || ((AlmostEqual(vectors[i + 1].y, vectors[i].y)) && (vectors[i + 1].x > vectors[i].x)))
			{
				ERROR_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " in ASCENDING order by Y component failed. Vectors[", i + 1, "] (", vectors[i + 1],
					") (sorting key value = ", vectors[i + 1].y, ") should precede Vectors[", i, "] (", vectors[i], ") (sorting key value = ", vectors[i].y, ")");
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " results (ASCENDING order by Y component):\n");
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG_MATH_TEST("Vectors[", i, "] = ", vectors[i]);
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
			sorter->Sort(vectors.data(), NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by Y component", timer, Timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			if ((vectors[i + 1].y > vectors[i].y) || ((AlmostEqual(vectors[i + 1].y, vectors[i].y)) && (vectors[i + 1].x > vectors[i].x)))
			{
				ERROR_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " in DESCENDING order by Y component failed. Vectors[", i + 1, "] (", vectors[i + 1],
					") (sorting key value = ", vectors[i + 1].y, ") should precede Vectors[", i, "] (", vectors[i], ") (sorting key value = ", vectors[i].y, ")");
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " results (DESCENDING order by Y component):\n");
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG_MATH_TEST("Vectors[", i, "] = ", vectors[i]);
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
			sorter->Sort(vectors.data(), NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of components", timer, Timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfComponents1 = vectors[i].SumOfComponents();
			Math::Real sumOfComponents2 = vectors[i + 1].SumOfComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfComponents2, sumOfComponents1);
			if ((!firstCriterionEqual && sumOfComponents2 < sumOfComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].x > vectors[i].x)))
			{
				ERROR_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " in ASCENDING order by sum of components failed. Vectors[", i + 1, "] (", vectors[i + 1],
					") (sorting key value = ", sumOfComponents2, ") should precede Vectors[", i, "] (", vectors[i], ") (sorting key value = ", sumOfComponents1, ")");
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " results (ASCENDING order by sum of components):\n");
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG_MATH_TEST("Vectors[", i, "] = ", vectors[i]);
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
			sorter->Sort(vectors.data(), NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of components", timer, Timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfComponents1 = vectors[i].SumOfComponents();
			Math::Real sumOfComponents2 = vectors[i + 1].SumOfComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfComponents2, sumOfComponents1);
			if ((!firstCriterionEqual && sumOfComponents2 > sumOfComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].x > vectors[i].x)))
			{
				ERROR_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " in DESCENDING order by sum of components failed. Vectors[", i + 1, "] (", vectors[i + 1],
					") (sorting key value = ", sumOfComponents2, ") should precede Vectors[", i, "] (", vectors[i], ") (sorting key value = ", sumOfComponents1, ")");
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " results (DESCENDING order by sum of components):\n");
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG_MATH_TEST("Vectors[", i, "] = ", vectors[i]);
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
			sorter->Sort(vectors.data(), NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of absolute components", timer, Timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfAbsoluteComponents1 = vectors[i].SumOfAbsoluteComponents();
			Math::Real sumOfAbsoluteComponents2 = vectors[i + 1].SumOfAbsoluteComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfAbsoluteComponents2, sumOfAbsoluteComponents1);
			if ((!firstCriterionEqual && sumOfAbsoluteComponents2 < sumOfAbsoluteComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].x > vectors[i].x)))
			{
				ERROR_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " in ASCENDING order by sum of absolute components failed. Vectors[", i + 1, "] (", vectors[i + 1],
					") (sorting key value = ", sumOfAbsoluteComponents2, ") should precede Vectors[", i, "] (", vectors[i], ") (sorting key value = ", sumOfAbsoluteComponents1, ")");
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " results (ASCENDING order by sum of absolute components):\n");
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG_MATH_TEST("Vectors[", i, "] = ", vectors[i]);
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
			sorter->Sort(vectors.data(), NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of absolute components", timer, Timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real sumOfAbsoluteComponents1 = vectors[i].SumOfAbsoluteComponents();
			Math::Real sumOfAbsoluteComponents2 = vectors[i + 1].SumOfAbsoluteComponents();
			bool firstCriterionEqual = AlmostEqual(sumOfAbsoluteComponents2, sumOfAbsoluteComponents1);
			if ((!firstCriterionEqual && sumOfAbsoluteComponents2 > sumOfAbsoluteComponents1) ||
				(firstCriterionEqual && (vectors[i + 1].x > vectors[i].x)))
			{
				ERROR_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " in DESCENDING order by sum of absolute components failed. Vectors[", i + 1, "] (", vectors[i + 1],
					") (sorting key value = ", sumOfAbsoluteComponents2, ") should precede Vectors[", i, "] (", vectors[i], ") (sorting key value = ", sumOfAbsoluteComponents1, ")");
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " results (DESCENDING order by sum of absolute components):\n");
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG_MATH_TEST("Vectors[", i, "] = ", vectors[i]);
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
			sorter->Sort(vectors.data(), NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of squared components", timer, Timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real lengthSquared1 = vectors[i].LengthSquared();
			Math::Real lengthSquared2 = vectors[i + 1].LengthSquared();
			bool firstCriterionEqual = AlmostEqual(lengthSquared2, lengthSquared1);
			if ((!firstCriterionEqual && lengthSquared2 < lengthSquared1) ||
				(firstCriterionEqual && (vectors[i + 1].x > vectors[i].x)))
			{
				ERROR_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " in ASCENDING order by sum of squared components failed. Vectors[", i + 1, "] (", vectors[i + 1],
					") (sorting key value = ", lengthSquared2, ") should precede Vectors[", i, "] (", vectors[i], ") (sorting key value = ", lengthSquared1, ")");
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " results (ASCENDING order by sum of squared components):\n");
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG_MATH_TEST("Vectors[", i, "] = ", vectors[i]);
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
			sorter->Sort(vectors.data(), NUMBER_OF_VECTORS, sortingParameters);
		}
		timer.Stop();
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " by sum of squared components", timer, Timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			Math::Real lengthSquared1 = vectors[i].LengthSquared();
			Math::Real lengthSquared2 = vectors[i + 1].LengthSquared();
			bool firstCriterionEqual = AlmostEqual(lengthSquared2, lengthSquared1);
			if ((!firstCriterionEqual && lengthSquared2 > lengthSquared1) ||
				(firstCriterionEqual && (vectors[i + 1].x > vectors[i].x)))
			{
				ERROR_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " in DESCENDING order by sum of squared components failed. Vectors[", i + 1, "] (", vectors[i + 1],
					") (sorting key value = ", lengthSquared2, ") should precede Vectors[", i, "] (", vectors[i], ") (sorting key value = ", lengthSquared1, ")");
				sortingTestCasePassed = false;
				break;
			}
		}
		DEBUG_LOG_MATH_TEST(sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]], " results (DESCENDING order by sum of squared components):\n");
		for (int i = 0; i < NUMBER_OF_VECTORS; ++i)
		{
			DEBUG_LOG_MATH_TEST("Vectors[", i, "] = ", vectors[i]);
		}
		TestReport(sortingTestCasePassed, sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " in DESCENDING order by sum of squared components failed.");
		/* ==================== SORTING TEST #10- sorting Vector2D objects by sum of squared components descending end ==================== */
	}

	NOTICE_LOG_MATH_TEST("Sorting test finished");
}

void KDTreeTest()
{
	if (!kdTreeTestEnabled)
	{
		return;
	}

	NOTICE_LOG_MATH_TEST("K-d tree test started");
	constexpr int NUMBER_OF_POSITIONS = 6;
	constexpr int NUMBER_OF_SEARCH_POSITIONS = 10;
	constexpr int NUMBER_OF_INTERPOLATED_NEAREST_POINTS = 1; // number of nearest points which will be used for calculating the final result
	constexpr Real LOWER_BOUND_X = -20.0f; constexpr Real HIGHER_BOUND_X = 20.0f;
	constexpr Real LOWER_BOUND_Y = -20.0f; constexpr Real HIGHER_BOUND_Y = 20.0f;
	constexpr Real LOWER_BOUND_Z = -20.0f; constexpr Real HIGHER_BOUND_Z = 20.0f;
	std::array<Vector3D, NUMBER_OF_POSITIONS> positions;
	positions[0].x = 2.0f; positions[0].y = 3.0f; positions[0].z = 2.0f;
	positions[1].x = 5.0f; positions[1].y = 4.0f; positions[1].z = 4.0f;
	positions[2].x = 9.0f; positions[2].y = 6.0f; positions[2].z = -2.0f;
	positions[3].x = 4.0f; positions[3].y = 7.0f; positions[3].z = 1.0f;
	positions[4].x = 8.0f; positions[4].y = 1.0f; positions[4].z = 12.0f;
	positions[5].x = 7.0f; positions[5].y = 2.0f; positions[5].z = 6.0f;
	//for (int i = 0; i < NUMBER_OF_POSITIONS; ++i)
	//{
	//	positions[i].SetX(g_randomGenerator.NextFloat(LOWER_BOUND_X, HIGHER_BOUND_X));
	//	positions[i].SetY(g_randomGenerator.NextFloat(LOWER_BOUND_Y, HIGHER_BOUND_Y));
	//	positions[i].SetZ(g_randomGenerator.NextFloat(LOWER_BOUND_Z, HIGHER_BOUND_Z));
	//	DEBUG_LOG_MATH_TEST("positions[", i, "] = ", positions[i].ToString());
	//}

	std::unique_ptr<KDTree> kdTree = std::make_unique<KDTree>(positions.data(), NUMBER_OF_POSITIONS, NUMBER_OF_INTERPOLATED_NEAREST_POINTS);

	//DEBUG_LOG_MATH_TEST("K-d tree structure:\n", kdTree->ToString());

	std::array<Math::Vector2D, NUMBER_OF_SEARCH_POSITIONS> searchPositions;
	searchPositions[0].x = 2.0f; searchPositions[0].y = 3.0f;
	searchPositions[1].x = 5.0f; searchPositions[1].y = 4.0f;
	searchPositions[2].x = 9.0f; searchPositions[2].y = 6.0f;
	searchPositions[3].x = 4.0f; searchPositions[3].y = 7.0f;
	searchPositions[4].x = 8.0f; searchPositions[4].y = 1.0f;
	searchPositions[5].x = 7.0f; searchPositions[5].y = 2.0f;
	searchPositions[6].x = 6.0f; searchPositions[6].y = 2.0f;
	searchPositions[7].x = 7.0f; searchPositions[7].y = 3.0f;
	searchPositions[8].x = 8.0f; searchPositions[8].y = 4.0f;
	searchPositions[9].x = 9.0f; searchPositions[9].y = 5.0f;
	for (int i = 0; i < NUMBER_OF_SEARCH_POSITIONS; ++i)
	{
		Real height = kdTree->SearchNearestValue(searchPositions[i]);
		//INFO_LOG_MATH_TEST("The nearest point for search point (", searchPositions[i].ToString(), ") is (", minDistancePositions[0].ToString(),
		//	"). The minimum distance equals ", minDistances[0], " and returned height is ", height);
		INFO_LOG_MATH_TEST("The final result for position ", searchPositions[i], " in k-d tree search is ", height);
	}
}

void StatsTest()
{
	if (!statsTestEnabled)
	{
		return;
	}

	const int STATS_TEST_1_METHOD_1_INVOCATIONS_COUNT = 5;
	const int STATS_TEST_1_METHOD_2_INVOCATIONS_COUNT = 15;
	const int STATS_TEST_1_METHOD_3_INVOCATIONS_COUNT = 4;
	const int STATS_TEST_2_METHOD_1_INVOCATIONS_COUNT = 2;
	const int STATS_TEST_2_METHOD_2_INVOCATIONS_COUNT = 1000;
	const int STATS_TEST_2_METHOD_3_INVOCATIONS_COUNT = 8;

	MathTest::StatsTest1 statsTest1;
	for (int i = 0; i < STATS_TEST_1_METHOD_1_INVOCATIONS_COUNT; ++i)
	{
		statsTest1.Method1();
	}
	//DEBUG_LOG_MATH_TEST("StatsTest1Method1 tested");
	for (int i = 0; i < STATS_TEST_1_METHOD_2_INVOCATIONS_COUNT; ++i)
	{
		statsTest1.Method2();
	}
	//CRITICAL_LOG_MATH_TEST("StatsTest1Method2 tested");
	for (int i = 0; i < STATS_TEST_1_METHOD_3_INVOCATIONS_COUNT; ++i)
	{
		statsTest1.Method3();
	}
	//CRITICAL_LOG_MATH_TEST("StatsTest1Method3 tested");

	MathTest::StatsTest2 statsTest2;
	for (int i = 0; i < STATS_TEST_2_METHOD_1_INVOCATIONS_COUNT; ++i)
	{
		statsTest2.Method1();
	}
	//CRITICAL_LOG_MATH_TEST("StatsTest2Method1 tested");
	for (int i = 0; i < STATS_TEST_2_METHOD_2_INVOCATIONS_COUNT; ++i)
	{
		statsTest2.Method2();
	}
	//CRITICAL_LOG_MATH_TEST("StatsTest2Method2 tested");
	for (int i = 0; i < STATS_TEST_2_METHOD_3_INVOCATIONS_COUNT; ++i)
	{
		statsTest2.Method3();
	}
	//CRITICAL_LOG_MATH_TEST("StatsTest2Method2 tested");

	//double anotherTotalElapsedTime = CalculateElapsedTime(outerBegin, outerEnd, SECONDS);
	//std::cout << "Outer end = " << outerEnd << "[ms]. OuterBegin = " << outerBegin << "[ms]." << std::endl;
	//std::cout << "Total elapsed time = " << totalElapsedTime << "[s]. Another total elapsed time = " << anotherTotalElapsedTime << "[s]." << std::endl;
}

void HeightsGeneratorTests()
{
	if (!heightsGeneratorTestsEnabled)
	{
		return;
	}
	Timing::Timer timer;
	timer.Start();

	Math::Real amplitude = 70.0f;
	int octaves = 3;
	Math::Real roughness = 0.3f;
	Math::HeightsGenerator heightsGenerator(0, 0, 128, amplitude, octaves, roughness);

	constexpr int WIDTH = 8;
	constexpr int HEIGHT = 8;
	std::array<Math::Real, WIDTH * HEIGHT> heights;
	for (int z = 0; z < HEIGHT; ++z)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			heights[z * WIDTH + x] = heightsGenerator.GenerateHeight(x, z);
			INFO_LOG_MATH_TEST("heights[", z, "][", x, "] = ", heights[z * WIDTH + x]);
		}
	}

	timer.Stop();

	//double anotherTotalElapsedTime = CalculateElapsedTime(outerBegin, outerEnd, SECONDS);
	//std::cout << "Outer end = " << outerEnd << "[ms]. OuterBegin = " << outerBegin << "[ms]." << std::endl;
	//std::cout << "Total elapsed time = " << totalElapsedTime << "[s]. Another total elapsed time = " << anotherTotalElapsedTime << "[s]." << std::endl;
}

void OtherTests()
{
	if (!otherTestsEnabled)
	{
		return;
	}

	constexpr unsigned int VALUES_COUNT = 5;
	std::array<Math::Real, VALUES_COUNT> values = { -2.15f, -1.05f, 0.0f, 1.1f, 2.2f };
	std::array<Math::Real, VALUES_COUNT> floorValues = { -3.0f, -2.0f, 0.0f, 1.0f, 2.0f };
	std::array<Math::Real, VALUES_COUNT> zeroToOneClampValues = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f };
	std::array<Math::Real, VALUES_COUNT> minusOneAndHalfToOneClampValues = { -1.5f, -1.05f, 0.0f, 1.0f, 1.0f };
	std::array<int, VALUES_COUNT> intValues = { -4, 0, 1, 3, 13 };
	std::array<int, VALUES_COUNT> roundUpPow2Values = { 0, 0, 1, 4, 16 };
	for (unsigned int i = 0; i < VALUES_COUNT; ++i)
	{
		std::stringstream ss("");
		ss << "Absolute value of " << values[i] << " must be greater or equal to 0.0, but is equal to " << Absolute(values[i]);
		TestReport(Absolute(values[i]) > REAL_ZERO || AlmostEqual(Absolute(values[i]), REAL_ZERO), ss.str());
		ss.clear();
		ss << "Floor value of " << values[i] << " should be equal to " << floorValues[i];
		TestReport(AlmostEqual(Floor(values[i]), floorValues[i]), ss.str());
		ss.clear();
		ss << "Value " << values[i] << " clamped to range [0.0; 1.0] equals " << zeroToOneClampValues[i];
		TestReport(AlmostEqual(Clamp(values[i], 0.0f, 1.0f), zeroToOneClampValues[i]), ss.str());
		ss.clear();
		ss << "Value " << values[i] << " clamped to range [-1.5; 1.0] equals " << minusOneAndHalfToOneClampValues[i];
		TestReport(AlmostEqual(Clamp(values[i], -1.5f, 1.0f), minusOneAndHalfToOneClampValues[i]), ss.str());

		ss.clear();
		ss << "RoundUpPow2 for the value " << intValues[i] << " should be equal to " << roundUpPow2Values[i];
		TestReport(AlmostEqual(RoundUpPow2(intValues[i]), roundUpPow2Values[i]), ss.str());
	}

	constexpr unsigned int ANGLES_COUNT = 5;
	std::array<Math::Real, ANGLES_COUNT> anglesInDegrees = { -30.0f, 0.0f, 30.0f, 60.0f, 90.0f };
	std::array<Math::Real, ANGLES_COUNT> anglesInRadians = { -Math::PI / 6.0f, 0.0f, Math::PI / 6.0f, Math::PI / 3.0f, Math::PI / 2.0f };
	for (unsigned int i = 0; i < ANGLES_COUNT; ++i)
	{
		std::stringstream ss("");
		ss << "Angle " << anglesInDegrees[i] << " [deg] should be equal to angle " << anglesInRadians[i] << " [rad].";
		TestReport(Math::AlmostEqual(anglesInDegrees[i], Math::ToDeg(anglesInRadians[i])), ss.str());
		TestReport(Math::AlmostEqual(Math::ToRad(anglesInDegrees[i]), anglesInRadians[i]), ss.str());
	}
}

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	std::unique_ptr<ICommandLineMapper> commandLineMapper = ICommandLineMapper::CreateCommandLineMapper(argc, argv);
	//if (commandLineMapper.IsPresent("-help"))
	//{
	//	PrintHelp();
	//	system("pause");
	//	return 0;
	//}

	//constexpr int ACCEPTABLE_PROGRAM_ARGUMENTS_COUNT = 4;
	//const std::array<std::string, ACCEPTABLE_PROGRAM_ARGUMENTS_COUNT> acceptableProgramArguments = { "-help", "-logUtility", "-logMath", "-log" };

	Logging::ILogger::GetLogger("Utility").Fill(commandLineMapper->Get("-logUtility", "Info"), Logging::INFO);
	Logging::ILogger::GetLogger("Math").Fill(commandLineMapper->Get("-logMath", "Info"), Logging::INFO);
	Logging::ILogger::GetLogger(MODULE_NAME).Fill(commandLineMapper->Get("-log", ""), Logging::INFO);
	

	STATS_STORAGE.StartTimer();

	//AngleTest();
	//VectorTest();
	//QuaternionTest();
	MatrixTest();
	NewMatrixTest();
	TransformTest();
	SortTest();
	//SortTestTime();

	KDTreeTest();

	//StatsTest();

	//HeightsGeneratorTests();
	//OtherTests();

	STATS_STORAGE.StopTimer();
	STATS_STORAGE.PrintReport();

	Logging::ILogger::GetLogger(MODULE_NAME).ResetConsoleColor();
	std::cout << "Bye!" << std::endl;

	return 0;
}
