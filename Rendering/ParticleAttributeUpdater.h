#ifndef __RENDERING_PARTICLES_UPDATER_H__
#define __RENDERING_PARTICLES_UPDATER_H__

#include "Rendering.h"
#include "ParticlesContainer.h"

#include "Math/Effect.h"

namespace rendering
{
	namespace particles
	{
		namespace updaters
		{
			/// <summary>
			/// Particles updater. It follows the Single-responsibility-principle. It is used only to update particle's parameters
			/// and finally decide if the particle should be killed or not. Ideally, an updater should focus only on one set of attributes.
			/// </summary>
			/// <remarks>
			/// See http://www.bfilipek.com/2014/06/flexible-particle-system-updaters.html for details.
			/// </remarks>
			class ParticleAttributeUpdater
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				/// <summary> Particles attribute updater constructor. </summary>
				/// <param name="attributeIds">
				/// The integer representing the mask of attributes to be modified by the updater to be created.
				/// </param>
				RENDERING_API ParticleAttributeUpdater(int attributeIds);

				/// <summary> Particle attribute updater constructor. </summary>
				/// <param name="attributesMask">
				/// The mask of attributes to be modified by the updater to be created.
				/// </param>
				RENDERING_API ParticleAttributeUpdater(const attributes::AttributesMask& attributesMask);

				/// <summary> Destructor. </summary>
				RENDERING_API virtual ~ParticleAttributeUpdater();

				/// <summary> Particle updater copy constructor. </summary>
				/// <param name="particleAttributeUpdater"> The particles updater to copy-construct from. </param>
				ParticleAttributeUpdater(const ParticleAttributeUpdater& particleAttributeUpdater) = delete;

				/// <summary> Particle updater move constructor. </summary>
				/// <param name="particleAttributeUpdater"> The particles updater to move-construct from. </param>
				ParticleAttributeUpdater(ParticleAttributeUpdater&& particleAttributeUpdater) = default;

				/// <summary> Particle updater copy assignment operator. </summary>
				/// <param name="particleAttributeUpdater"> The particles updater to copy-assign from. </param>
				/// <returns> The reference to the newly copy-assigned particles updater. </returns>
				ParticleAttributeUpdater& operator=(const ParticleAttributeUpdater& particleAttributeUpdater) = delete;

				/// <summary> Particle updater move assignment operator. </summary>
				/// <param name="particleAttributeUpdater"> The particles updater to move-assign from. </param>
				/// <returns> The reference to the newly move-assigned particles updater. </returns>
				ParticleAttributeUpdater& operator=(ParticleAttributeUpdater&& particleAttributeUpdater) = default;
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				/// <summary>
				/// Updates the particles states by <paramref name="deltaTime"/>.
				/// </summary>
				/// <param name="deltaTime"> The elapsed time since last particles update. </param>
				/// <param name="particlesContainer"> The container of particles. </param>
				RENDERING_API virtual void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const = 0;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				attributes::AttributesMask m_attributesMask;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ParticleAttributeUpdater */

			//class RotationParticlesUpdater : public ParticleAttributeUpdater
			//{
			//	/* ==================== Static variables and functions begin ==================== */
			//	/* ==================== Static variables and functions end ==================== */

			//	/* ==================== Constructors and destructors begin ==================== */
			//public:
			//	RENDERING_API explicit RotationParticlesUpdater(const math::Angle& rotationSpeedAngle);
			//	RENDERING_API virtual ~RotationParticlesUpdater();
			//	/* ==================== Constructors and destructors end ==================== */

			//	/* ==================== Non-static member functions begin ==================== */
			//	RENDERING_API void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const override;
			//	/* ==================== Non-static member functions end ==================== */

			//	/* ==================== Non-static member variables begin ==================== */
			//private:
			//	math::Angle m_rotationSpeedAngle;
			//	/* ==================== Non-static member variables end ==================== */
			//}; /* end class RotationParticlesUpdater */

			//class ScaleEffectParticlesUpdater : public ParticleAttributeUpdater
			//{
			//	/* ==================== Static variables and functions begin ==================== */
			//	/* ==================== Static variables and functions end ==================== */

			//	/* ==================== Constructors and destructors begin ==================== */
			//public:
			//	RENDERING_API explicit ScaleEffectParticlesUpdater(math::effects::Effect<math::Real>* effect);
			//	RENDERING_API virtual ~ScaleEffectParticlesUpdater();
			//	/* ==================== Constructors and destructors end ==================== */

			//	/* ==================== Non-static member functions begin ==================== */
			//	RENDERING_API void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const override;
			//	/* ==================== Non-static member functions end ==================== */

			//	/* ==================== Non-static member variables begin ==================== */
			//private:
			//	math::effects::Effect<math::Real>* m_effect;
			//	/* ==================== Non-static member variables end ==================== */
			//}; /* end class ScaleEffectParticlesUpdater */
		} /* end namespace updaters */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLES_UPDATER_H__ */