#pragma once

#include "Rendering.h"
#include "GameNode.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Material.h"
#include "PointLight.h"
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
	GLuint LoadShaders(const char* vertexFilePath, const char* fragmentFilePath); // TODO: Move this function somewhere else in the future

	virtual void Render();

	virtual void Init();
	virtual void Input();
	virtual void Update();
	virtual void CloseWindowEvent(GLFWwindow* window);
	virtual void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

	inline GameNode& GetRootGameNode() { return rootGameNode; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GameNode rootGameNode;
	Mesh* mesh;
	Shader* shader;
	Transform* transform;
	Material* material;
	Camera* camera;
	PointLight* pointLights;
/* ==================== Non-static member variables end ==================== */
}; /* end class Game */

} /* end namespace Rendering */