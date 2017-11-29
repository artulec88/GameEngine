#include "stdafx.h"
#include "GameManager.h"
#include "GameStateManager.h"
#include "CoreEngine.h"

#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"

#include "Math/Math.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility/ILogger.h"
#include "Utility/IConfig.h"
#include "Utility/Time.h"

#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBar/include/AntTweakBar.h"
#endif

engine::GameManager* engine::GameManager::s_gameManager = nullptr;

/* static */ engine::GameManager* engine::GameManager::GetGameManager()
{
	return s_gameManager;
}

/* static */ void engine::GameManager::LoadGame(GameManager* gameManager)
{
	CHECK_CONDITION_EXIT_ENGINE(gameManager != NULL, Utility::Logging::CRITICAL, "Cannot load the game. Specified game manager is NULL.");
	glfwMakeContextCurrent(CoreEngine::GetCoreEngine()->GetThreadWindow());
	engine::CoreEngine::GetCoreEngine()->InitGlew(); // glew init
	gameManager->Load();
}

engine::GameManager::GameManager() :
	//Observer(),
	IUpdateable(),
	m_gameStateManager(std::make_unique<engine::DefaultGameStateManager>()),
	m_isGameLoaded(false),
	m_emptyGameCommand(),
	m_actionsToGameCommandsMap(),
	//m_actionsToHandlersMap(),
	//m_actionsToGameCommandsMap({ { Input::Actions::EMPTY, std::make_unique<EmptyGameCommand>() } }), // cannot do it this way since it requires copying the unique_ptr object
	m_actionsToGameNodesMap()
#ifdef ANT_TWEAK_BAR_ENABLED
	, m_gameBar(nullptr)
#endif
#ifdef PROFILING_ENGINE_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("GameManager"))
#endif
{
	INFO_LOG_ENGINE("Game manager construction started");

	if (engine::GameManager::s_gameManager != nullptr)
	{
		ERROR_LOG_ENGINE("Constructor called when a singleton instance of CoreEngine class has already been created");
		SAFE_DELETE(engine::GameManager::s_gameManager);
	}

	m_actionsToGameCommandsMap.insert(std::make_pair(actions::EMPTY, &m_emptyGameCommand));
	//m_actionsToGameCommandsMap.insert(make_pair(Input::Actions::Action::EMPTY, std::make_unique<EmptyGameCommand>()));

	engine::GameManager::s_gameManager = this;
	DEBUG_LOG_ENGINE("Game manager construction finished");
}


engine::GameManager::~GameManager()
{
	INFO_LOG_ENGINE("Game manager destruction started");
	DEBUG_LOG_ENGINE("Game manager destruction finished");
}

void engine::GameManager::WindowResizeEvent(int width, int height)
{
	NOTICE_LOG_ENGINE("Window resize event (width = ", width, ", height = ", height, ")");
	// TODO: Check if core engine finds out about the resizing of the window.
}

void engine::GameManager::CloseWindowEvent()
{
	NOTICE_LOG_ENGINE("Close window event");
	CoreEngine::GetCoreEngine()->Stop();
}

void engine::GameManager::MousePosEvent(double xPos, double yPos)
{
	DEBUG_LOG_ENGINE("Mouse position event x=", xPos, ", y=", yPos);
}

void engine::GameManager::ScrollEvent(double xOffset, double yOffset)
{
	DEBUG_LOG_ENGINE("Scroll event: xOffset=", xOffset, "\t yOffset=", yOffset);
}

//GameNode& GameManager::GetRootGameNode() const
//{
//	CHECK_CONDITION_EXIT_ENGINE(rootGameNode != NULL, EMERGENCY, "Root game node is NULL.");
//	return *rootGameNode;
//}

//Shader* GameManager::GetShader() const
//{
//	CHECK_CONDITION_ENGINE(shader != NULL, ERR, "Shader is NULL.");
//	return shader;
//}

