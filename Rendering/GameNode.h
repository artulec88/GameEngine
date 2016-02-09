#pragma once

#include "Rendering.h"
#include "Transform.h"
#include "IRenderable.h"
#include "IInputable.h"
#include "IUpdateable.h"
#include "Math\Math.h"
#include <vector>

namespace Rendering
{

class GameComponent;
class Shader;
class Renderer;
class CoreEngine;

class GameNode
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

	RENDERING_API void InputAll(Math::Real delta);
	RENDERING_API void UpdateAll(Math::Real delta);
	RENDERING_API void RenderAll(Shader* shader, Renderer* renderer) const;

	RENDERING_API Transform& GetTransform() { return m_transform; };
	RENDERING_API const Transform& GetTransform() const { return m_transform; }

	RENDERING_API std::vector<GameNode*> GetAllDescendants() const;
private:
	void Input(Math::Real delta);
	void Update(Math::Real delta);
	void Render(Shader* shader, Renderer* renderer) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	int m_ID; // just for easier debugging purposes
	std::vector<GameNode*> m_childrenGameNodes;
	std::vector<GameComponent*> m_components;
	std::vector<IRenderable*> m_renderableComponents;
	std::vector<Input::IInputable*> m_inputableComponents;
	std::vector<IUpdateable*> m_updateableComponents;
	Transform m_transform;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameNode */

} /* end namespace Rendering */