#include "stdafx.h"
#include "TextEffect.h"

Rendering::Text::TextEffectColor::TextEffectColor(const Math::Vector3D& staticColor) :
	m_currentColor(staticColor)
{
}

Rendering::Text::TextEffectColor::~TextEffectColor()
{
}

void Rendering::Text::TextEffectColor::Update(Math::Real deltaTime)
{
	// NOP
	return;
}

Rendering::Text::TextEffectColorGradient::TextEffectColorGradient(const Math::Vector3D& firstColor /* = Math::Vector3D(REAL_ONE, REAL_ONE, REAL_ONE) */) :
	TextEffectColor(firstColor),
	m_colorInterpolator(),
	m_timer(0.0f),
	m_isTimerIncreasing(true)
{
}

Rendering::Text::TextEffectColorGradient::TextEffectColorGradient(const Math::Vector3D* colors, const Math::Real* times, unsigned int colorsCount) :
	TextEffectColor(colors[0]),
	m_colorInterpolator(colors, times ,colorsCount),
	m_timer(times[0]),
	m_isTimerIncreasing(true)
{
}

Rendering::Text::TextEffectColorGradient::~TextEffectColorGradient()
{
}

void Rendering::Text::TextEffectColorGradient::Update(Math::Real deltaTime)
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