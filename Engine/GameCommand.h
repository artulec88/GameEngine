#ifndef __ENGINE_GAME_COMMAND_H__
#define __ENGINE_GAME_COMMAND_H__

#include "Engine.h"
#include "Utility\ILogger.h"

namespace engine
{

	class GameManager;

	/// <summary>
	/// A class representing a single game action (e.g. start, load, quit).
	/// </summary>
	class GameCommand
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API GameCommand();
		ENGINE_API virtual ~GameCommand();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Execute(GameManager* gameManager) const = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameCommand */


	/// <summary>
	/// A class representing a "Do-nothing" action.
	/// </summary>
	/// TODO: Use Flyweight pattern because EmptyGameCommand is a stateless chunk of pure behavior. There is no need to store more than one instance of this class.
	class EmptyGameCommand : public GameCommand
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API EmptyGameCommand() :
			GameCommand()
		{
		}
		ENGINE_API virtual ~EmptyGameCommand()
		{
		}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Execute(GameManager* gameManager) const
		{
			DEBUG_LOG_ENGINE("Do-nothing command started. Remember that executing a DO-NOTHING command wastes precious CPU time.");
			// Do nothing
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class EmptyGameCommand */

} /* end namespace Engine */

#endif /* __ENGINE_GAME_COMMAND_H__ */