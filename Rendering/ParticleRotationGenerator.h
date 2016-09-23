#ifndef __RENDERING_PARTICLE_ROTATION_GENERATOR_H__
#define __RENDERING_PARTICLE_ROTATION_GENERATOR_H__

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
		namespace Generators
		{
			namespace RotationGeneratorTypes
			{
				enum RotationGeneratorType
				{
					CONSTANT = 0,
					RANGE
				}; /* end enum RotationGeneratorType */
			} /* end namespace RotationGeneratorTypes */

			/// <summary>
			/// Generates rotation for the particle.
			/// </summary>
			class RotationGenerator : public ParticleAttributeGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RotationGenerator() :
					ParticleAttributeGenerator(Attributes::ROTATION)
				{
				}
				virtual ~RotationGenerator()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				inline void Set(ParticlesContainer* particleContainer, size_t i, const Math::Angle& rotationAngle)
				{
					particleContainer->SetRotation(i, rotationAngle);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				/* ==================== Non-static member variables end ==================== */
			}; /* end class RotationGenerator */

			/// <summary>
			/// Generates constant rotation and sets it in the particle.
			/// </summary>
			class ConstantRotationGenerator : public RotationGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API ConstantRotationGenerator(const Math::Angle& angle);
				RENDERING_API virtual ~ConstantRotationGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				Math::Angle m_angle;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ConstantRotationGenerator */

			/// <summary>
			/// Generates random rotation from a given range of angles and sets it in the particle.
			/// </summary>
			class RangeRotationGenerator : public RotationGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API RangeRotationGenerator(const Math::Angle& minAngle, const Math::Angle& maxAngle);
				RENDERING_API virtual ~RangeRotationGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				Math::Real m_minAngleInRadians;
				Math::Real m_maxAngleInRadians;
				const Math::Random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class RangeRotationGenerator */
		} /* end namespace Generators */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_ROTATION_GENERATOR_H__ */