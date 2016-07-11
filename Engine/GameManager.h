#ifndef __ENGINE_GAME_MANAGER_H__
#define __ENGINE_GAME_MANAGER_H__

#include "Engine.h"
#include "GameStateManager.h"
#include "ParticleGenerator.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "GameNode.h"
#include "GameCommand.h"
#include "InputMapping.h"
//#include "Observer.h"

#include "EffectFactory.h"
#include "Rendering\Mesh.h"
#include "Rendering\Particle.h"
#include "Rendering\Shader.h"
#include "Rendering\Material.h"
#include "Rendering\PointLight.h"
#include "Rendering\SpotLight.h"
#include "Rendering\Camera.h"
#include "Rendering\Font.h"
#include "Rendering\GuiText.h"
#include "Rendering\Texture.h"

#include "Math\Transform.h"
#include "Math\Math.h"

#include <amp.h>
#include <vector>

namespace Engine
{

class GameManager : public IUpdateable
{
	typedef std::map<const Rendering::Text::Font*, std::vector<Rendering::Controls::GuiTextControl>, Rendering::Text::FontComparator> FontMap;
	typedef std::map<Actions::Action, std::list<GameNode*>> ActionsToGameNodesMap;
	typedef std::map<Actions::Action, const GameCommand*> ActionsToGameCommandsMap;
	typedef std::map<States::State, std::list<GameNode*>> StatesToGameNodesMap;
	typedef std::map<States::State, const GameCommand*> StatesToGameCommandsMap;
/* ==================== Static variables and functions begin ==================== */
protected:
	static GameManager* s_gameManager;
public:
	ENGINE_API static GameManager* GetGameManager();
	ENGINE_API static void Load(void* arg);
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary>
	/// Creates a game manager.
	/// </summary>
	/// <remarks>
	/// One must remember to create a game manager after rendering context is properly setup.
	/// Game manager will try to create game fonts which require the graphics context to be initialized.
	/// </remarks>
	ENGINE_API GameManager();

	/// <summary>
	/// Destroys the game manager.
	/// </summary>
	ENGINE_API virtual ~GameManager(void);
	GameManager(GameManager& gameManager) = delete;
	void operator=(GameManager& gameManager) = delete;
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	ENGINE_API virtual void Load() = 0; // Loads the game
	ENGINE_API virtual void Input(const Engine::Input::MappedInput& input);
	//ENGINE_API virtual void Notify(GameNode* gameNode, Actions::Action action /*const GameEvent& gameEvent*/) const;
	ENGINE_API void Render(Rendering::Renderer* renderer) const;

	ENGINE_API inline GameNode& GetRootGameNode() { return m_rootGameNode; }
	ENGINE_API inline GameNode* GetWaterNode() { return m_waterNode; }
	ENGINE_API inline GameNode* GetTerrainNode() { return m_terrainNode; }
	ENGINE_API inline GameNode* GetSkyboxNode() { return m_skyboxNode; }
	ENGINE_API inline const std::vector<GameNode*>& GetBillboardRenderers() const { return m_billboardsRenderers; }
	ENGINE_API inline const std::vector<ParticleGenerator*>& GetParticleGenerators() const { return m_particleGenerators; }
	//ENGINE_API inline const FontMap& GetTexts() const { return m_texts; }

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
	virtual void MousePosEvent(double xPos, double yPos);
	ENGINE_API virtual void ScrollEvent(double xOffset, double yOffset);

	/// <summary> Sets the game state transition object. The transition itself is not performed.
	/// The transition itself is performed in the <code>PerformStateTransition</code> method.</summary>
	/// <see cref="PerformStateTransition">
	ENGINE_API void SetTransition(GameStateTransitioning::GameStateTransition* gameStateTransition);
	void PerformStateTransition();
	ENGINE_API void PopState();
	ENGINE_API void RequestGameQuit() const;
	ENGINE_API const GameCommand& GetCommand(Actions::Action action) const
	{
		ActionsToGameCommandsMap::const_iterator actionToGameCommandItr = m_actionsToGameCommandsMap.find(action);
		if (actionToGameCommandItr == m_actionsToGameCommandsMap.end())
		{
			return m_emptyGameCommand;
		}
		return *actionToGameCommandItr->second;
	}
	//ENGINE_API Rendering::Effects::Effect<Math::Real>* GetSingleValueEffect(Rendering::Effects::EffectType effectType, unsigned int variant)
	//{
	//	return m_effectFactory.GetSingleValueEffect(effectType, variant);
	//}
	//ENGINE_API Rendering::Effects::Effect<Math::Vector2D>* GetVec2DEffect(Rendering::Effects::EffectType effectType, unsigned int variant)
	//{
	//	return m_effectFactory.GetVec2DEffect(effectType, variant);
	//}
	//ENGINE_API Rendering::Effects::Effect<Math::Vector3D>* GetVec3DEffect(Rendering::Effects::EffectType effectType, unsigned int variant)
	//{
	//	return m_effectFactory.GetVec3DEffect(effectType, variant);
	//}
public:
	ENGINE_API void AddTerrainNode(GameNode* terrainNode);
	ENGINE_API void AddWaterNode(GameNode* waterNode);
	ENGINE_API void AddBillboardsRenderer(GameNode* billboardsRenderer);
	ENGINE_API void AddText(const Rendering::Controls::GuiTextControl& text);
	ENGINE_API void AddSkyboxNode(GameNode* skyboxNode);
	ENGINE_API void AddParticleGenerator(ParticleGenerator* particleGenerator);
protected:
	ENGINE_API void AddToSceneRoot(GameNode* child);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GameNode m_rootGameNode;
	GameNode* m_terrainNode;
	GameNode* m_skyboxNode;
	GameNode* m_waterNode;
	std::vector<GameNode*> m_billboardsRenderers;
	std::vector<ParticleGenerator*> m_particleGenerators;
	//FontMap m_texts;
	GameStateManager* m_gameStateManager;
	bool m_isGameLoaded;

	Math::Angle m_skyboxAngle;
	const Math::Angle m_skyboxAngleStep;

	const EmptyGameCommand m_emptyGameCommand;
	ActionsToGameCommandsMap m_actionsToGameCommandsMap;
	//Rendering::Effects::EffectFactory m_effectFactory;

	/// <summary>
	/// The list of game nodes that are interested in handling the particular action event that occurred in the game.
	/// </summary>
	ActionsToGameNodesMap m_actionsToGameNodesMap;

	/// <summary>
	/// The map of game commands identified by a state.
	/// </summary>
	StatesToGameCommandsMap m_statesToGameCommandsMap;
	
	/// <summary>
	/// The list of game nodes that are interested in handling the particular state that occurred in the game.
	/// </summary>
	StatesToGameNodesMap m_statesToGameNodesMap;
/* ==================== Non-static member variables end ==================== */
}; /* end class GameManager */

} /* end namespace Engine */

#endif // __ENGINE_GAME_MANAGER_H__
