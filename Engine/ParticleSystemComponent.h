#ifndef __ENGINE_PARTICLE_SYSTEM_COMPONENT_H__
#define __ENGINE_PARTICLE_SYSTEM_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "GameManager.h"
#include "IUpdateable.h"
#include "ParticleSystem.h"

//#include "Math\Math.h"
//#include "Math\Angle.h"
//#include "Math\Vector.h"

namespace Engine
{

	class ParticleSystemComponent : public GameComponent, public IUpdateable
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ParticleSystemComponent(GameManager* gameManager, Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit,
			Math::Real particleSpeed, Math::Real particleGravityComplient, const Math::Angle& particleRotation, Math::Real particleScale);
		ENGINE_API virtual ~ParticleSystemComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Update(Math::Real deltaTime);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		ParticleSystem* m_particleSystem;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticleSystemComponent */

} /* end namespace Engine */

#endif /* __ENGINE_PARTICLE_SYSTEM_COMPONENT_H__ */