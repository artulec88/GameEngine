#ifndef __GAME_SAVE_GAME_COMMAND_H__
#define __GAME_SAVE_GAME_COMMAND_H__

#include "Engine/GameCommand.h"

namespace game
{

	class SaveGameCommand : public engine::GameCommand
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Saving game command default constructor. </summary>
		SaveGameCommand();

		/// <summary> Saving game command destructor. </summary>
		virtual ~SaveGameCommand();

		/// <summary> Saving game command copy constructor. </summary>
		/// <param name="saveGameCommand"> The reference to saving game command object to copy construct from. </param>
		SaveGameCommand(const SaveGameCommand& saveGameCommand) = delete;

		/// <summary> Saving game command move constructor. </summary>
		/// <param name="saveGameCommand"> The r-value reference to saving game command object to move construct from. </param>
		SaveGameCommand(SaveGameCommand&& saveGameCommand) = delete;

		/// <summary> Saving game command copy assignment operator. </summary>
		/// <param name="saveGameCommand"> The reference to saving game command object to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned saving game command object. </returns>
		SaveGameCommand& operator=(const SaveGameCommand& saveGameCommand) = delete;

		/// <summary> Saving game command move assignment operator. </summary>
		/// <param name="saveGameCommand"> The r-value reference to saving game command object to move assign from. </param>
		/// <returns> The reference to the newly move-assigned saving game command object. </returns>
		SaveGameCommand& operator=(SaveGameCommand&& saveGameCommand) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Execute(engine::GameManager* gameManager) const override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SaveGameCommand */

} /* end namespace game */

#endif /* __GAME_SAVE_GAME_COMMAND_H__ */