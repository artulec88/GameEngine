#ifndef __RENDERING_EFFECTS_EFFECT_H__
#define __RENDERING_EFFECTS_EFFECT_H__

#include "Rendering.h"

#include "Math\Interpolation.h"
#include "Math\Interpolation_impl.h"

#include <map>

namespace Rendering
{
	namespace Effects
	{
		enum ColorEffectType
		{
			STATIC = 0,
			SMOOTH = 1,
			BLINK = 2
		};

		/// <summary>
		/// Color effect. The base implementation does no change to the output color in the runtime.
		/// </summary>
		class ColorEffect
		{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new color effect.
			/// </summary>
			/// <param name="color"> The color. </param>
			RENDERING_API ColorEffect(const Math::Vector3D& staticColor);
			RENDERING_API virtual ~ColorEffect();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Updates the gui text.
			/// </summary>
			RENDERING_API virtual void Update(Math::Real deltaTime);
			RENDERING_API const Math::Vector3D& GetColor() const { return m_currentColor; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		protected:
			Math::Vector3D m_currentColor;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class ColorEffect */

		/// <summary>
		/// Color effect that smoothly, linearly transitions from one color to the other.
		/// </summary>
		class SmoothTransitionColorEffect : public ColorEffect
		{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new smooth color effect.
			/// </summary>
			/// <param name="colors"> The color of the text. </param>
			RENDERING_API SmoothTransitionColorEffect(const Math::Vector3D* colors, const Math::Real* times, unsigned int colorsCount);
			RENDERING_API virtual ~SmoothTransitionColorEffect();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Updates the gui text.
			/// </summary>
			RENDERING_API virtual void Update(Math::Real deltaTime);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Interpolation::LinearInterpolator<Math::Vector3D> m_colorInterpolator;
			Math::Real m_timer;
			bool m_isTimerIncreasing;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class SmoothTransitionColorEffect */

		/// <summary>
		/// Color effect with blinking.
		/// </summary>
		class BlinkColorEffect : public ColorEffect
		{
		/* ==================== Static variables begin ==================== */
		private:
			static const Math::Real DEFAULT_DURATION;
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new text effect.
			/// </summary>
			/// <param name="color"> The color of the text. </param>
			RENDERING_API BlinkColorEffect(const Math::Vector3D* colors, const Math::Real* durations, unsigned int colorsCount);
			RENDERING_API virtual ~BlinkColorEffect();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Updates the gui text.
			/// </summary>
			RENDERING_API virtual void Update(Math::Real deltaTime);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		private:
			std::vector<Math::Vector3D> m_colors;
			std::vector<Math::Real> m_durations;
			size_t m_currentIndex;
			Math::Real m_timer;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class BlinkColorEffect */

		class ColorEffectFactory
		{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new text effect factory.
			/// </summary>
			RENDERING_API ColorEffectFactory();
			RENDERING_API virtual ~ColorEffectFactory();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API void CreateColorEffect(ColorEffectType colorEffectType, ColorEffect* colorEffect);

			/// <summary> Returns the specific color effect. </summary>
			/// <param name="colorEffectType"> Type of the color effect </param>
			/// <returns> The color effect. </returns>
			RENDERING_API ColorEffect* GetColorEffect(ColorEffectType colorEffectType, unsigned int variant);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		private:
			std::map<ColorEffectType, std::vector<ColorEffect*>> m_colorEffects;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class ColorEffectFactory */

	} /* end namespace Effects */
} /* end namespace Rendering */

#endif /* __RENDERING_EFFECTS_EFFECT_H__ */