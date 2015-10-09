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
	CHECK_CONDITION_ALWAYS(conjugate == m_expectedConjugate, Utility::Error,
		"The quaternion %s has conjugate %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), conjugate.ToString().c_str(), m_expectedConjugate.ToString().c_str());
	CHECK_CONDITION_ALWAYS(conjugate.Conjugate() == m_quaternion, Utility::Error,
		"The conjugate of a quaternion is supposed to be an involution, but the conjugate of conjugate of a quaternion (%s) is not an original quaternion (%s).",
		conjugate.Conjugate().ToString().c_str(), m_quaternion.ToString().c_str());
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
	CHECK_CONDITION_ALWAYS(sumQuaternion == m_expectedSumQuaternion, Utility::Error,
		"The sum of quaternions %s and %s is a quaternion %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), m_quaternion2.ToString().c_str(),
		sumQuaternion.ToString().c_str(), m_expectedSumQuaternion.ToString().c_str());
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
	CHECK_CONDITION_ALWAYS(subtractQuaternion == m_expectedSubtractQuaternion, Utility::Error,
		"The subtraction of quaternions %s and %s is a quaternion %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), m_quaternion2.ToString().c_str(),
		subtractQuaternion.ToString().c_str(), m_expectedSubtractQuaternion.ToString().c_str());
}
/* ==================== class QuaternionTestSubtractOperator end ==================== */


/* ==================== class QuaternionTestDivideOperator begin ==================== */
QuaternionTestDivideOperator::QuaternionTestDivideOperator(const Math::Quaternion& quaternion, const Math::Quaternion& quaternion2, const Math::Quaternion& expectedDivideQuaternion) :
	QuaternionTestBase(quaternion),
	m_quaternion2(quaternion2),
	m_expectedDivideQuaternion(expectedDivideQuaternion)
{
}

QuaternionTestDivideOperator::~QuaternionTestDivideOperator(void)
{
}

void QuaternionTestDivideOperator::StartTest()
{
	Math::Quaternion divideQuaternion = m_quaternion / m_quaternion2;
	CHECK_CONDITION_ALWAYS(divideQuaternion == m_expectedDivideQuaternion, Utility::Error,
		"The division of quaternions %s and %s is a quaternion %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), m_quaternion2.ToString().c_str(),
		divideQuaternion.ToString().c_str(), m_expectedDivideQuaternion.ToString().c_str());
}
/* ==================== class QuaternionTestDivideOperator end ==================== */


/* ==================== class QuaternionTestNormalize begin ==================== */
QuaternionTestNormalize::QuaternionTestNormalize(const Math::Quaternion& quaternion, const Math::Quaternion& expectedNormalizedQuaternion) :
	QuaternionTestBase(quaternion),
	m_expectedNormalizedQuaternion(expectedNormalizedQuaternion)
{
	Math::Real lengthSquared = m_expectedNormalizedQuaternion.LengthSquared();
	Math::Real length = m_expectedNormalizedQuaternion.Length();
	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(lengthSquared, REAL_ONE), Utility::Error,
		"Given expected normalized quaternion %s is in fact not normalized.", m_expectedNormalizedQuaternion.ToString().c_str());
	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(length, lengthSquared), Utility::Error,
		"Given expected normalized quaternion %s gives different results for length and squared length (%.3f and %.3f respectively).",
		m_expectedNormalizedQuaternion.ToString().c_str(), length, lengthSquared);
	CHECK_CONDITION_ALWAYS(m_expectedNormalizedQuaternion.IsNormalized(), Utility::Error,
		"Given expected normalized quaternion %s is in fact not normalized.",
		m_expectedNormalizedQuaternion.ToString().c_str());
}

QuaternionTestNormalize::~QuaternionTestNormalize(void)
{
}

