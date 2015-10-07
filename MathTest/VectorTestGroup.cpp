#include "VectorTestGroup.h"
#include "Math\FloatingPoint.h"

using namespace MathTest;

/* ==================== class VectorTestGroup begin ==================== */
VectorTestGroup::VectorTestGroup(void) :
	TestGroup()
{
	NOTICE_LOG("Vector test started");
}

VectorTestGroup::~VectorTestGroup(void)
{
	NOTICE_LOG("Vector test finished");
}

void VectorTestGroup::StartTests()
{
	if (!IsTestGroupEnabled())
	{
		return;
	}
}
/* ==================== class VectorTestGroup end ==================== */


/* ==================== class Vector2DTestLength begin ==================== */
//Vector2DTestLength::Vector2DTestLength(const Math::Vector2D& vector, Math::Real expectedLength) :
//	VectorTestBase<Math::Vector2D>(vector),
//	m_expectedLength(expectedLength)
//{
//}
//
//Vector2DTestLength::~Vector2DTestLength(void)
//{
//}
//
//void Vector2DTestLength::StartTest()
//{
//	Math::Real lengthSquared = m_vector.LengthSquared();
//	Math::Real length = m_vector.Length();
//	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(length, m_expectedLength), Utility::Error,
//		"The vector %s has length %.2f, but was expected to have length %.2f",
//		m_vector.ToString().c_str(), length, m_expectedLength);
//	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(lengthSquared, m_expectedLength * m_expectedLength), Utility::Error,
//		"The vector %s has length %.2f and squared length %.2f, but expected to have squared length %.2f",
//		m_vector.ToString().c_str(), lengthSquared, m_expectedLength * m_expectedLength);
//}
/* ==================== class Vector2DTestLength end ==================== */


/* ==================== class Vector2DTestAddOperator begin ==================== */
//Vector2DTestAddOperator::Vector2DTestAddOperator(const Math::Vector2D& vector1, const Math::Vector2D& vector2, const Math::Vector2D& expectedSumVector) :
//	VectorTestBase<Math::Vector2D>(vector1),
//	m_vector2(vector2),
//	m_expectedSumVector(expectedSumVector)
//{
//}
//
//Vector2DTestAddOperator::~Vector2DTestAddOperator(void)
//{
//}
//
//void Vector2DTestAddOperator::StartTest()
//{
//	Math::Vector2D sumVector = m_vector + m_vector2;
//	CHECK_CONDITION_ALWAYS(sumVector == m_expectedSumVector, Utility::Error,
//		"The sum of vectors %s and %s is a vector %s. It is different than expected %s",
//		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), sumVector.ToString().c_str(),
//		m_expectedSumVector.ToString().c_str());
//
//	m_vector += m_vector2;
//	CHECK_CONDITION_ALWAYS(m_vector == m_expectedSumVector, Utility::Error,
//		"The operator += returned different result than operator + (%s != %s)",
//		m_vector.ToString().c_str(), m_expectedSumVector.ToString().c_str());
//}
/* ==================== class Vector2DTestAddOperator end ==================== */


/* ==================== class Vector2DTestSubtractOperator begin ==================== */
//Vector2DTestSubtractOperator::Vector2DTestSubtractOperator(const Math::Vector2D& vector1, const Math::Vector2D& vector2, const Math::Vector2D& expectedSubtractVector) :
//	VectorTestBase<Math::Vector2D>(vector1),
//	m_vector2(vector2),
//	m_expectedSubtractVector(expectedSubtractVector)
//{
//}
//
//Vector2DTestSubtractOperator::~Vector2DTestSubtractOperator(void)
//{
//}
//
//void Vector2DTestSubtractOperator::StartTest()
//{
//	Math::Vector2D subtractVector = m_vector - m_vector2;
//	CHECK_CONDITION_ALWAYS(subtractVector == m_expectedSubtractVector, Utility::Error,
//		"The subtraction of vectors %s and %s is a vector %s. It is different than expected %s",
//		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), subtractVector.ToString().c_str(),
//		m_expectedSubtractVector.ToString().c_str());
//
//	m_vector -= m_vector2;
//	CHECK_CONDITION_ALWAYS(m_vector == m_expectedSubtractVector, Utility::Error,
//		"The operator -= returned different result than operator - (%s != %s)",
//		m_vector.ToString().c_str(), m_expectedSubtractVector.ToString().c_str());
//}
/* ==================== class Vector2DTestSubtractOperator end ==================== */


