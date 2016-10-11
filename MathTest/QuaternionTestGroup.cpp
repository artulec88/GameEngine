#include "QuaternionTestGroup.h"
#include "Math\FloatingPoint.h"
#include "Def.h"

using namespace MathTest;

/* ==================== class QuaternionTestGroup begin ==================== */
QuaternionTestGroup::QuaternionTestGroup(void) :
	TestGroup()
{
	NOTICE_LOG_MATH_TEST("Quaternion test started");
}


QuaternionTestGroup::~QuaternionTestGroup(void)
{
	NOTICE_LOG_MATH_TEST("Quaternion test finished");
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
/* ==================== class QuaternionTestBase end ==================== */


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
	CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult == m_expectedCompareResult, Utility::Logging::ERR,
		"The quaternion ", m_quaternion, " and quaternion ", m_compareQuaternion, " comparison result is not as expected.");
}

void QuaternionTestCompare::StartTimeTest()
{
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
	CHECK_CONDITION_ALWAYS_MATH_TEST(Math::AlmostEqual(length, m_expectedLength), Utility::Logging::ERR,
		"The quaternion ", m_quaternion, " has length ", length, ", but was expected to have length ", m_expectedLength);
	CHECK_CONDITION_ALWAYS_MATH_TEST(Math::AlmostEqual(lengthSquared, m_expectedLength * m_expectedLength), Utility::Logging::ERR,
		"The quaternion ", m_quaternion, " has squared length ", lengthSquared, ", but expected to have squared length ",
		m_expectedLength * m_expectedLength);
}

void QuaternionTestLength::StartTimeTest()
{
}
/* ==================== class QuaternionTestLength end ==================== */


/* ==================== class QuaternionTestConjugate begin ==================== */
QuaternionTestConjugate::QuaternionTestConjugate(const Math::Quaternion& quaternion, const Math::Quaternion& expectedConjugate) :
	QuaternionTestBase(quaternion),
	m_expectedConjugate(expectedConjugate)
{
}

QuaternionTestConjugate::~QuaternionTestConjugate(void)
{
}

void QuaternionTestConjugate::StartTest()
{
	Math::Quaternion conjugate = m_quaternion.Conjugate();
	CHECK_CONDITION_ALWAYS_MATH_TEST(conjugate == m_expectedConjugate, Utility::Logging::ERR,
		"The quaternion ", m_quaternion, " has conjugate ", conjugate, ". It is different than expected ", m_expectedConjugate);
	CHECK_CONDITION_ALWAYS_MATH_TEST(conjugate.Conjugate() == m_quaternion, Utility::Logging::ERR,
		"The conjugate of a quaternion is supposed to be an involution, but the conjugate of conjugate of a quaternion (", conjugate.Conjugate(),
		") is not an original quaternion (", m_quaternion, ").");
}

void QuaternionTestConjugate::StartTimeTest()
{
}
/* ==================== class QuaternionTestConjugate end ==================== */


/* ==================== class QuaternionTestAddOperator begin ==================== */
QuaternionTestAddOperator::QuaternionTestAddOperator(const Math::Quaternion& quaternion, const Math::Quaternion& quaternion2, const Math::Quaternion& expectedSumQuaternion) :
	QuaternionTestBase(quaternion),
	m_quaternion2(quaternion2),
	m_expectedSumQuaternion(expectedSumQuaternion)
{
}

QuaternionTestAddOperator::~QuaternionTestAddOperator(void)
{
}

void QuaternionTestAddOperator::StartTest()
{
	Math::Quaternion sumQuaternion = m_quaternion + m_quaternion2;
	CHECK_CONDITION_ALWAYS_MATH_TEST(sumQuaternion == m_expectedSumQuaternion, Utility::Logging::ERR,
		"The sum of quaternions ", m_quaternion, " and ", m_quaternion2, " is a quaternion ",
		sumQuaternion, ". It is different than expected ", m_expectedSumQuaternion);
}

void QuaternionTestAddOperator::StartTimeTest()
{
}
/* ==================== class QuaternionTestAddOperator end ==================== */


/* ==================== class QuaternionTestSubtractOperator begin ==================== */
QuaternionTestSubtractOperator::QuaternionTestSubtractOperator(const Math::Quaternion& quaternion, const Math::Quaternion& quaternion2, const Math::Quaternion& expectedSubtractQuaternion) :
	QuaternionTestBase(quaternion),
	m_quaternion2(quaternion2),
	m_expectedSubtractQuaternion(expectedSubtractQuaternion)
{
}

QuaternionTestSubtractOperator::~QuaternionTestSubtractOperator(void)
{
}

