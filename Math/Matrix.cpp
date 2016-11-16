#include "StdAfx.h"
#include "Matrix.h"
#include "FloatingPoint.h"

#include "Utility\Utility.h"

#include <sstream>
#include <iostream>
#include <utility>

#include <xmmintrin.h>

/* static */ const Math::Matrix4D Math::Matrix4D::IDENTITY_MATRIX;

Math::Matrix4D::Matrix4D(const Angle& angleX, const Angle& angleY) :
	Matrix4D()
{
	START_PROFILING_MATH(false, "7");
	const Real xSin = angleX.Sin();
	const Real xCos = angleX.Cos();
	const Real ySin = angleY.Sin();
	const Real yCos = angleY.Cos();
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = yCos;			m_values[0][1] = REAL_ZERO;	m_values[0][2] = ySin;			m_values[0][3] = REAL_ZERO;
	m_values[1][0] = -xSin * ySin;	m_values[1][1] = xCos;		m_values[1][2] = xSin * yCos;	m_values[1][3] = REAL_ZERO;
	m_values[2][0] = -xCos * ySin;	m_values[2][1] = -xSin;		m_values[2][2] = xCos * yCos;	m_values[2][3] = REAL_ZERO;
	m_values[3][0] = REAL_ZERO;		m_values[3][1] = REAL_ZERO;	m_values[3][2] = REAL_ZERO;		m_values[3][3] = REAL_ONE;
#else
	m_values[0] = yCos;			m_values[1] = REAL_ZERO;	m_values[2] = ySin;			m_values[3] = REAL_ZERO;
	m_values[4] = -xSin * ySin;	m_values[5] = xCos;			m_values[6] = xSin * yCos;	m_values[7] = REAL_ZERO;
	m_values[8] = -xCos * ySin;	m_values[9] = -xSin;		m_values[10] = xCos * yCos;	m_values[11] = REAL_ZERO;
	m_values[12] = REAL_ZERO;	m_values[13] = REAL_ZERO;	m_values[14] = REAL_ZERO;	m_values[15] = REAL_ONE;
#endif
	CHECK_CONDITION_MATH((*this) == Matrix4D(angleX, angleY, Angle(REAL_ZERO)), Utility::Logging::ERR, "Two RotationEuler functions give different results.");
	STOP_PROFILING_MATH("7");
}

Math::Matrix4D::Matrix4D(const Angle& angleX, const Angle& angleY, const Angle& angleZ) :
	Matrix4D()
{
	START_PROFILING_MATH(false, "8"); // TODO: As there are two methods with the same name "RotationEuler" their stats will be stored in the same place (which is not good). Fix it!
	const Real xSin = angleX.Sin();
	const Real xCos = angleX.Cos();
	const Real ySin = angleY.Sin();
	const Real yCos = angleY.Cos();
	const Real zSin = angleZ.Sin();
	const Real zCos = angleZ.Cos();
	/* ==================== SOLUTION #1 begin ==================== */
	//Matrix4D rotX, rotY, rotZ; // rotation around X, Y and Z axis respectively

	//rotX.m_values[0][0] = REAL_ONE;	rotX.m_values[0][1] = REAL_ZERO;	rotX.m_values[0][2] = REAL_ZERO;	rotX.m_values[0][3] = REAL_ZERO;
	//rotX.m_values[1][0] = REAL_ZERO;	rotX.m_values[1][1] = xCos;		rotX.m_values[1][2] = xSin;		rotX.m_values[1][3] = REAL_ZERO;
	//rotX.m_values[2][0] = REAL_ZERO;	rotX.m_values[2][1] = -xSin;		rotX.m_values[2][2] = xCos;		rotX.m_values[2][3] = REAL_ZERO;
	//rotX.m_values[3][0] = REAL_ZERO;	rotX.m_values[3][1] = REAL_ZERO;	rotX.m_values[3][2] = REAL_ZERO;	rotX.m_values[3][3] = REAL_ONE;

	//rotY.m_values[0][0] = yCos;		rotY.m_values[0][1] = REAL_ZERO;	rotY.m_values[0][2] = ySin;		rotY.m_values[0][3] = REAL_ZERO;
	//rotY.m_values[1][0] = REAL_ZERO;	rotY.m_values[1][1] = REAL_ONE;	rotY.m_values[1][2] = REAL_ZERO;	rotY.m_values[1][3] = REAL_ZERO;
	//rotY.m_values[2][0] = -ySin;		rotY.m_values[2][1] = REAL_ZERO;	rotY.m_values[2][2] = yCos;		rotY.m_values[2][3] = REAL_ZERO;
	//rotY.m_values[3][0] = REAL_ZERO;	rotY.m_values[3][1] = REAL_ZERO;	rotY.m_values[3][2] = REAL_ZERO;	rotY.m_values[3][3] = REAL_ONE;

	//rotZ.m_values[0][0] = zCos;		rotZ.m_values[0][1] = zSin;		rotZ.m_values[0][2] = REAL_ZERO;	rotZ.m_values[0][3] = REAL_ZERO;
	//rotZ.m_values[1][0] = -zSin;		rotZ.m_values[1][1] = zCos;		rotZ.m_values[1][2] = REAL_ZERO;	rotZ.m_values[1][3] = REAL_ZERO;
	//rotZ.m_values[2][0] = REAL_ZERO;	rotZ.m_values[2][1] = REAL_ZERO;	rotZ.m_values[2][2] = REAL_ONE;	rotZ.m_values[2][3] = REAL_ZERO;
	//rotZ.m_values[3][0] = REAL_ZERO;	rotZ.m_values[3][1] = REAL_ZERO;	rotZ.m_values[3][2] = REAL_ZERO;	rotZ.m_values[3][3] = REAL_ONE;

	//return rotZ * rotY * rotX;
	/* ==================== SOLUTION #1 end ==================== */

	/* ==================== SOLUTION #2 begin ==================== */
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = yCos * zCos;						m_values[0][1] = yCos * zSin;						m_values[0][2] = ySin;			m_values[0][3] = REAL_ZERO;
	m_values[1][0] = -xSin * ySin * zCos - xCos * zSin;	m_values[1][1] = -xSin * ySin * zSin + xCos * zCos;	m_values[1][2] = xSin * yCos;	m_values[1][3] = REAL_ZERO;
	m_values[2][0] = -xCos * ySin * zCos + xSin * zSin;	m_values[2][1] = -xCos * ySin * zSin - xSin * zCos;	m_values[2][2] = xCos * yCos;	m_values[2][3] = REAL_ZERO;
	m_values[3][0] = REAL_ZERO;							m_values[3][1] = REAL_ZERO;							m_values[3][2] = REAL_ZERO;		m_values[3][3] = REAL_ONE;
#else
	m_values[0] = yCos * zCos;							m_values[1] = yCos * zSin;							m_values[2] = ySin;				m_values[3] = REAL_ZERO;
	m_values[4] = -xSin * ySin * zCos - xCos * zSin;	m_values[5] = -xSin * ySin * zSin + xCos * zCos;	m_values[6] = xSin * yCos;		m_values[7] = REAL_ZERO;
	m_values[8] = -xCos * ySin * zCos + xSin * zSin;	m_values[9] = -xCos * ySin * zSin - xSin * zCos;	m_values[10] = xCos * yCos;		m_values[11] = REAL_ZERO;
	m_values[12] = REAL_ZERO;							m_values[13] = REAL_ZERO;							m_values[14] = REAL_ZERO;		m_values[15] = REAL_ONE;
#endif
	//Matrix4D matrixToCompare = rotZ * rotY * rotX;
	//if (rot != matrixToCompare)
	//{
	//	ERROR_LOG_MATH("Incorrect euler rotation calculation. Rot=\n", rot, "\nInstead it should be equal to:\n", matrixToCompare);
	//}
	STOP_PROFILING_MATH("8");
	/* ==================== SOLUTION #2 end ==================== */
}

