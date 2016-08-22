#include "stdafx.h"
#include "GameManager.h"
#include "GameStateManager.h"
#include "CoreEngine.h"

#include "Rendering\Renderer.h"
#include "Rendering\Attenuation.h"
#include "Rendering\PointLight.h"
#include "Rendering\Texture.h"

#include "Math\Math.h"
#include "Math\Vector.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"
#include "Utility\Time.h"

#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBar\include\AntTweakBar.h"
#endif

#include <fstream>

Engine::GameManager* Engine::GameManager::s_gameManager = NULL;

/* static */ Engine::GameManager* Engine::GameManager::GetGameManager()
{
	return s_gameManager;
}

/* static */ void Engine::GameManager::LoadGame(GameManager* gameManager)
{
	if (gameManager == NULL)
	{
		EMERGENCY_LOG_ENGINE("Cannot load the game. Passed parameter is not a game manager object.");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(CoreEngine::GetCoreEngine()->GetThreadWindow());
	Engine::CoreEngine::GetCoreEngine()->InitGlew(); // glew init
	gameManager->Load();
}

Engine::GameManager::GameManager() :
	//Observer(),
	IUpdateable(),
	m_rootGameNode(),
	m_terrainNode(NULL),
	m_skyboxNode(NULL),
	m_waterNode(NULL),
	m_shaderFactory(CoreEngine::GetCoreEngine()->GetShadersDirectory()),
	m_textureFactory(CoreEngine::GetCoreEngine()->GetTexturesDirectory()),
	m_fontFactory(m_shaderFactory.GetShader(ShaderTypes::TEXT), CoreEngine::GetCoreEngine()->GetTexturesDirectory(), CoreEngine::GetCoreEngine()->GetFontsDirectory()),
	m_gameStateManager(NULL),
	m_isGameLoaded(false),
	m_skyboxAngle(REAL_ZERO, Math::Unit::RADIAN),
	m_skyboxAngleStep(GET_CONFIG_VALUE_ENGINE("skyboxAngleStep", 0.02f), Math::Unit::RADIAN),
	m_directionalLightsCount(0),
	m_lights(),
	m_directionalAndSpotLights(),
	m_pointLights(),
	m_cameras(),
	m_currentCameraIndex(0),
	m_emptyGameCommand(),
	m_actionsToGameCommandsMap(),
	//m_actionsToGameCommandsMap({ { Input::Actions::EMPTY, std::make_unique<EmptyGameCommand>() } }), // cannot do it this way since it requires copying the unique_ptr object
	m_actionsToGameNodesMap()
#ifdef ANT_TWEAK_BAR_ENABLED
	, m_gameBar(NULL),
	m_cameraCountMinusOne(-1)
#endif
#ifdef CALCULATE_MATH_STATS
	, m_classStats(STATS_STORAGE.GetClassStats("GameManager"))
#endif
{
	INFO_LOG_ENGINE("Game manager construction started");
	//rootGameNode = new GameNode();
	//CHECK_CONDITION_EXIT_ENGINE(rootGameNode != NULL, CRITICAL, "Root game node construction failed.");

	if (Engine::GameManager::s_gameManager != NULL)
	{
		ERROR_LOG_ENGINE("Constructor called when a singleton instance of CoreEngine class has already been created");
		SAFE_DELETE(Engine::GameManager::s_gameManager);
	}
	m_gameStateManager = new Engine::DefaultGameStateManager();

	m_actionsToGameCommandsMap.insert(std::make_pair(Actions::EMPTY, &m_emptyGameCommand));
	//m_actionsToGameCommandsMap.insert(std::make_pair(Input::Actions::Action::EMPTY, std::make_unique<EmptyGameCommand>()));

	Engine::GameManager::s_gameManager = this;
	DEBUG_LOG_ENGINE("Game manager construction finished");
}


Engine::GameManager::~GameManager(void)
{
	INFO_LOG_ENGINE("Game manager destruction started");
	//SAFE_DELETE(m_rootGameNode);
	SAFE_DELETE(m_gameStateManager);
	SAFE_DELETE(m_terrainNode);
	SAFE_DELETE(m_skyboxNode);
	DEBUG_LOG_ENGINE("Game manager destruction finished");
}

void Engine::GameManager::WindowResizeEvent(int width, int height)
{
	NOTICE_LOG_ENGINE("Window resize event (width = ", width, ", height = ", height, ")");
	// TODO: Check if core engine finds out about the resizing of the window.
}

void Engine::GameManager::CloseWindowEvent()
{
	NOTICE_LOG_ENGINE("Close window event");
	CoreEngine::GetCoreEngine()->Stop();
}

void Engine::GameManager::MousePosEvent(double xPos, double yPos)
{
	DEBUG_LOG_ENGINE("Mouse position event x=", xPos, ", y=", yPos);
}

void Engine::GameManager::ScrollEvent(double xOffset, double yOffset)
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

void Engine::GameManager::AddToSceneRoot(GameNode* child)
{
	m_rootGameNode.AddChild(child);
}

void Engine::GameManager::AddWaterNode(GameNode* waterNode)
{
	m_waterNode = waterNode;
}

void Engine::GameManager::AddBillboardsRenderer(GameNode* billboardsRenderer)
{
	//CoreEngine::GetCoreEngine()->AddBillboardNode(billboardNode);
	// TODO: Check conditions
	m_billboardsRenderers.push_back(billboardsRenderer);
}

void Engine::GameManager::AddGuiControl(const Rendering::Controls::GuiControl& guiControl)
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

void Engine::GameManager::AddLight(Rendering::Lighting::BaseLight* light)
{
	Rendering::Lighting::DirectionalLight* directionalLight = dynamic_cast<Rendering::Lighting::DirectionalLight*>(light);
	if (directionalLight != NULL)
	{
		INFO_LOG_RENDERING("Directional light with intensity = ", directionalLight->GetIntensity(), " is being added to directional / spot lights vector");
		++m_directionalLightsCount;
		m_directionalAndSpotLights.push_back(directionalLight);
	}
	else
	{
		Rendering::Lighting::SpotLight* spotLight = dynamic_cast<Rendering::Lighting::SpotLight*>(light);
		if (spotLight != NULL)
		{
			INFO_LOG_RENDERING("Spot light with intensity = ", spotLight->GetIntensity(), " is being added to directional / spot lights vector");
			m_directionalAndSpotLights.push_back(spotLight);
		}
		else
		{
			Rendering::Lighting::PointLight* pointLight = dynamic_cast<Rendering::Lighting::PointLight*>(light);
			if (pointLight != NULL)
			{
				INFO_LOG_RENDERING("Point light with intensity = ", pointLight->GetIntensity(), " is being added to point lights vector");
				m_pointLights.push_back(pointLight);
			}
			else
			{
				EMERGENCY_LOG_RENDERING("Adding the light of unknown type. It is neither a directional nor spot nor point light.");
			}
		}
	}
	m_lights.push_back(light);
}

void Engine::GameManager::AddTerrainNode(GameNode* terrainNode)
{
	//CoreEngine::GetCoreEngine()->AddTerrainNode(terrainNode);
	m_terrainNode = terrainNode;
}

void Engine::GameManager::AddSkyboxNode(GameNode* skyboxNode)
{
	//CoreEngine::GetCoreEngine()->AddSkyboxNode(skyboxNode);
	m_skyboxNode = skyboxNode;
}

void Engine::GameManager::AddParticleGenerator(ParticleGenerator* particleGenerator)
{
	m_particleGenerators.push_back(particleGenerator);
}

void Engine::GameManager::Input(const Engine::Input::MappedInput& input)
{
	for (Input::ActionsContainer::const_iterator actionItr = input.m_actions.begin(); actionItr != input.m_actions.end(); ++actionItr)
	{
		const Actions::Action action = *actionItr;
		ActionsToGameCommandsMap::const_iterator gameCommandItr = m_actionsToGameCommandsMap.find(action);
		if (gameCommandItr != m_actionsToGameCommandsMap.end())
		{
			gameCommandItr->second->Execute(this);
		}
		else
		{
			m_gameStateManager->Handle(action);
			for (std::list<GameNode*>::iterator gameNodeItr = m_actionsToGameNodesMap[action].begin(); gameNodeItr != m_actionsToGameNodesMap[action].end(); ++gameNodeItr)
			{
				(*gameNodeItr)->Handle(action);
			}
		}
	}
	for (Input::StatesContainer::const_iterator stateItr = input.m_states.begin(); stateItr != input.m_states.end(); ++stateItr)
	{
		const States::State state = *stateItr;
		DEBUG_LOG_ENGINE("Handling the state ", state);
		StatesToGameCommandsMap::const_iterator gameCommandItr = m_statesToGameCommandsMap.find(state);
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
	for (Input::RangesContainer::const_iterator rangeItr = input.m_ranges.begin(); rangeItr != input.m_ranges.end(); ++rangeItr)
	{
		// TODO: Ranges processing.
		const Ranges::Range range = rangeItr->first;
		const Math::Real value = rangeItr->second;
		m_gameStateManager->Handle(range, value);
	}
}

//void Engine::GameManager::Notify(GameNode* gameNode, Actions::Action action /*const GameEvent& gameEvent*/) const
//{
//}

void Engine::GameManager::Render(Rendering::Renderer* renderer) const
{
	m_gameStateManager->Render(renderer);
}

void Engine::GameManager::SetTransition(Engine::GameStateTransitioning::GameStateTransition* gameStateTransition)
{
	m_gameStateManager->SetTransition(gameStateTransition);
}

void Engine::GameManager::PerformStateTransition()
{
	m_gameStateManager->PerformStateTransition();
}

void Engine::GameManager::PopState()
{
	m_gameStateManager->Pop();
}

void Engine::GameManager::RequestGameQuit() const
{
	// TODO: Ask the user to save the game if the inGameState can be found in the game states stack
	//glfwSetWindowShouldClose(window, GL_TRUE); // Doesn't work, because we don't have a window

	CoreEngine::GetCoreEngine()->RequestWindowClose();
}

const Rendering::Text::Font* Engine::GameManager::GetFont(Rendering::Text::FontTypes::FontType fontType)
{
	return m_fontFactory.GetFont(fontType);
}

const Rendering::Shader& Engine::GameManager::GetAmbientShader(const Rendering::FogEffect::FogInfo& fogInfo) const
{
	START_PROFILING("");
	if (fogInfo.IsEnabled()) // if (fogInfo != NULL)
	{
		//DEBUG_LOG_RENDERING("Fog fall-off type: ", m_fogFallOffType, ". Fog distance calculation type: ", m_fogCalculationType);

		// TODO: A very ugly way. If we decide to add more fog fall off or calculation types then we will surely have a big problem in here.
		if (fogInfo.GetFallOffType() == Rendering::FogEffect::LINEAR)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING("");
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_FOG_LINEAR_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING("");
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_FOG_LINEAR_RANGE_BASED);
			}
		}
		else if (fogInfo.GetFallOffType() == Rendering::FogEffect::EXPONENTIAL)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING("");
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING("");
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED);
			}
		}
	}
	STOP_PROFILING("");
	return m_shaderFactory.GetShader(ShaderTypes::AMBIENT);
}

