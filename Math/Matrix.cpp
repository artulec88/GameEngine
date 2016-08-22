#include "StdAfx.h"
#include "Matrix.h"
#include "FloatingPoint.h"

#include "Utility\Utility.h"

#include <sstream>
#include <iostream>
#include <utility>

#ifdef CALCULATE_MATH_STATS
/* static */ Math::Statistics::ClassStats& Math::Matrix4D::s_classStats(STATS_STORAGE.GetClassStats("Matrix4DStatic"));
#endif

/* static */ const Math::Matrix4D Math::Matrix4D::IDENTITY_MATRIX;

/* static */ int Math::Matrix4D::Signum(int i, int j)
{
	return ((i + j) % 2) ? -1 : 1;
}

Math::Matrix4D::Matrix4D()
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("1");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = REAL_ONE;	m_values[0][1] = REAL_ZERO;	m_values[0][2] = REAL_ZERO;	m_values[0][3] = REAL_ZERO;
	m_values[1][0] = REAL_ZERO;	m_values[1][1] = REAL_ONE;	m_values[1][2] = REAL_ZERO;	m_values[1][3] = REAL_ZERO;
	m_values[2][0] = REAL_ZERO;	m_values[2][1] = REAL_ZERO;	m_values[2][2] = REAL_ONE;	m_values[2][3] = REAL_ZERO;
	m_values[3][0] = REAL_ZERO;	m_values[3][1] = REAL_ZERO;	m_values[3][2] = REAL_ZERO;	m_values[3][3] = REAL_ONE;
#else
	m_values[0] = REAL_ONE;		m_values[1] = REAL_ZERO;	m_values[2] = REAL_ZERO;	m_values[3] = REAL_ZERO;
	m_values[4] = REAL_ZERO;	m_values[5] = REAL_ONE;		m_values[6] = REAL_ZERO;	m_values[7] = REAL_ZERO;
	m_values[8] = REAL_ZERO;	m_values[9] = REAL_ZERO;	m_values[10] = REAL_ONE;	m_values[11] = REAL_ZERO;
	m_values[12] = REAL_ZERO;	m_values[13] = REAL_ZERO;	m_values[14] = REAL_ZERO;	m_values[15] = REAL_ONE;
#endif

	STOP_PROFILING("1");
}

Math::Matrix4D::Matrix4D(Math::Real m00, Math::Real m01, Math::Real m02, Math::Real m03,
	Math::Real m10, Math::Real m11, Math::Real m12, Math::Real m13,
	Math::Real m20, Math::Real m21, Math::Real m22, Math::Real m23,
	Math::Real m30, Math::Real m31, Math::Real m32, Math::Real m33)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("2");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = m00;	m_values[0][1] = m01;	m_values[0][2] = m02;	m_values[0][3] = m03;
	m_values[1][0] = m10;	m_values[1][1] = m11;	m_values[1][2] = m12;	m_values[1][3] = m13;
	m_values[2][0] = m20;	m_values[2][1] = m21;	m_values[2][2] = m22;	m_values[2][3] = m23;
	m_values[3][0] = m30;	m_values[3][1] = m31;	m_values[3][2] = m32;	m_values[3][3] = m33;
#else
	m_values[0] = m00;		m_values[1] = m01;		m_values[2] = m02;		m_values[3] = m03;
	m_values[4] = m10;		m_values[5] = m11;		m_values[6] = m12;		m_values[7] = m13;
	m_values[8] = m20;		m_values[9] = m21;		m_values[10] = m22;		m_values[11] = m23;
	m_values[12] = m30;		m_values[13] = m31;		m_values[14] = m32;		m_values[15] = m33;
#endif
	STOP_PROFILING("2");
}

Math::Matrix4D::Matrix4D(const Vector2D& screenPosition, const Vector2D& scale)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("3");
	Matrix4D translateMatrix(screenPosition.GetX(), screenPosition.GetY(), REAL_ZERO);
	Matrix4D scaleMatrix;
	scaleMatrix.SetScaleMatrix(scale.GetX(), scale.GetY(), REAL_ONE);
	Matrix4D result = translateMatrix * scaleMatrix; // FIXME: Check matrix multiplication

#ifdef MATRIX_MODE_TWO_DIMENSIONS
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			m_values[i][j] = result[i][j];
		}
	}
