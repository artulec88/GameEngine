#include "stdafx.h"
#include "ParticlesSystemBuilder.h"
#include "ParticlesSystem.h"
#include "ParticlePositionGenerator.h"
#include "ParticleVelocityGenerator.h"
#include "ParticleAccelerationGenerator.h"
#include "ParticleRotationGenerator.h"
#include "ParticleScaleGenerator.h"
#include "ParticleLifeSpanGenerator.h"
#include "TextureIDs.h"
#include "ShaderIDs.h"

#include "Utility/IConfig.h"

rendering::particles::ParticlesSystemBuilder::ParticlesSystemBuilder() :
	Builder<ParticlesSystem>(),
	m_defaultMaxParticlesCount(GET_CONFIG_VALUE_RENDERING("defaultMaxParticlesCount", 10000)),
	m_defaultAttributesMask(GET_CONFIG_VALUE_RENDERING("defaultParticlesAttributesMask",
		static_cast<int>(attributes::POSITION | attributes::LIFE_SPAN | attributes::COLOR | attributes::SCALE))),
	m_maxCount(m_defaultMaxParticlesCount),
	m_attributesMask(m_defaultAttributesMask),
	m_textureId(texture_ids::INVALID),
	m_shaderId(shader_ids::INVALID),
	m_emitters(),
	m_updaters(),
	m_particlesKiller(nullptr)
	//m_particleTexture(nullptr),
	//m_particleEffect(particleEffect),
	//m_configurationSuffix(""),
	//m_attributesMask(0),
	//m_particleEmittersCount(1),
	//m_particleUpdatersCount(1)
{
	//switch (m_particleEffect)
	//{
	//case Particleeffects::RAIN:
	//	m_configurationSuffix = "_Rain";
	//	m_textureID = TextureIDs::PARTICLE_WATER;
	//	break;
	//case Particleeffects::FIRE:
	//	m_configurationSuffix = "_Fire";
	//	m_textureID = TextureIDs::PARTICLE_FIRE;
	//	break;
	//case Particleeffects::FOUNTAIN:
	//	m_configurationSuffix = "_Fountain";
	//	m_textureID = TextureIDs::PARTICLE_WATER;
	//	break;
	//case Particleeffects::SMOKE:
	//	m_configurationSuffix = "_Smoke";
	//	m_textureID = TextureIDs::PARTICLE_SMOKE;
	//	break;
	//	//default: // TODO: Implement
	//}
}


rendering::particles::ParticlesSystemBuilder::~ParticlesSystemBuilder()
{
}

void rendering::particles::ParticlesSystemBuilder::SetDefault()
{
	m_maxCount = m_defaultMaxParticlesCount;
	m_attributesMask = m_defaultAttributesMask;
	m_textureId = texture_ids::INVALID;
	m_shaderId = shader_ids::INVALID;
	m_emitters.clear();
	m_updaters.clear();
	m_particlesKiller = nullptr;
	//m_particleTexture(nullptr),
	//m_particleEffect(particleEffect),
	//m_configurationSuffix(""),
	//m_attributesMask(0),
	//m_particleEmittersCount(1),
	//m_particleUpdatersCount(1)
}

rendering::particles::ParticlesSystem rendering::particles::ParticlesSystemBuilder::Build()
{
	return ParticlesSystem(*this);
}