void QuaternionTestSubtractOperator::StartTest()
{
	Math::Quaternion subtractQuaternion = m_quaternion - m_quaternion2;
	CHECK_CONDITION_ALWAYS_MATH_TEST(subtractQuaternion == m_expectedSubtractQuaternion, Utility::Logging::ERR,
		"The subtraction of quaternions ", m_quaternion, " and ", m_quaternion2, " is a quaternion ",
		subtractQuaternion, ". It is different than expected ", m_expectedSubtractQuaternion);
}

void QuaternionTestSubtractOperator::StartTimeTest()
{
}
/* ==================== class QuaternionTestSubtractOperator end ==================== */


/* ==================== class QuaternionTestDivideOperator begin ==================== */
QuaternionTestDivideOperator::QuaternionTestDivideOperator(const Math::Quaternion& quaternion, Math::Real divideValue, const Math::Quaternion& expectedDivideQuaternion) :
	QuaternionTestBase(quaternion),
	m_divideValue(divideValue),
	m_expectedDivideQuaternion(expectedDivideQuaternion)
{
}

QuaternionTestDivideOperator::~QuaternionTestDivideOperator(void)
{
}

void QuaternionTestDivideOperator::StartTest()
{
	Math::Quaternion divideQuaternion = m_quaternion / m_divideValue;
	CHECK_CONDITION_ALWAYS_MATH_TEST(divideQuaternion == m_expectedDivideQuaternion, Utility::Logging::ERR,
		"The division of quaternion ", m_quaternion, " by value ", m_divideValue, " is a quaternion ",
		divideQuaternion, ". It is different than expected ", m_expectedDivideQuaternion);
}

void QuaternionTestDivideOperator::StartTimeTest()
{
}
/* ==================== class QuaternionTestDivideOperator end ==================== */


/* ==================== class QuaternionTestNormalize begin ==================== */
QuaternionTestNormalize::QuaternionTestNormalize(const Math::Quaternion& quaternion, const Math::Quaternion& expectedNormalizedQuaternion) :
	QuaternionTestBase(quaternion),
	m_expectedNormalizedQuaternion(expectedNormalizedQuaternion)
{
	Math::Real lengthSquared = m_expectedNormalizedQuaternion.LengthSquared();
	Math::Real length = m_expectedNormalizedQuaternion.Length();
	CHECK_CONDITION_ALWAYS_MATH_TEST(Math::AlmostEqual(lengthSquared, REAL_ONE), Utility::Logging::ERR,
		"Given expected normalized quaternion ", m_expectedNormalizedQuaternion, " is in fact not normalized.");
	CHECK_CONDITION_ALWAYS_MATH_TEST(Math::AlmostEqual(length, lengthSquared), Utility::Logging::ERR,
		"Given expected normalized quaternion ", m_expectedNormalizedQuaternion, " gives different results for length and squared length (", length, " and ", lengthSquared, " respectively).");
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_expectedNormalizedQuaternion.IsNormalized(), Utility::Logging::ERR,
		"Given expected normalized quaternion ", m_expectedNormalizedQuaternion, " is in fact not normalized.");
}

QuaternionTestNormalize::~QuaternionTestNormalize(void)
{
}

void QuaternionTestNormalize::StartTest()
{
	Math::Quaternion normalizedQuaternion = m_quaternion.Normalized();
	CHECK_CONDITION_ALWAYS_MATH_TEST(normalizedQuaternion == m_expectedNormalizedQuaternion, Utility::Logging::ERR,
		"The quaternion ", m_quaternion, " after normalization equals ", normalizedQuaternion, ". It is different than expected ",
		m_expectedNormalizedQuaternion, ".");
	CHECK_CONDITION_ALWAYS_MATH_TEST(normalizedQuaternion.IsNormalized(), Utility::Logging::ERR,
		"Calculated normalized quaternion ", normalizedQuaternion, " is in fact not normalized.");

	m_quaternion.Normalize();
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_quaternion == m_expectedNormalizedQuaternion, Utility::Logging::ERR,
		"The quaternion after normalization ", m_quaternion, " is different than expected ", m_expectedNormalizedQuaternion);
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_quaternion.IsNormalized(), Utility::Logging::ERR,
		"Calculated normalized quaternion ", m_quaternion, " is in fact not normalized.");
}

void QuaternionTestNormalize::StartTimeTest()
{
}
/* ==================== class QuaternionTestNormalize end ==================== */


/* ==================== class QuaternionTestDot begin ==================== */
QuaternionTestDot::QuaternionTestDot(const Math::Quaternion& quaternion, const Math::Quaternion& quaternion2, Math::Real expectedDotResult) :
	QuaternionTestBase(quaternion),
	m_quaternion2(quaternion2),
	m_expectedDotResult(expectedDotResult)
{
}

QuaternionTestDot::~QuaternionTestDot(void)
{
}

