#ifndef __MATH_TEST_VECTOR_TEST_GROUP_H__
#define __MATH_TEST_VECTOR_TEST_GROUP_H__

#include "Test.h"
#include "Def.h"
#include "Math/Vector.h"
#include "Math/Angle.h"

namespace math_test
{

	class VectorTestGroup : public TestGroup
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		VectorTestGroup(const std::string& vectorTestGroupName, int testTimeIterationsCount);
		virtual ~VectorTestGroup();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class VectorTestGroup */

	template <class T>
	class VectorTestBase : public Test
	{
		/* ==================== Constructors and destructors begin ==================== */
	protected:
		explicit VectorTestBase(const T& vector) :
			Test(),
			m_vector(vector)
		{
		}
		virtual ~VectorTestBase()
		{
		}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		T m_vector;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class VectorTestBase */

	template <class T>
	class VectorTestCompare : public VectorTestBase<T>
	{
	public:
		VectorTestCompare(const T& vector1, const T& vector2, bool expectedCompareResult) :
			VectorTestBase<T>(vector1),
			m_compareVector(vector2),
			m_expectedCompareResult(expectedCompareResult)
		{
		}
		virtual ~VectorTestCompare()
		{
		}
	public:
		virtual void StartTest() override
		{
			bool compareResult = (m_vector == m_compareVector);
			CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult == m_expectedCompareResult, utility::logging::ERR,
				"The vector ", m_vector, " and vector ", m_compareVector, " comparison result not as expected.");
			bool diffCompareResult = (m_vector != m_compareVector);
			CHECK_CONDITION_ALWAYS_MATH_TEST(diffCompareResult != m_expectedCompareResult, utility::logging::ERR,
				"The vector ", m_vector, " and vector ", m_compareVector, " diff operator result not as expected.");
			CHECK_CONDITION_ALWAYS_MATH_TEST(diffCompareResult != compareResult, utility::logging::ERR,
				"The operators == and != should always give opposite results.");
		}
		virtual void StartTimeTest(unsigned int iterationsCount) override
		{
			for (unsigned int i = 0; i < iterationsCount; ++i)
			{
				bool compareResult = (m_vector == m_compareVector);
			}
		}
	protected:
		T m_compareVector;
		bool m_expectedCompareResult;
	}; /* end class VectorTestCompare */

	template <class T>
	class VectorTestLength : public VectorTestBase<T>
	{
	public:
		VectorTestLength(const T& vector, math::Real expectedLength) :
			VectorTestBase<T>(vector),
			m_expectedLength(expectedLength)
		{
		}
		virtual ~VectorTestLength()
		{
		}
	public:
		virtual void StartTest() override
		{
			math::Real lengthSquared = m_vector.LengthSquared();
			math::Real length = m_vector.Length();
			CHECK_CONDITION_ALWAYS_MATH_TEST(math::AlmostEqual(length, m_expectedLength), utility::logging::ERR,
				"The vector ", m_vector, " has length ", length, ", but was expected to have length ", m_expectedLength);
			CHECK_CONDITION_ALWAYS_MATH_TEST(math::AlmostEqual(lengthSquared, m_expectedLength * m_expectedLength), utility::logging::ERR,
				"The vector ", m_vector, " has squared length ", lengthSquared, ", but expected to have squared length ", m_expectedLength * m_expectedLength);
		}
		virtual void StartTimeTest(unsigned int iterationsCount) override
		{
			for (unsigned int i = 0; i < iterationsCount; ++i)
			{
				math::Real length = m_vector.Length();
			}
		}
	protected:
		math::Real m_expectedLength;
	}; /* end class VectorTestLength */

