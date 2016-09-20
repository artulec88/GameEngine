#ifndef __RENDERING_PARTICLE_ACCELERATION_GENERATOR_H__
#define __RENDERING_PARTICLE_ACCELERATION_GENERATOR_H__

#include "Rendering.h"
#include "ParticleAttributeGenerator.h"

#include "Math\RandomGenerator.h"

namespace Rendering
{
	namespace Particles
	{
		namespace Generators
		{
			namespace AccelerationGeneratorTypes
			{
				enum AccelerationGeneratorType
				{
					CONSTANT = 0,
					RANGE
				}; /* end enum AccelerationGeneratorType */
			} /* end namespace AccelerationGeneratorTypes */

			/// <summary>
			/// Generates acceleration for the particle.
			/// </summary>
			class AccelerationGenerator : public ParticleAttributeGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				AccelerationGenerator() :
					ParticleAttributeGenerator(Attributes::ACCELERATION)
				{
				}
				virtual ~AccelerationGenerator()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				void Set(ParticlesContainer* particleContainer, size_t i, const Math::Vector3D& acceleration)
				{
					particleContainer->SetAcceleration(i, acceleration);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				/* ==================== Non-static member variables end ==================== */
			}; /* end class AccelerationGenerator */

			/// <summary>
			/// Generates constant acceleration and sets it in the particle.
			/// </summary>
			class ConstantAccelerationGenerator : public AccelerationGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API ConstantAccelerationGenerator(Math::Real accelerationX, Math::Real accelerationY, Math::Real accelerationZ);
				RENDERING_API ConstantAccelerationGenerator(const Math::Vector3D& acceleration);
				RENDERING_API virtual ~ConstantAccelerationGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				Math::Vector3D m_acceleration;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ConstantAccelerationGenerator */

			/// <summary>
			/// Generates random acceleration from a given range and sets it in the particle.
			/// </summary>
			class RangeAccelerationGenerator : public AccelerationGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API RangeAccelerationGenerator(Math::Real minAccelerationX, Math::Real maxAccelerationX, Math::Real minAccelerationY, Math::Real maxAccelerationY, Math::Real minAccelerationZ, Math::Real maxAccelerationZ);
				RENDERING_API virtual ~RangeAccelerationGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				Math::Real m_minAccelerationX;
				Math::Real m_maxAccelerationX;
				Math::Real m_minAccelerationY;
				Math::Real m_maxAccelerationY;
				Math::Real m_minAccelerationZ;
				Math::Real m_maxAccelerationZ;
				const Math::Random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class RangeAccelerationGenerator */
		} /* end namespace Generators */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_ACCELERATION_GENERATOR_H__ */