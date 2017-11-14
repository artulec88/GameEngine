#ifndef __ENGINE_GAME_NODE_COMMAND_H__
#define __ENGINE_GAME_NODE_COMMAND_H__

#include "Engine.h"
#include "Utility\ILogger.h"

namespace engine
{

	class GameNode;

	/// <summary>
	/// A class representing a single entity action (e.g. jump, strafe, move).
	/// </summary>
	class GameNodeCommand
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API GameNodeCommand();
		ENGINE_API virtual ~GameNodeCommand();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Execute(GameNode* gameNode) const = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameNodeCommand */


	/// <summary>
	/// A class representing a "Do-nothing" entity action.
	/// </summary>
	/// TODO: Use Flyweight pattern because EmptyGameNodeCommand is a stateless chunk of pure behavior. There is no need to store more than one instance of this class.
	class EmptyGameNodeCommand : public GameNodeCommand
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API EmptyGameNodeCommand() :
			GameNodeCommand()
		{
		}
		ENGINE_API virtual ~EmptyGameNodeCommand()
		{
		}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Execute(GameNode* gameNode) const
		{
			DEBUG_LOG_ENGINE("Do-nothing command started. Remember that executing a DO-NOTHING command wastes precious CPU time.");
			// Do nothing
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class EmptyGameNodeCommand */

} /* end namespace Engine */

#endif /* __ENGINE_GAME_NODE_COMMAND_H__ */