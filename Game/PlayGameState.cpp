#include "PlayGameState.h"
#include "Def.h"
#include "Engine\GameManager.h"
#include "Utility\ILogger.h"
#include "PlayMenuGameState.h"
#include "Engine\CoreEngine.h"
#include "Rendering\Shader.h"
#include "Engine\GameNode.h"
#include "Engine\ParticleGenerator.h"

#include "Math\FloatingPoint.h"

#include "tinythread.h"

using namespace Game;

PlayGameState::PlayGameState(Engine::GameManager* gameManager, const std::string& inputMappingContextName) :
	GameState(inputMappingContextName),
	m_isMouseLocked(false),
	m_gameManager(gameManager),
	m_mousePicker()
#ifdef CALCULATE_GAME_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("PlayGameState"))
#endif
{
}

PlayGameState::~PlayGameState(void)
{
}

void PlayGameState::Entered()
{
	START_PROFILING;
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
	STOP_PROFILING;
}

void PlayGameState::Leaving()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("PLAY game state is about to be removed from the game state manager");
#ifdef CALCULATE_STATS
	Rendering::CoreEngine::GetCoreEngine()->StopSamplingSpf();
#endif
}

void PlayGameState::Obscuring()
{
	Engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of PLAY game state");
}

void PlayGameState::Revealed()
{
	Engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("PLAY game state has become the topmost game state in the game state manager's stack");
}

void PlayGameState::Handle(Engine::Actions::Action action)
{
	START_PROFILING;
	switch (action)
	{
	case Engine::Actions::SHOW_PLAY_MENU:
		m_gameManager->SetTransition(new Engine::GameStateTransitioning::GameStateTransition(m_gameManager->GetPlayMainMenuGameState(), Engine::GameStateTransitioning::PUSH, Engine::GameStateModality::EXCLUSIVE));
		break;
	default:
		INFO_LOG_GAME("Not supported action %d", action);
	}
	STOP_PROFILING;
}

void PlayGameState::Handle(Engine::States::State state)
{
}

void PlayGameState::Handle(Engine::Ranges::Range range, Math::Real value)
{
}

void PlayGameState::MouseButtonEvent(int button, int action, int mods)
{
	START_PROFILING;
	m_gameManager->GetRootGameNode().MouseButtonEvent(button, action, mods);
	//switch (action)
	//{
	//case GLFW_PRESS:
	//	m_isMouseLocked = ! m_isMouseLocked;
	//	if (m_isMouseLocked)
	//	{
	//		Rendering::CoreEngine::GetCoreEngine()->CentralizeCursor();
	//	}
	//	INFO_LOG_GAME("Mouse button pressed: button=%d\t mods=%d", button, mods);
	//	break;
	//case GLFW_RELEASE:
	//	INFO_LOG_GAME("Mouse button released: button=%d\t mods=%d", button, mods);
	//	break;
	//default:
	//	WARNING_LOG_GAME("Unknown action performed with the mouse. Button=%d\t action=%d\t mods=%d", button, action, mods);
	//}
	STOP_PROFILING;
}

void PlayGameState::MousePosEvent(double xPos, double yPos)
{
	START_PROFILING;
	DEBUG_LOG_GAME("Cursor position = (%.2f, %.2f)", xPos, yPos);

	//const Rendering::CameraBase& currentCamera = Engine::CoreEngine::GetCoreEngine()->GetCurrentCamera();
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
	//	const Math::Real sensitivity = static_cast<Math::Real>(Rendering::CameraBase::GetSensitivity());
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
	STOP_PROFILING;
}

void PlayGameState::ScrollEvent(double xOffset, double yOffset)
{
	START_PROFILING;
	m_gameManager->GetRootGameNode().ScrollEvent(xOffset, yOffset);
	STOP_PROFILING;
}

void PlayGameState::Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const
{
	// TODO: Updating the state of the rendering engine (e.g. the values of some of its member variables)
	// in this function is not good. This should be done in the Update function (or maybe not?).
	START_PROFILING;
	CHECK_CONDITION_EXIT_GAME(renderer != NULL, Utility::CRITICAL, "Cannot render the game. The rendering engine is NULL.");
	DEBUG_LOG_GAME("PLAY game state rendering");

	Math::Real daytimeTransitionFactor;
	Utility::Timing::Daytime daytime = Engine::CoreEngine::GetCoreEngine()->GetCurrentDaytime(daytimeTransitionFactor);
	renderer->AdjustAmbientLightAccordingToCurrentTime(daytime, daytimeTransitionFactor);

	renderer->InitRenderScene();
	
	RenderWaterTextures(renderer);

	renderer->BindDisplayTexture();
	renderer->ClearScreen();
	renderer->SetCurrentCamera();

	RenderSceneWithAmbientLight(renderer);
	//m_gameManager->GetRootGameNode().Render(shader, renderer);
	//RenderSceneWithPointLights(renderer); // Point light rendering
	RenderSceneWithDirectionalAndSpotLights(renderer); // Directional and spot light rendering

	RenderWaterNodes(renderer);

	RenderBillboardNodes(renderer);

	RenderSkybox(renderer);

	RenderParticles(renderer);

	renderer->FinalizeRenderScene();

	STOP_PROFILING;
}