	template <class T>
	class VectorTestAddOperator : public VectorTestBase<T>
	{
	public:
		VectorTestAddOperator(const T& vector1, const T& vector2, const T& expectedSumVector) :
			VectorTestBase<T>(vector1),
			m_vector2(vector2),
			m_expectedSumVector(expectedSumVector)
		{
		}
		virtual ~VectorTestAddOperator()
		{
		}
	public:
		virtual void StartTest() override
		{
			T sumVector = m_vector + m_vector2;
			CHECK_CONDITION_ALWAYS_MATH_TEST(sumVector == m_expectedSumVector, utility::logging::ERR,
				"The sum of vectors ", m_vector, " and ", m_vector2, " is a vector ", sumVector,
				". It is different than expected ", m_expectedSumVector);

			m_vector += m_vector2;
			CHECK_CONDITION_ALWAYS_MATH_TEST(m_vector == m_expectedSumVector, utility::logging::ERR,
				"The operator += returned different result than operator + (", m_vector, " != ", m_expectedSumVector, ")");
		}
		virtual void StartTimeTest(unsigned int iterationsCount) override
		{
			for (unsigned int i = 0; i < iterationsCount; ++i)
			{
				T sumVector = m_vector + m_vector2;
			}
		}
	protected:
		T m_vector2;
		T m_expectedSumVector;
	}; /* end class VectorTestAddOperator */

	template <class T>
	class VectorTestSubtractOperator : public VectorTestBase<T>
	{
	public:
		VectorTestSubtractOperator(const T& vector1, const T& vector2, const T& expectedSubtractVector) :
			VectorTestBase<T>(vector1),
			m_vector2(vector2),
			m_expectedSubtractVector(expectedSubtractVector)
		{
		}
		virtual ~VectorTestSubtractOperator()
		{
		}
	public:
		virtual void StartTest() override
		{
			T subtractVector = m_vector - m_vector2;
			CHECK_CONDITION_ALWAYS_MATH_TEST(subtractVector == m_expectedSubtractVector, utility::logging::ERR,
				"The subtraction of vectors", m_vector, " and ", m_vector2, " is a vector ",
				subtractVector, ". It is different than expected ", m_expectedSubtractVector);

			m_vector -= m_vector2;
			CHECK_CONDITION_ALWAYS_MATH_TEST(m_vector == m_expectedSubtractVector, utility::logging::ERR,
				"The operator -= returned different result than operator - (", m_vector, " != ", m_expectedSubtractVector, ")");
		}
		virtual void StartTimeTest(unsigned int iterationsCount) override
		{
			for (unsigned int i = 0; i < iterationsCount; ++i)
			{
				T subtractVector = m_vector - m_vector2;
			}
		}
	protected:
		T m_vector2;
		T m_expectedSubtractVector;
	}; /* end class VectorTestSubtractOperator */


	template <class T>
	class VectorTestMultiplyOperator : public VectorTestBase<T>
	{
	public:
		VectorTestMultiplyOperator(const T& vector1, const T& vector2, math::Real value, const T& expectedMultiplyVector1, const T& expectedMultiplyVector2) :
			VectorTestBase<T>(vector1),
			m_vector2(vector2),
			m_value(value),
			m_expectedMultiplyVector1(expectedMultiplyVector1),
			m_expectedMultiplyVector2(expectedMultiplyVector2)
		{
		}
		virtual ~VectorTestMultiplyOperator()
		{
		}
	public:
		virtual void StartTest() override
		{
			T multiplyVector1 = m_vector * m_vector2;
			T multiplyVector2 = m_vector * m_value;
			CHECK_CONDITION_ALWAYS_MATH_TEST(multiplyVector1 == m_expectedMultiplyVector1, utility::logging::ERR,
				"The multiplication of vectors ", m_vector, " and ", m_vector2, " is a vector ",
				multiplyVector1, ". It is different than expected ", m_expectedMultiplyVector1);
			CHECK_CONDITION_ALWAYS_MATH_TEST(multiplyVector2 == m_expectedMultiplyVector2, utility::logging::ERR,
				"The multiplication of vector ", m_vector, " and value ", m_value, " is a vector ",
				multiplyVector2, ". It is different than expected ", m_expectedMultiplyVector2);

			T tempVector(m_vector);
			m_vector *= m_vector2;
			CHECK_CONDITION_ALWAYS_MATH_TEST(m_vector == m_expectedMultiplyVector1, utility::logging::ERR,
				"The operator *= returned different result than operator * (", m_vector, " != ", m_expectedMultiplyVector1, ")");
			m_vector = tempVector;
			m_vector *= m_value;
			CHECK_CONDITION_ALWAYS_MATH_TEST(m_vector == m_expectedMultiplyVector2, utility::logging::ERR,
				"The operator *= returned different result than operator * (", m_vector, " != ", m_expectedMultiplyVector2, ")");
		}
		virtual void StartTimeTest(unsigned int iterationsCount) override
		{
			for (unsigned int i = 0; i < iterationsCount; ++i)
			{
				T multiplyVector1 = m_vector * m_vector2;
			}
		}
	protected:
		T m_vector2;
		math::Real m_value;
		T m_expectedMultiplyVector1;
		T m_expectedMultiplyVector2;
	}; /* end class VectorTestMultiplyOperator */

