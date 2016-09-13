#ifndef __RENDERING_PARTICLE_SYSTEM_H__
#define __RENDERING_PARTICLE_SYSTEM_H__

#include "Rendering.h"
#include "ParticleContainer.h"
#include "ParticleEmitter.h"
#include "ParticlesUpdater.h"

#include <vector>

namespace Rendering
{
	/// <summary>
	/// Particles system.
	/// </summary>
	class ParticleSystem
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API explicit ParticleSystem(size_t maxCount);
		RENDERING_API ~ParticleSystem();

		/// <summary> Particle container copy constructor. </summary>
		RENDERING_API ParticleSystem(const ParticleSystem& particleSystem) = delete;
		/// <summary> Particle container move constructor. </summary>
		RENDERING_API ParticleSystem(ParticleSystem&& particleSystem) = delete;
		/// <summary> Particle container copy assignment operator. </summary>
		RENDERING_API ParticleSystem& operator=(const ParticleSystem& particleSystem) = delete;
		/// <summary> Particle container move assignment operator. </summary>
		RENDERING_API ParticleSystem& operator=(ParticleSystem&& particleSystem) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API void Update(Math::Real deltaTime);
		RENDERING_API void Reset();
		RENDERING_API size_t GetParticlesCount() const { return m_particles.GetCount(); }
		RENDERING_API size_t GetAliveParticlesCount() const { return m_particles.GetAliveCount(); }
		RENDERING_API void AddEmitter(const ParticleEmitter& emitter) { m_emitters.push_back(emitter); }
		RENDERING_API void AddUpdater(ParticlesUpdater* updater) { m_updaters.push_back(updater); }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		size_t m_count;
		ParticleContainer m_particles;
		std::vector<ParticleEmitter> m_emitters;
		std::vector<ParticlesUpdater*> m_updaters;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticleContainer */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_SYSTEM_H__ */