void PlayGameState::RenderSceneWithAmbientLight(Rendering::Renderer* renderer) const
{
	const Rendering::Shader* ambientShader = renderer->GetAmbientShader();
	const Rendering::Shader* ambientTerrainShader = renderer->GetAmbientTerrainShader();
	if (ambientShader != NULL)
	{
		m_gameManager->GetRootGameNode().Render(ambientShader, renderer);
	}
	if (ambientTerrainShader != NULL)
	{
		CHECK_CONDITION_GAME(m_gameManager->GetTerrainNode() != NULL, Utility::ERR, "Cannot render terrain. There are no terrain nodes registered.");
		m_gameManager->GetTerrainNode()->Render(ambientTerrainShader, renderer);
	}
}

void PlayGameState::RenderSceneWithPointLights(Rendering::Renderer* renderer) const
{
	if (!Rendering::Lighting::PointLight::ArePointLightsEnabled())
	{
		DEBUG_LOG_GAME("All point lights are disabled");
		return;
	}

	for (size_t i = 0; i < renderer->GetPointLightsCount(); ++i)
	{
		const Rendering::Lighting::PointLight* currentPointLight = renderer->SetCurrentPointLight(i);
		if (currentPointLight->IsEnabled())
		{
			DEBUG_LOG_GAME("Point light at index %d is disabled", i);
			continue;
		}
		m_gameManager->GetRootGameNode().Render(currentPointLight->GetShader(), renderer);
		m_gameManager->GetTerrainNode()->Render(currentPointLight->GetTerrainShader(), renderer);
	}
}

void PlayGameState::RenderSceneWithDirectionalAndSpotLights(Rendering::Renderer* renderer) const
{
	START_PROFILING;
	size_t lightsCount = renderer->GetDirectionalLightsCount() + renderer->GetSpotLightsCount();
	for (size_t i = 0; i < lightsCount; ++i)
	{
		const Rendering::Lighting::BaseLight* currentLight = renderer->SetCurrentLight(i);
		if (!currentLight->IsEnabled())
		{
			DEBUG_LOG_GAME("Light at index %d is disabled", i);
			continue;
		}
		if (renderer->InitShadowMap())
		{
			DEBUG_LOG_GAME("Shadow mapping enabled for light %d", i);
			// Render scene using shadow mapping shader
			m_gameManager->GetRootGameNode().Render(renderer->GetShadowMapShader(), renderer);
			m_gameManager->GetTerrainNode()->Render(renderer->GetShadowMapShader(), renderer); // TODO: Probably unnecessary
			renderer->FinalizeShadowMapRendering();
		}

		renderer->InitLightRendering();
		// TODO: Render scene with light is not ready. Check the function Renderer::RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode, bool isCastingShadowsEnabled /* = true */).
		m_gameManager->GetRootGameNode().Render(currentLight->GetShader(), renderer);
		m_gameManager->GetTerrainNode()->Render(currentLight->GetTerrainShader(), renderer);
		renderer->FinalizeLightRendering();
	}
	STOP_PROFILING;
}

void PlayGameState::RenderSkybox(Rendering::Renderer* renderer) const
{
	START_PROFILING;
	DEBUG_LOG_GAME("Skybox rendering started");

	Engine::GameNode* skyboxNode = m_gameManager->GetSkyboxNode();
	skyboxNode->GetTransform().SetPos(renderer->GetCurrentCameraTransform().GetTransformedPos());
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
	skyboxNode->Render(renderer->GetSkyboxShader(), renderer);
	renderer->SetDepthFuncDefault();
	renderer->SetCullFaceDefault();
	//glEnable(GL_DEPTH_TEST);
	//Rendering::CheckErrorCode("Renderer::Render", "Rendering skybox");

	STOP_PROFILING;
}

void PlayGameState::RenderWaterTextures(Rendering::Renderer* renderer) const
{
	START_PROFILING;
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != NULL, Utility::DEBUG, "There are no water nodes registered in the rendering engine");
	// TODO: For now we only support one water node (you can see that in the "distance" calculation). In the future there might be more.

	RenderWaterReflectionTexture(renderer);
	RenderWaterRefractionTexture(renderer);
	
	renderer->DisableClippingPlanes();
	STOP_PROFILING;
}

