#ifndef __ENGINE_EFFECTS_EFFECT_H__
#define __ENGINE_EFFECTS_EFFECT_H__

#include "Engine.h"

#include "Math\Interpolation.h"
#include "Math\Interpolation_impl.h"

namespace Engine
{
	namespace Effects
	{
		/// <summary>
		/// Base class for an effect. The base implementation does no change to the output value in the runtime.
		/// </summary>
		template <class T>
		class Effect
		{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new effect.
			/// </summary>
			/// <param name="value"> The pointer to the attribute we want to apply effect on. </param>
			Effect(T* attribute);
			virtual ~Effect();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			void SetAttribute(T* attribute)
			{
				if (m_attribute != NULL)
				{
					*m_attribute = m_initialValue;
				}
				m_initialValue = *attribute;
				m_attribute = attribute;
			}

			/// <summary>
			/// Updates the current value of the attribute accordingly to some specific algorithm.
			/// </summary>
			virtual void Update(Math::Real deltaTime);
			//const T& Get() const { return m_currentValue; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		protected:
			T* m_attribute;
			T m_initialValue;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class Effect */

		/// <summary>
		/// Effect that smoothly, linearly transitions from one value to the other.
		/// </summary>
		template <class T>
		class SmoothTransitionEffect : public Effect<T>
		{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new smooth effect.
			/// </summary>
			/// <param name="values"> The values that effect will use. </param>
			/// <param name="times"> The times that specify when each effect value is to be applied. </param>
			/// <param name="valuesCount"> The number of values (and times too). </param>
			SmoothTransitionEffect(T* attribute, const T* values, const Math::Real* times, unsigned int valuesCount, bool isGoingBackAndForthEnabled);
			virtual ~SmoothTransitionEffect();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Updates the effect.
			/// </summary>
			virtual void Update(Math::Real deltaTime);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Interpolation::LinearInterpolator<T> m_valuesInterpolator;
			Math::Real m_timer;
			bool m_isGoingBackAndForthEnabled;
			bool m_isTimerIncreasing;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class SmoothTransitionEffect */

		/// <summary>
		/// Effect with blinking. Like in Smooth effect we have a specific number of values and we go from one to the other,
		/// but what differs the two effects is that Blink does the transition immediately after specific time span passes.
		/// </summary>
		template <class T>
		class BlinkEffect : public Effect<T>
		{
		/* ==================== Static variables begin ==================== */
		private:
			static const Math::Real DEFAULT_DURATION;
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new blinking effect.
			/// </summary>
			/// <param name="values"> The values that effect will use. </param>
			/// <param name="durations"> The time durations each value is applied for. </param>
			/// <param name="valuesCount"> The number of values (and durations too). </param>
			BlinkEffect(T* attribute, const T* values, const Math::Real* durations, unsigned int valuesCount);
			virtual ~BlinkEffect();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Updates the effect.
			/// </summary>
			virtual void Update(Math::Real deltaTime);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		private:
			std::vector<T> m_values;
			std::vector<Math::Real> m_durations;
			size_t m_currentIndex;
			Math::Real m_timer;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class BlinkEffect */

	} /* end namespace Effects */
} /* end namespace Engine */

#endif /* __ENGINE_EFFECTS_EFFECT_H__ */