#ifndef __RENDERING_PARTICLE_VELOCITY_GENERATOR_H__
#define __RENDERING_PARTICLE_VELOCITY_GENERATOR_H__

#include "Rendering.h"
#include "ParticleAttributeGenerator.h"

#include "Math\RandomGenerator.h"

namespace Rendering
{
	namespace Particles
	{
		namespace Generators
		{
			namespace VelocityGeneratorTypes
			{
				enum VelocityGeneratorType
				{
					CONSTANT = 0,
					RANGE
				}; /* end enum VelocityGeneratorType */
			} /* end namespace VelocityGeneratorTypes */

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
				void Set(ParticlesContainer* particleContainer, size_t i, const math::Vector3D& velocity)
				{
					particleContainer->SetVelocity(i, velocity);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				/* ==================== Non-static member variables end ==================== */
			}; /* end class VelocityGenerator */

			/// <summary>
			/// Generates constant velocity for particles.
			/// </summary>
			class ConstantVelocityGenerator : public VelocityGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API ConstantVelocityGenerator(const math::Vector3D& velocity);
				RENDERING_API ConstantVelocityGenerator(math::Real velocityX, math::Real velocityY, math::Real velocityZ);
				RENDERING_API virtual ~ConstantVelocityGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Vector3D m_velocity;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ConstantVelocityGenerator */

			/// <summary>
			/// Generates random velocity from a given range and sets it in the particle.
			/// </summary>
			class RangeVelocityGenerator : public VelocityGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API RangeVelocityGenerator(math::Real minSpeedX, math::Real maxSpeedX, math::Real minSpeedY, math::Real maxSpeedY, math::Real minSpeedZ, math::Real maxSpeedZ);
				RENDERING_API virtual ~RangeVelocityGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Real m_minSpeedX;
				math::Real m_maxSpeedX;
				math::Real m_minSpeedY;
				math::Real m_maxSpeedY;
				math::Real m_minSpeedZ;
				math::Real m_maxSpeedZ;
				const math::random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class RangeVelocityGenerator */
		} /* end namespace Generators */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_VELOCITY_GENERATOR_H__ */