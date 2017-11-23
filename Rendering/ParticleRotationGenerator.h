#ifndef __RENDERING_PARTICLE_ROTATION_GENERATOR_H__
#define __RENDERING_PARTICLE_ROTATION_GENERATOR_H__

#include "Rendering.h"
#include "ParticleAttributeGenerator.h"

#include "Math\RandomGenerator.h"
#include "Math/AABB.h"
#include "Math\Ellipsoid.h"
#include "Math/Plane.h"

namespace rendering
{
	namespace particles
	{
		namespace generators
		{
			namespace rotation_generator_types
			{
				enum RotationGeneratorType
				{
					CONSTANT = 0,
					RANGE
				}; /* end enum RotationGeneratorType */
			} /* end namespace rotation_generator_types */

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
					ParticleAttributeGenerator(attributes::ROTATION)
				{
				}
				virtual ~RotationGenerator()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				void Set(ParticlesContainer* particleContainer, size_t i, const math::Angle& rotationAngle)
				{
					particleContainer->SetRotation(i, rotationAngle);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
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
				RENDERING_API explicit ConstantRotationGenerator(const math::Angle& angle);
				RENDERING_API virtual ~ConstantRotationGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Angle m_angle;
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
				RENDERING_API RangeRotationGenerator(const math::Angle& minAngle, const math::Angle& maxAngle);
				RENDERING_API virtual ~RangeRotationGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Real m_minAngleInRadians;
				math::Real m_maxAngleInRadians;
				const math::random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class RangeRotationGenerator */
		} /* end namespace generators */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_ROTATION_GENERATOR_H__ */