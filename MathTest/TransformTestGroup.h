#ifndef __MATH_TEST_TRANSFORM_TEST_GROUP_H__
#define __MATH_TEST_TRANSFORM_TEST_GROUP_H__

#include "Test.h"
#include "Math/Transform.h"

namespace math_test
{

	class TransformTestGroup : public TestGroup
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		TransformTestGroup(const std::string& transformTestGroupName, int testTimeIterationsCount);
		virtual ~TransformTestGroup();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class TransformTestGroup */

	class TransformTestBase : public Test
	{
		/* ==================== Constructors and destructors begin ==================== */
	protected:
		explicit TransformTestBase(const math::Transform& transform);
		virtual ~TransformTestBase();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		math::Transform m_transform;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class TransformTestBase */

	class TransformTestCompare : public TransformTestBase
	{
	public:
		TransformTestCompare(const math::Transform& transform1, const math::Transform& transform2, bool expectedCompareResult);
		virtual ~TransformTestCompare();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		math::Transform m_compareTransform;
		bool m_expectedCompareResult;
	}; /* end class TransformTestCompare */

	class TransformTestParent : public TransformTestBase
	{
	public:
		TransformTestParent(const math::Transform& parentTransform, const math::Transform& childTransform, const math::Transform& expectedChildFinalTransform);
		virtual ~TransformTestParent();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		math::Transform m_childTransform;
		math::Transform m_expectedChildFinalTransform;
	}; /* end class TransformTestParent */

} /* end namespace MathTest */

#endif /* __MATH_TEST_TRANSFORM_TEST_GROUP_H__ */