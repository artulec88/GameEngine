#include "PlayGameState.h"
#include "PlayMenuGameState.h"

#include "Engine\GameManager.h"
#include "Engine\CoreEngine.h"
#include "Engine\GameNode.h"
#include "Engine\ParticleGenerator.h"
#include "Engine\LightBuilder.h"
#include "Engine\LightBuilder_impl.h"

#include "Rendering\Shader.h"

#include "Math\FloatingPoint.h"

#include "Utility\BuilderDirector.h"
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
	m_ambientLightColor(m_ambientDaytimeColor),
	m_directionalLightsCount(0),
	m_lights(),
	m_directionalAndSpotLights(),
	m_pointLights()
#ifdef PROFILING_GAME_MODULE_ENABLED
	,m_classStats(STATS_STORAGE.GetClassStats("PlayGameState"))
#endif
{
}

Game::PlayGameState::~PlayGameState(void)
{
}

void Game::PlayGameState::Entered()
{
	START_PROFILING_GAME(true, "");

	AddLights(); // Adding all kinds of light (directional, point, spot)

	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("PLAY game state has been placed in the game state manager");
	//tthread::thread t(GameManager::Load, GameManager::GetGameManager());
	CHECK_CONDITION_GAME(m_gameManager->IsGameLoaded(), Utility::Logging::ERR, "PLAY game state has been placed in the game state manager before loading the game.");
#ifdef ANT_TWEAK_BAR_ENABLED
	Engine::CoreEngine::GetCoreEngine()->InitializeGameTweakBars();
#endif

#ifdef CALCULATE_STATS
	CoreEngine::GetCoreEngine()->StartSamplingSpf();
#endif
	STOP_PROFILING_GAME("");
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

void Game::PlayGameState::AddLights()
{
	START_PROFILING_GAME(true, "");
	AddDirectionalLight(); // Adding directional light (if enabled)
	AddPointLights();
	AddSpotLights();
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::AddDirectionalLight()
{
	// TODO: For now we only check if directionalLightsCount is zero or not.
	// In the future there might be many directional lights enabled (?)
	int directionalLightsCount = GET_CONFIG_VALUE_GAME("directionalLightsCount", 1);
	if (directionalLightsCount == 0)
	{
		NOTICE_LOG_GAME("Directional lights disabled");
		return;
	}
	NOTICE_LOG_GAME("Directional lights enabled");

	Engine::DirectionalLightBuilder directionalLightBuilder(m_gameManager->GetShaderFactory());
	Utility::BuilderDirector<Rendering::Lighting::DirectionalLight> lightBuilderDirector(directionalLightBuilder);
	lightBuilderDirector.Construct();
	Rendering::Lighting::DirectionalLight* directionalLight = directionalLightBuilder.Get();
	if (directionalLight != NULL)
	{
		INFO_LOG_RENDERING("Directional light with intensity = ", directionalLight->GetIntensity(), " is being added to directional / spot lights vector");
		++m_directionalLightsCount;
		m_directionalAndSpotLights.push_back(directionalLight);
		m_lights.push_back(directionalLight);
	}
}

void Game::PlayGameState::AddPointLights()
{
	const int pointLightsCount = GET_CONFIG_VALUE_GAME("pointLightsCount", 1);
	if (pointLightsCount > 0)
	{
		DEBUG_LOG_GAME("Creating ", pointLightsCount, " point lights");
		Engine::PointLightBuilder pointLightBuilder(m_gameManager->GetShaderFactory());
		Utility::BuilderDirector<Rendering::Lighting::PointLight> lightBuilderDirector(pointLightBuilder);
		for (int i = 0; i < pointLightsCount; ++i)
		{
			pointLightBuilder.SetLightIndex(i);
			lightBuilderDirector.Construct();
			Rendering::Lighting::PointLight* pointLight = pointLightBuilder.Get();
			
			if (pointLight != NULL)
			{
				INFO_LOG_RENDERING("Point light with intensity = ", pointLight->GetIntensity(), " is being added to point lights vector");
				m_pointLights.push_back(pointLight);
				m_lights.push_back(pointLight);
			}

			//GameNode* bulbNode = new GameNode();
			//bulbNode->AddComponent(new MeshRenderer(new Mesh("Bulb\\Bulb.obj") /* new Mesh("PointLight.obj") */, new Material(new Texture("PointLight.png"), 1.0f, 8.0f)));
			//bulbNode->GetTransform().SetPos(REAL_ZERO, REAL_ONE, REAL_ZERO);
			//bulbNode->GetTransform().SetScale(5.0f);
			//pointLightNode->AddChild(bulbNode);
		}
		NOTICE_LOG_GAME(pointLightsCount, " point lights created");
	}
	else
	{
		NOTICE_LOG_GAME("Point lights disabled");
	}
}

void Game::PlayGameState::AddSpotLights()
{
	const int spotLightsCount = GET_CONFIG_VALUE_GAME("spotLightsCount", 1);
	if (spotLightsCount > 0)
	{
		DEBUG_LOG_GAME("Creating ", spotLightsCount, " spot lights");
		Engine::SpotLightBuilder spotLightBuilder(m_gameManager->GetShaderFactory());
		Utility::BuilderDirector<Rendering::Lighting::SpotLight> lightBuilderDirector(spotLightBuilder);
		for (int i = 0; i < spotLightsCount; ++i)
		{
			spotLightBuilder.SetLightIndex(i);
			lightBuilderDirector.Construct();
			Rendering::Lighting::SpotLight* spotLight = spotLightBuilder.Get();
			if (spotLight != NULL)
			{
				INFO_LOG_RENDERING("Spot light with intensity = ", spotLight->GetIntensity(), " is being added to directional / spot lights vector");
				m_directionalAndSpotLights.push_back(spotLight);
				m_lights.push_back(spotLight);
			}
		}
		NOTICE_LOG_GAME(spotLightsCount, " spot lights created");
	}
	else
	{
		NOTICE_LOG_GAME("Spot lights disabled");
	}
}

void Game::PlayGameState::Handle(Engine::Actions::Action action)
{
	START_PROFILING_GAME(true, "");
	switch (action)
	{
	case Engine::Actions::SHOW_PLAY_MENU:
		m_gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(m_gameManager->GetPlayMainMenuGameState(), Engine::GameStateTransitioning::PUSH, Engine::GameStateModality::EXCLUSIVE));
		break;
	default:
		INFO_LOG_GAME("Action ", action, " is not supported by the PLAY game state.");
	}
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::Handle(Engine::States::State state)
{
}

void Game::PlayGameState::Handle(Engine::Ranges::Range range, Math::Real value)
{
}

void Game::PlayGameState::MouseButtonEvent(int button, int action, int mods)
{
	START_PROFILING_GAME(true, "");
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
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::MousePosEvent(double xPos, double yPos)
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Cursor position = (", xPos, ", ", yPos, ")");

	//const Rendering::Camera& currentCamera = Engine::CoreEngine::GetCoreEngine()->GetCurrentCamera();
	//m_mousePicker.CalculateCurrentRay(xPos, yPos, currentCamera.GetProjection(), currentCamera.GetViewMatrix());

	//m_gameManager->GetRootGameNode().MousePosEvent(xPos, yPos);


	//if (!m_isMouseLocked)
	//{
	//	STOP_PROFILING_GAME("");
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
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::ScrollEvent(double xOffset, double yOffset)
{
	START_PROFILING_GAME(true, "");
	m_gameManager->GetRootGameNode().ScrollEvent(xOffset, yOffset);
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::Render(Rendering::Renderer* renderer) const
{
	// TODO: Updating the state of the rendering engine (e.g. the values of some of its member variables)
	// in this function is not good. This should be done in the Update function (or maybe not?).
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_EXIT_GAME(renderer != NULL, Utility::Logging::CRITICAL, "Cannot render the game. The rendering engine is NULL.");
	DEBUG_LOG_GAME("PLAY game state rendering");

	renderer->InitRenderScene(m_ambientLightColor, m_dayNightMixFactor);
	renderer->SetCurrentCamera(m_gameManager->GetCurrentCamera());

	RenderWaterTextures(renderer);

	renderer->BindDisplayTexture();
	renderer->ClearScreen();

	RenderSceneWithAmbientLight(renderer);
	//m_gameManager->GetRootGameNode().Render(shader, renderer);
	//RenderSceneWithPointLights(renderer); // Point light rendering
	RenderSceneWithDirectionalAndSpotLights(renderer); // Directional and spot light rendering

	RenderWaterNodes(renderer);

	RenderBillboardNodes(renderer);

	RenderSkybox(renderer);

	RenderParticles(renderer);

#ifdef DEBUG_RENDERING_ENABLED
	renderer->RenderDebugNodes(m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::GUI));
#endif
	
	renderer->FinalizeRenderScene((renderer->GetAntiAliasingMethod() == Rendering::Aliasing::FXAA) ?
		m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::FILTER_FXAA) :
		m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::FILTER_NULL));

#ifdef DRAW_GAME_TIME
	renderer->RenderGuiControl(m_inGameTimeGuiButton, m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::TEXT));
#endif

	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderSceneWithAmbientLight(Rendering::Renderer* renderer) const
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(renderer->IsAmbientLightEnabled(), Utility::Logging::DEBUG, "Ambient light is disabled by the rendering engine.");
	const Rendering::Shader& ambientShader = GetAmbientShader(renderer->GetFogInfo());
	renderer->BindShader(ambientShader);
	//renderer->UpdateRendererUniforms(ambientShader);
	m_gameManager->GetRootGameNode().Render(ambientShader, renderer);
	CHECK_CONDITION_GAME(m_gameManager->GetTerrainNode() != NULL, Utility::Logging::ERR, "Cannot render terrain. There are no terrain nodes registered.");
	const Rendering::Shader& ambientTerrainShader = GetAmbientTerrainShader(renderer->GetFogInfo());
	renderer->BindShader(ambientTerrainShader);
	//renderer->UpdateRendererUniforms(ambientTerrainShader);
	m_gameManager->GetTerrainNode()->Render(ambientTerrainShader, renderer);
}

