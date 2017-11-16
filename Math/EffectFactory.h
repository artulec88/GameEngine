#ifndef __MATH_EFFECTS_EFFECT_FACTORY_H__
#define __MATH_EFFECTS_EFFECT_FACTORY_H__

#include "Math.h"

namespace math
{
	namespace effects
	{
		enum EffectType
		{
			STATIC = 0,
			SMOOTH = 1,
			BLINK = 2,
			COUNT
		};

		class EffectFactory
		{
			/* ==================== Static variables begin ==================== */
			/* ==================== Static variables end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Creates a new effect factory. </summary>
			MATH_API EffectFactory();

			/// <summary> Effect factory destructor. </summary>
			MATH_API ~EffectFactory();

			/// <summary> Effect factory copy constructor. </summary>
			/// <param name="effectFactory"> The effect factory to copy construct from. </param>
			EffectFactory(const EffectFactory& effectFactory) = delete;

			/// <summary> Effect factory move constructor. </summary>
			/// <param name="effectFactory"> The effect factory to move construct from. </param>
			EffectFactory(EffectFactory&& effectFactory) = delete;

			/// <summary> Effect factory copy assignment operator. </summary>
			/// <param name="effectFactory"> The effect factory to copy assign from. </param>
			/// <returns> The newly copy-assigned effect factory. </returns>
			EffectFactory& operator=(const EffectFactory& effectFactory) = delete;

			/// <summary> Effect factory move assignment operator. </summary>
			/// <param name="effectFactory"> The effect factory to move assign from. </param>
			/// <returns> The newly move-assigned effect factory. </returns>
			EffectFactory& operator=(EffectFactory&& effectFactory) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			//template <typename T>
			//MATH_API std::unique_ptr<Effect<T>> CreateEffect(const std::string& effectConfigurationFileName)
			//{

			//}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class EffectFactory */
	} /* end namespace effects */
} /* end namespace math */

#endif /* __MATH_EFFECTS_EFFECT_FACTORY_H__ */