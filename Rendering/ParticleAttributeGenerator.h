#ifndef __RENDERING_PARTICLE_ATTRIBUTE_GENERATOR_H__
#define __RENDERING_PARTICLE_ATTRIBUTE_GENERATOR_H__

#include "Rendering.h"
#include "ParticlesContainer.h"
#include "ParticleAttributes.h"

#include "Math\RandomGenerator.h"
#include "Math\AABB.h"
#include "Math\Ellipsoid.h"
#include "Math\Plane.h"

namespace Rendering
{
	namespace Particles
	{
		/// <summary>
		/// Particle attribute generator. Particles attributes generator can generate one or more different attributes for a particle.
		/// A generator just takes a range of particles and sets new values for some specific attributes of the particles.
		/// </summary>
		class ParticleAttributeGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			ParticleAttributeGenerator(Attributes::AttributesMask attributesMask) :
				m_attributesMask(attributesMask)
			{
			}
			virtual ~ParticleAttributeGenerator()
			{
			}

			///// <summary> Particle generator copy constructor. </summary>
			//ParticleAttributeGenerator(const ParticleAttributeGenerator& particleAttributeGenerator) = delete;
			///// <summary> Particle generator move constructor. </summary>
			//ParticleAttributeGenerator(ParticleAttributeGenerator&& particleAttributeGenerator) = delete;
			///// <summary> Particle generator copy assignment operator. </summary>
			//ParticleAttributeGenerator& operator=(const ParticleAttributeGenerator& particleAttributeGenerator) = delete;
			///// <summary> Particle generator move assignment operator. </summary>
			//ParticleAttributeGenerator& operator=(ParticleAttributeGenerator&& particleAttributeGenerator) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) = 0;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Attributes::AttributesMask m_attributesMask;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticleAttributeGenerator */

		/// <summary>
		/// Generates ID for the particle.
		/// </summary>
		class IdGenerator : public ParticleAttributeGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			IdGenerator() :
				ParticleAttributeGenerator(Attributes::ID)
			{
			}
			virtual ~IdGenerator()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			inline void Set(ParticlesContainer* particleContainer, size_t i, int ID)
			{
				particleContainer->SetID(i, ID);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class IdGenerator */

		/// <summary>
		/// Generates ID and sets it in the particle.
		/// </summary>
		class BasicIdGenerator : public IdGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API BasicIdGenerator();
			RENDERING_API virtual ~BasicIdGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			int m_id;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BasicIdGenerator */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_ATTRIBUTE_GENERATOR_H__ */