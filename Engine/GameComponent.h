#ifndef __ENGINE_GAME_COMPONENT_H__
#define __ENGINE_GAME_COMPONENT_H__

#include "Engine.h"
#include "GameNode.h"
//#include "InputConstants.h"
//#include "GameNodeCommand.h"

#include "Math/Transform.h"

//#include <map>

namespace engine
{
	/// <summary>
	/// The base class for all game components. Game components are the actual logic that one can perform on the entity.
	/// </summary>
	class GameComponent //: public GameNodeCommand
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Game component default constructor.
		/// </summary>
		GameComponent();

		/// <summary>
		/// Game component destructor.
		/// </summary>
		virtual ~GameComponent();

		/// <summary>
		/// Game component copy constructor.
		/// </summary>
		/// <param name="gameComponent> The game component we want to copy construct from. </param>
		GameComponent(const GameComponent& gameComponent) = delete;

		/// <summary>
		/// Game component move constructor.
		/// </summary>
		/// <param name="gameComponent> The game component we want to move construct from. </param>
		GameComponent(GameComponent&& gameComponent) = default;

		/// <summary>
		/// Game component copy assignment operator.
		/// </summary>
		/// <param name="gameComponent> The game component we want to copy assign from. </param>
		/// <returns> The game component we assigned new values to. </returns>
		GameComponent& operator=(const GameComponent& gameComponent) = delete;

		/// <summary>
		/// Game component move assignment operator.
		/// </summary>
		/// <param name="gameComponent> The game component we want to move assign from. </param>
		/// <returns> The game component we assigned new values to. </returns>
		GameComponent& operator=(GameComponent&& gameComponent) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		//virtual void Execute() const = 0;

		/// <summary>
		/// Sets the parent entity for the game component.
		/// </summary>
		/// <param name="parentGameNode"> The game node this component is attached to. </param>
		void SetParent(GameNode* parentGameNode);

		/// <summary>
		/// Retrieves the transform of the parent game node.
		/// </summary>
		/// <returns> The modifiable transform of the parent game node. </returns>
		math::Transform& GetTransform();

		/// <summary>
		/// Retrieves the transform of the parent game node.
		/// </summary>
		/// <returns> The unmodifiable transform of the parent game node. </returns>
		const math::Transform& GetTransform() const;

		physics::PhysicsObject* GetPhysicsObject() const { return m_parentGameNode->GetPhysicsObject(); }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		GameNode* m_parentGameNode;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameComponent */

} /* end namespace engine */

#endif // __ENGINE_GAME_COMPONENT_H__