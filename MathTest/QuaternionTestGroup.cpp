#include "QuaternionTestGroup.h"
#include "Math/FloatingPoint.h"
#include "Def.h"

using namespace math_test;

/* ==================== class QuaternionTestGroup begin ==================== */
QuaternionTestGroup::QuaternionTestGroup(const std::string& quaternionTestGroupName, int testTimeIterationsCount) :
	TestGroup(quaternionTestGroupName, testTimeIterationsCount)
{
	NOTICE_LOG_MATH_TEST("Quaternion test started");
}


QuaternionTestGroup::~QuaternionTestGroup()
{
	NOTICE_LOG_MATH_TEST("Quaternion test finished");
}
/* ==================== class QuaternionTestGroup end ==================== */


/* ==================== class QuaternionTestBase begin ==================== */
QuaternionTestBase::QuaternionTestBase(const math::Quaternion& quaternion) :
	Test(),
	m_quaternion(quaternion)
{
}

QuaternionTestBase::~QuaternionTestBase()
{
}
/* ==================== class QuaternionTestBase end ==================== */


/* ==================== class QuaternionTestCompare begin ==================== */
QuaternionTestCompare::QuaternionTestCompare(const math::Quaternion& quaternion1, const math::Quaternion& quaternion2, bool expectedCompareResult) :
	QuaternionTestBase(quaternion1),
	m_compareQuaternion(quaternion2),
	m_expectedCompareResult(expectedCompareResult)
{
}

QuaternionTestCompare::~QuaternionTestCompare()
{
}

void QuaternionTestCompare::StartTest()
{
	bool compareResult = (m_quaternion == m_compareQuaternion);
	CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult == m_expectedCompareResult, utility::logging::ERR,
		"The quaternion ", m_quaternion, " and quaternion ", m_compareQuaternion, " comparison result is not as expected.");
}

void QuaternionTestCompare::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestCompare end ==================== */


/* ==================== class QuaternionTestLength begin ==================== */
QuaternionTestLength::QuaternionTestLength(const math::Quaternion& quaternion, math::Real expectedLength) :
	QuaternionTestBase(quaternion),
	m_expectedLength(expectedLength)
{
}

QuaternionTestLength::~QuaternionTestLength()
{
}

void QuaternionTestLength::StartTest()
{
	math::Real lengthSquared = m_quaternion.LengthSquared();
	math::Real length = m_quaternion.Length();
	CHECK_CONDITION_ALWAYS_MATH_TEST(math::AlmostEqual(length, m_expectedLength), utility::logging::ERR,
		"The quaternion ", m_quaternion, " has length ", length, ", but was expected to have length ", m_expectedLength);
	CHECK_CONDITION_ALWAYS_MATH_TEST(math::AlmostEqual(lengthSquared, m_expectedLength * m_expectedLength), utility::logging::ERR,
		"The quaternion ", m_quaternion, " has squared length ", lengthSquared, ", but expected to have squared length ",
		m_expectedLength * m_expectedLength);
}

void QuaternionTestLength::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestLength end ==================== */


/* ==================== class QuaternionTestConjugate begin ==================== */
QuaternionTestConjugate::QuaternionTestConjugate(const math::Quaternion& quaternion, const math::Quaternion& expectedConjugate) :
	QuaternionTestBase(quaternion),
	m_expectedConjugate(expectedConjugate)
{
}

QuaternionTestConjugate::~QuaternionTestConjugate()
{
}

void QuaternionTestConjugate::StartTest()
{
	math::Quaternion conjugate = m_quaternion.Conjugate();
	CHECK_CONDITION_ALWAYS_MATH_TEST(conjugate == m_expectedConjugate, utility::logging::ERR,
		"The quaternion ", m_quaternion, " has conjugate ", conjugate, ". It is different than expected ", m_expectedConjugate);
	CHECK_CONDITION_ALWAYS_MATH_TEST(conjugate.Conjugate() == m_quaternion, utility::logging::ERR,
		"The conjugate of a quaternion is supposed to be an involution, but the conjugate of conjugate of a quaternion (", conjugate.Conjugate(),
		") is not an original quaternion (", m_quaternion, ").");
}

