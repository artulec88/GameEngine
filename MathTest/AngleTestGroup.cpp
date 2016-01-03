#include "AngleTestGroup.h"
#include "Math\FloatingPoint.h"

using namespace MathTest;


/* ==================== class AngleTestGroup begin ==================== */
AngleTestGroup::AngleTestGroup(void) :
	TestGroup()
{
	NOTICE_LOG("Angle tests started");
}

AngleTestGroup::~AngleTestGroup(void)
{
	NOTICE_LOG("Angle tests finished");
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
	CHECK_CONDITION_ALWAYS(expectedCompareResult || expectedLessResult || expectedGreaterResult, Utility::Error, "Incorrect setup of the test. One of the values for comparing angles must be true.");
	CHECK_CONDITION_ALWAYS(!expectedLessResult || !expectedGreaterResult, Utility::Error, "Incorrect setup of the test. Any given angle cannot be both smaller and greater than some other angle.");
	if (expectedCompareResult)
	{
		CHECK_CONDITION_ALWAYS(!expectedLessResult && !expectedGreaterResult, Utility::Error, "Incorrect setup of the test. Expected ==, < and > operators result: %d, %d, %d are incorrect.",
			expectedCompareResult, expectedLessResult, expectedGreaterResult);
	}
	else
	{
		CHECK_CONDITION_ALWAYS(expectedLessResult || expectedGreaterResult, Utility::Error, "Incorrect setup of the test. Expected ==, < and > operators result: %d, %d, %d are incorrect.",
			expectedCompareResult, expectedLessResult, expectedGreaterResult);
	}
}

AngleTestCompare::~AngleTestCompare()
{
}

void AngleTestCompare::StartTest()
{
	bool compareResult = (m_angle == m_compareAngle);
	CHECK_CONDITION_ALWAYS(compareResult == m_expectedCompareResult, Utility::Error,
		"The angle %s and angle %s comparison result not as expected.",
		m_angle.ToString().c_str(), m_compareAngle.ToString().c_str());
	bool diffCompareResult = (m_angle != m_compareAngle);
	CHECK_CONDITION_ALWAYS(diffCompareResult != m_expectedCompareResult, Utility::Error,
		"The angle %s and angle %s diff operator result not as expected.",
		m_angle.ToString().c_str(), m_compareAngle.ToString().c_str());
	CHECK_CONDITION_ALWAYS(diffCompareResult != compareResult, Utility::Error,
		"The operators == and != should always give opposite results.");

	bool lessResult = (m_angle < m_compareAngle);
	CHECK_CONDITION_ALWAYS(lessResult == m_expectedLessResult, Utility::Error,
		"The angle %s was expected to be smaller than angle %s but it is not.",
		m_angle.ToString().c_str(), m_compareAngle.ToString().c_str());

	bool greaterResult = (m_angle > m_compareAngle);
	CHECK_CONDITION_ALWAYS(greaterResult == m_expectedGreaterResult, Utility::Error,
		"The angle %s was expected to be greater than angle %s but it is not.",
		m_angle.ToString().c_str(), m_compareAngle.ToString().c_str());

	CHECK_CONDITION_ALWAYS(!lessResult || !greaterResult, Utility::Error,
		"The angle %s cannot be both smaller and greater than some other angle at the same time.");
}
/* ==================== class AngleTestCompare end ==================== */