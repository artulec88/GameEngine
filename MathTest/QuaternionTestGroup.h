#ifndef __MATH_TEST_QUATERNION_TEST_GROUP_H__
#define __MATH_TEST_QUATERNION_TEST_GROUP_H__

#include "Test.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace math_test
{

	class QuaternionTestGroup : public TestGroup
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		QuaternionTestGroup(const std::string& quaternionTestGroupName, int testTimeIterationsCount);
		virtual ~QuaternionTestGroup();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class QuaternionTestGroup */

	class QuaternionTestBase : public Test
	{
		/* ==================== Constructors and destructors begin ==================== */
	protected:
		explicit QuaternionTestBase(const Math::Quaternion& quaternion);
		virtual ~QuaternionTestBase();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Math::Quaternion m_quaternion;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class QuaternionTestBase */

	class QuaternionTestCompare : public QuaternionTestBase
	{
	public:
		QuaternionTestCompare(const Math::Quaternion& quaternion1, const Math::Quaternion& quaternion2, bool expectedCompareResult);
		virtual ~QuaternionTestCompare();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Quaternion m_compareQuaternion;
		bool m_expectedCompareResult;
	}; /* end class QuaternionTestCompare */

	class QuaternionTestLength : public QuaternionTestBase
	{
	public:
		QuaternionTestLength(const Math::Quaternion& quaternion, Math::Real expectedLength);
		virtual ~QuaternionTestLength();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Real m_expectedLength;
	}; /* end class QuaternionTestLength */

	class QuaternionTestConjugate : public QuaternionTestBase
	{
	public:
		QuaternionTestConjugate(const Math::Quaternion& quaternion, const Math::Quaternion& expectedConjugate);
		virtual ~QuaternionTestConjugate();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Quaternion m_expectedConjugate;
	}; /* end class QuaternionTestConjugate */

	class QuaternionTestAddOperator : public QuaternionTestBase
	{
	public:
		QuaternionTestAddOperator(const Math::Quaternion& quaternion, const Math::Quaternion& quaternion2, const Math::Quaternion& expectedSumQuaternion);
		virtual ~QuaternionTestAddOperator();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Quaternion m_quaternion2;
		Math::Quaternion m_expectedSumQuaternion;
	}; /* end class QuaternionTestAddOperator */

	class QuaternionTestSubtractOperator : public QuaternionTestBase
	{
	public:
		QuaternionTestSubtractOperator(const Math::Quaternion& quaternion, const Math::Quaternion& quaternion2, const Math::Quaternion& expectedSubtractQuaternion);
		virtual ~QuaternionTestSubtractOperator();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Quaternion m_quaternion2;
		Math::Quaternion m_expectedSubtractQuaternion;
	}; /* end class QuaternionTestSubtractOperator */

	template <class T>
	class QuaternionTestMultiplyOperator : public QuaternionTestBase
	{
	public:
		QuaternionTestMultiplyOperator(const Math::Quaternion& quaternion, const T& val, const Math::Quaternion& expectedMultiplyQuaternion) :
			QuaternionTestBase(quaternion),
			m_val(val),
			m_expectedMultiplyQuaternion(expectedMultiplyQuaternion)
		{
		}
		virtual ~QuaternionTestMultiplyOperator()
		{
		}
	public:
		virtual void StartTest() override
		{
			Math::Quaternion multiplyQuaternion = m_quaternion * m_val;
			// TODO: what if m_val is a Math::Real? It doesn't have ToString() method
			CHECK_CONDITION_ALWAYS_MATH_TEST(multiplyQuaternion == m_expectedMultiplyQuaternion, utility::logging::ERR,
				"The multiplication of quaternion ", m_quaternion, " and ", m_val, " is a quaternion ", multiplyQuaternion,
				". It is different than expected ", m_expectedMultiplyQuaternion);
		}
		virtual void StartTimeTest(unsigned int iterationsCount) override
		{
		}
	protected:
		T m_val;
		Math::Quaternion m_expectedMultiplyQuaternion;
	}; /* end class QuaternionTestMultiplyOperator */

	class QuaternionTestDivideOperator : public QuaternionTestBase
	{
	public:
		QuaternionTestDivideOperator(const Math::Quaternion& quaternion, Math::Real divideValue, const Math::Quaternion& expectedDivideQuaternion);
		virtual ~QuaternionTestDivideOperator();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Real m_divideValue;
		Math::Quaternion m_expectedDivideQuaternion;
	}; /* end class QuaternionTestDivideOperator */

	class QuaternionTestNormalize : public QuaternionTestBase
	{
	public:
		QuaternionTestNormalize(const Math::Quaternion& quaternion, const Math::Quaternion& expectedNormalizedQuaternion);
		virtual ~QuaternionTestNormalize();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Quaternion m_expectedNormalizedQuaternion;
	}; /* end class QuaternionTestNormalize */

	class QuaternionTestDot : public QuaternionTestBase
	{
	public:
		QuaternionTestDot(const Math::Quaternion& quaternion, const Math::Quaternion& quaternion2, Math::Real expectedDotResult);
		virtual ~QuaternionTestDot();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Quaternion m_quaternion2;
		Math::Real m_expectedDotResult;
	}; /* end class QuaternionTestDot */

	class QuaternionTestNlerp : public QuaternionTestBase
	{
	public:
		QuaternionTestNlerp(const Math::Quaternion& quaternion, const Math::Quaternion& quaternion2, Math::Real nlerpFactor, bool shortest, const Math::Quaternion& expectedNlerpQuaternion);
		virtual ~QuaternionTestNlerp();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Quaternion m_quaternion2;
		Math::Real m_nlerpFactor;
		bool m_shortest;
		Math::Quaternion m_expectedNlerpQuaternion;
	}; /* end class QuaternionTestNlerp */

	class QuaternionTestSlerp : public QuaternionTestBase
	{
	public:
		QuaternionTestSlerp(const Math::Quaternion& quaternion, const Math::Quaternion& quaternion2, Math::Real slerpFactor, bool shortest, const Math::Quaternion& expectedSlerpQuaternion);
		virtual ~QuaternionTestSlerp();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Quaternion m_quaternion2;
		Math::Real m_slerpFactor;
		bool m_shortest;
		Math::Quaternion m_expectedSlerpQuaternion;
	}; /* end class QuaternionTestSlerp */

	class QuaternionTestForward : public QuaternionTestBase
	{
	public:
		QuaternionTestForward(const Math::Quaternion& quaternion, const Math::Vector3D& expectedForwardVector);
		virtual ~QuaternionTestForward();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Vector3D m_expectedForwardVector;
	}; /* end class QuaternionTestForward */

	class QuaternionTestUp : public QuaternionTestBase
	{
	public:
		QuaternionTestUp(const Math::Quaternion& quaternion, const Math::Vector3D& expectedUpVector);
		virtual ~QuaternionTestUp();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Vector3D m_expectedUpVector;
	}; /* end class QuaternionTestUp */

	class QuaternionTestRight : public QuaternionTestBase
	{
	public:
		QuaternionTestRight(const Math::Quaternion& quaternion, const Math::Vector3D& expectedRightVector);
		virtual ~QuaternionTestRight();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Vector3D m_expectedRightVector;
	}; /* end class QuaternionTestRight */

	class QuaternionTestBack : public QuaternionTestBase
	{
	public:
		QuaternionTestBack(const Math::Quaternion& quaternion, const Math::Vector3D& expectedBackVector);
		virtual ~QuaternionTestBack();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Vector3D m_expectedBackVector;
	}; /* end class QuaternionTestBack */

	class QuaternionTestDown : public QuaternionTestBase
	{
	public:
		QuaternionTestDown(const Math::Quaternion& quaternion, const Math::Vector3D& expectedDownVector);
		virtual ~QuaternionTestDown();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Vector3D m_expectedDownVector;
	}; /* end class QuaternionTestDown */

	class QuaternionTestLeft : public QuaternionTestBase
	{
	public:
		QuaternionTestLeft(const Math::Quaternion& quaternion, const Math::Vector3D& expectedLeftVector);
		virtual ~QuaternionTestLeft();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Vector3D m_expectedLeftVector;
	}; /* end class QuaternionTestLeft */

	class QuaternionTestRotationMatrix : public QuaternionTestBase
	{
	public:
		QuaternionTestRotationMatrix(const Math::Quaternion& quaternion, const Math::Matrix4D& expectedRotationMatrix);
		virtual ~QuaternionTestRotationMatrix();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		Math::Matrix4D m_expectedRotationMatrix;
	}; /* end class QuaternionTestRotationMatrix */

} /* end namespace MathTest */

#endif /* __MATH_TEST_QUATERNION_TEST_GROUP_H__ */