#pragma once

#include "Rendering.h"
#include "Transform.h"
#include "Math\Math.h"
#include <vector>

namespace Rendering
{

class GameComponent;
class Shader;
class Renderer;
class CoreEngine;

class RENDERING_API GameNode
{
/* ==================== Static variables begin ==================== */
private:
	static int gameNodeCount; // just for easier debugging purposes
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	GameNode();
	virtual ~GameNode(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	int GetID() const { return this->ID; }; // just for easier debugging purposes
	GameNode* AddChild(GameNode* child);
	GameNode* AddComponent(GameComponent* component);
	void Input(Math::Real delta);
	void Update(Math::Real delta);
	void Render(Shader* shader, Renderer* renderer);

	void SetEngine(CoreEngine* coreEngine);

	Transform& GetTransform() { return this->transform; };
	const Transform& GetTransform() const { return this->transform; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	int ID; // just for easier debugging purposes
	std::vector<GameNode*> childrenGameNodes;
	std::vector<GameComponent*> components;
	Transform transform;
	CoreEngine* coreEngine;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameNode */

} /* end namespace Rendering */