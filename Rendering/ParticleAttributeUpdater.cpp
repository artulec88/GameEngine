#include "stdafx.h"
#include "ParticleAttributeUpdater.h"

/* ==================== class ParticlesUpdater begin ==================== */
rendering::particles::updaters::ParticleAttributeUpdater::ParticleAttributeUpdater(int attributeIds) :
	m_attributesMask(attributeIds)
{
}

rendering::particles::updaters::ParticleAttributeUpdater::ParticleAttributeUpdater(const attributes::AttributesMask& attributesMask) :
	m_attributesMask(attributesMask)
{
}

rendering::particles::updaters::ParticleAttributeUpdater::~ParticleAttributeUpdater()
{
}
/* ==================== class ParticlesUpdater end ==================== */

///* ==================== class RotationParticlesUpdater begin ==================== */
//rendering::particles::updaters::RotationParticlesUpdater::RotationParticlesUpdater(const math::Angle& rotationSpeedAngle) :
//	ParticleAttributeUpdater(),
//	m_rotationSpeedAngle(rotationSpeedAngle)
//{
//}
//
//rendering::particles::updaters::RotationParticlesUpdater::~RotationParticlesUpdater()
//{
//}
//
//void rendering::particles::updaters::RotationParticlesUpdater::Update(math::Real deltaTime, ParticlesContainer* particlesContainer)
//{
//	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
//	{
//		particlesContainer->IncreaseRotation(i, m_rotationSpeedAngle * deltaTime);
//	}
//}
///* ==================== class RotationParticlesUpdater end ==================== */

///* ==================== class ScaleEffectParticlesUpdater begin ==================== */
//rendering::particles::updaters::ScaleEffectParticlesUpdater::ScaleEffectParticlesUpdater(math::effects::Effect<math::Real>* effect) :
//	ParticleAttributeUpdater(),
//	m_effect(effect)
//{
//}
//
//rendering::particles::updaters::ScaleEffectParticlesUpdater::~ScaleEffectParticlesUpdater()
//{
//}
//
//void rendering::particles::updaters::ScaleEffectParticlesUpdater::Update(math::Real deltaTime, ParticlesContainer* particlesContainer)
//{
//	m_effect->Update(deltaTime);
//	for (size_t i = 0; i < particlesContainer->GetAliveCount(); ++i)
//	{
//		m_effect->Apply(particlesContainer->GetScalePtr(i));
//	}
//}
///* ==================== class ScaleEffectParticlesUpdater end ==================== */