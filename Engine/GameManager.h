#ifndef __ENGINE_GAME_MANAGER_H__
#define __ENGINE_GAME_MANAGER_H__

#include "Engine.h"
#include "GameStateManager.h"

#include "Rendering\GameNode.h"
#include "Rendering\Mesh.h"
#include "Rendering\Shader.h"
#include "Rendering\Material.h"
#include "Rendering\PointLight.h"
#include "Rendering\SpotLight.h"
#include "Rendering\Camera.h"
#include "Rendering\IUpdateable.h"
#include "Rendering\IRenderable.h"

#include "Math\Transform.h"
#include "Math\Math.h"

namespace Engine
{

class GameManager : public Rendering::IUpdateable
{
/* ==================== Static variables begin ==================== */
protected:
	static GameManager* s_gameManager;
/* ==================== Static variables end ==================== */

/* ==================== Static functions begin ==================== */
public:
	ENGINE_API static GameManager* GetGameManager();
	ENGINE_API static void Load(void* arg);
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ENGINE_API GameManager();
	ENGINE_API virtual ~GameManager(void);
private:
	GameManager(GameManager& gameManager) {}
	void operator=(GameManager& gameManager) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	ENGINE_API virtual void Load() = 0; // Loads the game
	ENGINE_API void Render(Rendering::Renderer* renderer) const;

	ENGINE_API inline Rendering::GameNode& GetRootGameNode() { return m_rootGameNode; }

	ENGINE_API virtual Math::Real GetLoadingProgress() const = 0;
	ENGINE_API bool IsGameLoaded() const { return m_isGameLoaded; }

	ENGINE_API bool IsInGameTimeCalculationEnabled() const { return m_gameStateManager->IsInGameTimeCalculationEnabled(); }

#ifdef ANT_TWEAK_BAR_ENABLED
	virtual void InitializeTweakBars();
#endif

	// TODO: Think about removing the window parameter from the event handling functions below.
	ENGINE_API virtual void WindowResizeEvent(int width, int height);
	ENGINE_API virtual void CloseWindowEvent();
	ENGINE_API virtual void KeyEvent(int key, int scancode, int action, int mods);
	virtual void MouseButtonEvent(int button, int action, int mods);
	virtual void MousePosEvent(double xPos, double yPos);
	ENGINE_API virtual void ScrollEvent(double xOffset, double yOffset);

	/// <summary> Sets the game state transition object. The transition itself is not performed.
	/// The transition itself is performed in the <code>PerformStateTransition</code> method.</summary>
	/// <see cref="PerformStateTransition">
	ENGINE_API void SetTransition(GameStateTransitioning::GameStateTransition* gameStateTransition);
	ENGINE_API void PerformStateTransition();
	ENGINE_API void PopState();
	ENGINE_API void RequestGameQuit() const;
protected:
	ENGINE_API void AddTerrainNode(Rendering::GameNode* terrainNode);
	ENGINE_API void AddToSceneRoot(Rendering::GameNode* child);
	ENGINE_API void AddWaterNode(Rendering::GameNode* waterNode);
	ENGINE_API void AddBillboardNode(Rendering::GameNode* billboardNode);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Rendering::GameNode m_rootGameNode;
	Rendering::GameNode* m_terrainNode;
	GameStateManager* m_gameStateManager;
	bool m_isGameLoaded;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameManager */

} /* end namespace Engine */

#endif // __ENGINE_GAME_MANAGER_H__