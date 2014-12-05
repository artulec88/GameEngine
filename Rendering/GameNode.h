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

class GameNode
{
/* ==================== Static variables begin ==================== */
private:
	static int gameNodeCount; // just for easier debugging purposes
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API GameNode();
	RENDERING_API virtual ~GameNode(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API int GetID() const { return this->ID; }; // just for easier debugging purposes
	RENDERING_API GameNode* AddChild(GameNode* child);
	RENDERING_API GameNode* AddComponent(GameComponent* component);

	RENDERING_API void InputAll(Math::Real delta);
	RENDERING_API void UpdateAll(Math::Real delta);
	RENDERING_API void RenderAll(Shader* shader, Renderer* renderer);

	RENDERING_API void Input(Math::Real delta);
	RENDERING_API void Update(Math::Real delta);
	RENDERING_API void Render(Shader* shader, Renderer* renderer);

	RENDERING_API void SetEngine(CoreEngine* coreEngine);

	RENDERING_API Transform& GetTransform() { return this->transform; };
	RENDERING_API const Transform& GetTransform() const { return this->transform; }

	RENDERING_API std::vector<GameNode*> GetAllDescendants() const;
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