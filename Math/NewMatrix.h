#ifndef __MATH_NEW_MATRIX_H__
#define __MATH_NEW_MATRIX_H__

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
#include <string>
#include <array>

namespace Math
{
	namespace MatrixOrderings
	{
		enum MatrixOrdering
		{
			ROW_MAJOR = 0,
			COLUMN_MAJOR
		}; /* end enum MatrixOrdering */
	} /* end MatrixOrderings */

	template <int...> struct seq {};
	template <int N, int... S> struct gens : gens<N-1, N-1, S...> {};
	template <int... S> struct gens<0, S...> { typedef seq<S...> type; };

	template <typename T, std::size_t ROWS = 4, std::size_t COLS = ROWS, MatrixOrderings::MatrixOrdering ORDERING = MatrixOrderings::ROW_MAJOR>
	class Matrix;

	template <typename T, std::size_t ROWS = 4, std::size_t COLS = ROWS, MatrixOrderings::MatrixOrdering ORDERING = MatrixOrderings::ROW_MAJOR>
	struct IdentityMatrix
	{
		constexpr IdentityMatrix()
		{
		}
		
		struct Row
		{
			constexpr explicit Row(std::size_t r) :
				m_i(r)
			{
			}

			constexpr T operator[] (std::size_t j) const
			{
				return m_i < ROWS && j < COLS ? j == m_i : (throw "out of range", -1);
				//return m_i == j;
			}

			const std::size_t m_i;
		};

		constexpr Row operator[] (std::size_t i) const { return Row(i); }
		
		template <std::size_t COLS_RIGHT>
		constexpr IdentityMatrix<T, ROWS, COLS_RIGHT, ORDERING> operator*(const IdentityMatrix<T, COLS, COLS_RIGHT, ORDERING>& identityMatrix) const
		{
			return IdentityMatrix<T, ROWS, COLS_RIGHT, ORDERING>();
		}
		
		template <std::size_t COLS_RIGHT>
		constexpr Matrix<T, ROWS, COLS_RIGHT, ORDERING> operator*(const Matrix<T, COLS, COLS_RIGHT, ORDERING>& matrix) const
		{
			return matrix;
		}
	};

	/// <summary> The class representing the matrix. </summary>
	/// <remarks>
	///	The matrix is defined in a column-major ordering.
	/// See http://www.in.tum.de/fileadmin/user_upload/Lehrstuehle/Lehrstuhl_XV/Teaching/SS07/Praktikum/MatricesTips.pdf for details.
	/// http://stackoverflow.com/questions/10718061/should-arrays-be-used-in-c.
	/// </remarks>
	template <typename T, std::size_t ROWS, std::size_t COLS, MatrixOrderings::MatrixOrdering ORDERING>
	class Matrix
	{
		/* ==================== Static variables and functions begin ==================== */
	public:
		static constexpr IdentityMatrix<T, ROWS, COLS, ORDERING> IDENTITY{};
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		constexpr Matrix()
		{
		}
		~Matrix()
		{
		}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		template <std::size_t COLS_RIGHT>
		constexpr Matrix<T, ROWS, COLS_RIGHT, ORDERING> operator*(const IdentityMatrix<T, COLS, COLS_RIGHT, ORDERING>& identityMatrix) const
		{
			return *this;
		}

		/// <summary>
		/// Multiplying two matrices. The main condition of matrix multiplication is that the number of columns of the 1st matrix must equal
		/// the number of rows of the 2nd one. As a result of multiplication new matrix is created that has the same number of rows as the 1st matrix
		/// and the same number of columns as the 2nd matrix.
		/// For example, if you multiply a NxM matrix with a MxK matrix you'll get a new matrix of NxK size.
		/// See http://matrix.reshish.com/multiplication.php.
		/// </summary>
		//template <std::size_t COLS_RIGHT>
		//Matrix<T, ROWS, COLS_RIGHT> operator*(const Matrix<T, COLS, COLS_RIGHT>& matrix) const
		//{
		//	Matrix<T, ROWS, COLS_RIGHT> matrix;
		//	for ()
		//	return matrix;
		//}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		std::array<T, ROWS * COLS> m_values;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Matrix */

} /* end namespace Math */

#endif /* __MATH_NEW_MATRIX_H__ */