	template <class T>
	class VectorTestDivideOperator : public VectorTestBase<T>
	{
	public:
		VectorTestDivideOperator(const T& vector1, const T& vector2, math::Real value, const T& expectedDivideVector1, const T& expectedDivideVector2) :
			VectorTestBase<T>(vector1),
			m_vector2(vector2),
			m_value(value),
			m_expectedDivideVector1(expectedDivideVector1),
			m_expectedDivideVector2(expectedDivideVector2)
		{
		}
		virtual ~VectorTestDivideOperator()
		{
		}
	public:
		virtual void StartTest() override
		{
			T divideVector1 = m_vector / m_vector2;
			T divideVector2 = m_vector / m_value;
			CHECK_CONDITION_ALWAYS_MATH_TEST(divideVector1 == m_expectedDivideVector1, utility::logging::ERR,
				"The division of vectors ", m_vector, " and ", m_vector2, " is a vector ",
				divideVector1, ". It is different than expected ", m_expectedDivideVector1);
			CHECK_CONDITION_ALWAYS_MATH_TEST(divideVector2 == m_expectedDivideVector2, utility::logging::ERR,
				"The division of vector ", m_vector, " and value ", m_value, " is a vector ",
				divideVector2, ". It is different than expected ", m_expectedDivideVector2);

			T tempVector(m_vector);
			m_vector /= m_vector2;
			CHECK_CONDITION_ALWAYS_MATH_TEST(m_vector == m_expectedDivideVector1, utility::logging::ERR,
				"The operator /= returned different result than operator / (", m_vector, " != ", m_expectedDivideVector1, ")");
			m_vector = tempVector;
			m_vector /= m_value;
			CHECK_CONDITION_ALWAYS_MATH_TEST(m_vector == m_expectedDivideVector2, utility::logging::ERR,
				"The operator /= returned different result than operator / (", m_vector, " != ", m_expectedDivideVector2, ")");
		}
		virtual void StartTimeTest(unsigned int iterationsCount) override
		{
			for (unsigned int i = 0; i < iterationsCount; ++i)
			{
				T divideVector1 = m_vector / m_vector2;
			}
		}
	protected:
		T m_vector2;
		math::Real m_value;
		T m_expectedDivideVector1;
		T m_expectedDivideVector2;
	}; /* end class VectorTestDivideOperator */

	template <class T>
	class VectorTestNormalize : public VectorTestBase<T>
	{
	public:
		VectorTestNormalize(const T& vector, const T& expectedNormalizedVector) :
			VectorTestBase<T>(vector),
			m_expectedNormalizedVector(expectedNormalizedVector)
		{
			math::Real lengthSquared = m_expectedNormalizedVector.LengthSquared();
			math::Real length = m_expectedNormalizedVector.Length();
			CHECK_CONDITION_ALWAYS_MATH_TEST(math::AlmostEqual(lengthSquared, REAL_ONE), utility::logging::ERR,
				"Given expected normalized vector ", m_expectedNormalizedVector, " is in fact not normalized.");
			CHECK_CONDITION_ALWAYS_MATH_TEST(math::AlmostEqual(length, lengthSquared), utility::logging::ERR,
				"Given expected normalized vector ", m_expectedNormalizedVector, " gives different results for length and squared length (", length, " and ", lengthSquared, " respectively).");
			CHECK_CONDITION_ALWAYS_MATH_TEST(m_expectedNormalizedVector.IsNormalized(), utility::logging::ERR,
				"Given expected normalized vector ", m_expectedNormalizedVector, " is in fact not normalized.");
		}
		virtual ~VectorTestNormalize()
		{
		}
	public:
		virtual void StartTest() override
		{
			T normalizedVector = m_vector.Normalized();
			CHECK_CONDITION_ALWAYS_MATH_TEST(normalizedVector == m_expectedNormalizedVector, utility::logging::ERR,
				"The vector ", m_vector, " after normalization equals ", normalizedVector, ". It is different than expected ", m_expectedNormalizedVector, ".");
			CHECK_CONDITION_ALWAYS_MATH_TEST(normalizedVector.IsNormalized(), utility::logging::ERR,
				"Calculated normalized vector ", normalizedVector, " is in fact not normalized.");

			m_vector.Normalize();
			CHECK_CONDITION_ALWAYS_MATH_TEST(m_vector == m_expectedNormalizedVector, utility::logging::ERR,
				"The vector after normalization ", m_vector, " is different than expected ", m_expectedNormalizedVector);
			CHECK_CONDITION_ALWAYS_MATH_TEST(m_vector.IsNormalized(), utility::logging::ERR,
				"Calculated normalized vector ", m_vector, " is in fact not normalized.");
		}
		virtual void StartTimeTest(unsigned int iterationsCount) override
		{
			for (unsigned int i = 0; i < iterationsCount; ++i)
			{
				T normalizedVector = m_vector.Normalized();
			}
		}
	protected:
		T m_expectedNormalizedVector;
	}; /* end class VectorTestNormalize */


