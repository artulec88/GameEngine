#ifndef __RENDERING_PARTICLES_KILLER_H__
#define __RENDERING_PARTICLES_KILLER_H__

#include "Rendering.h"

#include "Math/Math.h"
#include "Math/RandomGenerator.h"

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
			/// Simulates the killer by the specified <paramref name="deltaTime"/> time.
			/// The function simply iterates through the particles in the specified particles container <paramref name="particlesContainer"/>
			/// and kill the ones that are no longer alive.
			/// </summary>
			/// <param name="deltaTime"> The time that has passed since the last killing procedure took place. </param>
			/// <param name="particleContainer">
			/// The container of particles that will be cleansed of the particles that are assumed to be dead.
			/// </param>
			RENDERING_API virtual void Kill(math::Real deltaTime, ParticlesContainer* particleContainer) = 0;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesKiller */

		/// <summary>
		/// Life span particles killer. This kind of particles killer decides to kill the particles based on their life span attribute.
		/// </summary>
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
			RENDERING_API void Kill(math::Real deltaTime, ParticlesContainer* particleContainer) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class LifeSpanParticlesKiller */

		/// <summary>
		/// Timer particles killer. This kind of particles killer performs the killing procedure with specified frequency.
		/// In other words the timer particles killer is given the information about the number of particles to be killed every second
		/// (or the time between two separate particles being killed).
		/// Whenever the time limit is reached the particles killer picks random particles from the given particles container and kills them.
		/// </summary>
		class TimerParticlesKiller : public ParticlesKiller
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// The constructor of the particles killer based on the particles life span.
			/// </summary>
			/// <param name="particlesToKillPerSecond">
			/// The number of particles to be killed every passing second.
			/// </param>
			RENDERING_API TimerParticlesKiller(math::Real particlesToKillPerSecond);

			/// <summary> Timer particles killer destructor. </summary>
			RENDERING_API virtual ~TimerParticlesKiller();

			/// <summary> Timer particles killer copy constructor. </summary>
			/// <param name="timerParticlesKiller"> The reference to timer particles killer to copy-construct from. </param>
			TimerParticlesKiller(const TimerParticlesKiller& timerParticlesKiller) = default;

			/// <summary> Timer particles killer move constructor. </summary>
			/// <param name="timerParticlesKiller"> The r-value reference to timer particles killer to move-construct from. </param>
			TimerParticlesKiller(TimerParticlesKiller&& timerParticlesKiller) = default;

			/// <summary> Timer particles killer copy assignment operator. </summary>
			/// <param name="timerParticlesKiller"> The reference to timer particles killer to copy-assign from. </param>
			/// <returns> The reference to the newly copy-assigned timer particles killer. </returns>
			TimerParticlesKiller& operator=(const TimerParticlesKiller& timerParticlesKiller) = delete;

			/// <summary> Timer particles killer move assignment operator. </summary>
			/// <param name="timerParticlesKiller"> The r-value reference to timer particles killer to move-assign from. </param>
			/// <returns> The reference to the newly move-assigned timer particles killer. </returns>
			TimerParticlesKiller& operator=(TimerParticlesKiller&& timerParticlesKiller) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			RENDERING_API void Kill(math::Real deltaTime, ParticlesContainer* particleContainer) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			math::Real m_timeToKillOneParticle;
			math::Real m_currentTimer;
			const math::random::RandomGenerator& m_randomGenerator;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class TimerParticlesKiller */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLES_KILLER_H__ */