Math::Matrix4D::Matrix4D(const Vector3D& forward, const Vector3D& up, const Vector3D& right) :
	Matrix4D(right.GetX(), up.GetX(), forward.GetX(), REAL_ZERO, right.GetY(), up.GetY(), forward.GetY(), REAL_ZERO, right.GetZ(), up.GetZ(), forward.GetZ(), REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE)
{
	START_PROFILING_MATH(false, "9");
	CHECK_CONDITION_MATH(right.IsNormalized(), Utility::Logging::ERR, "Cannot correctly perform the rotation. The specified right vector is not normalized.");
	CHECK_CONDITION_MATH(up.IsNormalized(), Utility::Logging::ERR, "Cannot correctly perform the rotation. The specified up vector is not normalized.");
	CHECK_CONDITION_MATH(forward.IsNormalized(), Utility::Logging::ERR, "Cannot correctly perform the rotation. The specified forward vector is not normalized.");
	STOP_PROFILING_MATH("9");
}

Math::Matrix4D::Matrix4D(const Vector3D& forward, const Vector3D& up) :
	Matrix4D()
{
	START_PROFILING_MATH(false, "10");
	Vector3D n = forward.Normalized();
	Vector3D u = up.Normalized().Cross(n);
	Vector3D v = n.Cross(u);
	SetRotationFromVectors(n, v, u);
	STOP_PROFILING_MATH("10");
}

Math::Matrix4D::Matrix4D(const Angle& fov /* Field of View */, Real aspect /* Aspect */, Real nearPlane /* Near plane */, Real farPlane /* Far plane */) :
	Matrix4D()
{
	START_PROFILING_MATH(false, "11");
	SetPerspectiveProjection(fov, aspect, nearPlane, farPlane);
	STOP_PROFILING_MATH("11");
}

Math::Matrix4D::Matrix4D(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane) :
	Matrix4D(2.0f / (right - left), REAL_ZERO, REAL_ZERO, REAL_ZERO,
		REAL_ZERO, 2.0f / (top - bottom), REAL_ZERO, REAL_ZERO,
		REAL_ZERO, REAL_ZERO, -2.0f / (farPlane - nearPlane), REAL_ZERO,
		-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farPlane + nearPlane) / (farPlane - nearPlane), REAL_ONE)
{
	START_PROFILING_MATH(false, "12");
	STOP_PROFILING_MATH("12");
}

Math::Matrix4D::Matrix4D(const Matrix4D& mat) :
	m_values(mat.m_values)
#ifdef PROFILING_MATH_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING_MATH(false, "13");
	//ERROR_LOG_MATH("Matrix copy constructor");
	CHECK_CONDITION_MATH((*this) == mat, Utility::Logging::ERR, "The copy constructor should cause the two matrices to be equal, but they are not.");
	STOP_PROFILING_MATH("13");
}

Math::Matrix4D::Matrix4D(Matrix4D&& mat) :
	m_values(std::move(mat.m_values))
#ifdef PROFILING_MATH_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING_MATH(false, "14");
	//ERROR_LOG_MATH("Matrix move constructor");
	STOP_PROFILING_MATH("14");
}

Math::Matrix4D::~Matrix4D()
{
}

Math::Matrix4D& Math::Matrix4D::operator=(const Matrix4D& mat)
{
	START_PROFILING_MATH(true, "");

	m_values = mat.m_values;
	//std::copy(mat.m_values.begin(), mat.m_values.end(), m_values.begin());
	//ERROR_LOG_MATH("Matrix copy assignment operator");

	SLOW_ASSERT((*this) == mat);
	STOP_PROFILING_MATH("");
	return *this;
}

Math::Matrix4D& Math::Matrix4D::operator=(Matrix4D&& mat)
{
	START_PROFILING_MATH(false, "");
	m_values = std::move(mat.m_values);
	//ERROR_LOG_MATH("Matrix move assignment operator");
	STOP_PROFILING_MATH("");
	return *this;
}

Math::Matrix4D::Matrix4D(const Math::Real* values) :
	Matrix4D(values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8], values[9], values[10], values[11], values[12], values[13], values[14], values[15])
{
	START_PROFILING_MATH(false, "15");
	STOP_PROFILING_MATH("15");
}