const Rendering::Shader& Engine::GameManager::GetAmbientTerrainShader(const Rendering::FogEffect::FogInfo& fogInfo) const
{
	START_PROFILING("");
	if (fogInfo.IsEnabled())
	{
		//DEBUG_LOG_RENDERING("Fog fall-off type: ", m_fogFallOffType, ". Fog distance calculation type: ", m_fogCalculationType);
		// TODO: A very ugly way. If we decide to add more fog fall off or calculation types then we will surely have a big problem in here.
		if (fogInfo.GetFallOffType() == Rendering::FogEffect::LINEAR)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING("");
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING("");
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED);
			}
		}
		else if (fogInfo.GetFallOffType() == Rendering::FogEffect::EXPONENTIAL)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING("");
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING("");
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED);
			}
		}
	}
	STOP_PROFILING("");
	return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_TERRAIN);
}

void Engine::GameManager::AddCamera(Rendering::Camera* camera)
{
	if (m_cameras.empty())
	{
		camera->Activate();
	}
	m_cameras.push_back(camera);
#ifdef ANT_TWEAK_BAR_ENABLED
	++m_cameraCountMinusOne;
#endif
}

unsigned int Engine::GameManager::SetCurrentCamera(unsigned int cameraIndex)
{
	CHECK_CONDITION_RENDERING((cameraIndex >= 0) && (cameraIndex < m_cameras.size()), ERR, "Incorrect current camera index. Passed ",
		cameraIndex, " when the correct range is (", 0, ", ", m_cameras.size(), ").");
	m_cameras[m_currentCameraIndex]->Deactivate();
	m_currentCameraIndex = cameraIndex;
	m_cameras[m_currentCameraIndex]->Activate();
#ifndef ANT_TWEAK_BAR_ENABLED
	NOTICE_LOG_RENDERING("Switched to camera #", m_currentCameraIndex + 1);
	//DEBUG_LOG_RENDERING("Current camera parameters: ", m_cameras[m_currentCameraIndex]->ToString());
#endif
	return m_currentCameraIndex;
}