#else
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; ++i)
	{
		m_values[i] = result[i];
	}
#endif

	STOP_PROFILING("3");
}

Math::Matrix4D::Matrix4D(Real scale)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("4");
	SetScaleMatrix(scale, scale, scale);
	STOP_PROFILING("4");
}

Math::Matrix4D::Matrix4D(Real posX, Real posY, Real posZ)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("5");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = REAL_ONE;		m_values[0][1] = REAL_ZERO;	m_values[0][2] = REAL_ZERO;	m_values[0][3] = REAL_ZERO;
	m_values[1][0] = REAL_ZERO;	m_values[1][1] = REAL_ONE;		m_values[1][2] = REAL_ZERO;	m_values[1][3] = REAL_ZERO;
	m_values[2][0] = REAL_ZERO;	m_values[2][1] = REAL_ZERO;	m_values[2][2] = REAL_ONE;		m_values[2][3] = REAL_ZERO;
	m_values[3][0] = posX;			m_values[3][1] = posY;			m_values[3][2] = posZ;			m_values[3][3] = REAL_ONE;
#else
	m_values[0] = REAL_ONE;	m_values[1] = REAL_ZERO;	m_values[2] = REAL_ZERO;	m_values[3] = REAL_ZERO;
	m_values[4] = REAL_ZERO;	m_values[5] = REAL_ONE;	m_values[6] = REAL_ZERO;	m_values[7] = REAL_ZERO;
	m_values[8] = REAL_ZERO;	m_values[9] = REAL_ZERO;	m_values[10] = REAL_ONE;	m_values[11] = REAL_ZERO;
	m_values[12] = posX;		m_values[13] = posY;		m_values[14] = posZ;		m_values[15] = REAL_ONE;
#endif

	STOP_PROFILING("5");
}

Math::Matrix4D::Matrix4D(const Vector3D& pos)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("6");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = REAL_ONE;		m_values[0][1] = REAL_ZERO;	m_values[0][2] = REAL_ZERO;	m_values[0][3] = REAL_ZERO;
	m_values[1][0] = REAL_ZERO;	m_values[1][1] = REAL_ONE;		m_values[1][2] = REAL_ZERO;	m_values[1][3] = REAL_ZERO;
	m_values[2][0] = REAL_ZERO;	m_values[2][1] = REAL_ZERO;	m_values[2][2] = REAL_ONE;		m_values[2][3] = REAL_ZERO;
	m_values[3][0] = pos.GetX();	m_values[3][1] = pos.GetY();	m_values[3][2] = pos.GetZ();	m_values[3][3] = REAL_ONE;
#else
	m_values[0] = REAL_ONE;	m_values[1] = REAL_ZERO;	m_values[2] = REAL_ZERO;	m_values[3] = REAL_ZERO;
	m_values[4] = REAL_ZERO;	m_values[5] = REAL_ONE;	m_values[6] = REAL_ZERO;	m_values[7] = REAL_ZERO;
	m_values[8] = REAL_ZERO;	m_values[9] = REAL_ZERO;	m_values[10] = REAL_ONE;	m_values[11] = REAL_ZERO;
	m_values[12] = pos.GetX();	m_values[13] = pos.GetY();	m_values[14] = pos.GetZ();	m_values[15] = REAL_ONE;
#endif
	STOP_PROFILING("6");
}

Math::Matrix4D::Matrix4D(const Angle& angleX, const Angle& angleY)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("7");
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
	CHECK_CONDITION_MATH((*this) == Matrix4D(angleX, angleY, Angle(REAL_ZERO)), Utility::ERR, "Two RotationEuler functions give different results.");
	STOP_PROFILING("7");
}

Math::Matrix4D::Matrix4D(const Angle& angleX, const Angle& angleY, const Angle& angleZ)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("8"); // TODO: As there are two methods with the same name "RotationEuler" their stats will be stored in the same place (which is not good). Fix it!
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
	//	ERROR_LOG_MATH("Incorrect euler rotation calculation. Rot=\n", rot.ToString(), "\nInstead it should be equal to:\n", matrixToCompare.ToString());
	//}
	STOP_PROFILING("8");
	/* ==================== SOLUTION #2 end ==================== */
}