void Math::Matrix4D::M4x4_SSE(const Real* A, const Real* B, Real* C) const {
	__m128 row1 = _mm_load_ps(&B[0]);
	__m128 row2 = _mm_load_ps(&B[4]);
	__m128 row3 = _mm_load_ps(&B[8]);
	__m128 row4 = _mm_load_ps(&B[12]);
	for (int i = 0; i<4; i++) {
		__m128 brod1 = _mm_set1_ps(A[4 * i + 0]);
		__m128 brod2 = _mm_set1_ps(A[4 * i + 1]);
		__m128 brod3 = _mm_set1_ps(A[4 * i + 2]);
		__m128 brod4 = _mm_set1_ps(A[4 * i + 3]);
		__m128 row = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(brod1, row1),
				_mm_mul_ps(brod2, row2)),
			_mm_add_ps(
				_mm_mul_ps(brod3, row3),
				_mm_mul_ps(brod4, row4)));
		_mm_store_ps(&C[4 * i], row);
	}
}

Math::Matrix4D Math::Matrix4D::operator*(const Matrix4D& mat) const
{
	START_PROFILING_MATH(true, "");
	/* ==================== SOLUTION #1 (doesn't work!!!) begin ==================== */
	//Matrix4D matrix;
	//matrix.m_values[0][0] = m_values[0][0] * mat.m_values[0][0] + m_values[1][0] * mat.m_values[0][1] + m_values[2][0] * mat.m_values[0][2];
	//matrix.m_values[0][1] = m_values[0][1] * mat.m_values[0][0] + m_values[1][1] * mat.m_values[0][1] + m_values[2][1] * mat.m_values[0][2];
	//matrix.m_values[0][2] = m_values[0][2] * mat.m_values[0][0] + m_values[1][2] * mat.m_values[0][1] + m_values[2][2] * mat.m_values[0][2];
	//matrix.m_values[0][3] = REAL_ZERO;

	//matrix.m_values[1][0] = m_values[0][0] * mat.m_values[1][0] + m_values[1][0] * mat.m_values[1][1] + m_values[2][0] * mat.m_values[1][2];
	//matrix.m_values[1][1] = m_values[0][1] * mat.m_values[1][0] + m_values[1][1] * mat.m_values[1][1] + m_values[2][1] * mat.m_values[1][2];
	//matrix.m_values[1][2] = m_values[0][2] * mat.m_values[1][0] + m_values[1][2] * mat.m_values[1][1] + m_values[2][2] * mat.m_values[1][2];
	//matrix.m_values[1][3] = REAL_ZERO;

	//matrix.m_values[2][0] = m_values[0][0] * mat.m_values[2][0] + m_values[1][0] * mat.m_values[2][1] + m_values[2][0] * mat.m_values[2][2];
	//matrix.m_values[2][1] = m_values[0][1] * mat.m_values[2][0] + m_values[1][1] * mat.m_values[2][1] + m_values[2][1] * mat.m_values[2][2];
	//matrix.m_values[2][2] = m_values[0][2] * mat.m_values[2][0] + m_values[1][2] * mat.m_values[2][1] + m_values[2][2] * mat.m_values[2][2];
	//matrix.m_values[2][3] = REAL_ZERO;

	//matrix.m_values[3][0] = m_values[0][0] * mat.m_values[3][0] + m_values[1][0] * mat.m_values[3][1] + m_values[2][0] * mat.m_values[3][2] + m_values[3][0];
	//matrix.m_values[3][1] = m_values[0][1] * mat.m_values[3][0] + m_values[1][1] * mat.m_values[3][1] + m_values[2][1] * mat.m_values[3][2] + m_values[3][1];
	//matrix.m_values[3][2] = m_values[0][2] * mat.m_values[3][0] + m_values[1][2] * mat.m_values[3][1] + m_values[2][2] * mat.m_values[3][2] + m_values[3][2];
	//matrix.m_values[3][3] = REAL_ONE;
	/* ==================== SOLUTION #1 (doesn't work!!!) end ==================== */

	/* ==================== SOLUTION #2 begin ==================== */
	Matrix4D matrix;
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	matrix.m_values[0][0] = m_values[0][0] * mat.m_values[0][0] + m_values[1][0] * mat.m_values[0][1] + m_values[2][0] * mat.m_values[0][2] + m_values[3][0] * mat.m_values[0][3];
	matrix.m_values[0][1] = m_values[0][1] * mat.m_values[0][0] + m_values[1][1] * mat.m_values[0][1] + m_values[2][1] * mat.m_values[0][2] + m_values[3][1] * mat.m_values[0][3];
	matrix.m_values[0][2] = m_values[0][2] * mat.m_values[0][0] + m_values[1][2] * mat.m_values[0][1] + m_values[2][2] * mat.m_values[0][2] + m_values[3][2] * mat.m_values[0][3];
	matrix.m_values[0][3] = m_values[0][3] * mat.m_values[0][0] + m_values[1][3] * mat.m_values[0][1] + m_values[2][3] * mat.m_values[0][2] + m_values[3][3] * mat.m_values[0][3];

	matrix.m_values[1][0] = m_values[0][0] * mat.m_values[1][0] + m_values[1][0] * mat.m_values[1][1] + m_values[2][0] * mat.m_values[1][2] + m_values[3][0] * mat.m_values[1][3];
	matrix.m_values[1][1] = m_values[0][1] * mat.m_values[1][0] + m_values[1][1] * mat.m_values[1][1] + m_values[2][1] * mat.m_values[1][2] + m_values[3][1] * mat.m_values[1][3];
	matrix.m_values[1][2] = m_values[0][2] * mat.m_values[1][0] + m_values[1][2] * mat.m_values[1][1] + m_values[2][2] * mat.m_values[1][2] + m_values[3][2] * mat.m_values[1][3];
	matrix.m_values[1][3] = m_values[0][3] * mat.m_values[1][0] + m_values[1][3] * mat.m_values[1][1] + m_values[2][3] * mat.m_values[1][2] + m_values[3][3] * mat.m_values[1][3];

	matrix.m_values[2][0] = m_values[0][0] * mat.m_values[2][0] + m_values[1][0] * mat.m_values[2][1] + m_values[2][0] * mat.m_values[2][2] + m_values[3][0] * mat.m_values[2][3];
	matrix.m_values[2][1] = m_values[0][1] * mat.m_values[2][0] + m_values[1][1] * mat.m_values[2][1] + m_values[2][1] * mat.m_values[2][2] + m_values[3][1] * mat.m_values[2][3];
	matrix.m_values[2][2] = m_values[0][2] * mat.m_values[2][0] + m_values[1][2] * mat.m_values[2][1] + m_values[2][2] * mat.m_values[2][2] + m_values[3][2] * mat.m_values[2][3];
	matrix.m_values[2][3] = m_values[0][3] * mat.m_values[2][0] + m_values[1][3] * mat.m_values[2][1] + m_values[2][3] * mat.m_values[2][2] + m_values[3][3] * mat.m_values[2][3];

	matrix.m_values[3][0] = m_values[0][0] * mat.m_values[3][0] + m_values[1][0] * mat.m_values[3][1] + m_values[2][0] * mat.m_values[3][2] + m_values[3][0] * mat.m_values[3][3];
	matrix.m_values[3][1] = m_values[0][1] * mat.m_values[3][0] + m_values[1][1] * mat.m_values[3][1] + m_values[2][1] * mat.m_values[3][2] + m_values[3][1] * mat.m_values[3][3];
	matrix.m_values[3][2] = m_values[0][2] * mat.m_values[3][0] + m_values[1][2] * mat.m_values[3][1] + m_values[2][2] * mat.m_values[3][2] + m_values[3][2] * mat.m_values[3][3];
	matrix.m_values[3][3] = m_values[0][3] * mat.m_values[3][0] + m_values[1][3] * mat.m_values[3][1] + m_values[2][3] * mat.m_values[3][2] + m_values[3][3] * mat.m_values[3][3];
#else
	matrix.m_values[0] = m_values[0] * mat.m_values[0] + m_values[4] * mat.m_values[1] + m_values[8] * mat.m_values[2] + m_values[12] * mat.m_values[3];
	matrix.m_values[1] = m_values[1] * mat.m_values[0] + m_values[5] * mat.m_values[1] + m_values[9] * mat.m_values[2] + m_values[13] * mat.m_values[3];
	matrix.m_values[2] = m_values[2] * mat.m_values[0] + m_values[6] * mat.m_values[1] + m_values[10] * mat.m_values[2] + m_values[14] * mat.m_values[3];
	matrix.m_values[3] = m_values[3] * mat.m_values[0] + m_values[7] * mat.m_values[1] + m_values[11] * mat.m_values[2] + m_values[15] * mat.m_values[3];

	matrix.m_values[4] = m_values[0] * mat.m_values[4] + m_values[4] * mat.m_values[5] + m_values[8] * mat.m_values[6] + m_values[12] * mat.m_values[7];
	matrix.m_values[5] = m_values[1] * mat.m_values[4] + m_values[5] * mat.m_values[5] + m_values[9] * mat.m_values[6] + m_values[13] * mat.m_values[7];
	matrix.m_values[6] = m_values[2] * mat.m_values[4] + m_values[6] * mat.m_values[5] + m_values[10] * mat.m_values[6] + m_values[14] * mat.m_values[7];
	matrix.m_values[7] = m_values[3] * mat.m_values[4] + m_values[7] * mat.m_values[5] + m_values[11] * mat.m_values[6] + m_values[15] * mat.m_values[7];

	matrix.m_values[8] = m_values[0] * mat.m_values[8] + m_values[4] * mat.m_values[9] + m_values[8] * mat.m_values[10] + m_values[12] * mat.m_values[11];
	matrix.m_values[9] = m_values[1] * mat.m_values[8] + m_values[5] * mat.m_values[9] + m_values[9] * mat.m_values[10] + m_values[13] * mat.m_values[11];
	matrix.m_values[10] = m_values[2] * mat.m_values[8] + m_values[6] * mat.m_values[9] + m_values[10] * mat.m_values[10] + m_values[14] * mat.m_values[11];
	matrix.m_values[11] = m_values[3] * mat.m_values[8] + m_values[7] * mat.m_values[9] + m_values[11] * mat.m_values[10] + m_values[15] * mat.m_values[11];

	matrix.m_values[12] = m_values[0] * mat.m_values[12] + m_values[4] * mat.m_values[13] + m_values[8] * mat.m_values[14] + m_values[12] * mat.m_values[15];
	matrix.m_values[13] = m_values[1] * mat.m_values[12] + m_values[5] * mat.m_values[13] + m_values[9] * mat.m_values[14] + m_values[13] * mat.m_values[15];
	matrix.m_values[14] = m_values[2] * mat.m_values[12] + m_values[6] * mat.m_values[13] + m_values[10] * mat.m_values[14] + m_values[14] * mat.m_values[15];
	matrix.m_values[15] = m_values[3] * mat.m_values[12] + m_values[7] * mat.m_values[13] + m_values[11] * mat.m_values[14] + m_values[15] * mat.m_values[15];
#endif
	STOP_PROFILING_MATH("");
	return matrix;
	/* ==================== SOLUTION #2 end ==================== */

	/* ==================== SOLUTION #3 begin ==================== */
	//Matrix4D matrix;
//	const Math::Real m00 = m_values[0][0]; const Math::Real m01 = m_values[0][1]; const Math::Real m02 = m_values[0][2]; const Math::Real m03 = m_values[0][3];
//	const Math::Real m10 = m_values[1][0]; const Math::Real m11 = m_values[1][1]; const Math::Real m12 = m_values[1][2]; const Math::Real m13 = m_values[1][3];
//	const Math::Real m20 = m_values[2][0]; const Math::Real m21 = m_values[2][1]; const Math::Real m22 = m_values[2][2]; const Math::Real m23 = m_values[2][3];
//	const Math::Real m30 = m_values[3][0]; const Math::Real m31 = m_values[3][1]; const Math::Real m32 = m_values[3][2]; const Math::Real m33 = m_values[3][3];
//
//	const Math::Real b00 = mat.m_values[0][0]; const Math::Real b01 = mat.m_values[0][1]; const Math::Real b02 = mat.m_values[0][2]; const Math::Real b03 = mat.m_values[0][3];
//	const Math::Real b10 = mat.m_values[1][0]; const Math::Real b11 = mat.m_values[1][1]; const Math::Real b12 = mat.m_values[1][2]; const Math::Real b13 = mat.m_values[1][3];
//	const Math::Real b20 = mat.m_values[2][0]; const Math::Real b21 = mat.m_values[2][1]; const Math::Real b22 = mat.m_values[2][2]; const Math::Real b23 = mat.m_values[2][3];
//	const Math::Real b30 = mat.m_values[3][0]; const Math::Real b31 = mat.m_values[3][1]; const Math::Real b32 = mat.m_values[3][2]; const Math::Real b33 = mat.m_values[3][3];
//
//#ifdef MATRIX_MODE_TWO_DIMENSIONS
//	matrix.m_values[0][0] = m00 * b00 + m10 * b01 + m20 * b02 + m30 * b03;
//	matrix.m_values[0][1] = m01 * b00 + m11 * b01 + m21 * b02 + m31 * b03;
//	matrix.m_values[0][2] = m02 * b00 + m12 * b01 + m22 * b02 + m32 * b03;
//	matrix.m_values[0][3] = m03 * b00 + m13 * b01 + m23 * b02 + m33 * b03;
//
//	matrix.m_values[1][0] = m00 * b10 + m10 * b11 + m20 * b12 + m30 * b13;
//	matrix.m_values[1][1] = m01 * b10 + m11 * b11 + m21 * b12 + m31 * b13;
//	matrix.m_values[1][2] = m02 * b10 + m12 * b11 + m22 * b12 + m32 * b13;
//	matrix.m_values[1][3] = m03 * b10 + m13 * b11 + m23 * b12 + m33 * b13;
//
//	matrix.m_values[2][0] = m00 * b20 + m10 * b21 + m20 * b22 + m30 * b23;
//	matrix.m_values[2][1] = m01 * b20 + m11 * b21 + m21 * b22 + m31 * b23;
//	matrix.m_values[2][2] = m02 * b20 + m12 * b21 + m22 * b22 + m32 * b23;
//	matrix.m_values[2][3] = m03 * b20 + m13 * b21 + m23 * b22 + m33 * b23;
//
//	matrix.m_values[3][0] = m00 * b30 + m10 * b31 + m20 * b32 + m30 * b33;
//	matrix.m_values[3][1] = m01 * b30 + m11 * b31 + m21 * b32 + m31 * b33;
//	matrix.m_values[3][2] = m02 * b30 + m12 * b31 + m22 * b32 + m32 * b33;
//	matrix.m_values[3][3] = m03 * b30 + m13 * b31 + m23 * b32 + m33 * b33;
//#else
//	matrix.m_values[0] = m00 * b00 + m10 * b01 + m20 * b02 + m30 * b03;
//	matrix.m_values[1] = m01 * b00 + m11 * b01 + m21 * b02 + m31 * b03;
//	matrix.m_values[2] = m02 * b00 + m12 * b01 + m22 * b02 + m32 * b03;
//	matrix.m_values[3] = m03 * b00 + m13 * b01 + m23 * b02 + m33 * b03;
//
//	matrix.m_values[4] = m00 * b10 + m10 * b11 + m20 * b12 + m30 * b13;
//	matrix.m_values[5] = m01 * b10 + m11 * b11 + m21 * b12 + m31 * b13;
//	matrix.m_values[6] = m02 * b10 + m12 * b11 + m22 * b12 + m32 * b13;
//	matrix.m_values[7] = m03 * b10 + m13 * b11 + m23 * b12 + m33 * b13;
//
//	matrix.m_values[8] = m00 * b20 + m10 * b21 + m20 * b22 + m30 * b23;
//	matrix.m_values[9] = m01 * b20 + m11 * b21 + m21 * b22 + m31 * b23;
//	matrix.m_values[10] = m02 * b20 + m12 * b21 + m22 * b22 + m32 * b23;
//	matrix.m_values[11] = m03 * b20 + m13 * b21 + m23 * b22 + m33 * b23;
//
//	matrix.m_values[12] = m00 * b30 + m10 * b31 + m20 * b32 + m30 * b33;
//	matrix.m_values[13] = m01 * b30 + m11 * b31 + m21 * b32 + m31 * b33;
//	matrix.m_values[14] = m02 * b30 + m12 * b31 + m22 * b32 + m32 * b33;
//	matrix.m_values[15] = m03 * b30 + m13 * b31 + m23 * b32 + m33 * b33;
//#endif
	/* ==================== SOLUTION #3 end ==================== */

	/* ==================== SOLUTION #4 begin ==================== */
	//Matrix4D matrix;
	//for (unsigned int i = 0; i < SIZE; ++i)
	//{
	//	for (unsigned int j = 0; j < SIZE; ++j)
	//	{
	//		//matrix.m_values[i][j] = REAL_ZERO;
	//		Real sum = REAL_ZERO;
	//		for (unsigned int k = 0; k < SIZE; ++k)
	//		{
	//			sum += m_values[k][j] * mat.m_values[i][k];
	//		}
	//		matrix.m_values[i][j] = sum;
	//	}
	//}
	//return matrix;
	/* ==================== SOLUTION #4 end ==================== */

	/* ==================== SOLUTION #5 begin ==================== */
//#ifdef MATRIX_MODE_TWO_DIMENSIONS
//	std::array<std::array<Real, SIZE>, SIZE> result;
//	M4x4_SSE(Data(), mat.Data(), &result[0][0]);
//	STOP_PROFILING_MATH("");
//	return Matrix4D(&result[0][0]);
//#else
//	std::array<Real, SIZE * SIZE> result;
//	M4x4_SSE(Data(), mat.Data(), &result[0]);
//	STOP_PROFILING_MATH("");
//	return Matrix4D(&result[0]);
//#endif
	/* ==================== SOLUTION #5 end ==================== */
}

