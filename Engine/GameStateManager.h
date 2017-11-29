#ifndef __ENGINE_GAME_STATE_MANAGER_H__
#define __ENGINE_GAME_STATE_MANAGER_H__

#include "Engine.h"
#include "GameState.h"
#include "ActionConstants.h"
#include "IInputableMouse.h"
#include "IUpdateable.h"

#include "Rendering/Shader.h"
#include "Rendering/Renderer.h"

#include "Math/Math.h"

#include "Utility/Utility.h"

#include <vector>

namespace engine
{
	/// <summary>
	/// Interface for a stack-based game state manager.
	/// </summary>
	class GameStateManager : public IUpdateable, public IActionHandler, public IStateHandler, public IRangeHandler
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		GameStateManager();
		
		/// <summary>
		/// Destroys the game state manager, leaving and dropping any active game state.
		/// </summary>
		virtual ~GameStateManager();

		/// <summary> Game state manager copy constructor. </summary>
		/// <param name="gameStateManager"> Game state manager to copy construct from. </param>
		GameStateManager(const GameStateManager& gameStateManager) = delete;

		/// <summary> Game state manager move constructor. </summary>
		/// <param name="gameStateManager"> Game state manager to move construct from. </param>
		GameStateManager(GameStateManager&& gameStateManager) = delete;

		/// <summary> Game state manager copy assignment operator. </summary>
		/// <param name="gameStateManager"> Game state manager to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned game state manager. </returns>
		GameStateManager& operator=(const GameStateManager& gameStateManager) = delete;

		/// <summary> Game state manager move assignment operator. </summary>
		/// <param name="gameStateManager"> The r-value reference to the game state manager to move assign from. </param>
		/// <returns> The reference to the newly move-assigned game state manager. </returns>
		GameStateManager& operator=(GameStateManager&& gameStateManager) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		using IActionHandler::Handle;
		using IStateHandler::Handle;
		using IRangeHandler::Handle;

		/// <summary> Replaces the lastmost game state on the stack. </summary>
		/// <param name="state"> State the lastmost state on the stack will be replaced with. </param>
		/// <param name="modality"> Whether the state completely obscures the state below it. </param>
		/// <returns> The previously lastmost state on the stack that was replaced. </returns>
		/// <remarks>
		/// This method is mostly just syntactic sugar for a call to Pop() followed by Push(),
		/// except that it will also work if the game state stack is currently empty, in which
		/// case it will equal the Push() method and return an empty smart pointer.
		/// </remarks>
		virtual GameState* Switch(GameState* gameState, game_state_modality::ModalityType modality = game_state_modality::EXCLUSIVE);

		/// <summary> Removes the lastmost game state from the stack </summary>
		/// <returns> The state that has been removed from the stack </returns>
		virtual GameState* Pop() = 0;

		/// <summary> Appends a new game state to the stack </summary>
		/// <param name="gameState"> Game state that will be pushed onto the stack </param>
		/// <param name="modality"> Indicates whether the state completely obscures the state below it </param>
		virtual void Push(GameState* gameState, game_state_modality::ModalityType modality = game_state_modality::EXCLUSIVE) = 0;

		/// <summary>
		/// Returns the currently active game state
		/// </summary>
		/// <returns> The lastmost game state on the stack </returns>
		virtual GameState* Peek() const = 0;

		virtual void ScrollEvent(double xOffset, double yOffset) = 0;

		/// <summary> Collects mouse button input according to current game state </summary>
		/// <param name="key"> The mouse button that triggered the event </param>
		/// <param name="action"> The action (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT) </param>
		/// <param name="mods"> Bit field describing which modifier keys were held down </param>
		virtual void MouseButtonEvent(int button, int action, int mods) = 0;

		virtual void MousePosEvent(double xPos, double yPos) = 0;

		/// <summary> Instructs the active game states to render themselves or to update the scene graph </summary>
		/// <param name="renderer"> The rendering engine. </param>
		virtual void Render(rendering::Renderer* renderer) const = 0;

		void SetTransition(game_state_transitioning::GameStateTransition* gameStateTransition);
		void PerformStateTransition();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary> The variable holding the game state to which the game state manager will transition </summary>
		game_state_transitioning::GameStateTransition* m_gameStateTransition;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameStateManager */

