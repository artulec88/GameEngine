#include "AngleTestGroup.h"
#include "Math\FloatingPoint.h"
#include "Def.h"

using namespace MathTest;


/* ==================== class AngleTestGroup begin ==================== */
AngleTestGroup::AngleTestGroup(void) :
	TestGroup()
{
	NOTICE_LOG_MATH_TEST("Angle tests started");
}

AngleTestGroup::~AngleTestGroup(void)
{
	NOTICE_LOG_MATH_TEST("Angle tests finished");
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

/* ==================== class AngleTestCompare begin ==================== */
/* static */ std::vector<Utility::Timing::TimeSpan> AngleTestCompare::s_equalOperatorTimeSpans;
/* static */ std::vector<Utility::Timing::TimeSpan> AngleTestCompare::s_diffOperatorTimeSpans;
/* static */ std::vector<Utility::Timing::TimeSpan> AngleTestCompare::s_lessOperatorTimeSpans;
/* static */ std::vector<Utility::Timing::TimeSpan> AngleTestCompare::s_greaterOperatorTimeSpans;

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
	if (!s_equalOperatorTimeSpans.empty())
	{
		Utility::Timing::TimeSpan minTimeSpan(9999, Utility::Timing::HOUR), maxTimeSpan(0L, Utility::Timing::NANOSECOND);
		Utility::Timing::TimeSpan timeSpanSum(0L, Utility::Timing::NANOSECOND);
		for (std::vector<Utility::Timing::TimeSpan>::const_iterator equalOperatorTimeSpanItr = s_equalOperatorTimeSpans.begin(); equalOperatorTimeSpanItr != s_equalOperatorTimeSpans.end(); ++equalOperatorTimeSpanItr)
		{
			timeSpanSum += *equalOperatorTimeSpanItr;
			if ((*equalOperatorTimeSpanItr) < minTimeSpan)
			{
				minTimeSpan = *equalOperatorTimeSpanItr;
			}
			if ((*equalOperatorTimeSpanItr) > maxTimeSpan)
			{
				maxTimeSpan = (*equalOperatorTimeSpanItr);
			}
			DEBUG_LOG_MATH_TEST("Time span for Angle operator== is: ", (*equalOperatorTimeSpanItr).ToString(Utility::Timing::NANOSECOND));
		}
		INFO_LOG_MATH_TEST("Minimum time for Angle operator== is: ", minTimeSpan.ToString(Utility::Timing::NANOSECOND));
		INFO_LOG_MATH_TEST("Maximum time for Angle operator== is: ", maxTimeSpan.ToString(Utility::Timing::NANOSECOND));
		INFO_LOG_MATH_TEST("Average time for Angle operator== is: ", (timeSpanSum / s_equalOperatorTimeSpans.size()).ToString(Utility::Timing::NANOSECOND));
		s_equalOperatorTimeSpans.clear();
	}
}

