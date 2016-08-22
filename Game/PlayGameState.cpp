#include "PlayGameState.h"
#include "Def.h"
#include "PlayMenuGameState.h"

#include "Engine\GameManager.h"
#include "Engine\CoreEngine.h"
#include "Engine\GameNode.h"
#include "Engine\ParticleGenerator.h"

#include "Rendering\Shader.h"

#include "Math\FloatingPoint.h"

#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

//#define DRAW_GAME_TIME // TODO: Investigate this macro

Game::PlayGameState::PlayGameState(Engine::GameManager* gameManager, const std::string& inputMappingContextName) :
	GameState(inputMappingContextName),
	m_isMouseLocked(false),
	m_gameManager(gameManager),
	m_mousePicker(),
	m_clockSpeed(GET_CONFIG_VALUE_GAME("clockSpeed", REAL_ONE)),
	m_inGameDateTime(GET_CONFIG_VALUE_GAME("inGameYear", 2016), GET_CONFIG_VALUE_GAME("inGameMonth", 5),
		GET_CONFIG_VALUE_GAME("inGameDay", 22), GET_CONFIG_VALUE_GAME("inGameHour", 9), GET_CONFIG_VALUE_GAME("inGameMinute", 30), GET_CONFIG_VALUE_GAME("inGameSecond", 30)),
	m_inGameTimeGuiButton("9:00:00", gameManager->GetFont(Rendering::Text::FontTypes::CANDARA), GET_CONFIG_VALUE_GAME("fontSizeInGameTime", 2.5f), NULL,
		Math::Vector2D(GET_CONFIG_VALUE_GAME("screenPositionInGameTimeX", 0.0f), GET_CONFIG_VALUE_GAME("screenPositionInGameTimeY", 0.0f)), GET_CONFIG_VALUE_GAME("maxLineLengthInGameTime", 0.5f),
		Rendering::Color(GET_CONFIG_VALUE_GAME("colorInGameTimeRed", 1.0f), GET_CONFIG_VALUE_GAME("colorInGameTimeGreen", 0.0f), GET_CONFIG_VALUE_GAME("colorInGameTimeBlue", 0.0f)),
		Rendering::Color(GET_CONFIG_VALUE_GAME("outlineColorInGameTimeRed", 0.0f), GET_CONFIG_VALUE_GAME("outlineColorInGameTimeGreen", 1.0f), GET_CONFIG_VALUE_GAME("outlineColorInGameTimeBlue", 0.0f)),
		Math::Vector2D(GET_CONFIG_VALUE_GAME("offsetInGameTimeX", 0.005f), GET_CONFIG_VALUE_GAME("offsetInGameTimeY", 0.005f)), GET_CONFIG_VALUE_GAME("isCenteredInGameTime", false),
		GET_CONFIG_VALUE_GAME("characterWidthInGameTime", 0.5f), GET_CONFIG_VALUE_GAME("characterEdgeTransitionWidthInGameTime", 0.1f), GET_CONFIG_VALUE_GAME("borderWidthInGameTime", 0.4f),
		GET_CONFIG_VALUE_GAME("borderEdgeTransitionWidthInGameTime", 0.1f)),
	m_dayNightMixFactor(REAL_ZERO),
	m_ambientDaytimeColor(GET_CONFIG_VALUE_GAME("ambientDaytimeColorRed", 0.2f),
		GET_CONFIG_VALUE_GAME("ambientDaytimeColorGreen", 0.2f),
		GET_CONFIG_VALUE_GAME("ambientDaytimeColorBlue", 0.2f)),
	m_ambientSunNearHorizonColor(GET_CONFIG_VALUE_GAME("ambientSunNearHorizonColorRed", 0.1f),
		GET_CONFIG_VALUE_GAME("ambientSunNearHorizonColorGreen", 0.1f),
		GET_CONFIG_VALUE_GAME("ambientSunNearHorizonColorBlue", 0.1f)),
	m_ambientNighttimeColor(GET_CONFIG_VALUE_GAME("ambientNighttimeColorRed", 0.02f),
		GET_CONFIG_VALUE_GAME("ambientNighttimeColorGreen", 0.02f),
		GET_CONFIG_VALUE_GAME("ambientNighttimeColorBlue", 0.02f)),
	m_ambientLightColor(m_ambientDaytimeColor)
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("PlayGameState"))
#endif
{
}

