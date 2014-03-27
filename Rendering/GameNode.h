#pragma once

#include "Rendering.h"
#include "Transform.h"
#include <vector>

namespace Rendering
{

class GameComponent;
class Shader;
class Renderer;

class RENDERING_API GameNode
{
/* ==================== Constructors and destructors begin ==================== */
public:
	GameNode();
	virtual ~GameNode(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	GameNode* AddChild(GameNode* child);
	GameNode* AddComponent(GameComponent* component);
	void Input(Math::Real delta);
	void Update(Math::Real delta);
	void Render(Shader* shader, Renderer* renderer);
	void AddToRenderingEngine(Renderer* renderer);

	Transform& GetTransform() { return this->transform; };
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	std::vector<GameNode*> childrenGameNodes;
	std::vector<GameComponent*> components;
	Transform transform;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameNode */

} /* end namespace Rendering */