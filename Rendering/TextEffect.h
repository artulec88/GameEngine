#ifndef __RENDERING_TEXT_EFFECT_H__
#define __RENDERING_TEXT_EFFECT_H__

#include "Rendering.h"

#include "Math\Interpolation.h"
#include "Math\Interpolation_impl.h"

#include <map>

namespace Rendering
{
	namespace Text
	{
		//enum TextEffectType
		//{
		//	BLINKING = 0,
		//	GLOWING = 1,
		//	SHADOW = 2
		//};

		/// <summary>
		/// Text effect for the color.
		/// </summary>
		class TextEffectColor
		{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new text effect.
			/// </summary>
			/// <param name="color"> The color of the text. </param>
			RENDERING_API TextEffectColor(const Math::Vector3D& staticColor);
			RENDERING_API virtual ~TextEffectColor();
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
		}; /* end class TextEffectColor */

		/// <summary>
		/// Text effect for the color.
		/// </summary>
		class TextEffectColorGradient : public TextEffectColor
		{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new text effect.
			/// </summary>
			/// <param name="color"> The color of the text. </param>
			RENDERING_API TextEffectColorGradient(const Math::Vector3D& firstColor = Math::Vector3D(REAL_ONE, REAL_ONE, REAL_ONE));
			RENDERING_API TextEffectColorGradient(const Math::Vector3D* colors, const Math::Real* times, unsigned int colorsCount);
			RENDERING_API virtual ~TextEffectColorGradient();
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
		}; /* end class TextEffectColorGradient */

		//class TextEffectFactory
		//{
		///* ==================== Static variables begin ==================== */
		///* ==================== Static variables end ==================== */

		///* ==================== Constructors and destructors begin ==================== */
		//public:
		//	/// <summary>
		//	/// Creates a new text effect factory.
		//	/// </summary>
		//	/// <param name="color"> The color of the text. </param>
		//	/// <param name="outlineColor"> The outline color. </param>
		//	/// <param name="offset"> The offset for the second font texture atlas sampling call. </param>
		//	/// <param name="characterWidth">
		//	/// The width of the single character in the text. It has nothing to do with the font size.
		//	/// It simply defines how "fat" the single character should be.
		//	/// </param>
		//	/// <param name="characterEdgeTransitionWidth">
		//	/// Together with <paramref name="characterWidth"/> it defines what values of distances should be handled for anti-aliasing of the characters.
		//	/// For distances between <paramref name="characterWidth"/> and the sum <paramref name="characterWidth"/> + <paramref name="characterEdgeTransitionWidth"/>
		//	/// there will be a smooth transition between the character and the background.
		//	/// </param>
		//	/// <param name="borderWidth"> The border width. </param>
		//	/// <param name="borderEdgeTransitionWidth"> The border edge transition width. </param>
		//	RENDERING_API TextEffectFactory()
		//	{
		//		m_textEffects[BLINKING] = new BlinkingTextEffect();
		//		m_textEffects[GLOWING] = new GlowingTextEffect();
		//		m_textEffects[SHADOW] = new ShadowTextEffect();
		//	}
		//	RENDERING_API virtual ~TextEffectFactory()
		//	{
		//	}
		///* ==================== Constructors and destructors end ==================== */

		///* ==================== Non-static member functions begin ==================== */
		//public:
		//	/// <summary> Returns the specific text effect. </summary>
		//	/// <returns> The text effect. </returns>
		//	RENDERING_API TextEffect* GetTextEffect(TextEffectType textEffectType) const { return m_textEffects[textEffectType]; }
		//	//RENDERING_API TextEffect* GetTextEffect(const Math::Vector3D& color, const Math::Vector3D& outlineColor, const Math::Vector2D& offset,
		//	//	Math::Real characterWidth = 0.5f, Math::Real characterEdgeTransitionWidth = 0.1f, Math::Real borderWidth = 0.4f, Math::Real borderEdgeTransitionWidth = 0.1f) const
		//	//{
		//	//	return new TextEffect(const Math::Vector3D& color, const Math::Vector3D& outlineColor, const Math::Vector2D& offset,
		//	//		Math::Real characterWidth, Math::Real characterEdgeTransitionWidth, Math::Real borderWidth, Math::Real borderEdgeTransitionWidth);
		//	//}
		///* ==================== Non-static member functions end ==================== */

		///* ==================== Non-static member variables begin ==================== */
		//private:
		//	std::map<TextEffectType, TextEffect*> m_textEffects;
		//	//std::vector<TextEffect*> m_customTextEffects;
		///* ==================== Non-static member variables end ==================== */
		//}; /* end class TextEffectFactory */

	} /* end namespace Text */
} /* end namespace Rendering */

#endif /* __RENDERING_TEXT_EFFECT_H__ */