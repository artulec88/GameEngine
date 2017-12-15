#ifndef __RENDERING_PARTICLE_SYSTEM_BUILDER_H__
#define __RENDERING_PARTICLE_SYSTEM_BUILDER_H__

#include "Rendering.h"
//#include "ParticleEffects.h"
#include "ParticlesEmitter.h"
//#include "TextureIDs.h"
#include "ParticleAttributeUpdater.h"
#include "ParticlesKiller.h"

#include "Utility/Builder.h"

#include <string>

namespace rendering
{
	namespace particles
	{
		class ParticleAttributeUpdater;
		class ParticlesSystem;

		/// <summary>
		/// The particles system builder.
		/// </summary>
		class ParticlesSystemBuilder : public utility::Builder<ParticlesSystem>
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Particles system builder constructor. </summary>
			RENDERING_API ParticlesSystemBuilder();

			/// <summary> Particles system builder destructor. </summary>
			RENDERING_API virtual ~ParticlesSystemBuilder();

			/// <summary> Particles system builder copy constructor. </summary>
			ParticlesSystemBuilder(const ParticlesSystemBuilder& particlesSystemBuilder) = delete;

			/// <summary> Particles system builder move constructor. </summary>
			ParticlesSystemBuilder(ParticlesSystemBuilder&& particlesSystemBuilder) = delete;

			/// <summary> Particles system builder copy assignment operator. </summary>
			ParticlesSystemBuilder& operator=(const ParticlesSystemBuilder& particlesSystemBuilder) = delete;

			/// <summary> Particles system builder move assignment operator. </summary>
			ParticlesSystemBuilder& operator=(ParticlesSystemBuilder&& particlesSystemBuilder) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			ParticlesSystem Build() override;

			void SetDefault() override;

			/// <summary> Sets the maximum number of particles that final particles system will be able to maintain. </summary>
			/// <param name="maxCount"> The maximum number of particles that final particles system will be able to maintain. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetMaxCount(size_t maxCount)
			{
				m_maxCount = maxCount;
				return *this;
			}

			/// <summary> Sets the attributes mask that the final particles system will maintain. </summary>
			/// <param name="attributeIds"> The mask of attributes that particles system will maintain. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetAttributesMask(int attributeIds)
			{
				m_attributesMask = attributes::AttributesMask(attributeIds);
				return *this;
			}

			/// <summary> Sets the attributes mask that the final particles system will maintain. </summary>
			/// <param name="attributesMask"> The mask of attributes that particles system will maintain. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetAttributesMask(attributes::AttributesMask attributesMask)
			{
				m_attributesMask = attributesMask;
				return *this;
			}
			
			/// <summary>
			/// Sets the ID of the mesh that the final particles system will use.
			/// </summary>
			/// <param name="meshId"> The ID of the mesh to be used by the final particles system. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetMeshId(int meshId)
			{
				m_meshId = meshId;
				return *this;
			}

			/// <summary>
			/// Sets the ID of the texture that the final particles system will use.
			/// One must remember that texture ID may not be used if it stays in contradiction with the attributes mask given to the particles system.
			/// For example, the particles system will just use <code>texture_ids::INVALID</code> as the texture ID if the attributes mask declares that particles do not have texture offsets property.
			/// </summary>
			/// <param name="textureId"> The ID of the texture to be used by the final particles system. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetTextureId(int textureId)
			{
				m_textureId = textureId;
				return *this;
			}

			RENDERING_API ParticlesSystemBuilder& SetTextureAtlasRowsCount(int textureAtlasRowsCount)
			{
				m_textureAtlasRowsCount = textureAtlasRowsCount;
				return *this;
			}

			RENDERING_API ParticlesSystemBuilder& SetIsAdditive(bool isAdditive)
			{
				m_isAdditive = isAdditive;
				return *this;
			}

			/// <summary>
			/// Sets the ID of the shader that the final particles system will use.
			/// One must remember that in fact different shader ID may be used if the chosen one stays in contradiction with the attributes mask given to the particles system.
			/// </summary>
			/// <param name="shaderId"> The ID of the shader to be used by the final particles system. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetShaderId(int shaderId)
			{
				m_shaderId = shaderId;
				return *this;
			}

			/// <summary>
			/// Sets the uniform color that the final particles system will use.
			/// The value will only be useful when the shader used by the particles system will define the uniform <code>R_particleColor</color>.
			/// </summary>
			/// <param name="color"> The uniform color to be used by the final particles system. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetUniformColor(const Color& color)
			{
				m_uniformColor = color;
				return *this;
			}