void Game::PlayGameState::RenderSceneWithPointLights(Rendering::Renderer* renderer) const
{
	if (!Rendering::Lighting::PointLight::ArePointLightsEnabled())
	{
		DEBUG_LOG_GAME("All point lights are disabled");
		return;
	}

	for (size_t i = 0; i < m_pointLights.size(); ++i)
	{
		const Rendering::Lighting::PointLight* currentPointLight = renderer->SetCurrentPointLight(m_pointLights[i]);
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
	START_PROFILING_GAME(true, "");
	for (std::vector<Rendering::Lighting::BaseLight*>::const_iterator lightItr = m_directionalAndSpotLights.begin(); lightItr != m_directionalAndSpotLights.end(); ++lightItr)
	{
		if ((*lightItr)->IsEnabled())
		{
			const Rendering::Lighting::BaseLight* currentLight = renderer->SetCurrentLight(*lightItr);
			if (renderer->InitShadowMap())
			{
				// Render scene using shadow mapping shader
				const Rendering::Shader& shadowMapShader = m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::SHADOW_MAP);
				renderer->BindShader(shadowMapShader);
				//renderer->UpdateRendererUniforms(shadowMapShader);
				m_gameManager->GetRootGameNode().Render(shadowMapShader, renderer);
				m_gameManager->GetTerrainNode()->Render(shadowMapShader, renderer); // TODO: Probably unnecessary
				renderer->FinalizeShadowMapRendering(m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::FILTER_GAUSSIAN_BLUR));
			}

			renderer->InitLightRendering();
			// TODO: Render scene with light is not ready. Check the function Renderer::RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode, bool isCastingShadowsEnabled /* = true */).
			renderer->BindShader(currentLight->GetShader());
			//renderer->UpdateRendererUniforms(currentLight->GetShader());
			m_gameManager->GetRootGameNode().Render(currentLight->GetShader(), renderer);
			renderer->BindShader(currentLight->GetTerrainShader());
			//renderer->UpdateRendererUniforms(currentLight->GetTerrainShader());
			m_gameManager->GetTerrainNode()->Render(currentLight->GetTerrainShader(), renderer);
			renderer->FinalizeLightRendering();
		}
	}
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderSkybox(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Skybox rendering started");

	Engine::GameNode* skyboxNode = m_gameManager->GetSkyboxNode();
	skyboxNode->GetTransform().SetPos(renderer->GetCurrentCamera().GetPos());
	// TODO: Rotating the skybox
	//skyboxNode->GetTransform().SetRot(Math::Quaternion(Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), skyboxNode->GetTransfom));
	//m_skyboxAngle += m_skyboxAngleStep;
	//if (m_fogEnabled)
	//{
	//	STOP_PROFILING_GAME("");
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
	const Rendering::Shader& skyboxShader = m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::SKYBOX);
	renderer->BindShader(skyboxShader);
	//renderer->UpdateRendererUniforms(skyboxShader);
	skyboxNode->Render(skyboxShader, renderer);
	renderer->SetDepthFuncDefault();
	renderer->SetCullFaceDefault();
	//glEnable(GL_DEPTH_TEST);
	//Rendering::CheckErrorCode("Renderer::Render", "Rendering skybox");

	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderWaterTextures(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != NULL, Utility::Logging::DEBUG, "There are no water nodes registered in the rendering engine");
	// TODO: For now we only support one water node (you can see that in the "distance" calculation). In the future there might be more.

	RenderWaterReflectionTexture(renderer);
	RenderWaterRefractionTexture(renderer);
	
	renderer->DisableClippingPlanes();
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderWaterReflectionTexture(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != NULL, Utility::Logging::DEBUG, "There are no water nodes registered in the rendering engine");
	
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

	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderWaterRefractionTexture(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != NULL, Utility::Logging::DEBUG, "There are no water nodes registered in the rendering engine");
	
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
	
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderWaterNodes(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
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
	const Rendering::Shader& waterShader = GetWaterShader(renderer);
	renderer->BindShader(waterShader);
	//renderer->UpdateRendererUniforms(waterShader);
	m_gameManager->GetWaterNode()->Render(waterShader, renderer);
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderBillboardNodes(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Rendering billboards started");
	//renderer->SetDepthTest(false);
	renderer->SetBlendingEnabled(true);
	//GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
	// GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE,
	// GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR, GL_SRC1_ALPHA, and GL_ONE_MINUS_SRC1_ALPHA
	renderer->SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	const Rendering::Shader& billboardShader = m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::BILLBOARD);
	renderer->BindShader(billboardShader);
	//renderer->UpdateRendererUniforms(billboardShader);
	for (std::vector<Engine::GameNode*>::const_iterator billboardsNodeItr = m_gameManager->GetBillboardNodes().begin(); billboardsNodeItr != m_gameManager->GetBillboardNodes().end(); ++billboardsNodeItr)
	{
		(*billboardsNodeItr)->Render(billboardShader, renderer);
	}
	//renderer->SetDepthTest(true);
	renderer->SetBlendingEnabled(false);
	//renderer->SetBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::RenderParticles(Rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("Rendering particles started");
	const Rendering::Shader& particlesShader = m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::PARTICLES);
	renderer->BindShader(particlesShader);
	//renderer->UpdateRendererUniforms(particlesShader);
	for (std::vector<Engine::ParticleGenerator*>::const_iterator particleGeneratorItr = m_gameManager->GetParticleGenerators().begin(); particleGeneratorItr != m_gameManager->GetParticleGenerators().end(); ++particleGeneratorItr)
	{
		//if (!(*particleGeneratorItr)->GetTexture()->IsAdditive())
		//{
			(*particleGeneratorItr)->SortParticles(renderer->GetCurrentCamera().GetPos());
		//}
		renderer->RenderParticles(particlesShader, (*particleGeneratorItr)->GetTexture(), (*particleGeneratorItr)->GetParticles(), (*particleGeneratorItr)->GetAliveParticlesCount());
	}
	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::Update(Math::Real elapsedTime)
{
	START_PROFILING_GAME(true, "");
	DEBUG_LOG_GAME("PLAY game state updating");
	Engine::GameManager::GetGameManager()->GetRootGameNode().Update(elapsedTime);

	//EMERGENCY_LOG_GAME("Elapsed time: ", elapsedTime * 1000.0f, " [ms]");
	m_inGameDateTime += Utility::Timing::TimeSpan(elapsedTime * m_clockSpeed * 1000.0f, Utility::Timing::MILLISECOND);
	//CRITICAL_LOG_GAME("In-game time: ", m_inGameDateTime.ToDateString());
	m_inGameTimeGuiButton.SetText(m_inGameDateTime.ToString());

	//AdjustAmbientLightAccordingToCurrentTime();

	CalculateSunElevationAndAzimuth();

	STOP_PROFILING_GAME("");
}

