#include "TransformTestGroup.h"
#include "Def.h"

#include "Utility/ILogger.h"

using namespace math_test;

/* ==================== class TransformTestGroup begin ==================== */
TransformTestGroup::TransformTestGroup(const std::string& transformTestGroupName, int testTimeIterationsCount) :
	TestGroup(transformTestGroupName, testTimeIterationsCount)
{
	NOTICE_LOG_MATH_TEST("Transform tests started");
}


TransformTestGroup::~TransformTestGroup()
{
	NOTICE_LOG_MATH_TEST("Transform tests finished");
}

/* ==================== class TransformTestGroup end ==================== */

/* ==================== class TransformTestBase begin ==================== */
TransformTestBase::TransformTestBase(const math::Transform& transform) :
	Test(),
	m_transform(transform)
{
}

TransformTestBase::~TransformTestBase()
{
}
/* ==================== class TransformTestBase end ==================== */

/* ==================== class TransformTestCompare begin ==================== */
TransformTestCompare::TransformTestCompare(const math::Transform& transform1, const math::Transform& transform2, bool expectedCompareResult) :
	TransformTestBase(transform1),
	m_compareTransform(transform2),
	m_expectedCompareResult(expectedCompareResult)
{
}

TransformTestCompare::~TransformTestCompare()
{
}

void TransformTestCompare::StartTest()
{
	bool compareResult = (m_transform == m_compareTransform);
	CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult == m_expectedCompareResult, utility::logging::ERR,
		"The transform ", m_transform, " and transform ", m_compareTransform, " equality comparison result is not as expected.");

	bool diffResult = (m_transform != m_compareTransform);
	CHECK_CONDITION_ALWAYS_MATH_TEST(diffResult == !m_expectedCompareResult, utility::logging::ERR,
		"The transform ", m_transform, " and transform ", m_compareTransform, " inequality comparison result is not as expected.");

	CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult != diffResult, utility::logging::ERR,
		"The transform ", m_transform, " and transform ", m_compareTransform, " equality and inequality comparisons results are the same.");
}

void TransformTestCompare::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class TransformTestCompare end ==================== */


/* ==================== class TransformTestParent begin ==================== */
TransformTestParent::TransformTestParent(const math::Transform& parentTransform, const math::Transform& childTransform, const math::Transform& expectedChildFinalTransform) :
	TransformTestBase(parentTransform),
	m_childTransform(childTransform),
	m_expectedChildFinalTransform(expectedChildFinalTransform)
{
	m_childTransform.SetParent(&m_transform);
}

TransformTestParent::~TransformTestParent()
{
}

void TransformTestParent::StartTest()
{
	math::Vector3D childPos = m_childTransform.GetTransformedPos();
	CHECK_CONDITION_ALWAYS_MATH_TEST(childPos == m_expectedChildFinalTransform.GetPos(), utility::logging::ERR,
		"The transformed position of the child transform equals: ", childPos, " while it was expected to equal: ", m_expectedChildFinalTransform.GetPos());

	math::Quaternion childRot = m_childTransform.GetTransformedRot();
	CHECK_CONDITION_ALWAYS_MATH_TEST(childRot == m_expectedChildFinalTransform.GetRot(), utility::logging::ERR,
		"The transformed rotation of the child transform equals: ", childRot, " while it was expected to equal: ", m_expectedChildFinalTransform.GetRot());

	// TODO: GetTransformedScale
}

void TransformTestParent::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class TransformTestParent end ==================== */
