#include "stdafx.h"
#include "Effect.h"

/* ==================== Effect<T> class begin ==================== */
template <class T>
Rendering::Effects::Effect<T>::Effect(const T& staticValue) :
	m_currentValue(staticValue)
{
}

template <class T>
Rendering::Effects::Effect<T>::~Effect()
{
}

template <class T>
void Rendering::Effects::Effect<T>::Update(Math::Real deltaTime)
{
	// NOP
	return;
}
/* ==================== Effect<T> class end ==================== */

/* ==================== SmoothTransitionEffect<T> class begin ==================== */
template <class T>
Rendering::Effects::SmoothTransitionEffect<T>::SmoothTransitionEffect(const T* values, const Math::Real* times, unsigned int valuesCount) :
	Effect(values[0]),
	m_valuesInterpolator(values, times, valuesCount),
	m_timer(times[0]),
	m_isTimerIncreasing(true)
{
}

template <class T>
Rendering::Effects::SmoothTransitionEffect<T>::~SmoothTransitionEffect()
{
}

template <class T>
void Rendering::Effects::SmoothTransitionEffect<T>::Update(Math::Real deltaTime)
{
	if (m_isTimerIncreasing)
	{
		m_timer += deltaTime;
		if (m_timer > m_valuesInterpolator.GetEndTime())
		{
			m_timer = m_valuesInterpolator.GetEndTime() - (m_timer - m_valuesInterpolator.GetEndTime());
			m_isTimerIncreasing = false;
		}
	}
	else
	{
		m_timer -= deltaTime;
		if (m_timer < m_valuesInterpolator.GetStartTime())
		{
			m_timer = m_valuesInterpolator.GetStartTime() + (m_valuesInterpolator.GetStartTime() - m_timer);
			m_isTimerIncreasing = true;
		}
	}
	m_currentValue = m_valuesInterpolator.Interpolate(m_timer);
}
/* ==================== SmoothTransitionEffect<T> class end ==================== */

/* ==================== BlinkEffect<T> class begin ==================== */
template <class T>
/* static */ const Math::Real Rendering::Effects::BlinkEffect<T>::DEFAULT_DURATION = 0.5f;

template <class T>
Rendering::Effects::BlinkEffect<T>::BlinkEffect(const T* values, const Math::Real* durations, unsigned int valuesCount) :
	Effect(values[0]),
	m_currentIndex(0),
	m_timer(0.0f)
{
	for (unsigned int i = 0; i < valuesCount; ++i)
	{
		m_values.push_back(values[i]);
		if (!(durations[i] > REAL_ZERO))
		{
			WARNING_LOG("Blinking effect's duration[%d] equals %.3f which is less than 0.0. Using default duration %.3f", i, durations[i], DEFAULT_DURATION);
			m_durations.push_back(DEFAULT_DURATION);
		}
		else
		{
			m_durations.push_back(durations[i]);
		}
	}
}

template <class T>
Rendering::Effects::BlinkEffect<T>::~BlinkEffect()
{
}

template <class T>
void Rendering::Effects::BlinkEffect<T>::Update(Math::Real deltaTime)
{
	m_timer += deltaTime;
	if (m_timer >= m_durations[m_currentIndex])
	{
		m_timer = 0.0f;
		if (m_currentIndex == m_values.size() - 1)
		{
			m_currentIndex = 0;
		}
		else
		{
			++m_currentIndex;
		}
		CHECK_CONDITION(m_currentIndex >= 0 && m_currentIndex < m_values.size(), Utility::Error, "Blinking effect's index calculation incorrect. The index %d is out of range [0; %d)",
			m_currentIndex, m_values.size());
		DEBUG_LOG("Switching to effect %d", m_currentIndex);
		m_currentValue = m_values[m_currentIndex];
	}
}
/* ==================== BlinkEffect<T> class end ==================== */
