#ifndef __MATH_RANDOM_GENERATOR_H__
#define __MATH_RANDOM_GENERATOR_H__

#include "Math.h"
#include <random>

namespace math
{
	namespace random
	{
		/// <summary>
		/// The random number generator.
		/// </summary>
		class RandomGenerator
		{
		/* ==================== Static variables begin ==================== */
		protected:
			static bool CheckIfInRange(int value, int lowerBound, int upperBound);
			static bool CheckIfInRange(Real value, Real lowerBound, Real upperBound);
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// The constructor of the random number generator.
			/// </summary>
			MATH_API explicit RandomGenerator(unsigned seed);

			/// <summary>
			/// The destructor of the random number generator.
			/// </summary>
			MATH_API virtual ~RandomGenerator();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
			MATH_API unsigned GetSeed() const
			{
				return m_seed;
			}

			MATH_API void SetSeed(unsigned seed) const
			{
				// We don't set the m_seed value (and that's fine!).
				srand(seed);
			}

			/// <summary>
			/// Returns a pseudo-random integral number.
			/// </summary>
			MATH_API virtual int NextInt() const = 0;

			/// <summary>
			/// Returns a pseudo-random integral number in the range [<paramref name="lowerBound"/>; <paramref name="upperBound"/>].
			/// For example, for <paramref name="lowerBound"/> equal to <code>5</code> and <paramref name="upperBound"/>
			/// equal to <code>10</code> the result may be one of the following integers: 5, 6, 7, 8, 9, 10.
			/// </summary>
			/// <param name="lowerBound">Specifies the lower bound (inclusive) of the random integer result. </param>
			/// <param name="upperBound">Specifies the upper bound (inclusive) of the random integer result. </param>
			MATH_API virtual int NextInt(int lowerBound, int upperBound) const = 0;

			/// <summary>
			/// Returns a pseudo-random floating-point number in the range [0; 1].
			/// </summary>
			MATH_API virtual Real NextFloat() const = 0;

			/// <summary>
			/// Returns a pseudo-random floating-point number in the range [<paramref name="lowerBound"/>; <paramref name="upperBound"/>].
			/// </summary>
			/// <param name="lowerBound">Specifies the lower bound (inclusive) of the random floating-point result. </param>
			/// <param name="upperBound">Specifies the upper bound (inclusive) of the random floating-point result. </param>
			MATH_API virtual Real NextFloat(Real lowerBound, Real upperBound) const = 0;
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
			unsigned m_seed;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class RandomGenerator */

		/// <summary>
		/// The simple random number generator.
		/// </summary>
		class SimpleRandomGenerator : public RandomGenerator
		{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// The constructor of the simple random number generator.
			/// </summary>
			MATH_API explicit SimpleRandomGenerator(unsigned seed);

			/// <summary>
			/// The destructor of the simple random number generator.
			/// </summary>
			MATH_API virtual ~SimpleRandomGenerator();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
			/// <summary>
			/// Returns a pseudo-random integral number in the range between <code>0</code> and <code>RAND_MAX</code>.
			/// </summary>
			MATH_API int NextInt() const override;

			/// <summary>
			/// Returns a pseudo-random integral number in the range [<paramref name="lowerBound/>; <paramref name="upperBound"/>).
			/// For example, for <paramref name="lowerBound"/> equal to <code>5</code> and <paramref name="upperBound"/>
			/// equal to <code>10</code> the result may be one of the following integers: 5, 6, 7, 8, 9.
			/// </summary>
			/// <param name="lowerBound">Specifies the lower bound (inclusive) of the random integer result. </param>
			/// <param name="upperBound">Specifies the upper bound (exclusive) of the random integer result. </param>
			MATH_API int NextInt(int lowerBound, int upperBound) const override;

			/// <summary>
			/// Returns a pseudo-random floating-point number in the range [0; 1].
			/// </summary>
			MATH_API Real NextFloat() const override;

			/// <summary>
			/// Returns a pseudo-random floating-point number in the range [<paramref name="lowerBound/>; <paramref name="upperBound"/>).
			/// </summary>
			/// <param name="lowerBound">Specifies the lower bound (inclusive) of the random floating-point result. </param>
			/// <param name="upperBound">Specifies the upper bound (exclusive) of the random floating-point result. </param>
			MATH_API Real NextFloat(Real lowerBound, Real upperBound) const override;
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
		}; /* end class SimpleRandomGenerator */

		/// <summary>
		/// The default random number generator.
		/// </summary>
		class DefaultRandomGenerator : public RandomGenerator
		{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// The constructor of the default random number generator.
			/// </summary>
			MATH_API explicit DefaultRandomGenerator(unsigned seed);

			/// <summary>
			/// The destructor of the deault random number generator.
			/// </summary>
			MATH_API virtual ~DefaultRandomGenerator();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
			/// <summary>
			/// Returns a pseudo-random integral number in the range between <code>0</code> and <code>RAND_MAX</code>.
			/// </summary>
			MATH_API int NextInt() const override;

			/// <summary>
			/// Returns a pseudo-random integral number in the range [<paramref name="lowerBound/>; <paramref name="upperBound"/>).
			/// For example, for <paramref name="lowerBound"/> equal to <code>5</code> and <paramref name="upperBound"/>
			/// equal to <code>10</code> the result may be one of the following integers: 5, 6, 7, 8, 9.
			/// </summary>
			/// <param name="lowerBound">Specifies the lower bound (inclusive) of the random integer result. </param>
			/// <param name="upperBound">Specifies the upper bound (exclusive) of the random integer result. </param>
			MATH_API int NextInt(int lowerBound, int upperBound) const override;

			/// <summary>
			/// Returns a pseudo-random floating-point number in the range [0; 1].
			/// </summary>
			MATH_API Real NextFloat() const override;

			/// <summary>
			/// Returns a pseudo-random floating-point number in the range [<paramref name="lowerBound/>; <paramref name="upperBound"/>).
			/// </summary>
			/// <param name="lowerBound">Specifies the lower bound (inclusive) of the random floating-point result. </param>
			/// <param name="upperBound">Specifies the upper bound (exclusive) of the random floating-point result. </param>
			MATH_API Real NextFloat(Real lowerBound, Real upperBound) const override;
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
			//std::default_random_engine m_engine;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class DefaultRandomGenerator */

	} /* end namespace random */

} /* end namespace math */

#endif /* __MATH_RANDOM_GENERATOR_H__ */