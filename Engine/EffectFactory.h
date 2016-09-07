#ifndef __ENGINE_EFFECTS_EFFECT_FACTORY_H__
#define __ENGINE_EFFECTS_EFFECT_FACTORY_H__

#include "Engine.h"

#include "Math\Effect.h"

#include <map>

namespace Engine
{
	namespace Effects
	{
		enum EffectType
		{
			STATIC = 0,
			SMOOTH = 1,
			BLINK = 2
		};

		class EffectFactory
		{
		/* ==================== Static variables begin ==================== */
		private:
			typedef std::map<EffectType, std::vector<Math::Effects::Effect<Math::Real>*>> RealEffectsMap;
			typedef std::map<EffectType, std::vector<Math::Effects::Effect<Math::Vector2D>*>> Vec2DEffectsMap;
			typedef std::map<EffectType, std::vector<Math::Effects::Effect<Math::Vector3D>*>> Vec3DEffectsMap;
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new effect factory.
			/// </summary>
			ENGINE_API EffectFactory();
			ENGINE_API virtual ~EffectFactory();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			ENGINE_API void CreateEffect(EffectType effectType, Math::Effects::Effect<Math::Real>* singleValueEffect);
			ENGINE_API void CreateEffect(EffectType effectType, Math::Effects::Effect<Math::Vector2D>* vec2DEffect);
			ENGINE_API void CreateEffect(EffectType effectType, Math::Effects::Effect<Math::Vector3D>* vec3DEffect);

			/// <summary> Returns the specific single value effect. </summary>
			/// <param name="effectType"> Type of the effect. </param>
			/// <returns> The single value effect. </returns>
			ENGINE_API Math::Effects::Effect<Math::Real>* GetSingleValueEffect(EffectType effectType, unsigned int variant);

			/// <summary> Returns the specific vector 2D effect. </summary>
			/// <param name="effectType"> Type of the effect. </param>
			/// <returns> The vector 2D effect. </returns>
			ENGINE_API Math::Effects::Effect<Math::Vector2D>* GetVec2DEffect(EffectType effectType, unsigned int variant);

			/// <summary> Returns the specific vector 3D effect. </summary>
			/// <param name="effectType"> Type of the effect. </param>
			/// <returns> The vector 3D effect. </returns>
			ENGINE_API Math::Effects::Effect<Math::Vector3D>* GetVec3DEffect(EffectType effectType, unsigned int variant);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		private:
			RealEffectsMap m_singleValueEffects;
			Vec2DEffectsMap m_vec2DEffects;
			Vec3DEffectsMap m_vec3DEffects;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class EffectFactory */
	} /* end namespace Effects */
} /* end namespace Engine */

#endif /* __ENGINE_EFFECTS_EFFECT_FACTORY_H__ */