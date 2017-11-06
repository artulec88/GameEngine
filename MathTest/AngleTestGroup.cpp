#include "AngleTestGroup.h"
#include "Math\FloatingPoint.h"
#include "Def.h"

using namespace MathTest;


/* ==================== class AngleTestGroup begin ==================== */
AngleTestGroup::AngleTestGroup(const std::string& angleTestGroupName, int testTimeIterationsCount) :
	TestGroup(angleTestGroupName, testTimeIterationsCount)
{
}

AngleTestGroup::~AngleTestGroup(void)
{
}
/* ==================== class AngleTestGroup end ==================== */

/* ==================== class AngleTestBase begin ==================== */
AngleTestBase::AngleTestBase(const Math::Angle& angle) :
	Test(),
	m_angle(angle)
{
}

AngleTestBase::~AngleTestBase(void)
{
}
/* ==================== class AngleTestBase end ==================== */

/* ==================== class AngleTestTrigonometry begin ==================== */
AngleTestTrigonometry::AngleTestTrigonometry(const Math::Angle& angle, Math::Real expectedTrigonometryOperatorValue) :
	AngleTestBase(angle),
	m_expectedTrigonometryOperatorValue(expectedTrigonometryOperatorValue)
{
}

AngleTestTrigonometry::~AngleTestTrigonometry()
{
}
/* ==================== class AngleTestTrigonometry end ==================== */

/* ==================== class AngleTestSin begin ==================== */
AngleTestSin::AngleTestSin(const Math::Angle& angle, Math::Real expectedSinValue) :
	AngleTestTrigonometry(angle, expectedSinValue)
{
}

AngleTestSin::~AngleTestSin()
{
}

void AngleTestSin::StartTest()
{
	const Math::Real DIFF_THRESHOLD = 0.002f;

	Math::Real sinValue = m_angle.Sin();
	bool sinEqual = Math::AlmostEqual(sinValue, m_expectedTrigonometryOperatorValue);
	CHECK_CONDITION_ALWAYS_MATH_TEST(sinEqual, Utility::Logging::ERR,
		"The sinus of an angle ", m_angle, " equals ", sinValue, " while it was expected to be ", m_expectedTrigonometryOperatorValue, ".");

	sinValue = m_angle.FastSin1();
	sinEqual = abs(sinValue - m_expectedTrigonometryOperatorValue) < DIFF_THRESHOLD;
	CHECK_CONDITION_ALWAYS_MATH_TEST(sinEqual, Utility::Logging::ERR,
		"The first approximate sinus of an angle ", m_angle, " equals ", sinValue, " while it was expected to be ", m_expectedTrigonometryOperatorValue, ".");

	sinValue = m_angle.FastSin2();
	sinEqual = abs(sinValue - m_expectedTrigonometryOperatorValue) < DIFF_THRESHOLD;
	CHECK_CONDITION_ALWAYS_MATH_TEST(sinEqual, Utility::Logging::ERR,
		"The second approximate sinus of an angle ", m_angle, " equals ", sinValue, " while it was expected to be ", m_expectedTrigonometryOperatorValue, ".");
}

void AngleTestSin::StartTimeTest(unsigned int iterationsCount)
{
	for (unsigned int i = 0; i < iterationsCount; ++i)
	{
		m_angle.Sin();
	}
}
/* ==================== class AngleTestSin end ==================== */

/* ==================== class AngleTestCos begin ==================== */
AngleTestCos::AngleTestCos(const Math::Angle& angle, Math::Real expectedCosValue) :
	AngleTestTrigonometry(angle, expectedCosValue)
{
}

AngleTestCos::~AngleTestCos()
{
}