Math::Matrix4D::Matrix4D(const Vector3D& forward, const Vector3D& up, const Vector3D& right)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("9");
	SetRotationFromVectors(forward, up, right);
	STOP_PROFILING("9");
}

Math::Matrix4D::Matrix4D(const Vector3D& forward, const Vector3D& up)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("10");
	Vector3D n = forward.Normalized();
	Vector3D u = up.Normalized().Cross(n);
	Vector3D v = n.Cross(u);
	SetRotationFromVectors(n, v, u);
	STOP_PROFILING("10");
}

Math::Matrix4D::Matrix4D(const Angle& fov /* Field of View */, Real aspect /* Aspect */, Real nearPlane /* Near plane */, Real farPlane /* Far plane */)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("11");
	SetPerspectiveProjection(fov, aspect, nearPlane, farPlane);
	STOP_PROFILING("11");
}

Math::Matrix4D::Matrix4D(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("12");

	const Real width = right - left;
	const Real height = top - bottom;
	const Real depth = farPlane - nearPlane;
	const Real temp = static_cast<Real>(2.0f);
	
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = temp / width;				m_values[0][1] = REAL_ZERO;					m_values[0][2] = REAL_ZERO;							m_values[0][3] = REAL_ZERO;
	m_values[1][0] = REAL_ZERO;					m_values[1][1] = temp / height;				m_values[1][2] = REAL_ZERO;							m_values[1][3] = REAL_ZERO;
	m_values[2][0] = REAL_ZERO;					m_values[2][1] = REAL_ZERO;					m_values[2][2] = -temp / depth;						m_values[2][3] = REAL_ZERO;
	m_values[3][0] = -(right + left) / width;	m_values[3][1] = -(top + bottom) / height;	m_values[3][2] = -(farPlane + nearPlane) / depth;	m_values[3][3] = REAL_ONE;
#else
	m_values[0] = temp / width;				m_values[1] = REAL_ZERO;					m_values[2] = REAL_ZERO;							m_values[3] = REAL_ZERO;
	m_values[4] = REAL_ZERO;				m_values[5] = temp / height;				m_values[6] = REAL_ZERO;							m_values[7] = REAL_ZERO;
	m_values[8] = REAL_ZERO;				m_values[9] = REAL_ZERO;					m_values[10] = -temp / depth;						m_values[11] = REAL_ZERO;
	m_values[12] = -(right + left) / width;	m_values[13] = -(top + bottom) / height;	m_values[14] = -(farPlane + nearPlane) / depth;		m_values[15] = REAL_ONE;
#endif
	STOP_PROFILING("12");
}

Math::Matrix4D::Matrix4D(const Matrix4D& mat)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("13");
	// TODO: Check which of the three solution is faster
	
	/* ==================== SOLUTION #1 begin ==================== */
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = mat[0][0];	m_values[0][1] = mat[0][1];	m_values[0][2] = mat[0][2];	m_values[0][3] = mat[0][3];
	m_values[1][0] = mat[1][0];	m_values[1][1] = mat[1][1];	m_values[1][2] = mat[1][2];	m_values[1][3] = mat[1][3];
	m_values[2][0] = mat[2][0];	m_values[2][1] = mat[2][1];	m_values[2][2] = mat[2][2];	m_values[2][3] = mat[2][3];
	m_values[3][0] = mat[3][0];	m_values[3][1] = mat[3][1];	m_values[3][2] = mat[3][2];	m_values[3][3] = mat[3][3];
#else
	m_values[0] = mat[0];		m_values[1] = mat[1];		m_values[2] = mat[2];		m_values[3] = mat[3];
	m_values[4] = mat[4];		m_values[5] = mat[5];		m_values[6] = mat[6];		m_values[7] = mat[7];
	m_values[8] = mat[8];		m_values[9] = mat[9];		m_values[10] = mat[10];		m_values[11] = mat[11];
	m_values[12] = mat[12];		m_values[13] = mat[13];		m_values[14] = mat[14];		m_values[15] = mat[15];
#endif
	/* ==================== SOLUTION #1 end ==================== */
	
	/* ==================== SOLUTION #2 begin ==================== */
	//for (int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (int j = 0; j < MATRIX_SIZE; ++j)
	//	{
// #ifdef MATRIX_MODE_TWO_DIMENSIONS
	//		m_values[i][j] = mat[i][j];