/* ==================== class Vector2DTestMultiplyOperator begin ==================== */
//Vector2DTestMultiplyOperator::Vector2DTestMultiplyOperator(const Math::Vector2D& vector1, const Math::Vector2D& vector2, const Math::Vector2D& expectedMultiplyVector) :
//	VectorTestBase<Math::Vector2D>(vector1),
//	m_vector2(vector2),
//	m_expectedMultiplyVector(expectedMultiplyVector)
//{
//}
//
//Vector2DTestMultiplyOperator::~Vector2DTestMultiplyOperator(void)
//{
//}
//
//void Vector2DTestMultiplyOperator::StartTest()
//{
//	Math::Vector2D multiplyVector = m_vector * m_vector2;
//	CHECK_CONDITION_ALWAYS(multiplyVector == m_expectedMultiplyVector, Utility::Error,
//		"The multiplication of vectors %s and %s is a vector %s. It is different than expected %s",
//		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), multiplyVector.ToString().c_str(),
//		m_expectedMultiplyVector.ToString().c_str());
//
//	m_vector *= m_vector2;
//	CHECK_CONDITION_ALWAYS(m_vector == m_expectedMultiplyVector, Utility::Error,
//		"The operator *= returned different result than operator * (%s != %s)",
//		m_vector.ToString().c_str(), m_expectedMultiplyVector.ToString().c_str());
//}
/* ==================== class Vector2DTestMultiplyOperator end ==================== */


/* ==================== class Vector2DTestDivideOperator begin ==================== */
//Vector2DTestDivideOperator::Vector2DTestDivideOperator(const Math::Vector2D& vector1, const Math::Vector2D& vector2, const Math::Vector2D& expectedDivideVector) :
//	VectorTestBase<Math::Vector2D>(vector1),
//	m_vector2(vector2),
//	m_expectedDivideVector(expectedDivideVector)
//{
//}
//
//Vector2DTestDivideOperator::~Vector2DTestDivideOperator(void)
//{
//}
//
//void Vector2DTestDivideOperator::StartTest()
//{
//	Math::Vector2D divideVector = m_vector / m_vector2;
//	CHECK_CONDITION_ALWAYS(divideVector == m_expectedDivideVector, Utility::Error,
//		"The division of vectors %s and %s is a vector %s. It is different than expected %s",
//		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), divideVector.ToString().c_str(),
//		m_expectedDivideVector.ToString().c_str());
//
//	m_vector /= m_vector2;
//	CHECK_CONDITION_ALWAYS(m_vector == m_expectedDivideVector, Utility::Error,
//		"The operator /= returned different result than operator / (%s != %s)",
//		m_vector.ToString().c_str(), m_expectedDivideVector.ToString().c_str());
//}
/* ==================== class Vector2DTestDivideOperator end ==================== */


/* ==================== class Vector2DTestNormalize begin ==================== */
//Vector2DTestNormalize::Vector2DTestNormalize(const Math::Vector2D& vector, const Math::Vector2D& expectedNormalizedVector) :
//	VectorTestBase<Math::Vector2D>(vector),
//	m_expectedNormalizedVector(expectedNormalizedVector)
//{
//	Math::Real lengthSquared = m_expectedNormalizedVector.LengthSquared();
//	Math::Real length = m_expectedNormalizedVector.Length();
//	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(lengthSquared, REAL_ONE), Utility::Error,
//		"Given expected normalized vector %s is in fact not normalized.", m_expectedNormalizedVector.ToString().c_str());
//	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(length, lengthSquared), Utility::Error,
//		"Given expected normalized vector %s gives different results for length and squared length (%.3f and %.3f respectively).",
//		m_expectedNormalizedVector.ToString().c_str(), length, lengthSquared);
//	CHECK_CONDITION_ALWAYS(m_expectedNormalizedVector.IsNormalized(), Utility::Error,
//		"Given expected normalized vector %s is in fact not normalized.",
//		m_expectedNormalizedVector.ToString().c_str());
//}
//
//Vector2DTestNormalize::~Vector2DTestNormalize(void)
//{
//}
//
//void Vector2DTestNormalize::StartTest()
//{
//	Math::Vector2D normalizedVector = m_vector.Normalized();
//	CHECK_CONDITION_ALWAYS(normalizedVector == m_expectedNormalizedVector, Utility::Error,
//		"The vector %s after normalization equals %s. It is different than expected %s.",
//		m_vector.ToString().c_str(), normalizedVector.ToString().c_str(),
//		m_expectedNormalizedVector.ToString().c_str());
//	CHECK_CONDITION_ALWAYS(normalizedVector.IsNormalized(), Utility::Error,
//		"Calculated normalized vector %s is in fact not normalized.",
//		normalizedVector.ToString().c_str());
//
//	m_vector.Normalize();
//	CHECK_CONDITION_ALWAYS(m_vector == m_expectedNormalizedVector, Utility::Error,
//		"The vector after normalization %s is different than expected %s",
//		m_vector.ToString().c_str(), m_expectedNormalizedVector.ToString().c_str());
//	CHECK_CONDITION_ALWAYS(m_vector.IsNormalized(), Utility::Error,
//		"Calculated normalized vector %s is in fact not normalized.",
//		m_vector.ToString().c_str());
//}
/* ==================== class Vector2DTestNormalize end ==================== */


