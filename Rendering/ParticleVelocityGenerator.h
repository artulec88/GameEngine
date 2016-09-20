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
			/// Generates constant velocity for particles.
			/// </summary>
			class ConstantVelocityGenerator : public VelocityGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API ConstantVelocityGenerator(const Math::Vector3D& velocity);
				RENDERING_API ConstantVelocityGenerator(Math::Real velocityX, Math::Real velocityY, Math::Real velocityZ);
				RENDERING_API virtual ~ConstantVelocityGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				Math::Vector3D m_velocity;
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
				RENDERING_API RangeVelocityGenerator(Math::Real minSpeedX, Math::Real maxSpeedX, Math::Real minSpeedY, Math::Real maxSpeedY, Math::Real minSpeedZ, Math::Real maxSpeedZ);
				RENDERING_API virtual ~RangeVelocityGenerator();
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
			}; /* end class RangeVelocityGenerator */
		} /* end namespace Generators */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_VELOCITY_GENERATOR_H__ */