void QuaternionTestDot::StartTest()
{
	Math::Real dotResult = m_quaternion.Dot(m_quaternion2);
	CHECK_CONDITION_ALWAYS_MATH_TEST(Math::AlmostEqual(dotResult, m_expectedDotResult), Utility::Logging::ERR,
		"The dot product of quaternions ", m_quaternion, " and ", m_quaternion2, " equals ", dotResult, ". It is different than expected ", m_expectedDotResult);
}

void QuaternionTestDot::StartTimeTest()
{
}
/* ==================== class QuaternionTestDot end ==================== */


/* ==================== class QuaternionTestNlerp begin ==================== */
QuaternionTestNlerp::QuaternionTestNlerp(const Math::Quaternion& quaternion, const Math::Quaternion& quaternion2, Math::Real nlerpFactor, bool shortest, const Math::Quaternion& expectedNlerpQuaternion) :
	QuaternionTestBase(quaternion),
	m_quaternion2(quaternion2),
	m_nlerpFactor(nlerpFactor),
	m_shortest(shortest),
	m_expectedNlerpQuaternion(expectedNlerpQuaternion)
{
}

QuaternionTestNlerp::~QuaternionTestNlerp(void)
{
}

void QuaternionTestNlerp::StartTest()
{
	Math::Quaternion nlerpQuaternion = m_quaternion.Nlerp(m_quaternion2, m_nlerpFactor, m_shortest);
	CHECK_CONDITION_ALWAYS_MATH_TEST(nlerpQuaternion == m_expectedNlerpQuaternion, Utility::Logging::ERR,
		"The linear interpolation of quaternions ", m_quaternion, " and ", m_quaternion2, " with the factor equal to ",
		m_nlerpFactor, " is a quaternion ", nlerpQuaternion, ". It is different than expected ", m_expectedNlerpQuaternion);
}

void QuaternionTestNlerp::StartTimeTest()
{
}
/* ==================== class QuaternionTestNlerp end ==================== */

/* ==================== class QuaternionTestSlerp begin ==================== */
QuaternionTestSlerp::QuaternionTestSlerp(const Math::Quaternion& quaternion, const Math::Quaternion& quaternion2, Math::Real slerpFactor, bool shortest, const Math::Quaternion& expectedSlerpQuaternion) :
	QuaternionTestBase(quaternion),
	m_quaternion2(quaternion2),
	m_slerpFactor(slerpFactor),
	m_shortest(shortest),
	m_expectedSlerpQuaternion(expectedSlerpQuaternion)
{
}

QuaternionTestSlerp::~QuaternionTestSlerp(void)
{
}

void QuaternionTestSlerp::StartTest()
{
	Math::Quaternion slerpQuaternion = m_quaternion.Slerp(m_quaternion2, m_slerpFactor, m_shortest);
	CHECK_CONDITION_ALWAYS_MATH_TEST(slerpQuaternion == m_expectedSlerpQuaternion, Utility::Logging::ERR,
		"The spherical linear interpolation of quaternions ", m_quaternion, " and ", m_quaternion2, " with the factor equal to ",
		m_slerpFactor, " is a quaternion ", slerpQuaternion, ". It is different than expected ", m_expectedSlerpQuaternion);
}

void QuaternionTestSlerp::StartTimeTest()
{
}
/* ==================== class QuaternionTestSlerp end ==================== */

/* ==================== class QuaternionTestForward begin ==================== */
QuaternionTestForward::QuaternionTestForward(const Math::Quaternion& quaternion, const Math::Vector3D& expectedForwardVector) :
	QuaternionTestBase(quaternion),
	m_expectedForwardVector(expectedForwardVector)
{
}

QuaternionTestForward::~QuaternionTestForward(void)
{
}

void QuaternionTestForward::StartTest()
{
	Math::Vector3D forwardVector = m_quaternion.GetForward();
	CHECK_CONDITION_ALWAYS_MATH_TEST(forwardVector == m_expectedForwardVector, Utility::Logging::ERR,
		"The forward vector for quaternion ", m_quaternion, " is ", forwardVector, ". It is different than expected ", m_expectedForwardVector);
}

void QuaternionTestForward::StartTimeTest()
{
}
/* ==================== class QuaternionTestForward end ==================== */

/* ==================== class QuaternionTestUp begin ==================== */
QuaternionTestUp::QuaternionTestUp(const Math::Quaternion& quaternion, const Math::Vector3D& expectedUpVector) :
	QuaternionTestBase(quaternion),
	m_expectedUpVector(expectedUpVector)
{
}

QuaternionTestUp::~QuaternionTestUp(void)
{
}

void QuaternionTestUp::StartTest()
{
	Math::Vector3D upVector = m_quaternion.GetUp();
	CHECK_CONDITION_ALWAYS_MATH_TEST(upVector == m_expectedUpVector, Utility::Logging::ERR,
		"The up vector for quaternion ", m_quaternion, " is ", upVector, ". It is different than expected ", m_expectedUpVector);
}