//void rendering::particles::ParticlesSystemBuilder::Build()
//{
	////m_particleTexture = m_gameManager->AddParticleTexture(m_textureID,
	////	GET_CONFIG_VALUE_STR_GAME("particleTexture" + m_configurationSuffix, "particleRain.png"),
	////	GET_CONFIG_VALUE_RENDERING("particleTextureRowsCount" + m_configurationSuffix, 4),
	////	GET_CONFIG_VALUE_RENDERING("particleTextureIsAdditive" + m_configurationSuffix, true));
	////CHECK_CONDITION_GAME(m_particleTexture != NULL, Utility::Logging::CRITICAL, "Cannot create particle system. The texture is NULL.");

	////if (GET_CONFIG_VALUE_RENDERING("particleAttributePositionEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::POSITION;
	////}
	////if (GET_CONFIG_VALUE_RENDERING("particleAttributeVelocityEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::VELOCITY;
	////}
	////if (GET_CONFIG_VALUE_RENDERING("particleAttributeAccelerationEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::ACCELERATION;
	////}
	////if (GET_CONFIG_VALUE_RENDERING("particleAttributeGravityEffectFactorEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::GRAVITY_EFFECT_FACTOR;
	////}
	////if (GET_CONFIG_VALUE_RENDERING("particleAttributeLifeSpanEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::LIFE_SPAN;
	////}
	////if (GET_CONFIG_VALUE_RENDERING("particleAttributeRotationEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::ROTATION;
	////}
	////if (GET_CONFIG_VALUE_RENDERING("particleAttributeScaleEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::SCALE;
	////}
	////if (GET_CONFIG_VALUE_RENDERING("particleAttributeTextureOffsetEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::TEXTURE_OFFSET;
	////}
	////if (GET_CONFIG_VALUE_RENDERING("particleAttributeColorEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::COLOR;
	////}
	////if (GET_CONFIG_VALUE_RENDERING("particleAttributeMassEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::MASS;
	////}
	////if (GET_CONFIG_VALUE_RENDERING("particleAttributeAliveEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::ALIVE;
	////}
	////if (GET_CONFIG_VALUE_RENDERING("particleAttributeIdEnabled" + m_configurationSuffix, true))
	////{
	////	m_attributesMask |= Attributes::ID;
	////}
	////DEBUG_LOG_GAME("Attributes mask = ", m_attributesMask.m_attributesMask);
//}

//void ParticlesSystemBuilder::BuildPart2()
//{
//	m_particleEmittersCount = GET_CONFIG_VALUE_RENDERING("particleEmittersCount" + m_configurationSuffix, 1);
//	for (unsigned int i = 0; i < m_particleEmittersCount; ++i)
//	{
//		const std::string iStr = std::to_string(i + 1);
//		m_emitters.emplace_back(GET_CONFIG_VALUE_RENDERING("particleEmitterGeneratedParticlesPerSecond" + m_configurationSuffix + "_" + iStr, 400.0f));
//
//		AddPositionGenerator(&m_emitters.back(), iStr);
//		AddVelocityGenerator(&m_emitters.back(), iStr);
//		AddAccelerationGenerator(&m_emitters.back(), iStr);
//		AddGravityEffectFactorGenerator(&m_emitters.back(), iStr);
//		AddLifeSpanGenerator(&m_emitters.back(), iStr);
//		AddRotationGenerator(&m_emitters.back(), iStr);
//		AddScaleGenerator(&m_emitters.back(), iStr);
//		AddTextureOffsetGenerator(&m_emitters.back(), iStr);
//		AddColorGenerator(&m_emitters.back(), iStr);
//		AddMassGenerator(&m_emitters.back(), iStr);
//		AddAliveGenerator(&m_emitters.back(), iStr);
//		AddIdGenerator(&m_emitters.back(), iStr);
//	}
//}

//void ParticlesSystemBuilder::BuildPart3()
//{
//	//m_particleUpdatersCount = GET_CONFIG_VALUE_RENDERING("particleUpdatersCount" + m_configurationSuffix, 1);
//	//for (unsigned int i = 0; i < m_particleUpdatersCount; ++i)
//	//{
//	//	const std::string iStr = std::to_string(i + 1);
//	//	// TODO: Configurable updaters
//	//}
//	m_updaters.push_back(new EulerParticlesUpdater(math::Vector3D(0.0f, 0.0f, 0.0f)));
//	m_updaters.push_back(new LifeSpanParticlesUpdater());
//	//m_updaters.push_back(new RotationParticlesUpdater(math::Angle(90.0f, math::units::DEGREE)));
//
//	m_object->SetMaxParticlesCount(GET_CONFIG_VALUE_RENDERING("particleMaxParticlesCount" + m_configurationSuffix, 5000));
//	m_object->SetAttributesMask(m_attributesMask);
//	m_object->SetParticleTexture(m_particleTexture);
//
//	for (auto emitter = m_emitters.begin(); emitter != m_emitters.end(); ++emitter)
//	{
//		m_object->AddEmitter(*emitter);
//	}
//	for (auto updater = m_updaters.begin(); updater != m_updaters.end(); ++updater)
//	{
//		m_object->AddUpdater(*updater);
//	}
//}