void QuaternionTestConjugate::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestConjugate end ==================== */


/* ==================== class QuaternionTestAddOperator begin ==================== */
QuaternionTestAddOperator::QuaternionTestAddOperator(const math::Quaternion& quaternion, const math::Quaternion& quaternion2, const math::Quaternion& expectedSumQuaternion) :
	QuaternionTestBase(quaternion),
	m_quaternion2(quaternion2),
	m_expectedSumQuaternion(expectedSumQuaternion)
{
}

QuaternionTestAddOperator::~QuaternionTestAddOperator()
{
}

void QuaternionTestAddOperator::StartTest()
{
	math::Quaternion sumQuaternion = m_quaternion + m_quaternion2;
	CHECK_CONDITION_ALWAYS_MATH_TEST(sumQuaternion == m_expectedSumQuaternion, utility::logging::ERR,
		"The sum of quaternions ", m_quaternion, " and ", m_quaternion2, " is a quaternion ",
		sumQuaternion, ". It is different than expected ", m_expectedSumQuaternion);
}

void QuaternionTestAddOperator::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestAddOperator end ==================== */


/* ==================== class QuaternionTestSubtractOperator begin ==================== */
QuaternionTestSubtractOperator::QuaternionTestSubtractOperator(const math::Quaternion& quaternion, const math::Quaternion& quaternion2, const math::Quaternion& expectedSubtractQuaternion) :
	QuaternionTestBase(quaternion),
	m_quaternion2(quaternion2),
	m_expectedSubtractQuaternion(expectedSubtractQuaternion)
{
}

QuaternionTestSubtractOperator::~QuaternionTestSubtractOperator()
{
}

void QuaternionTestSubtractOperator::StartTest()
{
	math::Quaternion subtractQuaternion = m_quaternion - m_quaternion2;
	CHECK_CONDITION_ALWAYS_MATH_TEST(subtractQuaternion == m_expectedSubtractQuaternion, utility::logging::ERR,
		"The subtraction of quaternions ", m_quaternion, " and ", m_quaternion2, " is a quaternion ",
		subtractQuaternion, ". It is different than expected ", m_expectedSubtractQuaternion);
}

void QuaternionTestSubtractOperator::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestSubtractOperator end ==================== */

/* ==================== class QuaternionTestDivideOperator begin ==================== */
QuaternionTestDivideOperator::QuaternionTestDivideOperator(const math::Quaternion& quaternion, math::Real divideValue, const math::Quaternion& expectedDivideQuaternion) :
	QuaternionTestBase(quaternion),
	m_divideValue(divideValue),
	m_expectedDivideQuaternion(expectedDivideQuaternion)
{
}

QuaternionTestDivideOperator::~QuaternionTestDivideOperator()
{
}

void QuaternionTestDivideOperator::StartTest()
{
	math::Quaternion divideQuaternion = m_quaternion / m_divideValue;
	CHECK_CONDITION_ALWAYS_MATH_TEST(divideQuaternion == m_expectedDivideQuaternion, utility::logging::ERR,
		"The division of quaternion ", m_quaternion, " by value ", m_divideValue, " is a quaternion ",
		divideQuaternion, ". It is different than expected ", m_expectedDivideQuaternion);
}

void QuaternionTestDivideOperator::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestDivideOperator end ==================== */