Math::Vector3D Math::Matrix4D::operator*(const Vector3D& vec) const
{
	START_PROFILING_MATH(true, "");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	const Real oneperw = REAL_ONE / (m_values[0][3] * vec.GetX() + m_values[1][3] * vec.GetY() + m_values[2][3] * vec.GetZ() + m_values[3][3]);
	Real x = (m_values[0][0] * vec.GetX() + m_values[1][0] * vec.GetY() + m_values[2][0] * vec.GetZ() + m_values[3][0]) * oneperw;
	Real y = (m_values[0][1] * vec.GetX() + m_values[1][1] * vec.GetY() + m_values[2][1] * vec.GetZ() + m_values[3][1]) * oneperw;
	Real z = (m_values[0][2] * vec.GetX() + m_values[1][2] * vec.GetY() + m_values[2][2] * vec.GetZ() + m_values[3][2]) * oneperw;
#else
	const Real oneperw = REAL_ONE / (m_values[3] * vec.GetX() + m_values[7] * vec.GetY() + m_values[11] * vec.GetZ() + m_values[15]);
	Real x = (m_values[0] * vec.GetX() + m_values[4] * vec.GetY() + m_values[8] * vec.GetZ() + m_values[12]) * oneperw;
	Real y = (m_values[1] * vec.GetX() + m_values[5] * vec.GetY() + m_values[9] * vec.GetZ() + m_values[13]) * oneperw;
	Real z = (m_values[2] * vec.GetX() + m_values[6] * vec.GetY() + m_values[10] * vec.GetZ() + m_values[14]) * oneperw;
#endif
	STOP_PROFILING_MATH("");
	return Vector3D(x, y, z);

	//const Real oneperw = REAL_ONE / (m_values[0][3] * vec.GetX() + m_values[1][3] * vec.GetY() + m_values[2][3] * vec.GetZ() + m_values[3][3]);
	//return Vector3D((m_values[0][0] * vec.GetX() + m_values[1][0] * vec.GetY() + m_values[2][0] * vec.GetZ() + m_values[3][0]) * oneperw,
	//	(m_values[0][1] * vec.GetX() + m_values[1][1] * vec.GetY() + m_values[2][1] * vec.GetZ() + m_values[3][1]) * oneperw,
	//	(m_values[0][2] * vec.GetX() + m_values[1][2] * vec.GetY() + m_values[2][2] * vec.GetZ() + m_values[3][2]) * oneperw);
}

