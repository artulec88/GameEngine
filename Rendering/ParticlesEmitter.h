#ifndef __RENDERING_PARTICLE_EMITTER_H__
#define __RENDERING_PARTICLE_EMITTER_H__

#include "Rendering.h"
#include "ParticlesContainer.h"
#include "ParticleAttributeGenerator.h"

#include "Math/Math.h"

namespace rendering
{
	namespace particles
	{
		/// <summary>
		/// Particles emitter. A particle emitter holds one or more particles property generators.
		/// Each frame, the emitter should emit specific number of particles. The pace of emission
		/// depends on emission rate that is specified when particles emitter is created.
		/// The emitter should generate all needed particle attributes, but each attribute can be
		/// set by a different generator.
		/// </summary>
		class ParticlesEmitter
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Particles emitter constructor.
			/// </summary>
			/// <param name="numberOfParticlesToGeneratePerSecond">
			/// The number of particles to generate per second.
			/// The value does not necessarily have to be an integer.
			/// </param>
			RENDERING_API explicit ParticlesEmitter(math::Real numberOfParticlesToGeneratePerSecond);

			/// <summary> Particles emitter destructor. </summary>
			RENDERING_API ~ParticlesEmitter();

			/// <summary> Particles emitter copy constructor. </summary>
			/// <param name="particlesEmitter"> The particles emitter to copy-construct from. </param>
			ParticlesEmitter(const ParticlesEmitter& particlesEmitter) = default;

			/// <summary> Particles emitter move constructor. </summary>
			/// <param name="particlesEmitter"> The particles emitter to move-construct from. </param>
			ParticlesEmitter(ParticlesEmitter&& particlesEmitter) = default;

			/// <summary> Particles emitter copy assignment operator. </summary>
			/// <param name="particlesEmitter"> The particles emitter to copy-assign from. </param>
			ParticlesEmitter& operator=(const ParticlesEmitter& particlesEmitter) = delete;

			/// <summary> Particles emitter move assignment operator. </summary>
			/// <param name="particlesEmitter"> The particles emitter to move-assign from. </param>
			ParticlesEmitter& operator=(ParticlesEmitter&& particlesEmitter) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Based on the given <paramref name="numberOfParticlesToGeneratePerSecond"/> the time for one particle to be generated is calculated.
			/// </summary>
			/// <param name="numberOfParticlesToGeneratePerSecond">
			/// The number of particles to generate per second.
			/// The value does not necessarily have to be an integer.
			/// </param>
			RENDERING_API void SetParticlesCountToGeneratePerSecond(math::Real numberOfParticlesToGeneratePerSecond)
			{
				m_timeToEmitOneParticle = REAL_ONE / numberOfParticlesToGeneratePerSecond;
			}

			/// <summary>
			/// Simulates the emitter by the specified <paramref name="deltaTime"/> time.
			/// It emits <code>P = floor(deltaTime / m_timeToEmitOneParticle)</code> particles in the given <paramref name="particleContainer"/>.
			/// Emission of a single particle means setting its properties by the particles generators and reviving it.
			/// </summary>
			/// <param name="deltaTime"> The time that has passed since the last emission. </param>
			/// <param name="particleContainer"> The container of particles that will store the emitted particles. </param>
			RENDERING_API void Emit(math::Real deltaTime, ParticlesContainer* particleContainer);

			/// <summary>
			/// Adds additional particle attribute generator to the emitter.
			/// The generator will be used when new particles are emitted by this specific particles emitter.
			/// </summary>
			/// <param name="particleAttributeGenerator"> The specific particle attribute generator. </param>
			RENDERING_API void AddGenerator(generators::ParticleAttributeGenerator* particleAttributeGenerator) { m_generators.push_back(particleAttributeGenerator); }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/// <summary>
			/// Elapsed time since last particles emission in seconds.
			/// </summary>
			math::Real m_currentTimer; // TODO: Replace with timespan object

			/// <summary>
			/// The time in seconds that needs to pass for the one particle to be generated.
			/// </summary>
			math::Real m_timeToEmitOneParticle;

			/// <summary>
			/// The generators that particles emitter uses.
			/// </summary>
			std::vector<generators::ParticleAttributeGenerator*> m_generators;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesEmitter */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_EMITTER_H__ */