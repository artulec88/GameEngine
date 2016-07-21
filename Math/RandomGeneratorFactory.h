#ifndef __MATH_RANDOM_GENERATOR_FACTORY_H__
#define __MATH_RANDOM_GENERATOR_FACTORY_H__

#include "Math.h"
#include "RandomGenerator.h"
#include <time.h>
#include <map>
#include <memory>

namespace Math
{
	namespace Random
	{
		namespace Generators
		{
			enum Generator
			{
				SIMPLE = 0,
				DEFAULT_RANDOM_ENGINE,
				DISCARD_BLOCK_ENGINE,
				INDEPENDENT_BITS_ENGINE,
				KNUTH_B,
				LINEAR_CONGRUENTIAL_ENGINE,
				MERSENNE_TWISTER_ENGINE,
				MINSTD_RAND,
				MINSTD_RAND0,
				MT19937,
				MT19937_64,
				RANDOM_DEVICE,
				RANLUX24,
				RANLUX24_BASE,
				RANLUX48,
				RANLUX48_BASE,
				SHUFFLE_ORDER_ENGINE,
				SUBTRACT_WITH_CARRY_ENGINE
			}; /* end enum Generator */
		} /* end namespace Generators */

		/// <summary>
		/// The random number generator factory.
		/// </summary>
		class RandomGeneratorFactory
		{
			typedef std::map<Generators::Generator, std::unique_ptr<RandomGenerator>> RandomGeneratorsMap;
		/* ==================== Static variables begin ==================== */
		public:
			MATH_API static RandomGeneratorFactory& GetRandomGeneratorFactory();
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		private:
			/// <summary>
			/// The constructor of the random number generator factory.
			/// </summary>
			RandomGeneratorFactory();

			/// <summary>
			/// The destructor of the random number generator factory.
			/// </summary>
			~RandomGeneratorFactory(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
		public:
			/// <summary>
			/// Returns a random generator object.
			/// </summary>
			MATH_API const RandomGenerator& GetRandomGenerator(Generators::Generator randomGenerator, unsigned seed = ((unsigned int)time(NULL)));
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		private:
			RandomGeneratorsMap m_randomGenerators;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class RandomGeneratorFactory */

	} /* end namespace Random */

} /* end namespace Math */

#endif /* __MATH_RANDOM_GENERATOR_FACTORY_H__ */