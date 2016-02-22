#ifndef __ENGINE_GAME_STATE_MANAGER_H__
#define __ENGINE_GAME_STATE_MANAGER_H__

#include "Engine.h"
#include "GameState.h"
#include "Rendering\IInputableKeyboard.h"
#include "Rendering\IInputableMouse.h"
#include "Rendering\IRenderable.h"
#include "Rendering\IUpdateable.h"
#include "Math\Math.h"
#include <vector>
#include <stack>

namespace Engine
{
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

/// <summary>
/// Interface for a stack-based game state manager.
/// </summary>
class GameStateManager : public virtual Rendering::Input::IInputableKeyboard, public virtual Rendering::Input::IInputableMouse, public virtual Rendering::IRenderable, public virtual Rendering::IUpdateable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	ENGINE_API GameStateManager();
	/// <summary>
	/// Destroys the game state manager, leaving and dropping any active game state.
	/// </summary>
	ENGINE_API virtual ~GameStateManager(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/// <summary> Replaces the lastmost game state on the stack </summary>
    /// <param name="state">State the lastmost state on the stack will be replaced with</param>
    /// <param name="modality">Whether the state completely obscures the state below it</param>
    /// <returns>The previously lastmost state on the stack that was replaced</returns>
    /// <remarks>
    ///   This method is mostly just syntactic sugar for a call to Pop() followed by Push(),
    ///   except that it will also work if the game state stack is currently empty, in which
    ///   case it will equal the Push() method and return an empty smart pointer.
    /// </remarks>
	virtual GameState* Switch(GameState* gameState, GameStateModality::ModalityType modality = GameStateModality::EXCLUSIVE);

	/// <summary> Removes the lastmost game state from the stack </summary>
	/// <returns> The state that has been removed from the stack </returns>
	virtual GameState* Pop() = 0;

	/// <summary> Appends a new game state to the stack </summary>
	/// <param name="gameState"> Game state that will be pushed onto the stack </param>
	/// <param name="modality"> Indicates whether the state completely obscures the state below it </param>
	virtual void Push(GameState* gameState, GameStateModality::ModalityType modality = GameStateModality::EXCLUSIVE) = 0;

	/// <summary>
	/// Returns the currently active game state
	/// </summary>
	/// <returns> The lastmost game state on the stack </returns>
	virtual GameState* Peek() const = 0;

	/// <summary> Collects keyboard key input according to current game state </summary>
	/// <param name="key"> The key that triggered the event </param>
	/// <param name="scancode"> The system-specific scancode of the key </param>
	/// <param name="action"> The action (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT) </param>
	/// <param name="mods"> Bit field describing which modifier keys were held down </param>
	virtual void KeyEvent(int key, int scancode, int action, int mods) = 0;

	virtual void ScrollEvent(double xOffset, double yOffset) = 0;

	/// <summary> Collects mouse button input according to current game state </summary>
	/// <param name="key"> The mouse button that triggered the event </param>
	/// <param name="action"> The action (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT) </param>
	/// <param name="mods"> Bit field describing which modifier keys were held down </param>
	virtual void MouseButtonEvent(int button, int action, int mods) = 0;

	virtual void MousePosEvent(double xPos, double yPos) = 0;

	/// <summary> Advances the time of the active game states </summary>
	/// <param name="deltaTime"> Elapsed simulation time </param>
	virtual void Update(Math::Real deltaTime) = 0;

	/// <summary> Instructs the active game states to render themselves or to update the scene graph </summary>
	/// <param name="shader"> The shader to be used for rendering. </param>
	/// <param name="renderer"> The rendering engine. </param>
	virtual void Render(Rendering::Shader* shader, Rendering::Renderer* renderer) const = 0;
	
	bool IsInGameTimeCalculationEnabled() const;

	void SetTransition(GameStateTransitioning::GameStateTransition* gameStateTransition);
	void PerformStateTransition();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	/// <summary> The variable holding the game state to which the game state manager will transition </summary>
	GameStateTransitioning::GameStateTransition* m_gameStateTransition;
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
private:
	typedef std::pair<GameState*, GameStateModality::ModalityType> GameStateModalityTypePair;
/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary> Initializes a new game state manager </summary>
	ENGINE_API DefaultGameStateManager();

	/// <summary> Destroys the game state manager, leaving and dropping any active game state. </summary>
	ENGINE_API virtual ~DefaultGameStateManager(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	/// <summary> Removes the lastmost game state from the stack </summary>
	/// <returns> The state that has been removed from the stack </returns>
	virtual GameState* Pop();

	/// <summary> Appends a new game state to the stack </summary>
	/// <param name="gameState"> Game state that will be pushed onto the stack </param>
	/// <param name="modality"> Indicates whether the state completely obscures the state below it </param>
	virtual void Push(GameState* gameState, GameStateModality::ModalityType modality = GameStateModality::EXCLUSIVE);

	/// <summary>
	/// Returns the currently active game state
	/// </summary>
	/// <returns> The lastmost game state on the stack </returns>
	virtual GameState* Peek() const;

	/// <summary> Collects input according to current game state </summary>
	/// <param name="key"> The key that triggered the event </param>
	/// <param name="scancode"> The system-specific scancode of the key </param>
	/// <param name="action"> The action (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT) </param>
	/// <param name="mods"> Bit field describing which modifier keys were held down </param>
	virtual void KeyEvent(int key, int scancode, int action, int mods);

	virtual void ScrollEvent(double xOffset, double yOffset);

	virtual void MouseButtonEvent(int button, int action, int mods);
	virtual void MousePosEvent(double xPos, double yPos);

	/// <summary> Advances the time of the active game states </summary>
	/// <param name="deltaTime"> Elapsed simulation time </param>
	void Update(Math::Real deltaTime);

	/// <summary> Instructs the active game states to render themselves or to update the scene graph </summary>
	/// <param name="shader"> The shader to be used for rendering. </param>
	/// <param name="renderer"> The rendering engine. </param>
	void Render(Rendering::Shader* shader, Rendering::Renderer* renderer) const;
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
private:
	std::vector<GameStateModalityTypePair> m_activeStates;
	std::vector<Rendering::Input::IInputableKeyboard*> m_exposedInputablesKeyboard;
	std::vector<Rendering::Input::IInputableMouse*> m_exposedInputablesMouse;
	std::vector<Rendering::IRenderable*> m_exposedRenderables;
	std::vector<Rendering::IUpdateable*> m_exposedUpdateables;
/* ==================== Non-static member variables end ==================== */
}; /* end class DefaultGameStateManager */

} /* end namespace Engine */

#endif /* __ENGINE_GAME_STATE_MANAGER_H__ */
