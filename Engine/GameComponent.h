#ifndef __ENGINE_GAME_COMPONENT_H__
#define __ENGINE_GAME_COMPONENT_H__

#include "Engine.h"
#include "GameNode.h"

#include "Math\Transform.h"

namespace Engine
{

	class GameComponent
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API GameComponent();
		ENGINE_API virtual ~GameComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		void SetParent(GameNode* parentGameNode);
		Math::Transform& GetTransform();
		const Math::Transform& GetTransform() const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		GameNode* m_parentGameNode;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class GameComponent */

} /* end namespace Engine */

#endif // __ENGINE_GAME_COMPONENT_H__