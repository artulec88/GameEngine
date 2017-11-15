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
				void Set(ParticlesContainer* particleContainer, size_t i, const math::Vector3D& acceleration)
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
				RENDERING_API ConstantAccelerationGenerator(math::Real accelerationX, math::Real accelerationY, math::Real accelerationZ);
				RENDERING_API ConstantAccelerationGenerator(const math::Vector3D& acceleration);
				RENDERING_API virtual ~ConstantAccelerationGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Vector3D m_acceleration;
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
				RENDERING_API RangeAccelerationGenerator(math::Real minAccelerationX, math::Real maxAccelerationX, math::Real minAccelerationY, math::Real maxAccelerationY, math::Real minAccelerationZ, math::Real maxAccelerationZ);
				RENDERING_API virtual ~RangeAccelerationGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Real m_minAccelerationX;
				math::Real m_maxAccelerationX;
				math::Real m_minAccelerationY;
				math::Real m_maxAccelerationY;
				math::Real m_minAccelerationZ;
				math::Real m_maxAccelerationZ;
				const math::random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class RangeAccelerationGenerator */
		} /* end namespace Generators */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_ACCELERATION_GENERATOR_H__ */