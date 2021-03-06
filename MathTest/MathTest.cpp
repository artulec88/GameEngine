#include "Math/Angle.h"
#include "Math/FloatingPoint.h"
#include "Math/Matrix.h"
#include "Math/NewMatrix.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"
#include "Math/ISort.h"
#include "Math/KDTree.h"
#include "Math/HeightsGenerator.h"
#include "Math/RandomGeneratorFactory.h"
#include "Math/Surface.h"

#include "Utility/ICommandLineMapper.h"
#include "Utility/ILogger.h"
#include "Utility/Time.h"

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
#include <filesystem>
//#include <iostream>
//#include <fstream>

using namespace math;
using namespace utility;
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
bool interpolationTestsEnabled = true;
bool surfaceTestsEnabled = true;
bool otherTestsEnabled = true;

const math::random::RandomGenerator& g_randomGenerator = math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE);
const std::string MODULE_NAME = "MathTest";

void ReportError(const std::string& reportStr)
{
	ERROR_LOG_MATH_TEST("Test #", testNumber, " FAILED. ", reportStr);
}

void TestReport(bool statusCode /* false if error */, const std::string& reportErrorStr)
{
	testNumber++;
	if (!statusCode)
	{
		ReportError(reportErrorStr);
	}
	else
	{
		INFO_LOG_MATH_TEST("Test #", testNumber, " passed");
	}
}

void TimeReport(const std::string& reportStr, timing::Timer& timer, timing::TimeUnit timeUnit, const unsigned int NUMBER_OF_ITERATIONS = 1)
{
	//CHECK_CONDITION_EXIT_ALWAYS_MATH_TEST(!timer.IsRunning(), Logging::ERR, "Timer is still running");
	double elapsedTime = static_cast<double>(timer.GetDuration(timeUnit));
	elapsedTime /= NUMBER_OF_ITERATIONS;
	//timeSpan.AdjustUnitToValue();
	NOTICE_LOG_MATH_TEST(reportStr, ":\t", elapsedTime, " ", timing::DateTime::ConvertTimeUnitToString(timeUnit));
}

void AngleTest()
{
	if (!angleTestEnabled)
	{
		return;
	}

	math_test::AngleTestGroup angleTests("Angle tests", 100000);
	//MathTest::AngleTestGroup* angleCompareTests = new MathTest::AngleTestGroup();
	const Angle angle1(90.0);
	const Angle angle2(90.0, units::DEGREE);
	const Angle angle3(PI / 2.0f, units::RADIAN);
	const Angle angle4(PI / 2.0f, units::DEGREE);
	const Angle angle5(45.0f);
	const Angle angle6(PI / 4.0f, units::RADIAN);
	//angleCompareTests->AddTest(new MathTest::AngleTestCompare(angle1, angle2, true, false, false));
	//angleCompareTests->AddTest(new MathTest::AngleTestCompare(angle1, angle3, true, false, false));
	//angleCompareTests->AddTest(new MathTest::AngleTestCompare(angle2, angle3, true, false, false));
	//angleCompareTests->AddTest(new MathTest::AngleTestCompare(angle1, angle4, false, false, true));
	//angleCompareTests->AddTest(new MathTest::AngleTestCompare(angle4, angle1, false, true, false));
	//angleCompareTests->AddTest(new MathTest::AngleTestCompare(angle2, angle4, false, false, true));
	//angleCompareTests->AddTest(new MathTest::AngleTestCompare(angle4, angle2, false, true, false));
	//angleCompareTests->AddTest(new MathTest::AngleTestCompare(angle3, angle4, false, false, true));
	//angleCompareTests->AddTest(new MathTest::AngleTestCompare(angle4, angle3, false, true, false));
	//angleCompareTests->AddTest(new MathTest::AngleTestCompare(angle5, angle6, true, false, false));
	//angleTests.AddTestGroup(angleCompareTests);

	math_test::AngleTestGroup* angleTrigonometryTests = new math_test::AngleTestGroup("Trigonometry tests", 10000);
	math_test::AngleTestGroup* angleSinTests = new math_test::AngleTestGroup("Sinus tests", 10000000);
	math_test::AngleTestGroup* angleCosTests = new math_test::AngleTestGroup("Cosinus tests", 10000000);
	math_test::AngleTestGroup* angleTanTests = new math_test::AngleTestGroup("Tangent tests", 10000000);
	angleSinTests->AddTest(new math_test::AngleTestSin(Angle(90.0f), REAL_ONE));
	angleCosTests->AddTest(new math_test::AngleTestCos(Angle(90.0f), REAL_ZERO));
	angleTanTests->AddTest(new math_test::AngleTestTan(Angle(90.0f), REAL_ZERO));
	angleSinTests->AddTest(new math_test::AngleTestSin(Angle(90.0f, units::DEGREE), REAL_ONE));
	angleCosTests->AddTest(new math_test::AngleTestCos(Angle(90.0f, units::DEGREE), REAL_ZERO));
	angleTanTests->AddTest(new math_test::AngleTestTan(Angle(90.0f, units::DEGREE), REAL_ZERO));
	angleSinTests->AddTest(new math_test::AngleTestSin(Angle(PI / 2.0f, units::RADIAN), REAL_ONE));
	angleCosTests->AddTest(new math_test::AngleTestCos(Angle(PI / 2.0f, units::RADIAN), REAL_ZERO));
	angleTanTests->AddTest(new math_test::AngleTestTan(Angle(PI / 2.0f, units::RADIAN), REAL_ZERO));
	angleSinTests->AddTest(new math_test::AngleTestSin(Angle(PI / 2.0f, units::DEGREE), 0.027412f));
	angleCosTests->AddTest(new math_test::AngleTestCos(Angle(PI / 2.0f, units::DEGREE), 0.999624f));
	angleTanTests->AddTest(new math_test::AngleTestTan(Angle(PI / 2.0f, units::DEGREE), 0.027422f));
	angleSinTests->AddTest(new math_test::AngleTestSin(Angle(45.0f), static_cast<math::Real>(sqrt(2.0) / 2.0)));
	angleCosTests->AddTest(new math_test::AngleTestCos(Angle(45.0f), static_cast<math::Real>(sqrt(2.0) / 2.0)));
	angleTanTests->AddTest(new math_test::AngleTestTan(Angle(45.0f), REAL_ONE));
	angleSinTests->AddTest(new math_test::AngleTestSin(Angle(PI / 4.0f, units::RADIAN), static_cast<math::Real>(sqrt(2.0) / 2.0)));
	angleCosTests->AddTest(new math_test::AngleTestCos(Angle(PI / 4.0f, units::RADIAN), static_cast<math::Real>(sqrt(2.0) / 2.0)));
	angleTanTests->AddTest(new math_test::AngleTestTan(Angle(PI / 4.0f, units::RADIAN), REAL_ONE));
	angleTrigonometryTests->AddTestGroup(angleSinTests);
	angleTrigonometryTests->AddTestGroup(angleCosTests);
	angleTrigonometryTests->AddTestGroup(angleTanTests);
	angleTests.AddTestGroup(angleTrigonometryTests);

	//MathTest::AngleTestGroup* angleOperatorsTests = new MathTest::AngleTestGroup();
	//angleOperatorsTests->AddTest(new MathTest::AngleTestSum(angle1, angle2, Angle(180.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestSum(angle1, angle3, Angle(180.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestSum(angle1, angle4, Angle(91.5708f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestSum(angle1, angle5, Angle(135.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestSum(angle1, angle6, Angle(135.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestSum(Angle(30.0f), Angle(60.0f), Angle(90.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestSum(Angle(40.0f), Angle(20.0f), Angle(60.0f)));

	//angleOperatorsTests->AddTest(new MathTest::AngleTestDiff(angle1, angle2, Angle(0.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDiff(angle1, angle3, Angle(0.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDiff(angle1, angle4, Angle(88.4292037f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDiff(angle1, angle5, Angle(45.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDiff(angle1, angle6, Angle(45.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDiff(Angle(30.0f), Angle(60.0f), Angle(-30.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDiff(Angle(40.0f), Angle(20.0f), Angle(20.0f)));

	//angleOperatorsTests->AddTest(new MathTest::AngleTestMultiplyByValue(angle1, 0.3f, Angle(27.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestMultiplyByValue(angle1, 2.0f, Angle(180.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestMultiplyByValue(angle1, 5.0f, Angle(450.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestMultiplyByValue(angle1, 0.5f, Angle(45.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestMultiplyByValue(angle1, 0.2f, Angle(18.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestMultiplyByValue(Angle(-30.0f), 0.5f, Angle(-15.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestMultiplyByValue(Angle(40.0f), 0.75f, Angle(30.0f)));

	//angleOperatorsTests->AddTest(new MathTest::AngleTestDivideByValue(angle1, 0.3f, Angle(300.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDivideByValue(angle1, 2.0f, Angle(45.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDivideByValue(angle1, 5.0f, Angle(18.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDivideByValue(angle1, 0.5f, Angle(180.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDivideByValue(angle1, 0.2f, Angle(450.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDivideByValue(Angle(-30.0f), 0.5f, Angle(-60.0f)));
	//angleOperatorsTests->AddTest(new MathTest::AngleTestDivideByValue(Angle(40.0f), 0.8f, Angle(50.0f)));

	//angleTests.AddTestGroup(angleOperatorsTests);

	NOTICE_LOG_MATH_TEST("Angle tests started");
	angleTests.StartTests();
	NOTICE_LOG_MATH_TEST("Angle tests finished");
	NOTICE_LOG_MATH_TEST("Angle time tests started");
	angleTests.StartTimeTests();
	NOTICE_LOG_MATH_TEST("Angle time tests finished");
}