void QuaternionTestNormalize::StartTest()
{
	Math::Quaternion normalizedQuaternion = m_quaternion.Normalized();
	CHECK_CONDITION_ALWAYS(normalizedQuaternion == m_expectedNormalizedQuaternion, Utility::Error,
		"The quaternion %s after normalization equals %s. It is different than expected %s.",
		m_quaternion.ToString().c_str(), normalizedQuaternion.ToString().c_str(),
		m_expectedNormalizedQuaternion.ToString().c_str());
	CHECK_CONDITION_ALWAYS(normalizedQuaternion.IsNormalized(), Utility::Error,
		"Calculated normalized quaternion %s is in fact not normalized.",
		normalizedQuaternion.ToString().c_str());

	m_quaternion.Normalize();
	CHECK_CONDITION_ALWAYS(m_quaternion == m_expectedNormalizedQuaternion, Utility::Error,
		"The quaternion after normalization %s is different than expected %s",
		m_quaternion.ToString().c_str(), m_expectedNormalizedQuaternion.ToString().c_str());
	CHECK_CONDITION_ALWAYS(m_quaternion.IsNormalized(), Utility::Error,
		"Calculated normalized quaternion %s is in fact not normalized.",
		m_quaternion.ToString().c_str());
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
	Math::Quaternion dotResult = m_quaternion.Dot(m_quaternion2);
	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(dotResult, m_expectedDotResult), Utility::Error,
		"The dot product of quaternions %s and %s equals %.3f. It is different than expected %.3f",
		m_quaternion.ToString().c_str(), m_quaternion2.ToString().c_str(), dotResult, m_expectedDotResult);
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
	CHECK_CONDITION_ALWAYS(nlerpQuaternion == m_expectedNlerpQuaternion, Utility::Error,
		"The linear interpolation of quaternions %s and %s with the factor equal to %.2f is a vector %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), m_quaternion2.ToString().c_str(), m_nlerpFactor, nlerpQuaternion.ToString().c_str(),
		m_expectedNlerpQuaternion.ToString().c_str());
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
	CHECK_CONDITION_ALWAYS(slerpQuaternion == m_expectedSlerpQuaternion, Utility::Error,
		"The spherical linear interpolation of quaternions %s and %s with the factor equal to %.2f is a vector %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), m_quaternion2.ToString().c_str(), m_slerpFactor, slerpQuaternion.ToString().c_str(),
		m_expectedSlerpQuaternion.ToString().c_str());
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
	CHECK_CONDITION_ALWAYS(forwardVector == m_expectedForwardVector, Utility::Error,
		"The forward vector for quaternion %s is %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), forwardVector.ToString().c_str(),
		m_expectedForwardVector.ToString().c_str());
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
	CHECK_CONDITION_ALWAYS(upVector == m_expectedUpVector, Utility::Error,
		"The up vector for quaternion %s is %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), upVector.ToString().c_str(),
		m_expectedUpVector.ToString().c_str());
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
	CHECK_CONDITION_ALWAYS(rightVector == m_expectedRightVector, Utility::Error,
		"The right vector for quaternion %s is %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), rightVector.ToString().c_str(),
		m_expectedRightVector.ToString().c_str());
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
	CHECK_CONDITION_ALWAYS(backVector == m_expectedBackVector, Utility::Error,
		"The back vector for quaternion %s is %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), backVector.ToString().c_str(),
		m_expectedBackVector.ToString().c_str());
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
	CHECK_CONDITION_ALWAYS(downVector == m_expectedDownVector, Utility::Error,
		"The down vector for quaternion %s is %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), downVector.ToString().c_str(),
		m_expectedDownVector.ToString().c_str());
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
	CHECK_CONDITION_ALWAYS(leftVector == m_expectedLeftVector, Utility::Error,
		"The left vector for quaternion %s is %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), leftVector.ToString().c_str(),
		m_expectedLeftVector.ToString().c_str());
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
	Math::Vector3D rotationMatrix = m_quaternion.ToRotationMatrix();
	CHECK_CONDITION_ALWAYS(rotationMatrix == m_expectedRotationMatrix, Utility::Error,
		"The rotation matrix for quaternion %s is %s. It is different than expected %s",
		m_quaternion.ToString().c_str(), rotationMatrix.ToString().c_str(),
		m_expectedRotationMatrix.ToString().c_str());
}
/* ==================== class QuaternionTestRotationMatrix end ==================== */