void engine::GameManager::AddGuiControl(const rendering::controls::GuiControl& guiControl)
{
	//FontMap::const_iterator textItr = m_texts.find(guiText.GetFont());
	//if (textItr == m_texts.end())
	//{
	//}
	//std::vector<Rendering::Text::GuiTextControl>::const_iterator guiTextItr = std::find(m_texts[guiText.GetFont()].begin(), m_texts[guiText.GetFont()].end(), guiText);
	//m_texts[guiText.GetFont()].push_back(guiText); // TODO: What about duplicates?
	//m_texts.insert(std::pair<const Rendering::Text::Font*, std::vector<Rendering::Text::GuiTextControl>>(guiText.GetFont(), std::vector<Rendering::Text::GuiTextControl>()));
	//m_texts[guiText.GetFont()].push_back(guiText);
}

void engine::GameManager::AddParticlesSystem(rendering::particles::ParticlesSystem* particlesSystem)
{
	m_particlesSystems.push_back(particlesSystem);
}

const rendering::Mesh* engine::GameManager::AddMesh(int meshId, const std::string& meshFileName) const
{
	return CoreEngine::GetCoreEngine()->AddMesh(meshId, meshFileName);
}

const rendering::Mesh* engine::GameManager::GetMesh(int meshId) const
{
	return CoreEngine::GetCoreEngine()->GetMesh(meshId);
}

const rendering::Texture* engine::GameManager::AddTexture(int textureId, const std::string& textureFileName) const
{
	return CoreEngine::GetCoreEngine()->AddTexture(textureId, textureFileName);
}

const rendering::Texture* engine::GameManager::AddCubeTexture(int textureId, const std::string& cubeMapTextureDirectory) const
{
	return CoreEngine::GetCoreEngine()->AddCubeTexture(textureId, cubeMapTextureDirectory);
}

const rendering::particles::ParticleTexture* engine::GameManager::AddParticleTexture(int textureId, const std::string& particleTextureFileName, int rowsCount, bool isAdditive) const
{
	return CoreEngine::GetCoreEngine()->AddParticleTexture(textureId, particleTextureFileName, rowsCount, isAdditive);
}

const rendering::Texture* engine::GameManager::GetTexture(int textureId) const
{
	return CoreEngine::GetCoreEngine()->GetTexture(textureId);
}

const rendering::Shader* engine::GameManager::AddShader(int shaderId, const std::string& shaderFileName) const
{
	return CoreEngine::GetCoreEngine()->AddShader(shaderId, shaderFileName);
}

const rendering::Shader* engine::GameManager::GetShader(int shaderId) const
{
	return CoreEngine::GetCoreEngine()->GetShader(shaderId);
}

void engine::GameManager::Input(actions::Action actionId)
{
	CRITICAL_LOG_ENGINE("Handling action: ", actionId);
	const auto gameCommandItr = m_actionsToGameCommandsMap.find(actionId);
	if (gameCommandItr != m_actionsToGameCommandsMap.end())
	{
		gameCommandItr->second->Execute(this);
	}
	else
	{
		//auto actionHandlerItr = m_actionsToHandlersMap.find(actionID);
		//if (actionHandlerItr != m_actionsToHandlersMap.end())
		//{
		//	for (auto actionHandler = actionHandlerItr->second.begin(); actionHandler != actionHandlerItr->second.end(); ++actionHandlerItr)
		//	{
		//		(*actionHandler)->Handle(actionID);
		//	}
		//}
		m_gameStateManager->Handle(actionId);
		for (std::list<GameNode*>::iterator gameNodeItr = m_actionsToGameNodesMap[actionId].begin(); gameNodeItr != m_actionsToGameNodesMap[actionId].end(); ++gameNodeItr)
		{
			(*gameNodeItr)->Handle(actionId);
		}
	}
}

