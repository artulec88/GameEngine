#pragma once

#include "Math.h"
#include "Vector.h"
#include "Utility\ISerializable.h"
#include <string>

namespace Math
{

#define MATRIX_SIZE 4

class Matrix4D : public Utility::ISerializable
{
/* ==================== Non-static member variables begin ==================== */
protected:
	Real m[MATRIX_SIZE][MATRIX_SIZE];
/* ==================== Non-static member variables end ==================== */

/* ==================== Static functions begin ==================== */
public:
	static Matrix4D Identity();
	static Matrix4D Projection(Real fov /* Field of View */, Real aspect /* Aspect */, Real nearPlane /* Near plane */, Real farPlane /* Far plane */);
	static Matrix4D Translation(Real x, Real y, Real z);
	static Matrix4D Translation(const Vector3D& vec);
	static Matrix4D Scale(Real x, Real y, Real z);
	static Matrix4D Scale(const Vector3D& vec);
	static Matrix4D Rotation(Real x, Real y, Real z, Real angleInDegrees);
	static Matrix4D Rotation(const Vector3D& vec, Real angleInDegrees);

	static int Signum(int i, int j);
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Matrix4D(void);
	~Matrix4D(void);
/* ==================== Constructors and destructors end ==================== */


public:
	void SetElement(int i, int j, Real value);
	Real GetElement (int i, int j) const;
	
	Matrix4D operator*(const Matrix4D& m) const;
	Vector3D operator*(const Vector3D& vec) const;
	bool operator==(const Matrix4D& m) const;

	Matrix4D Transposition() const;
	Matrix4D Inversion() const;
	Real Det(int p, int q) const;
	
	bool IsIdentity() const;

	virtual std::string ToString() const; // derived from ISerializable interface

}; /* end class Matrix */

} /* end namespace Math */