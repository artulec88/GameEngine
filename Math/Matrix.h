#pragma once

#include "Angle.h"
#include "Math.h"
#include "Vector.h"

//#include "Utility\ISerializable.h"
#include "Utility\ILogger.h"

#include <string>

namespace Math
{

#define MATRIX_SIZE 4

class MATH_API Matrix4D// : public Utility::ISerializable
{
/* ==================== Static variables and functions begin ==================== */
public:
	static Matrix4D Identity();
	static Matrix4D PerspectiveProjection(const Angle& fov /* Field of View */, Real aspect /* Aspect */, Real nearPlane /* Near plane */, Real farPlane /* Far plane */);
	static Matrix4D OrtographicProjection(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane);
	static Matrix4D Translation(Real x, Real y, Real z);
	static Matrix4D Translation(const Vector3D& vec);
	static Matrix4D Scale(Real x, Real y, Real z);
	static Matrix4D Scale(const Vector3D& vec);
	//static Matrix4D Rotation(Real x, Real y, Real z, const Angle& angle);
	//static Matrix4D Rotation(const Vector3D& vec, const Angle& angle);

	static Matrix4D RotationEuler(const Angle& angleX, const Angle& angleY);
	static Matrix4D RotationEuler(const Angle& angleX, const Angle& angleY, const Angle& angleZ);
	static Matrix4D RotationFromVectors(const Vector3D& forward, const Vector3D& up, const Vector3D& right);
	static Matrix4D RotationFromDirection(const Vector3D& forward, const Vector3D& up);

	static int Signum(int i, int j);
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Matrix4D();
	Matrix4D(const Matrix4D& mat);
	~Matrix4D();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void SetElement(int i, int j, Real value);
	Real GetElement (int i, int j) const;
	
	Matrix4D operator*(const Matrix4D& m) const;
	Vector3D operator*(const Vector3D& vec) const;
	bool operator==(const Matrix4D& m) const;
	bool operator!=(const Matrix4D& m) const;
	Matrix4D& operator=(const Matrix4D& mat);

	const Math::Real* operator[](int index) const;
	Math::Real* operator[](int index);

	Vector3D Transform(const Vector3D& vec); // Write tests
	
	Matrix4D Transposition() const;
	Matrix4D Inversion() const; // TODO: Write tests
	Real Det(int p, int q) const;
	
	/**
	 * Checks whether the matrix is an identity matrix or not.
	 * 
	 * @brief Used only during tests
	 */
	bool IsIdentity() const;

	std::string ToString() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Real m[MATRIX_SIZE][MATRIX_SIZE];
/* ==================== Non-static member variables end ==================== */
}; /* end class Matrix */

inline Real Matrix4D::GetElement (int i, int j) const
{
#ifdef _DEBUG
	if ((i < 0) || (i >= MATRIX_SIZE))
	{
		LOG(Utility::Error, LOGPLACE, "Incorrect row index given (%d)", i);
		exit(EXIT_FAILURE);
	}
	if ((j < 0) || (j >= MATRIX_SIZE))
	{
		LOG(Utility::Error, LOGPLACE, "Incorrect column index given (%d)", j);
		exit(EXIT_FAILURE);
	}
#endif
	return m[i][j];
}

inline void Matrix4D::SetElement(int i, int j, Real value)
{
#ifdef _DEBUG
	if ((i < 0) || (i >= 3))
	{
		LOG(Utility::Error, LOGPLACE, "Incorrect row index given (%d)", i);
		exit(EXIT_FAILURE);
	}
	if ((j < 0) || (j >= 3))
	{
		LOG(Utility::Error, LOGPLACE, "Incorrect column index given (%d)", j);
		exit(EXIT_FAILURE);
	}
#endif
	m[i][j] = value;
}

inline const Math::Real* Matrix4D::operator[](int index) const
{
#ifdef _DEBUG
	if ((index < 0) || (index >= MATRIX_SIZE))
	{
		LOG(Utility::Error, LOGPLACE, "Incorrect row index given (%d)", index);
		exit(EXIT_FAILURE);
	}
#endif
	return m[index];
}
inline Math::Real* Matrix4D::operator[](int index)
{
#ifdef _DEBUG
	if ((index < 0) || (index >= MATRIX_SIZE))
	{
		LOG(Utility::Error, LOGPLACE, "Incorrect row index given (%d)", index);
		exit(EXIT_FAILURE);
	}
#endif
	return m[index];
}

} /* end namespace Math */
