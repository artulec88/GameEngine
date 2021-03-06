#include "MatrixTestGroup.h"
#include "Def.h"

#include "Utility/ILogger.h"

using namespace math_test;

/* ==================== class MatrixTestGroup begin ==================== */
MatrixTestGroup::MatrixTestGroup(const std::string& matrixTestGroupName, int testTimeIterationsCount) :
	TestGroup(matrixTestGroupName, testTimeIterationsCount)
{
	NOTICE_LOG_MATH_TEST("Matrix tests started");
}


MatrixTestGroup::~MatrixTestGroup()
{
	NOTICE_LOG_MATH_TEST("Matrix tests finished");
}

/* ==================== class MatrixTestGroup end ==================== */

/* ==================== class MatrixTestBase begin ==================== */
MatrixTestBase::MatrixTestBase(const math::Matrix4D& matrix) :
	Test(),
	m_matrix(matrix)
{
}

MatrixTestBase::~MatrixTestBase()
{
}
/* ==================== class MatrixTestBase end ==================== */

/* ==================== class MatrixTestCompare begin ==================== */
MatrixTestCompare::MatrixTestCompare(const math::Matrix4D& matrix1, const math::Matrix4D& matrix2, bool expectedCompareResult) :
	MatrixTestBase(matrix1),
	m_compareMatrix(matrix2),
	m_expectedCompareResult(expectedCompareResult)
{
}

MatrixTestCompare::~MatrixTestCompare()
{
}

void MatrixTestCompare::StartTest()
{
	bool compareResult = (m_matrix == m_compareMatrix);
	CHECK_CONDITION_ALWAYS_MATH_TEST(compareResult == m_expectedCompareResult, utility::logging::ERR,
		"The matrix ", m_matrix, " and matrix ", m_compareMatrix, " comparison result is not as expected.");

	// TODO: Test != operator
}

void MatrixTestCompare::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class MatrixTestCompare end ==================== */


/* ==================== class MatrixTestMultiplyByMatrixOperator begin ==================== */
MatrixTestMultiplyByMatrixOperator::MatrixTestMultiplyByMatrixOperator(const math::Matrix4D& matrix1, const math::Matrix4D& matrix2, const math::Matrix4D& expectedMultiplyResultMatrix) :
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
	math::Matrix4D multiplyResultMatrix = m_matrix * m_matrix2; // FIXME: Check matrix multiplication
	CHECK_CONDITION_ALWAYS_MATH_TEST(multiplyResultMatrix == m_expectedMultiplyResultMatrix, utility::logging::ERR,
		"The multiplication of matrices:\n", m_matrix, " and:\n", m_matrix2, " equals:\n", multiplyResultMatrix,
		". It is different than expected:\n", m_expectedMultiplyResultMatrix);
}

void MatrixTestMultiplyByMatrixOperator::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class MatrixTestMultiplyByMatrixOperator end ==================== */


/* ==================== class MatrixTestMultiplyByVectorOperator begin ==================== */
MatrixTestMultiplyByVectorOperator::MatrixTestMultiplyByVectorOperator(const math::Matrix4D& matrix, const math::Vector3D& vector, const math::Vector3D& expectedMultiplyResultVector) :
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
	math::Vector3D multiplyResultVector = m_matrix * m_vector; // FIXME: Check matrix multiplication
	CHECK_CONDITION_ALWAYS_MATH_TEST(multiplyResultVector == m_expectedMultiplyResultVector, utility::logging::ERR,
		"The multiplication of matrix:\n", m_matrix, " and vector ", m_vector, " equals ", multiplyResultVector,
		". It is different than expected ", m_expectedMultiplyResultVector, ".");
}

void MatrixTestMultiplyByVectorOperator::StartTimeTest(unsigned int iterationsCount)
{
}
/* ==================== class MatrixTestMultiplyByVectorOperator end ==================== */