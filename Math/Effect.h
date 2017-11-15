#ifndef __MATH_EFFECTS_EFFECT_H__
#define __MATH_EFFECTS_EFFECT_H__

#include "Math.h"

#include "Interpolation.h"
#include "Interpolation_impl.h"

namespace math
{
	namespace effects
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
			/// <summary> Creates a new effect with a given initial value. </summary>
			explicit Effect(const T& initialValue);

			/// <summary> Effect destructor. </summary>
			virtual ~Effect();

			/// <summary> Effect copy constructor. </summary>
			/// <param name="effect"> The effect to copy construct from. </param>
			Effect(const Effect<T>& effect) = delete;

			/// <summary> Effect move constructor. </summary>
			/// <param name="effect"> The effect to move construct from. </param>
			Effect(Effect<T>&& effect) = delete;

			/// <summary> Effect copy assignment operator. </summary>
			/// <param name="effect"> The effect to copy assign from. </param>
			/// <returns> The newly copy-assigned effect. </returns>
			Effect& operator=(const Effect<T>& effect) = delete;

			/// <summary> Effect move assignment operator. </summary>
			/// <param name="effect"> The effect to move assign from. </param>
			/// <returns> The newly move-assigned effect. </returns>
			Effect& operator=(Effect<T>&& effect) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary> Applies the current value of the effect to the specified <paramref name="attribute"/>. </summary>
			/// <param name="attribute"> The attribute for which the effect will be applied. </param>
			void Apply(T* attribute) const
			{
				*attribute = m_value;
			}

			/// <summary>
			/// Updates the current value of the effect accordingly to some specific algorithm.
			/// </summary>
			/// <param name="deltaTime"> The time passed since the last update. </param>
			virtual void Update(Real deltaTime) = 0;
			//const T& Get() const { return m_currentValue; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		protected:
			T m_value;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Effect<T> */

		/// <summary>
		/// Effect that simply simulates no effect at all.
		/// </summary>
		template <class T>
		class NoEffect : public Effect<T>
		{
			/* ==================== Static variables begin ==================== */
			/* ==================== Static variables end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new no-effect.
			/// </summary>
			/// <param name="initialValue"> The values that effect will use. </param>
			NoEffect(const T& initialValue);

			/// <summary> No-effect destructor. </summary>
			virtual ~NoEffect();

			/// <summary> No-effect copy constructor. </summary>
			/// <param name="noEffect"> The no-effect to copy construct from. </param>
			NoEffect(const NoEffect<T>& noEffect) = delete;

			/// <summary> No-effect move constructor. </summary>
			/// <param name="noEffect"> The no-effect to move construct from. </param>
			NoEffect(NoEffect<T>&& noEffect) = delete;

			/// <summary> No-effect copy assignment operator. </summary>
			/// <param name="noEffect"> The no-effect to copy assign from. </param>
			/// <returns> The newly copy-assigned no-effect. </returns>
			NoEffect& operator=(const NoEffect<T>& noEffect) = delete;

			/// <summary> No-effect move assignment operator. </summary>
			/// <param name="noEffect"> The no-effect to move assign from. </param>
			/// <returns> The newly move-assigned no-effect. </returns>
			NoEffect& operator=(NoEffect<T>&& noEffect) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Performs no operation since the no-effect does not change with time.
			/// </summary>
			/// <param name="deltaTime"> The time passed since the last update. </param>
			virtual void Update(Real deltaTime) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class NoEffect */

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
			SmoothTransitionEffect(const T* values, const Real* times, unsigned int valuesCount, bool isGoingBackAndForthEnabled);

			/// <summary> Smooth transition effect destructor. </summary>
			virtual ~SmoothTransitionEffect();

			/// <summary> Smooth transition effect copy constructor. </summary>
			/// <param name="smoothTransitionEffect"> The smooth transition effect to copy construct from. </param>
			SmoothTransitionEffect(const SmoothTransitionEffect<T>& smoothTransitionEffect) = delete;

			/// <summary> Smooth transition effect move constructor. </summary>
			/// <param name="smoothTransitionEffect"> The smooth transition effect to move construct from. </param>
			SmoothTransitionEffect(SmoothTransitionEffect<T>&& smoothTransitionEffect) = delete;

			/// <summary> Smooth transition effect copy assignment operator. </summary>
			/// <param name="smoothTransitionEffect"> The smooth transition effect to copy assign from. </param>
			/// <returns> The newly copy-assigned smooth transition effect. </returns>
			SmoothTransitionEffect& operator=(const SmoothTransitionEffect<T>& smoothTransitionEffect) = delete;

			/// <summary> Smooth transition effect move assignment operator. </summary>
			/// <param name="smoothTransitionEffect"> The smooth transition effect to move assign from. </param>
			/// <returns> The newly move-assigned smooth transition effect. </returns>
			SmoothTransitionEffect& operator=(SmoothTransitionEffect<T>&& smoothTransitionEffect) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Updates the effect.
			/// </summary>
			/// <param name="deltaTime"> The time passed since the last update. </param>
			virtual void Update(Real deltaTime) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			interpolation::LinearInterpolator<T> m_valuesInterpolator;
			Real m_timer;
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
			static constexpr Real DEFAULT_DURATION = 0.5f;
			/* ==================== Static variables end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new blinking effect.
			/// </summary>
			/// <param name="values"> The values that effect will use. </param>
			/// <param name="durations"> The time durations each value is applied for. </param>
			/// <param name="valuesCount"> The number of values (and durations too). </param>
			BlinkEffect(const T* values, const Real* durations, unsigned int valuesCount);

			/// <summary> Blinking effect destructor. </summary>
			virtual ~BlinkEffect();

			/// <summary> Blinking effect copy constructor. </summary>
			/// <param name="blinkEffect"> The blinking effect to copy construct from. </param>
			BlinkEffect(const BlinkEffect<T>& blinkEffect) = delete;

			/// <summary> Blinking effect move constructor. </summary>
			/// <param name="blinkEffect"> The blinking effect to move construct from. </param>
			BlinkEffect(BlinkEffect<T>&& blinkEffect) = delete;

			/// <summary> Blinking effect copy assignment operator. </summary>
			/// <param name="blinkEffect"> The blinking effect to copy assign from. </param>
			/// <returns> The newly copy-assigned blinking effect. </returns>
			BlinkEffect& operator=(const BlinkEffect<T>& blinkEffect) = delete;

			/// <summary> Blinking effect move assignment operator. </summary>
			/// <param name="blinkEffect"> The blinking effect to move assign from. </param>
			/// <returns> The newly move-assigned blinking effect. </returns>
			BlinkEffect& operator=(BlinkEffect<T>&& blinkEffect) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Updates the effect.
			/// </summary>
			/// <param name="deltaTime"> The time passed since the last update. </param>
			virtual void Update(Real deltaTime) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::vector<T> m_values;
			std::vector<Real> m_durations;
			size_t m_currentIndex;
			Real m_timer;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BlinkEffect */

	} /* end namespace Effects */
} /* end namespace Math */

#endif /* __MATH_EFFECTS_EFFECT_H__ */