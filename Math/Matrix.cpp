#include "StdAfx.h"
#include "Matrix.h"
#include "FloatingPoint.h"

#include "Utility\Utility.h"

#include <sstream>

using namespace Math;
using namespace Utility;

#ifdef CALCULATE_MATH_STATS
/* static */ Statistics::ClassStats& Matrix4D::s_classStats(STATS_STORAGE.GetClassStats("Matrix4DStatic"));
#endif

/* static */ const Matrix4D Matrix4D::IDENTITY_MATRIX;

/* static */ int Matrix4D::Signum(int i, int j)
{
	return ((i + j) % 2) ? -1 : 1;
}

Matrix4D::Matrix4D()
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = REAL_ONE;		m[0][1] = REAL_ZERO;	m[0][2] = REAL_ZERO;	m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;	m[1][1] = REAL_ONE;		m[1][2] = REAL_ZERO;	m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;	m[2][1] = REAL_ZERO;	m[2][2] = REAL_ONE;		m[2][3] = REAL_ZERO;
	m[3][0] = REAL_ZERO;	m[3][1] = REAL_ZERO;	m[3][2] = REAL_ZERO;	m[3][3] = REAL_ONE;
#else
	m[0] = REAL_ONE;	m[1] = REAL_ZERO;	m[2] = REAL_ZERO;	m[3] = REAL_ZERO;
	m[4] = REAL_ZERO;	m[5] = REAL_ONE;	m[6] = REAL_ZERO;	m[7] = REAL_ZERO;
	m[8] = REAL_ZERO;	m[9] = REAL_ZERO;	m[10] = REAL_ONE;	m[11] = REAL_ZERO;
	m[12] = REAL_ZERO;	m[13] = REAL_ZERO;	m[14] = REAL_ZERO;	m[15] = REAL_ONE;
#endif

	STOP_PROFILING;
}

Matrix4D::Matrix4D(Math::Real m00, Math::Real m01, Math::Real m02, Math::Real m03,
	Math::Real m10, Math::Real m11, Math::Real m12, Math::Real m13,
	Math::Real m20, Math::Real m21, Math::Real m22, Math::Real m23,
	Math::Real m30, Math::Real m31, Math::Real m32, Math::Real m33)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = m00;	m[0][1] = m01;	m[0][2] = m02;	m[0][3] = m03;
	m[1][0] = m10;	m[1][1] = m11;	m[1][2] = m12;	m[1][3] = m13;
	m[2][0] = m20;	m[2][1] = m21;	m[2][2] = m22;	m[2][3] = m23;
	m[3][0] = m30;	m[3][1] = m31;	m[3][2] = m32;	m[3][3] = m33;
#else
	m[0] = m00;		m[1] = m01;		m[2] = m02;		m[3] = m03;
	m[4] = m10;		m[5] = m11;		m[6] = m12;		m[7] = m13;
	m[8] = m20;		m[9] = m21;		m[10] = m22;	m[11] = m23;
	m[12] = m30;	m[13] = m31;	m[14] = m32;	m[15] = m33;
#endif
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Vector2D& screenPosition, const Vector2D& scale)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	Matrix4D translateMatrix(screenPosition.GetX(), screenPosition.GetY(), REAL_ZERO);
	Matrix4D scaleMatrix;
	scaleMatrix.SetScaleMatrix(scale.GetX(), scale.GetY(), REAL_ONE);
	Matrix4D result = translateMatrix * scaleMatrix;

#ifdef MATRIX_MODE_TWO_DIMENSIONS
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			m[i][j] = result[i][j];
		}
	}
#else
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; ++i)
	{
		m[i] = result[i];
	}
#endif

	STOP_PROFILING;
}

Matrix4D::Matrix4D(Real scale)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	SetScaleMatrix(scale, scale, scale);
	STOP_PROFILING;
}

Matrix4D::Matrix4D(Real posX, Real posY, Real posZ)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = REAL_ONE;		m[0][1] = REAL_ZERO;	m[0][2] = REAL_ZERO;	m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;	m[1][1] = REAL_ONE;		m[1][2] = REAL_ZERO;	m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;	m[2][1] = REAL_ZERO;	m[2][2] = REAL_ONE;		m[2][3] = REAL_ZERO;
	m[3][0] = posX;			m[3][1] = posY;			m[3][2] = posZ;			m[3][3] = REAL_ONE;
#else
	m[0] = REAL_ONE;	m[1] = REAL_ZERO;	m[2] = REAL_ZERO;	m[3] = REAL_ZERO;
	m[4] = REAL_ZERO;	m[5] = REAL_ONE;	m[6] = REAL_ZERO;	m[7] = REAL_ZERO;
	m[8] = REAL_ZERO;	m[9] = REAL_ZERO;	m[10] = REAL_ONE;	m[11] = REAL_ZERO;
	m[12] = posX;		m[13] = posY;		m[14] = posZ;		m[15] = REAL_ONE;
#endif

	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Vector3D& pos)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = REAL_ONE;		m[0][1] = REAL_ZERO;	m[0][2] = REAL_ZERO;	m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;	m[1][1] = REAL_ONE;		m[1][2] = REAL_ZERO;	m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;	m[2][1] = REAL_ZERO;	m[2][2] = REAL_ONE;		m[2][3] = REAL_ZERO;
	m[3][0] = pos.GetX();	m[3][1] = pos.GetY();	m[3][2] = pos.GetZ();	m[3][3] = REAL_ONE;
