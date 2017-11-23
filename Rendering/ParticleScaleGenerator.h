#ifndef __RENDERING_PARTICLE_SCALE_GENERATOR_H__
#define __RENDERING_PARTICLE_SCALE_GENERATOR_H__

#include "Rendering.h"
#include "ParticleAttributeGenerator.h"

#include "Math/RandomGenerator.h"
#include "Math/AABB.h"

namespace rendering
{
	namespace particles
	{
		namespace generators
		{
			namespace scale_generator_types
			{
				enum ScaleGeneratorType
				{
					CONSTANT = 0,
					RANGE
				}; /* end enum ScaleGeneratorType */
			} /* end namespace scale_generator_types */

		/// <summary>
		/// Generates scale for the particle.
		/// </summary>
			class ScaleGenerator : public ParticleAttributeGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				ScaleGenerator() :
					ParticleAttributeGenerator(attributes::SCALE)
				{
				}
				virtual ~ScaleGenerator()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				void Set(ParticlesContainer* particleContainer, size_t i, math::Real scale)
				{
					particleContainer->SetScale(i, scale);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ScaleGenerator */

			/// <summary>
			/// Generates scale and sets it in the particle.
			/// </summary>
			class ConstantScaleGenerator : public ScaleGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API explicit ConstantScaleGenerator(math::Real scale);
				RENDERING_API virtual ~ConstantScaleGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Real m_scale;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ConstantScaleGenerator */

			/// <summary>
			/// Generates random scale from a given range and sets it in the particle.
			/// </summary>
			class RangeScaleGenerator : public ScaleGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API RangeScaleGenerator(math::Real minScale, math::Real maxScale);
				RENDERING_API virtual ~RangeScaleGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Real m_minScale;
				math::Real m_maxScale;
				const math::random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class RangeScaleGenerator */
		} /* end namespace generators */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_SCALE_GENERATOR_H__ */