//#else
	//		m_values[i * MATRIX_SIZE + j] = mat[i * MATRIX_SIZE + j];
//#endif
	//	}
	//}
	/* ==================== SOLUTION #2 end ==================== */
	
	/* ==================== SOLUTION #3 begin ==================== */
	//for (int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		this->SetElement(i, j, mat.GetElement(i, j));
	//	}
	//}
	/* ==================== SOLUTION #3 end ==================== */
	
	CHECK_CONDITION_MATH((*this) == mat, Utility::ERR, "The copy constructor should cause the two matrices to be equal, but they are not.");
	STOP_PROFILING("13");
}

Math::Matrix4D::Matrix4D(Matrix4D&& mat) :
	m_values(std::move(mat.m_values)),
#ifdef CALCULATE_MATH_STATS
	m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING("14");
	STOP_PROFILING("14");
}

Math::Matrix4D::~Matrix4D()
{
}

std::string Math::Matrix4D::ToString() const
{
	const std::string INDENTATION_STRING = "\t";
	std::stringstream s("");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	s << m_values[0][0] << INDENTATION_STRING;
	s << m_values[0][1] << INDENTATION_STRING;
	s << m_values[0][2] << INDENTATION_STRING;
	s << m_values[0][3] << INDENTATION_STRING;
#else
	s << m_values[0] << INDENTATION_STRING;
	s << m_values[1] << INDENTATION_STRING;
	s << m_values[2] << INDENTATION_STRING;
	s << m_values[3] << INDENTATION_STRING;
#endif
	s << std::endl;

#ifdef MATRIX_MODE_TWO_DIMENSIONS
	s << m_values[1][0] << INDENTATION_STRING;
	s << m_values[1][1] << INDENTATION_STRING;
	s << m_values[1][2] << INDENTATION_STRING;
	s << m_values[1][3] << INDENTATION_STRING;
#else
	s << m_values[4] << INDENTATION_STRING;
	s << m_values[5] << INDENTATION_STRING;
	s << m_values[6] << INDENTATION_STRING;
	s << m_values[7] << INDENTATION_STRING;
#endif
	s << std::endl;

#ifdef MATRIX_MODE_TWO_DIMENSIONS
	s << m_values[2][0] << INDENTATION_STRING;
	s << m_values[2][1] << INDENTATION_STRING;
	s << m_values[2][2] << INDENTATION_STRING;
	s << m_values[2][3] << INDENTATION_STRING;
#else
	s << m_values[8] << INDENTATION_STRING;
	s << m_values[9] << INDENTATION_STRING;
	s << m_values[10] << INDENTATION_STRING;
	s << m_values[11] << INDENTATION_STRING;
#endif
	s << std::endl;

#ifdef MATRIX_MODE_TWO_DIMENSIONS
	s << m_values[3][0] << INDENTATION_STRING;
	s << m_values[3][1] << INDENTATION_STRING;
	s << m_values[3][2] << INDENTATION_STRING;
	s << m_values[3][3] << INDENTATION_STRING;
#else
	s << m_values[12] << INDENTATION_STRING;
	s << m_values[13] << INDENTATION_STRING;
	s << m_values[14] << INDENTATION_STRING;
	s << m_values[15] << INDENTATION_STRING;
#endif
	s << std::endl;
	return s.str();
}
	
Math::Matrix4D Math::Matrix4D::operator*(const Matrix4D& mat) const
{
	START_PROFILING("");
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
	//
	//return matrix;
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
	
	STOP_PROFILING("");
	return matrix;
	/* ==================== SOLUTION #2 end ==================== */

	/* ==================== SOLUTION #3 begin ==================== */
	//for (unsigned int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (unsigned int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		//matrix.m_values[i][j] = REAL_ZERO;
	//		Real sum = REAL_ZERO;
	//		for (unsigned int k = 0; k < MATRIX_SIZE; ++k)
	//		{
	//			sum += m_values[k][j] * mat.m_values[i][k];
	//		}
	//		matrix.m_values[i][j] = sum;
	//	}
	//}
	//return matrix;
	/* ==================== SOLUTION #3 end ==================== */
}

