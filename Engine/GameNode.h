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

namespace Engine
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
		ENGINE_API GameNode();
		ENGINE_API ~GameNode(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API int GetID() const { return m_ID; }; // just for easier debugging purposes
		ENGINE_API GameNode* AddChild(GameNode* child);
		ENGINE_API GameNode* AddComponent(GameComponent* component);

		ENGINE_API void MouseButtonEvent(int button, int action, int mods);
		ENGINE_API void MousePosEvent(double xPos, double yPos);
		ENGINE_API void ScrollEvent(double xOffset, double yOffset);

		//ENGINE_API void InputAll(Math::Real delta);
		ENGINE_API void Update(Math::Real elapsedTime);
		ENGINE_API void Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const;
		ENGINE_API void RegisterCommandForAction(Actions::Action action, GameNodeCommand* gameNodeCommand)
		{
			m_actionsToCommands.insert(std::make_pair(action, gameNodeCommand));
		}
		ENGINE_API void Handle(Actions::Action action)
		{
			CRITICAL_LOG_ENGINE("Executing action %d for entity %d", action, m_ID);
			ActionsToGameNodeCommands::iterator actionToCommandItr = m_actionsToCommands.find(action);
			if (actionToCommandItr != m_actionsToCommands.end())
			{
				actionToCommandItr->second->Execute(this);
			}
		}
		ENGINE_API void Handle(States::State state)
		{
			CRITICAL_LOG_ENGINE("Handling the state %d for entity", state, m_ID);
			std::map<States::State, GameNodeCommand*>::iterator stateToCommandItr = m_statesToCommands.find(state);
			if (stateToCommandItr != m_statesToCommands.end())
			{
				stateToCommandItr->second->Execute(this);
			}
		}

		ENGINE_API Math::Transform& GetTransform() { return m_transform; };
		ENGINE_API const Math::Transform& GetTransform() const { return m_transform; }

		ENGINE_API Physics::PhysicsObject* GetPhysicsObject() { return m_physicsObject.get(); }
		ENGINE_API void CreatePhysicsObject(Math::Real mass, const Math::Vector3D& linearVelocity);

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
		Math::Transform m_transform;
		std::unique_ptr<Physics::PhysicsObject> m_physicsObject;
		ActionsToGameNodeCommands m_actionsToCommands;
		StatesToGameNodeCommands m_statesToCommands;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameNode */

} /* end namespace Engine */

#endif // __ENGINE_GAME_NODE_H__