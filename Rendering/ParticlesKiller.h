#ifndef __RENDERING_PARTICLES_KILLER_H__
#define __RENDERING_PARTICLES_KILLER_H__

#include "Rendering.h"

#include "Math/Math.h"

namespace rendering
{
	namespace particles
	{
		class ParticlesContainer;

		/// <summary>
		/// Particles killer. A particle killer is responsible for determining whether given particle is alive and,
		/// if that's the case, for killing those that are dead.
		/// </summary>
		class ParticlesKiller
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Particles killer constructor.
			/// </summary>
			RENDERING_API ParticlesKiller();

			/// <summary> Particles killer destructor. </summary>
			RENDERING_API virtual ~ParticlesKiller();

			/// <summary> Particles killer copy constructor. </summary>
			/// <param name="particlesKiller"> The reference to particles killer to copy-construct from. </param>
			ParticlesKiller(const ParticlesKiller& particlesKiller) = default;

			/// <summary> Particles killer move constructor. </summary>
			/// <param name="particlesKiller"> The r-value reference to particles killer to move-construct from. </param>
			ParticlesKiller(ParticlesKiller&& particlesKiller) = default;

			/// <summary> Particles killer copy assignment operator. </summary>
			/// <param name="particlesKiller"> The reference to particles killer to copy-assign from. </param>
			/// <returns> The reference to the newly copy-assigned particles killer. </returns>
			ParticlesKiller& operator=(const ParticlesKiller& particlesKiller) = delete;

			/// <summary> Particles killer move assignment operator. </summary>
			/// <param name="particlesKiller"> The r-value reference to particles killer to move-assign from. </param>
			/// <returns> The reference to the newly move-assigned particles killer. </returns>
			ParticlesKiller& operator=(ParticlesKiller&& particlesKiller) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// The function checks the particle under given index <paramref name="i"/> in the specified
			/// particles container <paramref name="particlesContainer"/> and returns the simple <code>bool</code>
			/// flag indicating whether it is alive or dead.
			/// </summary>
			/// <param name="particlesContainer"> The container of particles. </param>
			/// <param name="i"> The index of the specific particle in the given particles container. </param>
			/// <returns>
			/// <code>True</code> when the particle <paramref name="i"/> in the <paramref name="particlesContainer"/> is alive
			/// and <code>false</code> otherwise.
			/// </returns>
			RENDERING_API virtual bool IsAlive(ParticlesContainer* particlesContainer, size_t i) const = 0;

			/// <summary>
			/// Simulates the killer by the specified <paramref name="deltaTime"/> time.
			/// The function simply iterates through the particles in the specified particles container <paramref name="particlesContainer"/>
			/// and kill the ones that are no longer alive, i.e. the function <code>IsAlive(/*...*/)</code> returns <code>false</code>.
			/// </summary>
			/// <param name="deltaTime"> The time that has passed since the last killing procedure took place. </param>
			/// <param name="particleContainer"> The container of particles that will be cleansed of dead particles. </param>
			RENDERING_API void Kill(math::Real deltaTime, ParticlesContainer* particleContainer) const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesKiller */

		class LifeSpanParticlesKiller : public ParticlesKiller
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// The constructor of the particles killer based on the particles life span.
			/// </summary>
			RENDERING_API LifeSpanParticlesKiller();

			/// <summary> Life-span particles killer destructor. </summary>
			RENDERING_API virtual ~LifeSpanParticlesKiller();

			/// <summary> Life-span particles killer copy constructor. </summary>
			/// <param name="lifeSpanParticlesKiller"> The reference to life span particles killer to copy-construct from. </param>
			LifeSpanParticlesKiller(const LifeSpanParticlesKiller& lifeSpanParticlesKiller) = default;

			/// <summary> Life-span particles killer move constructor. </summary>
			/// <param name="lifeSpanParticlesKiller"> The r-value reference to life-span particles killer to move-construct from. </param>
			LifeSpanParticlesKiller(LifeSpanParticlesKiller&& lifeSpanParticlesKiller) = default;

			/// <summary> Life-span particles killer copy assignment operator. </summary>
			/// <param name="lifeSpanParticlesKiller"> The reference to life-span particles killer to copy-assign from. </param>
			/// <returns> The reference to the newly copy-assigned life-span particles killer. </returns>
			LifeSpanParticlesKiller& operator=(const LifeSpanParticlesKiller& lifeSpanParticlesKiller) = delete;

			/// <summary> Life-span particles killer move assignment operator. </summary>
			/// <param name="lifeSpanParticlesKiller"> The r-value reference to life-span particles killer to move-assign from. </param>
			/// <returns> The reference to the newly move-assigned life-span particles killer. </returns>
			LifeSpanParticlesKiller& operator=(LifeSpanParticlesKiller&& lifeSpanParticlesKiller) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			RENDERING_API inline bool IsAlive(ParticlesContainer* particlesContainer, size_t i) const override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class LifeSpanParticlesKiller */

		//class TimerParticlesKiller : public ParticlesKiller
		//{
		//	/* ==================== Static variables and functions begin ==================== */
		//	/* ==================== Static variables and functions end ==================== */

		//	/* ==================== Constructors and destructors begin ==================== */
		//public:
		//	/// <summary>
		//	/// The constructor of the particles killer based on the particles life span.
		//	/// </summary>
		//	RENDERING_API TimerParticlesKiller();

		//	/// <summary> Life-span particles killer destructor. </summary>
		//	RENDERING_API virtual ~TimerParticlesKiller();

		//	/// <summary> Timer particles killer copy constructor. </summary>
		//	/// <param name="timerParticlesKiller"> The reference to timer particles killer to copy-construct from. </param>
		//	TimerParticlesKiller(const TimerParticlesKiller& timerParticlesKiller) = default;

		//	/// <summary> Timer particles killer move constructor. </summary>
		//	/// <param name="timerParticlesKiller"> The r-value reference to timer particles killer to move-construct from. </param>
		//	TimerParticlesKiller(TimerParticlesKiller&& timerParticlesKiller) = default;

		//	/// <summary> Timer particles killer copy assignment operator. </summary>
		//	/// <param name="timerParticlesKiller"> The reference to timer particles killer to copy-assign from. </param>
		//	/// <returns> The reference to the newly copy-assigned timer particles killer. </returns>
		//	TimerParticlesKiller& operator=(const TimerParticlesKiller& timerParticlesKiller) = delete;

		//	/// <summary> Timer particles killer move assignment operator. </summary>
		//	/// <param name="timerParticlesKiller"> The r-value reference to timer particles killer to move-assign from. </param>
		//	/// <returns> The reference to the newly move-assigned timer particles killer. </returns>
		//	TimerParticlesKiller& operator=(TimerParticlesKiller&& timerParticlesKiller) = default;
		//	/* ==================== Constructors and destructors end ==================== */

		//	/* ==================== Non-static member functions begin ==================== */
		//	RENDERING_API bool IsAlive(ParticlesContainer* particlesContainer, size_t i) const override
		//	{
		//		// TODO: Improve the implementation.
		//		return false;
		//	}
		//	/* ==================== Non-static member functions end ==================== */

		//	/* ==================== Non-static member variables begin ==================== */
		//	/* ==================== Non-static member variables end ==================== */
		//}; /* end class TimerParticlesKiller */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLES_KILLER_H__ */