rendering::particles::ParticlesSystemBuilder& rendering::particles::ParticlesSystemBuilder::AddEmitter(const ParticlesEmitter& particlesEmitter)
{
	m_emitters.push_back(particlesEmitter);
	return *this;
}

rendering::particles::ParticlesSystemBuilder& rendering::particles::ParticlesSystemBuilder::AddUpdater(std::shared_ptr<updaters::ParticleAttributeUpdater> particlesUpdater)
{
	m_updaters.push_back(particlesUpdater);
	return *this;
}

rendering::particles::ParticlesSystemBuilder& rendering::particles::ParticlesSystemBuilder::SetKiller(ParticlesKiller* particlesKiller)
{
	m_particlesKiller = particlesKiller;
	return *this;
}


void rendering::particles::ParticlesSystemBuilder::AddPositionGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	//CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(Attributes::POSITION), Utility::Logging::DEBUG,
	//	"The position attribute is disabled for the current particles system");

	//generators::position_generator_types::PositionGeneratorType positionGeneratorType = static_cast<generators::position_generator_types::PositionGeneratorType>(
	//	GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(generators::position_generator_types::CONSTANT)));
	//switch (positionGeneratorType)
	//{
	//case generators::position_generator_types::CONSTANT:
	//	emitter->AddGenerator(new generators::ConstantPositionGenerator(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorPosX" + m_configurationSuffix + "_" + indexStr, 50.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorPosY" + m_configurationSuffix + "_" + indexStr, -1.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorPosZ" + m_configurationSuffix + "_" + indexStr, 60.0f)));
	//	break;
	//case generators::position_generator_types::BOX:
	//	emitter->AddGenerator(new generators::BoxPositionGenerator(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorMinX" + m_configurationSuffix + "_" + indexStr, 40.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorMaxX" + m_configurationSuffix + "_" + indexStr, 60.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorMinY" + m_configurationSuffix + "_" + indexStr, -2.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorMaxY" + m_configurationSuffix + "_" + indexStr, 2.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorMinZ" + m_configurationSuffix + "_" + indexStr, 50.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorMaxZ" + m_configurationSuffix + "_" + indexStr, 40.0f)));
	//	break;
	//case generators::position_generator_types::PLANE:
	//	emitter->AddGenerator(new generators::PlanePositionGenerator(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorPlaneNormalX" + m_configurationSuffix + "_" + indexStr, 40.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorPlaneNormalY" + m_configurationSuffix + "_" + indexStr, 40.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorPlaneNormalZ" + m_configurationSuffix + "_" + indexStr, 40.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorPlaneDistance" + m_configurationSuffix + "_" + indexStr, 40.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterPositionGeneratorPlaneRadius" + m_configurationSuffix + "_" + indexStr, 40.0f)));
	//	break;
	//case generators::position_generator_types::ELLIPSOID:
	//	WARNING_LOG_RENDERING("Not yet supported");
	//	break;
	//default:
	//	ERROR_LOG_RENDERING("Unknown particle position generator type: %d", positionGeneratorType);
	//}
}

void rendering::particles::ParticlesSystemBuilder::AddVelocityGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	//CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(Attributes::VELOCITY), Utility::Logging::DEBUG,
	//	"The velocity attribute is disabled for the current particles system");

	//generators::velocity_generator_types::VelocityGeneratorType velocityGeneratorType = static_cast<generators::velocity_generator_types::VelocityGeneratorType>(
	//	GET_CONFIG_VALUE_RENDERING("particleEmitterVelocityGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(generators::velocity_generator_types::CONSTANT)));
	//switch (velocityGeneratorType)
	//{
	//case generators::velocity_generator_types::CONSTANT:
	//	emitter->AddGenerator(new generators::ConstantVelocityGenerator(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterVelocityGeneratorX" + m_configurationSuffix + "_" + indexStr, 50.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterVelocityGeneratorY" + m_configurationSuffix + "_" + indexStr, -1.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterVelocityGeneratorZ" + m_configurationSuffix + "_" + indexStr, 60.0f)));
	//	break;
	//case generators::velocity_generator_types::RANGE:
	//	emitter->AddGenerator(new generators::RangeVelocityGenerator(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterVelocityGeneratorMinX" + m_configurationSuffix + "_" + indexStr, -3.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterVelocityGeneratorMaxX" + m_configurationSuffix + "_" + indexStr, 3.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterVelocityGeneratorMinY" + m_configurationSuffix + "_" + indexStr, 5.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterVelocityGeneratorMaxY" + m_configurationSuffix + "_" + indexStr, 10.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterVelocityGeneratorMinZ" + m_configurationSuffix + "_" + indexStr, -3.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterVelocityGeneratorMaxZ" + m_configurationSuffix + "_" + indexStr, 3.0f)));
	//	break;
	//default:
	//	ERROR_LOG_RENDERING("Unknown particle velocity generator type: %d", velocityGeneratorType);
	//}
}

