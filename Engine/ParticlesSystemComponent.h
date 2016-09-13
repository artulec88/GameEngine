#ifndef __ENGINE_PARTICLES_SYSTEM_COMPONENT_H__
#define __ENGINE_PARTICLES_SYSTEM_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "GameManager.h"
#include "IUpdateable.h"
#include "Rendering\ParticlesSystem.h"

//#include "Math\Math.h"
//#include "Math\Angle.h"
//#include "Math\Vector.h"

namespace Engine
{

	class ParticlesSystemComponent : public GameComponent, public IUpdateable
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ParticlesSystemComponent(GameManager* gameManager, Rendering::Particles::ParticlesSystem* particlesSystem);
		ENGINE_API virtual ~ParticlesSystemComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Update(Math::Real deltaTime);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Rendering::Particles::ParticlesSystem* m_particlesSystem;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticlesSystemComponent */

} /* end namespace Engine */

#endif /* __ENGINE_PARTICLES_SYSTEM_COMPONENT_H__ */