Game::PlayGameState::~PlayGameState(void)
{
}

void Game::PlayGameState::Entered()
{
	START_PROFILING("");
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("PLAY game state has been placed in the game state manager");
	//tthread::thread t(GameManager::Load, GameManager::GetGameManager());
	CHECK_CONDITION_GAME(m_gameManager->IsGameLoaded(), Utility::ERR, "PLAY game state has been placed in the game state manager before loading the game.");
#ifdef ANT_TWEAK_BAR_ENABLED
	Engine::CoreEngine::GetCoreEngine()->InitializeGameTweakBars();
#endif

#ifdef CALCULATE_STATS
	CoreEngine::GetCoreEngine()->StartSamplingSpf();
#endif
	STOP_PROFILING("");
}

void Game::PlayGameState::Leaving()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("PLAY game state is about to be removed from the game state manager");
#ifdef CALCULATE_STATS
	Rendering::CoreEngine::GetCoreEngine()->StopSamplingSpf();
#endif
}

void Game::PlayGameState::Obscuring()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of PLAY game state");
}

void Game::PlayGameState::Revealed()
{
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("PLAY game state has become the topmost game state in the game state manager's stack");
}

void Game::PlayGameState::Handle(Engine::Actions::Action action)
{
	START_PROFILING("");
	switch (action)
	{
	case Engine::Actions::SHOW_PLAY_MENU:
		m_gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(m_gameManager->GetPlayMainMenuGameState(), Engine::GameStateTransitioning::PUSH, Engine::GameStateModality::EXCLUSIVE));
		break;
	default:
		INFO_LOG_GAME("Action ", action, " is not supported by the PLAY game state.");
	}
	STOP_PROFILING("");
}

void Game::PlayGameState::Handle(Engine::States::State state)
{
}

void Game::PlayGameState::Handle(Engine::Ranges::Range range, Math::Real value)
{
}

void Game::PlayGameState::MouseButtonEvent(int button, int action, int mods)
{
	START_PROFILING("");
	m_gameManager->GetRootGameNode().MouseButtonEvent(button, action, mods);
	//switch (action)
	//{
	//case GLFW_PRESS:
	//	m_isMouseLocked = ! m_isMouseLocked;
	//	if (m_isMouseLocked)
	//	{
	//		Rendering::CoreEngine::GetCoreEngine()->CentralizeCursor();
	//	}
	//	INFO_LOG_GAME("Mouse button pressed: button=", button, "\t mods=", mods);
	//	break;
	//case GLFW_RELEASE:
	//	INFO_LOG_GAME("Mouse button released: button=", button, "\t mods=", mods);
	//	break;
	//default:
	//	WARNING_LOG_GAME("Unknown action performed with the mouse. Button=", button, "\t action=", action, "\t mods=", mods);
	//}
	STOP_PROFILING("");
}

void Game::PlayGameState::MousePosEvent(double xPos, double yPos)
{
	START_PROFILING("");
	DEBUG_LOG_GAME("Cursor position = (", xPos, ", ", yPos, ")");

	//const Rendering::Camera& currentCamera = Engine::CoreEngine::GetCoreEngine()->GetCurrentCamera();
	//m_mousePicker.CalculateCurrentRay(xPos, yPos, currentCamera.GetProjection(), currentCamera.GetViewMatrix());

	//m_gameManager->GetRootGameNode().MousePosEvent(xPos, yPos);


	//if (!m_isMouseLocked)
	//{
	//	STOP_PROFILING;
	//	return;
	//}

	//int width = Rendering::CoreEngine::GetCoreEngine()->GetWindowWidth();
	//int height = Rendering::CoreEngine::GetCoreEngine()->GetWindowHeight();
	//Math::Vector2D centerPosition(static_cast<Math::Real>(width) / 2, static_cast<Math::Real>(height) / 2);
	//Math::Vector2D deltaPosition(static_cast<Math::Real>(xPos), static_cast<Math::Real>(yPos));
	//deltaPosition -= centerPosition;
	//
	//bool rotX = ! Math::AlmostEqual(deltaPosition.GetX(), REAL_ZERO);
	//bool rotY = ! Math::AlmostEqual(deltaPosition.GetY(), REAL_ZERO);

	//if (rotX || rotY)
	//{
	//	Rendering::Transform& transform = Rendering::CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera().GetTransform();
	//	const Math::Real sensitivity = static_cast<Math::Real>(Rendering::Camera::GetSensitivity());
	//	if (rotX)
	//	{
	//		transform.Rotate(Math::Vector3D(0, 1, 0), Math::Angle(deltaPosition.GetX() * sensitivity));
	//	}
	//	if (rotY)
	//	{
	//		transform.Rotate(transform.GetRot().GetRight(), Math::Angle(deltaPosition.GetY() * sensitivity));
	//	}
	//	Rendering::CoreEngine::GetCoreEngine()->CentralizeCursor();
	//}
	STOP_PROFILING("");
}

