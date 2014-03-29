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
	static void WindowCloseEventCallback(GLFWwindow* window);
	//static void WindowResizeCallback(GLFWwindow* window, int width, int height);
	static void KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void CharEventCallback(GLFWwindow* window, unsigned int codepoint);
	static void MouseEventCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset);
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
	virtual void Input(Math::Real delta) = 0;
	virtual void Update(Math::Real delta) = 0;

	virtual void InitializeCameras(); // TODO: Consider deleting this function. Otherwise, implement it

	virtual void CloseWindowEvent(GLFWwindow* window);
	virtual void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void MouseEvent(GLFWwindow* window, int button, int action, int mods);
	virtual void ScrollEvent(GLFWwindow* window, double xOffset, double yOffset);

	GameNode& GetRootGameNode() const;
	//Shader* GetShader() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GameNode* rootGameNode;
	//Shader* shader;
/* ==================== Non-static member variables end ==================== */
}; /* end class Game */

} /* end namespace Rendering */