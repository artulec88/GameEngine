#ifndef __RENDERING_PARTICLES_SYSTEM_H__
#define __RENDERING_PARTICLES_SYSTEM_H__

#include "Rendering.h"
#include "ParticlesContainer.h"
#include "ParticlesEmitter.h"
#include "ParticlesUpdater.h"

#include <vector>

namespace Rendering
{
	namespace Particles
	{
		/// <summary>
		/// Particles system. It contains a particles container, list of emitters and particles updaters.
		/// The particles system initializes, cleans up and provides the update procedure.
		/// It is a duty of <code>ParticlesUpdater</code>'s to update and kill particles.
		/// </summary>
		class ParticlesSystem
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Particles system default constructor. </summary>
			RENDERING_API ParticlesSystem();

			/// <summary> Particles system constructor. </summary>
			/// <param name="maxCount"> The maximum number of particles the system may store. </param>
			/// <param name="attributesMask">
			/// The mask representing which attributes each particle in the system will have.
			/// The mask of attributes defines which shader will be used by the particles system.
			/// </param>
			/// <param name="particlesTextureID"> The ID of the texture the particles system will use. </param>
			/// <param name=""> The ID of the shader the particles system will use. </param>
			RENDERING_API ParticlesSystem(size_t maxCount, Attributes::AttributesMask attributesMask, int particlesTextureID, int particlesShaderID);

			/// <summary> Particles system destructor. </summary>
			RENDERING_API ~ParticlesSystem();

			/// <summary> Particles system copy constructor. </summary>
			ParticlesSystem(const ParticlesSystem& particlesSystem) = default;

			/// <summary> Particles system move constructor. </summary>
			RENDERING_API ParticlesSystem(ParticlesSystem&& particlesSystem) = default;

			/// <summary> Particles system copy assignment operator. </summary>
			ParticlesSystem& operator=(const ParticlesSystem& particlesSystem) = default;

			/// <summary> Particles system move assignment operator. </summary>
			RENDERING_API ParticlesSystem& operator=(ParticlesSystem&& particlesSystem) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Updates the particles system. The simulation moves forward in time by the specified <paramref name="deltaTime"/> value.
			/// </summary>
			/// <param name="deltaTime"> The amount of time passed since the last update. </param>
			RENDERING_API void Update(math::Real deltaTime);
			RENDERING_API void Reset();
			RENDERING_API int GetTextureID() const { return m_textureID; }
			RENDERING_API int GetShaderID() const { return m_shaderID; }
			RENDERING_API size_t GetParticlesCount() const { return m_particles.GetMaxCount(); }
			RENDERING_API size_t GetAliveParticlesCount() const { return m_particles.GetAliveCount(); }
			RENDERING_API void AddEmitter(const ParticlesEmitter& emitter) { m_emitters.push_back(emitter); }
			RENDERING_API void AddUpdater(ParticlesUpdater* updater) { m_updaters.push_back(updater); }
			RENDERING_API const math::Vector3D& GetPosition(size_t i) const { return m_particles.GetPosition(i); }
			RENDERING_API const math::Angle& GetRotation(size_t i) const { return m_particles.GetRotation(i); }
			RENDERING_API math::Real GetScale(size_t i) const { return m_particles.GetScale(i); }
			RENDERING_API math::Real CalculateLifeStageFactor(size_t i) const { return m_particles.CalculateLifeStageFactor(i); }
			
			/// <summary> Determines whether the specified <paramref name="attribute"/> is maintained by the particles in the system. </summary>
			/// <param name="attribute"> The property that we want to know if it is being maintained by each particle. </param>
			/// <returns>
			/// <code>true</code> if the specified <paramref name="attribute"/> is maintained by the particles in the system and
			/// <code>false</code> otherwise.
			/// </returns>
			RENDERING_API bool IsAttributeEnabled(Attributes::Attribute attribute) const
			{
				return m_particles.IsAttributeEnabled(attribute);
			}

			//RENDERING_API void SetMaxParticlesCount(size_t maxCount)
			//{
			//	m_count = maxCount;
			//	m_particles.SetMaxParticlesCount(m_count);
			//}
			//RENDERING_API void SetAttributesMask(Attributes::AttributesMask attributesMask)
			//{
			//	m_particles.SetAttributesMask(attributesMask);
			//	// TODO: Change shaderID.
			//}
			//RENDERING_API void SetParticleTexture(const ParticleTexture* particleTexture) { m_texture = particleTexture; }

			friend std::ostream& operator<<(std::ostream& out, const ParticlesSystem& particlesSystem)
			{
				out << "Particles system: count: " << particlesSystem.m_maxCount << std::endl << particlesSystem.m_particles <<
					std::endl << "Emitters count: " << particlesSystem.m_emitters.size() << std::endl << "Updaters count: " <<
					particlesSystem.m_updaters.size() << std::endl << "TextureID: " << particlesSystem.m_textureID << "; shaderID: " << particlesSystem.m_shaderID;
				return out;
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/// <summary>
			/// The maximum number of particles the system may store.
			/// </summary>
			size_t m_maxCount;

			/// <summary>
			/// The particles container.
			/// </summary>
			ParticlesContainer m_particles;

			/// <summary>
			/// The vector of particles emitters.
			/// </summary>
			std::vector<ParticlesEmitter> m_emitters;

			/// <summary>
			/// The vector of particles updaters.
			/// </summary>
			std::vector<ParticlesUpdater*> m_updaters;

			/// <summary>
			/// The ID of the particle texture. May be <code>TextureIDs::INVALID</code> if no texture is used in the system.
			/// </summary>
			int m_textureID;

			/// <summary>
			/// The ID of the shader the particles system will use.
			/// Based on the attributes the particles have the shader's ID should be chosen appropriately.
			/// </summary>
			int m_shaderID;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesSystem */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLES_SYSTEM_H__ */