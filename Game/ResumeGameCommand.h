#ifndef __GAME_RESUME_GAME_COMMAND_H__
#define __GAME_RESUME_GAME_COMMAND_H__

#include "Engine/GameCommand.h"

namespace game
{

	class ResumeGameCommand : public engine::GameCommand
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Resuming game command default constructor. </summary>
		ResumeGameCommand();

		/// <summary> Resuming game command destructor. </summary>
		virtual ~ResumeGameCommand();

		/// <summary> Resuming game command copy constructor. </summary>
		/// <param name="resumeGameCommand"> The reference to resuming game command object to copy construct from. </param>
		ResumeGameCommand(const ResumeGameCommand& resumeGameCommand) = delete;

		/// <summary> Resuming game command move constructor. </summary>
		/// <param name="resumeGameCommand"> The r-value reference to resuming game command object to move construct from. </param>
		ResumeGameCommand(ResumeGameCommand&& resumeGameCommand) = delete;

		/// <summary> Resuming game command copy assignment operator. </summary>
		/// <param name="resumeGameCommand"> The reference to resuming game command object to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned resuming game command object. </returns>
		ResumeGameCommand& operator=(const ResumeGameCommand& resumeGameCommand) = delete;

		/// <summary> Resuming game command move assignment operator. </summary>
		/// <param name="resumeGameCommand"> The r-value reference to resuming game command object to move assign from. </param>
		/// <returns> The reference to the newly move-assigned resuming game command object. </returns>
		ResumeGameCommand& operator=(ResumeGameCommand&& resumeGameCommand) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Execute(engine::GameManager* gameManager) const override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ResumeGameCommand */

} /* end namespace game */

#endif /* __GAME_RESUME_GAME_COMMAND_H__ */