			/// <summary>
			/// Adds next particles emitter to the particles system.
			/// </summary>
			/// <param name="particlesEmitter"> The particles emitter to be used by the particles system. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& AddEmitter(const ParticlesEmitter& particlesEmitter);

			/// <summary>
			/// Adds next particles updater to the particles system.
			/// </summary>
			/// <param name="particlesUpdater"> The particles updater to be used by the particles system. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& AddUpdater(std::shared_ptr<updaters::ParticleAttributeUpdater> particlesUpdater);

			/// <summary>
			/// Sets particles killer to be used by the final particles system.
			/// </summary>
			/// <param name="particlesKiller"> The particles killer to be used by the particles system. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetKiller(ParticlesKiller* particlesKiller);

			RENDERING_API size_t GetMaxCount() const
			{
				return m_maxCount;
			}

			RENDERING_API attributes::AttributesMask GetAttributesMask() const
			{
				return m_attributesMask;
			}

			RENDERING_API int GetMeshId() const
			{
				return m_meshId;
			}

			RENDERING_API int GetTextureId() const
			{
				return m_textureId;
			}

			RENDERING_API int GetTextureAtlasRowsCount() const
			{
				return m_textureAtlasRowsCount;
			}

			RENDERING_API bool IsAdditive() const
			{
				return m_isAdditive;
			}

			RENDERING_API int GetShaderId() const
			{
				return m_shaderId;
			}

			RENDERING_API const Color& GetUniformColor() const
			{
				return m_uniformColor;
			}

			RENDERING_API const std::vector<ParticlesEmitter>& GetEmitters() const
			{
				return m_emitters;
			}

			RENDERING_API const std::vector<std::shared_ptr<updaters::ParticleAttributeUpdater>>& GetUpdaters() const
			{
				return m_updaters;
			}

			RENDERING_API ParticlesKiller* GetKiller() const
			{
				return m_particlesKiller;
			}
		private:
			void AddPositionGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddVelocityGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddAccelerationGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddGravityEffectFactorGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddLifeSpanGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddRotationGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddScaleGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddTextureOffsetGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddColorGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddMassGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddAliveGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddIdGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			const size_t m_defaultMaxParticlesCount; // TODO: Make it static constexpr in the future.
			const attributes::AttributesMask m_defaultAttributesMask; // TODO: Make it a static constexpr in the future.

			//ParticlesSystem m_particlesSystem;

			/// <summary> The maximum number of particles the system may store. </summary>
			size_t m_maxCount;
			
			/// <summary>
			/// The mask representing which attributes each particle in the final system will have.
			/// The mask of attributes defines which shader will be used by the final particles system.
			/// </summary>
			attributes::AttributesMask m_attributesMask;

			/// <summary> The ID of the mesh final particles system will use. </summary>
			int m_meshId;

			/// <summary> The ID of the texture final particles system will use. </summary>
			int m_textureId;

			/// <summary> The integer representing the number of rows (and columns) in the texture atlas. </summary>
			/// <remarks>
			/// The value is only useful when the particles system uses some texture in the first place,
			/// meaning that <code>m_textureId</code> is not equal to <code>texture_ids::INVALID</code>.
			/// </remarks>
			int m_textureAtlasRowsCount;

			/// <summary>
			/// For some particles we want additive blending (e.g. magic effects) and for others we want GL_ONE_MINUS_SRC_ALPHA (e.g. smoke).
			/// </summary>
			bool m_isAdditive;
			
			/// <summary> The ID of the shader final particles system will use. </summary>
			int m_shaderId;

			/// <summary>
			/// The uniform color to be used by the particles system.
			/// Only useful when the shader used by the particles system defines the uniform <code>R_particleColor</code>.
			/// </summary>
			Color m_uniformColor;

			/// <summary> The emitters to be used by the final particles system. </summary>
			std::vector<ParticlesEmitter> m_emitters;

			//const ParticleTexture* m_particleTexture;
			//Particleeffects::ParticleEffect m_particleEffect;
			//std::string m_configurationSuffix;
			//unsigned int m_particleEmittersCount;
			//unsigned int m_particleUpdatersCount;
			
			std::vector<std::shared_ptr<updaters::ParticleAttributeUpdater>> m_updaters;

			ParticlesKiller* m_particlesKiller;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesSystemBuilder */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_SYSTEM_BUILDER_H__ */