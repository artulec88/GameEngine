#include "stdafx.h"
#include "EffectFactory.h"


/* ==================== EffectFactory class begin ==================== */
Rendering::Effects::EffectFactory::EffectFactory()
{
}

Rendering::Effects::EffectFactory::~EffectFactory()
{
}

void Rendering::Effects::EffectFactory::CreateEffect(EffectType effectType, Effect<Math::Real>* singleValueEffect)
{
	m_singleValueEffects[effectType].push_back(singleValueEffect);
}

void Rendering::Effects::EffectFactory::CreateEffect(EffectType effectType, Effect<Math::Vector2D>* vec2DEffect)
{
	m_vec2DEffects[effectType].push_back(vec2DEffect);
}

void Rendering::Effects::EffectFactory::CreateEffect(EffectType effectType, Effect<Math::Vector3D>* vec3DEffect)
{
	m_vec3DEffects[effectType].push_back(vec3DEffect);
}

Rendering::Effects::Effect<Math::Real>* Rendering::Effects::EffectFactory::GetSingleValueEffect(EffectType effectType, unsigned int variant)
{
	unsigned int correctVariant;
	if (variant >= m_singleValueEffects[effectType].size())
	{
		correctVariant = m_singleValueEffects[effectType].size() - 1;
	}
	else
	{
		correctVariant = variant;
	}
	return m_singleValueEffects[effectType][correctVariant - 1 /* because we count variants from 1 in cfg file */];
}

Rendering::Effects::Effect<Math::Vector2D>* Rendering::Effects::EffectFactory::GetVec2DEffect(EffectType effectType, unsigned int variant)
{
	unsigned int correctVariant;
	if (variant >= m_vec2DEffects[effectType].size())
	{
		correctVariant = m_vec2DEffects[effectType].size() - 1;
	}
	else
	{
		correctVariant = variant;
	}
	return m_vec2DEffects[effectType][correctVariant - 1 /* because we count variants from 1 in cfg file */];
}

Rendering::Effects::Effect<Math::Vector3D>* Rendering::Effects::EffectFactory::GetVec3DEffect(EffectType effectType, unsigned int variant)
{
	unsigned int correctVariant;
	if (variant >= m_vec3DEffects[effectType].size())
	{
		correctVariant = m_vec3DEffects[effectType].size() - 1;
	}
	else
	{
		correctVariant = variant;
	}
	return m_vec3DEffects[effectType][correctVariant - 1 /* because we count variants from 1 in cfg file */];
}
/* ==================== EffectFactory class end ==================== */