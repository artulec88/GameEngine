#ifndef __ENGINE_GAME_NODE_H__
#define __ENGINE_GAME_NODE_H__

#include "Engine.h"
#include "IRenderable.h"
#include "IInputableKeyboard.h"
#include "IInputableMouse.h"
#include "IUpdateable.h"

#include "Rendering\Renderer.h"
#include "Rendering\Shader.h"

#include "Math\Transform.h"

#include "Physics\PhysicsObject.h"

#include <vector>

namespace Engine
{

	class GameComponent;

	class GameNode : public Input::IInputableKeyboard, public Input::IInputableMouse, public IUpdateable, public IRenderable
	{
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
		ENGINE_API virtual ~GameNode(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API int GetID() const { return m_ID; }; // just for easier debugging purposes
		ENGINE_API GameNode* AddChild(GameNode* child);
		ENGINE_API GameNode* AddComponent(GameComponent* component);

		ENGINE_API virtual void KeyEvent(int key, int scancode, int action, int mods);
		ENGINE_API virtual void MouseButtonEvent(int button, int action, int mods);
		ENGINE_API virtual void MousePosEvent(double xPos, double yPos);
		ENGINE_API virtual void ScrollEvent(double xOffset, double yOffset);

		//ENGINE_API void InputAll(Math::Real delta);
		ENGINE_API virtual void Update(Math::Real elapsedTime);
		ENGINE_API virtual void Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const;

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
		std::vector<Input::IInputableKeyboard*> m_inputableKeyboardComponents;
		std::vector<Input::IInputableMouse*> m_inputableMouseComponents;
		std::vector<IUpdateable*> m_updateableComponents;
		Math::Transform m_transform;
		std::unique_ptr<Physics::PhysicsObject> m_physicsObject;
		// TODO: set of commands (GameCommand objects) to be performed by the game entity in the UPDATE step
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameNode */

} /* end namespace Engine */

#endif // __ENGINE_GAME_NODE_H__