#else
	m[0] = REAL_ONE;	m[1] = REAL_ZERO;	m[2] = REAL_ZERO;	m[3] = REAL_ZERO;
	m[4] = REAL_ZERO;	m[5] = REAL_ONE;	m[6] = REAL_ZERO;	m[7] = REAL_ZERO;
	m[8] = REAL_ZERO;	m[9] = REAL_ZERO;	m[10] = REAL_ONE;	m[11] = REAL_ZERO;
	m[12] = pos.GetX();	m[13] = pos.GetY();	m[14] = pos.GetZ();	m[15] = REAL_ONE;
#endif
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Angle& angleX, const Angle& angleY)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	const Real xSin = angleX.Sin();
	const Real xCos = angleX.Cos();
	const Real ySin = angleY.Sin();
	const Real yCos = angleY.Cos();
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = yCos;			m[0][1] = REAL_ZERO;	m[0][2] = ySin;			m[0][3] = REAL_ZERO;
	m[1][0] = -xSin * ySin;	m[1][1] = xCos;			m[1][2] = xSin * yCos;	m[1][3] = REAL_ZERO;
	m[2][0] = -xCos * ySin;	m[2][1] = -xSin;		m[2][2] = xCos * yCos;	m[2][3] = REAL_ZERO;
	m[3][0] = REAL_ZERO;	m[3][1] = REAL_ZERO;	m[3][2] = REAL_ZERO;	m[3][3] = REAL_ONE;
#else
	m[0] = yCos;			m[1] = REAL_ZERO;	m[2] = ySin;			m[3] = REAL_ZERO;
	m[4] = -xSin * ySin;	m[5] = xCos;		m[6] = xSin * yCos;		m[7] = REAL_ZERO;
	m[8] = -xCos * ySin;	m[9] = -xSin;		m[10] = xCos * yCos;	m[11] = REAL_ZERO;
	m[12] = REAL_ZERO;		m[13] = REAL_ZERO;	m[14] = REAL_ZERO;		m[15] = REAL_ONE;
#endif
	CHECK_CONDITION((*this) == Matrix4D(angleX, angleY, Angle(REAL_ZERO)), Utility::Error, "Two RotationEuler functions give different results.");
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Angle& angleX, const Angle& angleY, const Angle& angleZ)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING; // TODO: As there are two methods with the same name "RotationEuler" their stats will be stored in the same place (which is not good). Fix it!
	/* ==================== SOLUTION #1 begin ==================== */
	//Matrix4D rotX, rotY, rotZ; // rotation around X, Y and Z axis respectively

	//Real xSin = angleX.Sin();
	//Real xCos = angleX.Cos();
	//Real ySin = angleY.Sin();
	//Real yCos = angleY.Cos();
	//Real zSin = angleZ.Sin();
	//Real zCos = angleZ.Cos();

	//rotX.m[0][0] = REAL_ONE;	rotX.m[0][1] = REAL_ZERO;	rotX.m[0][2] = REAL_ZERO;	rotX.m[0][3] = REAL_ZERO;
	//rotX.m[1][0] = REAL_ZERO;	rotX.m[1][1] = xCos;		rotX.m[1][2] = xSin;		rotX.m[1][3] = REAL_ZERO;
	//rotX.m[2][0] = REAL_ZERO;	rotX.m[2][1] = -xSin;		rotX.m[2][2] = xCos;		rotX.m[2][3] = REAL_ZERO;
	//rotX.m[3][0] = REAL_ZERO;	rotX.m[3][1] = REAL_ZERO;	rotX.m[3][2] = REAL_ZERO;	rotX.m[3][3] = REAL_ONE;

	//rotY.m[0][0] = yCos;		rotY.m[0][1] = REAL_ZERO;	rotY.m[0][2] = ySin;		rotY.m[0][3] = REAL_ZERO;
	//rotY.m[1][0] = REAL_ZERO;	rotY.m[1][1] = REAL_ONE;	rotY.m[1][2] = REAL_ZERO;	rotY.m[1][3] = REAL_ZERO;
	//rotY.m[2][0] = -ySin;		rotY.m[2][1] = REAL_ZERO;	rotY.m[2][2] = yCos;		rotY.m[2][3] = REAL_ZERO;
	//rotY.m[3][0] = REAL_ZERO;	rotY.m[3][1] = REAL_ZERO;	rotY.m[3][2] = REAL_ZERO;	rotY.m[3][3] = REAL_ONE;

	//rotZ.m[0][0] = zCos;		rotZ.m[0][1] = zSin;		rotZ.m[0][2] = REAL_ZERO;	rotZ.m[0][3] = REAL_ZERO;
	//rotZ.m[1][0] = -zSin;		rotZ.m[1][1] = zCos;		rotZ.m[1][2] = REAL_ZERO;	rotZ.m[1][3] = REAL_ZERO;
	//rotZ.m[2][0] = REAL_ZERO;	rotZ.m[2][1] = REAL_ZERO;	rotZ.m[2][2] = REAL_ONE;	rotZ.m[2][3] = REAL_ZERO;
	//rotZ.m[3][0] = REAL_ZERO;	rotZ.m[3][1] = REAL_ZERO;	rotZ.m[3][2] = REAL_ZERO;	rotZ.m[3][3] = REAL_ONE;

	//return rotZ * rotY * rotX;
	/* ==================== SOLUTION #1 end ==================== */

	/* ==================== SOLUTION #2 begin ==================== */
	const Real xSin = angleX.Sin();
	const Real xCos = angleX.Cos();
	const Real ySin = angleY.Sin();
	const Real yCos = angleY.Cos();
	const Real zSin = angleZ.Sin();
	const Real zCos = angleZ.Cos();
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = yCos * zCos;							m[0][1] = yCos * zSin;							m[0][2] = ySin;			m[0][3] = REAL_ZERO;
	m[1][0] = -xSin * ySin * zCos - xCos * zSin;	m[1][1] = -xSin * ySin * zSin + xCos * zCos;	m[1][2] = xSin * yCos;	m[1][3] = REAL_ZERO;
	m[2][0] = -xCos * ySin * zCos + xSin * zSin;	m[2][1] = -xCos * ySin * zSin - xSin * zCos;	m[2][2] = xCos * yCos;	m[2][3] = REAL_ZERO;
	m[3][0] = REAL_ZERO;							m[3][1] = REAL_ZERO;							m[3][2] = REAL_ZERO;	m[3][3] = REAL_ONE;
