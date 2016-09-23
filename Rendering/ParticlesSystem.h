#ifndef __RENDERING_PARTICLES_SYSTEM_H__
#define __RENDERING_PARTICLES_SYSTEM_H__

#include "Rendering.h"
#include "ParticlesContainer.h"
#include "ParticlesEmitter.h"
#include "ParticlesUpdater.h"
#include "Texture.h"

#include <vector>

namespace Rendering
{
	namespace Particles
	{
		/// <summary>
		/// Particles system. It contains a particles container, list of emitters and particles updaters.
		/// The particles system initializes, cleans up and manages the update procedure.
		/// It is a duty of <code>ParticlesUpdater</code>'s to update and kill particles.
		/// </summary>
		class ParticlesSystem
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API ParticlesSystem();
			RENDERING_API ParticlesSystem(size_t maxCount, Attributes::AttributesMask attributesMask, const ParticleTexture* particleTexture);
			RENDERING_API ~ParticlesSystem();

			/// <summary> Particles system copy constructor. </summary>
			ParticlesSystem(const ParticlesSystem& particlesSystem) = delete;
			/// <summary> Particles system move constructor. </summary>
			RENDERING_API ParticlesSystem(ParticlesSystem&& particlesSystem) = default;
			/// <summary> Particles system copy assignment operator. </summary>
			ParticlesSystem& operator=(const ParticlesSystem& particlesSystem) = delete;
			/// <summary> Particles system move assignment operator. </summary>
			RENDERING_API ParticlesSystem& operator=(ParticlesSystem&& particlesSystem) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API void Update(Math::Real deltaTime);
			RENDERING_API void Reset();
			RENDERING_API const ParticleTexture* GetTexture() const { return m_texture; }
			RENDERING_API size_t GetParticlesCount() const { return m_particles.GetCount(); }
			RENDERING_API size_t GetAliveParticlesCount() const { return m_particles.GetAliveCount(); }
			RENDERING_API void AddEmitter(const ParticlesEmitter& emitter) { m_emitters.push_back(emitter); }
			RENDERING_API void AddUpdater(ParticlesUpdater* updater) { m_updaters.push_back(updater); }
			RENDERING_API const Math::Vector3D& GetPosition(size_t i) const { return m_particles.GetPosition(i); }
			RENDERING_API const Math::Angle& GetRotation(size_t i) const { return m_particles.GetRotation(i); }
			RENDERING_API Math::Real GetScale(size_t i) const { return m_particles.GetScale(i); }
			RENDERING_API Math::Real CalculateLifeStageFactor(size_t i) const { return m_particles.CalculateLifeStageFactor(i); }
			RENDERING_API bool IsAttributeEnabled(Attributes::Attribute attribute) const { return m_particles.IsAttributeEnabled(attribute); }
			RENDERING_API void SetMaxParticlesCount(size_t maxCount)
			{
				m_count = maxCount;
				m_particles.SetMaxParticlesCount(m_count);
			}
			RENDERING_API void SetAttributesMask(Attributes::AttributesMask attributesMask)
			{
				m_particles.SetAttributesMask(attributesMask);
			}
			RENDERING_API void SetParticleTexture(const ParticleTexture* particleTexture) { m_texture = particleTexture; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			size_t m_count;
			ParticlesContainer m_particles;
			std::vector<ParticlesEmitter> m_emitters;
			std::vector<ParticlesUpdater*> m_updaters;
			const ParticleTexture* m_texture;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesSystem */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLES_SYSTEM_H__ */