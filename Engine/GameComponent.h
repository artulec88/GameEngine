#ifndef __ENGINE_GAME_COMPONENT_H__
#define __ENGINE_GAME_COMPONENT_H__

#include "Engine.h"
#include "GameNode.h"
//#include "InputConstants.h"
//#include "GameNodeCommand.h"

#include "Math\Transform.h"

//#include <map>

namespace Engine
{

	class GameComponent //: public GameNodeCommand
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		GameComponent();
		virtual ~GameComponent(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		//virtual void Execute() const = 0;
		void SetParent(GameNode* parentGameNode);
		Math::Transform& GetTransform();
		const Math::Transform& GetTransform() const;
		Physics::PhysicsObject* GetPhysicsObject() { return m_parentGameNode->GetPhysicsObject(); }
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		GameNode* m_parentGameNode;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class GameComponent */

} /* end namespace Engine */

#endif // __ENGINE_GAME_COMPONENT_H__