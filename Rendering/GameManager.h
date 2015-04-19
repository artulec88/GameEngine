#ifndef __RENDERING_GAME_MANAGER_H__
#define __RENDERING_GAME_MANAGER_H__

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

class GameManager
{
/* ==================== Static variables begin ==================== */
protected:
	static GameManager* s_gameManager;
/* ==================== Static variables end ==================== */

/* ==================== Static functions begin ==================== */
public:
	RENDERING_API static GameManager* GetGameManager();
	
	RENDERING_API static void Load(void* arg);

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
	RENDERING_API GameManager();
	RENDERING_API virtual ~GameManager(void);
private:
	GameManager(GameManager& gameManager) {}
	void operator=(GameManager& gameManager) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API virtual void Load() = 0; // Loads the game
	RENDERING_API virtual void Input(Math::Real delta) = 0;
	RENDERING_API virtual void Update(Math::Real delta) = 0;
	RENDERING_API void Render(Renderer* renderer);

	RENDERING_API inline GameNode& GetRootGameNode() { return m_rootGameNode; }

	RENDERING_API virtual Math::Real GetLoadingProgress() const = 0;
	RENDERING_API bool IsGameLoaded() const { return m_isGameLoaded; }

	RENDERING_API bool IsInGameTimeCalculationEnabled() const { return m_gameStateManager->IsInGameTimeCalculationEnabled(); }

#ifdef ANT_TWEAK_BAR_ENABLED
	virtual void InitializeTweakBars();
#endif

	RENDERING_API virtual void WindowResizeEvent(GLFWwindow* window, int width, int height);
	RENDERING_API virtual void CloseWindowEvent(GLFWwindow* window);
	RENDERING_API virtual void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	RENDERING_API virtual void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	RENDERING_API virtual void MousePosEvent(GLFWwindow* window, double xPos, double yPos);
	RENDERING_API virtual void ScrollEvent(GLFWwindow* window, double xOffset, double yOffset);

	/// <summary> Sets the game state transition object. The transition itself is not performed.
	/// The transition itself is performed in the <code>PerformStateTransition</code> method.</summary>
	/// <see cref="PerformStateTransition">
	RENDERING_API void SetTransition(GameStateTransitioning::GameStateTransition* gameStateTransition);
	RENDERING_API void PerformStateTransition();
	RENDERING_API void PopState();
	RENDERING_API void RequestGameQuit() const;

protected:
	RENDERING_API void RegisterTerrainNode(GameNode* terrainNode);
	RENDERING_API void AddToSceneRoot(GameNode* child);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GameNode m_rootGameNode;
	GameStateManager* m_gameStateManager;
	bool m_isGameLoaded;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameManager */

} /* end namespace Rendering */

#endif // __RENDERING_GAME_MANAGER_H__
