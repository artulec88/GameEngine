#pragma once

#include "Rendering.h"
//#include "CoreEngine.h"
#include "Math\Math.h"

namespace Rendering
{

// Beware of circular dependencies
class Renderer;
class GameNode;
class Shader;
class Transform;
class CoreEngine;


/** 
 * TODO: GameComponent should not have empty definitions for Input, Update and Render functions.
 * Instead, game components should inherit from IInputable when they need to react to the input, from IUpdateable when they need to update themselves and
 * from IRenderable when they can be rendered. The GameNode should have a four lists of game components:
 * First storing all associated game components, the second one storing only associated game components which react to input,
 * the third one storing only associated components which can be updated and the fourth one with game components that can be rendered.
 * This way we could prevent the application from calling empty functions. As game components functions are used very often this should help in FPS / SPF.
 */
class GameComponent
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API GameComponent();
	RENDERING_API virtual ~GameComponent(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void SetParent(GameNode* parentGameNode);
	Transform& GetTransform();
	const Transform& GetTransform() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GameNode* m_parentGameNode;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameComponent */

} /* end namespace Rendering */