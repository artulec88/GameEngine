#ifndef __RENDERING_EFFECTS_EFFECT_FACTORY_H__
#define __RENDERING_EFFECTS_EFFECT_FACTORY_H__

#include "Rendering.h"
#include "Effect.h"
#include <map>

namespace Rendering
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
			typedef std::map<EffectType, std::vector<Effect<Math::Real>*>> RealEffectsMap;
			typedef std::map<EffectType, std::vector<Effect<Math::Vector2D>*>> Vec2DEffectsMap;
			typedef std::map<EffectType, std::vector<Effect<Math::Vector3D>*>> Vec3DEffectsMap;
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new effect factory.
			/// </summary>
			RENDERING_API EffectFactory();
			RENDERING_API virtual ~EffectFactory();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API void CreateEffect(EffectType effectType, Effect<Math::Real>* singleValueEffect);
			RENDERING_API void CreateEffect(EffectType effectType, Effect<Math::Vector2D>* vec2DEffect);
			RENDERING_API void CreateEffect(EffectType effectType, Effect<Math::Vector3D>* vec3DEffect);

			/// <summary> Returns the specific single value effect. </summary>
			/// <param name="effectType"> Type of the effect. </param>
			/// <returns> The single value effect. </returns>
			RENDERING_API Effect<Math::Real>* GetSingleValueEffect(EffectType effectType, unsigned int variant);

			/// <summary> Returns the specific vector 2D effect. </summary>
			/// <param name="effectType"> Type of the effect. </param>
			/// <returns> The vector 2D effect. </returns>
			RENDERING_API Effect<Math::Vector2D>* GetVec2DEffect(EffectType effectType, unsigned int variant);

			/// <summary> Returns the specific vector 3D effect. </summary>
			/// <param name="effectType"> Type of the effect. </param>
			/// <returns> The vector 3D effect. </returns>
			RENDERING_API Effect<Math::Vector3D>* GetVec3DEffect(EffectType effectType, unsigned int variant);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		private:
			RealEffectsMap m_singleValueEffects;
			Vec2DEffectsMap m_vec2DEffects;
			Vec3DEffectsMap m_vec3DEffects;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class EffectFactory */
	} /* end namespace Effects */
} /* end namespace Rendering */

#endif /* __RENDERING_EFFECTS_EFFECT_FACTORY_H__ */