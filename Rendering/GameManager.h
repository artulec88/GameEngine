#ifndef __CORE_GAME_MANAGER_H__
#define __CORE_GAME_MANAGER_H__

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
#include "IUpdateable.h"
#include "IRenderable.h"
#include "Math\Math.h"

namespace Core
{

class Renderer;

class GameManager : public Rendering::IUpdateable
{
/* ==================== Static variables begin ==================== */
protected:
	static GameManager* s_gameManager;
/* ==================== Static variables end ==================== */

/* ==================== Static functions begin ==================== */
public:
	RENDERING_API static GameManager* GetGameManager();
	RENDERING_API static void Load(void* arg);
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
	RENDERING_API void Render(Rendering::Renderer* renderer) const;

	RENDERING_API inline Rendering::GameNode& GetRootGameNode() { return m_rootGameNode; }

	RENDERING_API virtual Math::Real GetLoadingProgress() const = 0;
	RENDERING_API bool IsGameLoaded() const { return m_isGameLoaded; }

	RENDERING_API bool IsInGameTimeCalculationEnabled() const { return m_gameStateManager->IsInGameTimeCalculationEnabled(); }

#ifdef ANT_TWEAK_BAR_ENABLED
	virtual void InitializeTweakBars();
#endif

	// TODO: Think about removing the window parameter from the event handling functions below.
	RENDERING_API virtual void WindowResizeEvent(int width, int height);
	RENDERING_API virtual void CloseWindowEvent();
	RENDERING_API virtual void KeyEvent(int key, int scancode, int action, int mods);
	virtual void MouseButtonEvent(int button, int action, int mods);
	virtual void MousePosEvent(double xPos, double yPos);
	RENDERING_API virtual void ScrollEvent(double xOffset, double yOffset);

	/// <summary> Sets the game state transition object. The transition itself is not performed.
	/// The transition itself is performed in the <code>PerformStateTransition</code> method.</summary>
	/// <see cref="PerformStateTransition">
	RENDERING_API void SetTransition(Rendering::GameStateTransitioning::GameStateTransition* gameStateTransition);
	RENDERING_API void PerformStateTransition();
	RENDERING_API void PopState();
	RENDERING_API void RequestGameQuit() const;
protected:
	RENDERING_API void AddTerrainNode(Rendering::GameNode* terrainNode);
	RENDERING_API void AddToSceneRoot(Rendering::GameNode* child);
	RENDERING_API void AddWaterNode(Rendering::GameNode* waterNode);
	RENDERING_API void AddBillboardNode(Rendering::GameNode* billboardNode);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Rendering::GameNode m_rootGameNode;
	Rendering::GameNode* m_terrainNode;
	Rendering::GameStateManager* m_gameStateManager;
	bool m_isGameLoaded;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameManager */

} /* end namespace Core */

#endif // __CORE_GAME_MANAGER_H__