void AngleTestCos::StartTest()
{
	const Math::Real DIFF_THRESHOLD = 0.002f;

	Math::Real cosValue = m_angle.Cos();
	bool cosEqual = Math::AlmostEqual(cosValue, m_expectedTrigonometryOperatorValue);
	CHECK_CONDITION_ALWAYS_MATH_TEST(cosEqual, Utility::Logging::ERR,
		"The cosinus of an angle ", m_angle, " equals ", cosValue, " while it was expected to be ", m_expectedTrigonometryOperatorValue, ".");

	cosValue = m_angle.FastCos1();
	cosEqual = abs(cosValue - m_expectedTrigonometryOperatorValue) < DIFF_THRESHOLD;
	CHECK_CONDITION_ALWAYS_MATH_TEST(cosEqual, Utility::Logging::ERR,
		"The first approximate cosinus of an angle ", m_angle, " equals ", cosValue, " while it was expected to be ", m_expectedTrigonometryOperatorValue, ".");

	cosValue = m_angle.FastCos2();
	cosEqual = abs(cosValue - m_expectedTrigonometryOperatorValue) < DIFF_THRESHOLD;
	CHECK_CONDITION_ALWAYS_MATH_TEST(cosEqual, Utility::Logging::ERR,
		"The second approximate cosinus of an angle ", m_angle, " equals ", cosValue, " while it was expected to be ", m_expectedTrigonometryOperatorValue, ".");
}

void AngleTestCos::StartTimeTest(unsigned int iterationsCount)
{
	for (unsigned int i = 0; i < iterationsCount; ++i)
	{
		m_angle.Cos();
	}
}
/* ==================== class AngleTestCos end ==================== */

/* ==================== class AngleTestTan begin ==================== */
AngleTestTan::AngleTestTan(const Math::Angle& angle, Math::Real expectedTanValue) :
	AngleTestTrigonometry(angle, expectedTanValue)
{
}

AngleTestTan::~AngleTestTan()
{
}

void AngleTestTan::StartTest()
{
	if (!Math::AlmostEqual(m_angle.Cos(), REAL_ZERO))
	{
		const Math::Real DIFF_THRESHOLD = 0.002f;

		Math::Real tanValue = m_angle.Tan();
		bool tanEqual = Math::AlmostEqual(tanValue, m_expectedTrigonometryOperatorValue);
		CHECK_CONDITION_ALWAYS_MATH_TEST(tanEqual, Utility::Logging::ERR,
			"The tangent of an angle ", m_angle, " equals ", tanValue, " while it was expected to be ", m_expectedTrigonometryOperatorValue, ".");

		tanValue = m_angle.FastTan1();
		tanEqual = abs(tanValue - m_expectedTrigonometryOperatorValue) < DIFF_THRESHOLD;
		CHECK_CONDITION_ALWAYS_MATH_TEST(tanEqual, Utility::Logging::ERR,
			"The first approximate tangent of an angle ", m_angle, " equals ", tanValue, " while it was expected to be ", m_expectedTrigonometryOperatorValue, ".");

		tanValue = m_angle.FastTan2();
		tanEqual = abs(tanValue - m_expectedTrigonometryOperatorValue) < DIFF_THRESHOLD;
		CHECK_CONDITION_ALWAYS_MATH_TEST(tanEqual, Utility::Logging::ERR,
			"The second approximate tangent of an angle ", m_angle, " equals ", tanValue, " while it was expected to be ", m_expectedTrigonometryOperatorValue, ".");
	}
}

void AngleTestTan::StartTimeTest(unsigned int iterationsCount)
{
	for (unsigned int i = 0; i < iterationsCount; ++i)
	{
		m_angle.Tan();
	}
}
/* ==================== class AngleTestTan end ==================== */

/* ==================== class AngleTestCompare begin ==================== */
AngleTestCompare::AngleTestCompare(const Math::Angle& angle1, const Math::Angle& angle2, bool expectedCompareResult, bool expectedLessResult, bool expectedGreaterResult) :
	AngleTestBase(angle1),
	m_compareAngle(angle2),
	m_expectedCompareResult(expectedCompareResult),
	m_expectedLessResult(expectedLessResult),
	m_expectedGreaterResult(expectedGreaterResult)
{
	CHECK_CONDITION_ALWAYS_MATH_TEST(expectedCompareResult || expectedLessResult || expectedGreaterResult, Utility::Logging::ERR, "Incorrect setup of the test. One of the values for comparing angles must be true.");
	CHECK_CONDITION_ALWAYS_MATH_TEST(!expectedLessResult || !expectedGreaterResult, Utility::Logging::ERR, "Incorrect setup of the test. Any given angle cannot be both smaller and greater than some other angle.");
	if (expectedCompareResult)
	{
		CHECK_CONDITION_ALWAYS_MATH_TEST(!expectedLessResult && !expectedGreaterResult, Utility::Logging::ERR, "Incorrect setup of the test. Expected ==, < and > operators result: ",
			expectedCompareResult, ", ", expectedLessResult, ", ", expectedGreaterResult, " are incorrect.");
	}
	else
	{
		CHECK_CONDITION_ALWAYS_MATH_TEST(expectedLessResult || expectedGreaterResult, Utility::Logging::ERR, "Incorrect setup of the test. Expected ==, < and > operators result: ",
			expectedCompareResult, ", ", expectedLessResult, ", ", expectedGreaterResult, " are incorrect.");
	}
}

