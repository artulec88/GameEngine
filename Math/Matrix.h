#ifndef __MATH_MATRIX_H__
#define __MATH_MATRIX_H__

#include "Math.h"
#include "Angle.h"
#include "Vector.h"
#ifdef PROFILING_MATH_MODULE_ENABLED
#include "Statistics.h"
#include "StatisticsStorage.h"
#endif

#include "Utility\ILogger.h"

#include <iostream>
#include <type_traits> // for static_assert

//#define MATRIX_MODE_TWO_DIMENSIONS // if disabled one dimensional array will be used to store the matrix's values.

namespace Math
{
	/// <summary> The class representing the 4x4 matrix. </summary>
	/// <remarks>
	///	The matrix is defined in a column-major ordering.
	/// See http://www.in.tum.de/fileadmin/user_upload/Lehrstuehle/Lehrstuhl_XV/Teaching/SS07/Praktikum/MatricesTips.pdf for details.
	/// http://stackoverflow.com/questions/10718061/should-arrays-be-used-in-c.
	/// </remarks>
	class Matrix4D
	{
		/* ==================== Static variables and functions begin ==================== */
	public:
		static constexpr int SIZE = 4;
		MATH_API static const Matrix4D IDENTITY_MATRIX; // TODO: Try to make IDENTITY_MATRIX constexpr (see http://www.cplusplus.com/forum/general/121300/).
		static constexpr int Signum(int i, int j)
		{
			return ((i + j) % 2) ? -1 : 1;
		}
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>Default Matrix4D constructor. It creates an identity matrix.</summary>
		/// <returns>Identity matrix.</returns>
		MATH_API CONSTEXPR_IF_PROFILING_DISABLED_MATH Matrix4D() :
			Matrix4D(REAL_ONE, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE)
		{
			//static_assert(std::alignment_of(Matrix4D)::value == 16, “Alignment of Matrix4D must be 16”);
			START_PROFILING_MATH(false, "1");
			STOP_PROFILING_MATH("1");
		}
		/// <summary>
		/// Creates a 4x4 matrix and assigns a specified value for each of all 16 elements.
		/// </summary>
		MATH_API CONSTEXPR_IF_PROFILING_DISABLED_MATH Matrix4D(Real m00, Real m01, Real m02, Real m03,
			Real m10, Real m11, Real m12, Real m13,
			Real m20, Real m21, Real m22, Real m23,
			Real m30, Real m31, Real m32, Real m33) :
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			m_values({ { { { m00, m01, m02, m03 } },
			{ { m10, m11, m12, m13 } },
			{ { m20, m21, m22, m23 } },
			{ { m30, m31, m32, m33 } } } })
#else
			m_values({ { m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 } })
#endif
#ifdef PROFILING_MATH_MODULE_ENABLED
			, m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#else
			, m_padding({0, 0, 0, 0})
#endif
		{
			START_PROFILING_MATH(false, "2");
			STOP_PROFILING_MATH("2");
		}

