#ifndef __ENGINE_GAME_NODE_COMMAND_H__
#define __ENGINE_GAME_NODE_COMMAND_H__

#include "Engine.h"
#include "Utility/ILogger.h"

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
		/// <summary> Base game node command constructor. </summary>
		ENGINE_API GameNodeCommand();

		/// <summary> Base game node command destructor. </summary>
		ENGINE_API virtual ~GameNodeCommand();

		/// <summary> Game node command copy constructor. </summary>
		/// <param name="gameNodeCommand"> Game node command to copy construct from. </param>
		GameNodeCommand(const GameNodeCommand& gameNodeCommand) = delete;

		/// <summary> Game node command move constructor. </summary>
		/// <param name="gameNodeCommand"> Game node command to move construct from. </param>
		GameNodeCommand(GameNodeCommand&& gameNodeCommand) = delete;

		/// <summary> Game node command copy assignment operator. </summary>
		/// <param name="gameNodeCommand"> Game node command to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned game node command. </returns>
		GameNodeCommand& operator=(const GameNodeCommand& gameNodeCommand) = delete;

		/// <summary> Game node command move assignment operator. </summary>
		/// <param name="gameNodeCommand"> The r-value reference to the game node command to move assign from. </param>
		/// <returns> The reference to the newly move-assigned game node command. </returns>
		GameNodeCommand& operator=(GameNodeCommand&& gameNodeCommand) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary> Executes the operation on the specified game node <paramref name="gameNode"/>. </summary>
		/// <param name="gameNode"> The game node the command will be performed for. </param>
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
		ENGINE_API void Execute(GameNode* gameNode) const override
		{
			DEBUG_LOG_ENGINE("Do-nothing command started. Remember that executing a DO-NOTHING command wastes precious CPU time.");
			// Do nothing
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class EmptyGameNodeCommand */

} /* end namespace engine */

#endif /* __ENGINE_GAME_NODE_COMMAND_H__ */