void PlayGameState::RenderWaterReflectionTexture(Rendering::Renderer* renderer) const
{
	START_PROFILING;
	CHECK_CONDITION_RETURN_VOID_GAME(m_gameManager->GetWaterNode() != NULL, Utility::DEBUG, "There are no water nodes registered in the rendering engine");
	
	// TODO: The camera should be accessible from the game manager. It shouldn't be necessary to access them via rendering engine.
	Math::Transform& cameraTransform = renderer->GetCurrentCameraTransform();
	const Math::Real cameraHeight = cameraTransform.GetTransformedPos().GetY();
	Math::Real distance = 2.0f * (cameraHeight - m_gameManager->GetWaterNode()->GetTransform().GetTransformedPos().GetY());
	cameraTransform.GetPos().SetY(cameraHeight - distance); // TODO: use m_altCamera instead of the main camera.
	cameraTransform.GetRot().InvertPitch();

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
	//	ApplyFilter(m_fxaaFilterShader, m_waterReflectionTexture, NULL);
	//}
	//else
	//{
	//	ApplyFilter(m_nullFilterShader, m_waterReflectionTexture, NULL);
	//}

	//BindAsRenderTarget();
	
	cameraTransform.GetPos().SetY(cameraHeight); // TODO: use m_altCamera instead of the main camera.
	cameraTransform.GetRot().InvertPitch();

	STOP_PROFILING;
}

void PlayGameState::RenderWaterRefractionTexture(Rendering::Renderer* renderer) const
{
	START_PROFILING;
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
	//	ApplyFilter(m_fxaaFilterShader, m_waterReflectionTexture, NULL);
	//}
	//else
	//{
	//	ApplyFilter(m_nullFilterShader, m_waterReflectionTexture, NULL);
	//}
	
	//BindAsRenderTarget();
	
	STOP_PROFILING;
}

void PlayGameState::RenderWaterNodes(Rendering::Renderer* renderer) const
{
	START_PROFILING;
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_gameManager->GetWaterNode() != NULL, Utility::Logging::DEBUG, "There are no water nodes registered in the rendering engine");
	renderer->InitWaterNodesRendering();

	// TODO: In the future there might be more than one water node.
	//for (std::vector<GameNode*>::const_iterator waterNodeItr = m_waterNodes.begin(); waterNodeItr != m_waterNodes.end(); ++waterNodeItr)
	//{
	//	if (m_waterLightReflectionEnabled)
	//	{
	//		(*waterNodeItr)->Render(m_waterShader, this);
	//	}
	//	else
	//	{
	//		(*waterNodeItr)->Render(m_waterNoDirectionalLightShader, this);
	//	}
	//}
	m_gameManager->GetWaterNode()->Render(renderer->GetWaterShader(), renderer);
	STOP_PROFILING;
}

void PlayGameState::RenderBillboardNodes(Rendering::Renderer* renderer) const
{
	START_PROFILING;
	DEBUG_LOG_GAME("Rendering billboards started");
	//renderer->SetDepthTest(false);
	renderer->SetBlendingEnabled(true);
	//GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
	// GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE,
	// GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR, GL_SRC1_ALPHA, and GL_ONE_MINUS_SRC1_ALPHA
	renderer->SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (std::vector<Engine::GameNode*>::const_iterator billboardsRendererItr = m_gameManager->GetBillboardRenderers().begin(); billboardsRendererItr != m_gameManager->GetBillboardRenderers().end(); ++billboardsRendererItr)
	{
		(*billboardsRendererItr)->Render(renderer->GetBillboardShader(), renderer);
	}
	//renderer->SetDepthTest(true);
	renderer->SetBlendingEnabled(false);
	//renderer->SetBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	STOP_PROFILING;
}

void PlayGameState::RenderParticles(Rendering::Renderer* renderer) const
{
	START_PROFILING;
	DEBUG_LOG_GAME("Rendering particles started");
	for (std::vector<Engine::ParticleGenerator*>::const_iterator particleGeneratorItr = m_gameManager->GetParticleGenerators().begin(); particleGeneratorItr != m_gameManager->GetParticleGenerators().end(); ++particleGeneratorItr)
	{
		//if (!(*particleGeneratorItr)->GetTexture()->IsAdditive())
		//{
			(*particleGeneratorItr)->SortParticles(renderer->GetCurrentCameraTransform().GetTransformedPos());
		//}
		renderer->RenderParticles((*particleGeneratorItr)->GetTexture(), (*particleGeneratorItr)->GetParticles(), (*particleGeneratorItr)->GetAliveParticlesCount());
	}
	STOP_PROFILING;
}

void PlayGameState::Update(Math::Real elapsedTime)
{
	START_PROFILING;
	DEBUG_LOG_GAME("PLAY game state updating");
	Engine::GameManager::GetGameManager()->GetRootGameNode().Update(elapsedTime);
	STOP_PROFILING;
}
