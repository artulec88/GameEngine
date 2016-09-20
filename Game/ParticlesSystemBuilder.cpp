#include "ParticlesSystemBuilder.h"

#include "Rendering\ParticlePositionGenerator.h"
#include "Rendering\ParticleVelocityGenerator.h"
#include "Rendering\ParticleAccelerationGenerator.h"
#include "Rendering\ParticleRotationGenerator.h"
#include "Rendering\ParticleScaleGenerator.h"
#include "Rendering\ParticleLifeSpanGenerator.h"
#include "Rendering\ParticlesUpdater.h"

#include "Utility\IConfig.h"

Game::ParticlesSystemBuilder::ParticlesSystemBuilder(Engine::GameManager* gameManager, ParticleEffects::ParticleEffect particleEffect) :
	m_gameManager(gameManager),
	m_particleEffect(particleEffect),
	m_configurationSuffix(""),
	m_textureID(TextureIDs::PARTICLE_WATER),
	m_attributesMask(0)
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

	if (GET_CONFIG_VALUE_GAME("particleAttributePositionEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::POSITION;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeVelocityEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::VELOCITY;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeAccelerationEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::ACCELERATION;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeGravityEffectFactorEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::GRAVITY_EFFECT_FACTOR;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeLifeSpanEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::LIFE_SPAN;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeRotationEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::ROTATION;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeScaleEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::SCALE;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeTextureOffsetEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::TEXTURE_OFFSET;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeColorEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::COLOR;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeMassEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::MASS;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeAliveEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::ALIVE;
	}
	if (GET_CONFIG_VALUE_GAME("particleAttributeIdEnabled" + m_configurationSuffix, true))
	{
		m_attributesMask |= Rendering::Particles::Attributes::ID;
	}
	DEBUG_LOG_GAME("Attributes mask = ", m_attributesMask.m_attributesMask);

	m_object = new Rendering::Particles::ParticlesSystem(GET_CONFIG_VALUE_GAME("particleMaxParticlesCount" + m_configurationSuffix, 5000),
		m_attributesMask, *particleTexture);
}

