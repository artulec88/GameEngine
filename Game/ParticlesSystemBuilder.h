#ifndef __GAME_PARTICLE_SYSTEM_BUILDER_H__
#define __GAME_PARTICLE_SYSTEM_BUILDER_H__

#include "Def.h"
#include "ParticleEffects.h"
#include "TextureIDs.h"

#include "Engine\GameManager.h"

#include "Rendering\ParticlesSystem.h"

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
		ParticlesSystemBuilder(Engine::GameManager* gameManager, ParticleEffects::ParticleEffect particleEffect);
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
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Engine::GameManager* m_gameManager;
		ParticleEffects::ParticleEffect m_particleEffect;
		std::string m_configurationSuffix;
		TextureIDs::TextureID m_textureID;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticlesSystemBuilder */
} /* end namespace Game */

#endif /* __GAME_PARTICLE_SYSTEM_BUILDER_H__ */