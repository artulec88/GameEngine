#ifndef __RENDERING_PARTICLE_SYSTEM_BUILDER_H__
#define __RENDERING_PARTICLE_SYSTEM_BUILDER_H__

#include "Rendering.h"
#include "ParticleEffects.h"
#include "ParticlesSystem.h"
#include "ParticlesEmitter.h"
#include "TextureIDs.h"

#include "Utility\Builder.h"

#include <string>

namespace Rendering
{
	namespace Particles
	{
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
			RENDERING_API virtual ~ParticlesSystemBuilder(void);

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
		public:
			virtual ParticlesSystem Get() override
			{
				return ParticlesSystem(m_maxCount, m_attributesMask, m_textureID, m_shaderID);
			}
			virtual void Build() override;
			
			/// <summary> Sets the maximum number of particles that final particles system will be able to maintain. </summary>
			/// <param name="maxCount"> The maximum number of particles that final particles system will be able to maintain. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetMaxCount(size_t maxCount)
			{
				m_maxCount = maxCount;
				return *this;
			}

			/// <summary> Sets the maximum number of particles that final particles system will be able to maintain. </summary>
			/// <param name="maxCount"> The maximum number of particles that final particles system will be able to maintain. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetAttributesMask(Attributes::AttributesMask attributesMask)
			{
				m_attributesMask = attributesMask;
				return *this;
			}

			/// <summary>
			/// Sets the ID of the texture that the final particles system will use.
			/// One must remember that texture ID may not be used if it stays in contradiction with the attributes mask given to the particles system.
			/// For example, the particles system will just use <code>TextureIDs::INVALID</code> as the texture ID if the attributes mask declares that particles do not have texture offsets property.
			/// </summary>
			/// <param name="textureID"> The ID of the texture to be used by the final particles system. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetTextureID(int textureID)
			{
				m_textureID = textureID;
				return *this;
			}

			/// <summary>
			/// Sets the ID of the shader that the final particles system will use.
			/// One must remember that in fact different shader ID may be used if the chosen one stays in contradiction with the attributes mask given to the particles system.
			/// </summary>
			/// <param name="textureID"> The ID of the texture to be used by the final particles system. </param>
			/// <returns> The reference to the <code>this</code> object which allows the client to chain methods invocations easily. </returns>
			RENDERING_API ParticlesSystemBuilder& SetShaderID(int shaderID)
			{
				m_shaderID = shaderID;
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
			//RENDERING_API ParticlesSystemBuilder& AddUpdater(const ParticlesUpdater& particlesUpdater);
		private:
			void AddPositionGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddVelocityGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddAccelerationGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddGravityEffectFactorGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddLifeSpanGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddRotationGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddScaleGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddTextureOffsetGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddColorGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddMassGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddAliveGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			void AddIdGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			const size_t M_DEFAULT_MAX_PARTICLES_COUNT; // TODO: Make it static constexpr in the future.
			const Attributes::AttributesMask M_DEFAULT_ATTRIBUTES_MASK; // TODO: Make it a static constexpr in the future.

			//ParticlesSystem m_particlesSystem;

			size_t m_maxCount;
			Attributes::AttributesMask m_attributesMask;
			int m_textureID;
			int m_shaderID;

			std::vector<Rendering::Particles::ParticlesEmitter> m_emitters;

			//const ParticleTexture* m_particleTexture;
			//Particleeffects::ParticleEffect m_particleEffect;
			//std::string m_configurationSuffix;
			//Rendering::Particles::Attributes::AttributesMask m_attributesMask;
			//unsigned int m_particleEmittersCount;
			//unsigned int m_particleUpdatersCount;
			//std::vector<Rendering::Particles::ParticlesUpdater*> m_updaters;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesSystemBuilder */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_SYSTEM_BUILDER_H__ */