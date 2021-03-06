#ifndef __ENGINE_OBSERVER_H__
#define __ENGINE_OBSERVER_H__

#include "Engine.h"
//#include "GameManager.h"
#include "GameNode.h"
//#include "GameEvent.h"

namespace engine
{

	/// <summary>
	/// A class representing an observer in the game engine.
	/// </summary>
	class Observer
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API Observer();
		ENGINE_API virtual ~Observer();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API virtual void Notify(GameNode* gameNode, actions::Action action /*const GameEvent& gameEvent*/) const = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Observer */

} /* end namespace engine */

#endif /* __ENGINE_OBSERVER_H__ */