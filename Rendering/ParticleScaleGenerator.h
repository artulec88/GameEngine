#ifndef __RENDERING_PARTICLE_SCALE_GENERATOR_H__
#define __RENDERING_PARTICLE_SCALE_GENERATOR_H__

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
				ParticleAttributeGenerator(Attributes::SCALE)
			{
			}
			virtual ~ScaleGenerator()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			inline void Set(ParticlesContainer* particleContainer, size_t i, Math::Real scale)
			{
				particleContainer->SetScale(i, scale);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
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
			RENDERING_API ConstantScaleGenerator(Math::Real scale);
			RENDERING_API virtual ~ConstantScaleGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Real m_scale;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ConstantScaleGenerator */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_SCALE_GENERATOR_H__ */