void QuaternionTestUp::StartTimeTest()
{
}
/* ==================== class QuaternionTestUp end ==================== */

/* ==================== class QuaternionTestRight begin ==================== */
QuaternionTestRight::QuaternionTestRight(const Math::Quaternion& quaternion, const Math::Vector3D& expectedRightVector) :
	QuaternionTestBase(quaternion),
	m_expectedRightVector(expectedRightVector)
{
}

QuaternionTestRight::~QuaternionTestRight(void)
{
}

void QuaternionTestRight::StartTest()
{
	Math::Vector3D rightVector = m_quaternion.GetRight();
	CHECK_CONDITION_ALWAYS_MATH_TEST(rightVector == m_expectedRightVector, Utility::Logging::ERR,
		"The right vector for quaternion ", m_quaternion, " is ", rightVector, ". It is different than expected ", m_expectedRightVector);
}

void QuaternionTestRight::StartTimeTest()
{
}
/* ==================== class QuaternionTestRight end ==================== */

/* ==================== class QuaternionTestBack begin ==================== */
QuaternionTestBack::QuaternionTestBack(const Math::Quaternion& quaternion, const Math::Vector3D& expectedBackVector) :
	QuaternionTestBase(quaternion),
	m_expectedBackVector(expectedBackVector)
{
}

QuaternionTestBack::~QuaternionTestBack(void)
{
}

void QuaternionTestBack::StartTest()
{
	Math::Vector3D backVector = m_quaternion.GetBack();
	CHECK_CONDITION_ALWAYS_MATH_TEST(backVector == m_expectedBackVector, Utility::Logging::ERR,
		"The back vector for quaternion ", m_quaternion, " is ", backVector, ". It is different than expected ", m_expectedBackVector);
}

void QuaternionTestBack::StartTimeTest()
{
}
/* ==================== class QuaternionTestBack end ==================== */

/* ==================== class QuaternionTestDown begin ==================== */
QuaternionTestDown::QuaternionTestDown(const Math::Quaternion& quaternion, const Math::Vector3D& expectedDownVector) :
	QuaternionTestBase(quaternion),
	m_expectedDownVector(expectedDownVector)
{
}

QuaternionTestDown::~QuaternionTestDown(void)
{
}

void QuaternionTestDown::StartTest()
{
	Math::Vector3D downVector = m_quaternion.GetDown();
	CHECK_CONDITION_ALWAYS_MATH_TEST(downVector == m_expectedDownVector, Utility::Logging::ERR,
		"The down vector for quaternion ", m_quaternion, " is ", downVector, ". It is different than expected ", m_expectedDownVector);
}

void QuaternionTestDown::StartTimeTest()
{
}
/* ==================== class QuaternionTestDown end ==================== */

/* ==================== class QuaternionTestLeft begin ==================== */
QuaternionTestLeft::QuaternionTestLeft(const Math::Quaternion& quaternion, const Math::Vector3D& expectedLeftVector) :
	QuaternionTestBase(quaternion),
	m_expectedLeftVector(expectedLeftVector)
{
}

QuaternionTestLeft::~QuaternionTestLeft(void)
{
}

void QuaternionTestLeft::StartTest()
{
	Math::Vector3D leftVector = m_quaternion.GetLeft();
	CHECK_CONDITION_ALWAYS_MATH_TEST(leftVector == m_expectedLeftVector, Utility::Logging::ERR,
		"The left vector for quaternion ", m_quaternion, " is ", leftVector, ". It is different than expected ", m_expectedLeftVector);
}

void QuaternionTestLeft::StartTimeTest()
{
}
/* ==================== class QuaternionTestLeft end ==================== */

/* ==================== class QuaternionTestRotationMatrix begin ==================== */
QuaternionTestRotationMatrix::QuaternionTestRotationMatrix(const Math::Quaternion& quaternion, const Math::Matrix4D& expectedRotationMatrix) :
	QuaternionTestBase(quaternion),
	m_expectedRotationMatrix(expectedRotationMatrix)
{
}

QuaternionTestRotationMatrix::~QuaternionTestRotationMatrix(void)
{
}

void QuaternionTestRotationMatrix::StartTest()
{
	Math::Matrix4D rotationMatrix = m_quaternion.ToRotationMatrix();
	CHECK_CONDITION_ALWAYS_MATH_TEST(rotationMatrix == m_expectedRotationMatrix, Utility::Logging::ERR,
		"The rotation matrix for quaternion ", m_quaternion, " is ", rotationMatrix, ". It is different than expected ", m_expectedRotationMatrix);
}

void QuaternionTestRotationMatrix::StartTimeTest()
{
}
/* ==================== class QuaternionTestRotationMatrix end ==================== */