Math::Vector4D Math::Matrix4D::operator*(const Vector4D& vec) const
{
	START_PROFILING_MATH(true, "");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	Real x = m_values[0][0] * vec.GetX() + m_values[0][1] * vec.GetY() + m_values[0][2] * vec.GetZ() + m_values[0][3];
	Real y = m_values[1][0] * vec.GetX() + m_values[1][1] * vec.GetY() + m_values[1][2] * vec.GetZ() + m_values[1][3];
	Real z = m_values[2][0] * vec.GetX() + m_values[2][1] * vec.GetY() + m_values[2][2] * vec.GetZ() + m_values[2][3];
	Real w = m_values[3][0] * vec.GetX() + m_values[3][1] * vec.GetY() + m_values[3][2] * vec.GetZ() + m_values[3][3];
#else
	Real x = m_values[0] * vec.GetX() + m_values[1] * vec.GetY() + m_values[2] * vec.GetZ() + m_values[3];
	Real y = m_values[4] * vec.GetX() + m_values[5] * vec.GetY() + m_values[6] * vec.GetZ() + m_values[7];
	Real z = m_values[8] * vec.GetX() + m_values[9] * vec.GetY() + m_values[10] * vec.GetZ() + m_values[11];
	Real w = m_values[12] * vec.GetX() + m_values[13] * vec.GetY() + m_values[14] * vec.GetZ() + m_values[15];
#endif
	STOP_PROFILING_MATH("");
	return Vector4D(x, y, z, w);
}