/* ==================== class QuaternionTestNormalize begin ==================== */
QuaternionTestNormalize::QuaternionTestNormalize(const math::Quaternion& quaternion, const math::Quaternion& expectedNormalizedQuaternion) :
	QuaternionTestBase(quaternion),
	m_expectedNormalizedQuaternion(expectedNormalizedQuaternion)
{
	math::Real lengthSquared = m_expectedNormalizedQuaternion.LengthSquared();
	math::Real length = m_expectedNormalizedQuaternion.Length();
	CHECK_CONDITION_ALWAYS_MATH_TEST(math::AlmostEqual(lengthSquared, REAL_ONE), utility::logging::ERR,
		"Given expected normalized quaternion ", m_expectedNormalizedQuaternion, " is in fact not normalized.");
	CHECK_CONDITION_ALWAYS_MATH_TEST(math::AlmostEqual(length, lengthSquared), utility::logging::ERR,
		"Given expected normalized quaternion ", m_expectedNormalizedQuaternion, " gives different results for length and squared length (", length, " and ", lengthSquared, " respectively).");
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_expectedNormalizedQuaternion.IsNormalized(), utility::logging::ERR,
		"Given expected normalized quaternion ", m_expectedNormalizedQuaternion, " is in fact not normalized.");
}

QuaternionTestNormalize::~QuaternionTestNormalize()
{
}

void QuaternionTestNormalize::StartTest()
{
	math::Quaternion normalizedQuaternion = m_quaternion.Normalized();
	CHECK_CONDITION_ALWAYS_MATH_TEST(normalizedQuaternion == m_expectedNormalizedQuaternion, utility::logging::ERR,
		"The quaternion ", m_quaternion, " after normalization equals ", normalizedQuaternion, ". It is different than expected ",
		m_expectedNormalizedQuaternion, ".");
	CHECK_CONDITION_ALWAYS_MATH_TEST(normalizedQuaternion.IsNormalized(), utility::logging::ERR,
		"Calculated normalized quaternion ", normalizedQuaternion, " is in fact not normalized.");

	m_quaternion.Normalize();
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_quaternion == m_expectedNormalizedQuaternion, utility::logging::ERR,
		"The quaternion after normalization ", m_quaternion, " is different than expected ", m_expectedNormalizedQuaternion);
	CHECK_CONDITION_ALWAYS_MATH_TEST(m_quaternion.IsNormalized(), utility::logging::ERR,
		"Calculated normalized quaternion ", m_quaternion, " is in fact not normalized.");
}

void QuaternionTestNormalize::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestNormalize end ==================== */


/* ==================== class QuaternionTestDot begin ==================== */
QuaternionTestDot::QuaternionTestDot(const math::Quaternion& quaternion, const math::Quaternion& quaternion2, math::Real expectedDotResult) :
	QuaternionTestBase(quaternion),
	m_quaternion2(quaternion2),
	m_expectedDotResult(expectedDotResult)
{
}

QuaternionTestDot::~QuaternionTestDot()
{
}

void QuaternionTestDot::StartTest()
{
	math::Real dotResult = m_quaternion.Dot(m_quaternion2);
	CHECK_CONDITION_ALWAYS_MATH_TEST(math::AlmostEqual(dotResult, m_expectedDotResult), utility::logging::ERR,
		"The dot product of quaternions ", m_quaternion, " and ", m_quaternion2, " equals ", dotResult, ". It is different than expected ", m_expectedDotResult);
}

void QuaternionTestDot::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestDot end ==================== */


/* ==================== class QuaternionTestNlerp begin ==================== */
QuaternionTestNlerp::QuaternionTestNlerp(const math::Quaternion& quaternion, const math::Quaternion& quaternion2, math::Real nlerpFactor, bool shortest, const math::Quaternion& expectedNlerpQuaternion) :
	QuaternionTestBase(quaternion),
	m_quaternion2(quaternion2),
	m_nlerpFactor(nlerpFactor),
	m_shortest(shortest),
	m_expectedNlerpQuaternion(expectedNlerpQuaternion)
{
}

QuaternionTestNlerp::~QuaternionTestNlerp()
{
}

