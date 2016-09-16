#ifndef __RENDERING_PARTICLE_VELOCITY_GENERATOR_H__
#define __RENDERING_PARTICLE_VELOCITY_GENERATOR_H__

#include "Rendering.h"
#include "ParticleAttributeGenerator.h"

#include "Math\RandomGenerator.h"

namespace Rendering
{
	namespace Particles
	{

		/// <summary>
		/// Generates velocity for the particle.
		/// </summary>
		class VelocityGenerator : public ParticleAttributeGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			VelocityGenerator() :
				ParticleAttributeGenerator(Attributes::VELOCITY)
			{
			}
			virtual ~VelocityGenerator()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			void Set(ParticlesContainer* particleContainer, size_t i, const Math::Vector3D& velocity)
			{
				particleContainer->SetVelocity(i, velocity);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class VelocityGenerator */

		   /// <summary>
		   /// Generates random velocity from a given range and sets it in the particle.
		   /// </summary>
		class BasicVelocityGenerator : public VelocityGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API BasicVelocityGenerator(Math::Real minSpeedX, Math::Real maxSpeedX, Math::Real minSpeedY, Math::Real maxSpeedY, Math::Real minSpeedZ, Math::Real maxSpeedZ);
			RENDERING_API virtual ~BasicVelocityGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Real m_minSpeedX;
			Math::Real m_maxSpeedX;
			Math::Real m_minSpeedY;
			Math::Real m_maxSpeedY;
			Math::Real m_minSpeedZ;
			Math::Real m_maxSpeedZ;
			const Math::Random::RandomGenerator& m_randomGenerator;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BasicVelocityGenerator */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_VELOCITY_GENERATOR_H__ */