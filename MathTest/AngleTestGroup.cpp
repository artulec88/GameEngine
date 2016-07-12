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
		CHECK_CONDITION_ALWAYS_MATH_TEST(!expectedLessResult && !expectedGreaterResult, Utility::Logging::ERR, "Incorrect setup of the test. Expected ==, < and > operators result: %d, %d, %d are incorrect.",
			expectedCompareResult, expectedLessResult, expectedGreaterResult);
	}
	else
	{
		CHECK_CONDITION_ALWAYS_MATH_TEST(expectedLessResult || expectedGreaterResult, Utility::Logging::ERR, "Incorrect setup of the test. Expected ==, < and > operators result: %d, %d, %d are incorrect.",
			expectedCompareResult, expectedLessResult, expectedGreaterResult);
	}
}

AngleTestCompare::~AngleTestCompare()
{
}

void AngleTestCompare::StartTest()
{
	bool compareResult = (m_angle == m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult == m_expectedCompareResult, Utility::Logging::ERR,
		"The angle %s and angle %s comparison result not as expected.",
		m_angle.ToString().c_str(), m_compareAngle.ToString().c_str());
	bool diffCompareResult = (m_angle != m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(diffCompareResult != m_expectedCompareResult, Utility::Logging::ERR,
		"The angle %s and angle %s diff operator result not as expected.",
		m_angle.ToString().c_str(), m_compareAngle.ToString().c_str());
	CHECK_CONDITION_ALWAYS_MATH_TEST(diffCompareResult != compareResult, Utility::Logging::ERR,
		"The operators == and != should always give opposite results.");

	bool lessResult = (m_angle < m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(lessResult == m_expectedLessResult, Utility::Logging::ERR,
		"The angle %s was expected to be smaller than angle %s but it is not.",
		m_angle.ToString().c_str(), m_compareAngle.ToString().c_str());

	bool greaterResult = (m_angle > m_compareAngle);
	CHECK_CONDITION_ALWAYS_MATH_TEST(greaterResult == m_expectedGreaterResult, Utility::Logging::ERR,
		"The angle %s was expected to be greater than angle %s but it is not.",
		m_angle.ToString().c_str(), m_compareAngle.ToString().c_str());

	CHECK_CONDITION_ALWAYS_MATH_TEST(!lessResult || !greaterResult, Utility::Logging::ERR,
		"The angle %s cannot be both smaller and greater than some other angle at the same time.");
}
/* ==================== class AngleTestCompare end ==================== */