#else
	m[0] = yCos * zCos;							m[1] = yCos * zSin;							m[2] = ySin;			m[3] = REAL_ZERO;
	m[4] = -xSin * ySin * zCos - xCos * zSin;	m[5] = -xSin * ySin * zSin + xCos * zCos;	m[6] = xSin * yCos;		m[7] = REAL_ZERO;
	m[8] = -xCos * ySin * zCos + xSin * zSin;	m[9] = -xCos * ySin * zSin - xSin * zCos;	m[10] = xCos * yCos;	m[11] = REAL_ZERO;
	m[12] = REAL_ZERO;							m[13] = REAL_ZERO;							m[14] = REAL_ZERO;		m[15] = REAL_ONE;
#endif
	//Matrix4D matrixToCompare = rotZ * rotY * rotX;
	//if (rot != matrixToCompare)
	//{
	//	ERROR_LOG("Incorrect euler rotation calculation. Rot =\n%s\nInstead it should be equal to\n%s", rot.ToString().c_str(), matrixToCompare.ToString().c_str());
	//}
	STOP_PROFILING;
	/* ==================== SOLUTION #2 end ==================== */
}

Matrix4D::Matrix4D(const Vector3D& forward, const Vector3D& up, const Vector3D& right)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	SetRotationFromVectors(forward, up, right);
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Vector3D& forward, const Vector3D& up)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	Vector3D n = forward.Normalized();
	Vector3D u = up.Normalized().Cross(n);
	Vector3D v = n.Cross(u);
	SetRotationFromVectors(n, v, u);
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Angle& fov /* Field of View */, Real aspect /* Aspect */, Real nearPlane /* Near plane */, Real farPlane /* Far plane */)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	SetPerspectiveProjection(fov, aspect, nearPlane, farPlane);
	STOP_PROFILING;
}

Matrix4D::Matrix4D(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;

	const Real width = right - left;
	const Real height = top - bottom;
	const Real depth = farPlane - nearPlane;
	const Real temp = static_cast<Real>(2.0f);
	
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = temp / width;				m[0][1] = REAL_ZERO;				m[0][2] = REAL_ZERO;						m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;				m[1][1] = temp / height;			m[1][2] = REAL_ZERO;						m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;				m[2][1] = REAL_ZERO;				m[2][2] = -temp / depth;					m[2][3] = REAL_ZERO;
	m[3][0] = -(right + left) / width;	m[3][1] = -(top + bottom) / height;	m[3][2] = -(farPlane + nearPlane) / depth;	m[3][3] = REAL_ONE;
#else
	m[0] = temp / width;				m[1] = REAL_ZERO;					m[2] = REAL_ZERO;							m[3] = REAL_ZERO;
	m[4] = REAL_ZERO;					m[5] = temp / height;				m[6] = REAL_ZERO;							m[7] = REAL_ZERO;
	m[8] = REAL_ZERO;					m[9] = REAL_ZERO;					m[10] = -temp / depth;						m[11] = REAL_ZERO;
	m[12] = -(right + left) / width;	m[13] = -(top + bottom) / height;	m[14] = -(farPlane + nearPlane) / depth;	m[15] = REAL_ONE;
#endif
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Matrix4D& mat)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	// TODO: Check which of the three solution is faster
	
	/* ==================== SOLUTION #1 begin ==================== */
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = mat[0][0];	m[0][1] = mat[0][1];	m[0][2] = mat[0][2];	m[0][3] = mat[0][3];
	m[1][0] = mat[1][0];	m[1][1] = mat[1][1];	m[1][2] = mat[1][2];	m[1][3] = mat[1][3];
	m[2][0] = mat[2][0];	m[2][1] = mat[2][1];	m[2][2] = mat[2][2];	m[2][3] = mat[2][3];
	m[3][0] = mat[3][0];	m[3][1] = mat[3][1];	m[3][2] = mat[3][2];	m[3][3] = mat[3][3];
#else
	m[0] = mat[0];		m[1] = mat[1];		m[2] = mat[2];		m[3] = mat[3];
	m[4] = mat[4];		m[5] = mat[5];		m[6] = mat[6];		m[7] = mat[7];
	m[8] = mat[8];		m[9] = mat[9];		m[10] = mat[10];	m[11] = mat[11];
	m[12] = mat[12];	m[13] = mat[13];	m[14] = mat[14];	m[15] = mat[15];
#endif
	/* ==================== SOLUTION #1 end ==================== */
	
	/* ==================== SOLUTION #2 begin ==================== */
	//for (int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (int j = 0; j < MATRIX_SIZE; ++j)
	//	{
