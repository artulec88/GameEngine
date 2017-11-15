#ifndef __ENGINE_SOUND_EFFECT_COMPONENT_H__
#define __ENGINE_SOUND_EFFECT_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
//#include "IUpdateable.h"

//#include "Math\Math.h"
//#include "Math\Angle.h"
//#include "Math\Vector.h"

namespace engine
{

	class SoundEffectComponent : public GameComponent
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API SoundEffectComponent(const std::string& soundEffectPath);
		ENGINE_API virtual ~SoundEffectComponent(void);
		SoundEffectComponent(const SoundEffectComponent& soundEffectComponent) = delete;
		SoundEffectComponent(SoundEffectComponent&& soundEffectComponent) = default;
		SoundEffectComponent& operator=(const SoundEffectComponent& soundEffectComponent) = delete;
		SoundEffectComponent& operator=(SoundEffectComponent&& soundEffectComponent) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		//ENGINE_API virtual void Update(math::Real deltaTime);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		std::string m_soundEffectPath;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AABBColliderComponent */

} /* end namespace Engine */

#endif /* __ENGINE_SOUND_EFFECT_COMPONENT_H__ */