void Game::PlayGameState::ScrollEvent(double xOffset, double yOffset)
{
	START_PROFILING("");
	m_gameManager->GetRootGameNode().ScrollEvent(xOffset, yOffset);
	STOP_PROFILING("");
}

void Game::PlayGameState::Render(Rendering::Renderer* renderer) const
{
	// TODO: Updating the state of the rendering engine (e.g. the values of some of its member variables)
	// in this function is not good. This should be done in the Update function (or maybe not?).
	START_PROFILING("");
	CHECK_CONDITION_EXIT_GAME(renderer != NULL, Utility::CRITICAL, "Cannot render the game. The rendering engine is NULL.");
	DEBUG_LOG_GAME("PLAY game state rendering");

	renderer->InitRenderScene(m_ambientLightColor, m_dayNightMixFactor);
	
	RenderWaterTextures(renderer);

	renderer->BindDisplayTexture();
	renderer->ClearScreen();
	renderer->SetCurrentCamera(m_gameManager->GetCurrentCamera());

	RenderSceneWithAmbientLight(renderer);
	//m_gameManager->GetRootGameNode().Render(shader, renderer);
	//RenderSceneWithPointLights(renderer); // Point light rendering
	RenderSceneWithDirectionalAndSpotLights(renderer); // Directional and spot light rendering

	RenderWaterNodes(renderer);

	RenderBillboardNodes(renderer);

	RenderSkybox(renderer);

	RenderParticles(renderer);

#ifdef DEBUG_RENDERING_ENABLED
	renderer->RenderDebugNodes(m_gameManager->GetGuiShader());
#endif
	
	renderer->FinalizeRenderScene((renderer->GetAntiAliasingMethod() == Rendering::Aliasing::FXAA) ?
		m_gameManager->GetShader(Engine::ShaderTypes::FILTER_FXAA) :
		m_gameManager->GetShader(Engine::ShaderTypes::FILTER_NULL));

#ifdef DRAW_GAME_TIME
	renderer->RenderGuiControl(m_inGameTimeGuiButton, m_gameManager->GetGuiTextShader());
#endif

	STOP_PROFILING("");
}

void Game::PlayGameState::RenderSceneWithAmbientLight(Rendering::Renderer* renderer) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(renderer->IsAmbientLightEnabled(), Utility::Logging::DEBUG, "Ambient light is disabled by the rendering engine.");
	const Rendering::Shader& ambientShader = m_gameManager->GetAmbientShader(renderer->GetFogInfo());
	const Rendering::Shader& ambientTerrainShader = m_gameManager->GetAmbientTerrainShader(renderer->GetFogInfo());
	m_gameManager->GetRootGameNode().Render(ambientShader, renderer);
	CHECK_CONDITION_GAME(m_gameManager->GetTerrainNode() != NULL, Utility::ERR, "Cannot render terrain. There are no terrain nodes registered.");
	m_gameManager->GetTerrainNode()->Render(ambientTerrainShader, renderer);
}

void Game::PlayGameState::RenderSceneWithPointLights(Rendering::Renderer* renderer) const
{
	if (!Rendering::Lighting::PointLight::ArePointLightsEnabled())
	{
		DEBUG_LOG_GAME("All point lights are disabled");
		return;
	}

	for (size_t i = 0; i < m_gameManager->GetPointLightsCount(); ++i)
	{
		const Rendering::Lighting::PointLight* currentPointLight = renderer->SetCurrentPointLight(m_gameManager->GetPointLight(i));
		if (currentPointLight->IsEnabled())
		{
			DEBUG_LOG_GAME("Point light at index ", i, " is disabled");
			continue;
		}
		m_gameManager->GetRootGameNode().Render(currentPointLight->GetShader(), renderer);
		m_gameManager->GetTerrainNode()->Render(currentPointLight->GetTerrainShader(), renderer);
	}
}

