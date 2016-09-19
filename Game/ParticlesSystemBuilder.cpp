#include "ParticlesSystemBuilder.h"

#include "Rendering\ParticlePositionGenerator.h"
#include "Rendering\ParticleVelocityGenerator.h"
#include "Rendering\ParticleAccelerationGenerator.h"
#include "Rendering\ParticleRotationGenerator.h"
#include "Rendering\ParticleScaleGenerator.h"
#include "Rendering\ParticleLifeSpanGenerator.h"
#include "Rendering\ParticlesEmitter.h"
#include "Rendering\ParticlesUpdater.h"

#include "Utility\IConfig.h"

Game::ParticlesSystemBuilder::ParticlesSystemBuilder(Engine::GameManager* gameManager, ParticleEffects::ParticleEffect particleEffect) :
	m_gameManager(gameManager),
	m_particleEffect(particleEffect),
	m_configurationSuffix(""),
	m_textureID(TextureIDs::PARTICLE_RAIN)
{
	switch (m_particleEffect)
	{
	case ParticleEffects::RAIN:
		m_configurationSuffix = "_Rain";
		m_textureID = TextureIDs::PARTICLE_RAIN;
		break;
	case ParticleEffects::FIRE:
		m_configurationSuffix = "_Fire";
		m_textureID = TextureIDs::PARTICLE_FIRE;
		break;
	case ParticleEffects::SMOKE:
		m_configurationSuffix = "_Smoke";
		m_textureID = TextureIDs::PARTICLE_SMOKE;
		break;
	//default: // TODO: Implement
	}
}


Game::ParticlesSystemBuilder::~ParticlesSystemBuilder()
{
}

void Game::ParticlesSystemBuilder::BuildPart1()
{
	const Rendering::Particles::ParticleTexture* particleTexture = m_gameManager->AddParticleTexture(TextureIDs::PARTICLE_RAIN,
		GET_CONFIG_VALUE_STR_GAME("particleTexture" + m_configurationSuffix, "particleRain.png"),
		GET_CONFIG_VALUE_GAME("particleTextureRowsCount" + m_configurationSuffix, 4),
		GET_CONFIG_VALUE_GAME("particleTextureIsAdditive" + m_configurationSuffix, true));
	CHECK_CONDITION_GAME(particleTexture != NULL, Utility::Logging::CRITICAL, "Cannot create particle system. The texture is NULL.");

	Rendering::Particles::Attributes::AttributesMask attributesMask{ 0 };
	if (GET_CONFIG_VALUE_GAME("particleAttributePositionEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::POSITION;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeVelocityEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::VELOCITY;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeAccelerationEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::ACCELERATION;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeGravityEffectFactorEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::GRAVITY_EFFECT_FACTOR;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeLifeSpanEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::LIFE_SPAN;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeRotationEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::ROTATION;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeScaleEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::SCALE;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeTextureOffsetEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::TEXTURE_OFFSET;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeColorEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::COLOR;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeMassEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::MASS;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeAliveEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::ALIVE;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeIdEnabled" + m_configurationSuffix, true))
	{
		attributesMask |= Rendering::Particles::Attributes::ID;
	}
	DEBUG_LOG_GAME("Attributes mask = ", attributesMask.m_attributesMask);

	m_object = new Rendering::Particles::ParticlesSystem(GET_CONFIG_VALUE_GAME("particleMaxParticlesCount" + m_configurationSuffix, 5000),
		attributesMask, *particleTexture);
}

void Game::ParticlesSystemBuilder::BuildPart2()
{
	const unsigned int particleEmittersCount = GET_CONFIG_VALUE_GAME("particleEmittersCount" + m_configurationSuffix, 1);
	for (unsigned int i = 0; i < particleEmittersCount; ++i)
	{
		Rendering::Particles::ParticlesEmitter emitter(GET_CONFIG_VALUE_GAME("particleEmitterGeneratedParticlesPerSecond" + m_configurationSuffix + "_" + std::to_string(i + 1), 400));
		//if (attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::POSITION))
		//{
		//}
		//emitter.AddGenerator(new Rendering::Particles::BasicIdGenerator());
		//emitter.AddGenerator(new Rendering::Particles::BoxPositionGenerator(40.0f, 60.0f, 0.0f, 10.0f, 69.0f, 91.0f));
		emitter.AddGenerator(new Rendering::Particles::PositionGenerators::ConstantPositionGenerator(50.0f, -1.0f, 60.0f));
		emitter.AddGenerator(new Rendering::Particles::BasicVelocityGenerator(-3.3f, 3.3f, 8.0f, 17.0f, -3.3f, 3.3f));
		emitter.AddGenerator(new Rendering::Particles::ConstantAccelerationGenerator(0.0f, -10.0f, 0.0f));
		emitter.AddGenerator(new Rendering::Particles::BasicLifeSpanLimitGenerator(3.0f, 4.5f));
		//emitter.AddGenerator(new Rendering::Particles::RandomRotationGenerator(Math::Angle(0.0f), Math::Angle(360.0f)));
		emitter.AddGenerator(new Rendering::Particles::ConstantScaleGenerator(0.1f));
		m_object->AddEmitter(emitter);
	}

	Rendering::Particles::ParticlesUpdater* eulerUpdater = new Rendering::Particles::EulerParticlesUpdater(Math::Vector3D(0.0f, 0.0f, 0.0f));
	Rendering::Particles::ParticlesUpdater* lifeSpanUpdater = new Rendering::Particles::LifeSpanParticlesUpdater();
	//Rendering::Particles::ParticlesUpdater* rotationUpdater = new Rendering::Particles::RotationParticlesUpdater(Math::Angle(90.0f, Math::Unit::DEGREE));
	m_object->AddUpdater(eulerUpdater);
	m_object->AddUpdater(lifeSpanUpdater);
	//m_object->AddUpdater(rotationUpdater);
}