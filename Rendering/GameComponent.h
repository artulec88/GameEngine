#pragma once

#include "Rendering.h"
#include "Transform.h"
//#include "Renderer.h"
#include "GameNode.h"
//#include "Shader.h"

namespace Rendering
{

class Renderer;
class Shader;

class RENDERING_API GameComponent
{
/* ==================== Constructors and destructors begin ==================== */
public:
	GameComponent();
	virtual ~GameComponent(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Input(Math::Real delta);
	virtual void Update(Math::Real delta);
	virtual void Render(Shader* shader, Renderer* renderer);

	virtual void AddToRenderingEngine(Renderer* renderer);
	void SetParent(GameNode* parentGameNode);
	Transform& GetTransform();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GameNode* parentGameNode;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameComponent */

} /* end namespace Rendering */