#ifndef __MATH_EFFECTS_EFFECT_FACTORY_H__
#define __MATH_EFFECTS_EFFECT_FACTORY_H__

#include "Math.h"
#include "Effect.h"
#include "EffectIDs.h"
#include "Vector.h"

#include <array>

namespace Math
{
	namespace Effects
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
			/// <summary>
			/// Creates a new effect factory.
			/// </summary>
			MATH_API EffectFactory();
			MATH_API ~EffectFactory();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			//template <typename T>
			//MATH_API std::unique_ptr<Effect<T>> CreateEffect(const std::string& effectConfigurationFileName)
			//{

			//}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		private:
		/* ==================== Non-static member variables end ==================== */
		}; /* end class EffectFactory */
	} /* end namespace Effects */
} /* end namespace Math */

#endif /* __MATH_EFFECTS_EFFECT_FACTORY_H__ */