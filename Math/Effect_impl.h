#ifndef __MATH_EFFECTS_EFFECT_IMPL_H__
#define __MATH_EFFECTS_EFFECT_IMPL_H__

#include "stdafx.h"
#include "Effect.h"

/* ==================== Effect<T> class begin ==================== */
template <class T>
math::effects::Effect<T>::Effect(const T& initialValue) :
	m_value(initialValue)
{
}

template <class T>
math::effects::Effect<T>::~Effect()
{
}
/* ==================== Effect<T> class end ==================== */

/* ==================== NoEffect<T> class begin ==================== */
template <class T>
math::effects::NoEffect<T>::NoEffect(const T& initialValue) :
	Effect(initialValue)
{
}

template <class T>
math::effects::NoEffect<T>::~NoEffect()
{
}

template <class T>
void math::effects::NoEffect<T>::Update(Real deltaTime)
{
	// NOP
}
/* ==================== NoEffect<T> class end ==================== */

/* ==================== SmoothTransitionEffect<T> class begin ==================== */
template <class T>
math::effects::SmoothTransitionEffect<T>::SmoothTransitionEffect(const T* values, const Real* times, unsigned int valuesCount, bool isGoingBackAndForthEnabled) :
	Effect(values[0]),
	m_valuesInterpolator(values, times, valuesCount),
	m_timer(times[0]),
	m_isGoingBackAndForthEnabled(isGoingBackAndForthEnabled),
	m_isTimerIncreasing(true)
{
}

template <class T>
math::effects::SmoothTransitionEffect<T>::~SmoothTransitionEffect()
{
}

template <class T>
void math::effects::SmoothTransitionEffect<T>::Update(Real deltaTime)
{
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
	m_value = m_valuesInterpolator.Interpolate(m_timer); // move copy assignment
}
/* ==================== SmoothTransitionEffect<T> class end ==================== */

/* ==================== BlinkEffect<T> class begin ==================== */
template <class T>
math::effects::BlinkEffect<T>::BlinkEffect(const T* values, const Real* durations, unsigned int valuesCount) :
	Effect(values[0]),
	m_currentIndex(0),
	m_timer(0.0f)
{
	for (unsigned int i = 0; i < valuesCount; ++i)
	{
		m_values.push_back(values[i]);
		if (!(durations[i] > REAL_ZERO))
		{
			WARNING_LOG_MATH("Blinking effect's duration[", i, "] equals ", durations[i], " which is less than 0.0. Using default duration ", DEFAULT_DURATION);
			m_durations.push_back(DEFAULT_DURATION);
		}
		else
		{
			m_durations.push_back(durations[i]);
		}
	}
}

template <class T>
math::effects::BlinkEffect<T>::~BlinkEffect()
{
}

template <class T>
void math::effects::BlinkEffect<T>::Update(Real deltaTime)
{
	m_timer += deltaTime;
	if (m_timer >= m_durations[m_currentIndex])
	{
		m_timer = 0.0f;
		m_currentIndex = (m_currentIndex + 1) % m_values.size();
		CHECK_CONDITION_MATH(m_currentIndex >= 0 && m_currentIndex < m_values.size(), Utility::Logging::ERR, "Blinking effect's index calculation incorrect. The index ",
			m_currentIndex, " is out of range [0; ", m_values.size(), ")");
		DEBUG_LOG_MATH("Switching to effect ", m_currentIndex);
		m_value = m_values[m_currentIndex];
	}
}
/* ==================== BlinkEffect<T> class end ==================== */

#endif /* __MATH_EFFECTS_EFFECT_IMPL_H__ */