/**
 * @see Matrix4D::IsIdentity
 */
bool Math::Matrix4D::operator==(const Matrix4D& matrix) const
{
	START_PROFILING_MATH(false, "");
	/**
	 * TODO: This function is rather slow. Maybe before creating FloatingPoint objects compare
	 * the numbers using simple tools, like epsilon?
	 * Additionaly, maybe consider creating a static function in the Math library for comparing numbers?
	 */
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			if (!AlmostEqual(matrix.GetElement(i, j), m_values[i][j]))
#else
			if (!AlmostEqual(matrix.GetElement(i, j), m_values[i * SIZE + j]))
#endif
			{
				STOP_PROFILING_MATH("");
				return false;
			}
		}
}
	STOP_PROFILING_MATH("");
	return true;
}

bool Math::Matrix4D::operator!=(const Matrix4D& matrix) const
{
	return (!((*this) == matrix));
}

Math::Matrix4D Math::Matrix4D::Transposition() const
{
	START_PROFILING_MATH(true, "");
	Matrix4D matrix;

	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			matrix.m_values[i][j] = m_values[j][i];
#else
			matrix.m_values[i * SIZE + j] = m_values[j * SIZE + i];
#endif
		}
}

	// TODO: According to wikipedia (http://en.wikipedia.org/wiki/Transpose) (A^T)^T = A, so
	// check this condition here. Use SLOW_ASSERT, but use it wisely just once not to cause a runtime stack overflow
	// (due to recursive calls on all control paths).
	// SLOW_ASSERT(matrix.Transposition() == (*this));
	STOP_PROFILING_MATH("");
	return matrix;
}

Math::Real Math::Matrix4D::Det(int p, int q) const
{
	START_PROFILING_MATH(true, "");
	/*
		0: 1 2 3
		1: 0 2 3
		2: 0 1 3
		3: 0 1 2
	*/

	int i[3] =
	{
		((p == 0) ? 1 : 0),
		((p < 2) ? 2 : 1),
		((p == 3) ? 2 : 3)
	};
	int j[3] =
	{
		((q == 0) ? 1 : 0),
		((q < 2) ? 2 : 1),
		((q == 3) ? 2 : 3)
	};
	Real result = 0;
	//#define _det3(M, A00, A10, A20, A01, A11, A21, A02, A12, A22)
	for (int k = 0; k < 3; ++k)
	{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		result += m_values[i[k]][j[0]] * m_values[i[(k + 1) % 3]][j[1]] * m_values[i[(k + 2) % 3]][j[2]];
		result -= m_values[i[k]][j[2]] * m_values[i[(k + 1) % 3]][j[1]] * m_values[i[(k + 2) % 3]][j[0]];
#else
		result += m_values[i[k] * SIZE + j[0]] * m_values[i[(k + 1) % 3] * SIZE + j[1]] * m_values[i[(k + 2) % 3] * SIZE + j[2]];
		result -= m_values[i[k] * SIZE + j[2]] * m_values[i[(k + 1) % 3] * SIZE + j[1]] * m_values[i[(k + 2) % 3] * SIZE + j[0]];
#endif
}

	// TODO: Consider creating some asserts here.
	STOP_PROFILING_MATH("");
	return result;
}