AngleTestCompare::~AngleTestCompare()
{
}

void AngleTestCompare::StartTest()
{
	bool compareResult = (m_angle == m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult == m_expectedCompareResult, Utility::Logging::ERR,
		"The angle ", m_angle, " and angle ", m_compareAngle, " comparison result not as expected.");
	bool diffCompareResult = (m_angle != m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(diffCompareResult != m_expectedCompareResult, Utility::Logging::ERR,
		"The angle ", m_angle, " and angle ", m_compareAngle, " diff operator result not as expected.");
	CHECK_CONDITION_ALWAYS_MATH_TEST(diffCompareResult != compareResult, Utility::Logging::ERR,
		"The operators == and != should always give opposite results.");

	bool lessResult = (m_angle < m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(lessResult == m_expectedLessResult, Utility::Logging::ERR,
		"The angle ", m_angle, " was expected to be smaller than angle ", m_compareAngle, " but it is not.");

	bool greaterResult = (m_angle > m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(greaterResult == m_expectedGreaterResult, Utility::Logging::ERR,
		"The angle ", m_angle, " was expected to be greater than angle ", m_compareAngle, " but it is not.");

	CHECK_CONDITION_ALWAYS_MATH_TEST(!lessResult || !greaterResult, Utility::Logging::ERR,
		"The angle ", m_angle, " cannot be both smaller and greater than some other angle at the same time.");
}

void AngleTestCompare::StartTimeTest(unsigned int iterationsCount)
{
	m_timer.Start();
	for (unsigned int i = 0; i < iterationsCount; ++i)
	{
		bool compareResult = (m_angle == m_compareAngle);
	}
	m_timer.Stop();
	DEBUG_LOG_MATH_TEST("Angle operator == takes: ", static_cast<Math::Real>(m_timer.GetDuration(Utility::Timing::NANOSECOND)) / iterationsCount,
		" [ns] on average.");

	m_timer.Start();
	for (unsigned int i = 0; i < iterationsCount; ++i)
	{
		bool diffCompareResult = (m_angle != m_compareAngle);
	}
	m_timer.Stop();
	DEBUG_LOG_MATH_TEST("Angle operator != takes: ", m_timer.GetDuration(Utility::Timing::NANOSECOND) / iterationsCount,
		" [ns] on average.");

	m_timer.Start();
	for (unsigned int i = 0; i < iterationsCount; ++i)
	{
		bool lessResult = (m_angle < m_compareAngle);
	}
	m_timer.Stop();
	DEBUG_LOG_MATH_TEST("Angle operator < takes: ", m_timer.GetDuration(Utility::Timing::NANOSECOND) / iterationsCount,
		" [ns] on average.");

	m_timer.Start();
	for (unsigned int i = 0; i < iterationsCount; ++i)
	{
		bool lessResult = (m_angle > m_compareAngle);
	}
	m_timer.Stop();
	DEBUG_LOG_MATH_TEST("Angle operator > takes: ", static_cast<double>(m_timer.GetDuration(Utility::Timing::NANOSECOND)) / iterationsCount,
		" [ns] on average.");
}
/* ==================== class AngleTestCompare end ==================== */

/* ==================== class AngleTestSum begin ==================== */
AngleTestSum::AngleTestSum(const Math::Angle& angle1, const Math::Angle& angle2, const Math::Angle& expectedAnglesSum) :
	AngleTestOperator(angle1, angle2, expectedAnglesSum)
{
}

AngleTestSum::~AngleTestSum()
{
}

void AngleTestSum::StartTest()
{
	const Math::Angle angleCopy(m_angle);

	const Math::Angle anglesSum = m_angle + m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(anglesSum == m_expectedResult, Utility::Logging::ERR,
		"The sum of angle ", m_angle, " and angle ", m_value, " equals ", anglesSum,
		" while it was expected to equal ", m_expectedResult);
	m_angle += m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_angle == m_expectedResult, Utility::Logging::ERR,
		"The sum of angle ", angleCopy, " and angle ", m_value, " equals ", m_angle,
		" while it was expected to equal ", m_expectedResult);
}

void AngleTestSum::StartTimeTest(unsigned int iterationsCount)
{
	for (unsigned int i = 0; i < iterationsCount; ++i)
	{
		const Math::Angle anglesSum = m_angle + m_value;
	}
}
/* ==================== class AngleTestSum end ==================== */

/* ==================== class AngleTestDiff begin ==================== */
AngleTestDiff::AngleTestDiff(const Math::Angle& angle1, const Math::Angle& angle2, const Math::Angle& expectedAnglesDiff) :
	AngleTestOperator(angle1, angle2, expectedAnglesDiff)
{
}

AngleTestDiff::~AngleTestDiff()
{
}

void AngleTestDiff::StartTest()
{
	const Math::Angle angleCopy(m_angle);

	const Math::Angle anglesDiff = m_angle - m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(anglesDiff == m_expectedResult, Utility::Logging::ERR,
		"The difference between angle ", m_angle, " and angle ", m_value, " equals ", anglesDiff,
		" while it was expected to equal ", m_expectedResult);
	m_angle -= m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_angle == m_expectedResult, Utility::Logging::ERR,
		"The difference between angle ", angleCopy, " and angle ", m_value, " equals ", m_angle,
		" while it was expected to equal ", m_expectedResult);
}

void AngleTestDiff::StartTimeTest(unsigned int iterationsCount)
{
	for (unsigned int i = 0; i < iterationsCount; ++i)
	{
		const Math::Angle anglesDiff = m_angle - m_value;
	}
}
/* ==================== class AngleTestDiff end ==================== */

/* ==================== class AngleTestMultiplyByValue begin ==================== */
AngleTestMultiplyByValue::AngleTestMultiplyByValue(const Math::Angle& angle1, Math::Real multValue, const Math::Angle& expectedMultResult) :
	AngleTestOperator(angle1, multValue, expectedMultResult)
{
}

AngleTestMultiplyByValue::~AngleTestMultiplyByValue()
{
}

void AngleTestMultiplyByValue::StartTest()
{
	const Math::Angle angleCopy(m_angle);
	const Math::Angle angle1TimesValue = m_angle * m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(angle1TimesValue == m_expectedResult, Utility::Logging::ERR,
		"The angle ", m_angle, " multiplied by ", m_value, " equals ", angle1TimesValue,
		" while it was expected to equal ", m_expectedResult);
	m_angle *= m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_angle == m_expectedResult, Utility::Logging::ERR,
		"The angle ", angleCopy, " multiplied by ", m_value, " equals ", m_angle,
		" while it was expected to equal ", m_expectedResult);
}

