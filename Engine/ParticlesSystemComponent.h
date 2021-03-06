#ifndef __ENGINE_PARTICLES_SYSTEM_COMPONENT_H__
#define __ENGINE_PARTICLES_SYSTEM_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "GameManager.h"
#include "IUpdateable.h"
#include "Rendering/ParticlesSystem.h"

//#include "Math/Math.h"
//#include "Math/Angle.h"
//#include "Math/Vector.h"

namespace engine
{

	class ParticlesSystemComponent : public GameComponent, public IUpdateable
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ParticlesSystemComponent(GameManager* gameManager, rendering::particles::ParticlesSystem* particlesSystem);
		ENGINE_API virtual ~ParticlesSystemComponent();
		ParticlesSystemComponent(const ParticlesSystemComponent& particlesSystemComponent) = delete;
		ParticlesSystemComponent(ParticlesSystemComponent&& particlesSystemComponent) noexcept;
		ParticlesSystemComponent& operator=(const ParticlesSystemComponent& particlesSystemComponent) = delete;
		ParticlesSystemComponent& operator=(ParticlesSystemComponent&& particlesSystemComponent) noexcept;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void Update(math::Real deltaTime) override;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		rendering::particles::ParticlesSystem* m_particlesSystem;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticlesSystemComponent */

} /* end namespace engine */

#endif /* __ENGINE_PARTICLES_SYSTEM_COMPONENT_H__ */