Math::Matrix4D Math::Matrix4D::Inversion() const
{
	START_PROFILING_MATH(true, "");
	Real det = 0;
	for (int j = 0; j < SIZE; ++j)
	{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		det += Signum(3, j) * m_values[3][j] * Det(3, j);
#else
		det += Signum(3, j) * m_values[3 * SIZE + j] * Det(3, j);
#endif
	}

	if (det == 0)
	{
		STOP_PROFILING_MATH("");
		return Matrix4D::IDENTITY_MATRIX;
	}

	Matrix4D result;

	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			result.m_values[j][i] = Signum(i, j) * Det(i, j) / det;
#else
			result.m_values[j * SIZE + i] = Signum(i, j) * Det(i, j) / det;
#endif
		}
}

	// TODO: Use SLOW_ASSERT(IsIdentity()) for the result of the multiplication M*M^(-1)
	SLOW_ASSERT(((*this) * result).IsIdentity());

	STOP_PROFILING_MATH("");
	return result;
}

/**
 * @see Matrix4D::operator ==
 */
bool Math::Matrix4D::IsIdentity() const
{
	START_PROFILING_MATH(false, "");
	/**
	 * TODO: This function is rather slow. Maybe before creating FloatingPoint objects compare
	 * the numbers using simple tools, like epsilon?
	 * Additionaly, maybe consider creating a static function in the Math library for comparing numbers?
	 */

	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			if (i == j)
			{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
				if (!AlmostEqual(REAL_ONE, m_values[i][j]))
#else
				if (!AlmostEqual(REAL_ONE, m_values[i * SIZE + j]))
#endif
				{
					STOP_PROFILING_MATH("");
					return false;
				}
			}
			else /* i != j */
			{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
				if (!AlmostEqual(REAL_ZERO, m_values[i][j]))
#else
				if (!AlmostEqual(REAL_ZERO, m_values[i * SIZE + j]))
#endif
				{
					STOP_PROFILING_MATH("");
					return false;
				}
			}
			}
	}
	STOP_PROFILING_MATH("");
	return true;
}

void Math::Matrix4D::SetScaleMatrix(Real scaleX, Real scaleY, Real scaleZ)
{
	START_PROFILING_MATH(false, "");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = scaleX;		m_values[0][1] = REAL_ZERO;	m_values[0][2] = REAL_ZERO;	m_values[0][3] = REAL_ZERO;
	m_values[1][0] = REAL_ZERO;	m_values[1][1] = scaleY;		m_values[1][2] = REAL_ZERO;	m_values[1][3] = REAL_ZERO;
	m_values[2][0] = REAL_ZERO;	m_values[2][1] = REAL_ZERO;	m_values[2][2] = scaleZ;		m_values[2][3] = REAL_ZERO;
	m_values[3][0] = REAL_ZERO;	m_values[3][1] = REAL_ZERO;	m_values[3][2] = REAL_ZERO;	m_values[3][3] = REAL_ONE;
#else
	m_values[0] = scaleX;		m_values[1] = REAL_ZERO;	m_values[2] = REAL_ZERO;	m_values[3] = REAL_ZERO;
	m_values[4] = REAL_ZERO;	m_values[5] = scaleY;		m_values[6] = REAL_ZERO;	m_values[7] = REAL_ZERO;
	m_values[8] = REAL_ZERO;	m_values[9] = REAL_ZERO;	m_values[10] = scaleZ;		m_values[11] = REAL_ZERO;
	m_values[12] = REAL_ZERO;	m_values[13] = REAL_ZERO;	m_values[14] = REAL_ZERO;	m_values[15] = REAL_ONE;
#endif
	STOP_PROFILING_MATH("");
}

void Math::Matrix4D::SetOrthogonalProjection(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane)
{
	SetElement(0, 0, 2.0f / (right - left));
	SetElement(0, 1, REAL_ZERO);
	SetElement(0, 2, REAL_ZERO);
	SetElement(0, 3, REAL_ZERO);
	SetElement(1, 0, REAL_ZERO);
	SetElement(1, 1, 2.0f / (top - bottom));
	SetElement(1, 2, REAL_ZERO);
	SetElement(1, 3, REAL_ZERO);
	SetElement(2, 0, REAL_ZERO);
	SetElement(2, 1, REAL_ZERO);
	SetElement(2, 2, -2.0f / (farPlane - nearPlane));
	SetElement(2, 3, REAL_ZERO);
	SetElement(3, 0, (-(right + left)) / (right - left));
	SetElement(3, 1, (-(top + bottom)) / (top - bottom));
	SetElement(3, 2, (-(farPlane + nearPlane)) / (farPlane - nearPlane));
	SetElement(3, 3, REAL_ONE);
}

