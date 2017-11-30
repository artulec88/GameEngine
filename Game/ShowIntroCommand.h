#ifndef __GAME_SHOW_INTRO_COMMAND_H__
#define __GAME_SHOW_INTRO_COMMAND_H__

#include "Engine/GameCommand.h"

namespace game
{

	class ShowIntroCommand : public engine::GameCommand
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> "Show intro" game command default constructor. </summary>
		ShowIntroCommand();

		/// <summary> "Show intro" game command destructor. </summary>
		virtual ~ShowIntroCommand();

		/// <summary> "Show intro" game command copy constructor. </summary>
		/// <param name="showIntroCommand"> The reference to "Show intro" game command object to copy construct from. </param>
		ShowIntroCommand(const ShowIntroCommand& showIntroCommand) = delete;

		/// <summary> "Show intro" game command move constructor. </summary>
		/// <param name="showIntroCommand"> The r-value reference to "Show intro" game command object to move construct from. </param>
		ShowIntroCommand(ShowIntroCommand&& showIntroCommand) = delete;

		/// <summary> "Show intro" game command copy assignment operator. </summary>
		/// <param name="showIntroCommand"> The reference to "Show intro" game command object to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned "Show intro" game command object. </returns>
		ShowIntroCommand& operator=(const ShowIntroCommand& showIntroCommand) = delete;

		/// <summary> "Show intro" game command move assignment operator. </summary>
		/// <param name="showIntroCommand"> The r-value reference to "Show intro" game command object to move assign from. </param>
		/// <returns> The reference to the newly move-assigned "Show intro" game command object. </returns>
		ShowIntroCommand& operator=(ShowIntroCommand&& showIntroCommand) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Execute(engine::GameManager* gameManager) const override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ShowIntroCommand */

} /* end namespace game */

#endif /* __GAME_SHOW_INTRO_COMMAND_H__ */