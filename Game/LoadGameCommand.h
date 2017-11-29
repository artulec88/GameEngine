#ifndef __GAME_LOAD_GAME_COMMAND_H__
#define __GAME_LOAD_GAME_COMMAND_H__

#include "Engine/GameCommand.h"

namespace game
{
	/// <summary>
	/// The game command responsible for loading the previously saved game.
	/// </summary>
	class LoadGameCommand : public engine::GameCommand
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Loading game command default constructor. </summary>
		LoadGameCommand();

		/// <summary> Loading game command destructor. </summary>
		virtual ~LoadGameCommand();

		/// <summary> Loading game command copy constructor. </summary>
		/// <param name="loadGameCommand"> The reference to loading game command to copy construct from. </param>
		LoadGameCommand(const LoadGameCommand& loadGameCommand) = delete;

		/// <summary> Loading game command move constructor. </summary>
		/// <param name="loadGameCommand"> The r-value reference to loading game command to move construct from. </param>
		LoadGameCommand(LoadGameCommand&& loadGameCommand) = delete;

		/// <summary> Loading game command copy assignment operator. </summary>
		/// <param name="loadGameCommand"> The reference to loading game command to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned loading game command. </returns>
		LoadGameCommand& operator=(const LoadGameCommand& loadGameCommand) = delete;

		/// <summary> Loading game command move assignment operator. </summary>
		/// <param name="loadGameCommand"> The r-value reference to loading game command to move assign from. </param>
		/// <returns> The reference to the newly move-assigned loading game command. </returns>
		LoadGameCommand& operator=(LoadGameCommand&& loadGameCommand) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Execute(engine::GameManager* gameManager) const override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LoadGameCommand */

} /* end namespace Game */

#endif /* __GAME_LOAD_GAME_COMMAND_H__ */