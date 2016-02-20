#pragma once

#include "Rendering.h"
#include "IRenderable.h"
#include "IInputableKeyboard.h"
#include "IInputableMouse.h"
#include "IUpdateable.h"

#include "Math\Transform.h"

#include <vector>

namespace Rendering
{

class GameComponent;
class Shader;
class Renderer;
class CoreEngine;

class GameNode : public Input::IInputableKeyboard, public Input::IInputableMouse, public IUpdateable, public IRenderable
{
/* ==================== Static variables and functions begin ==================== */
private:
	static int gameNodeCount; // just for easier debugging purposes
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API GameNode();
	RENDERING_API virtual ~GameNode(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API int GetID() const { return m_ID; }; // just for easier debugging purposes
	RENDERING_API GameNode* AddChild(GameNode* child);
	RENDERING_API GameNode* AddComponent(GameComponent* component);

	RENDERING_API virtual void KeyEvent(int key, int scancode, int action, int mods);
	RENDERING_API virtual void MouseButtonEvent(int button, int action, int mods);
	RENDERING_API virtual void MousePosEvent(double xPos, double yPos);
	RENDERING_API virtual void ScrollEvent(double xOffset, double yOffset);

	//RENDERING_API void InputAll(Math::Real delta);
	RENDERING_API virtual void Update(Math::Real elapsedTime);
	RENDERING_API virtual void Render(Shader* shader, Renderer* renderer) const;

	RENDERING_API Math::Transform& GetTransform() { return m_transform; };
	RENDERING_API const Math::Transform& GetTransform() const { return m_transform; }

	RENDERING_API std::vector<GameNode*> GetAllDescendants() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	int m_ID; // just for easier debugging purposes
	std::vector<GameNode*> m_childrenGameNodes;
	std::vector<GameComponent*> m_components;
	std::vector<IRenderable*> m_renderableComponents;
	std::vector<Input::IInputableKeyboard*> m_inputableKeyboardComponents;
	std::vector<Input::IInputableMouse*> m_inputableMouseComponents;
	std::vector<IUpdateable*> m_updateableComponents;
	Math::Transform m_transform;
	// TODO: set of commands (GameCommand objects) to be performed by the game entity in the UPDATE step
/* ==================== Non-static member variables end ==================== */
}; /* end class GameNode */

} /* end namespace Rendering */