#ifndef __RENDERING_PARTICLE_ATTRIBUTE_GENERATOR_H__
#define __RENDERING_PARTICLE_ATTRIBUTE_GENERATOR_H__

#include "Rendering.h"
#include "ParticlesContainer.h"
#include "ParticleAttributes.h"

#include "Math/RandomGenerator.h"

namespace rendering
{
	namespace particles
	{
		namespace generators
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
				explicit ParticleAttributeGenerator(int attributeIds) :
					m_attributesMask(attributeIds)
				{	
				}

				explicit ParticleAttributeGenerator(const attributes::AttributesMask& attributesMask) :
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
				virtual void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) = 0;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				attributes::AttributesMask m_attributesMask;
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
					ParticleAttributeGenerator(attributes::ID)
				{
				}
				virtual ~IdGenerator()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				void Set(ParticlesContainer* particleContainer, size_t i, int id)
				{
					particleContainer->SetId(i, id);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
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
				RENDERING_API void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				int m_id;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class BasicIdGenerator */
		} /* end namespace generators */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_ATTRIBUTE_GENERATOR_H__ */