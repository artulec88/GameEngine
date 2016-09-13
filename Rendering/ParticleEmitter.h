#ifndef __RENDERING_PARTICLE_EMITTER_H__
#define __RENDERING_PARTICLE_EMITTER_H__

#include "Rendering.h"
#include "ParticleContainer.h"
#include "ParticlePropertyGenerator.h"

#include "Math\Math.h"

#include <memory>

namespace Rendering
{
	/// <summary>
	/// Particles emitter.
	/// </summary>
	class ParticleEmitter
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit ParticleEmitter(Math::Real emitRate);
		~ParticleEmitter();

		/// <summary> Particle emitter copy constructor. </summary>
		ParticleEmitter(const ParticleEmitter& particleEmitter) = default;
		/// <summary> Particle emitter move constructor. </summary>
		ParticleEmitter(ParticleEmitter&& particleEmitter) = default;
		/// <summary> Particle emitter copy assignment operator. </summary>
		ParticleEmitter& operator=(const ParticleEmitter& particleEmitter) = delete;
		/// <summary> Particle emitter move assignment operator. </summary>
		ParticleEmitter& operator=(ParticleEmitter&& particleEmitter) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		void Emit(Math::Real deltaTime, ParticleContainer* particleContainer);
		void AddGenerator(ParticlePropertyGenerator* particlePropertyGenerator) { m_generators.push_back(particlePropertyGenerator); }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Math::Real m_emitRate;
		std::vector<ParticlePropertyGenerator*> m_generators;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticleContainer */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_EMITTER_H__ */