// #ifdef MATRIX_MODE_TWO_DIMENSIONS
	//		m[i][j] = mat[i][j];
//#else
	//		m[i * MATRIX_SIZE + j] = mat[i * MATRIX_SIZE + j];
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
	
	CHECK_CONDITION((*this) == mat, Utility::Error, "The copy constructor should cause the two matrices to be equal, but they are not.");
	STOP_PROFILING;
}

Matrix4D::~Matrix4D()
{
}

std::string Matrix4D::ToString() const
{
	const std::string INDENTATION_STRING = " ";
	std::stringstream s("");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	s << m[0][0] << INDENTATION_STRING;
	s << m[0][1] << INDENTATION_STRING;
	s << m[0][2] << INDENTATION_STRING;
	s << m[0][3] << INDENTATION_STRING;
#else
	s << m[0] << INDENTATION_STRING;
	s << m[1] << INDENTATION_STRING;
	s << m[2] << INDENTATION_STRING;
	s << m[3] << INDENTATION_STRING;
#endif
	s << std::endl;

#ifdef MATRIX_MODE_TWO_DIMENSIONS
	s << m[1][0] << INDENTATION_STRING;
	s << m[1][1] << INDENTATION_STRING;
	s << m[1][2] << INDENTATION_STRING;
	s << m[1][3] << INDENTATION_STRING;
#else
	s << m[4] << INDENTATION_STRING;
	s << m[5] << INDENTATION_STRING;
	s << m[6] << INDENTATION_STRING;
	s << m[7] << INDENTATION_STRING;
#endif
	s << std::endl;

#ifdef MATRIX_MODE_TWO_DIMENSIONS
	s << m[2][0] << INDENTATION_STRING;
	s << m[2][1] << INDENTATION_STRING;
	s << m[2][2] << INDENTATION_STRING;
	s << m[2][3] << INDENTATION_STRING;
#else
	s << m[8] << INDENTATION_STRING;
	s << m[9] << INDENTATION_STRING;
	s << m[10] << INDENTATION_STRING;
	s << m[11] << INDENTATION_STRING;
#endif
	s << std::endl;

#ifdef MATRIX_MODE_TWO_DIMENSIONS
	s << m[3][0] << INDENTATION_STRING;
	s << m[3][1] << INDENTATION_STRING;
	s << m[3][2] << INDENTATION_STRING;
	s << m[3][3] << INDENTATION_STRING;
#else
	s << m[12] << INDENTATION_STRING;
	s << m[13] << INDENTATION_STRING;
	s << m[14] << INDENTATION_STRING;
	s << m[15] << INDENTATION_STRING;
#endif
	s << std::endl;
	return s.str();
}
	
