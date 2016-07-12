#include "VectorTestGroup.h"
#include "Math\FloatingPoint.h"

using namespace MathTest;

/* ==================== class VectorTestGroup begin ==================== */
VectorTestGroup::VectorTestGroup(void) :
	TestGroup()
{
	NOTICE_LOG_MATH_TEST("Vector tests started");
}

VectorTestGroup::~VectorTestGroup(void)
{
	NOTICE_LOG_MATH_TEST("Vector tests finished");
}

void VectorTestGroup::StartTests()
{
	if (!IsTestGroupEnabled())
	{
		return;
	}
	TestGroup::StartTests();
}
/* ==================== class VectorTestGroup end ==================== */

/* ==================== class Vector2DTestCross begin ==================== */
Vector2DTestCross::Vector2DTestCross(const Math::Vector2D& vector1, const Math::Vector2D& vector2, Math::Real expectedCrossResult) :
	VectorTestBase<Math::Vector2D>(vector1),
	m_vector2(vector2),
	m_expectedCrossResult(expectedCrossResult)
{
}

Vector2DTestCross::~Vector2DTestCross(void)
{
}

void Vector2DTestCross::StartTest()
{
	DEBUG_LOG_MATH_TEST("Vector2D cross product testing begins...");
	Math::Real crossResult = m_vector.Cross(m_vector2);
	CHECK_CONDITION_ALWAYS_MATH_TEST(Math::AlmostEqual(crossResult, m_expectedCrossResult), Utility::Logging::ERR,
		"The cross product of vectors %s and %s equals %.3f. It is different than expected %.3f",
		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), crossResult, m_expectedCrossResult);
}
/* ==================== class Vector2DTestCross end ==================== */

/* ==================== class Vector3DTestCross begin ==================== */
Vector3DTestCross::Vector3DTestCross(const Math::Vector3D& vector1, const Math::Vector3D& vector2, const Math::Vector3D& expectedCrossResult) :
	VectorTestBase<Math::Vector3D>(vector1),
	m_vector2(vector2),
	m_expectedCrossResult(expectedCrossResult)
{
}

Vector3DTestCross::~Vector3DTestCross(void)
{
}

void Vector3DTestCross::StartTest()
{
	DEBUG_LOG_MATH_TEST("Vector3D cross product testing begins...");
	Math::Vector3D crossResult = m_vector.Cross(m_vector2);
	CHECK_CONDITION_ALWAYS_MATH_TEST(crossResult == m_expectedCrossResult, Utility::Logging::ERR,
		"The cross product of vectors %s and %s equals %s. It is different than expected %s",
		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), crossResult.ToString().c_str(),
		m_expectedCrossResult.ToString().c_str());
}
/* ==================== class Vector3DTestCross end ==================== */


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
//	CHECK_CONDITION_ALWAYS_MATH_TEST(rotateVector == m_expectedRotateVector, Utility::ERR,
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
//	CHECK_CONDITION_ALWAYS_MATH_TEST(lerpVector == m_expectedLerpVector, Utility::ERR,
//		"The linear interpolation of vectors %s and %s with the factor equal to %.2f is a vector %s. It is different than expected %s",
//		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), m_lerpFactor, lerpVector.ToString().c_str(),
//		m_expectedLerpVector.ToString().c_str());
//}
/* ==================== class Vector2DTestLerp end ==================== */