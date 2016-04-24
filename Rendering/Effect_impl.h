#include "stdafx.h"
#include "Effect.h"

/* ==================== Effect<T> class begin ==================== */
template <class T>
Rendering::Effects::Effect<T>::Effect(T* attribute) :
	m_attribute(attribute),
	m_initialValue()
{
	if (m_attribute != NULL)
	{
		m_initialValue = *m_attribute;
	}
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
Rendering::Effects::SmoothTransitionEffect<T>::SmoothTransitionEffect(T* attribute, const T* values, const Math::Real* times, unsigned int valuesCount, bool isGoingBackAndForthEnabled) :
	Effect(attribute),
	m_valuesInterpolator(values, times, valuesCount),
	m_timer(times[0]),
	m_isGoingBackAndForthEnabled(isGoingBackAndForthEnabled),
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
	if (m_attribute == NULL)
	{
		return;
	}
	if (m_isTimerIncreasing)
	{
		m_timer += deltaTime;
		if (m_timer > m_valuesInterpolator.GetEndTime())
		{
			if (m_isGoingBackAndForthEnabled)
			{
				m_timer = REAL_ZERO;
			}
			else
			{
				m_timer = m_valuesInterpolator.GetEndTime() - (m_timer - m_valuesInterpolator.GetEndTime());
				m_isTimerIncreasing = false;
			}
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
	*m_attribute = m_valuesInterpolator.Interpolate(m_timer);
}
/* ==================== SmoothTransitionEffect<T> class end ==================== */

/* ==================== BlinkEffect<T> class begin ==================== */
template <class T>
/* static */ const Math::Real Rendering::Effects::BlinkEffect<T>::DEFAULT_DURATION = 0.5f;

template <class T>
Rendering::Effects::BlinkEffect<T>::BlinkEffect(T* attribute, const T* values, const Math::Real* durations, unsigned int valuesCount) :
	Effect(attribute),
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
	if (m_attribute == NULL)
	{
		return;
	}
	m_timer += deltaTime;
	if (m_timer >= m_durations[m_currentIndex])
	{
		m_timer = 0.0f;
		m_currentIndex = (m_currentIndex + 1) % m_values.size();
		CHECK_CONDITION(m_currentIndex >= 0 && m_currentIndex < m_values.size(), Utility::Error, "Blinking effect's index calculation incorrect. The index %d is out of range [0; %d)",
			m_currentIndex, m_values.size());
		DEBUG_LOG("Switching to effect %d", m_currentIndex);
		*m_attribute = m_values[m_currentIndex];
	}
}
/* ==================== BlinkEffect<T> class end ==================== */