void Game::PlayGameState::CalculateSunElevationAndAzimuth()
{
	//const int timeGMTdifference = 1;

	//const Math::Angle b(0.9863014f * (m_inGameDateTime->GetDayInYear() - 81)); // 0,98630136986301369863013698630137 = 360 / 365
	//const Math::Real bSin = b.Sin();
	//const Math::Real bCos = b.Cos();

	//const Math::Real equationOfTime = 19.74f * bSin * bCos - 7.53f * bCos - 1.5f * bSin; // EoT
	//const Math::Real declinationSin = TROPIC_OF_CANCER_SINUS * bSin;
	//const Math::Angle declinationAngle(asin(declinationSin), Math::Unit::RADIAN);
	////DEBUG_LOG_ENGINE("Declination in degrees = ", declinationAngle.GetAngleInDegrees());

	//const Math::Real timeCorrectionInSeconds = 60.0f * (4.0f * (m_longitude.GetAngleInDegrees() - 15.0f * timeGMTdifference) + equationOfTime);
	//const Math::Real localSolarTime = m_inGameDateTime->GetDayTime() + timeCorrectionInSeconds;
	////DEBUG_LOG_ENGINE("Time correction in seconds = ", timeCorrectionInSeconds);
	////DEBUG_LOG_ENGINE("Local time = ", m_timeOfDay, "\tLocal solar time = ", localSolarTime);

	//const Math::Angle hourAngle(15.0f * (localSolarTime - 12 * Utility::Timing::DateTime::SECONDS_PER_HOUR) / Utility::Timing::DateTime::SECONDS_PER_HOUR);
	////DEBUG_LOG_ENGINE("Hour angle = ", hourAngle.GetAngleInDegrees());

	//const Math::Real sunElevationSin = declinationSin * m_latitude.Sin() + declinationAngle.Cos() * m_latitude.Cos() * hourAngle.Cos();
	//m_sunElevation.SetAngleInRadians(asin(sunElevationSin));
	////DEBUG_LOG_ENGINE("Sun elevation = ", m_sunElevation.GetAngleInDegrees());

	//const Math::Real sunAzimuthCos = ((declinationSin * m_latitude.Cos()) - (declinationAngle.Cos() * m_latitude.Sin() * hourAngle.Cos())) / m_sunElevation.Cos();
	//m_sunAzimuth.SetAngleInRadians(acos(sunAzimuthCos));
	//bool isAfternoon = (localSolarTime > 12.0f * Utility::Timing::DateTime::SECONDS_PER_HOUR) || (hourAngle.GetAngleInDegrees() > REAL_ZERO);
	//if (isAfternoon)
	//{
	//	m_sunAzimuth.SetAngleInDegrees(360.0f - m_sunAzimuth.GetAngleInDegrees());
	//}
	//DEBUG_LOG_ENGINE("Sun azimuth = ", m_sunAzimuth.GetAngleInDegrees());
}