void AngleTestMultiplyByValue::StartTimeTest(unsigned int iterationsCount)
{
	for (unsigned int i = 0; i < iterationsCount; ++i)
	{
		const Math::Angle anglesDiff = m_angle * m_value;
	}
}
/* ==================== class AngleTestMultiplyByValue end ==================== */

/* ==================== class AngleTestDivideByValue begin ==================== */
AngleTestDivideByValue::AngleTestDivideByValue(const Math::Angle& angle1, Math::Real divValue, const Math::Angle& expectedDivResult) :
	AngleTestOperator(angle1, divValue, expectedDivResult)
{
}

AngleTestDivideByValue::~AngleTestDivideByValue()
{
}

void AngleTestDivideByValue::StartTest()
{
	const Math::Angle angleCopy(m_angle);
	const Math::Angle angle1OverValue = m_angle / m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(angle1OverValue == m_expectedResult, Utility::Logging::ERR,
		"The angle ", m_angle, " divided by ", m_value, " equals ", angle1OverValue,
		" while it was expected to equal ", m_expectedResult);
	m_angle /= m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_angle == m_expectedResult, Utility::Logging::ERR,
		"The angle ", angleCopy, " divided by ", m_value, " equals ", m_angle,
		" while it was expected to equal ", m_expectedResult);
}

void AngleTestDivideByValue::StartTimeTest(unsigned int iterationsCount)
{
	for (unsigned int i = 0; i < iterationsCount; ++i)
	{
		const Math::Angle anglesDiff = m_angle / m_value;
	}
}
/* ==================== class AngleTestDivideByValue end ==================== */