void rendering::particles::ParticlesSystemBuilder::AddAccelerationGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	//CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(Attributes::ACCELERATION), Utility::Logging::DEBUG,
	//	"The acceleration attribute is disabled for the current particles system");

	//Generators::acceleration_generator_types::AccelerationGeneratorType accelerationGeneratorType = static_cast<Generators::AccelerationGeneratorTypes::AccelerationGeneratorType>(
	//	GET_CONFIG_VALUE_RENDERING("particleEmitterAccelerationGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(Generators::acceleration_generator_types::CONSTANT)));
	//switch (accelerationGeneratorType)
	//{
	//case Generators::acceleration_generator_types::CONSTANT:
	//	emitter->AddGenerator(new Generators::ConstantAccelerationGenerator(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterAccelerationGeneratorX" + m_configurationSuffix + "_" + indexStr, 0.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterAccelerationGeneratorY" + m_configurationSuffix + "_" + indexStr, -5.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterAccelerationGeneratorZ" + m_configurationSuffix + "_" + indexStr, 0.0f)));
	//	break;
	//case Generators::acceleration_generator_types::RANGE:
	//	emitter->AddGenerator(new Generators::RangeAccelerationGenerator(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterAccelerationGeneratorMinX" + m_configurationSuffix + "_" + indexStr, -1.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterAccelerationGeneratorMaxX" + m_configurationSuffix + "_" + indexStr, 1.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterAccelerationGeneratorMinY" + m_configurationSuffix + "_" + indexStr, -8.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterAccelerationGeneratorMaxY" + m_configurationSuffix + "_" + indexStr, -12.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterAccelerationGeneratorMinZ" + m_configurationSuffix + "_" + indexStr, -1.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterAccelerationGeneratorMaxZ" + m_configurationSuffix + "_" + indexStr, 1.0f)));
	//	break;
	//default:
	//	ERROR_LOG_RENDERING("Unknown particle acceleration generator type: %d", accelerationGeneratorType);
	//}
}

void rendering::particles::ParticlesSystemBuilder::AddGravityEffectFactorGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(attributes::GRAVITY_EFFECT_FACTOR), utility::logging::DEBUG,
		"The gravity effect factor attribute is disabled for the current particles system");

	// TODO: Implement this function
}

void rendering::particles::ParticlesSystemBuilder::AddLifeSpanGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	//CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(Attributes::LIFE_SPAN), Utility::Logging::DEBUG,
	//	"The life span attribute is disabled for the current particles system");

	//generators::life_span_generator_types::LifeSpanGeneratorType lifeSpanGeneratorType = static_cast<generators::life_span_generator_types::LifeSpanGeneratorType>(
	//	GET_CONFIG_VALUE_RENDERING("particleEmitterLifeSpanGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(generators::life_span_generator_types::CONSTANT)));
	//switch (lifeSpanGeneratorType)
	//{
	//case generators::life_span_generator_types::CONSTANT:
	//	emitter->AddGenerator(new Generators::ConstantLifeSpanLimitGenerator(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterLifeSpanGenerator" + m_configurationSuffix + "_" + indexStr, 5.0f)));
	//	break;
	//case generators::life_span_generator_types::RANGE:
	//	emitter->AddGenerator(new Generators::RangeLifeSpanLimitGenerator(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterLifeSpanGeneratorMin" + m_configurationSuffix + "_" + indexStr, 2.0f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterLifeSpanGeneratorMax" + m_configurationSuffix + "_" + indexStr, 3.0f)));
	//	break;
	//default:
	//	ERROR_LOG_RENDERING("Unknown particle life span limit generator type: %d", lifeSpanGeneratorType);
	//}
}

