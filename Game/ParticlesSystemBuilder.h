#ifndef __GAME_PARTICLE_SYSTEM_BUILDER_H__
#define __GAME_PARTICLE_SYSTEM_BUILDER_H__

#include "Def.h"
#include "ParticleEffects.h"
#include "TextureIDs.h"

#include "Engine\GameManager.h"

#include "Rendering\ParticlesSystem.h"
#include "Rendering\ParticlesEmitter.h"

#include "Utility\Builder.h"

#include <string>

namespace Game
{
	class ParticlesSystemBuilder : public Utility::Builder<Rendering::Particles::ParticlesSystem>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ParticlesSystemBuilder(Engine::GameManager* gameManager, Rendering::Particles::ParticlesSystem* particlesSystem, ParticleEffects::ParticleEffect particleEffect);
		virtual ~ParticlesSystemBuilder(void);
		ParticlesSystemBuilder(ParticlesSystemBuilder& particlesSystemBuilder) = delete;
		ParticlesSystemBuilder(ParticlesSystemBuilder&& particlesSystemBuilder) = delete;
		ParticlesSystemBuilder& operator=(const ParticlesSystemBuilder& particlesSystemBuilder) = delete;
		ParticlesSystemBuilder& operator=(ParticlesSystemBuilder&& particlesSystemBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
		virtual void BuildPart3();
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
		Engine::GameManager* m_gameManager;
		const Rendering::Particles::ParticleTexture* m_particleTexture;
		ParticleEffects::ParticleEffect m_particleEffect;
		std::string m_configurationSuffix;
		TextureIDs::TextureID m_textureID;
		Rendering::Particles::Attributes::AttributesMask m_attributesMask;
		unsigned int m_particleEmittersCount;
		std::vector<Rendering::Particles::ParticlesEmitter> m_emitters;
		unsigned int m_particleUpdatersCount;
		std::vector<Rendering::Particles::ParticlesUpdater*> m_updaters;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticlesSystemBuilder */
} /* end namespace Game */

#endif /* __GAME_PARTICLE_SYSTEM_BUILDER_H__ */