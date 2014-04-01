#pragma once

#include "Math.h"
#include "Vector.h"
//#include "Utility\ISerializable.h"
#include <string>

namespace Math
{

#define MATRIX_SIZE 4

class MATH_API Matrix4D// : public Utility::ISerializable
{
/* ==================== Non-static member variables begin ==================== */
protected:
	Real m[MATRIX_SIZE][MATRIX_SIZE];
/* ==================== Non-static member variables end ==================== */

/* ==================== Static functions begin ==================== */
public:
	static Matrix4D Identity();
	static Matrix4D PerspectiveProjection(Real fov /* Field of View */, Real aspect /* Aspect */, Real nearPlane /* Near plane */, Real farPlane /* Far plane */);
	static Matrix4D OrtographicProjection(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane);
	static Matrix4D Translation(Real x, Real y, Real z);
	static Matrix4D Translation(const Vector3D& vec);
	static Matrix4D Scale(Real x, Real y, Real z);
	static Matrix4D Scale(const Vector3D& vec);
	static Matrix4D Rotation(Real x, Real y, Real z, Real angleInDegrees);
	static Matrix4D Rotation(const Vector3D& vec, Real angleInDegrees);

	static Matrix4D Rotation(Real x, Real y, Real z); // TODO: Check all Matrix4D::Rotation(...) functions.

	static Matrix4D Rotation(const Vector3D& forward, const Vector3D& up, const Vector3D& right);

	static int Signum(int i, int j);
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Matrix4D();
	Matrix4D(const Matrix4D& mat);
	~Matrix4D();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void SetElement(int i, int j, Real value);
	Real GetElement (int i, int j) const { };
	
	Matrix4D operator*(const Matrix4D& m) const;
	Vector3D operator*(const Vector3D& vec) const;
	bool operator==(const Matrix4D& m) const;
	Matrix4D& operator=(const Matrix4D& mat);

	const Math::Real* operator[](int index) const;
	Math::Real* operator[](int index);

	Matrix4D Transposition() const;
	Matrix4D Inversion() const;
	Real Det(int p, int q) const;
	
	/**
	 * Checks whether the matrix is an identity matrix or not.
	 * 
	 * @brief Used only during tests
	 */
	bool IsIdentity() const;

	std::string ToString() const;
/* ==================== Non-static member functions end ==================== */
}; /* end class Matrix */

inline Real Matrix4D::GetElement (int i, int j) const
{
#ifdef _DEBUG
	if ((i < 0) || (i >= MATRIX_SIZE))
	{
		fprintf(stderr, "Incorrect row index given (%d)", i);
		exit(-1);
	}
	if ((j < 0) || (j >= MATRIX_SIZE))
	{
		fprintf(stderr, "Incorrect column index given (%d)", j);
		exit(-1);
	}
#endif
	return m[i][j];
}

inline void Matrix4D::SetElement(int i, int j, Real value)
{
#ifdef _DEBUG
	if ((i < 0) || (i >= 3))
	{
		fprintf(stderr, "Incorrect row index given (%d)", i);
		exit(-1);
	}
	if ((j < 0) || (j >= 3))
	{
		fprintf(stderr, "Incorrect column index given (%d)", j);
		exit(-1);
	}
#endif
	m[i][j] = value;
}

inline const Math::Real* Matrix4D::operator[](int index) const
{
	if ((index < 0) || (index >= MATRIX_SIZE))
	{
		fprintf(stderr, "Incorrect row index given (%d)", index);
		exit(-1);
	}
	return m[index];
}
inline Math::Real* Matrix4D::operator[](int index)
{
	if ((index < 0) || (index >= MATRIX_SIZE))
	{
		fprintf(stderr, "Incorrect row index given (%d)", index);
		exit(-1);
	}
	return m[index];
}

} /* end namespace Math */
