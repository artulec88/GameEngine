#ifndef __ENGINE_GAME_NODE_H__
#define __ENGINE_GAME_NODE_H__

#include "Engine.h"
#include "IRenderable.h"
#include "IActionHandler.h"
#include "IInputableMouse.h"
#include "ActionConstants.h"
#include "InputConstants.h"
#include "IUpdateable.h"
#include "GameNodeCommand.h"

#include "Rendering\Renderer.h"
#include "Rendering\Shader.h"

#include "Math\Transform.h"

#include "Physics\PhysicsObject.h"

#include <vector>

namespace engine
{

	class GameComponent;

	class GameNode : public IActionHandler, public Input::IInputableMouse, public IUpdateable, public IRenderable
	{
		typedef std::map<Actions::Action, GameNodeCommand*> ActionsToGameNodeCommands;
		typedef std::map<States::State, GameNodeCommand*> StatesToGameNodeCommands;
		/* ==================== Static variables and functions begin ==================== */
	private:
		/// <summary>
		/// The number of entities currently being maintained in the application.
		/// </summary>
		static int gameNodeCount;
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Game node constructor. </summary>
		ENGINE_API GameNode();

		/// <summary> Game node destructor. </summary>
		ENGINE_API ~GameNode(void);

		/// <summary>
		/// Game node copy constructor.
		/// </summary>
		GameNode(const GameNode& gameNode) = delete;
		/// <summary>
		/// Game node move constructor.
		/// </summary>
		ENGINE_API GameNode(GameNode&& gameNode);
		/// <summary>
		/// Game node copy assignment operator.
		/// </summary>
		GameNode& operator=(const GameNode& gameNode) = delete;
		/// <summary>
		/// Game node move assignment operator.
		/// </summary>
		ENGINE_API GameNode& operator=(GameNode&& gameNode);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API GameNode Clone() const;
		ENGINE_API int GetID() const { return m_ID; }; // just for easier debugging purposes
		ENGINE_API GameNode* AddChild(GameNode* child);
		ENGINE_API GameNode* AddComponent(GameComponent* component);

		ENGINE_API void MouseButtonEvent(int button, int action, int mods);
		ENGINE_API void MousePosEvent(double xPos, double yPos);
		ENGINE_API void ScrollEvent(double xOffset, double yOffset);

		//ENGINE_API void InputAll(math::Real delta);
		ENGINE_API void Update(math::Real elapsedTime);
		ENGINE_API void Render(int shaderID, Rendering::Renderer* renderer) const override;
		ENGINE_API void RegisterCommandForAction(Actions::Action action, GameNodeCommand* gameNodeCommand)
		{
			m_actionsToCommands.insert(std::make_pair(action, gameNodeCommand));
		}
		ENGINE_API void Handle(Actions::Action action)
		{
			CRITICAL_LOG_ENGINE("Executing action ", action, " for entity ", m_ID);
			ActionsToGameNodeCommands::iterator actionToCommandItr = m_actionsToCommands.find(action);
			if (actionToCommandItr != m_actionsToCommands.end())
			{
				actionToCommandItr->second->Execute(this);
			}
		}
		ENGINE_API void Handle(States::State state)
		{
			CRITICAL_LOG_ENGINE("Handling the state ", state, " for entity ", m_ID);
			std::map<States::State, GameNodeCommand*>::iterator stateToCommandItr = m_statesToCommands.find(state);
			if (stateToCommandItr != m_statesToCommands.end())
			{
				stateToCommandItr->second->Execute(this);
			}
		}

		ENGINE_API math::Transform& GetTransform() { return m_transform; };
		ENGINE_API const math::Transform& GetTransform() const { return m_transform; }

		ENGINE_API Physics::PhysicsObject* GetPhysicsObject() { return m_physicsObject.get(); }
		ENGINE_API void CreatePhysicsObject(math::Real mass, const math::Vector3D& linearVelocity);

		ENGINE_API std::vector<GameNode*> GetAllDescendants() const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/// <summary>
		/// The unique ID of an entity. It simplifies the debugging.
		/// </summary>
		int m_ID;
		std::vector<GameNode*> m_childrenGameNodes;
		std::vector<GameComponent*> m_components;
		std::vector<IRenderable*> m_renderableComponents;
		std::vector<Input::IInputableMouse*> m_inputableMouseComponents;
		std::vector<IUpdateable*> m_updateableComponents;
		math::Transform m_transform;
		std::unique_ptr<Physics::PhysicsObject> m_physicsObject;
		ActionsToGameNodeCommands m_actionsToCommands;
		StatesToGameNodeCommands m_statesToCommands;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameNode */

} /* end namespace Engine */

#endif // __ENGINE_GAME_NODE_H__