void QuaternionTestNlerp::StartTest()
{
	math::Quaternion nlerpQuaternion = m_quaternion.Nlerp(m_quaternion2, m_nlerpFactor, m_shortest);
	CHECK_CONDITION_ALWAYS_MATH_TEST(nlerpQuaternion == m_expectedNlerpQuaternion, utility::logging::ERR,
		"The linear interpolation of quaternions ", m_quaternion, " and ", m_quaternion2, " with the factor equal to ",
		m_nlerpFactor, " is a quaternion ", nlerpQuaternion, ". It is different than expected ", m_expectedNlerpQuaternion);
}

void QuaternionTestNlerp::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestNlerp end ==================== */

/* ==================== class QuaternionTestSlerp begin ==================== */
QuaternionTestSlerp::QuaternionTestSlerp(const math::Quaternion& quaternion, const math::Quaternion& quaternion2, math::Real slerpFactor, bool shortest, const math::Quaternion& expectedSlerpQuaternion) :
	QuaternionTestBase(quaternion),
	m_quaternion2(quaternion2),
	m_slerpFactor(slerpFactor),
	m_shortest(shortest),
	m_expectedSlerpQuaternion(expectedSlerpQuaternion)
{
}

QuaternionTestSlerp::~QuaternionTestSlerp()
{
}

void QuaternionTestSlerp::StartTest()
{
	math::Quaternion slerpQuaternion = m_quaternion.Slerp(m_quaternion2, m_slerpFactor, m_shortest);
	CHECK_CONDITION_ALWAYS_MATH_TEST(slerpQuaternion == m_expectedSlerpQuaternion, utility::logging::ERR,
		"The spherical linear interpolation of quaternions ", m_quaternion, " and ", m_quaternion2, " with the factor equal to ",
		m_slerpFactor, " is a quaternion ", slerpQuaternion, ". It is different than expected ", m_expectedSlerpQuaternion);
}

void QuaternionTestSlerp::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestSlerp end ==================== */

/* ==================== class QuaternionTestForward begin ==================== */
QuaternionTestForward::QuaternionTestForward(const math::Quaternion& quaternion, const math::Vector3D& expectedForwardVector) :
	QuaternionTestBase(quaternion),
	m_expectedForwardVector(expectedForwardVector)
{
}

QuaternionTestForward::~QuaternionTestForward()
{
}

void QuaternionTestForward::StartTest()
{
	math::Vector3D forwardVector = m_quaternion.GetForward();
	CHECK_CONDITION_ALWAYS_MATH_TEST(forwardVector == m_expectedForwardVector, utility::logging::ERR,
		"The forward vector for quaternion ", m_quaternion, " is ", forwardVector, ". It is different than expected ", m_expectedForwardVector);
}

void QuaternionTestForward::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestForward end ==================== */

/* ==================== class QuaternionTestUp begin ==================== */
QuaternionTestUp::QuaternionTestUp(const math::Quaternion& quaternion, const math::Vector3D& expectedUpVector) :
	QuaternionTestBase(quaternion),
	m_expectedUpVector(expectedUpVector)
{
}

QuaternionTestUp::~QuaternionTestUp()
{
}

void QuaternionTestUp::StartTest()
{
	math::Vector3D upVector = m_quaternion.GetUp();
	CHECK_CONDITION_ALWAYS_MATH_TEST(upVector == m_expectedUpVector, utility::logging::ERR,
		"The up vector for quaternion ", m_quaternion, " is ", upVector, ". It is different than expected ", m_expectedUpVector);
}

void QuaternionTestUp::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestUp end ==================== */

/* ==================== class QuaternionTestRight begin ==================== */
QuaternionTestRight::QuaternionTestRight(const math::Quaternion& quaternion, const math::Vector3D& expectedRightVector) :
	QuaternionTestBase(quaternion),
	m_expectedRightVector(expectedRightVector)
{
}