void Game::PlayGameState::RenderSceneWithDirectionalAndSpotLights(Rendering::Renderer* renderer) const
{
	START_PROFILING("");
	size_t lightsCount = m_gameManager->GetDirectionalLightsCount() + m_gameManager->GetSpotLightsCount();
	for (size_t i = 0; i < lightsCount; ++i)
	{
		const Rendering::Lighting::BaseLight* currentLight = renderer->SetCurrentLight(m_gameManager->GetLight(i));
		if (!currentLight->IsEnabled())
		{
			DEBUG_LOG_GAME("Light at index ", i, " is disabled");
			continue;
		}
		if (renderer->InitShadowMap())
		{
			DEBUG_LOG_GAME("Shadow mapping enabled for light ", i);
			// Render scene using shadow mapping shader
			m_gameManager->GetRootGameNode().Render(m_gameManager->GetShadowMapShader(), renderer);
			m_gameManager->GetTerrainNode()->Render(m_gameManager->GetShadowMapShader(), renderer); // TODO: Probably unnecessary
			renderer->FinalizeShadowMapRendering(m_gameManager->GetShader(Engine::ShaderTypes::FILTER_GAUSSIAN_BLUR));
		}

		renderer->InitLightRendering();
		// TODO: Render scene with light is not ready. Check the function Renderer::RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode, bool isCastingShadowsEnabled /* = true */).
		m_gameManager->GetRootGameNode().Render(currentLight->GetShader(), renderer);
		m_gameManager->GetTerrainNode()->Render(currentLight->GetTerrainShader(), renderer);
		renderer->FinalizeLightRendering();
	}
	STOP_PROFILING("");
}

void Game::PlayGameState::RenderSkybox(Rendering::Renderer* renderer) const
{
	START_PROFILING("");
	DEBUG_LOG_GAME("Skybox rendering started");

	Engine::GameNode* skyboxNode = m_gameManager->GetSkyboxNode();
	skyboxNode->GetTransform().SetPos(renderer->GetCurrentCamera().GetPos());
	// TODO: Rotating the skybox
	//skyboxNode->GetTransform().SetRot(Math::Quaternion(Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), skyboxNode->GetTransfom));
	//m_skyboxAngle += m_skyboxAngleStep;
	//if (m_fogEnabled)
	//{
	//	STOP_PROFILING;
	//	return;
	//}
	
	//glDisable(GL_DEPTH_TEST);
	renderer->SetCullFaceFront();
	/**
	 * By default (GL_LESS) we tell OpenGL that an incoming fragment wins the depth test if its Z value is less than the stored one.
	 * However, in the case of a skybox the Z value is always the far Z. The far Z is clipped when the depth test function is set to "less than".
	 * To make it part of the scene we change the depth function to "less than or equal".
	 */
	renderer->SetDepthFuncLessOrEqual();
	skyboxNode->Render(m_gameManager->GetSkyboxShader(), renderer);
	renderer->SetDepthFuncDefault();
	renderer->SetCullFaceDefault();
	//glEnable(GL_DEPTH_TEST);
	//Rendering::CheckErrorCode("Renderer::Render", "Rendering skybox");

	STOP_PROFILING("");
}

void Game::PlayGameState::RenderWaterTextures(Rendering::Renderer* renderer) const
{
	START_PROFILING("");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != NULL, Utility::DEBUG, "There are no water nodes registered in the rendering engine");
	// TODO: For now we only support one water node (you can see that in the "distance" calculation). In the future there might be more.

	RenderWaterReflectionTexture(renderer);
	RenderWaterRefractionTexture(renderer);
	
	renderer->DisableClippingPlanes();
	STOP_PROFILING("");
}

