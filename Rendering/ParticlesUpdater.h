#ifndef __RENDERING_PARTICLES_UPDATER_H__
#define __RENDERING_PARTICLES_UPDATER_H__

#include "Rendering.h"
#include "ParticlesContainer.h"

#include "Math/Vector.h"
#include "Math/Effect.h"

namespace rendering
{
	namespace particles
	{
		/// <summary>
		/// Particles updater. It follows the Single-responsibility-principle. It is used only to update particle's parameters
		/// and finally decide if the particle should be killed or not. Ideally, an updater should focus only on one set of attributes.
		/// </summary>
		/// <remarks>
		/// See http://www.bfilipek.com/2014/06/flexible-particle-system-updaters.html for details.
		/// </remarks>
		class ParticlesUpdater
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Default constructor. </summary>
			RENDERING_API ParticlesUpdater();

			/// <summary> Destructor. </summary>
			RENDERING_API virtual ~ParticlesUpdater();

			/// <summary> Particle updater copy constructor. </summary>
			/// <param name="particlesUpdater"> The particles updater to copy-construct from. </param>
			ParticlesUpdater(const ParticlesUpdater& particlesUpdater) = delete;

			/// <summary> Particle updater move constructor. </summary>
			/// <param name="particlesUpdater"> The particles updater to move-construct from. </param>
			ParticlesUpdater(ParticlesUpdater&& particlesUpdater) = default;

			/// <summary> Particle updater copy assignment operator. </summary>
			/// <param name="particlesUpdater"> The particles updater to copy-assign from. </param>
			/// <returns> The reference to the newly copy-assigned particles updater. </returns>
			ParticlesUpdater& operator=(const ParticlesUpdater& particlesUpdater) = delete;

			/// <summary> Particle updater move assignment operator. </summary>
			/// <param name="particlesUpdater"> The particles updater to move-assign from. </param>
			/// <returns> The reference to the newly move-assigned particles updater. </returns>
			ParticlesUpdater& operator=(ParticlesUpdater&& particlesUpdater) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Updates the particles states by <paramref name="deltaTime"/>.
			/// </summary>
			/// <param name="deltaTime"> The elapsed time since last particles update. </param>
			/// <param name="particlesContainer"> The container of particles. </param>
			RENDERING_API virtual void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) = 0;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesUpdater */

		class EulerParticlesUpdater : public ParticlesUpdater
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API explicit EulerParticlesUpdater(const math::Vector3D& acceleration);
			RENDERING_API virtual ~EulerParticlesUpdater();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			RENDERING_API void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			math::Vector3D m_acceleration;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class EulerParticlesUpdater */

		class LifeSpanParticlesUpdater : public ParticlesUpdater
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API LifeSpanParticlesUpdater();
			RENDERING_API virtual ~LifeSpanParticlesUpdater();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			RENDERING_API void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class LifeSpanParticlesUpdater */

		class RotationParticlesUpdater : public ParticlesUpdater
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API explicit RotationParticlesUpdater(const math::Angle& rotationSpeedAngle);
			RENDERING_API virtual ~RotationParticlesUpdater();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			RENDERING_API void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			math::Angle m_rotationSpeedAngle;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class RotationParticlesUpdater */

		class ScaleEffectParticlesUpdater : public ParticlesUpdater
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API explicit ScaleEffectParticlesUpdater(math::effects::Effect<math::Real>* effect);
			RENDERING_API virtual ~ScaleEffectParticlesUpdater();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			RENDERING_API void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			math::effects::Effect<math::Real>* m_effect;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ScaleEffectParticlesUpdater */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLES_UPDATER_H__ */