/* ==================== class Vector2DTestCross begin ==================== */
//Vector2DTestCross::Vector2DTestCross(const Math::Vector2D& vector1, const Math::Vector2D& vector2, Math::Real expectedCrossResult) :
//	VectorTestBase<Math::Vector2D>(vector1),
//	m_vector2(vector2),
//	m_expectedCrossResult(expectedCrossResult)
//{
//}
//
//Vector2DTestCross::~Vector2DTestCross(void)
//{
//}
//
//void Vector2DTestCross::StartTest()
//{
//	Math::Real crossResult = m_vector.Cross(m_vector2);
//	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(crossResult, m_expectedCrossResult), Utility::Error,
//		"The cross product of vectors %s and %s equals %.3f. It is different than expected %.3f",
//		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), crossResult, m_expectedCrossResult);
//}
/* ==================== class Vector2DTestCross end ==================== */


/* ==================== class Vector2DTestDot begin ==================== */
//Vector2DTestDot::Vector2DTestDot(const Math::Vector2D& vector1, const Math::Vector2D& vector2, Math::Real expectedDotResult) :
//	VectorTestBase<Math::Vector2D>(vector1),
//	m_vector2(vector2),
//	m_expectedDotResult(expectedDotResult)
//{
//}
//
//Vector2DTestDot::~Vector2DTestDot(void)
//{
//}
//
//void Vector2DTestDot::StartTest()
//{
//	Math::Real dotResult = m_vector.Dot(m_vector2);
//	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(dotResult, m_expectedDotResult), Utility::Error,
//		"The dot product of vectors %s and %s equals %.3f. It is different than expected %.3f",
//		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), dotResult, m_expectedDotResult);
//}
/* ==================== class Vector2DTestDot end ==================== */


/* ==================== class Vector2DTestRotate begin ==================== */
//Vector2DTestRotate::Vector2DTestRotate(const Math::Vector2D& vector, const Math::Angle& angle, const Math::Vector2D& expectedRotateVector) :
//	VectorTestBase<Math::Vector2D>(vector),
//	m_angle(angle),
//	m_expectedRotateVector(expectedRotateVector)
//{
//}
//
//Vector2DTestRotate::~Vector2DTestRotate(void)
//{
//}
//
//void Vector2DTestRotate::StartTest()
//{
//	Math::Vector2D rotateVector = m_vector.Rotate(m_angle);
//	CHECK_CONDITION_ALWAYS(rotateVector == m_expectedRotateVector, Utility::Error,
//		"The rotation of vector %s by angle %s is a vector %s. It is different than expected %s",
//		m_vector.ToString().c_str(), m_angle.ToString().c_str(), rotateVector.ToString().c_str(),
//		m_expectedRotateVector.ToString().c_str());
//}
/* ==================== class Vector2DTestRotate end ==================== */


/* ==================== class Vector2DTestLerp begin ==================== */
//Vector2DTestLerp::Vector2DTestLerp(const Math::Vector2D& vector, const Math::Vector2D& vector2, Math::Real lerpFactor, const Math::Vector2D& expectedLerpVector) :
//	VectorTestBase<Math::Vector2D>(vector),
//	m_vector2(vector2),
//	m_lerpFactor(lerpFactor),
//	m_expectedLerpVector(expectedLerpVector)
//{
//}
//
//Vector2DTestLerp::~Vector2DTestLerp(void)
//{
//}
//
//void Vector2DTestLerp::StartTest()
//{
//	Math::Vector2D lerpVector = m_vector.Lerp(m_vector2, m_lerpFactor);
//	CHECK_CONDITION_ALWAYS(lerpVector == m_expectedLerpVector, Utility::Error,
//		"The linear interpolation of vectors %s and %s with the factor equal to %.2f is a vector %s. It is different than expected %s",
//		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), m_lerpFactor, lerpVector.ToString().c_str(),
//		m_expectedLerpVector.ToString().c_str());
//}
/* ==================== class Vector2DTestLerp end ==================== */