	class Vector2DTestCross : public VectorTestBase<math::Vector2D>
	{
	public:
		Vector2DTestCross(const math::Vector2D& vector1, const math::Vector2D& vector2, math::Real expectedCrossResult);
		virtual ~Vector2DTestCross();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		math::Vector2D m_vector2;
		math::Real m_expectedCrossResult;
	}; /* end class Vector2DTestCross */

	class Vector3DTestCross : public VectorTestBase<math::Vector3D>
	{
	public:
		Vector3DTestCross(const math::Vector3D& vector1, const math::Vector3D& vector2, const math::Vector3D& expectedCrossResult);
		virtual ~Vector3DTestCross();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		math::Vector3D m_vector2;
		math::Vector3D m_expectedCrossResult;
	}; /* end class Vector3DTestCross */

	template <class T>
	class VectorTestDot : public VectorTestBase<T>
	{
	public:
		VectorTestDot(const T& vector1, const T& vector2, math::Real expectedDotResult) :
			VectorTestBase<T>(vector1),
			m_vector2(vector2),
			m_expectedDotResult(expectedDotResult)
		{
		}
		virtual ~VectorTestDot()
		{
		}
	public:
		virtual void StartTest() override
		{
			math::Real dotResult = m_vector.Dot(m_vector2);
			CHECK_CONDITION_ALWAYS_MATH_TEST(math::AlmostEqual(dotResult, m_expectedDotResult), utility::ERR,
				"The dot product of vectors ", m_vector.ToString(), " and ", m_vector2.ToString(), " equals ", dotResult, ". It is different than expected ", m_expectedDotResult);
		}
		virtual void StartTimeTest(unsigned int iterationsCount) override
		{
			for (unsigned int i = 0; i < iterationsCount; ++i)
			{
				math::Real dotResult = m_vector.Dot(m_vector2);
			}
		}
	protected:
		T m_vector2;
		math::Real m_expectedDotResult;
	}; /* end class VectorTestDot */

	//class Vector2DTestRotate : public VectorTestBase<math::Vector2D>
	//{
	//public:
	//	Vector2DTestRotate(const math::Vector2D& vec1, const math::Angle& angle, const math::Vector2D& expectedRotateVector);
	//	virtual ~Vector2DTestRotate();
	//public:
	//	virtual void StartTest();
	//protected:
	//	math::Angle m_angle;
	//	math::Vector2D m_expectedRotateVector;
	//}; /* end class Vector2DTestRotate */
	//
	//class Vector2DTestLerp : public VectorTestBase<math::Vector2D>
	//{
	//public:
	//	Vector2DTestLerp(const math::Vector2D& vector1, const math::Vector2D& vector2, math::Real lerpFactor, const math::Vector2D& expectedLerpVector);
	//	virtual ~Vector2DTestLerp();
	//public:
	//	virtual void StartTest();
	//protected:
	//	math::Vector2D m_vector2;
	//	math::Real m_lerpFactor;
	//	math::Vector2D m_expectedLerpVector;
	//}; /* end class Vector2DTestLerp */

} /* end namespace MathTest */

#endif /* __MATH_TEST_VECTOR_TEST_GROUP_H__ */