Math::Vector3D Math::Matrix4D::operator*(const Vector3D& vec) const
{
	START_PROFILING("");
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
	STOP_PROFILING("");
	return Vector3D(x, y, z);

	//const Real oneperw = REAL_ONE / (m_values[0][3] * vec.GetX() + m_values[1][3] * vec.GetY() + m_values[2][3] * vec.GetZ() + m_values[3][3]);
	//return Vector3D((m_values[0][0] * vec.GetX() + m_values[1][0] * vec.GetY() + m_values[2][0] * vec.GetZ() + m_values[3][0]) * oneperw,
	//	(m_values[0][1] * vec.GetX() + m_values[1][1] * vec.GetY() + m_values[2][1] * vec.GetZ() + m_values[3][1]) * oneperw,
	//	(m_values[0][2] * vec.GetX() + m_values[1][2] * vec.GetY() + m_values[2][2] * vec.GetZ() + m_values[3][2]) * oneperw);
}

Math::Vector4D Math::Matrix4D::operator*(const Vector4D& vec) const
{
	START_PROFILING("");
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
	STOP_PROFILING("");
	return Vector4D(x, y, z, w);
}

/**
 * @see Matrix4D::IsIdentity
 */
bool Math::Matrix4D::operator==(const Matrix4D& matrix) const
{
	START_PROFILING("");
	/**
	 * TODO: This function is rather slow. Maybe before creating FloatingPoint objects compare
	 * the numbers using simple tools, like epsilon?
	 * Additionaly, maybe consider creating a static function in the Math library for comparing numbers?
	 */
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			if (! AlmostEqual(matrix.GetElement(i, j), m_values[i][j]))
#else
			if (! AlmostEqual(matrix.GetElement(i, j), m_values[i * MATRIX_SIZE + j]))
#endif
			{
				STOP_PROFILING("");
				return false;
			}
		}
	}
	STOP_PROFILING("");
	return true;
}

bool Math::Matrix4D::operator!=(const Matrix4D& matrix) const
{
	return (!((*this) == matrix));
}

Math::Matrix4D& Math::Matrix4D::operator=(const Matrix4D& mat)
{
	START_PROFILING("");
	// TODO: Check which of the three solution is faster
	
	/* ==================== SOLUTION #1 begin ==================== */
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m_values[0][0] = mat[0][0];	m_values[0][1] = mat[0][1];	m_values[0][2] = mat[0][2];	m_values[0][3] = mat[0][3];
	m_values[1][0] = mat[1][0];	m_values[1][1] = mat[1][1];	m_values[1][2] = mat[1][2];	m_values[1][3] = mat[1][3];
	m_values[2][0] = mat[2][0];	m_values[2][1] = mat[2][1];	m_values[2][2] = mat[2][2];	m_values[2][3] = mat[2][3];
	m_values[3][0] = mat[3][0];	m_values[3][1] = mat[3][1];	m_values[3][2] = mat[3][2];	m_values[3][3] = mat[3][3];
#else
	m_values[0] = mat[0];		m_values[1] = mat[1];		m_values[2] = mat[2];		m_values[3] = mat[3];
	m_values[4] = mat[4];		m_values[5] = mat[5];		m_values[6] = mat[6];		m_values[7] = mat[7];
	m_values[8] = mat[8];		m_values[9] = mat[9];		m_values[10] = mat[10];	m_values[11] = mat[11];
	m_values[12] = mat[12];	m_values[13] = mat[13];	m_values[14] = mat[14];	m_values[15] = mat[15];
#endif
	/* ==================== SOLUTION #1 end ==================== */
	
	/* ==================== SOLUTION #2 begin ==================== */
	//for (int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		m_values[i][j] = mat[i][j];
	//	}
	//}
	/* ==================== SOLUTION #2 end ==================== */
	
	/* ==================== SOLUTION #3 begin ==================== */
	//for (int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		this->SetElement(i, j, mat.GetElement(i, j));
	//	}
	//}
	/* ==================== SOLUTION #3 end ==================== */
	
	SLOW_ASSERT((*this) == mat);
	
	STOP_PROFILING("");
	return *this;
}

Math::Matrix4D& Math::Matrix4D::operator=(Matrix4D&& mat)
{
	START_PROFILING("");
	m_values = std::move(mat.m_values);
	STOP_PROFILING("");
	return *this;
}

