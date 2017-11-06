#include "TransformTestGroup.h"
#include "Def.h"

using namespace MathTest;

/* ==================== class TransformTestGroup begin ==================== */
TransformTestGroup::TransformTestGroup(const std::string& transformTestGroupName, int testTimeIterationsCount) :
	TestGroup(transformTestGroupName, testTimeIterationsCount)
{
	NOTICE_LOG_MATH_TEST("Transform tests started");
}


TransformTestGroup::~TransformTestGroup(void)
{
	NOTICE_LOG_MATH_TEST("Transform tests finished");
}

/* ==================== class TransformTestGroup end ==================== */

/* ==================== class TransformTestBase begin ==================== */
TransformTestBase::TransformTestBase(const Math::Transform& transform) :
	Test(),
	m_transform(transform)
{
}

TransformTestBase::~TransformTestBase(void)
{
}
/* ==================== class TransformTestBase end ==================== */

/* ==================== class TransformTestCompare begin ==================== */
TransformTestCompare::TransformTestCompare(const Math::Transform& transform1, const Math::Transform& transform2, bool expectedCompareResult) :
	TransformTestBase(transform1),
	m_compareTransform(transform2),
	m_expectedCompareResult(expectedCompareResult)
{
}

TransformTestCompare::~TransformTestCompare(void)
{
}

void TransformTestCompare::StartTest()
{
	bool compareResult = (m_transform == m_compareTransform);
	CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult == m_expectedCompareResult, Utility::Logging::ERR,
		"The transform ", m_transform, " and transform ", m_compareTransform, " equality comparison result is not as expected.");

	bool diffResult = (m_transform != m_compareTransform);
	CHECK_CONDITION_ALWAYS_MATH_TEST(diffResult == !m_expectedCompareResult, Utility::Logging::ERR,
		"The transform ", m_transform, " and transform ", m_compareTransform, " inequality comparison result is not as expected.");

	CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult != diffResult, Utility::Logging::ERR,
		"The transform ", m_transform, " and transform ", m_compareTransform, " equality and inequality comparisons results are the same.");
}

void TransformTestCompare::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class TransformTestCompare end ==================== */


/* ==================== class TransformTestParent begin ==================== */
TransformTestParent::TransformTestParent(const Math::Transform& parentTransform, const Math::Transform& childTransform, const Math::Transform& expectedChildFinalTransform) :
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
	Math::Vector3D childPos = m_childTransform.GetTransformedPos();
	CHECK_CONDITION_ALWAYS_MATH_TEST(childPos == m_expectedChildFinalTransform.GetPos(), Utility::Logging::ERR,
		"The transformed position of the child transform equals: ", childPos, " while it was expected to equal: ", m_expectedChildFinalTransform.GetPos());

	Math::Quaternion childRot = m_childTransform.GetTransformedRot();
	CHECK_CONDITION_ALWAYS_MATH_TEST(childRot == m_expectedChildFinalTransform.GetRot(), Utility::Logging::ERR,
		"The transformed rotation of the child transform equals: ", childRot, " while it was expected to equal: ", m_expectedChildFinalTransform.GetRot());

	// TODO: GetTransformedScale
}

void TransformTestParent::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class TransformTestParent end ==================== */