void Game::PlayGameState::RenderWaterReflectionTexture(Rendering::Renderer* renderer) const
{
	START_PROFILING("");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != NULL, Utility::DEBUG, "There are no water nodes registered in the rendering engine");
	
	// TODO: The camera should be accessible from the game manager. It shouldn't be necessary to access them via rendering engine.
	Rendering::Camera& currentCamera = renderer->GetCurrentCamera();
	const Math::Real cameraHeight = currentCamera.GetPos().GetY();
	Math::Real distance = 2.0f * (cameraHeight - m_gameManager->GetWaterNode()->GetTransform().GetTransformedPos().GetY());
	currentCamera.GetPos().SetY(cameraHeight - distance); // TODO: use m_altCamera instead of the main camera.
	currentCamera.GetRot().InvertPitch();

	renderer->EnableWaterReflectionClippingPlane(-m_gameManager->GetWaterNode()->GetTransform().GetTransformedPos().GetY() + 0.1f /* we add 0.1f to remove some glitches on the water surface */);
	renderer->BindWaterReflectionTexture();
	renderer->ClearScreen(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE);

	renderer->SetDepthTest(false);
	RenderSkybox(renderer);
	RenderSceneWithAmbientLight(renderer);

	//RenderSceneWithPointLights(renderer);
	//for (std::vector<Rendering::Lighting::BaseLight*>::iterator lightItr = m_directionalAndSpotLights.begin(); lightItr != m_directionalAndSpotLights.end(); ++lightItr)
	//{
	//	m_currentLight = (*lightItr);
	//	if (!m_currentLight->IsEnabled())
	//	{
	//		continue;
	//	}
	//	RenderSceneWithLight(m_currentLight, gameNode, false);
	//}
	//SetVector3D("inverseFilterTextureSize", Vector3D(REAL_ONE / m_waterReflectionTexture->GetWidth(), REAL_ONE / m_waterReflectionTexture->GetHeight(), REAL_ZERO));

	renderer->SetDepthTest(true);

	//if (Rendering::antiAliasingMethod == Rendering::Aliasing::FXAA)
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_FXAA), m_waterReflectionTexture, NULL);
	//}
	//else
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_NULL), m_waterReflectionTexture, NULL);
	//}

	//BindAsRenderTarget();
	
	currentCamera.GetPos().SetY(cameraHeight); // TODO: use m_altCamera instead of the main camera.
	currentCamera.GetRot().InvertPitch();

	STOP_PROFILING("");
}

void Game::PlayGameState::RenderWaterRefractionTexture(Rendering::Renderer* renderer) const
{
	START_PROFILING("");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != NULL, Utility::DEBUG, "There are no water nodes registered in the rendering engine");
	
	renderer->EnableWaterRefractionClippingPlane(m_gameManager->GetWaterNode()->GetTransform().GetTransformedPos().GetY());
	renderer->BindWaterRefractionTexture();
	renderer->ClearScreen(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE);
	
	//glDisable(GL_DEPTH_TEST);
	RenderSkybox(renderer);
	RenderSceneWithAmbientLight(renderer);
	
	//RenderSceneWithPointLights(renderer);
	//for (std::vector<Lighting::BaseLight*>::iterator lightItr = m_directionalAndSpotLights.begin(); lightItr != m_directionalAndSpotLights.end(); ++lightItr)
	//{
	//	m_currentLight = (*lightItr);
	//	if (!m_currentLight->IsEnabled())
	//	{
	//		continue;
	//	}
	//
	//	RenderSceneWithLight(m_currentLight, gameNode, false);
	//}
	//SetVector3D("inverseFilterTextureSize", Vector3D(REAL_ONE / m_waterReflectionTexture->GetWidth(), REAL_ONE / m_waterReflectionTexture->GetHeight(), REAL_ZERO));
	
	//glEnable(GL_DEPTH_TEST);
	
	//if (Rendering::antiAliasingMethod == Rendering::Aliasing::FXAA)
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_FXAA), m_waterReflectionTexture, NULL);
	//}
	//else
	//{
	//	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_NULL), m_waterReflectionTexture, NULL);
	//}
	
	//BindAsRenderTarget();
	
	STOP_PROFILING("");
}

void Game::PlayGameState::RenderWaterNodes(Rendering::Renderer* renderer) const
{
	START_PROFILING("");
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_gameManager->GetWaterNode() != NULL, Utility::Logging::DEBUG, "There are no water nodes registered in the rendering engine");
	renderer->InitWaterNodesRendering();

	// TODO: In the future there might be more than one water node.
	//for (std::vector<GameNode*>::const_iterator waterNodeItr = m_waterNodes.begin(); waterNodeItr != m_waterNodes.end(); ++waterNodeItr)
	//{
	//	if (m_waterLightReflectionEnabled)
	//	{
	//		(*waterNodeItr)->Render(waterShader, this);
	//	}
	//	else
	//	{
	//		(*waterNodeItr)->Render(waterNoDirectionalLightShader, this);
	//	}
	//}
	m_gameManager->GetWaterNode()->Render(m_gameManager->GetWaterShader(renderer), renderer);
	STOP_PROFILING("");
}