QuaternionTestRight::~QuaternionTestRight()
{
}

void QuaternionTestRight::StartTest()
{
	math::Vector3D rightVector = m_quaternion.GetRight();
	CHECK_CONDITION_ALWAYS_MATH_TEST(rightVector == m_expectedRightVector, utility::logging::ERR,
		"The right vector for quaternion ", m_quaternion, " is ", rightVector, ". It is different than expected ", m_expectedRightVector);
}

void QuaternionTestRight::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestRight end ==================== */

/* ==================== class QuaternionTestBack begin ==================== */
QuaternionTestBack::QuaternionTestBack(const math::Quaternion& quaternion, const math::Vector3D& expectedBackVector) :
	QuaternionTestBase(quaternion),
	m_expectedBackVector(expectedBackVector)
{
}

QuaternionTestBack::~QuaternionTestBack()
{
}

void QuaternionTestBack::StartTest()
{
	math::Vector3D backVector = m_quaternion.GetBack();
	CHECK_CONDITION_ALWAYS_MATH_TEST(backVector == m_expectedBackVector, utility::logging::ERR,
		"The back vector for quaternion ", m_quaternion, " is ", backVector, ". It is different than expected ", m_expectedBackVector);
}

void QuaternionTestBack::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestBack end ==================== */

/* ==================== class QuaternionTestDown begin ==================== */
QuaternionTestDown::QuaternionTestDown(const math::Quaternion& quaternion, const math::Vector3D& expectedDownVector) :
	QuaternionTestBase(quaternion),
	m_expectedDownVector(expectedDownVector)
{
}

QuaternionTestDown::~QuaternionTestDown()
{
}

void QuaternionTestDown::StartTest()
{
	math::Vector3D downVector = m_quaternion.GetDown();
	CHECK_CONDITION_ALWAYS_MATH_TEST(downVector == m_expectedDownVector, utility::logging::ERR,
		"The down vector for quaternion ", m_quaternion, " is ", downVector, ". It is different than expected ", m_expectedDownVector);
}

void QuaternionTestDown::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestDown end ==================== */

/* ==================== class QuaternionTestLeft begin ==================== */
QuaternionTestLeft::QuaternionTestLeft(const math::Quaternion& quaternion, const math::Vector3D& expectedLeftVector) :
	QuaternionTestBase(quaternion),
	m_expectedLeftVector(expectedLeftVector)
{
}

QuaternionTestLeft::~QuaternionTestLeft()
{
}

void QuaternionTestLeft::StartTest()
{
	math::Vector3D leftVector = m_quaternion.GetLeft();
	CHECK_CONDITION_ALWAYS_MATH_TEST(leftVector == m_expectedLeftVector, utility::logging::ERR,
		"The left vector for quaternion ", m_quaternion, " is ", leftVector, ". It is different than expected ", m_expectedLeftVector);
}

void QuaternionTestLeft::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestLeft end ==================== */

/* ==================== class QuaternionTestRotationMatrix begin ==================== */
QuaternionTestRotationMatrix::QuaternionTestRotationMatrix(const math::Quaternion& quaternion, const math::Matrix4D& expectedRotationMatrix) :
	QuaternionTestBase(quaternion),
	m_expectedRotationMatrix(expectedRotationMatrix)
{
}

QuaternionTestRotationMatrix::~QuaternionTestRotationMatrix()
{
}

void QuaternionTestRotationMatrix::StartTest()
{
	math::Matrix4D rotationMatrix = m_quaternion.ToRotationMatrix();
	CHECK_CONDITION_ALWAYS_MATH_TEST(rotationMatrix == m_expectedRotationMatrix, utility::logging::ERR,
		"The rotation matrix for quaternion ", m_quaternion, " is ", rotationMatrix, ". It is different than expected ", m_expectedRotationMatrix);
}

void QuaternionTestRotationMatrix::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class QuaternionTestRotationMatrix end ==================== */