void engine::GameManager::Input(const input::MappedInput& input)
{
	for (input::ActionsContainer::const_iterator actionItr = input.m_actions.begin(); actionItr != input.m_actions.end(); ++actionItr)
	{
		Input(*actionItr);
	}
	for (input::StatesContainer::const_iterator stateItr = input.m_states.begin(); stateItr != input.m_states.end(); ++stateItr)
	{
		const states::State state = *stateItr;
		DEBUG_LOG_ENGINE("Handling the state ", state);
		const auto gameCommandItr = m_statesToGameCommandsMap.find(state);
		if (gameCommandItr != m_statesToGameCommandsMap.end())
		{
			gameCommandItr->second->Execute(this);
		}
		else
		{
			m_gameStateManager->Handle(state);
			for (std::list<GameNode*>::iterator gameNodeItr = m_statesToGameNodesMap[state].begin(); gameNodeItr != m_statesToGameNodesMap[state].end(); ++gameNodeItr)
			{
				(*gameNodeItr)->Handle(state);
			}
		}
	}
	for (input::RangesContainer::const_iterator rangeItr = input.m_ranges.begin(); rangeItr != input.m_ranges.end(); ++rangeItr)
	{
		// TODO: Ranges processing.
		const ranges::Range range = rangeItr->first;
		const math::Real value = rangeItr->second;
		m_gameStateManager->Handle(range, value);
	}
}

//void Engine::GameManager::Notify(GameNode* gameNode, Actions::Action action /*const GameEvent& gameEvent*/) const
//{
//}

void engine::GameManager::Render(rendering::Renderer* renderer) const
{
	m_gameStateManager->Render(renderer);
}

void engine::GameManager::SetTransition(game_state_transitioning::GameStateTransition* gameStateTransition) const
{
	m_gameStateManager->SetTransition(gameStateTransition);
}

void engine::GameManager::PerformStateTransition() const
{
	m_gameStateManager->PerformStateTransition();
}

void engine::GameManager::PopState() const
{
	m_gameStateManager->Pop();
}

void engine::GameManager::RequestGameQuit() const
{
	// TODO: Ask the user to save the game if the inGameState can be found in the game states stack
	//glfwSetWindowShouldClose(window, GL_TRUE); // Doesn't work, because we don't have a window

	CoreEngine::GetCoreEngine()->RequestWindowClose();
}

const rendering::text::Font* engine::GameManager::CreateFont(int fontId, const std::string& fontTextureFileName, const std::string& fontMetaDataFileName)
{
	return CoreEngine::GetCoreEngine()->CreateFont(fontId, fontTextureFileName, fontMetaDataFileName);
}

const rendering::text::Font* engine::GameManager::GetFont(int fontId) const
{
	return CoreEngine::GetCoreEngine()->GetFont(fontId);
}

void engine::GameManager::LoadSoundEffect(const std::string& soundEffectFileName) const
{
	CoreEngine::GetCoreEngine()->GetAudioEngine().LoadSoundEffect(soundEffectFileName);
}

void engine::GameManager::PlaySoundEffect(const std::string& soundEffectFileName, math::Real volume, math::Real pitch) const
{
	CoreEngine::GetCoreEngine()->GetAudioEngine().PlaySoundEffect(soundEffectFileName, volume, pitch);
}

void engine::GameManager::CentralizeCursor() const
{
	CoreEngine::GetCoreEngine()->CentralizeCursor();
}

#ifdef ANT_TWEAK_BAR_ENABLED
void engine::GameManager::InitializeTweakBars()
{
	m_gameBar = TwNewBar("GameBar");
	//TwAddVarRW(m_gameBar, "currentCamera", TW_TYPE_UINT32, &m_currentCameraIndex, " label='Current camera' ");
	//TwSetParam(m_gameBar, "currentCamera", "max", TW_PARAM_INT32, 1, &m_cameraCountMinusOne);
	TwSetParam(m_gameBar, nullptr, "visible", TW_PARAM_CSTRING, 1, "true"); // Hide the bar at startup

	// TODO: Move these to PlayGameState InitializeTweakBars() method.
	//TwAddVarRO(m_gameBar, "ambientLight", TW_TYPE_COLOR4F, &m_ambientLightColor, " label='Color' group='Ambient light'");
	//TwAddVarRW(m_gameBar, "ambientLightDaytime", TW_TYPE_COLOR4F, &m_ambientDaytimeColor, " label='Daytime color' group='Ambient light'");
	//TwAddVarRW(m_gameBar, "ambientLightSunNearHorizon", TW_TYPE_COLOR4F, &m_ambientSunNearHorizonColor, " label='Sun near horizon color' group='Ambient light'");
	//TwAddVarRW(m_gameBar, "ambientLightNighttime", TW_TYPE_COLOR4F, &m_ambientNighttimeColor, " label='Nighttime color' group='Ambient light'");
}
#endif