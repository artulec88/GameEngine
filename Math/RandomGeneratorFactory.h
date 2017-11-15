#ifndef __MATH_RANDOM_GENERATOR_FACTORY_H__
#define __MATH_RANDOM_GENERATOR_FACTORY_H__

#include "Math.h"
#include "RandomGenerator.h"
#include <time.h>
#include <map>
#include <memory>

namespace math
{
	namespace random
	{
		namespace generator_ids
		{
			enum GeneratorID
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
			}; /* end enum GeneratorID */
		} /* end namespace generator_ids */

		/// <summary>
		/// The random number generator factory.
		/// </summary>
		class RandomGeneratorFactory
		{
			using RandomGeneratorsMap = std::map<generator_ids::GeneratorID, std::unique_ptr<RandomGenerator>>;
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
			~RandomGeneratorFactory();

			/// <summary> Random generator factory copy constructor. </summary>
			/// <param name="randomGeneratorFactory"> The random generator factory to copy construct from. </param>
			RandomGeneratorFactory(const RandomGeneratorFactory& randomGeneratorFactory) = delete;

			/// <summary> Random generator factory move constructor. </summary>
			/// <param name="randomGeneratorFactory"> The r-value reference of the random generator factory to move construct from. </param>
			RandomGeneratorFactory(RandomGeneratorFactory&& randomGeneratorFactory) = delete;

			/// <summary> Random generator factory copy assignment operator. </summary>
			/// <param name="randomGeneratorFactory"> The random generator factory to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned random generator factory. </returns>
			RandomGeneratorFactory& operator=(const RandomGeneratorFactory& randomGeneratorFactory) = delete;

			/// <summary> Random generator factory move assignment operator. </summary>
			/// <param name="randomGeneratorFactory"> The r-value reference of the random generator factory to move assign from. </param>
			/// <returns> The reference to the newly move-assigned random generator factory. </returns>
			RandomGeneratorFactory& operator=(RandomGeneratorFactory&& randomGeneratorFactory) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
		public:
			/// <summary>
			/// Returns a unmodifiable reference of the random generator object that client may use for generating random values.
			/// </summary>
			/// <param name="randomGeneratorID"> The ID of the random generator the client wants to use for random number generation. </param>
			/// <param name="seed"> The seed that the returned random generator will use. </param>
			/// <returns>
			/// The unmodifiable reference of the random generator stored for the specified <paramref name="randomGeneratorID"/> and initialized with the given <paramref name="seed"/>.
			/// </returns>
			MATH_API const RandomGenerator& GetRandomGenerator(generator_ids::GeneratorID randomGeneratorID, unsigned seed = (static_cast<unsigned int>(time(nullptr))));
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		private:
			RandomGeneratorsMap m_randomGenerators;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class RandomGeneratorFactory */

	} /* end namespace random */

} /* end namespace math */

#endif /* __MATH_RANDOM_GENERATOR_FACTORY_H__ */