Matrix4D Matrix4D::operator*(const Matrix4D& mat) const
{
	START_PROFILING;
	/* ==================== SOLUTION #1 (doesn't work!!!) begin ==================== */
	//Matrix4D matrix;

	//matrix.m[0][0] = m[0][0] * mat.m[0][0] + m[1][0] * mat.m[0][1] + m[2][0] * mat.m[0][2];
	//matrix.m[0][1] = m[0][1] * mat.m[0][0] + m[1][1] * mat.m[0][1] + m[2][1] * mat.m[0][2];
	//matrix.m[0][2] = m[0][2] * mat.m[0][0] + m[1][2] * mat.m[0][1] + m[2][2] * mat.m[0][2];
	//matrix.m[0][3] = REAL_ZERO;

	//matrix.m[1][0] = m[0][0] * mat.m[1][0] + m[1][0] * mat.m[1][1] + m[2][0] * mat.m[1][2];
	//matrix.m[1][1] = m[0][1] * mat.m[1][0] + m[1][1] * mat.m[1][1] + m[2][1] * mat.m[1][2];
	//matrix.m[1][2] = m[0][2] * mat.m[1][0] + m[1][2] * mat.m[1][1] + m[2][2] * mat.m[1][2];
	//matrix.m[1][3] = REAL_ZERO;

	//matrix.m[2][0] = m[0][0] * mat.m[2][0] + m[1][0] * mat.m[2][1] + m[2][0] * mat.m[2][2];
	//matrix.m[2][1] = m[0][1] * mat.m[2][0] + m[1][1] * mat.m[2][1] + m[2][1] * mat.m[2][2];
	//matrix.m[2][2] = m[0][2] * mat.m[2][0] + m[1][2] * mat.m[2][1] + m[2][2] * mat.m[2][2];
	//matrix.m[2][3] = REAL_ZERO;

	//matrix.m[3][0] = m[0][0] * mat.m[3][0] + m[1][0] * mat.m[3][1] + m[2][0] * mat.m[3][2] + m[3][0];
	//matrix.m[3][1] = m[0][1] * mat.m[3][0] + m[1][1] * mat.m[3][1] + m[2][1] * mat.m[3][2] + m[3][1];
	//matrix.m[3][2] = m[0][2] * mat.m[3][0] + m[1][2] * mat.m[3][1] + m[2][2] * mat.m[3][2] + m[3][2];
	//matrix.m[3][3] = REAL_ONE;
	//
	//return matrix;
	/* ==================== SOLUTION #1 (doesn't work!!!) end ==================== */

	/* ==================== SOLUTION #2 begin ==================== */
	Matrix4D matrix;
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	matrix.m[0][0] = m[0][0] * mat.m[0][0] + m[1][0] * mat.m[0][1] + m[2][0] * mat.m[0][2] + m[3][0] * mat.m[0][3];
	matrix.m[0][1] = m[0][1] * mat.m[0][0] + m[1][1] * mat.m[0][1] + m[2][1] * mat.m[0][2] + m[3][1] * mat.m[0][3];
	matrix.m[0][2] = m[0][2] * mat.m[0][0] + m[1][2] * mat.m[0][1] + m[2][2] * mat.m[0][2] + m[3][2] * mat.m[0][3];
	matrix.m[0][3] = m[0][3] * mat.m[0][0] + m[1][3] * mat.m[0][1] + m[2][3] * mat.m[0][2] + m[3][3] * mat.m[0][3];

	matrix.m[1][0] = m[0][0] * mat.m[1][0] + m[1][0] * mat.m[1][1] + m[2][0] * mat.m[1][2] + m[3][0] * mat.m[1][3];
	matrix.m[1][1] = m[0][1] * mat.m[1][0] + m[1][1] * mat.m[1][1] + m[2][1] * mat.m[1][2] + m[3][1] * mat.m[1][3];
	matrix.m[1][2] = m[0][2] * mat.m[1][0] + m[1][2] * mat.m[1][1] + m[2][2] * mat.m[1][2] + m[3][2] * mat.m[1][3];
	matrix.m[1][3] = m[0][3] * mat.m[1][0] + m[1][3] * mat.m[1][1] + m[2][3] * mat.m[1][2] + m[3][3] * mat.m[1][3];

	matrix.m[2][0] = m[0][0] * mat.m[2][0] + m[1][0] * mat.m[2][1] + m[2][0] * mat.m[2][2] + m[3][0] * mat.m[2][3];
	matrix.m[2][1] = m[0][1] * mat.m[2][0] + m[1][1] * mat.m[2][1] + m[2][1] * mat.m[2][2] + m[3][1] * mat.m[2][3];
	matrix.m[2][2] = m[0][2] * mat.m[2][0] + m[1][2] * mat.m[2][1] + m[2][2] * mat.m[2][2] + m[3][2] * mat.m[2][3];
	matrix.m[2][3] = m[0][3] * mat.m[2][0] + m[1][3] * mat.m[2][1] + m[2][3] * mat.m[2][2] + m[3][3] * mat.m[2][3];

	matrix.m[3][0] = m[0][0] * mat.m[3][0] + m[1][0] * mat.m[3][1] + m[2][0] * mat.m[3][2] + m[3][0] * mat.m[3][3];
	matrix.m[3][1] = m[0][1] * mat.m[3][0] + m[1][1] * mat.m[3][1] + m[2][1] * mat.m[3][2] + m[3][1] * mat.m[3][3];
	matrix.m[3][2] = m[0][2] * mat.m[3][0] + m[1][2] * mat.m[3][1] + m[2][2] * mat.m[3][2] + m[3][2] * mat.m[3][3];
	matrix.m[3][3] = m[0][3] * mat.m[3][0] + m[1][3] * mat.m[3][1] + m[2][3] * mat.m[3][2] + m[3][3] * mat.m[3][3];
#else
	matrix.m[0] = m[0] * mat.m[0] + m[4] * mat.m[1] + m[8] * mat.m[2] + m[12] * mat.m[3];
	matrix.m[1] = m[1] * mat.m[0] + m[5] * mat.m[1] + m[9] * mat.m[2] + m[13] * mat.m[3];
	matrix.m[2] = m[2] * mat.m[0] + m[6] * mat.m[1] + m[10] * mat.m[2] + m[14] * mat.m[3];
	matrix.m[3] = m[3] * mat.m[0] + m[7] * mat.m[1] + m[11] * mat.m[2] + m[15] * mat.m[3];

	matrix.m[4] = m[0] * mat.m[4] + m[4] * mat.m[5] + m[8] * mat.m[6] + m[12] * mat.m[7];
	matrix.m[5] = m[1] * mat.m[4] + m[5] * mat.m[5] + m[9] * mat.m[6] + m[13] * mat.m[7];
	matrix.m[6] = m[2] * mat.m[4] + m[6] * mat.m[5] + m[10] * mat.m[6] + m[14] * mat.m[7];
	matrix.m[7] = m[3] * mat.m[4] + m[7] * mat.m[5] + m[11] * mat.m[6] + m[15] * mat.m[7];

	matrix.m[8] = m[0] * mat.m[8] + m[4] * mat.m[9] + m[8] * mat.m[10] + m[12] * mat.m[11];
	matrix.m[9] = m[1] * mat.m[8] + m[5] * mat.m[9] + m[9] * mat.m[10] + m[13] * mat.m[11];
	matrix.m[10] = m[2] * mat.m[8] + m[6] * mat.m[9] + m[10] * mat.m[10] + m[14] * mat.m[11];
	matrix.m[11] = m[3] * mat.m[8] + m[7] * mat.m[9] + m[11] * mat.m[10] + m[15] * mat.m[11];

	matrix.m[12] = m[0] * mat.m[12] + m[4] * mat.m[13] + m[8] * mat.m[14] + m[12] * mat.m[15];
	matrix.m[13] = m[1] * mat.m[12] + m[5] * mat.m[13] + m[9] * mat.m[14] + m[13] * mat.m[15];
	matrix.m[14] = m[2] * mat.m[12] + m[6] * mat.m[13] + m[10] * mat.m[14] + m[14] * mat.m[15];
	matrix.m[15] = m[3] * mat.m[12] + m[7] * mat.m[13] + m[11] * mat.m[14] + m[15] * mat.m[15];
#endif
	
	STOP_PROFILING;
	return matrix;
	/* ==================== SOLUTION #2 end ==================== */

	/* ==================== SOLUTION #3 begin ==================== */
	//for (unsigned int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (unsigned int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		//matrix.m[i][j] = REAL_ZERO;
	//		Real sum = REAL_ZERO;
	//		for (unsigned int k = 0; k < MATRIX_SIZE; ++k)
	//		{
	//			sum += m[k][j] * mat.m[i][k];
	//		}
	//		matrix.m[i][j] = sum;
	//	}
	//}
	//return matrix;
	/* ==================== SOLUTION #3 end ==================== */
}

