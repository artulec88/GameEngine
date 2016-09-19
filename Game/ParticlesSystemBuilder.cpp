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
	m_textureID(TextureIDs::PARTICLE_WATER)
{
	switch (m_particleEffect)
	{
	case ParticleEffects::RAIN:
		m_configurationSuffix = "_Rain";
		m_textureID = TextureIDs::PARTICLE_WATER;
		break;
	case ParticleEffects::FIRE:
		m_configurationSuffix = "_Fire";
		m_textureID = TextureIDs::PARTICLE_FIRE;
		break;
	case ParticleEffects::FOUNTAIN:
		m_configurationSuffix = "_Fountain";
		m_textureID = TextureIDs::PARTICLE_WATER;
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
	const Rendering::Particles::ParticleTexture* particleTexture = m_gameManager->AddParticleTexture(m_textureID,
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
		const std::string iStr = std::to_string(i + 1);
		Rendering::Particles::ParticlesEmitter emitter(GET_CONFIG_VALUE_GAME("particleEmitterGeneratedParticlesPerSecond" + m_configurationSuffix + "_" + iStr, 400.0f));
		//if (attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::POSITION))
		//{
		//}
		//emitter.AddGenerator(new Rendering::Particles::BasicIdGenerator());

		/* ==================== Position generator begin ==================== */
		Rendering::Particles::Generators::PositionGeneratorTypes::PositionGeneratorType positionGeneratorType = static_cast<Rendering::Particles::Generators::PositionGeneratorTypes::PositionGeneratorType>(
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorType" + m_configurationSuffix + "_" + iStr, static_cast<int>(Rendering::Particles::Generators::PositionGeneratorTypes::CONSTANT)));
		switch (positionGeneratorType)
		{
		case Rendering::Particles::Generators::PositionGeneratorTypes::CONSTANT:
			emitter.AddGenerator(new Rendering::Particles::Generators::ConstantPositionGenerator(
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPosX" + m_configurationSuffix + "_" + iStr, 50.0f),
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPosY" + m_configurationSuffix + "_" + iStr, -1.0f),
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPosZ" + m_configurationSuffix + "_" + iStr, 60.0f)));
			break;
		case Rendering::Particles::Generators::PositionGeneratorTypes::BOX:
			emitter.AddGenerator(new Rendering::Particles::Generators::BoxPositionGenerator(
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMinX" + m_configurationSuffix + "_" + iStr, 40.0f),
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMaxX" + m_configurationSuffix + "_" + iStr, 60.0f),
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMinY" + m_configurationSuffix + "_" + iStr, -2.0f),
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMaxY" + m_configurationSuffix + "_" + iStr, 2.0f),
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMinZ" + m_configurationSuffix + "_" + iStr, 50.0f),
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMaxZ" + m_configurationSuffix + "_" + iStr, 40.0f)));
			break;
		case Rendering::Particles::Generators::PositionGeneratorTypes::PLANE:
			emitter.AddGenerator(new Rendering::Particles::Generators::PlanePositionGenerator(
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPlaneNormalX" + m_configurationSuffix + "_" + iStr, 40.0f),
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPlaneNormalY" + m_configurationSuffix + "_" + iStr, 40.0f),
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPlaneNormalZ" + m_configurationSuffix + "_" + iStr, 40.0f),
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPlaneDistance" + m_configurationSuffix + "_" + iStr, 40.0f),
				GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPlaneRadius" + m_configurationSuffix + "_" + iStr, 40.0f)));
			break;
		case Rendering::Particles::Generators::PositionGeneratorTypes::ELLIPSOID:
			WARNING_LOG_GAME("Not yet supported");
			break;
		default:
			ERROR_LOG_GAME("Unknown particle position generator type: %d", positionGeneratorType);
		}
		/* ==================== Position generator end ==================== */

		emitter.AddGenerator(new Rendering::Particles::BasicVelocityGenerator(-3.3f, 3.3f, 8.0f, 17.0f, -3.3f, 3.3f));
		emitter.AddGenerator(new Rendering::Particles::ConstantAccelerationGenerator(0.0f, -10.0f, 0.0f));
		emitter.AddGenerator(new Rendering::Particles::BasicLifeSpanLimitGenerator(3.0f, 4.5f));
		//emitter.AddGenerator(new Rendering::Particles::RandomRotationGenerator(Math::Angle(0.0f), Math::Angle(360.0f)));
		emitter.AddGenerator(new Rendering::Particles::ConstantScaleGenerator(0.1f));
		m_object->AddEmitter(emitter);
	}
}

void Game::ParticlesSystemBuilder::BuildPart3()
{
	Rendering::Particles::ParticlesUpdater* eulerUpdater = new Rendering::Particles::EulerParticlesUpdater(Math::Vector3D(0.0f, 0.0f, 0.0f));
	Rendering::Particles::ParticlesUpdater* lifeSpanUpdater = new Rendering::Particles::LifeSpanParticlesUpdater();
	//Rendering::Particles::ParticlesUpdater* rotationUpdater = new Rendering::Particles::RotationParticlesUpdater(Math::Angle(90.0f, Math::Unit::DEGREE));
	m_object->AddUpdater(eulerUpdater);
	m_object->AddUpdater(lifeSpanUpdater);
	//m_object->AddUpdater(rotationUpdater);
}