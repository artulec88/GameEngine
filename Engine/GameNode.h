#ifndef __ENGINE_GAME_NODE_H__
#define __ENGINE_GAME_NODE_H__

#include "Engine.h"
#include "IRenderable.h"
#include "IActionHandler.h"
#include "IStateHandler.h"
#include "IInputableMouse.h"
#include "ActionConstants.h"
#include "IUpdateable.h"
#include "GameNodeCommand.h"

#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"

#include "Math/Transform.h"

#include "Physics/PhysicsObject.h"

#include <vector>

namespace engine
{

	class GameComponent;

	class GameNode : public IActionHandler, public IStateHandler, public input::IInputableMouse, public IUpdateable, public IRenderable
	{
		typedef std::map<actions::Action, GameNodeCommand*> ActionsToGameNodeCommands;
		typedef std::map<states::State, GameNodeCommand*> StatesToGameNodeCommands;
		/* ==================== Static variables and functions begin ==================== */
		/// <summary>
		/// The number of entities currently being maintained in the application.
		/// </summary>
		static int s_gameNodeCount;
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Game node constructor. </summary>
		ENGINE_API GameNode();

		/// <summary> Game node destructor. </summary>
		ENGINE_API ~GameNode();

		/// <summary> Game node copy constructor. </summary>
		/// <param name="gameNode"> The game node to copy construct from. </param>
		GameNode(const GameNode& gameNode) = delete;
		
		/// <summary> Game node move constructor. </summary>
		/// <param name="gameNode"> The game node to move construct from. </param>
		ENGINE_API GameNode(GameNode&& gameNode) noexcept;

		/// <summary> Game node copy assignment operator. </summary>
		/// <param name="gameNode"> The game node to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned game node. </returns>
		GameNode& operator=(const GameNode& gameNode) = delete;

		/// <summary> Game node move assignment operator. </summary>
		/// <param name="gameNode"> The game node to move assign from. </param>
		/// <returns> The reference to the newly move-assigned game node. </returns>
		ENGINE_API GameNode& operator=(GameNode&& gameNode) noexcept;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API GameNode Clone() const;
		ENGINE_API int GetId() const { return m_id; } // just for easier debugging purposes
		ENGINE_API GameNode* AddChild(GameNode* child);
		ENGINE_API GameNode* AddComponent(GameComponent* component);

		ENGINE_API void MouseButtonEvent(int button, int action, int mods) override;
		ENGINE_API void MousePosEvent(double xPos, double yPos) override;
		ENGINE_API void ScrollEvent(double xOffset, double yOffset) override;

		//ENGINE_API void InputAll(math::Real delta);
		ENGINE_API void Update(math::Real elapsedTime) override;
		ENGINE_API void Render(int shaderId, rendering::Renderer* renderer) const override;
		ENGINE_API void RegisterCommandForAction(actions::Action action, GameNodeCommand* gameNodeCommand)
		{
			m_actionsToCommands.insert(std::make_pair(action, gameNodeCommand));
		}
		ENGINE_API void Handle(actions::Action action) override
		{
			CRITICAL_LOG_ENGINE("Executing action ", action, " for entity ", m_id);
			const auto actionToCommandItr = m_actionsToCommands.find(action);
			if (actionToCommandItr != m_actionsToCommands.end())
			{
				actionToCommandItr->second->Execute(this);
			}
		}
		ENGINE_API void Handle(states::State state) override
		{
			CRITICAL_LOG_ENGINE("Handling the state ", state, " for entity ", m_id);
			const auto stateToCommandItr = m_statesToCommands.find(state);
			if (stateToCommandItr != m_statesToCommands.end())
			{
				stateToCommandItr->second->Execute(this);
			}
		}

		ENGINE_API math::Transform& GetTransform() { return m_transform; }
		ENGINE_API const math::Transform& GetTransform() const { return m_transform; }

		ENGINE_API physics::PhysicsObject* GetPhysicsObject() const { return m_physicsObject.get(); }
		ENGINE_API void CreatePhysicsObject(math::Real mass, const math::Vector3D& linearVelocity);

		ENGINE_API std::vector<GameNode*> GetAllDescendants() const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/// <summary>
		/// The unique ID of an entity. It simplifies the debugging.
		/// </summary>
		int m_id;
		std::vector<GameNode*> m_childrenGameNodes;
		std::vector<GameComponent*> m_components;
		std::vector<IRenderable*> m_renderableComponents;
		std::vector<IInputableMouse*> m_inputableMouseComponents;
		std::vector<IUpdateable*> m_updateableComponents;
		math::Transform m_transform;
		std::unique_ptr<physics::PhysicsObject> m_physicsObject;
		ActionsToGameNodeCommands m_actionsToCommands;
		StatesToGameNodeCommands m_statesToCommands;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameNode */

} /* end namespace engine */

#endif // __ENGINE_GAME_NODE_H__