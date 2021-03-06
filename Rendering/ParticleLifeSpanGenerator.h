#ifndef __RENDERING_PARTICLE_LIFE_SPAN_GENERATOR_H__
#define __RENDERING_PARTICLE_LIFE_SPAN_GENERATOR_H__

#include "Rendering.h"
#include "ParticleAttributeGenerator.h"

#include "Math/RandomGenerator.h"
#include "Math/AABB.h"

namespace rendering
{
	namespace particles
	{
		namespace generators
		{
			namespace life_span_generator_types
			{
				enum LifeSpanGeneratorType
				{
					CONSTANT = 0,
					RANGE
				}; /* end enum LifeSpanGeneratorType */
			} /* end namespace life_span_generator_types */

			/// <summary>
			/// Generates life span limit for the particles.
			/// </summary>
			class LifeSpanLimitGenerator : public ParticleAttributeGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				LifeSpanLimitGenerator() :
					ParticleAttributeGenerator(attributes::LIFE_SPAN)
				{
				}
				virtual ~LifeSpanLimitGenerator()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				void Set(ParticlesContainer* particleContainer, size_t i, math::Real lifeSpanLimit)
				{
					particleContainer->SetLifeSpanLimit(i, lifeSpanLimit);
					particleContainer->SetLifeSpan(i, REAL_ZERO);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
				/* ==================== Non-static member variables end ==================== */
			}; /* end class LifeSpanLimitGenerator */

			/// <summary>
			/// Generates constant life span limit and sets it in the particles.
			/// </summary>
			class ConstantLifeSpanLimitGenerator : public LifeSpanLimitGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API explicit ConstantLifeSpanLimitGenerator(math::Real lifeSpanLimit);
				RENDERING_API virtual ~ConstantLifeSpanLimitGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Real m_lifeSpanLimit;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ConstantLifeSpanLimitGenerator */

			/// <summary>
			/// Generates random life span limit from a given range and sets it in the particles.
			/// </summary>
			class RangeLifeSpanLimitGenerator : public LifeSpanLimitGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API RangeLifeSpanLimitGenerator(math::Real minLifeSpanLimit, math::Real maxLifeSpanLimit);
				RENDERING_API virtual ~RangeLifeSpanLimitGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Real m_minLifeSpanLimit;
				math::Real m_maxLifeSpanLimit;
				const math::random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class RangeLifeSpanLimitGenerator */
		} /* end namespace generators */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_LIFE_SPAN_GENERATOR_H__ */