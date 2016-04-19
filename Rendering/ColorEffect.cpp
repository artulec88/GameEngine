#include "stdafx.h"
#include "ColorEffect.h"

/* ==================== ColorEffect class begin ==================== */
Rendering::Effects::ColorEffect::ColorEffect(const Math::Vector3D& staticColor) :
	m_currentColor(staticColor)
{
}

Rendering::Effects::ColorEffect::~ColorEffect()
{
}

void Rendering::Effects::ColorEffect::Update(Math::Real deltaTime)
{
	// NOP
	return;
}
/* ==================== ColorEffect class end ==================== */

/* ==================== SmoothTransitionColorEffect class begin ==================== */
Rendering::Effects::SmoothTransitionColorEffect::SmoothTransitionColorEffect(const Math::Vector3D* colors, const Math::Real* times, unsigned int colorsCount) :
	ColorEffect(colors[0]),
	m_colorInterpolator(colors, times ,colorsCount),
	m_timer(times[0]),
	m_isTimerIncreasing(true)
{
}

Rendering::Effects::SmoothTransitionColorEffect::~SmoothTransitionColorEffect()
{
}

void Rendering::Effects::SmoothTransitionColorEffect::Update(Math::Real deltaTime)
{
	if (m_isTimerIncreasing)
	{
		m_timer += deltaTime;
		if (m_timer > m_colorInterpolator.GetEndTime())
		{
			m_timer = m_colorInterpolator.GetEndTime() - (m_timer - m_colorInterpolator.GetEndTime());
			m_isTimerIncreasing = false;
		}
	}
	else
	{
		m_timer -= deltaTime;
		if (m_timer < m_colorInterpolator.GetStartTime())
		{
			m_timer = m_colorInterpolator.GetStartTime() + (m_colorInterpolator.GetStartTime() - m_timer);
			m_isTimerIncreasing = true;
		}
	}
	m_currentColor = m_colorInterpolator.Interpolate(m_timer);
}
/* ==================== SmoothTransitionColorEffect class end ==================== */

/* ==================== BlinkColorEffect class begin ==================== */
/* static */ const Math::Real Rendering::Effects::BlinkColorEffect::DEFAULT_DURATION = 0.5f;

Rendering::Effects::BlinkColorEffect::BlinkColorEffect(const Math::Vector3D* colors, const Math::Real* durations, unsigned int colorsCount) :
	ColorEffect(colors[0]),
	m_currentIndex(0),
	m_timer(0.0f)
{
	for (unsigned int i = 0; i < colorsCount; ++i)
	{
		m_colors.push_back(colors[i]);
		if (!(durations[i] > REAL_ZERO))
		{
			WARNING_LOG("Blinking color effect's duration[%d] equals %.3f which is less than 0.0. Using default duration %.3f", i, durations[i], DEFAULT_DURATION);
			m_durations.push_back(DEFAULT_DURATION);
		}
		else
		{
			m_durations.push_back(durations[i]);
		}
	}
}

Rendering::Effects::BlinkColorEffect::~BlinkColorEffect()
{
}

void Rendering::Effects::BlinkColorEffect::Update(Math::Real deltaTime)
{
	m_timer += deltaTime;
	if (m_timer >= m_durations[m_currentIndex])
	{
		m_timer = 0.0f;
		if (m_currentIndex == m_colors.size() - 1)
		{
			m_currentIndex = 0;
		}
		else
		{
			++m_currentIndex;
		}
		CHECK_CONDITION(m_currentIndex >= 0 && m_currentIndex < m_colors.size(), Utility::Error, "Blinking color effect's index calculation incorrect. The index %d is out of range [0; %d)",
			m_currentIndex, m_colors.size());
		DEBUG_LOG("Switching to color effect %d", m_currentIndex);
		m_currentColor = m_colors[m_currentIndex];
	}
}
/* ==================== BlinkColorEffect class end ==================== */

/* ==================== BlinkColorEffect class begin ==================== */
Rendering::Effects::ColorEffectFactory::ColorEffectFactory()
{
}

Rendering::Effects::ColorEffectFactory::~ColorEffectFactory()
{
}

void Rendering::Effects::ColorEffectFactory::CreateColorEffect(ColorEffectType colorEffectType, ColorEffect* colorEffect)
{
	m_colorEffects[colorEffectType].push_back(colorEffect);
}

Rendering::Effects::ColorEffect* Rendering::Effects::ColorEffectFactory::GetColorEffect(ColorEffectType colorEffectType, unsigned int variant)
{
	unsigned int correctVariant;
	if (variant >= m_colorEffects[colorEffectType].size())
	{
		correctVariant = m_colorEffects[colorEffectType].size() - 1;
	}
	else
	{
		correctVariant = variant;
	}
	return m_colorEffects[colorEffectType][correctVariant - 1 /* because we count variants from 1 in cfg file */];
}
/* ==================== ColorEffectFactory class end ==================== */