void VectorTest()
{
	if (!vectorTestEnabled)
	{
		return;
	}
	math_test::VectorTestGroup vectorTests("Vector tests", 1000);
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector2D>(ZERO_VECTOR_2D, ZERO_VECTOR_2D, true));
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector2D>(ZERO_VECTOR_2D, math::Vector2D(0.0f, 1.0f), false));
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector2D>(ZERO_VECTOR_2D, math::Vector2D(1.0f, 0.0f), false));
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector2D>(ZERO_VECTOR_2D, math::Vector2D(1.0f, 1.0f), false));
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector3D>(math::Vector3D(0.0f, 0.0f, 0.0f), math::Vector3D(0.0f, 0.0f, 0.0f), true));
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector3D>(math::Vector3D(0.0f, 0.0f, 0.0f), math::Vector3D(0.0f, 1.0f, 0.0f), false));
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector3D>(math::Vector3D(0.0f, 0.0f, 0.0f), math::Vector3D(1.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector3D>(math::Vector3D(0.0f, 0.0f, 0.0f), math::Vector3D(1.0f, 1.0f, 0.0f), false));
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector4D>(math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), true));
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector4D>(math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), math::Vector4D(0.0f, 1.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector4D>(math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), math::Vector4D(1.0f, 0.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new math_test::VectorTestCompare<math::Vector4D>(math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), math::Vector4D(1.0f, 1.0f, 0.0f, 0.0f), false));
	vectorTests.AddTest(new math_test::VectorTestLength<math::Vector2D>(ZERO_VECTOR_2D, 0.0f));
	vectorTests.AddTest(new math_test::VectorTestLength<math::Vector2D>(math::Vector2D(1.0f, 0.0f), 1.0f));
	vectorTests.AddTest(new math_test::VectorTestLength<math::Vector2D>(math::Vector2D(12.0f, -4.0f), 4.0f * sqrtf(10.0f)));
	vectorTests.AddTest(new math_test::VectorTestLength<math::Vector3D>(math::Vector3D(0.0f, 0.0f, 0.0f), 0.0f));
	vectorTests.AddTest(new math_test::VectorTestLength<math::Vector3D>(math::Vector3D(1.0f, 0.0f, 0.0f), 1.0f));
	vectorTests.AddTest(new math_test::VectorTestLength<math::Vector3D>(math::Vector3D(3.0f, 4.0f, 12.0f), 13.0f));
	vectorTests.AddTest(new math_test::VectorTestLength<math::Vector4D>(math::Vector4D(0.0f, 0.0f, 0.0f, 0.0f), 0.0f));
	vectorTests.AddTest(new math_test::VectorTestLength<math::Vector4D>(math::Vector4D(1.0f, 0.0f, 0.0f, 0.0f), 1.0f));
	vectorTests.AddTest(new math_test::VectorTestLength<math::Vector4D>(math::Vector4D(1.0f, 2.0f, 5.0f, -3.0f), sqrtf(39.0f)));
	vectorTests.AddTest(new math_test::VectorTestAddOperator<math::Vector2D>(math::Vector2D(0.0f, 1.0f), math::Vector2D(1.0f, 0.0f), math::Vector2D(1.0f, 1.0f)));
	vectorTests.AddTest(new math_test::VectorTestAddOperator<math::Vector2D>(ZERO_VECTOR_2D, math::Vector2D(1.0f, 0.0f), math::Vector2D(1.0f, 0.0f)));
	vectorTests.AddTest(new math_test::VectorTestAddOperator<math::Vector2D>(math::Vector2D(1.0f, 1.0f), math::Vector2D(1.0f, 0.0f), math::Vector2D(2.0f, 1.0f)));
	vectorTests.AddTest(new math_test::VectorTestAddOperator<math::Vector3D>(math::Vector3D(0.0f, 1.0f, 0.0f), math::Vector3D(1.0f, 0.0f, 0.0f), math::Vector3D(1.0f, 1.0f, 0.0f)));
	vectorTests.AddTest(new math_test::VectorTestAddOperator<math::Vector3D>(math::Vector3D(0.0f, 0.0f, 0.0f), math::Vector3D(1.0f, 0.0f, -1.0f), math::Vector3D(1.0f, 0.0f, -1.0f)));
	vectorTests.AddTest(new math_test::VectorTestAddOperator<math::Vector3D>(math::Vector3D(1.0f, 1.0f, -3.0f), math::Vector3D(1.0f, 0.0f, -2.0f), math::Vector3D(2.0f, 1.0f, -5.0f)));
	vectorTests.AddTest(new math_test::VectorTestAddOperator<math::Vector4D>(math::Vector4D(0.0f, 1.0f, 2.0f, 3.0f), math::Vector4D(1.0f, 0.0f, -1.0f, 0.0f), math::Vector4D(1.0f, 1.0f, 1.0f, 3.0f)));
	vectorTests.AddTest(new math_test::VectorTestAddOperator<math::Vector4D>(math::Vector4D(0.0f, 0.0f, -3.0f, 4.0f), math::Vector4D(1.0f, 0.0f, 2.0f, -3.0f), math::Vector4D(1.0f, 0.0f, -1.0f, 1.0f)));
	vectorTests.AddTest(new math_test::VectorTestAddOperator<math::Vector4D>(math::Vector4D(1.0f, 1.0f, 5.0f, 6.0f), math::Vector4D(1.0f, 0.0f, 5.0f, 5.0f), math::Vector4D(2.0f, 1.0f, 10.0f, 11.0f)));
	vectorTests.AddTest(new math_test::VectorTestSubtractOperator<math::Vector2D>(math::Vector2D(0.0f, 1.0f), math::Vector2D(1.0f, 0.0f), math::Vector2D(-1.0f, 1.0f)));
	vectorTests.AddTest(new math_test::VectorTestSubtractOperator<math::Vector2D>(ZERO_VECTOR_2D, math::Vector2D(1.0f, 0.0f), math::Vector2D(-1.0f, 0.0f)));
	vectorTests.AddTest(new math_test::VectorTestSubtractOperator<math::Vector2D>(math::Vector2D(1.0f, 1.0f), math::Vector2D(1.0f, 0.0f), math::Vector2D(0.0f, 1.0f)));
	vectorTests.AddTest(new math_test::VectorTestSubtractOperator<math::Vector3D>(math::Vector3D(0.0f, 1.0f, 2.0f), math::Vector3D(1.0f, 0.0f, 2.0f), math::Vector3D(-1.0f, 1.0f, 0.0f)));
	vectorTests.AddTest(new math_test::VectorTestSubtractOperator<math::Vector3D>(math::Vector3D(0.0f, 0.0f, -2.0f), math::Vector3D(1.0f, 0.0f, -1.0f), math::Vector3D(-1.0f, 0.0f, -1.0f)));
	vectorTests.AddTest(new math_test::VectorTestSubtractOperator<math::Vector3D>(math::Vector3D(1.0f, 1.0f, 3.0f), math::Vector3D(1.0f, 0.0f, 0.0f), math::Vector3D(0.0f, 1.0f, 3.0f)));
	vectorTests.AddTest(new math_test::VectorTestSubtractOperator<math::Vector4D>(math::Vector4D(0.0f, 1.0f, 2.0f, -2.0f), math::Vector4D(1.0f, 0.0f, -2.0f, -3.0f), math::Vector4D(-1.0f, 1.0f, 4.0f, 1.0f)));
	vectorTests.AddTest(new math_test::VectorTestSubtractOperator<math::Vector4D>(math::Vector4D(0.0f, 0.0f, -4.0f, -5.0f), math::Vector4D(1.0f, 0.0f, -5.0f, 4.0f), math::Vector4D(-1.0f, 0.0f, 1.0f, -9.0f)));
	vectorTests.AddTest(new math_test::VectorTestSubtractOperator<math::Vector4D>(math::Vector4D(1.0f, 1.0f, -8.0f, 5.0f), math::Vector4D(1.0f, 0.0f, 6.0f, -7.0f), math::Vector4D(0.0f, 1.0f, -14.0f, 12.0f)));
	vectorTests.AddTest(new math_test::VectorTestMultiplyOperator<math::Vector2D>(math::Vector2D(0.0f, 1.0f), math::Vector2D(1.0f, 0.0f), 0.2f, ZERO_VECTOR_2D, math::Vector2D(0.0f, 0.2f)));
	vectorTests.AddTest(new math_test::VectorTestMultiplyOperator<math::Vector2D>(ZERO_VECTOR_2D, math::Vector2D(1.0f, 0.0f), 0.5f, math::Vector2D(0.0f, 0.0f), math::Vector2D(0.0f, 0.0f)));
	vectorTests.AddTest(new math_test::VectorTestMultiplyOperator<math::Vector2D>(math::Vector2D(1.0f, 1.0f), math::Vector2D(1.0f, 0.0f), 0.75f, math::Vector2D(1.0f, 0.0f), math::Vector2D(0.75f, 0.75f)));
	vectorTests.AddTest(new math_test::VectorTestMultiplyOperator<math::Vector3D>(math::Vector3D(0.0f, 1.0f, 2.0f), math::Vector3D(1.0f, 0.0f, 4.0f), 0.2f, math::Vector3D(0.0f, 0.0f, 8.0f), math::Vector3D(0.0f, 0.2f, 0.4f)));
	vectorTests.AddTest(new math_test::VectorTestMultiplyOperator<math::Vector3D>(math::Vector3D(0.0f, 0.0f, -1.0f), math::Vector3D(1.0f, 0.0f, -5.0f), 0.5f, math::Vector3D(0.0f, 0.0f, 5.0f), math::Vector3D(0.0f, 0.0f, -0.5f)));
	vectorTests.AddTest(new math_test::VectorTestMultiplyOperator<math::Vector3D>(math::Vector3D(1.0f, 1.0f, 5.0f), math::Vector3D(1.0f, 0.0f, 0.0f), 0.75f, math::Vector3D(1.0f, 0.0f, 0.0f), math::Vector3D(0.75f, 0.75f, 3.75f)));
	vectorTests.AddTest(new math_test::VectorTestMultiplyOperator<math::Vector4D>(math::Vector4D(0.0f, 1.0f, 2.0f, 4.0f), math::Vector4D(1.0f, 0.0f, 4.0f, -6.0f), 0.2f, math::Vector4D(0.0f, 0.0f, 8.0f, -24.0f), math::Vector4D(0.0f, 0.2f, 0.4f, 0.8f)));
	vectorTests.AddTest(new math_test::VectorTestMultiplyOperator<math::Vector4D>(math::Vector4D(0.0f, 0.0f, -5.0f, -1.0f), math::Vector4D(1.0f, 0.0f, 0.0f, 1.0f), 0.5f, math::Vector4D(0.0f, 0.0f, 0.0f, -1.0f), math::Vector4D(0.0f, 0.0f, -2.5f, -0.5f)));
	vectorTests.AddTest(new math_test::VectorTestMultiplyOperator<math::Vector4D>(math::Vector4D(1.0f, 1.0f, 0.0f, 2.0f), math::Vector4D(1.0f, 0.0f, -2.0f, 5.0f), 0.75f, math::Vector4D(1.0f, 0.0f, 0.0f, 10.0f), math::Vector4D(0.75f, 0.75f, 0.0f, 1.5f)));
	vectorTests.AddTest(new math_test::VectorTestDivideOperator<math::Vector2D>(math::Vector2D(8.0f, 1.0f), math::Vector2D(1.0f, 2.0f), 0.2f, math::Vector2D(8.0f, 0.5f), math::Vector2D(40.0f, 5.0f)));
	vectorTests.AddTest(new math_test::VectorTestDivideOperator<math::Vector2D>(ZERO_VECTOR_2D, math::Vector2D(1.0f, 5.0f), 0.5f, math::Vector2D(0.0f, 0.0f), math::Vector2D(0.0f, 0.0f)));
	vectorTests.AddTest(new math_test::VectorTestDivideOperator<math::Vector2D>(math::Vector2D(1.0f, 10.0f), math::Vector2D(1.0f, -2.0f), 0.75f, math::Vector2D(1.0f, -5.0f), math::Vector2D(4.0f / 3.0f, 40.0f / 3.0f)));
	vectorTests.AddTest(new math_test::VectorTestDivideOperator<math::Vector3D>(math::Vector3D(0.0f, 1.0f, 2.0f), math::Vector3D(1.0f, 2.0f, 4.0f), 0.2f, math::Vector3D(0.0f, 0.5f, 0.5f), math::Vector3D(0.0f, 5.0f, 10.0f)));
	vectorTests.AddTest(new math_test::VectorTestDivideOperator<math::Vector3D>(math::Vector3D(0.0f, 0.0f, -1.0f), math::Vector3D(1.0f, 10.0f, -5.0f), 0.5f, math::Vector3D(0.0f, 0.0f, 0.2f), math::Vector3D(0.0f, 0.0f, -2.0f)));
	vectorTests.AddTest(new math_test::VectorTestDivideOperator<math::Vector3D>(math::Vector3D(1.0f, 1.0f, 5.0f), math::Vector3D(1.0f, -1.0f, 2.0f), 2.0f, math::Vector3D(1.0f, -1.0f, 2.5f), math::Vector3D(0.5f, 0.5f, 2.5f)));
	vectorTests.AddTest(new math_test::VectorTestDivideOperator<math::Vector4D>(math::Vector4D(0.0f, 1.0f, 2.0f, 4.0f), math::Vector4D(1.0f, -2.0f, 4.0f, -16.0f), -2.0f, math::Vector4D(0.0f, -0.5f, 0.5f, -0.25f), math::Vector4D(0.0f, -0.5f, -1.0f, -2.0f)));
	vectorTests.AddTest(new math_test::VectorTestDivideOperator<math::Vector4D>(math::Vector4D(70.0f, 100.0f, -5.0f, -1.0f), math::Vector4D(1.0f, -5.0f, 5.0f, 1.0f), 0.5f, math::Vector4D(70.0f, -20.0f, -1.0f, -1.0f), math::Vector4D(140.0f, 200.0f, -10.0f, -2.0f)));
	vectorTests.AddTest(new math_test::VectorTestDivideOperator<math::Vector4D>(math::Vector4D(21.0f, 144.0f, -3.0f, 2.0f), math::Vector4D(3.0f, 12.0f, -2.0f, 5.0f), 3.0f, math::Vector4D(7.0f, 12.0f, 1.5f, 0.4f), math::Vector4D(7.0f, 48.0f, -1.0f, 2.0f / 3.0f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector2D>(math::Vector2D(1.0f, 0.0f), math::Vector2D(1.0f, 0.0f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector2D>(math::Vector2D(2.0f, 0.0f), math::Vector2D(1.0f, 0.0f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector2D>(math::Vector2D(1.0f, 1.0f), math::Vector2D(sqrtf(2.0f) / 2.0f, sqrtf(2.0f) / 2.0f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector2D>(math::Vector2D(0.0f, 1.0f), math::Vector2D(0.0f, 1.0f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector2D>(math::Vector2D(5.0f, 5.0f), math::Vector2D(sqrtf(2.0f) / 2.0f, sqrtf(2.0f) / 2.0f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector2D>(math::Vector2D(3.0f, 4.0f), math::Vector2D(0.6f, 0.8f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector3D>(math::Vector3D(1.0f, 0.0f, 2.0f), math::Vector3D(sqrtf(5.0f) / 5.0f, 0.0f, sqrtf(5.0f) / 2.5f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector3D>(math::Vector3D(2.0f, 0.0f, 3.0f), math::Vector3D(sqrtf(13.0f) / 6.5f, 0.0f, 3.0f * sqrtf(13.0f) / 13.0f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector3D>(math::Vector3D(1.0f, -1.0f, 5.0f), math::Vector3D(sqrtf(27.0f) / 27.0f, -sqrtf(27.0f) / 27.0f, 5.0f * sqrtf(27.0f) / 27.0f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector3D>(math::Vector3D(0.0f, 1.0f, 2.0f), math::Vector3D(0.0f, 1.0f / sqrtf(5.0f), 2.0f / sqrtf(5.0f))));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector3D>(math::Vector3D(5.0f, -5.0f, 3.0f), math::Vector3D(5.0f / sqrtf(59.0f), -5.0f / sqrtf(59.0f), 3.0f / sqrtf(59.0f))));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector3D>(math::Vector3D(3.0f, 4.0f, 0.0f), math::Vector3D(0.6f, 0.8f, 0.0f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector4D>(math::Vector4D(1.0f, 0.0f, -3.0f, 0.0f), math::Vector4D(1.0f / sqrtf(10.0f), 0.0f, -3.0f / sqrtf(10.0f), 0.0f)));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector4D>(math::Vector4D(2.0f, 0.0f, 1.0f, 5.0f), math::Vector4D(2.0f / sqrtf(30.0f), 0.0f, 1.0f / sqrtf(30.0f), 5.0f / sqrtf(30.0f))));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector4D>(math::Vector4D(1.0f, 1.0f, -2.0f, 1.0f), math::Vector4D(1.0f / sqrtf(7.0f), 1.0f / sqrtf(7.0f), -2.0f / sqrtf(7.0f), 1.0f / sqrtf(7.0f))));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector4D>(math::Vector4D(0.0f, 1.0f, 3.0f, -2.0f), math::Vector4D(0.0f, 1.0f / sqrtf(14.0f), 3.0f / sqrtf(14.0f), -2.0f / sqrtf(14.0f))));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector4D>(math::Vector4D(5.0f, 5.0f, 0.0f, -1.0f), math::Vector4D(5.0f / sqrtf(51.0f), 5.0f / sqrtf(51.0f), 0.0f, -1.0f / sqrtf(51.0f))));
	vectorTests.AddTest(new math_test::VectorTestNormalize<math::Vector4D>(math::Vector4D(3.0f, 4.0f, 1.0f, -10.0f), math::Vector4D(3.0f / sqrtf(126.0f), 4.0f / sqrtf(126.0f), 1.0f / sqrtf(126.0f), -10.0f / sqrtf(126.0f))));
	vectorTests.AddTest(new math_test::Vector2DTestCross(math::Vector2D(3.0f, 4.0f), math::Vector2D(2.0f, 1.0f), -5.0f));
	vectorTests.AddTest(new math_test::Vector2DTestCross(math::Vector2D(5.0f, 4.0f), math::Vector2D(2.0f, -1.0f), -13.0f));
	vectorTests.AddTest(new math_test::Vector2DTestCross(math::Vector2D(-3.0f, 2.0f), math::Vector2D(-3.0f, 3.0f), -3.0f));
	vectorTests.AddTest(new math_test::Vector2DTestCross(math::Vector2D(0.0f, 4.0f), math::Vector2D(4.0f, 1.0f), -16.0f));
	vectorTests.AddTest(new math_test::Vector3DTestCross(math::Vector3D(3.0f, 4.0f, 1.0f), math::Vector3D(2.0f, 1.0f, 5.0f), math::Vector3D(19.0f, -13.0f, -5.0f)));
	vectorTests.AddTest(new math_test::Vector3DTestCross(math::Vector3D(5.0f, 4.0f, -2.0f), math::Vector3D(2.0f, -1.0f, 3.0f), math::Vector3D(10.0f, -19.0f, -13.0f)));
	vectorTests.AddTest(new math_test::Vector3DTestCross(math::Vector3D(-3.0f, 2.0f, 0.0f), math::Vector3D(-3.0f, 3.0f, 1.0f), math::Vector3D(2.0f, 3.0f, -3.0f)));
	vectorTests.AddTest(new math_test::Vector3DTestCross(math::Vector3D(0.0f, 4.0f, -2.0f), math::Vector3D(4.0f, 1.0f, -1.0f), math::Vector3D(-2.0f, -8.0f, -16.0f)));

	//vectorTests.AddTest(new MathTest::Vector2DTestDot(math::Vector2D(3.0f, 4.0f), math::Vector2D(2.0f, 1.0f), 11.0f));
	//vectorTests.AddTest(new MathTest::Vector2DTestDot(math::Vector2D(5.0f, 4.0f), math::Vector2D(2.0f, -1.0f), 6.0f));
	//vectorTests.AddTest(new MathTest::Vector2DTestDot(math::Vector2D(-3.0f, 2.0f), math::Vector2D(-3.0f, 3.0f), 15.0f));
	//vectorTests.AddTest(new MathTest::Vector2DTestDot(math::Vector2D(0.0f, 4.0f), math::Vector2D(4.0f, 1.0f), 4.0f));
	//vectorTests.AddTest(new MathTest::Vector2DTestRotate(math::Vector2D(0.0f, 4.0f), math::Angle(45.0f), math::Vector2D(-2.0f * sqrtf(2.0f), 2.0f * sqrtf(2.0f))));
	//vectorTests.AddTest(new MathTest::Vector2DTestRotate(math::Vector2D(5.0f, 4.0f), math::Angle(90.0f), math::Vector2D(-4.0f, 5.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestRotate(math::Vector2D(5.0f, 4.0f), math::Angle(360.0f), math::Vector2D(5.0f, 4.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestRotate(math::Vector2D(-3.0f, 2.0f), math::Angle(-45.0f), math::Vector2D(-sqrtf(2.0f) / 2.0f, 2.5f * sqrtf(2.0f))));
	//vectorTests.AddTest(new MathTest::Vector2DTestRotate(math::Vector2D(3.0f, 4.0f), math::Angle(60.0f), math::Vector2D((3.0f - 4.0f * sqrtf(3.0f)) / 2.0f, (4.0f + 3.0f * sqrtf(3.0f)) / 2.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(math::Vector2D(0.0f, 4.0f), math::Vector2D(1.0f, 2.0f), 0.2f, math::Vector2D(0.2f, 3.6f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(math::Vector2D(5.0f, 4.0f), math::Vector2D(0.5f, 2.0f), 0.5f, math::Vector2D(2.75f, 3.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(math::Vector2D(5.0f, 4.0f), math::Vector2D(0.5f, 2.0f), 1.0f, math::Vector2D(0.5f, 2.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(math::Vector2D(-3.0f, 2.0f), math::Vector2D(0.5f, 2.0f), 0.25f, math::Vector2D(-2.125f, 2.0f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(math::Vector2D(3.0f, 4.0f), math::Vector2D(0.5f, 2.0f), 0.75f, math::Vector2D(1.125f, 2.5f)));
	//vectorTests.AddTest(new MathTest::Vector2DTestLerp(math::Vector2D(3.0f, 4.0f), math::Vector2D(0.5f, 2.0f), 0.0f, math::Vector2D(3.0f, 4.0f)));
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
	math_test::QuaternionTestGroup quaternionTests("Quaternion tests", 1000);
	quaternionTests.AddTest(new math_test::QuaternionTestCompare(Quaternion(0.0f, 0.0f, 0.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.0f, 0.0f), false));
	quaternionTests.AddTest(new math_test::QuaternionTestCompare(Quaternion(0.0f, 0.0f, 0.0f, 0.0f), Quaternion(1.0f, 1.0f, 1.0f, 1.0f), false));
	quaternionTests.AddTest(new math_test::QuaternionTestCompare(Quaternion(0.0f, 0.0f, 0.0f, 0.0f), Quaternion(1.0f, 1.0f, 1.0f, 1.0f), false));
	quaternionTests.AddTest(new math_test::QuaternionTestCompare(Quaternion(0.0f, 0.0f, 0.0f, 1.0f), Quaternion(0.0f, 0.0f, 0.0f, 1.0f), true));
	quaternionTests.AddTest(new math_test::QuaternionTestLength(Quaternion(1.0f, 2.0f, 3.0f, 4.0f), sqrtf(30.0f)));
	quaternionTests.AddTest(new math_test::QuaternionTestLength(Quaternion(0.0f, 0.0f, 0.0f, 4.0f), 4.0f));
	quaternionTests.AddTest(new math_test::QuaternionTestLength(Quaternion(3.0f, 4.0f, 0.0f, 0.0f), 5.0f));
	quaternionTests.AddTest(new math_test::QuaternionTestConjugate(Quaternion(3.0f, 4.0f, 2.0f, 1.0f), Quaternion(-3.0, -4.0f, -2.0f, 1.0f)));
	quaternionTests.AddTest(new math_test::QuaternionTestConjugate(Quaternion(1.0f, -5.0f, 3.0f, -7.0f), Quaternion(-1.0, 5.0f, -3.0f, -7.0f)));
	quaternionTests.AddTest(new math_test::QuaternionTestAddOperator(Quaternion(3.0f, 0.0f, 1.0f, -2.0f), Quaternion(-7.0, 15.0f, -2.0f, -4.0f), Quaternion(-4.0f, 15.0f, -1.0f, -6.0f)));
	quaternionTests.AddTest(new math_test::QuaternionTestAddOperator(Quaternion(2.0f, -8.0f, 13.0f, -9.0f), Quaternion(-2.0, -5.0f, -6.0f, -2.0f), Quaternion(0.0f, -13.0f, 7.0f, -11.0f)));
	quaternionTests.AddTest(new math_test::QuaternionTestAddOperator(Quaternion(1.0f, 8.0f, 5.0f, -7.0f), Quaternion(-1.0, 5.0f, 5.0f, -1.0f), Quaternion(0.0f, 13.0f, 10.0f, -8.0f)));

	quaternionTests.StartTests();
	quaternionTests.StartTimeTests();

	/* ==================== QUATERNION TEST #2- calculating rotation matrix- begin ==================== */
	INFO_LOG_MATH_TEST("Profiling quaternion to rotation matrix conversion functions begin");
	Quaternion q = RandomQuaternion();
	const int QUATERNION_TO_ROTATION_MATRIX_ITERATIONS = 5000000;
	timing::Timer timer;
	timer.Start();
	for (int i = 0; i < QUATERNION_TO_ROTATION_MATRIX_ITERATIONS; ++i)
	{
		q = RandomQuaternion();
		q.ToRotationMatrix();
	}
	timer.Stop();
	TimeReport("Average time for quaternion to rotation matrix conversion", timer, timing::MICROSECOND, QUATERNION_TO_ROTATION_MATRIX_ITERATIONS);
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
	TimeReport("Average time for quaternion linear interpolation function", timer, timing::MICROSECOND, QUATERNION_LINEAR_INTERPOLATION_ITERATIONS);

	DEBUG_LOG_MATH_TEST("Profiling quaternion linear interpolation functions end.");
	/* ==================== QUATERNION TEST #3- calculating linear interpolation- end ==================== */
}

Angle RandomAngle()
{
	const Real MIN_ANGLE = REAL_ZERO;
	const Real MAX_ANGLE = 360.0f;
	Angle angle(g_randomGenerator.NextFloat(MIN_ANGLE, MAX_ANGLE), units::DEGREE);
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

void M4x4_SSE(const math::Real* A, const math::Real* B, math::Real* C) {
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
	//auto matrix = math::Matrix<int, 100>::IDENTITY;
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

	auto identityMatrix1 = math::Matrix<int, 60, 45, matrix_orderings::ROW_MAJOR>::IDENTITY;
	auto identityMatrix2 = math::Matrix<int, 45, 70, matrix_orderings::ROW_MAJOR>::IDENTITY;

	/* ==================== MATRIX TEST #1 begin ==================== */
	constexpr int NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS = 1000000;
	timing::Timer timer;
	timer.Start();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS; ++i)
	{
		auto m = math::Matrix<int, 33, 44, matrix_orderings::ROW_MAJOR>::IDENTITY;
	}
	timer.Stop();
	TimeReport("Average time for identity matrix creation:\t", timer, timing::NANOSECOND, NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS);
	/* ==================== MATRIX TEST #1 end ==================== */

	/* ==================== MATRIX TEST #2 begin ==================== */
	constexpr int NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS = 2000000;
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS; ++i)
	{
		auto result = identityMatrix1 * identityMatrix2; // FIXME: Check matrix multiplication
		CHECK_CONDITION_MATH_TEST(result == identityMatrix1 * identityMatrix2, utility::logging::ERR, "Identity matrix multiplication result is incorrect.");
	}
	timer.Stop();
	TimeReport("Average time for identity matrices multiplication:\t", timer, timing::NANOSECOND, NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS);
	/* ==================== MATRIX TEST #2 end ==================== */
}

void MatrixTest()
{
	if (!matrixTestEnabled)
	{
		return;
	}
	math_test::MatrixTestGroup matrixTests("Matrix tests", 1000);

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
	Matrix4D matrix13(Vector2D(5.0f, -2.0f), math::Angle(0.0f), Vector2D(0.5f, 1.2f));
	Matrix4D matrix14(Vector3D(1.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f)); // rotation matrix
	Matrix4D matrix15(Vector3D(1.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 2.0f, 5.0f)); // rotation matrix
	Matrix4D matrix16(1, 2, 3, 0, 5, 6, 7, 0, 9, 10, 11, 0, 13, 14, 15, 1);
	Matrix4D matrix17(-1, 2, -3, 4, -5, 6, -7, 8, -9, 10, -11, 12, -13, 14, -15, 16);
	Matrix4D leftMultiplication(-38, 44, -50, 56, -98, 116, -134, 152, -158, 188, -218, 248, -231, 274, -317, 360);
	Matrix4D rightMultiplication(34, 36, 38, 4, 66, 68, 70, 8, 98, 100, 102, 12, 130, 132, 134, 16);

	matrixTests.AddTest(new math_test::MatrixTestCompare(matrix1, Matrix4D::IDENTITY_MATRIX, true));
	matrixTests.AddTest(new math_test::MatrixTestCompare(Matrix4D::IDENTITY_MATRIX, matrix1, true));
	matrixTests.AddTest(new math_test::MatrixTestCompare(matrix1, matrix2, true));
	matrixTests.AddTest(new math_test::MatrixTestCompare(matrix2, matrix1, true));
	matrixTests.AddTest(new math_test::MatrixTestCompare(matrix2, Matrix4D::IDENTITY_MATRIX, true));
	matrixTests.AddTest(new math_test::MatrixTestCompare(Matrix4D::IDENTITY_MATRIX, matrix2, true));
	matrixTests.AddTest(new math_test::MatrixTestCompare(matrix4, Matrix4D::IDENTITY_MATRIX, true));
	matrixTests.AddTest(new math_test::MatrixTestCompare(matrix7, matrix8, true));
	matrixTests.AddTest(new math_test::MatrixTestCompare(matrix7, Matrix4D::IDENTITY_MATRIX, false));
	matrixTests.AddTest(new math_test::MatrixTestCompare(matrix9, matrix6, true));
	matrixTests.AddTest(new math_test::MatrixTestCompare(matrix9, matrix9, true)); // comparing the matrix with itself
	matrixTests.AddTest(new math_test::MatrixTestCompare(matrix10, matrix11, true));
	matrixTests.AddTest(new math_test::MatrixTestCompare(matrix11, matrix10, true));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByMatrixOperator(Matrix4D::IDENTITY_MATRIX, Matrix4D::IDENTITY_MATRIX, Matrix4D::IDENTITY_MATRIX));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByMatrixOperator(Matrix4D::IDENTITY_MATRIX, matrix3, matrix3));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByMatrixOperator(Matrix4D::IDENTITY_MATRIX, matrix5, matrix5));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByMatrixOperator(matrix12, Matrix4D::IDENTITY_MATRIX, matrix12));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByMatrixOperator(matrix3, Matrix4D::IDENTITY_MATRIX, matrix3));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByMatrixOperator(matrix16, matrix17, rightMultiplication));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByMatrixOperator(matrix17, matrix16, leftMultiplication));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByVectorOperator(matrix1, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByVectorOperator(matrix2, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByVectorOperator(matrix3, Vector3D(2, 3, 5), Vector3D(4, 6, 10)));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByVectorOperator(matrix4, Vector3D(2, 3, 5), Vector3D(2, 3, 5)));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByVectorOperator(matrix5, Vector3D(2, 3, 5), Vector3D(5, -2, 7)));
	matrixTests.AddTest(new math_test::MatrixTestMultiplyByVectorOperator(matrix6, Vector3D(2, 3, 5), Vector3D(0.2f, 0.3f, -1.102102102102102102102102f)));

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

	timing::Timer timer;
	timer.Start();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS; ++i)
	{
		Matrix4D testMatrix2(Matrix4D::IDENTITY_MATRIX);
	}
	timer.Stop();
	TimeReport("Average time for identity matrix creation:\t", timer, timing::NANOSECOND, NUMBER_OF_IDENTITY_MATRIX_CREATION_ITERATIONS);
	/* ==================== MATRIX TEST #1 end ==================== */

	/* ==================== MATRIX TEST #2 begin ==================== */
	constexpr int NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS = 2000000;
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS; ++i)
	{
		Matrix4D result = identityMatrix1 * identityMatrix2; // FIXME: Check matrix multiplication
		CHECK_CONDITION_MATH_TEST(result == identityMatrix1 * identityMatrix2, utility::logging::ERR, "Identity matrix multiplication result is incorrect.");
	}
	timer.Stop();
	TimeReport("Average time for identity matrices multiplication:\t", timer, timing::NANOSECOND, NUMBER_OF_IDENTITY_MATRIX_MULTIPLICATION_ITERATIONS);
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
	TimeReport("Average time for random matrices multiplication:\t", timer, timing::NANOSECOND, NUMBER_OF_RANDOM_MATRIX_MULTIPLICATION_ITERATIONS);
	/* ==================== MATRIX TEST #3 end ==================== */

	/* ==================== MATRIX TEST #4 begin ==================== */
	constexpr int NUMBER_OF_ROTATION_EULER_ITERATIONS = 100000;
	timer.Reset();
	for (unsigned int i = 0; i < NUMBER_OF_ROTATION_EULER_ITERATIONS; ++i)
	{
		Matrix4D result(RandomAngle(), RandomAngle(), RandomAngle());
	}
	timer.Stop();
	TimeReport("Average time for rotation Euler matrix calculation:\t", timer, timing::NANOSECOND, NUMBER_OF_ROTATION_EULER_ITERATIONS);
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
	TimeReport("Average time for SSE 1D matrix multiplication:\t", timer, timing::NANOSECOND, NUMBER_OF_SSE_MATRIX_MULTIPLICATIONS);

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
	TimeReport("Average time for SSE 2D matrix multiplication:\t", timer, timing::NANOSECOND, NUMBER_OF_SSE_MATRIX_MULTIPLICATIONS);
}

void TransformTest()
{
	if (!transformTestEnabled)
	{
		return;
	}
	math_test::TransformTestGroup transformTests("Transform tests", 1000);

	transformTests.AddTest(new math_test::TransformTestCompare(Transform(), Transform(), true));
	transformTests.AddTest(new math_test::TransformTestParent(Transform(Vector3D(1.0f, 2.0f, 3.0f), math::NO_ROTATION_QUATERNION, 1.0f),
		Transform(Vector3D(1.0f, 2.0f, 3.0f), math::NO_ROTATION_QUATERNION, 1.0f),
		Transform(Vector3D(2.0f, 4.0f, 6.0f), math::NO_ROTATION_QUATERNION, 1.0f)));
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

	logging::ILogger::GetLogger(MODULE_NAME).AddStream("sortingOutput.txt");
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
		DELOCUST_LOG_MATH_TEST("initialVectors[", i, "] = ", initialVectors[i]);
	}

	constexpr int NUMBER_OF_SORTING_METHODS = 1; /* the number of sorting methods in the Math library we want to check (10 means we want to check all of them) */
	const std::array<int, NUMBER_OF_SORTING_METHODS> chosenSortingMethodIndices = { 5 }; // its length must match the value of NUMBER_OF_SORTING_METHODS variable
	const std::array<sorting::sorting_algorithms::SortingAlgorithm, sorting::sorting_algorithms::COUNT> sortingMethods = { sorting::sorting_algorithms::BUBBLE_SORT,
		sorting::sorting_algorithms::INSERTION_SORT, sorting::sorting_algorithms::SELECTION_SORT, sorting::sorting_algorithms::MERGE_SORT,
		sorting::sorting_algorithms::HEAP_SORT, sorting::sorting_algorithms::QUICK_SORT, sorting::sorting_algorithms::SHELL_SORT,
		sorting::sorting_algorithms::COMB_SORT, sorting::sorting_algorithms::COUNTING_SORT, sorting::sorting_algorithms::RADIX_SORT,
		sorting::sorting_algorithms::BUCKET_SORT };
	const std::array<std::string, sorting::sorting_algorithms::COUNT> sortingMethodsStr = { "Bubble sort", "Insertion sort", "Selection sort", "Merge sort",
		"Heap sort", "Quick sort", "Shell sort", "Comb sort", "Counting sort", "Radix sort", "Bucket sort" };

	constexpr int NUMBER_OF_TIME_TESTS_ITERATIONS = 20;
	/**
	 * TODO: Instead of measuring the time needed to perform NUMBER_OF_TIME_TESTS_ITERATION sorts it should be more efficient to count the number of successful sorts within given period of time.
	 * If this time limit is passed then sorting is stopped no matter if it's finished or not.
	 */
	for (int sortingMethodIndex = 0; sortingMethodIndex < NUMBER_OF_SORTING_METHODS; ++sortingMethodIndex)
	{
		std::unique_ptr<sorting::ISort> sorter = sorting::ISort::GetSortingObject(sortingMethods[chosenSortingMethodIndices[sortingMethodIndex]]);
		/* ==================== SORTING TEST #1- sorting Vector2D objects by X component ascending begin ==================== */
		bool sortingTestCasePassed = true;
		sorting::SortingParametersChain sortingParameters(sorting::keys::COMPONENT_X, sorting::orders::ASCENDING);
		sortingParameters.AddChainLink(new sorting::SortingParametersChain(sorting::keys::COMPONENT_Y, sorting::orders::DESCENDING));
		timing::Timer timer;
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
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by X component", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by X component per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * NUMBER_OF_VECTORS);

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
		sortingParameters.SetKey(sorting::keys::COMPONENT_X);
		sortingParameters.SetOrder(sorting::orders::DESCENDING);
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
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by X component", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by X component per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * NUMBER_OF_VECTORS);

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
		sortingParameters.SetKey(sorting::keys::COMPONENT_Y);
		sortingParameters.SetOrder(sorting::orders::ASCENDING);
		sortingParameters.AddChainLink(new sorting::SortingParametersChain(sorting::keys::COMPONENT_X, sorting::orders::DESCENDING));
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
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by Y component", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by Y component per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * NUMBER_OF_VECTORS);

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
		sortingParameters.SetKey(sorting::keys::COMPONENT_Y);
		sortingParameters.SetOrder(sorting::orders::DESCENDING);
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
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by Y component", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by Y component per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * NUMBER_OF_VECTORS);

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
		sortingParameters.SetKey(sorting::keys::SUM_OF_COMPONENTS);
		sortingParameters.SetOrder(sorting::orders::ASCENDING);
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
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of components", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of components per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * NUMBER_OF_VECTORS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			math::Real sumOfComponents1 = vectors[i].SumOfComponents();
			math::Real sumOfComponents2 = vectors[i + 1].SumOfComponents();
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
		sortingParameters.SetKey(sorting::keys::SUM_OF_COMPONENTS);
		sortingParameters.SetOrder(sorting::orders::DESCENDING);
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
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of components", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of components per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * NUMBER_OF_VECTORS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			math::Real sumOfComponents1 = vectors[i].SumOfComponents();
			math::Real sumOfComponents2 = vectors[i + 1].SumOfComponents();
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
		sortingParameters.SetKey(sorting::keys::SUM_OF_ABSOLUTE_COMPONENTS);
		sortingParameters.SetOrder(sorting::orders::ASCENDING);
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
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of absolute components", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of absolute components per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * NUMBER_OF_VECTORS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			math::Real sumOfAbsoluteComponents1 = vectors[i].SumOfAbsoluteComponents();
			math::Real sumOfAbsoluteComponents2 = vectors[i + 1].SumOfAbsoluteComponents();
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
		sortingParameters.SetKey(sorting::keys::SUM_OF_ABSOLUTE_COMPONENTS);
		sortingParameters.SetOrder(sorting::orders::DESCENDING);
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
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of absolute components", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of absolute components per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * NUMBER_OF_VECTORS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			math::Real sumOfAbsoluteComponents1 = vectors[i].SumOfAbsoluteComponents();
			math::Real sumOfAbsoluteComponents2 = vectors[i + 1].SumOfAbsoluteComponents();
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
		sortingParameters.SetKey(sorting::keys::SUM_OF_SQUARED_COMPONENTS);
		sortingParameters.SetOrder(sorting::orders::ASCENDING);
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
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of squared components", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
		TimeReport("Average time for ASCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of squared components per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * NUMBER_OF_VECTORS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			math::Real lengthSquared1 = vectors[i].LengthSquared();
			math::Real lengthSquared2 = vectors[i + 1].LengthSquared();
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
		sortingParameters.SetKey(sorting::keys::SUM_OF_SQUARED_COMPONENTS);
		sortingParameters.SetOrder(sorting::orders::DESCENDING);
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
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of squared components", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
		TimeReport("Average time for DESCENDING " + sortingMethodsStr[chosenSortingMethodIndices[sortingMethodIndex]] + " for " + std::to_string(NUMBER_OF_VECTORS) + " elements by sum of squared components per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * NUMBER_OF_VECTORS);

		for (int i = 0; i < NUMBER_OF_VECTORS - 1; ++i) // Checking if vectors are sorted correctly
		{
			math::Real lengthSquared1 = vectors[i].LengthSquared();
			math::Real lengthSquared2 = vectors[i + 1].LengthSquared();
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

	const random::RandomGenerator& randomGenerator = random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(random::generator_ids::SIMPLE);
	constexpr auto countingSortArraySize = 1000000;
	constexpr auto countingSortIntLowerBound = -1000000;
	constexpr auto countingSortIntUpperBound = 1000000;
	std::unique_ptr<sorting::ISort> csSorter = sorting::ISort::GetSortingObject(sorting::sorting_algorithms::COUNTING_SORT);
	int* initialIntArray = new int[countingSortArraySize];
	int* intArray = new int[countingSortArraySize];
	for (int i = 0; i < countingSortArraySize; ++i)
	{
		initialIntArray[i] = randomGenerator.NextInt(countingSortIntLowerBound, countingSortIntUpperBound);
		intArray[i] = initialIntArray[i];
	}

	timing::Timer timer;
	timer.Start();
	for (auto k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
	{
		for (auto i = 0; i < countingSortArraySize; ++i)
		{
			intArray[i] = initialIntArray[i];
		}
		csSorter->Sort(intArray, countingSortArraySize, sorting::orders::ASCENDING);
	}
	timer.Stop();
	TimeReport("Average time for ASCENDING Counting sort for " + std::to_string(countingSortArraySize) + " elements", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
	TimeReport("Average time for ASCENDING Counting sort for " + std::to_string(countingSortArraySize) + " elements per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * countingSortArraySize);
	// Checking if vectors are sorted correctly
	for (int i = 0; i < countingSortArraySize - 1; ++i)
	{
		if (intArray[i + 1] < intArray[i])
		{
			ERROR_LOG_MATH_TEST("Counting sort in ASCENDING order failed. intArray[", i + 1, "] = ", intArray[i + 1], " should precede intArray[", i, "] = ", intArray[i]);
		}
	}
	timer.Reset();
	for (auto k = 0; k < NUMBER_OF_TIME_TESTS_ITERATIONS; ++k)
	{
		for (auto i = 0; i < countingSortArraySize; ++i)
		{
			intArray[i] = initialIntArray[i];
		}
		csSorter->Sort(intArray, countingSortArraySize, sorting::orders::DESCENDING);
	}
	timer.Stop();
	TimeReport("Average time for DESCENDING Counting sort for " + std::to_string(countingSortArraySize) + " elements", timer, timing::MILLISECOND, NUMBER_OF_TIME_TESTS_ITERATIONS);
	TimeReport("Average time for DESCENDING Counting sort for " + std::to_string(countingSortArraySize) + " elements per single element", timer, timing::NANOSECOND, NUMBER_OF_TIME_TESTS_ITERATIONS * countingSortArraySize);
	// Checking if vectors are sorted correctly
	for (int i = 0; i < countingSortArraySize - 1; ++i)
	{
		if (intArray[i + 1] > intArray[i])
		{
			ERROR_LOG_MATH_TEST("Counting sort in DESCENDING order failed. intArray[", i + 1, "] = ", intArray[i + 1], " should precede intArray[", i, "] = ", intArray[i]);
		}
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

	std::unique_ptr<KdTree> kdTree = std::make_unique<KdTree>(positions.data(), NUMBER_OF_POSITIONS, NUMBER_OF_INTERPOLATED_NEAREST_POINTS);

	//DEBUG_LOG_MATH_TEST("K-d tree structure:\n", kdTree->ToString());

	std::array<math::Vector2D, NUMBER_OF_SEARCH_POSITIONS> searchPositions;
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

	NOTICE_LOG_MATH_TEST("K-d tree test finished");
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

	math_test::StatsTest1 statsTest1;
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

	math_test::StatsTest2 statsTest2;
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
	timing::Timer timer;
	timer.Start();

	math::Real amplitude = 70.0f;
	int octaves = 3;
	math::Real roughness = 0.3f;
	math::HeightsGenerator heightsGenerator(0, 0, 128, amplitude, octaves, roughness);

	constexpr int WIDTH = 8;
	constexpr int HEIGHT = 8;
	std::array<math::Real, WIDTH * HEIGHT> heights;
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

void InterpolationTest()
{
	if (!interpolationTestsEnabled)
	{
		return;
	}

	NOTICE_LOG_MATH_TEST("Interpolation tests started");
	const Vector3D vertexA(REAL_ZERO, 10.0f, 2.0f / 3.0f);
	const Vector3D vertexB(2.0f / 3.0f, 9.0f, 2.0f / 3.0f);
	const Vector3D vertexC(2.0f / 3.0f, 8.0f, REAL_ZERO);
	const Vector3D vertexD(REAL_ZERO, 7.0f, REAL_ZERO);
	/* ==================== INTERPOLATION TEST #1- barycentric- begin ==================== */
	constexpr int TEST_POSITIONS_COUNT = 4;
	std::array<Vector2D, TEST_POSITIONS_COUNT> testPositions = { Vector2D(vertexA.x, vertexA.z), Vector2D(vertexB.x, vertexB.z), Vector2D(vertexC.x, vertexC.z),
		Vector2D(vertexD.x, vertexD.z) };
	std::array<Real, TEST_POSITIONS_COUNT> testPositionsExpectedHeights = { vertexA.y, vertexB.y, vertexC.y, vertexD.y };
	for (int i = 0; i < TEST_POSITIONS_COUNT; ++i)
	{
		Real height = interpolation::BarycentricInterpolation(vertexA, vertexC, vertexD, testPositions[i].x, testPositions[i].y);
		//Real height = Interpolation::BarycentricInterpolation(vertexA, vertexB, vertexC, testPositions[i].x, testPositions[i].y);
		stringstream ss("");
		ss << "Incorrect height (" << height << ") calculated for the triangle at position: " << testPositions[i] << ". Expected height equals: " << testPositionsExpectedHeights[i];
		TestReport(AlmostEqual(height, testPositionsExpectedHeights[i]), ss.str());
	}
	/* ==================== INTERPOLATION TEST #2- barycentric- begin ==================== */
	NOTICE_LOG_MATH_TEST("Interpolation tests finished");
}

void SurfaceTest()
{
	if (!surfaceTestsEnabled)
	{
		return;
	}

	/* ==================== SURFACE TEST #1- simple surface- begin ==================== */
	NOTICE_LOG_MATH_TEST("Simple surface test started");
	constexpr int SIMPLE_SURFACE_WIDTH = 10;
	constexpr int SIMPLE_SURFACE_DEPTH = 10;
	constexpr int SIMPLE_HORIZONTAL_VERTICES_COUNT = 2;
	constexpr int SIMPLE_VERTICAL_VERTICES_COUNT = 2;
	std::array<Real, SIMPLE_HORIZONTAL_VERTICES_COUNT * SIMPLE_VERTICAL_VERTICES_COUNT> simpleHeights = { 0, 1, 2, 3 };
	Surface surface1(Vector2D(0.0f, 0.0f), SIMPLE_SURFACE_WIDTH, SIMPLE_SURFACE_DEPTH, SIMPLE_HORIZONTAL_VERTICES_COUNT, SIMPLE_VERTICAL_VERTICES_COUNT, simpleHeights.data());

	constexpr int SIMPLE_SURFACE_HEIGHT_CALCULATION_TESTS_COUNT = 19;
	std::array<Vector3D, SIMPLE_SURFACE_HEIGHT_CALCULATION_TESTS_COUNT> surfacePositions = { Vector3D(0.0f, 0.0f, 0.0f), Vector3D(5.0f, 0.5f, 0.0f), Vector3D(10.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 5.0f),
		Vector3D(5.0f, 1.5f, 5.0f), Vector3D(10.0f, 2.0f, 5.0f), Vector3D(2.5f, 0.25f, 0.0f), Vector3D(2.5f, 0.75f, 2.5f), Vector3D(2.5f, 1.25f, 5.0f), Vector3D(2.5f, 1.75f, 7.5f), Vector3D(2.5f, 2.25f, 10.0f),
		Vector3D(4.0f, 1.7f, 6.5f), Vector3D(1.3f, 0.862f, 3.66f), Vector3D(7.2f, 1.42f, 3.5f), Vector3D(9.8f, 2.88f, 9.5f), Vector3D(9.9f, 0.998f, 0.04f), Vector3D(1.2f, 1.82f, 8.5f), Vector3D(3.5f, 1.608f, 6.29f),
		Vector3D(8.5f, 0.85f, 0.0f)};
	for (int i = 0; i < SIMPLE_SURFACE_HEIGHT_CALCULATION_TESTS_COUNT; ++i)
	{
		Real height = surface1.GetHeightAt(surfacePositions[i].GetXz());
		stringstream ss("");
		ss << "Incorrect height (" << height << ") calculated for the surface at position: " << surfacePositions[i].GetXz() << ". Expected height equals: " << surfacePositions[i].y;
		TestReport(AlmostEqual(height, surfacePositions[i].y), ss.str());
	}
	NOTICE_LOG_MATH_TEST("Simple surface test finished");
	/* ==================== SURFACE TEST #1- simple surface- end ==================== */

	/* ==================== SURFACE TEST #2- more complex surface- begin ==================== */
	NOTICE_LOG_MATH_TEST("More complex surface test started");
	constexpr int SURFACE_WIDTH = 50;
	constexpr int SURFACE_DEPTH = 50;
	constexpr int HORIZONTAL_VERTICES_COUNT = 6;
	constexpr int VERTICAL_VERTICES_COUNT = 6;
	std::array<Real, HORIZONTAL_VERTICES_COUNT * VERTICAL_VERTICES_COUNT> heights = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
	Surface surface2(Vector2D(5.0f, 5.0f), SURFACE_WIDTH, SURFACE_DEPTH, HORIZONTAL_VERTICES_COUNT, VERTICAL_VERTICES_COUNT, heights.data());

	constexpr int SURFACE2_HEIGHT_CALCULATION_TESTS_COUNT = 10;
	std::array<Vector3D, SURFACE2_HEIGHT_CALCULATION_TESTS_COUNT> surface2Positions = { Vector3D(5.0f, 0.0f, 5.0f), Vector3D(10.0f, 0.5f, 5.0f), Vector3D(12.2f, 0.72f, 5.0f), Vector3D(12.2f, 2.52f, 8.0f),
		Vector3D(15.0f, 1.0f, 5.0f), Vector3D(19.0f, 1.4f, 5.0f), Vector3D(32.2f, 8.12f, 14.0f), Vector3D(32.2f, 9.98f, 17.1f), Vector3D(32.2f, 10.94f, 18.7f), Vector3D(40.0f, 12.56f, 20.1f) };
	for (int i = 0; i < SURFACE2_HEIGHT_CALCULATION_TESTS_COUNT; ++i)
	{
		Real height = surface2.GetHeightAt(surface2Positions[i].GetXz());
		stringstream ss("");
		ss << "Incorrect height (" << height << ") calculated for the surface at position: " << surface2Positions[i].GetXz() << ". Expected height equals: " << surface2Positions[i].y;
		TestReport(AlmostEqual(height, surface2Positions[i].y), ss.str());
	}
	NOTICE_LOG_MATH_TEST("More complex surface test finished");
	/* ==================== SURFACE TEST #2- more complex surface- end ==================== */

	/* ==================== SURFACE TEST #3- create a simple surface out of collection of random 3D positions- begin ==================== */
	NOTICE_LOG_MATH_TEST("Customary simple surface test started");
	constexpr int RANDOM_POSITIONS_COUNT_3 = 5;
	std::array<Vector3D, RANDOM_POSITIONS_COUNT_3> positions3 = { Vector3D(0.0f, 0.0f, 0.0f), Vector3D(10.0f, 1.0f, 0.0f), Vector3D(10.0f, 3.0f, 10.0f),
		Vector3D(5.0f, 0.5f, 0.0f), Vector3D(2.5f, 2.0f, 8.0f) };
	Surface surface3(positions3.data(), RANDOM_POSITIONS_COUNT_3);

	constexpr int SURFACE_HEIGHT_CALCULATION_TESTS_COUNT_3 = 18;
	std::array<Vector3D, SURFACE_HEIGHT_CALCULATION_TESTS_COUNT_3> surfacePositions3 = { Vector3D(0.0f, 0.0f, 0.0f), Vector3D(5.0f, 0.5f, 0.0f), Vector3D(10.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.12402093f, 5.0f),
		Vector3D(5.0f, 1.68146205f, 5.0f), Vector3D(10.0f, 1.70000017f, 5.0f), Vector3D(2.5f, 0.25f, 0.0f), Vector3D(2.5f, 0.812010465f, 2.5f), Vector3D(2.5f, 1.40274149f, 5.0f), Vector3D(2.5f, 1.79819995f, 7.5f), Vector3D(2.5f, 2.04217964f, 10.0f),
		Vector3D(4.0f, 1.776953148f, 6.5f), Vector3D(1.3f, 0.952783321f, 3.66f), Vector3D(7.2f, 1.479618772f, 3.5f), Vector3D(9.8f, 2.836776874f, 9.5f), Vector3D(9.9f, 0.999451696f, 0.04f), Vector3D(1.2f, 1.811448643f, 8.5f), Vector3D(3.5f, 1.718286279f, 6.29f) };
	for (int i = 0; i < SURFACE_HEIGHT_CALCULATION_TESTS_COUNT_3; ++i)
	{
		Real height = surface3.GetHeightAt(surfacePositions3[i].GetXz());
		stringstream ss("");
		ss << "Incorrect height (" << height << ") calculated for the surface at position: " << surfacePositions3[i].GetXz() << ". Expected height equals: " << surfacePositions3[i].y;
		TestReport(AlmostEqual(height, surfacePositions3[i].y), ss.str());
	}
	NOTICE_LOG_MATH_TEST("Customary simple surface test finished");
	/* ==================== SURFACE TEST #3- create a simple surface out of collection of random 3D positions- end ==================== */

	/* ==================== SURFACE TEST #4- create a surface out of collection of random 3D positions- begin ==================== */
	NOTICE_LOG_MATH_TEST("Customary complex surface test started");
	constexpr int RANDOM_POSITIONS_COUNT_4 = 12;
	std::array<Vector3D, RANDOM_POSITIONS_COUNT_4> positions4 = { Vector3D(3.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(6.0f, 2.0f, 1.0f),
		Vector3D(4.0f, 3.0f, 2.0f), Vector3D(0.0f, 4.0f, 3.0f), Vector3D(2.0f, 5.0f, 3.0f), Vector3D(5.0f, 6.0f, 3.0f), Vector3D(4.5f, 7.0f, 3.5f),
		Vector3D(3.0f, 8.0f, 4.0f), Vector3D(1.5f, 9.0f, 5.0f), Vector3D(5.0f, 10.0f, 5.0f), Vector3D(4.0f, 11.0f, 6.0f) };
	Surface surface4(positions4.data(), RANDOM_POSITIONS_COUNT_4);

	constexpr int SURFACE_HEIGHT_CALCULATION_TESTS_COUNT_4 = 10;
	std::array<Vector3D, SURFACE_HEIGHT_CALCULATION_TESTS_COUNT_4> surfacePositions4 = { Vector3D(5.0f, 9.75358249f, 5.0f), Vector3D(1.0f, 3.71893555f, 2.5f), Vector3D(5.0f, 9.702269898f, 4.9f),
		Vector3D(5.0f, 9.825219501f, 5.1f), Vector3D(1.5f, 9.04133601f, 5.7f), Vector3D(5.7f, 2.386133103f, 1.25f), Vector3D(3.2f, 2.53062155f, 1.44f), Vector3D(4.75f, 6.227233357f, 2.99f), Vector3D(5.9f, 7.701587344f, 4.0f), Vector3D(0.9f, 6.761328315f, 4.1f) };
	for (int i = 0; i < SURFACE_HEIGHT_CALCULATION_TESTS_COUNT_4; ++i)
	{
		Real height = surface4.GetHeightAt(surfacePositions4[i].GetXz());
		stringstream ss("");
		ss << "Incorrect height (" << height << ") calculated for the surface at position: " << surfacePositions4[i].GetXz() << ". Expected height equals: " << surfacePositions4[i].y;
		TestReport(AlmostEqual(height, surfacePositions4[i].y), ss.str());
	}
	NOTICE_LOG_MATH_TEST("Customary complex surface test finished");
	/* ==================== SURFACE TEST #4- create a surface out of collection of random 3D positions- end ==================== */
}

void OtherTests()
{
	if (!otherTestsEnabled)
	{
		return;
	}

	constexpr unsigned int VALUES_COUNT = 5;
	std::array<math::Real, VALUES_COUNT> values = { -2.15f, -1.05f, 0.0f, 1.1f, 2.2f };
	std::array<math::Real, VALUES_COUNT> floorValues = { -3.0f, -2.0f, 0.0f, 1.0f, 2.0f };
	std::array<math::Real, VALUES_COUNT> zeroToOneClampValues = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f };
	std::array<math::Real, VALUES_COUNT> minusOneAndHalfToOneClampValues = { -1.5f, -1.05f, 0.0f, 1.0f, 1.0f };
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
	std::array<math::Real, ANGLES_COUNT> anglesInDegrees = { -30.0f, 0.0f, 30.0f, 60.0f, 90.0f };
	std::array<math::Real, ANGLES_COUNT> anglesInRadians = { -math::PI / 6.0f, 0.0f, math::PI / 6.0f, math::PI / 3.0f, math::PI / 2.0f };
	for (unsigned int i = 0; i < ANGLES_COUNT; ++i)
	{
		std::stringstream ss("");
		ss << "Angle " << anglesInDegrees[i] << " [deg] should be equal to angle " << anglesInRadians[i] << " [rad].";
		TestReport(math::AlmostEqual(anglesInDegrees[i], math::ToDeg(anglesInRadians[i])), ss.str());
		TestReport(math::AlmostEqual(math::ToRad(anglesInDegrees[i]), anglesInRadians[i]), ss.str());
	}

	const random::RandomGenerator& randomGenerator = random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(random::generator_ids::SIMPLE);
	constexpr auto randomIntTestsCount = 100000;
	constexpr auto minRandomValue = 0;
	constexpr auto maxRandomValue = 8;
	array<int, maxRandomValue - minRandomValue> randomValuesCounter;
	randomValuesCounter.fill(0);
	for (auto i = 0; i < randomIntTestsCount; ++i)
	{
		const auto value = randomGenerator.NextInt(minRandomValue, maxRandomValue);
		if (value < minRandomValue)
		{
			ERROR_LOG_MATH_TEST("Random value ", value, " is less than minimum ", minRandomValue);
		}
		else if (value >= maxRandomValue)
		{
			ERROR_LOG_MATH_TEST("Random value ", value, " is greater or equal than maximum ", maxRandomValue);
		}
		++randomValuesCounter[value - minRandomValue];
	}
	for (auto index = 0; index < randomValuesCounter.size(); ++index)
	{
		INFO_LOG_MATH_TEST("Value ", index - minRandomValue, " drawn ", randomValuesCounter[ index ], " times (",
			100.0f * static_cast<float>(randomValuesCounter[index]) / randomIntTestsCount, "%)");
	}
	TestReport(randomValuesCounter.front() > 0, "Min value hasn't been found.");
	TestReport(randomValuesCounter.back() > 0, "Max value hasn't been found.");
}

int main(int argc, char* argv[])
{
	srand(static_cast<unsigned int>(time(nullptr)));
	std::unique_ptr<ICommandLineMapper> commandLineMapper = ICommandLineMapper::CreateCommandLineMapper(argc, argv);
	//if (commandLineMapper.IsPresent("-help"))
	//{
	//	PrintHelp();
	//	system("pause");
	//	return 0;
	//}

	//constexpr int ACCEPTABLE_PROGRAM_ARGUMENTS_COUNT = 4;
	//const std::array<std::string, ACCEPTABLE_PROGRAM_ARGUMENTS_COUNT> acceptableProgramArguments = { "-help", "-logUtility", "-logMath", "-log" };

	logging::ILogger::GetLogger("Utility").Fill(commandLineMapper->Get("-logUtility", "Info"), logging::INFO);
	logging::ILogger::GetLogger("Math").Fill(commandLineMapper->Get("-logMath", "Info"), logging::INFO);
	logging::ILogger::GetLogger(MODULE_NAME).Fill(commandLineMapper->Get("-log", ""), logging::INFO);
	

	STATS_STORAGE.StartTimer();

	AngleTest();
	//VectorTest();
	//QuaternionTest();
	//MatrixTest();
	//NewMatrixTest();
	//TransformTest();
	SortTest();
	//SortTestTime();
	//InterpolationTest();
	//SurfaceTest();

	//KDTreeTest();

	//StatsTest();

	//HeightsGeneratorTests();
	OtherTests();

	STATS_STORAGE.StopTimer();
	STATS_STORAGE.PrintReport();

	logging::ILogger::GetLogger(MODULE_NAME).ResetConsoleColor();
	std::cout << "Bye!" << std::endl;

	return 0;
}
