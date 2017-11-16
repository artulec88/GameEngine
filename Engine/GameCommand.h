#ifndef __ENGINE_GAME_COMMAND_H__
#define __ENGINE_GAME_COMMAND_H__

#include "Engine.h"
#include "Utility/ILogger.h"

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
		/// <summary> Base game command constructor. </summary>
		ENGINE_API GameCommand();

		/// <summary> Base game command destructor. </summary>
		ENGINE_API virtual ~GameCommand();

		/// <summary> Base game command copy constructor. </summary>
		/// <param name="gameCommand"> Game command to copy construct from. </param>
		GameCommand(const GameCommand& gameCommand) = delete;

		/// <summary> Base game command move constructor. </summary>
		/// <param name="gameCommand"> Game command to move construct from. </param>
		GameCommand(GameCommand&& gameCommand) = delete;

		/// <summary> Base game command copy assignment operator. </summary>
		/// <param name="gameCommand"> Game command to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned base game command. </returns>
		GameCommand& operator=(const GameCommand& gameCommand) = delete;

		/// <summary> Base game command move assignment operator. </summary>
		/// <param name="gameCommand"> Game command to move assign from. </param>
		/// <returns> The reference to the newly move-assigned base game command. </returns>
		GameCommand& operator=(GameCommand&& gameCommand) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary> Executes the game command. </summary>
		/// <param name="gameManager"> The game manager the game command will performed in. </param>
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
		ENGINE_API void Execute(GameManager* gameManager) const override
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