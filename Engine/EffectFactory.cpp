#include "stdafx.h"
#include "EffectFactory.h"


/* ==================== EffectFactory class begin ==================== */
Engine::Effects::EffectFactory::EffectFactory()
{
}

Engine::Effects::EffectFactory::~EffectFactory()
{
}

void Engine::Effects::EffectFactory::CreateEffect(EffectType effectType, Effect<Math::Real>* singleValueEffect)
{
	m_singleValueEffects[effectType].push_back(singleValueEffect);
}

void Engine::Effects::EffectFactory::CreateEffect(EffectType effectType, Effect<Math::Vector2D>* vec2DEffect)
{
	m_vec2DEffects[effectType].push_back(vec2DEffect);
}

void Engine::Effects::EffectFactory::CreateEffect(EffectType effectType, Effect<Math::Vector3D>* vec3DEffect)
{
	m_vec3DEffects[effectType].push_back(vec3DEffect);
}

Engine::Effects::Effect<Math::Real>* Engine::Effects::EffectFactory::GetSingleValueEffect(EffectType effectType, unsigned int variant)
{
	unsigned int correctVariant;
	if (variant > m_singleValueEffects[effectType].size())
	{
		WARNING_LOG("Incorrect single value effect variant (%d) specified. The correct variant must be in range [1; %d].", variant, m_singleValueEffects.size());
		correctVariant = m_singleValueEffects[effectType].size();
	}
	else if (variant <= 0)
	{
		WARNING_LOG("Incorrect single value effect variant (%d) specified. The correct variant must be in range [1; %d].", variant, m_singleValueEffects.size());
		correctVariant = 1;
	}
	else
	{
		correctVariant = variant;
	}
	return m_singleValueEffects[effectType][correctVariant - 1 /* because we count variants from 1 in cfg file */];
}

Engine::Effects::Effect<Math::Vector2D>* Engine::Effects::EffectFactory::GetVec2DEffect(EffectType effectType, unsigned int variant)
{
	unsigned int correctVariant;
	if (variant > m_vec2DEffects[effectType].size())
	{
		WARNING_LOG("Incorrect vector 2D effect variant (%d) specified. The correct variant must be in range [1; %d].", variant, m_vec2DEffects.size());
		correctVariant = m_vec2DEffects[effectType].size();
	}
	else if (variant <= 0)
	{
		WARNING_LOG("Incorrect vector 2D effect variant (%d) specified. The correct variant must be in range [1; %d].", variant, m_vec2DEffects.size());
		correctVariant = 1;
	}
	else
	{
		correctVariant = variant;
	}
	return m_vec2DEffects[effectType][correctVariant - 1 /* because we count variants from 1 in cfg file */];
}

Engine::Effects::Effect<Math::Vector3D>* Engine::Effects::EffectFactory::GetVec3DEffect(EffectType effectType, unsigned int variant)
{
	unsigned int correctVariant;
	if (variant > m_vec3DEffects[effectType].size())
	{
		WARNING_LOG("Incorrect vector 3D effect variant (%d) specified. The correct variant must be in range [1; %d].", variant, m_vec3DEffects.size());
		correctVariant = m_vec3DEffects[effectType].size() - 1;
	}
	else if (variant <= 0)
	{
		WARNING_LOG("Incorrect vector 3D effect variant (%d) specified. The correct variant must be in range [1; %d].", variant, m_vec3DEffects.size());
		correctVariant = 1;
	}
	else
	{
		correctVariant = variant;
	}
	return m_vec3DEffects[effectType][correctVariant - 1 /* because we count variants from 1 in cfg file */];
}
/* ==================== EffectFactory class end ==================== */