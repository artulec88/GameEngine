#pragma once

#include "Rendering.h"
#include "GameNode.h"
#include "Shader.h"

namespace Rendering
{

class RENDERING_API GameComponent
{
/* ==================== Constructors and destructors begin ==================== */
public:
	GameComponent();
	virtual ~GameComponent(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Input();
	virtual void Update();
	virtual void Render(Shader* shader);

	//virtual void AddToRenderingEngine(RenderingEngine* renderingEngine);
	void SetParent(GameNode* parentGameNode);
	Transform& GetTransform();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GameNode* parentGameNode;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameComponent */

} /* end namespace Rendering */