void Math::Matrix4D::SetPerspectiveProjection(const Angle& fov, Real aspect, Real nearPlane, Real farPlane)
{
	START_PROFILING_MATH(false, "");
	Angle halfAngle(fov / static_cast<Real>(2.0));
	const Real f = static_cast<Real>(REAL_ONE / halfAngle.Tan());
	const Real div = static_cast<Real>(REAL_ONE / (nearPlane - farPlane));

	//m_values[0][0] = f / aspect;	m_values[0][1] = 0.0;	m_values[0][2] = 0.0;							m_values[0][3] = 0.0;
	//m_values[1][0] = 0.0;		m_values[1][1] = f;	m_values[1][2] = 0.0;							m_values[1][3] = 0.0;
	//m_values[2][0] = 0.0;		m_values[2][1] = 0.0;	m_values[2][2] = (farPlane + nearPlane) * div;	m_values[2][3] = 2.0 * farPlane * nearPlane * div;
	//m_values[3][0] = 0.0;		m_values[3][1] = 0.0;	m_values[3][2] = -1.0;							m_values[3][3] = 0.0;

	/*
	* IMPLEMENTATION FROM
	* https://www.youtube.com/watch?v=cgaixZEaDCg&list=PLEETnX-uPtBXP_B2yupUKlflXBznWIlL5 and
	* http://www.in.tum.de/fileadmin/user_upload/Lehrstuehle/Lehrstuhl_XV/Teaching/SS07/Praktikum/MatricesTips.pdf
	*/
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = f / aspect;	m_values[0][1] = REAL_ZERO;	m_values[0][2] = REAL_ZERO;											m_values[0][3] = REAL_ZERO;
	m_values[1][0] = REAL_ZERO;	m_values[1][1] = f;			m_values[1][2] = REAL_ZERO;											m_values[1][3] = REAL_ZERO;
	m_values[2][0] = REAL_ZERO;	m_values[2][1] = REAL_ZERO;	m_values[2][2] = (-nearPlane - farPlane) * div;						m_values[2][3] = REAL_ONE;
	m_values[3][0] = REAL_ZERO;	m_values[3][1] = REAL_ZERO;	m_values[3][2] = static_cast<Real>(2.0) * farPlane * nearPlane * div;	m_values[3][3] = REAL_ZERO;
#else
	m_values[0] = f / aspect;	m_values[1] = REAL_ZERO;	m_values[2] = REAL_ZERO;												m_values[3] = REAL_ZERO;
	m_values[4] = REAL_ZERO;	m_values[5] = f;			m_values[6] = REAL_ZERO;												m_values[7] = REAL_ZERO;
	m_values[8] = REAL_ZERO;	m_values[9] = REAL_ZERO;	m_values[10] = (-nearPlane - farPlane) * div;							m_values[11] = REAL_ONE;
	m_values[12] = REAL_ZERO;	m_values[13] = REAL_ZERO;	m_values[14] = static_cast<Real>(2.0) * farPlane * nearPlane * div;	m_values[15] = REAL_ZERO;
#endif
	/* IMPLEMENTATION FROM https://www.youtube.com/watch?v=cgaixZEaDCg&list=PLEETnX-uPtBXP_B2yupUKlflXBznWIlL5 end */

	STOP_PROFILING_MATH("");
}

Math::Vector3D Math::Matrix4D::Transform(const Vector3D& vec)
{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	return Vector3D(m_values[0][0] * vec.GetX() + m_values[1][0] * vec.GetY() + m_values[2][0] * vec.GetZ() + m_values[3][0],
		m_values[0][1] * vec.GetX() + m_values[1][1] * vec.GetY() + m_values[2][1] * vec.GetZ() + m_values[3][1],
		m_values[0][2] * vec.GetX() + m_values[1][2] * vec.GetY() + m_values[2][2] * vec.GetZ() + m_values[3][2]);
#else
	return Vector3D(m_values[0] * vec.GetX() + m_values[4] * vec.GetY() + m_values[8] * vec.GetZ() + m_values[12],
		m_values[1] * vec.GetX() + m_values[5] * vec.GetY() + m_values[9] * vec.GetZ() + m_values[13],
		m_values[2] * vec.GetX() + m_values[6] * vec.GetY() + m_values[10] * vec.GetZ() + m_values[14]);
#endif

	//Vector3D ret(REAL_ZERO, REAL_ZERO, REAL_ZERO);

	//for (unsigned int i = 0; i < SIZE; ++i)
	//{
	//	for (unsigned int j = 0; j < SIZE; ++j)
	//	{
	//		ret.opera
	//	}
	//}
}

void Math::Matrix4D::SetRotationFromVectors(const Vector3D& forward, const Vector3D& up, const Vector3D& right)
{
	Vector3D f = forward.Normalized();
	Vector3D u = up.Normalized();
	Vector3D r = right.Normalized();

	//Vector3D right = up;
	//right.Normalize(); // TODO: Should not be necessary
	//right = right.Cross(forw);

	//Vector3D newUp = forw.Cross(right);
	CHECK_CONDITION_MATH(f.IsNormalized(), Utility::Logging::ERR, "Cannot correctly perform the rotation. The specified forward vector is not normalized.");
	CHECK_CONDITION_MATH(u.IsNormalized(), Utility::Logging::ERR, "Cannot correctly perform the rotation. The specified up vector is not normalized.");
	CHECK_CONDITION_MATH(r.IsNormalized(), Utility::Logging::ERR, "Cannot correctly perform the rotation. The specified right vector is not normalized.");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = r.GetX();		m_values[0][1] = u.GetX();		m_values[0][2] = f.GetX();		m_values[0][3] = REAL_ZERO;
	m_values[1][0] = r.GetY();		m_values[1][1] = u.GetY();		m_values[1][2] = f.GetY();		m_values[1][3] = REAL_ZERO;
	m_values[2][0] = r.GetZ();		m_values[2][1] = u.GetZ();		m_values[2][2] = f.GetZ();		m_values[2][3] = REAL_ZERO;
	m_values[3][0] = REAL_ZERO;	m_values[3][1] = REAL_ZERO;	m_values[3][2] = REAL_ZERO;	m_values[3][3] = REAL_ONE;
#else
	m_values[0] = right.GetX();	m_values[1] = up.GetX();	m_values[2] = forward.GetX();	m_values[3] = REAL_ZERO;
	m_values[4] = right.GetY();	m_values[5] = up.GetY();	m_values[6] = forward.GetY();	m_values[7] = REAL_ZERO;
	m_values[8] = right.GetZ();	m_values[9] = up.GetZ();	m_values[10] = forward.GetZ();	m_values[11] = REAL_ZERO;
	m_values[12] = REAL_ZERO;		m_values[13] = REAL_ZERO;	m_values[14] = REAL_ZERO;		m_values[15] = REAL_ONE;
#endif
}
