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
		ENGINE_API virtual ~AiComponent();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void Update(math::Real deltaTime) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AiComponent */

} /* end namespace engine */

#endif /* __ENGINE_AI_COMPONENT_H__ */