void rendering::particles::ParticlesSystemBuilder::AddRotationGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	//CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(Attributes::ROTATION), Utility::Logging::DEBUG,
	//	"The rotation attribute is disabled for the current particles system");

	//generators::rotation_generator_types::RotationGeneratorType rotationGeneratorType = static_cast<generators::rotation_generator_types::RotationGeneratorType>(
	//	GET_CONFIG_VALUE_RENDERING("particleEmitterRotationGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(generators::rotation_generator_types::CONSTANT)));
	//switch (rotationGeneratorType)
	//{
	//case generators::rotation_generator_types::CONSTANT:
	//	emitter->AddGenerator(new generators::ConstantRotationGenerator(math::Angle(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterRotationGenerator" + m_configurationSuffix + "_" + indexStr, 5.0f))
	//		));
	//	break;
	//case generators::rotation_generator_types::RANGE:
	//	emitter->AddGenerator(new generators::RangeRotationGenerator(
	//		math::Angle(GET_CONFIG_VALUE_RENDERING("particleEmitterRotationGeneratorMin" + m_configurationSuffix + "_" + indexStr, 2.0f)),
	//		math::Angle(GET_CONFIG_VALUE_RENDERING("particleEmitterRotationGeneratorMax" + m_configurationSuffix + "_" + indexStr, 3.0f))));
	//	break;
	//default:
	//	ERROR_LOG_RENDERING("Unknown particle rotation generator type: %d", rotationGeneratorType);
	//}
}

void rendering::particles::ParticlesSystemBuilder::AddScaleGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	//CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(Attributes::SCALE), Utility::Logging::DEBUG,
	//	"The scale attribute is disabled for the current particles system");

	//generators::scale_generator_types::ScaleGeneratorType scaleGeneratorType = static_cast<generators::scale_generator_types::ScaleGeneratorType>(
	//	GET_CONFIG_VALUE_RENDERING("particleEmitterScaleGeneratorType" + m_configurationSuffix + "_" + indexStr, static_cast<int>(generators::scale_generator_types::CONSTANT)));
	//switch (scaleGeneratorType)
	//{
	//case generators::scale_generator_types::CONSTANT:
	//	emitter->AddGenerator(new generators::ConstantScaleGenerator(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterScaleGenerator" + m_configurationSuffix + "_" + indexStr, 0.5f)));
	//	break;
	//case generators::scale_generator_types::RANGE:
	//	emitter->AddGenerator(new generators::RangeScaleGenerator(
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterScaleGeneratorMin" + m_configurationSuffix + "_" + indexStr, 0.1f),
	//		GET_CONFIG_VALUE_RENDERING("particleEmitterScaleGeneratorMax" + m_configurationSuffix + "_" + indexStr, 2.0f)));
	//	break;
	//default:
	//	ERROR_LOG_RENDERING("Unknown particle scale generator type: %d", scaleGeneratorType);
	//}
}

void rendering::particles::ParticlesSystemBuilder::AddTextureOffsetGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(attributes::TEXTURE_OFFSET), utility::logging::DEBUG,
		"The texture offset attribute is disabled for the current particles system");
}

void rendering::particles::ParticlesSystemBuilder::AddColorGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(attributes::COLOR), utility::logging::DEBUG,
		"The color attribute is disabled for the current particles system");
}

void rendering::particles::ParticlesSystemBuilder::AddMassGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(attributes::MASS), utility::logging::DEBUG,
		"The mass attribute is disabled for the current particles system");
}

void rendering::particles::ParticlesSystemBuilder::AddAliveGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(attributes::ALIVE), utility::logging::DEBUG,
		"The alive attribute is disabled for the current particles system");
}

void rendering::particles::ParticlesSystemBuilder::AddIdGenerator(ParticlesEmitter* emitter, const std::string& indexStr) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(m_attributesMask.IsAttributeEnabled(attributes::ID), utility::logging::DEBUG,
		"The ID attribute is disabled for the current particles system");
}