Vector3D Matrix4D::operator*(const Vector3D& vec) const
{
	START_PROFILING;
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	const Real oneperw = REAL_ONE / (m[0][3] * vec.GetX() + m[1][3] * vec.GetY() + m[2][3] * vec.GetZ() + m[3][3]);
	Real x = (m[0][0] * vec.GetX() + m[1][0] * vec.GetY() + m[2][0] * vec.GetZ() + m[3][0]) * oneperw;
	Real y = (m[0][1] * vec.GetX() + m[1][1] * vec.GetY() + m[2][1] * vec.GetZ() + m[3][1]) * oneperw;
	Real z = (m[0][2] * vec.GetX() + m[1][2] * vec.GetY() + m[2][2] * vec.GetZ() + m[3][2]) * oneperw;
#else
	const Real oneperw = REAL_ONE / (m[3] * vec.GetX() + m[7] * vec.GetY() + m[11] * vec.GetZ() + m[15]);
	Real x = (m[0] * vec.GetX() + m[4] * vec.GetY() + m[8] * vec.GetZ() + m[12]) * oneperw;
	Real y = (m[1] * vec.GetX() + m[5] * vec.GetY() + m[9] * vec.GetZ() + m[13]) * oneperw;
	Real z = (m[2] * vec.GetX() + m[6] * vec.GetY() + m[10] * vec.GetZ() + m[14]) * oneperw;
#endif
	STOP_PROFILING;
	return Vector3D(x, y, z);

	//const Real oneperw = REAL_ONE / (m[0][3] * vec.GetX() + m[1][3] * vec.GetY() + m[2][3] * vec.GetZ() + m[3][3]);
	//return Vector3D((m[0][0] * vec.GetX() + m[1][0] * vec.GetY() + m[2][0] * vec.GetZ() + m[3][0]) * oneperw,
	//	(m[0][1] * vec.GetX() + m[1][1] * vec.GetY() + m[2][1] * vec.GetZ() + m[3][1]) * oneperw,
	//	(m[0][2] * vec.GetX() + m[1][2] * vec.GetY() + m[2][2] * vec.GetZ() + m[3][2]) * oneperw);
}

Vector4D Matrix4D::operator*(const Vector4D& vec) const
{
	START_PROFILING;
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	Real x = m[0][0] * vec.GetX() + m[0][1] * vec.GetY() + m[0][2] * vec.GetZ() + m[0][3];
	Real y = m[1][0] * vec.GetX() + m[1][1] * vec.GetY() + m[1][2] * vec.GetZ() + m[1][3];
	Real z = m[2][0] * vec.GetX() + m[2][1] * vec.GetY() + m[2][2] * vec.GetZ() + m[2][3];
	Real w = m[3][0] * vec.GetX() + m[3][1] * vec.GetY() + m[3][2] * vec.GetZ() + m[3][3];
#else
	Real x = m[0] * vec.GetX() + m[1] * vec.GetY() + m[2] * vec.GetZ() + m[3];
	Real y = m[4] * vec.GetX() + m[5] * vec.GetY() + m[6] * vec.GetZ() + m[7];
	Real z = m[8] * vec.GetX() + m[9] * vec.GetY() + m[10] * vec.GetZ() + m[11];
	Real w = m[12] * vec.GetX() + m[13] * vec.GetY() + m[14] * vec.GetZ() + m[15];
#endif
	STOP_PROFILING;
	return Vector4D(x, y, z, w);
}

/**
 * @see Matrix4D::IsIdentity
 */
bool Matrix4D::operator==(const Matrix4D& matrix) const
{
	START_PROFILING;
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
			if (! AlmostEqual(matrix.GetElement(i, j), m[i][j]))
#else
			if (! AlmostEqual(matrix.GetElement(i, j), m[i * MATRIX_SIZE + j]))
#endif
			{
				STOP_PROFILING;
				return false;
			}
		}
	}
	STOP_PROFILING;
	return true;
}

bool Matrix4D::operator!=(const Matrix4D& matrix) const
{
	return (!((*this) == matrix));
}

