#ifndef __GAME_START_GAME_COMMAND_H__
#define __GAME_START_GAME_COMMAND_H__

#include "Engine/GameCommand.h"

namespace game
{

	class StartGameCommand : public engine::GameCommand
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Starting game command default constructor. </summary>
		StartGameCommand();

		/// <summary> Starting game command destructor. </summary>
		virtual ~StartGameCommand();

		/// <summary> Starting game command copy constructor. </summary>
		/// <param name="startGameCommand"> The reference to starting game command object to copy construct from. </param>
		StartGameCommand(const StartGameCommand& startGameCommand) = delete;

		/// <summary> Starting game command move constructor. </summary>
		/// <param name="startGameCommand"> The r-value reference to starting game command object to move construct from. </param>
		StartGameCommand(StartGameCommand&& startGameCommand) = delete;

		/// <summary> Starting game command copy assignment operator. </summary>
		/// <param name="startGameCommand"> The reference to starting game command object to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned starting game command object. </returns>
		StartGameCommand& operator=(const StartGameCommand& startGameCommand) = delete;

		/// <summary> Starting game command move assignment operator. </summary>
		/// <param name="startGameCommand"> The r-value reference to starting game command object to move assign from. </param>
		/// <returns> The reference to the newly move-assigned starting game command object. </returns>
		StartGameCommand& operator=(StartGameCommand&& startGameCommand) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Execute(engine::GameManager* gameManager) const override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class StartGameCommand */

} /* end namespace game */

#endif /* __GAME_START_GAME_COMMAND_H__ */