		/// <summary> Creates a matrix based on the screen position and the scale. </summary>
		/// <param name='screenPosition'> The position on the screen </param>
		/// <param name="screenRotationAngle"> The rotation angle. </param>
		/// <param name='scale'> The scale </param>
		/// <remarks> The function is used for the gui textures. </remarks>
		MATH_API Matrix4D(const Vector2D& screenPosition, const Angle& screenRotationAngle, const Vector2D& scale) :
			Matrix4D(screenRotationAngle.Cos() * scale.x, screenRotationAngle.Sin() * scale.y, REAL_ZERO, REAL_ZERO,
				-screenRotationAngle.Sin() * scale.x, screenRotationAngle.Cos() * scale.y, REAL_ZERO, REAL_ZERO,
				REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO,
				screenPosition.x, screenPosition.y, REAL_ZERO, REAL_ONE)
		{
			START_PROFILING_MATH(false, "3");
			STOP_PROFILING_MATH("3");
		}
		/// <summary>Creates scale matrix based on the specified parameter.</summary>
		/// <param name='scale'>The scale in all dimensions: X, Y and Z.</param>
		/// <returns>Scale matrix.</returns>
		MATH_API explicit CONSTEXPR_IF_PROFILING_DISABLED_MATH Matrix4D(Real scale) :
			Matrix4D(scale, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO, scale, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO, scale, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE)
		{
			START_PROFILING_MATH(false, "4");
			STOP_PROFILING_MATH("4");
		}
		/// <summary>Creates translation matrix based on the specified parameters.</summary>
		/// <param name='posX'>The X coordinate of the translation.</param>
		/// <param name='posY'>The Y coordinate of the translation.</param>
		/// <param name='posZ'>The Z coordinate of the translation.</param>
		/// <returns>Translation matrix.</returns>
		MATH_API CONSTEXPR_IF_PROFILING_DISABLED_MATH Matrix4D(Real posX, Real posY, Real posZ) :
			Matrix4D(REAL_ONE, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE, REAL_ZERO, posX, posY, posZ, REAL_ONE)
		{
			START_PROFILING_MATH(false, "5");
			STOP_PROFILING_MATH("5");
		}
		/// <summary>Creates translation matrix based on the specified parameter.</summary>
		/// <param name='pos'>The 3D translation vector.</param>
		/// <returns>Translation matrix.</returns>
		MATH_API explicit CONSTEXPR_IF_PROFILING_DISABLED_MATH Matrix4D(const Vector3D& pos) :
			Matrix4D(REAL_ONE, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE, REAL_ZERO, pos.GetX(), pos.GetY(), pos.GetZ(), REAL_ONE)
		{
			START_PROFILING_MATH(false, "6");
			STOP_PROFILING_MATH("6");
		}
		/// <summary>Creates rotation matrix based on the specified parameters.</summary>
		/// <param name='angleX'>The rotation angle around X axis.</param>
		/// <param name='angleY'>The rotation angle around Y axis.</param>
		/// <returns>Rotation matrix.</returns>
		MATH_API Matrix4D(const Angle& angleX, const Angle& angleY);
		/// <summary>Creates rotation matrix based on the specified parameters.</summary>
		/// <param name='angleX'>The rotation angle around X axis.</param>
		/// <param name='angleY'>The rotation angle around Y axis.</param>
		/// <param name='angleZ'>The rotation angle around Z axis.</param>
		/// <returns>Rotation matrix.</returns>
		/// <remarks> See http://planning.cs.uiuc.edu/node102.html </remarks>
		MATH_API Matrix4D(const Angle& angleX, const Angle& angleY, const Angle& angleZ);
		/// <summary>Creates rotation matrix based on the specified parameters.</summary>
		/// <param name='forward'>The forward vector. It must be normalized.</param>
		/// <param name='up'>The up vector. It must be normalized.</param>
		/// <returns>Rotation matrix.</returns>
		MATH_API Matrix4D(const Vector3D& forward, const Vector3D& up);
		/// <summary>Creates rotation matrix based on the specified parameters.</summary>
		/// <param name='forward'>The forward vector. It must be normalized.</param>
		/// <param name='up'>The up vector. It must be normalized.</param>
		/// <param name='right'>The right vector. It must be normalized.</param>
		/// <returns>Rotation matrix.</returns>
		MATH_API Matrix4D(const Vector3D& forward, const Vector3D& up, const Vector3D& right);
		/// <summary>Creates perspective projection matrix based on the specified parameters.</summary>
		/// <param name='fov'>Field of view</param>
		/// <param name='aspect'>Aspect ratio</param>
		/// <param name='nearPlane'>Near plane</param>
		/// <param name='farPlane'>Far plane</param>
		/// <returns>Perspective projection matrix.</returns>
		MATH_API Matrix4D(const Angle& fov, Real aspect, Real nearPlane, Real farPlane);
		/// <summary>Creates ortographic projection matrix based on the specified parameters.</summary>
		/// <param name='left'>The left coordinate for the vertical clipping planes.</param>
		/// <param name='right'>The right coordinate for the vertical clipping planes.</param>
		/// <param name='bottom'>The bottom coordinate for the horizontal clipping planes.</param>
		/// <param name='top'>The top coordinate for the horizontal clipping planes.</param>
		/// <param name='nearPlane'>The distance to the nearer depth clipping planes. The value is negative if the plane is to be behind the viewer.</param>
		/// <param name='farPlane'>The distance to the farther depth clipping planes. The value is negative if the plane is to be behind the viewer.</param>
		/// <returns>Ortographic projection matrix.</returns>
		MATH_API Matrix4D(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane);

		/// <summary>A simple matrix copy-constructor.</summary>
		/// <param name='mat'>A matrix which is to be copied.</param>
		/// <returns>A deep copy of the given matrix.</returns>
		MATH_API Matrix4D(const Matrix4D& mat);

		/// <summary>A simple matrix move-constructor.</summary>
		/// <param name='mat'>A matrix which is to be moved.</param>
		MATH_API Matrix4D(Matrix4D&& mat);

		/// <summary> Matrix copy assignment operator. </summary>
		MATH_API Matrix4D& operator=(const Matrix4D& mat);
		/// <summary> Matrix move assignment operator. </summary>
		MATH_API Matrix4D& operator=(Matrix4D&& mat);

		/// <summary>A simple matrix destructor.</summary>
		MATH_API ~Matrix4D();
	private:
		Matrix4D(const Math::Real* values);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		MATH_API inline void SetElement(int i, int j, Real value);
		MATH_API inline Real GetElement(int i, int j) const;

		/// <summary>
		/// Performs the multiplication of matrices. The given matrix <paramref name="m"/> is multiplied with the current matrix.
		/// </summary>
		/// <returns> The result of multiplying the given matrix <paramref name="m"/> with the current matrix. </returns>
		MATH_API Matrix4D operator*(const Matrix4D& m) const;
		MATH_API Vector3D operator*(const Vector3D& vec) const;
		MATH_API Vector4D operator*(const Vector4D& vec) const;
		//MATH_API Matrix4D& operator*=(const Matrix4D& m) const; // TODO: Implement
		MATH_API bool operator==(const Matrix4D& m) const;
		MATH_API bool operator!=(const Matrix4D& m) const;

