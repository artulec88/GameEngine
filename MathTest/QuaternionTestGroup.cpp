#include "QuaternionTestGroup.h"
#include "Math\FloatingPoint.h"

using namespace MathTest;

/* ==================== class QuaternionTestGroup begin ==================== */
QuaternionTestGroup::QuaternionTestGroup(void) :
	TestGroup()
{
	NOTICE_LOG("Quaternion test started");
}


QuaternionTestGroup::~QuaternionTestGroup(void)
{
	NOTICE_LOG("Quaternion test finished");
}

void QuaternionTestGroup::StartTests()
{
	if (!IsTestGroupEnabled())
	{
		return;
	}
}
/* ==================== class QuaternionTestGroup end ==================== */


/* ==================== class QuaternionTestBase begin ==================== */
QuaternionTestBase::QuaternionTestBase(const Math::Quaternion& quaternion) :
	Test(),
	m_quaternion(quaternion)
{
}

QuaternionTestBase::~QuaternionTestBase(void)
{
}
/* ==================== class Quaternion2DTestBase end ==================== */


/* ==================== class QuaternionTestCompare begin ==================== */
QuaternionTestCompare::QuaternionTestCompare(const Math::Quaternion& quaternion1, const Math::Quaternion& quaternion2, bool expectedCompareResult) :
	QuaternionTestBase(quaternion1),
	m_compareQuaternion(quaternion2),
	m_expectedCompareResult(expectedCompareResult)
{
}

QuaternionTestCompare::~QuaternionTestCompare(void)
{
}

void QuaternionTestCompare::StartTest()
{
	bool compareResult = (m_quaternion == m_compareQuaternion);
	CHECK_CONDITION_ALWAYS(compareResult == m_expectedCompareResult, Utility::Error,
		"The quaternion %s and quaternion %s comparison result is not as expected.",
		m_quaternion.ToString().c_str(), m_compareQuaternion.ToString().c_str());
}
/* ==================== class QuaternionTestCompare end ==================== */


/* ==================== class QuaternionTestLength begin ==================== */
QuaternionTestLength::QuaternionTestLength(const Math::Quaternion& quaternion, Math::Real expectedLength) :
	QuaternionTestBase(quaternion),
	m_expectedLength(expectedLength)
{
}

QuaternionTestLength::~QuaternionTestLength(void)
{
}

void QuaternionTestLength::StartTest()
{
	Math::Real lengthSquared = m_quaternion.LengthSquared();
	Math::Real length = m_quaternion.Length();
	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(length, m_expectedLength), Utility::Error,
		"The quaternion %s has length %.2f, but was expected to have length %.2f",
		m_quaternion.ToString().c_str(), length, m_expectedLength);
	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(lengthSquared, m_expectedLength * m_expectedLength), Utility::Error,
		"The quaternion %s has length %.2f and squared length %.2f, but expected to have squared length %.2f",
		m_quaternion.ToString().c_str(), lengthSquared, m_expectedLength * m_expectedLength);
}
/* ==================== class QuaternionTestLength end ==================== */