void Game::PlayGameState::RenderBillboardNodes(Rendering::Renderer* renderer) const
{
	START_PROFILING("");
	DEBUG_LOG_GAME("Rendering billboards started");
	//renderer->SetDepthTest(false);
	renderer->SetBlendingEnabled(true);
	//GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
	// GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE,
	// GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR, GL_SRC1_ALPHA, and GL_ONE_MINUS_SRC1_ALPHA
	renderer->SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (std::vector<Engine::GameNode*>::const_iterator billboardsRendererItr = m_gameManager->GetBillboardRenderers().begin(); billboardsRendererItr != m_gameManager->GetBillboardRenderers().end(); ++billboardsRendererItr)
	{
		(*billboardsRendererItr)->Render(m_gameManager->GetBillboardShader(), renderer);
	}
	//renderer->SetDepthTest(true);
	renderer->SetBlendingEnabled(false);
	//renderer->SetBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	STOP_PROFILING("");
}

void Game::PlayGameState::RenderParticles(Rendering::Renderer* renderer) const
{
	START_PROFILING("");
	DEBUG_LOG_GAME("Rendering particles started");
	for (std::vector<Engine::ParticleGenerator*>::const_iterator particleGeneratorItr = m_gameManager->GetParticleGenerators().begin(); particleGeneratorItr != m_gameManager->GetParticleGenerators().end(); ++particleGeneratorItr)
	{
		//if (!(*particleGeneratorItr)->GetTexture()->IsAdditive())
		//{
			(*particleGeneratorItr)->SortParticles(renderer->GetCurrentCamera().GetPos());
		//}
		renderer->RenderParticles(m_gameManager->GetParticleShader(), (*particleGeneratorItr)->GetTexture(), (*particleGeneratorItr)->GetParticles(), (*particleGeneratorItr)->GetAliveParticlesCount());
	}
	STOP_PROFILING("");
}

void Game::PlayGameState::Update(Math::Real elapsedTime)
{
	START_PROFILING("");
	DEBUG_LOG_GAME("PLAY game state updating");
	Engine::GameManager::GetGameManager()->GetRootGameNode().Update(elapsedTime);

	//EMERGENCY_LOG_GAME("Elapsed time: ", elapsedTime * 1000.0f, " [ms]");
	m_inGameDateTime += Utility::Timing::TimeSpan(elapsedTime * m_clockSpeed * 1000.0f, Utility::Timing::MILLISECOND);
	//CRITICAL_LOG_GAME("In-game time: ", m_inGameDateTime.ToDateString());
	m_inGameTimeGuiButton.SetText(m_inGameDateTime.ToString());

	//AdjustAmbientLightAccordingToCurrentTime();

	STOP_PROFILING("");
}

//void Game::PlayGameState::AdjustAmbientLightAccordingToCurrentTime()
//{
//	START_PROFILING("");
//	switch (dayTime)
//	{
//	case Utility::Timing::NIGHT:
//		dayNightMixFactor = REAL_ZERO;
//		m_ambientLightColor = m_ambientNighttimeColor;
//		break;
//	case Utility::Timing::BEFORE_DAWN:
//		dayNightMixFactor = REAL_ZERO;
//		m_ambientLightColor = m_ambientNighttimeColor.Lerp(m_ambientSunNearHorizonColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	case Utility::Timing::SUNRISE:
//		dayNightMixFactor = dayTimeTransitionFactor;
//		m_ambientLightColor = m_ambientSunNearHorizonColor.Lerp(m_ambientDaytimeColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	case Utility::Timing::DAY:
//		dayNightMixFactor = REAL_ONE;
//		m_ambientLightColor = m_ambientDaytimeColor;
//		break;
//	case Utility::Timing::SUNSET:
//		dayNightMixFactor = dayTimeTransitionFactor;
//		m_ambientLightColor = m_ambientSunNearHorizonColor.Lerp(m_ambientDaytimeColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	case Utility::Timing::AFTER_DUSK:
//		dayNightMixFactor = REAL_ZERO;
//		m_ambientLightColor = m_ambientNighttimeColor.Lerp(m_ambientSunNearHorizonColor, dayTimeTransitionFactor); // move copy assignment
//		break;
//	}
//	STOP_PROFILING("");
//}