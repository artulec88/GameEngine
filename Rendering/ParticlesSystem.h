#ifndef __RENDERING_PARTICLES_SYSTEM_H__
#define __RENDERING_PARTICLES_SYSTEM_H__

#include "Rendering.h"
#include "ParticlesSystemBuilder.h"
#include "ParticlesContainer.h"
#include "ParticlesEmitter.h"
#include "ParticleAttributeUpdater.h"

#include <vector>
#include "ParticlesKiller.h"

namespace rendering
{
	class Camera;

	namespace particles
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
			/// <param name="particlesSystemBuilder">
			/// The particles system builder holding all the necessary information to create a fully-functional particles system.
			/// </param>
			RENDERING_API ParticlesSystem(const ParticlesSystemBuilder& particlesSystemBuilder);

			/// <summary> Particles system destructor. </summary>
			RENDERING_API ~ParticlesSystem();

			/// <summary> Particles system copy constructor. </summary>
			/// <param name="particlesSystem"> The reference to particles system to copy construct from. </param>
			ParticlesSystem(const ParticlesSystem& particlesSystem) = default;

			/// <summary> Particles system move constructor. </summary>
			/// <param name="particlesSystem"> The r-value reference to particles system to move construct from. </param>
			RENDERING_API ParticlesSystem(ParticlesSystem&& particlesSystem) = default;

			/// <summary> Particles system copy assignment operator. </summary>
			/// <param name="particlesSystem"> The reference to particles system to copy assign from. </param>
			/// <returns> The reference to newly copy-assigned particles system. </returns>
			ParticlesSystem& operator=(const ParticlesSystem& particlesSystem) = default;

			/// <summary> Particles system move assignment operator. </summary>
			/// <param name="particlesSystem"> The r-value reference to particles system to move assign from. </param>
			/// <returns> The reference to newly move-assigned particles system. </returns>
			RENDERING_API ParticlesSystem& operator=(ParticlesSystem&& particlesSystem) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Updates the particles system. The simulation moves forward in time by the specified <paramref name="deltaTime"/> value.
			/// </summary>
			/// <param name="deltaTime"> The amount of time passed since the last update. </param>
			RENDERING_API void Update(math::Real deltaTime);
			RENDERING_API void Reset();
			RENDERING_API int GetMeshId() const { return m_meshId; }
			RENDERING_API int GetTextureId() const { return m_textureId; }
			RENDERING_API int GetShaderId() const { return m_shaderId; }
			RENDERING_API size_t GetParticlesCount() const { return m_particles.GetMaxCount(); }
			RENDERING_API size_t GetAliveParticlesCount() const { return m_particles.GetAliveCount(); }
			RENDERING_API void AddEmitter(const ParticlesEmitter& emitter) { m_emitters.push_back(emitter); }
			RENDERING_API void AddUpdater(std::shared_ptr<updaters::ParticleAttributeUpdater> updater) { m_updaters.push_back(updater); }
			RENDERING_API void SetKiller(ParticlesKiller* killer) { m_killer = killer; }
			//RENDERING_API const math::Vector3D& GetPosition(size_t i) const { return m_particles.GetPosition(i); }
			//RENDERING_API const math::Angle& GetRotation(size_t i) const { return m_particles.GetRotation(i); }
			//RENDERING_API math::Real GetScale(size_t i) const { return m_particles.GetScale(i); }
			//RENDERING_API math::Real CalculateLifeStageFactor(size_t i) const { return m_particles.CalculateLifeStageFactor(i); }
			
			RENDERING_API int GetTextureAtlasRowsCount() const { return m_textureAtlasRowsCount; }

			RENDERING_API bool IsAdditive() const { return m_isAdditive; }

			/// <summary> Determines whether the specified <paramref name="attribute"/> is maintained by the particles in the system. </summary>
			/// <param name="attribute"> The property that we want to know if it is being maintained by each particle. </param>
			/// <returns>
			/// <code>true</code> if the specified <paramref name="attribute"/> is maintained by the particles in the system and
			/// <code>false</code> otherwise.
			/// </returns>
			RENDERING_API bool IsAttributeEnabled(attributes::Attribute attribute) const
			{
				return m_particles.IsAttributeEnabled(attribute);
			}
			
			RENDERING_API std::vector<math::Real> GetInstanceData(const Camera* camera, std::vector<math::Real>& particleInstanceVboData) const;

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
					particlesSystem.m_updaters.size() << std::endl << "TextureID: " << particlesSystem.m_textureId << "; shaderId: " << particlesSystem.m_shaderId;
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
			std::vector<std::shared_ptr<updaters::ParticleAttributeUpdater>> m_updaters;

			/// <summary> The killer of particles in the system. </summary>
			ParticlesKiller* m_killer;

			/// <summary>
			/// The ID of the particle mesh.
			/// </summary>
			int m_meshId;

			/// <summary>
			/// The ID of the particle texture. May be <code>texture_ids::INVALID</code> if no texture is used in the system.
			/// </summary>
			int m_textureId;

			/// <summary>
			/// The number of rows (and columns too) in the texture atlas.
			/// It is only useful when <code>m_textureId</code> is not equal to <code>texture_ids::INVALID</code>.
			/// </summary>
			int m_textureAtlasRowsCount;

			/// <summary>
			/// For some particles we want additive blending (e.g. magic effects) and for others we want GL_ONE_MINUS_SRC_ALPHA (e.g. smoke).
			/// It is only useful when <code>m_textureId</code> is not equal to <code>texture_ids::INVALID</code>.
			/// </summary>
			bool m_isAdditive;

			/// <summary>
			/// The ID of the shader the particles system will use.
			/// Based on the attributes the particles have the shader's ID should be chosen appropriately.
			/// </summary>
			int m_shaderId;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesSystem */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLES_SYSTEM_H__ */