Matrix4D& Matrix4D::operator=(const Matrix4D& mat)
{
	START_PROFILING;
	// TODO: Check which of the three solution is faster
	
	/* ==================== SOLUTION #1 begin ==================== */
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = mat[0][0];	m[0][1] = mat[0][1];	m[0][2] = mat[0][2];	m[0][3] = mat[0][3];
	m[1][0] = mat[1][0];	m[1][1] = mat[1][1];	m[1][2] = mat[1][2];	m[1][3] = mat[1][3];
	m[2][0] = mat[2][0];	m[2][1] = mat[2][1];	m[2][2] = mat[2][2];	m[2][3] = mat[2][3];
	m[3][0] = mat[3][0];	m[3][1] = mat[3][1];	m[3][2] = mat[3][2];	m[3][3] = mat[3][3];
#else
	m[0] = mat[0];		m[1] = mat[1];		m[2] = mat[2];		m[3] = mat[3];
	m[4] = mat[4];		m[5] = mat[5];		m[6] = mat[6];		m[7] = mat[7];
	m[8] = mat[8];		m[9] = mat[9];		m[10] = mat[10];	m[11] = mat[11];
	m[12] = mat[12];	m[13] = mat[13];	m[14] = mat[14];	m[15] = mat[15];
#endif
	/* ==================== SOLUTION #1 end ==================== */
	
	/* ==================== SOLUTION #2 begin ==================== */
	//for (int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		m[i][j] = mat[i][j];
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
	
	STOP_PROFILING;
	return *this;
}

Matrix4D Matrix4D::Transposition() const
{
	START_PROFILING;
	Matrix4D matrix;

	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			matrix.m[i][j] = m[j][i];
#else
			matrix.m[i * MATRIX_SIZE + j] = m[j * MATRIX_SIZE + i];
#endif
		}
	}

	// TODO: According to wikipedia (http://en.wikipedia.org/wiki/Transpose) (A^T)^T = A, so
	// check this condition here. Use SLOW_ASSERT, but use it wisely just once not to cause a runtime stack overflow
	// (due to recursive calls on all control paths).
	// SLOW_ASSERT(matrix.Transposition() == (*this));
	STOP_PROFILING;
	return matrix;
}

Real Matrix4D::Det(int p, int q) const
{
	START_PROFILING;
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
		result += m[i[k]][j[0]] * m[i[(k + 1) % 3]][j[1]] * m[i[(k + 2) % 3]][j[2]];
		result -= m[i[k]][j[2]] * m[i[(k + 1) % 3]][j[1]] * m[i[(k + 2) % 3]][j[0]];
#else
		result += m[i[k] * MATRIX_SIZE + j[0]] * m[i[(k + 1) % 3] * MATRIX_SIZE + j[1]] * m[i[(k + 2) % 3] * MATRIX_SIZE + j[2]];
		result -= m[i[k] * MATRIX_SIZE + j[2]] * m[i[(k + 1) % 3] * MATRIX_SIZE + j[1]] * m[i[(k + 2) % 3] * MATRIX_SIZE + j[0]];
#endif
	}
	
	// TODO: Consider creating some asserts here.
	STOP_PROFILING;
	return result;
}

Matrix4D Matrix4D::Inversion() const
{
	START_PROFILING;
	Real det = 0;
	for (int j = 0; j < MATRIX_SIZE; ++j)
	{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		det += Signum(3, j) * m[3][j] * Det(3, j);
#else
		det += Signum(3, j) * m[3 * MATRIX_SIZE + j] * Det(3, j);
#endif
	}

	if (det == 0)
	{
		STOP_PROFILING;
		return Matrix4D::IDENTITY_MATRIX;
	}

	Matrix4D result;

	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			result.m[j][i] = Signum(i,j) * Det(i,j) / det;
#else
			result.m[j * MATRIX_SIZE + i] = Signum(i,j) * Det(i,j) / det;
#endif
		}
	}
	
	// TODO: Use SLOW_ASSERT(IsIdentity()) for the result of the multiplication M*M^(-1)
	SLOW_ASSERT(((*this) * result).IsIdentity());

	STOP_PROFILING;
	return result;
}

/**
 * @see Matrix4D::operator ==
 */
bool Matrix4D::IsIdentity() const
{
	START_PROFILING;
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
				if (! AlmostEqual(REAL_ONE, m[i][j]))
#else
				if (! AlmostEqual(REAL_ONE, m[i * MATRIX_SIZE + j]))
#endif
				{
					STOP_PROFILING;
					return false;
				}
			}
			else /* i != j */
			{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
				if (! AlmostEqual(REAL_ZERO, m[i][j]))
#else
				if (! AlmostEqual(REAL_ZERO, m[i * MATRIX_SIZE + j]))
#endif
				{
					STOP_PROFILING;
					return false;
				}
			}
		}
	}
	STOP_PROFILING;	
	return true;
}

void Matrix4D::SetScaleMatrix(Real scaleX, Real scaleY, Real scaleZ)
{
	START_PROFILING;
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = scaleX;		m[0][1] = REAL_ZERO;	m[0][2] = REAL_ZERO;	m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;	m[1][1] = scaleY;		m[1][2] = REAL_ZERO;	m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;	m[2][1] = REAL_ZERO;	m[2][2] = scaleZ;		m[2][3] = REAL_ZERO;
	m[3][0] = REAL_ZERO;	m[3][1] = REAL_ZERO;	m[3][2] = REAL_ZERO;	m[3][3] = REAL_ONE;
#else
	m[0] = scaleX;		m[1] = REAL_ZERO;	m[2] = REAL_ZERO;	m[3] = REAL_ZERO;
	m[4] = REAL_ZERO;	m[5] = scaleY;		m[6] = REAL_ZERO;	m[7] = REAL_ZERO;
	m[8] = REAL_ZERO;	m[9] = REAL_ZERO;	m[10] = scaleZ;		m[11] = REAL_ZERO;
	m[12] = REAL_ZERO;	m[13] = REAL_ZERO;	m[14] = REAL_ZERO;	m[15] = REAL_ONE;
#endif
	STOP_PROFILING;
}