//void Game::PlayGameState::AdjustAmbientLightAccordingToCurrentTime()
//{
//	START_PROFILING_GAME(true, "");
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
//	STOP_PROFILING_GAME("");
//}

const Rendering::Shader& Game::PlayGameState::GetAmbientShader(const Rendering::FogEffect::FogInfo& fogInfo) const
{
	START_PROFILING_ENGINE(true, "");
	if (fogInfo.IsEnabled()) // if (fogInfo != NULL)
	{
		//DEBUG_LOG_RENDERING("Fog fall-off type: ", m_fogFallOffType, ". Fog distance calculation type: ", m_fogCalculationType);

		// TODO: A very ugly way. If we decide to add more fog fall off or calculation types then we will surely have a big problem in here.
		if (fogInfo.GetFallOffType() == Rendering::FogEffect::LINEAR)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_FOG_LINEAR_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_FOG_LINEAR_RANGE_BASED);
			}
		}
		else if (fogInfo.GetFallOffType() == Rendering::FogEffect::EXPONENTIAL)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED);
			}
		}
	}
	STOP_PROFILING_ENGINE("");
	return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT);
}

const Rendering::Shader& Game::PlayGameState::GetAmbientTerrainShader(const Rendering::FogEffect::FogInfo& fogInfo) const
{
	START_PROFILING_ENGINE(true, "");
	if (fogInfo.IsEnabled())
	{
		//DEBUG_LOG_RENDERING("Fog fall-off type: ", m_fogFallOffType, ". Fog distance calculation type: ", m_fogCalculationType);
		// TODO: A very ugly way. If we decide to add more fog fall off or calculation types then we will surely have a big problem in here.
		if (fogInfo.GetFallOffType() == Rendering::FogEffect::LINEAR)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED);
			}
		}
		else if (fogInfo.GetFallOffType() == Rendering::FogEffect::EXPONENTIAL)
		{
			if (fogInfo.GetCalculationType() == Rendering::FogEffect::PLANE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED);
			}
			else if (fogInfo.GetCalculationType() == Rendering::FogEffect::RANGE_BASED)
			{
				STOP_PROFILING_ENGINE("");
				return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED);
			}
		}
	}
	STOP_PROFILING_ENGINE("");
	return m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::AMBIENT_TERRAIN);
}