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

void AngleTestGroup::StartTests()
{
	if (!IsTestGroupEnabled())
	{
		return;
	}
	TestGroup::StartTests();
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
	Math::Real sinValue = m_angle.Sin();
	bool sinEqual = Math::AlmostEqual(sinValue, m_expectedSinValue);
	CHECK_CONDITION_ALWAYS_MATH_TEST(sinEqual, Utility::Logging::ERR,
		"The sinus of an angle ", m_angle.ToString(), " equals ", sinValue, " while it was expected to be ", m_expectedSinValue, ".");

	Math::Real cosValue = m_angle.Cos();
	bool cosEqual = Math::AlmostEqual(cosValue, m_expectedCosValue);
	CHECK_CONDITION_ALWAYS_MATH_TEST(cosEqual, Utility::Logging::ERR,
		"The cosinus of an angle ", m_angle.ToString(), " equals ", cosValue, " while it was expected to be ", m_expectedCosValue, ".");

	if (!Math::AlmostEqual(cosValue, REAL_ZERO))
	{
		Math::Real tanValue = m_angle.Tan();
		bool tanEqual = Math::AlmostEqual(tanValue, m_expectedTanValue);
		CHECK_CONDITION_ALWAYS_MATH_TEST(tanEqual, Utility::Logging::ERR,
			"The tangent of an angle ", m_angle.ToString(), " equals ", tanValue, " while it was expected to be ", m_expectedTanValue, ".");
	}
}
/* ==================== class AngleTestCompare end ==================== */