Math::Matrix4D Math::Matrix4D::Transposition() const
{
	START_PROFILING("");
	Matrix4D matrix;

	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			matrix.m_values[i][j] = m_values[j][i];
#else
			matrix.m_values[i * MATRIX_SIZE + j] = m_values[j * MATRIX_SIZE + i];
#endif
		}
	}

	// TODO: According to wikipedia (http://en.wikipedia.org/wiki/Transpose) (A^T)^T = A, so
	// check this condition here. Use SLOW_ASSERT, but use it wisely just once not to cause a runtime stack overflow
	// (due to recursive calls on all control paths).
	// SLOW_ASSERT(matrix.Transposition() == (*this));
	STOP_PROFILING("");
	return matrix;
}

Math::Real Math::Matrix4D::Det(int p, int q) const
{
	START_PROFILING("");
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
		result += m_values[i[k] * MATRIX_SIZE + j[0]] * m_values[i[(k + 1) % 3] * MATRIX_SIZE + j[1]] * m_values[i[(k + 2) % 3] * MATRIX_SIZE + j[2]];
		result -= m_values[i[k] * MATRIX_SIZE + j[2]] * m_values[i[(k + 1) % 3] * MATRIX_SIZE + j[1]] * m_values[i[(k + 2) % 3] * MATRIX_SIZE + j[0]];
#endif
	}
	
	// TODO: Consider creating some asserts here.
	STOP_PROFILING("");
	return result;
}

Math::Matrix4D Math::Matrix4D::Inversion() const
{
	START_PROFILING("");
	Real det = 0;
	for (int j = 0; j < MATRIX_SIZE; ++j)
	{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		det += Signum(3, j) * m_values[3][j] * Det(3, j);
#else
		det += Signum(3, j) * m_values[3 * MATRIX_SIZE + j] * Det(3, j);
#endif
	}

	if (det == 0)
	{
		STOP_PROFILING("");
		return Matrix4D::IDENTITY_MATRIX;
	}

	Matrix4D result;

	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			result.m_values[j][i] = Signum(i,j) * Det(i,j) / det;
#else
			result.m_values[j * MATRIX_SIZE + i] = Signum(i,j) * Det(i,j) / det;
#endif
		}
	}
	
	// TODO: Use SLOW_ASSERT(IsIdentity()) for the result of the multiplication M*M^(-1)
	SLOW_ASSERT(((*this) * result).IsIdentity());

	STOP_PROFILING("");
	return result;
}

/**
 * @see Matrix4D::operator ==
 */
bool Math::Matrix4D::IsIdentity() const
{
	START_PROFILING("");
	/**
	 * TODO: This function is rather slow. Maybe before creating FloatingPoint objects compare
	 * the numbers using simple tools, like epsilon?
	 * Additionaly, maybe consider creating a static function in the Math library for comparing numbers?
	 */
	
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			if (i == j)
			{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
				if (! AlmostEqual(REAL_ONE, m_values[i][j]))
#else
				if (! AlmostEqual(REAL_ONE, m_values[i * MATRIX_SIZE + j]))
#endif
				{
					STOP_PROFILING("");
					return false;
				}
			}
			else /* i != j */
			{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
				if (! AlmostEqual(REAL_ZERO, m_values[i][j]))
#else
				if (! AlmostEqual(REAL_ZERO, m_values[i * MATRIX_SIZE + j]))
#endif
				{
					STOP_PROFILING("");
					return false;
				}
			}
		}
	}
	STOP_PROFILING("");
	return true;
}

void Math::Matrix4D::SetScaleMatrix(Real scaleX, Real scaleY, Real scaleZ)
{
	START_PROFILING("");
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
	STOP_PROFILING("");
}

void Math::Matrix4D::SetPerspectiveProjection(const Angle& fov, Real aspect, Real nearPlane, Real farPlane)
{
	START_PROFILING("");
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

	STOP_PROFILING("");
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

	//for (unsigned int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (unsigned int j = 0; j < MATRIX_SIZE; ++j)
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
	CHECK_CONDITION_MATH(f.IsNormalized(), Utility::ERR, "Cannot correctly perform the rotation. The specified forward vector is not normalized.");
	CHECK_CONDITION_MATH(u.IsNormalized(), Utility::ERR, "Cannot correctly perform the rotation. The specified up vector is not normalized.");
	CHECK_CONDITION_MATH(r.IsNormalized(), Utility::ERR, "Cannot correctly perform the rotation. The specified right vector is not normalized.");
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