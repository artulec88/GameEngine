#ifndef __ENGINE_GAME_STATE_H__
#define __ENGINE_GAME_STATE_H__

#include "Engine.h"
//#include "IInputable.h"
//#include "IRenderable.h"
//#include "IUpdateable.h"
#include "Utility\Utility.h"

namespace Engine
{
	///<summary>
	/// The base class for each game state. It provides functionality for game states lifecycles.
	/// </summary>
	class GameState
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API GameState()
		{
		}

		ENGINE_API virtual ~GameState(void)
		{
		}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>The function invoked after the current game state has been placed in the game state manager.</summary>
		ENGINE_API virtual void Entered() = 0;

		/// <summary>The function invoked before the current game state is removed game state manager.</summary>
		ENGINE_API virtual void Leaving() = 0;

		/// <summary>The function invoked right before another game state is stacked on top of the current one.</summary>
		ENGINE_API virtual void Obscuring() = 0;

		/// <summary>The function invoked right after the game state has become the topmost game state on the stack again.</summary>
		ENGINE_API virtual void Revealed() = 0;

		/// <summary>The function indicating whether the current game state wants in-game time calculation to be performed or not.</summary>
		ENGINE_API virtual bool IsInGameTimeCalculationEnabled() const { return false; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameState */

	namespace GameStateModality
	{
		/// <summary>
		/// Possible behaviors of a game state in respect to the states below it on the stack
		/// </summary>
		enum ModalityType
		{
			/// <summary>
			/// The game state takes exclusive ownership of the screen does not require the state
			/// below it in the stack to be updated as long as it's active.
			/// </summary>
			EXCLUSIVE,

			/// <summary>
			/// The game state sits on top of the state below it in the stack, but does
			/// not completely obscure it or requires it to continue being updated.
			/// </summary>
			POPUP
		}; /* end enum ModalityType */
	} /* end namespace GameStateModality */

	namespace GameStateTransitioning
	{
		enum TransitionType
		{
			SWITCH = 0,
			PUSH
		}; /* end enum TransitionType */

		class GameStateTransition
		{
		/* ==================== Constructors and destructors begin ==================== */
		public:
			ENGINE_API GameStateTransition(GameState* gameState, TransitionType transitionType, GameStateModality::ModalityType modalityType) :
				m_transitionType(transitionType),
				m_gameState(gameState),
				m_modalityType(modalityType)
			{
			}

			ENGINE_API ~GameStateTransition()
			{
				SAFE_DELETE(m_gameState);
			}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			ENGINE_API GameState* GetGameState() const { return m_gameState; }
			ENGINE_API TransitionType GetTransitionType() const { return m_transitionType; }
			ENGINE_API GameStateModality::ModalityType GetModalityType() const { return m_modalityType; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		private:
			GameState* m_gameState;
			TransitionType m_transitionType;
			GameStateModality::ModalityType m_modalityType;
		/* ==================== Non-static member functions begin ==================== */
		}; /* end class GameStateTransition */
	} /* end namespace GameStateTransitioning */

} /* end namespace Engine */

#endif /* __ENGINE_GAME_STATE_H__ */