void Matrix4D::SetPerspectiveProjection(const Angle& fov, Real aspect, Real nearPlane, Real farPlane)
{
	START_PROFILING;
	Angle halfAngle(fov / static_cast<Real>(2.0));
	const Real f = static_cast<Real>(REAL_ONE / halfAngle.Tan());
	const Real div = static_cast<Real>(REAL_ONE / (nearPlane - farPlane));

	//m[0][0] = f / aspect;	m[0][1] = 0.0;	m[0][2] = 0.0;							m[0][3] = 0.0;
	//m[1][0] = 0.0;		m[1][1] = f;	m[1][2] = 0.0;							m[1][3] = 0.0;
	//m[2][0] = 0.0;		m[2][1] = 0.0;	m[2][2] = (farPlane + nearPlane) * div;	m[2][3] = 2.0 * farPlane * nearPlane * div;
	//m[3][0] = 0.0;		m[3][1] = 0.0;	m[3][2] = -1.0;							m[3][3] = 0.0;

	/* IMPLEMENTATION FROM https://www.youtube.com/watch?v=cgaixZEaDCg&list=PLEETnX-uPtBXP_B2yupUKlflXBznWIlL5 begin */
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = f / aspect;	m[0][1] = REAL_ZERO;	m[0][2] = REAL_ZERO;											m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;	m[1][1] = f;			m[1][2] = REAL_ZERO;											m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;	m[2][1] = REAL_ZERO;	m[2][2] = (-nearPlane - farPlane) * div;						m[2][3] = REAL_ONE;
	m[3][0] = REAL_ZERO;	m[3][1] = REAL_ZERO;	m[3][2] = static_cast<Real>(2.0) * farPlane * nearPlane * div;	m[3][3] = REAL_ZERO;
#else
	m[0] = f / aspect;	m[1] = REAL_ZERO;	m[2] = REAL_ZERO;												m[3] = REAL_ZERO;
	m[4] = REAL_ZERO;	m[5] = f;			m[6] = REAL_ZERO;												m[7] = REAL_ZERO;
	m[8] = REAL_ZERO;	m[9] = REAL_ZERO;	m[10] = (-nearPlane - farPlane) * div;							m[11] = REAL_ONE;
	m[12] = REAL_ZERO;	m[13] = REAL_ZERO;	m[14] = static_cast<Real>(2.0) * farPlane * nearPlane * div;	m[15] = REAL_ZERO;
#endif
	/* IMPLEMENTATION FROM https://www.youtube.com/watch?v=cgaixZEaDCg&list=PLEETnX-uPtBXP_B2yupUKlflXBznWIlL5 end */

	STOP_PROFILING;
}

Vector3D Matrix4D::Transform(const Vector3D& vec)
{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	return Vector3D(m[0][0] * vec.GetX() + m[1][0] * vec.GetY() + m[2][0] * vec.GetZ() + m[3][0],
					m[0][1] * vec.GetX() + m[1][1] * vec.GetY() + m[2][1] * vec.GetZ() + m[3][1],
					m[0][2] * vec.GetX() + m[1][2] * vec.GetY() + m[2][2] * vec.GetZ() + m[3][2]);
#else
	return Vector3D(m[0] * vec.GetX() + m[4] * vec.GetY() + m[8] * vec.GetZ() + m[12],
					m[1] * vec.GetX() + m[5] * vec.GetY() + m[9] * vec.GetZ() + m[13],
					m[2] * vec.GetX() + m[6] * vec.GetY() + m[10] * vec.GetZ() + m[14]);
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

void Matrix4D::SetRotationFromVectors(const Vector3D& forward, const Vector3D& up, const Vector3D& right)
{
	Vector3D f = forward.Normalized();
	Vector3D u = up.Normalized();
	Vector3D r = right.Normalized();
	
	//Vector3D right = up;
	//right.Normalize(); // TODO: Should not be necessary
	//right = right.Cross(forw);

	//Vector3D newUp = forw.Cross(right);
	CHECK_CONDITION(f.IsNormalized(), Utility::Error, "Cannot correctly perform the rotation. The specified forward vector is not normalized.");
	CHECK_CONDITION(u.IsNormalized(), Utility::Error, "Cannot correctly perform the rotation. The specified up vector is not normalized.");
	CHECK_CONDITION(r.IsNormalized(), Utility::Error, "Cannot correctly perform the rotation. The specified right vector is not normalized.");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
	m[0][0] = r.GetX();		m[0][1] = u.GetX();		m[0][2] = f.GetX();		m[0][3] = REAL_ZERO;
	m[1][0] = r.GetY();		m[1][1] = u.GetY();		m[1][2] = f.GetY();		m[1][3] = REAL_ZERO;
	m[2][0] = r.GetZ();		m[2][1] = u.GetZ();		m[2][2] = f.GetZ();		m[2][3] = REAL_ZERO;
	m[3][0] = REAL_ZERO;	m[3][1] = REAL_ZERO;	m[3][2] = REAL_ZERO;	m[3][3] = REAL_ONE;
#else
	m[0] = right.GetX();	m[1] = up.GetX();	m[2] = forward.GetX();	m[3] = REAL_ZERO;
	m[4] = right.GetY();	m[5] = up.GetY();	m[6] = forward.GetY();	m[7] = REAL_ZERO;
	m[8] = right.GetZ();	m[9] = up.GetZ();	m[10] = forward.GetZ();	m[11] = REAL_ZERO;
	m[12] = REAL_ZERO;		m[13] = REAL_ZERO;	m[14] = REAL_ZERO;		m[15] = REAL_ONE;
#endif
}