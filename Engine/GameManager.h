#ifndef __ENGINE_GAME_MANAGER_H__
#define __ENGINE_GAME_MANAGER_H__

#include "Engine.h"
#include "GameStateManager.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "GameNode.h"
#include "MenuEntry.h"
#include "GameCommand.h"

#include "Rendering\Mesh.h"
#include "Rendering\Shader.h"
#include "Rendering\Material.h"
#include "Rendering\PointLight.h"
#include "Rendering\SpotLight.h"
#include "Rendering\Camera.h"
#include "Rendering\Font.h"
#include "Rendering\GuiText.h"

#include "Math\Transform.h"
#include "Math\Math.h"

#include <vector>

namespace Engine
{

class GameManager : public IUpdateable
{
	typedef std::map<const Rendering::Text::Font*, std::vector<Rendering::Text::GuiText>, Rendering::Text::FontComparator> FontMap;
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
	GameManager(GameManager& gameManager);
	void operator=(GameManager& gameManager);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	ENGINE_API virtual void Load() = 0; // Loads the game
	ENGINE_API void Render(Rendering::Renderer* renderer) const;

	ENGINE_API inline GameNode& GetRootGameNode() { return m_rootGameNode; }
	ENGINE_API inline GameNode* GetWaterNode() { return m_waterNode; }
	ENGINE_API inline GameNode* GetTerrainNode() { return m_terrainNode; }
	ENGINE_API inline GameNode* GetSkyboxNode() { return m_skyboxNode; }
	ENGINE_API inline const std::vector<GameNode*>& GetBillboardNodes() const { return m_billboardNodes; }
	ENGINE_API inline const FontMap& GetTexts() const { return m_texts; }

	ENGINE_API virtual Math::Real GetLoadingProgress() const = 0;
	ENGINE_API bool IsGameLoaded() const { return m_isGameLoaded; }

	ENGINE_API bool IsInGameTimeCalculationEnabled() const { return m_gameStateManager->IsInGameTimeCalculationEnabled(); }

#ifdef ANT_TWEAK_BAR_ENABLED
	virtual void InitializeTweakBars();
#endif

	ENGINE_API virtual GameState* GetIntroGameState() = 0;
	ENGINE_API virtual GameState* GetMainMenuGameState() = 0;
	ENGINE_API virtual GameState* GetLoadGameState() = 0;
	ENGINE_API virtual GameState* GetPlayGameState() = 0;
	ENGINE_API virtual GameState* GetPlayMainMenuGameState() = 0;

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
	void PerformStateTransition();
	ENGINE_API void PopState();
	ENGINE_API void RequestGameQuit() const;
public:
	ENGINE_API void AddTerrainNode(GameNode* terrainNode);
	ENGINE_API void AddWaterNode(GameNode* waterNode);
	ENGINE_API void AddBillboardNode(GameNode* billboardNode);
	ENGINE_API void AddText(const Rendering::Text::GuiText& text);
	ENGINE_API void AddSkyboxNode(GameNode* skyboxNode);
protected:
	ENGINE_API void AddToSceneRoot(GameNode* child);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GameNode m_rootGameNode;
	GameNode* m_terrainNode;
	GameNode* m_skyboxNode;
	GameNode* m_waterNode;
	std::vector<GameNode*> m_billboardNodes;
	FontMap m_texts;
	GameStateManager* m_gameStateManager;
	bool m_isGameLoaded;

	Math::Angle m_skyboxAngle;
	const Math::Angle m_skyboxAngleStep;

	EmptyGameCommand m_emptyGameCommand;

	MenuEntry m_mainMenuRootEntry;
	MenuEntry m_playMainMenuRootEntry;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameManager */

} /* end namespace Engine */

#endif // __ENGINE_GAME_MANAGER_H__
