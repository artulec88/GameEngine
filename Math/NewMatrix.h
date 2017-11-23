#ifndef __MATH_NEW_MATRIX_H__
#define __MATH_NEW_MATRIX_H__

#include "Math.h"
#ifdef PROFILING_MATH_MODULE_ENABLED
#include "Statistics.h"
#include "StatisticsStorage.h"
#endif

//#include <type_traits> // for static_assert
#include <array>

namespace math
{
	namespace matrix_orderings
	{
		enum MatrixOrdering
		{
			ROW_MAJOR = 0,
			COLUMN_MAJOR
		}; /* end enum MatrixOrdering */
	} /* end matrix_orderings */

	template <int...> struct Seq {};
	template <int N, int... S> struct Gens : Gens<N-1, N-1, S...> {};
	template <int... S> struct Gens<0, S...> { typedef Seq<S...> type; };

	template <typename T, size_t Rows = 4, size_t Cols = Rows, matrix_orderings::MatrixOrdering Ordering = matrix_orderings::ROW_MAJOR>
	class Matrix;

	template <typename T, size_t Rows = 4, size_t Cols = Rows, matrix_orderings::MatrixOrdering Ordering = matrix_orderings::ROW_MAJOR>
	struct IdentityMatrix
	{
		constexpr IdentityMatrix()
		{
		}
		
		struct Row
		{
			constexpr explicit Row(size_t r) :
				index(r)
			{
			}

			constexpr T operator[] (size_t j) const
			{
				// TODO: unreachable code
				return index < Rows && j < Cols ? j == index : (throw "out of range", -1);
				//return index == j;
			}

			const size_t index;
		};

		constexpr Row operator[] (size_t i) const { return Row(i); }
		
		template <size_t ColsRight>
		constexpr IdentityMatrix<T, Rows, ColsRight, Ordering> operator*(const IdentityMatrix<T, Cols, ColsRight, Ordering>& identityMatrix) const
		{
			return IdentityMatrix<T, Rows, ColsRight, Ordering>();
		}
		
		template <size_t ColsRight>
		constexpr Matrix<T, Rows, ColsRight, Ordering> operator*(const Matrix<T, Cols, ColsRight, Ordering>& matrix) const
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
	template <typename T, size_t Rows, size_t Cols, matrix_orderings::MatrixOrdering Ordering>
	class Matrix
	{
		/* ==================== Static variables and functions begin ==================== */
	public:
		static constexpr IdentityMatrix<T, Rows, Cols, Ordering> IDENTITY{};
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		constexpr Matrix()
		{
		}
		~Matrix()
		{
		}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		template <size_t ColsRight>
		constexpr Matrix<T, Rows, ColsRight, Ordering> operator*(const IdentityMatrix<T, Cols, ColsRight, Ordering>& identityMatrix) const
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
		//template <size_t ColsRight>
		//Matrix<T, Rows, ColsRight> operator*(const Matrix<T, Cols, ColsRight>& matrix) const
		//{
		//	Matrix<T, Rows, ColsRight> matrix;
		//	for ()
		//	return matrix;
		//}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		std::array<T, Rows * Cols> m_values;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Matrix */

} /* end namespace math */

#endif /* __MATH_NEW_MATRIX_H__ */