	/// <summary>Stacked game state manager that forwards Draw() and Update() calls</summary>
	/// <remarks>
	///   <para>
	///     This game state manager will call the Updateable.Update() and Drawable.Draw() methods
	///     when any of those interfaces are implemented by the game states pushed onto its stack.
	///   </para>
	///   <para>
	///     To figure out whether a game state implements these interfaces, a dynamic_cast will be
	///     performed during the Push() call. All active updateables and drawables are then kept
	///     in separate list, allowing very cheap frame-by-frame processing.
	///   </para>
	/// </remarks>
	class DefaultGameStateManager : public GameStateManager
	{
		using GameStateModalityTypePair = std::pair<GameState*, game_state_modality::ModalityType>;
		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Initializes a new game state manager </summary>
		DefaultGameStateManager();

		/// <summary> Destroys the game state manager, leaving and dropping any active game state. </summary>
		virtual ~DefaultGameStateManager();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary> Removes the lastmost game state from the stack </summary>
		/// <returns> The state that has been removed from the stack </returns>
		GameState* Pop() override;

		/// <summary> Appends a new game state to the stack </summary>
		/// <param name="gameState"> Game state that will be pushed onto the stack </param>
		/// <param name="modality"> Indicates whether the state completely obscures the state below it </param>
		void Push(GameState* gameState, game_state_modality::ModalityType modality = game_state_modality::EXCLUSIVE) override;

		/// <summary>
		/// Returns the currently active game state
		/// </summary>
		/// <returns> The lastmost game state on the stack </returns>
		GameState* Peek() const override;

		void ScrollEvent(double xOffset, double yOffset) override;

		void MouseButtonEvent(int button, int action, int mods) override;
		void MousePosEvent(double xPos, double yPos) override;

		/// <summary> Advances the time of the active game states </summary>
		/// <param name="deltaTime"> Elapsed simulation time </param>
		void Update(math::Real deltaTime) override;

		/// <summary> Instructs the active game states to render themselves or to update the scene graph </summary>
		/// <param name="renderer"> The rendering engine. </param>
		void Render(rendering::Renderer* renderer) const override;

		/// <summary>
		/// Handles the incoming action appropriately.
		/// </summary>
		/// <param name="action"> The action that must be handled by active game state. </param>
		void Handle(actions::Action action) override;

		/// <summary>
		/// Handles the incoming state appropriately.
		/// </summary>
		/// <param name="state"> The state that must be handled by active game states. </param>
		void Handle(states::State state) override;

		/// <summary>
		/// Handles the incoming range appropriately.
		/// </summary>
		/// <param name="range"> The range that must be handled by active game states. </param>
		/// <param name="value"> The value associated with the specified range. </param>
		void Handle(ranges::Range range, math::Real value) override;
	private:
		/// <summary>
		///   Adds the specified game state to the exposed Drawables or Updateables if it
		///   implements the Drawable or Updateable interfaces
		/// </summary>
		/// <param name="gameState">
		///   State that will be checked for implementing the Drawable or Updateable interfaces
		/// </param>
		void AddToInterfaces(GameState* gameState);

		/// <summary> Clears all interface lists </summary>
		void ClearAllIntefaceLists();

		/// <summary>
		///   Removes the specified game state to the exposed Drawables or Updateables if it
		///   implements the Drawable or Updateable interfaces
		/// </summary>
		/// <param name="gameState">
		///   State that will be checked for implementing the Drawable or Updateable interfaces
		/// </param>
		void RemoveFromInterfaces(GameState *gameState);

		/// <summary>
		///   Rebuilds the separate updateable and drawable queues when an Hiding state has
		///   been popped from the stack
		/// </summary>
		void RebuildInterfaceQueues();

		/// <summary>Notifies all previously exposed states that they have been obscured</summary>
		void NotifyObscuredStates();

		/// <summary>Notifies all currently exposed states that they have been revealed</summary>
		void NotifyRevealedStates();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		std::vector<GameStateModalityTypePair> m_activeStates;
		std::vector<input::IInputableMouse*> m_exposedInputablesMouse;
		std::vector<IUpdateable*> m_exposedUpdateables;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class DefaultGameStateManager */

} /* end namespace engine */

#endif /* __ENGINE_GAME_STATE_MANAGER_H__ */