void AngleTestCompare::StartTest()
{
	bool compareResult = (m_angle == m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult == m_expectedCompareResult, Utility::Logging::ERR,
		"The angle ", m_angle.ToString(), " and angle ", m_compareAngle.ToString(), " comparison result not as expected.");
	bool diffCompareResult = (m_angle != m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(diffCompareResult != m_expectedCompareResult, Utility::Logging::ERR,
		"The angle ", m_angle.ToString() , " and angle ", m_compareAngle.ToString(), " diff operator result not as expected.");
	CHECK_CONDITION_ALWAYS_MATH_TEST(diffCompareResult != compareResult, Utility::Logging::ERR,
		"The operators == and != should always give opposite results.");

	bool lessResult = (m_angle < m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(lessResult == m_expectedLessResult, Utility::Logging::ERR,
		"The angle ", m_angle.ToString(), " was expected to be smaller than angle ", m_compareAngle.ToString(), " but it is not.");

	bool greaterResult = (m_angle > m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(greaterResult == m_expectedGreaterResult, Utility::Logging::ERR,
		"The angle ", m_angle.ToString(), " was expected to be greater than angle ", m_compareAngle.ToString(), " but it is not.");

	CHECK_CONDITION_ALWAYS_MATH_TEST(!lessResult || !greaterResult, Utility::Logging::ERR,
		"The angle ", m_angle.ToString(), " cannot be both smaller and greater than some other angle at the same time.");
}

void AngleTestCompare::StartTimeTest()
{
	m_timer.Start();
	const unsigned int ITERATIONS = 10000;
	for (unsigned int i = 0; i < ITERATIONS; ++i)
	{
		bool compareResult = (m_angle == m_compareAngle);
	}
	m_timer.Stop();
	s_equalOperatorTimeSpans.push_back(m_timer.GetTimeSpan() / ITERATIONS);
	DELOCUST_LOG_MATH_TEST("Angle operator == takes: ", m_timer.GetDuration(Utility::Timing::NANOSECOND) / ITERATIONS, " [ns].");

	m_timer.Start();
	for (unsigned int i = 0; i < ITERATIONS; ++i)
	{
		bool diffCompareResult = (m_angle != m_compareAngle);
	}
	m_timer.Stop();
	s_diffOperatorTimeSpans.push_back(m_timer.GetTimeSpan() / ITERATIONS);
	DELOCUST_LOG_MATH_TEST("Angle operator != takes: ", m_timer.GetDuration(Utility::Timing::NANOSECOND) / ITERATIONS, " [ns].");

	m_timer.Start();
	for (unsigned int i = 0; i < ITERATIONS; ++i)
	{
		bool lessResult = (m_angle < m_compareAngle);
	}
	m_timer.Stop();
	s_lessOperatorTimeSpans.push_back(m_timer.GetTimeSpan() / ITERATIONS);
	DELOCUST_LOG_MATH_TEST("Angle operator < takes: ", m_timer.GetDuration(Utility::Timing::NANOSECOND) / ITERATIONS, " [ns].");

	m_timer.Start();
	for (unsigned int i = 0; i < ITERATIONS; ++i)
	{
		bool lessResult = (m_angle > m_compareAngle);
	}
	m_timer.Stop();
	s_greaterOperatorTimeSpans.push_back(m_timer.GetTimeSpan() / ITERATIONS);
	DELOCUST_LOG_MATH_TEST("Angle operator > takes: ", static_cast<double>(m_timer.GetDuration(Utility::Timing::NANOSECOND)) / ITERATIONS, " [ns].");
}
/* ==================== class AngleTestCompare end ==================== */

/* ==================== class AngleTestCompare begin ==================== */
AngleTestTrigonometry::AngleTestTrigonometry(const Math::Angle& angle1, Math::Real expectedSinValue, Math::Real expectedCosValue, Math::Real expectedTanValue) :
	AngleTestBase(angle1),
	m_expectedSinValue(expectedSinValue),
	m_expectedCosValue(expectedCosValue),
	m_expectedTanValue(expectedTanValue)
{
}

AngleTestTrigonometry::~AngleTestTrigonometry()
{
}

void AngleTestTrigonometry::StartTest()
{
	const Math::Real DIFF_THRESHOLD = 0.002f;

	Math::Real sinValue = m_angle.Sin();
	bool sinEqual = Math::AlmostEqual(sinValue, m_expectedSinValue);
	CHECK_CONDITION_ALWAYS_MATH_TEST(sinEqual, Utility::Logging::ERR,
		"The sinus of an angle ", m_angle.ToString(), " equals ", sinValue, " while it was expected to be ", m_expectedSinValue, ".");

	sinValue = m_angle.FastSin1();
	sinEqual = abs(sinValue - m_expectedSinValue) < DIFF_THRESHOLD;
	CHECK_CONDITION_ALWAYS_MATH_TEST(sinEqual, Utility::Logging::ERR,
		"The first approximate sinus of an angle ", m_angle.ToString(), " equals ", sinValue, " while it was expected to be ", m_expectedSinValue, ".");

	sinValue = m_angle.FastSin2();
	sinEqual = abs(sinValue - m_expectedSinValue) < DIFF_THRESHOLD;
	CHECK_CONDITION_ALWAYS_MATH_TEST(sinEqual, Utility::Logging::ERR,
		"The second approximate sinus of an angle ", m_angle.ToString(), " equals ", sinValue, " while it was expected to be ", m_expectedSinValue, ".");

	Math::Real cosValue = m_angle.Cos();
	bool cosEqual = Math::AlmostEqual(cosValue, m_expectedCosValue);
	CHECK_CONDITION_ALWAYS_MATH_TEST(cosEqual, Utility::Logging::ERR,
		"The cosinus of an angle ", m_angle.ToString(), " equals ", cosValue, " while it was expected to be ", m_expectedCosValue, ".");

	cosValue = m_angle.FastCos1();
	cosEqual = abs(cosValue - m_expectedCosValue) < DIFF_THRESHOLD;
	CHECK_CONDITION_ALWAYS_MATH_TEST(cosEqual, Utility::Logging::ERR,
		"The first approximate cosinus of an angle ", m_angle.ToString(), " equals ", cosValue, " while it was expected to be ", m_expectedCosValue, ".");

	cosValue = m_angle.FastCos2();
	cosEqual = abs(cosValue - m_expectedCosValue) < DIFF_THRESHOLD;
	CHECK_CONDITION_ALWAYS_MATH_TEST(cosEqual, Utility::Logging::ERR,
		"The second approximate cosinus of an angle ", m_angle.ToString(), " equals ", cosValue, " while it was expected to be ", m_expectedCosValue, ".");

	if (!Math::AlmostEqual(cosValue, REAL_ZERO))
	{
		Math::Real tanValue = m_angle.Tan();
		bool tanEqual = Math::AlmostEqual(tanValue, m_expectedTanValue);
		CHECK_CONDITION_ALWAYS_MATH_TEST(tanEqual, Utility::Logging::ERR,
			"The tangent of an angle ", m_angle.ToString(), " equals ", tanValue, " while it was expected to be ", m_expectedTanValue, ".");

		tanValue = m_angle.FastTan1();
		tanEqual = abs(tanValue - m_expectedTanValue) < DIFF_THRESHOLD;
		CHECK_CONDITION_ALWAYS_MATH_TEST(tanEqual, Utility::Logging::ERR,
			"The first approximate tangent of an angle ", m_angle.ToString(), " equals ", tanValue, " while it was expected to be ", m_expectedTanValue, ".");

		tanValue = m_angle.FastTan2();
		tanEqual = abs(tanValue - m_expectedTanValue) < DIFF_THRESHOLD;
		CHECK_CONDITION_ALWAYS_MATH_TEST(tanEqual, Utility::Logging::ERR,
			"The second approximate tangent of an angle ", m_angle.ToString(), " equals ", tanValue, " while it was expected to be ", m_expectedTanValue, ".");
	}
}

void AngleTestTrigonometry::StartTimeTest()
{

}
/* ==================== class AngleTestTrigonometry end ==================== */

/* ==================== class AngleTestOperators begin ==================== */
AngleTestOperators::AngleTestOperators(const Math::Angle& angle1, const Math::Angle& angle2, Math::Real value, Math::Angle& expectedAnglesSum, const Math::Angle& expectedAnglesDiff,
	const Math::Angle& expectedAngle1TimesValue, const Math::Angle& expectedAngle1OverValue, const Math::Angle& expectedAngle2TimesValue, const Math::Angle& expectedAngle2OverValue) :
	AngleTestBase(angle1),
	m_angle2(angle2),
	m_value(value),
	m_expectedAnglesSum(expectedAnglesSum),
	m_expectedAnglesDiff(expectedAnglesDiff),
	m_expectedAngle1TimesValue(expectedAngle1TimesValue),
	m_expectedAngle1OverValue(expectedAngle1OverValue),
	m_expectedAngle2TimesValue(expectedAngle2TimesValue),
	m_expectedAngle2OverValue(expectedAngle2OverValue)
{
	CHECK_CONDITION_ALWAYS_MATH_TEST(!Math::AlmostEqual(m_value, REAL_ZERO), Utility::Logging::ERR,
		"Incorrect setup of the test. The value by which we want to multiply and divide angles equals 0.");
}

AngleTestOperators::~AngleTestOperators()
{
}

void AngleTestOperators::StartTest()
{
	const Math::Angle angleCopy(m_angle);
	const Math::Angle angle2Copy(m_angle2);

	const Math::Angle anglesSum = m_angle + m_angle2;
	CHECK_CONDITION_ALWAYS_MATH_TEST(anglesSum == m_expectedAnglesSum, Utility::Logging::ERR,
		"The sum of angle ", m_angle.ToString(), " and angle ", m_angle2.ToString(), " equals ", anglesSum.ToString(),
		" while it was expected to equal ", m_expectedAnglesSum.ToString());
	m_angle += m_angle2;
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_angle == m_expectedAnglesSum, Utility::Logging::ERR,
		"The sum of angle ", angleCopy.ToString(), " and angle ", m_angle2.ToString(), " equals ", m_angle.ToString(),
		" while it was expected to equal ", m_expectedAnglesSum.ToString());
	m_angle = angleCopy;

	const Math::Angle anglesDiff = m_angle - m_angle2;
	CHECK_CONDITION_ALWAYS_MATH_TEST(anglesDiff == m_expectedAnglesDiff, Utility::Logging::ERR,
		"The difference between angle ", m_angle.ToString(), " and angle ", m_angle2.ToString(), " equals ", anglesDiff.ToString(),
		" while it was expected to equal ", m_expectedAnglesDiff.ToString());
	m_angle -= m_angle2;
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_angle == m_expectedAnglesDiff, Utility::Logging::ERR,
		"The difference between angle ", angleCopy.ToString(), " and angle ", m_angle2.ToString(), " equals ", m_angle.ToString(),
		" while it was expected to equal ", m_expectedAnglesDiff.ToString());
	m_angle = angleCopy;

	const Math::Angle angle1TimesValue = m_angle * m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(angle1TimesValue == m_expectedAngle1TimesValue, Utility::Logging::ERR,
		"The angle ", m_angle.ToString(), " multiplied by ", m_value, " equals ", angle1TimesValue.ToString(),
		" while it was expected to equal ", m_expectedAngle1TimesValue.ToString());
	m_angle *= m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_angle == m_expectedAngle1TimesValue, Utility::Logging::ERR,
		"The angle ", angleCopy.ToString(), " multiplied by ", m_value, " equals ", m_angle.ToString(),
		" while it was expected to equal ", m_expectedAngle1TimesValue.ToString());
	m_angle = angleCopy;

	const Math::Angle angle1OverValue = m_angle / m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(angle1OverValue == m_expectedAngle1OverValue, Utility::Logging::ERR,
		"The angle ", m_angle.ToString(), " divided by ", m_value, " equals ", angle1OverValue.ToString(),
		" while it was expected to equal ", m_expectedAngle1OverValue.ToString());
	m_angle /= m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_angle == m_expectedAngle1OverValue, Utility::Logging::ERR,
		"The angle ", angleCopy.ToString(), " divided by ", m_value, " equals ", m_angle.ToString(),
		" while it was expected to equal ", m_expectedAngle1OverValue.ToString());
	m_angle = angleCopy;

	const Math::Angle angle2TimesValue = m_angle2 * m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(angle2TimesValue == m_expectedAngle2TimesValue, Utility::Logging::ERR,
		"The angle ", m_angle2.ToString(), " multiplied by ", m_value, " equals ", angle2TimesValue.ToString(),
		" while it was expected to equal ", m_expectedAngle2TimesValue.ToString());
	m_angle2 *= m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_angle2 == m_expectedAngle2TimesValue, Utility::Logging::ERR,
		"The angle ", angle2Copy.ToString(), " multiplied by ", m_value, " equals ", m_angle2.ToString(),
		" while it was expected to equal ", m_expectedAngle2TimesValue.ToString());
	m_angle2 = angle2Copy;

	const Math::Angle angle2OverValue = m_angle2 / m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(angle2OverValue == m_expectedAngle2OverValue, Utility::Logging::ERR,
		"The angle ", m_angle2.ToString(), " divided by ", m_value, " equals ", angle2OverValue.ToString(),
		" while it was expected to equal ", m_expectedAngle2OverValue.ToString());
	m_angle2 /= m_value;
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_angle2 == m_expectedAngle2OverValue, Utility::Logging::ERR,
		"The angle ", angle2Copy.ToString(), " divided by ", m_value, " equals ", m_angle2.ToString(),
		" while it was expected to equal ", m_expectedAngle2OverValue.ToString());
	m_angle2 = angle2Copy;
}

void AngleTestOperators::StartTimeTest()
{
	const unsigned int ITERATIONS = 100000;
	for (unsigned int i = 0; i < ITERATIONS; ++i)
	{
		const Math::Angle anglesSum = m_angle + m_angle2;
	}
}
/* ==================== class AngleTestOperators end ==================== */