#ifndef __ENGINE_PARTICLE_GENERATOR_COMPONENT_H__
#define __ENGINE_PARTICLE_GENERATOR_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "GameManager.h"
#include "IUpdateable.h"
#include "ParticleGenerator.h"

//#include "Math\Math.h"
//#include "Math\Angle.h"
//#include "Math\Vector.h"

namespace Engine
{

	class ParticleGeneratorComponent : public GameComponent, public IUpdateable
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ParticleGeneratorComponent(GameManager* gameManager, Math::Real particlesPerSecondCount, Math::Real speed, Math::Real gravityComplient, Math::Real lifeSpanLimit);
		ENGINE_API virtual ~ParticleGeneratorComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Update(Math::Real deltaTime);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		ParticleGenerator m_particleGenerator;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticleGeneratorComponent */

} /* end namespace Engine */

#endif /* __ENGINE_PARTICLE_GENERATOR_COMPONENT_H__ */