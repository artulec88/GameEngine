#ifndef __RENDERING_PARTICLE_EMITTER_H__
#define __RENDERING_PARTICLE_EMITTER_H__

#include "Rendering.h"
#include "ParticlesContainer.h"
#include "ParticleAttributeGenerator.h"

#include "Math\Math.h"

#include <memory>

namespace Rendering
{
	namespace Particles
	{
		/// <summary>
		/// Particles emitter. A particle emitter holds one or more particles property generators.
		/// Each frame, the emitter should emit a number of particles. The pace of emission
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
			RENDERING_API explicit ParticlesEmitter(Math::Real numberOfParticlesToGeneratePerSecond);
			RENDERING_API ~ParticlesEmitter();

			/// <summary> Particles emitter copy constructor. </summary>
			ParticlesEmitter(const ParticlesEmitter& particlesEmitter) = default;
			/// <summary> Particles emitter move constructor. </summary>
			ParticlesEmitter(ParticlesEmitter&& particlesEmitter) = default;
			/// <summary> Particles emitter copy assignment operator. </summary>
			ParticlesEmitter& operator=(const ParticlesEmitter& particlesEmitter) = delete;
			/// <summary> Particles emitter move assignment operator. </summary>
			ParticlesEmitter& operator=(ParticlesEmitter&& particlesEmitter) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API void Emit(Math::Real deltaTime, ParticlesContainer* particleContainer);
			RENDERING_API void AddGenerator(ParticleAttributeGenerator* particleAttributeGenerator) { m_generators.push_back(particleAttributeGenerator); }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/// <summary>
			/// Elapsed time since last particles emission in seconds.
			/// </summary>
			Math::Real m_currentTimer; // TODO: Replace with timespan object
			/// <summary>
			/// The time in seconds that needs to pass for the one particle to be generated.
			/// </summary>
			Math::Real m_timeToEmitOneParticle;
			std::vector<ParticleAttributeGenerator*> m_generators;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesEmitter */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_EMITTER_H__ */