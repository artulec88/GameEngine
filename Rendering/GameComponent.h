#pragma once

#include "Rendering.h"
#include "Math\Math.h"

namespace Rendering
{

// Beware of circular dependencies
class Renderer;
class GameNode;
class Shader;
class Transform;

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

protected:
	const Transform& GetTransform() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GameNode* parentGameNode;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameComponent */

} /* end namespace Rendering */