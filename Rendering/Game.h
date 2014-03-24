#pragma once

#include "Rendering.h"
#include "GameNode.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Material.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Camera.h"
#include "Math\Math.h"

namespace Rendering
{

class RENDERING_API Game
{
/* ==================== Static variables begin ==================== */
protected:
	static Game* game;
/* ==================== Static variables end ==================== */

/* ==================== Static functions begin ==================== */
public:
	static Game* GetGame();
	static void WindowCloseCallback(GLFWwindow* window);
	//static void WindowResizeCallback(GLFWwindow* window, int width, int height);
	static void KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void CharEventCallback(GLFWwindow* window, unsigned int codepoint);
	//static void MouseInputCallback(GLFWwindow* window, int button, int action, int mods);
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Game();
	virtual ~Game(void);
private:
	Game(Game& game);
	void operator=(Game& game);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Init() = 0;
	virtual void Input() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void CloseWindowEvent(GLFWwindow* window);
	virtual void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

	GameNode& GetRootGameNode() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GameNode* rootGameNode;
/* ==================== Non-static member variables end ==================== */
}; /* end class Game */

} /* end namespace Rendering */