unsigned int Engine::GameManager::NextCamera()
{
	if (m_currentCameraIndex == static_cast<int>(m_cameras.size()) - 1)
	{
		m_currentCameraIndex = -1;
	}
	return SetCurrentCamera(m_currentCameraIndex + 1);
}

unsigned int Engine::GameManager::PrevCamera()
{
	if (m_currentCameraIndex == 0)
	{
		m_currentCameraIndex = m_cameras.size();
	}
	return SetCurrentCamera(m_currentCameraIndex - 1);
}

#ifdef ANT_TWEAK_BAR_ENABLED
void Engine::GameManager::InitializeTweakBars()
{
	m_gameBar = TwNewBar("GameBar");
	TwAddVarRW(m_gameBar, "currentCamera", TW_TYPE_UINT32, &m_currentCameraIndex, " label='Current camera' ");
	TwSetParam(m_gameBar, "currentCamera", "max", TW_PARAM_INT32, 1, &m_cameraCountMinusOne);
	TwSetParam(m_gameBar, NULL, "visible", TW_PARAM_CSTRING, 1, "true"); // Hide the bar at startup

	// TODO: Move these to PlayGameState InitializeTweakBars() method.
	//TwAddVarRO(m_gameBar, "ambientLight", TW_TYPE_COLOR4F, &m_ambientLightColor, " label='Color' group='Ambient light'");
	//TwAddVarRW(m_gameBar, "ambientLightDaytime", TW_TYPE_COLOR4F, &m_ambientDaytimeColor, " label='Daytime color' group='Ambient light'");
	//TwAddVarRW(m_gameBar, "ambientLightSunNearHorizon", TW_TYPE_COLOR4F, &m_ambientSunNearHorizonColor, " label='Sun near horizon color' group='Ambient light'");
	//TwAddVarRW(m_gameBar, "ambientLightNighttime", TW_TYPE_COLOR4F, &m_ambientNighttimeColor, " label='Nighttime color' group='Ambient light'");
}
#endif