void Game::ParticlesSystemBuilder::BuildPart2()
{
	const unsigned int particleEmittersCount = GET_CONFIG_VALUE_GAME("particleEmittersCount" + m_configurationSuffix, 1);
	for (unsigned int i = 0; i < particleEmittersCount; ++i)
	{
		const std::string iStr = std::to_string(i + 1);
		Rendering::Particles::ParticlesEmitter emitter(GET_CONFIG_VALUE_GAME("particleEmitterGeneratedParticlesPerSecond" + m_configurationSuffix + "_" + iStr, 400.0f));

		AddPositionGenerator(&emitter, iStr);
		AddVelocityGenerator(&emitter, iStr);
		AddAccelerationGenerator(&emitter, iStr);
		AddGravityEffectFactorGenerator(&emitter, iStr);
		AddLifeSpanGenerator(&emitter, iStr);
		AddRotationGenerator(&emitter, iStr);
		AddScaleGenerator(&emitter, iStr);
		AddTextureOffsetGenerator(&emitter, iStr);
		AddColorGenerator(&emitter, iStr);
		AddMassGenerator(&emitter, iStr);
		AddAliveGenerator(&emitter, iStr);
		AddIdGenerator(&emitter, iStr);

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

void Game::ParticlesSystemBuilder::AddPositionGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::POSITION), Utility::Logging::DEBUG,
		"The position attribute is disabled for the current particles system");

	Rendering::Particles::Generators::PositionGeneratorTypes::PositionGeneratorType positionGeneratorType = static_cast<Rendering::Particles::Generators::PositionGeneratorTypes::PositionGeneratorType>(
		GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(Rendering::Particles::Generators::PositionGeneratorTypes::CONSTANT)));
	switch (positionGeneratorType)
	{
	case Rendering::Particles::Generators::PositionGeneratorTypes::CONSTANT:
		emitter->AddGenerator(new Rendering::Particles::Generators::ConstantPositionGenerator(
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPosX" + m_configurationSuffix + "_" + indexStr, 50.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPosY" + m_configurationSuffix + "_" + indexStr, -1.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPosZ" + m_configurationSuffix + "_" + indexStr, 60.0f)));
		break;
	case Rendering::Particles::Generators::PositionGeneratorTypes::BOX:
		emitter->AddGenerator(new Rendering::Particles::Generators::BoxPositionGenerator(
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMinX" + m_configurationSuffix + "_" + indexStr, 40.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMaxX" + m_configurationSuffix + "_" + indexStr, 60.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMinY" + m_configurationSuffix + "_" + indexStr, -2.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMaxY" + m_configurationSuffix + "_" + indexStr, 2.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMinZ" + m_configurationSuffix + "_" + indexStr, 50.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorMaxZ" + m_configurationSuffix + "_" + indexStr, 40.0f)));
		break;
	case Rendering::Particles::Generators::PositionGeneratorTypes::PLANE:
		emitter->AddGenerator(new Rendering::Particles::Generators::PlanePositionGenerator(
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPlaneNormalX" + m_configurationSuffix + "_" + indexStr, 40.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPlaneNormalY" + m_configurationSuffix + "_" + indexStr, 40.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPlaneNormalZ" + m_configurationSuffix + "_" + indexStr, 40.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPlaneDistance" + m_configurationSuffix + "_" + indexStr, 40.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterPositionGeneratorPlaneRadius" + m_configurationSuffix + "_" + indexStr, 40.0f)));
		break;
	case Rendering::Particles::Generators::PositionGeneratorTypes::ELLIPSOID:
		WARNING_LOG_GAME("Not yet supported");
		break;
	default:
		ERROR_LOG_GAME("Unknown particle position generator type: %d", positionGeneratorType);
	}
}

void Game::ParticlesSystemBuilder::AddVelocityGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::VELOCITY), Utility::Logging::DEBUG,
		"The velocity attribute is disabled for the current particles system");

	Rendering::Particles::Generators::VelocityGeneratorTypes::VelocityGeneratorType velocityGeneratorType = static_cast<Rendering::Particles::Generators::VelocityGeneratorTypes::VelocityGeneratorType>(
		GET_CONFIG_VALUE_GAME("particleEmitterVelocityGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(Rendering::Particles::Generators::VelocityGeneratorTypes::CONSTANT)));
	switch (velocityGeneratorType)
	{
	case Rendering::Particles::Generators::VelocityGeneratorTypes::CONSTANT:
		emitter->AddGenerator(new Rendering::Particles::Generators::ConstantVelocityGenerator(
			GET_CONFIG_VALUE_GAME("particleEmitterVelocityGeneratorX" + m_configurationSuffix + "_" + indexStr, 50.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterVelocityGeneratorY" + m_configurationSuffix + "_" + indexStr, -1.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterVelocityGeneratorZ" + m_configurationSuffix + "_" + indexStr, 60.0f)));
		break;
	case Rendering::Particles::Generators::VelocityGeneratorTypes::RANGE:
		emitter->AddGenerator(new Rendering::Particles::Generators::RangeVelocityGenerator(
			GET_CONFIG_VALUE_GAME("particleEmitterVelocityGeneratorMinX" + m_configurationSuffix + "_" + indexStr, -3.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterVelocityGeneratorMaxX" + m_configurationSuffix + "_" + indexStr, 3.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterVelocityGeneratorMinY" + m_configurationSuffix + "_" + indexStr, 5.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterVelocityGeneratorMaxY" + m_configurationSuffix + "_" + indexStr, 10.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterVelocityGeneratorMinZ" + m_configurationSuffix + "_" + indexStr, -3.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterVelocityGeneratorMaxZ" + m_configurationSuffix + "_" + indexStr, 3.0f)));
		break;
	default:
		ERROR_LOG_GAME("Unknown particle velocity generator type: %d", velocityGeneratorType);
	}
}

void Game::ParticlesSystemBuilder::AddAccelerationGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::ACCELERATION), Utility::Logging::DEBUG,
		"The acceleration attribute is disabled for the current particles system");

	Rendering::Particles::Generators::AccelerationGeneratorTypes::AccelerationGeneratorType accelerationGeneratorType = static_cast<Rendering::Particles::Generators::AccelerationGeneratorTypes::AccelerationGeneratorType>(
		GET_CONFIG_VALUE_GAME("particleEmitterAccelerationGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(Rendering::Particles::Generators::AccelerationGeneratorTypes::CONSTANT)));
	switch (accelerationGeneratorType)
	{
	case Rendering::Particles::Generators::AccelerationGeneratorTypes::CONSTANT:
		emitter->AddGenerator(new Rendering::Particles::Generators::ConstantAccelerationGenerator(
			GET_CONFIG_VALUE_GAME("particleEmitterAccelerationGeneratorX" + m_configurationSuffix + "_" + indexStr, 0.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterAccelerationGeneratorY" + m_configurationSuffix + "_" + indexStr, -5.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterAccelerationGeneratorZ" + m_configurationSuffix + "_" + indexStr, 0.0f)));
		break;
	case Rendering::Particles::Generators::AccelerationGeneratorTypes::RANGE:
		emitter->AddGenerator(new Rendering::Particles::Generators::RangeAccelerationGenerator(
			GET_CONFIG_VALUE_GAME("particleEmitterAccelerationGeneratorMinX" + m_configurationSuffix + "_" + indexStr, -1.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterAccelerationGeneratorMaxX" + m_configurationSuffix + "_" + indexStr, 1.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterAccelerationGeneratorMinY" + m_configurationSuffix + "_" + indexStr, -8.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterAccelerationGeneratorMaxY" + m_configurationSuffix + "_" + indexStr, -12.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterAccelerationGeneratorMinZ" + m_configurationSuffix + "_" + indexStr, -1.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterAccelerationGeneratorMaxZ" + m_configurationSuffix + "_" + indexStr, 1.0f)));
		break;
	default:
		ERROR_LOG_GAME("Unknown particle acceleration generator type: %d", accelerationGeneratorType);
	}
}

void Game::ParticlesSystemBuilder::AddGravityEffectFactorGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::GRAVITY_EFFECT_FACTOR), Utility::Logging::DEBUG,
		"The gravity effect factor attribute is disabled for the current particles system");

	// TODO: Implement this function
}

void Game::ParticlesSystemBuilder::AddLifeSpanGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::LIFE_SPAN), Utility::Logging::DEBUG,
		"The life span attribute is disabled for the current particles system");

	Rendering::Particles::Generators::LifeSpanGeneratorTypes::LifeSpanGeneratorType lifeSpanGeneratorType = static_cast<Rendering::Particles::Generators::LifeSpanGeneratorTypes::LifeSpanGeneratorType>(
		GET_CONFIG_VALUE_GAME("particleEmitterLifeSpanGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(Rendering::Particles::Generators::LifeSpanGeneratorTypes::CONSTANT)));
	switch (lifeSpanGeneratorType)
	{
	case Rendering::Particles::Generators::LifeSpanGeneratorTypes::CONSTANT:
		emitter->AddGenerator(new Rendering::Particles::Generators::ConstantLifeSpanLimitGenerator(
			GET_CONFIG_VALUE_GAME("particleEmitterLifeSpanGenerator" + m_configurationSuffix + "_" + indexStr, 5.0f)));
		break;
	case Rendering::Particles::Generators::LifeSpanGeneratorTypes::RANGE:
		emitter->AddGenerator(new Rendering::Particles::Generators::RangeLifeSpanLimitGenerator(
			GET_CONFIG_VALUE_GAME("particleEmitterLifeSpanGeneratorMin" + m_configurationSuffix + "_" + indexStr, 2.0f),
			GET_CONFIG_VALUE_GAME("particleEmitterLifeSpanGeneratorMax" + m_configurationSuffix + "_" + indexStr, 3.0f)));
		break;
	default:
		ERROR_LOG_GAME("Unknown particle life span limit generator type: %d", lifeSpanGeneratorType);
	}
}

void Game::ParticlesSystemBuilder::AddRotationGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::ROTATION), Utility::Logging::DEBUG,
		"The rotation attribute is disabled for the current particles system");

	Rendering::Particles::Generators::RotationGeneratorTypes::RotationGeneratorType rotationGeneratorType = static_cast<Rendering::Particles::Generators::RotationGeneratorTypes::RotationGeneratorType>(
		GET_CONFIG_VALUE_GAME("particleEmitterRotationGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(Rendering::Particles::Generators::RotationGeneratorTypes::CONSTANT)));
	switch (rotationGeneratorType)
	{
	case Rendering::Particles::Generators::RotationGeneratorTypes::CONSTANT:
		emitter->AddGenerator(new Rendering::Particles::Generators::ConstantRotationGenerator(Math::Angle(
			GET_CONFIG_VALUE_GAME("particleEmitterRotationGenerator" + m_configurationSuffix + "_" + indexStr, 5.0f))
			));
		break;
	case Rendering::Particles::Generators::RotationGeneratorTypes::RANGE:
		emitter->AddGenerator(new Rendering::Particles::Generators::RangeRotationGenerator(
			Math::Angle(GET_CONFIG_VALUE_GAME("particleEmitterRotationGeneratorMin" + m_configurationSuffix + "_" + indexStr, 2.0f)),
			Math::Angle(GET_CONFIG_VALUE_GAME("particleEmitterRotationGeneratorMax" + m_configurationSuffix + "_" + indexStr, 3.0f))));
		break;
	default:
		ERROR_LOG_GAME("Unknown particle rotation generator type: %d", rotationGeneratorType);
	}
}

void Game::ParticlesSystemBuilder::AddScaleGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::SCALE), Utility::Logging::DEBUG,
		"The scale attribute is disabled for the current particles system");

	Rendering::Particles::Generators::ScaleGeneratorTypes::ScaleGeneratorType scaleGeneratorType = static_cast<Rendering::Particles::Generators::ScaleGeneratorTypes::ScaleGeneratorType>(
		GET_CONFIG_VALUE_GAME("particleEmitterScaleGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(Rendering::Particles::Generators::ScaleGeneratorTypes::CONSTANT)));
	switch (scaleGeneratorType)
	{
	case Rendering::Particles::Generators::ScaleGeneratorTypes::CONSTANT:
		emitter->AddGenerator(new Rendering::Particles::Generators::ConstantScaleGenerator(
			GET_CONFIG_VALUE_GAME("particleEmitterScaleGenerator" + m_configurationSuffix + "_" + indexStr, 0.5f)));
		break;
	case Rendering::Particles::Generators::ScaleGeneratorTypes::RANGE:
		emitter->AddGenerator(new Rendering::Particles::Generators::RangeScaleGenerator(
			GET_CONFIG_VALUE_GAME("particleEmitterScaleGeneratorMin" + m_configurationSuffix + "_" + indexStr, 0.1f),
			GET_CONFIG_VALUE_GAME("particleEmitterScaleGeneratorMax" + m_configurationSuffix + "_" + indexStr, 2.0f)));
		break;
	default:
		ERROR_LOG_GAME("Unknown particle scale generator type: %d", scaleGeneratorType);
	}
}

void Game::ParticlesSystemBuilder::AddTextureOffsetGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::TEXTURE_OFFSET), Utility::Logging::DEBUG,
		"The texture offset attribute is disabled for the current particles system");
}

void Game::ParticlesSystemBuilder::AddColorGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::COLOR), Utility::Logging::DEBUG,
		"The color attribute is disabled for the current particles system");
}

void Game::ParticlesSystemBuilder::AddMassGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::MASS), Utility::Logging::DEBUG,
		"The mass attribute is disabled for the current particles system");
}

void Game::ParticlesSystemBuilder::AddAliveGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::ALIVE), Utility::Logging::DEBUG,
		"The alive attribute is disabled for the current particles system");
}

void Game::ParticlesSystemBuilder::AddIdGenerator(Rendering::Particles::ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_attributesMask.IsAttributeEnabled(Rendering::Particles::Attributes::ID), Utility::Logging::DEBUG,
		"The ID attribute is disabled for the current particles system");
}
