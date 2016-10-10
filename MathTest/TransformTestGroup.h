#ifndef __MATH_TEST_TRANSFORM_TEST_GROUP_H__
#define __MATH_TEST_TRANSFORM_TEST_GROUP_H__

#include "Test.h"
#include "Math\Transform.h"

namespace MathTest
{

	class TransformTestGroup : public TestGroup
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		TransformTestGroup(void);
		virtual ~TransformTestGroup(void);
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
		TransformTestBase(const Math::Transform& transform);
		virtual ~TransformTestBase(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Math::Transform m_transform;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class TransformTestBase */

	class TransformTestCompare : public TransformTestBase
	{
	public:
		TransformTestCompare(const Math::Transform& transform1, const Math::Transform& transform2, bool expectedCompareResult);
		virtual ~TransformTestCompare();
	public:
		virtual void StartTest();
		virtual void StartTimeTest();
	protected:
		Math::Transform m_compareTransform;
		bool m_expectedCompareResult;
	}; /* end class TransformTestCompare */

	class TransformTestParent : public TransformTestBase
	{
	public:
		TransformTestParent(const Math::Transform& parentTransform, const Math::Transform& childTransform, const Math::Transform& expectedChildFinalTransform);
		virtual ~TransformTestParent();
	public:
		virtual void StartTest();
		virtual void StartTimeTest();
	protected:
		Math::Transform m_childTransform;
		Math::Transform m_expectedChildFinalTransform;
	}; /* end class TransformTestParent */

} /* end namespace MathTest */

#endif /* __MATH_TEST_TRANSFORM_TEST_GROUP_H__ */