		/// <summary>
		/// Returns the pointer to the matrix data.
		/// </summary>
		/// <returns> Pointer to constant matrix data. </returns>
		MATH_API inline const Real* Data() const
		{
			//CHECK_CONDITION_EXIT_MATH((index >= 0) && (index < SIZE), Utility::Logging::ERR, "Incorrect row index given (", index, ")");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			return m_values[0].data();
#else
			return m_values.data();
#endif
		}

		MATH_API void SetScaleMatrix(Real scaleX, Real scaleY, Real scaleZ);
		MATH_API void SetOrthogonalProjection(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane);
		MATH_API void SetPerspectiveProjection(const Angle& fov, Real aspect, Real nearPlane, Real farPlane);

		MATH_API Vector3D Transform(const Vector3D& vec); // TODO: Write tests

		MATH_API Matrix4D Transposition() const;
		MATH_API Matrix4D Inversion() const; // TODO: Write tests
		MATH_API Real Det(int p, int q) const;

		/// <summary>Checks whether the matrix is an identity matrix or not. It should be used only in tests.</summary>
		/// <returns>True if the matrix is an identity matrix. False otherwise.</returns>
		MATH_API bool IsIdentity() const;

		friend std::ostream& operator<<(std::ostream& out, const Matrix4D& matrix)
		{
			out << std::setprecision(4) << matrix.GetElement(0, 0) << "; " << matrix.GetElement(0, 1) << "; " << matrix.GetElement(0, 2) << "; " << matrix.GetElement(0, 3) << std::endl;
			out << std::setprecision(4) << matrix.GetElement(1, 0) << "; " << matrix.GetElement(1, 1) << "; " << matrix.GetElement(1, 2) << "; " << matrix.GetElement(1, 3) << std::endl;
			out << std::setprecision(4) << matrix.GetElement(2, 0) << "; " << matrix.GetElement(2, 1) << "; " << matrix.GetElement(2, 2) << "; " << matrix.GetElement(2, 3) << std::endl;
			out << std::setprecision(4) << matrix.GetElement(3, 0) << "; " << matrix.GetElement(3, 1) << "; " << matrix.GetElement(3, 2) << "; " << matrix.GetElement(3, 3) << std::endl;
			return out;
		}
	private:
		/// <summary>
		/// Returns the pointer to the matrix data.
		/// </summary>
		/// <returns> Pointer to constant matrix data. </returns>
		MATH_API inline Real* DataPtr()
		{
			//CHECK_CONDITION_EXIT_MATH((index >= 0) && (index < SIZE), Utility::Logging::ERR, "Incorrect row index given (", index, ")");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			return &m_values[0][0];
#else
			return m_values.data();
#endif
		}
		void M4x4_SSE(const Real* A, const Real* B, Real* C) const;
		inline const Real* operator[](int index) const
		{
			CHECK_CONDITION_EXIT_MATH((index >= 0) && (index < SIZE), Utility::Logging::ERR, "Incorrect row index given (", index, ")");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			return &m_values[index][0];
#else
			return &m_values[index];
#endif
		}
		inline Real* operator[](int index)
		{
			CHECK_CONDITION_EXIT_MATH((index >= 0) && (index < SIZE), Utility::Logging::ERR, "Incorrect row index given (", index, ")");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
			return &m_values[index][0];
#else
			return &m_values[index];
#endif
		}
		void SetRotationFromVectors(const Vector3D& forward, const Vector3D& up, const Vector3D& right);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		// TODO: Consider using a one-dimensional array to store SIZE * SIZE elements.
		// TODO: Read an article http://stackoverflow.com/questions/17259877/1d-or-2d-array-whats-faster.
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		std::array<std::array<Real, SIZE>, SIZE> m_values;
#else
		std::array<Real, SIZE * SIZE> m_values;
#endif

#ifdef PROFILING_MATH_MODULE_ENABLED
		Statistics::ClassStats& m_classStats;
#else
		std::array<uint8_t, 4> m_padding;
#endif
		/* ==================== Non-static member variables end ==================== */

	}; /* end class Matrix4D */

	inline Real Matrix4D::GetElement(int i, int j) const
	{
		CHECK_CONDITION_EXIT_MATH((i >= 0) && (i < SIZE), Utility::Logging::ERR, "Incorrect row index given (", i, ")");
		CHECK_CONDITION_EXIT_MATH((j >= 0) && (j < SIZE), Utility::Logging::ERR, "Incorrect column index given (", j, ")");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		return m_values[i][j];
#else
		return m_values[i * SIZE + j];
#endif
	}

	inline void Matrix4D::SetElement(int i, int j, Real value)
	{
		CHECK_CONDITION_EXIT_MATH((i >= 0) && (i < SIZE), Utility::Logging::ERR, "Incorrect row index given (", i, ")");
		CHECK_CONDITION_EXIT_MATH((j >= 0) && (j < SIZE), Utility::Logging::ERR, "Incorrect column index given (", j, ")");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		m_values[i][j] = value;
#else
		m_values[i * SIZE + j] = value;
#endif
	}

} /* end namespace Math */

#endif /* __MATH_MATRIX_H__ */