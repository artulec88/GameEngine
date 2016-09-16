#ifndef __RENDERING_PARTICLE_LIFE_SPAN_GENERATOR_H__
#define __RENDERING_PARTICLE_LIFE_SPAN_GENERATOR_H__

#include "Rendering.h"
#include "ParticleAttributeGenerator.h"

#include "Math\RandomGenerator.h"
#include "Math\AABB.h"
#include "Math\Ellipsoid.h"
#include "Math\Plane.h"

namespace Rendering
{
	namespace Particles
	{

		/// <summary>
		/// Generates life span limit for the particle.
		/// </summary>
		class LifeSpanLimitGenerator : public ParticleAttributeGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			LifeSpanLimitGenerator() :
				ParticleAttributeGenerator(Attributes::LIFE_SPAN)
			{
			}
			virtual ~LifeSpanLimitGenerator()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			inline void Set(ParticlesContainer* particleContainer, size_t i, Math::Real lifeSpanLimit)
			{
				particleContainer->SetLifeSpanLimit(i, lifeSpanLimit);
				particleContainer->SetLifeSpan(i, REAL_ZERO);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class LifeSpanLimitGenerator */

		   /// <summary>
		   /// Generates random life span limit from a given range and sets it in the particle.
		   /// </summary>
		class BasicLifeSpanLimitGenerator : public LifeSpanLimitGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API BasicLifeSpanLimitGenerator(Math::Real minLifeSpanLimit, Math::Real maxLifeSpanLimit);
			RENDERING_API virtual ~BasicLifeSpanLimitGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Real m_minLifeSpanLimit;
			Math::Real m_maxLifeSpanLimit;
			const Math::Random::RandomGenerator& m_randomGenerator;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BasicLifeSpanLimitGenerator */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_LIFE_SPAN_GENERATOR_H__ */