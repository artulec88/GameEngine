#include "MatrixTestGroup.h"
#include "Def.h"

using namespace MathTest;

/* ==================== class MatrixTestGroup begin ==================== */
MatrixTestGroup::MatrixTestGroup(void) :
	TestGroup()
{
	NOTICE_LOG_MATH_TEST("Matrix tests started");
}


MatrixTestGroup::~MatrixTestGroup(void)
{
	NOTICE_LOG_MATH_TEST("Matrix tests finished");
}

/* ==================== class MatrixTestGroup end ==================== */

/* ==================== class MatrixTestBase begin ==================== */
MatrixTestBase::MatrixTestBase(const Math::Matrix4D& matrix) :
	Test(),
	m_matrix(matrix)
{
}

MatrixTestBase::~MatrixTestBase(void)
{
}
/* ==================== class MatrixTestBase end ==================== */

/* ==================== class MatrixTestCompare begin ==================== */
MatrixTestCompare::MatrixTestCompare(const Math::Matrix4D& matrix1, const Math::Matrix4D& matrix2, bool expectedCompareResult) :
	MatrixTestBase(matrix1),
	m_compareMatrix(matrix2),
	m_expectedCompareResult(expectedCompareResult)
{
}

MatrixTestCompare::~MatrixTestCompare(void)
{
}

void MatrixTestCompare::StartTest()
{
	bool compareResult = (m_matrix == m_compareMatrix);
	CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult == m_expectedCompareResult, Utility::Logging::ERR,
		"The matrix ", m_matrix, " and matrix ", m_compareMatrix, " comparison result is not as expected.");

	// TODO: Test != operator
}

void MatrixTestCompare::StartTimeTest()
{
}
/* ==================== class MatrixTestCompare end ==================== */


/* ==================== class MatrixTestMultiplyByMatrixOperator begin ==================== */
MatrixTestMultiplyByMatrixOperator::MatrixTestMultiplyByMatrixOperator(const Math::Matrix4D& matrix1, const Math::Matrix4D& matrix2, const Math::Matrix4D& expectedMultiplyResultMatrix) :
	MatrixTestBase(matrix1),
	m_matrix2(matrix2),
	m_expectedMultiplyResultMatrix(expectedMultiplyResultMatrix)
{
}
MatrixTestMultiplyByMatrixOperator::~MatrixTestMultiplyByMatrixOperator()
{
}

void MatrixTestMultiplyByMatrixOperator::StartTest()
{
	Math::Matrix4D multiplyResultMatrix = m_matrix * m_matrix2; // FIXME: Check matrix multiplication
	CHECK_CONDITION_ALWAYS_MATH_TEST(multiplyResultMatrix == m_expectedMultiplyResultMatrix, Utility::Logging::ERR,
		"The multiplication of matrices:\n", m_matrix, " and:\n", m_matrix2, " equals:\n", multiplyResultMatrix,
		". It is different than expected:\n", m_expectedMultiplyResultMatrix);
}

void MatrixTestMultiplyByMatrixOperator::StartTimeTest()
{
}
/* ==================== class MatrixTestMultiplyByMatrixOperator end ==================== */


/* ==================== class MatrixTestMultiplyByVectorOperator begin ==================== */
MatrixTestMultiplyByVectorOperator::MatrixTestMultiplyByVectorOperator(const Math::Matrix4D& matrix, const Math::Vector3D& vector, const Math::Vector3D& expectedMultiplyResultVector) :
	MatrixTestBase(matrix),
	m_vector(vector),
	m_expectedMultiplyResultVector(expectedMultiplyResultVector)
{
}
MatrixTestMultiplyByVectorOperator::~MatrixTestMultiplyByVectorOperator()
{
}

void MatrixTestMultiplyByVectorOperator::StartTest()
{
	Math::Vector3D multiplyResultVector = m_matrix * m_vector; // FIXME: Check matrix multiplication
	CHECK_CONDITION_ALWAYS_MATH_TEST(multiplyResultVector == m_expectedMultiplyResultVector, Utility::Logging::ERR,
		"The multiplication of matrix:\n", m_matrix, " and vector ", m_vector, " equals ", multiplyResultVector,
		". It is different than expected ", m_expectedMultiplyResultVector, ".");
}

void MatrixTestMultiplyByVectorOperator::StartTimeTest()
{
}
/* ==================== class MatrixTestMultiplyByVectorOperator end ==================== */