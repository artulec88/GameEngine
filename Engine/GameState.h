#ifndef __ENGINE_GAME_STATE_H__
#define __ENGINE_GAME_STATE_H__

#include "Engine.h"
#include "IActionHandler.h"
#include "IStateHandler.h"
#include "IRangeHandler.h"

#include "Rendering/Renderer.h"

#include "Utility/Utility.h"

#include <string>

//namespace rendering
//{
//	class Renderer;
//} /* end namespace rendering */

namespace engine
{
	/// <summary>
	/// The base class for each game state. It provides functionality for game states lifecycles.
	/// </summary>
	class GameState : public IActionHandler, public IStateHandler, public IRangeHandler
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Game state constructor. </summary>
		/// <param name="inputMappingContextName"> The name of the input mapping context the game state should use. </param>
		ENGINE_API explicit GameState(const std::string& inputMappingContextName) :
			m_inputMappingContextName(inputMappingContextName)
		{
		}

		/// <summary> Game state destructor. </summary>
		ENGINE_API virtual ~GameState()
		{
		}

		/// <summary> Game state copy constructor. </summary>
		/// <param name="gameState"> The reference to game state to copy construct from. </param>
		GameState(const GameState& gameState) = delete;

		/// <summary> Game state move constructor. </summary>
		/// <param name="gameState"> The r-value reference to game state to move construct from. </param>
		GameState(GameState&& gameState) = delete;

		/// <summary> Game state copy assignment operator. </summary>
		/// <param name="gameState"> The reference to game state to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned game state. </returns>
		GameState& operator=(const GameState& gameState) = delete;

		/// <summary> Game state move assignment operator. </summary>
		/// <param name="gameState"> The r-value reference to game state to move assign from. </param>
		/// <returns> The reference to the newly move-assigned game state. </returns>
		GameState& operator=(GameState&& gameState) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		using IActionHandler::Handle;
		using IStateHandler::Handle;
		using IRangeHandler::Handle;

		/// <summary>
		/// The function invoked after the current game state has been placed in the game state manager.
		/// </summary>
		ENGINE_API virtual void Entered() = 0;

		/// <summary>
		/// The function invoked before the current game state is removed game state manager.
		/// </summary>
		ENGINE_API virtual void Leaving() = 0;

		/// <summary>
		/// The function invoked right before another game state is stacked on top of the current one.
		/// </summary>
		ENGINE_API virtual void Obscuring() = 0;

		/// <summary>
		/// The function invoked right after the game state has become the topmost game state on the stack again.
		/// </summary>
		ENGINE_API virtual void Revealed() = 0;

		/// <summary>
		/// Renders the game state.
		/// </summary>
		ENGINE_API virtual void Render(rendering::Renderer* renderer) const = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/// <summary>
		/// The name of the input mapping context. It is used for switching between one input context and the other.
		/// E.g. we want different input mapping when main game menu is displayed from the one when the game is actually being played.
		/// </summary>
		const std::string m_inputMappingContextName;
		
		// TODO: The game state should also be able to respond to actions using the game commands instead of a HandleAction(...) function.
		// std::map<Engine::Input::Actions::Action, Engine::GameCommand*> m_actionsToCommandsMap;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameState */

	namespace game_state_modality
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
	} /* end namespace game_state_modality */

	namespace game_state_transitioning
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
			ENGINE_API GameStateTransition(GameState* gameState, TransitionType transitionType, game_state_modality::ModalityType modalityType) :
				m_gameState(gameState),
				m_transitionType(transitionType),
				m_modalityType(modalityType)
			{
			}

			ENGINE_API ~GameStateTransition()
			{
				SAFE_DELETE(m_gameState);
			}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
			ENGINE_API GameState* GetGameState() const { return m_gameState; }
			ENGINE_API TransitionType GetTransitionType() const { return m_transitionType; }
			ENGINE_API game_state_modality::ModalityType GetModalityType() const { return m_modalityType; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		private:
			GameState* m_gameState;
			TransitionType m_transitionType;
			game_state_modality::ModalityType m_modalityType;
		/* ==================== Non-static member functions begin ==================== */
		}; /* end class GameStateTransition */
	} /* end namespace game_state_transitioning */

} /* end namespace engine */

#endif /* __ENGINE_GAME_STATE_H__ */
