#ifndef __ENGINE_AI_COMPONENT_H__
#define __ENGINE_AI_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IUpdateable.h"

namespace engine
{

	/// <summary>
	/// Artifical intelligence game component.
	/// </summary>
	class AiComponent : public GameComponent, public IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API AiComponent();
		ENGINE_API virtual ~AiComponent(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Update(math::Real deltaTime);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AiComponent */

} /* end namespace Engine */

#endif /* __ENGINE_AI_COMPONENT_H__ */