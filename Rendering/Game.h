#ifndef RENDERING_GAME_H
#define RENDERING_GAME_H

#include "Rendering.h"
#include "GameNode.h"
#include "GameStateManager.h"
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

class Renderer;

class RENDERING_API Game
{
/* ==================== Static variables begin ==================== */
protected:
	static Game* s_game;
/* ==================== Static variables end ==================== */

/* ==================== Static functions begin ==================== */
public:
	static Game* GetGame();
	static void WindowCloseEventCallback(GLFWwindow* window);
	static void WindowResizeCallback(GLFWwindow* window, int width, int height);
	static void KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void CharEventCallback(GLFWwindow* window, unsigned int codepoint);
	static void MouseEventCallback(GLFWwindow* window, int button, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
	static void ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset);
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Game();
	virtual ~Game(void);
private:
	Game(Game& game) {}
	void operator=(Game& game) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Init();
	virtual void Input(Math::Real delta) = 0;
	virtual void Update(Math::Real delta) = 0;
	void Render(Renderer* renderer);

	inline GameNode& GetRootGameNode() { return m_rootGameNode; }

	void SetEngine(CoreEngine* coreEngine);

#ifdef ANT_TWEAK_BAR_ENABLED
	virtual void InitializeTweakBars();
#endif

	virtual void WindowResizeEvent(GLFWwindow* window, int width, int height);
	virtual void CloseWindowEvent(GLFWwindow* window);
	virtual void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	virtual void MousePosEvent(GLFWwindow* window, double xPos, double yPos);
	virtual void ScrollEvent(GLFWwindow* window, double xOffset, double yOffset);

protected:
	void AddToSceneRoot(GameNode* child);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GameNode m_rootGameNode;
	GameStateManager* m_gameStateManager;
/* ==================== Non-static member variables end ==================== */
}; /* end class Game */

} /* end namespace Rendering */

#endif // RENDERING_GAME_H