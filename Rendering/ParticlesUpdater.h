#ifndef __RENDERING_PARTICLES_UPDATER_H__
#define __RENDERING_PARTICLES_UPDATER_H__

#include "Rendering.h"
#include "ParticlesContainer.h"

#include "Math\Vector.h"

namespace Rendering
{
	namespace Particles
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
			RENDERING_API ParticlesUpdater();
			RENDERING_API virtual ~ParticlesUpdater();

			///// <summary> Particle generator copy constructor. </summary>
			//ParticlePropertyGenerator(const ParticlePropertyGenerator& particlePropertyGenerator) = delete;
			///// <summary> Particle generator move constructor. </summary>
			//ParticlePropertyGenerator(ParticlePropertyGenerator&& particlePropertyGenerator) = delete;
			///// <summary> Particle generator copy assignment operator. </summary>
			//ParticlePropertyGenerator& operator=(const ParticlePropertyGenerator& particlePropertyGenerator) = delete;
			///// <summary> Particle generator move assignment operator. </summary>
			//ParticlePropertyGenerator& operator=(ParticlePropertyGenerator&& particlePropertyGenerator) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Updates the particles states by <paramref name="deltaTime"/>.
			/// </summary>
			/// <param name="deltaTime"> The elapsed time since last particles update. </param>
			/// <param name="particlesContainer"> The container of particles. </param>
			RENDERING_API virtual void Update(Math::Real deltaTime, ParticlesContainer* particlesContainer) = 0;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesUpdater */

		class EulerParticlesUpdater : public ParticlesUpdater
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API EulerParticlesUpdater(const Math::Vector3D& acceleration);
			RENDERING_API virtual ~EulerParticlesUpdater();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Update(Math::Real deltaTime, ParticlesContainer* particlesContainer);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Vector3D m_acceleration;
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
		public:
			RENDERING_API virtual void Update(Math::Real deltaTime, ParticlesContainer* particlesContainer);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class LifeSpanParticlesUpdater */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLES_UPDATER_H__ */