#include "MenuGameState.h"
#include "Def.h"
#include "PlayGameState.h"
#include "LoadGameState.h"
#include "StartGameCommand.h"

#include "Engine/CoreEngine.h"
#include "Engine/GameManager.h"
#include "Engine/GameCommand.h"
#include "Engine/ParticlesSystemComponent.h"

#include "Rendering/ParticlesSystemBuilder.h"
#include "Rendering/Shader.h"

#include "Math/Effect_impl.h"

#include "Utility/ILogger.h"
#include "Utility/IConfig.h"
#include "Utility/BuilderDirector.h"

game::MenuGameState::MenuGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName, const rendering::text::Font* mainMenuFont, math::Real mainMenuFontSize) :
	engine::GameState(inputMappingContextName),
	m_gameManager(gameManager),
	//m_particlesSystem(nullptr),
	m_mainMenuRootEntry("Main menu", mainMenuFont, mainMenuFontSize, nullptr, math::ZERO_VECTOR_2D, math::Angle(0.0f), math::Vector2D(1.0f, 1.0f), 1.0f, rendering::Color(rendering::color_ids::BLACK),
		rendering::Color(rendering::color_ids::BLACK), math::Vector2D(REAL_ZERO, REAL_ZERO)),
	m_notSelectedMenuEntryColorEffect(std::make_unique<math::effects::NoEffect<rendering::Color>>(rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryColorRed", 1.0f),
		GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryColorGreen", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryColorBlue", 1.0f),
		GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryColorAlpha", 1.0f)))),
	m_selectedMenuEntryColorEffect(std::make_unique<math::effects::SmoothTransitionEffect<rendering::Color>>(
		std::vector<rendering::Color>{
			rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorRed_1", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorGreen_1", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorBlue_1", 0.0f)),
			rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorRed_2", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorGreen_2", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorBlue_2", 0.0f)),
			rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorRed_3", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorGreen_3", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorBlue_3", 1.0f)) }.data(),
		std::vector<math::Real>{ GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorTime_1", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorTime_2", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryColorTime_3", 2.0f) }.data(), 3, false)),
	m_notSelectedMenuEntryOutlineColorEffect(std::make_unique<math::effects::NoEffect<rendering::Color>>(rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOutlineColorRed", 1.0f),
		GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOutlineColorGreen", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOutlineColorBlue", 1.0f),
		GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOutlineColorAlpha", 1.0f)))),
		m_selectedMenuEntryOutlineColorEffect(std::make_unique<math::effects::BlinkEffect<rendering::Color>>(
			std::vector<rendering::Color>{
				rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorRed_1", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorGreen_1", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorBlue_1", 0.0f)),
				rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorRed_2", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorGreen_2", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorBlue_2", 0.0f)),
				rendering::Color(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorRed_3", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorGreen_3", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorBlue_3", 1.0f)) }.data(),
			std::vector<math::Real>{ GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorDuration_1", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorDuration_2", 1.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOutlineColorDuration_3", 2.0f) }.data(), 3)),
		m_notSelectedMenuEntryOffsetEffect(std::make_unique<math::effects::NoEffect<math::Vector2D>>(math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOffsetX", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryOffsetY", 0.0f)))),
			m_selectedMenuEntryOffsetEffect(std::make_unique<math::effects::SmoothTransitionEffect<math::Vector2D>>(
				std::vector<math::Vector2D>{
					math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetX_1", -0.015f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetY_1", 0.015f)),
					math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetX_2", 0.015f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetY_2", 0.015f)),
					math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetX_3", 0.015f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetY_3", -0.015f)),
					math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetX_4", -0.015f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetY_3", -0.015f)),
					math::Vector2D(GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetX_5", -0.015f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetY_5", 0.015f)) }.data(),
				std::vector<math::Real>{
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetTime_1", 0.0f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetTime_2", 0.75f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetTime_3", 1.5f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetTime_4", 2.25f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryOffsetTime_5", 3.0f)}.data(), 5, true)),
			m_notSelectedMenuEntryCharacterWidthEffect(std::make_unique<math::effects::NoEffect<math::Real>>(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryCharacterWidth", 0.5f))),
			m_selectedMenuEntryCharacterWidthEffect(std::make_unique<math::effects::SmoothTransitionEffect<math::Real>>(
				std::vector<math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidth_1", 0.4f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidth_2", 0.45f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidth_3", 0.5f), GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidth_4", 0.55f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidth_5", 0.6f)}.data(),
				std::vector<math::Real>{
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidthTime_1", 0.0f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidthTime_2", 0.2f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidthTime_3", 0.4f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidthTime_4", 0.6f),
						GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterWidthTime_5", 0.8f) }.data(), 5, false)),
			m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect(std::make_unique<math::effects::NoEffect<math::Real>>(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryCharacterEdgeTransitionWidth", 0.1f))),
			m_selectedMenuEntryCharacterEdgeTransitionWidthEffect(std::make_unique<math::effects::SmoothTransitionEffect<math::Real>>(
				std::vector<math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidth_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidth_2", 0.1f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidth_3", 0.2f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidth_4", 0.3f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidth_5", 0.4f) }.data(),
				std::vector<math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidthTime_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidthTime_2", 1.5f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidthTime_3", 3.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidthTime_4", 4.5f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryCharacterEdgeTransitionWidthTime_5", 6.0f) }.data(), 5, true)),
			m_notSelectedMenuEntryBorderWidthEffect(std::make_unique<math::effects::NoEffect<math::Real>>(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryBorderWidth", 0.4f))),
			m_selectedMenuEntryBorderWidthEffect(std::make_unique<math::effects::SmoothTransitionEffect<math::Real>>(
				std::vector<math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidth_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidth_2", 0.12f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidth_3", 0.24f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidth_4", 0.36f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidth_5", 0.48f) }.data(),
				std::vector<math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidthTime_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidthTime_2", 1.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidthTime_3", 2.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidthTime_4", 3.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderWidthTime_5", 4.0f) }.data(), 5, false)),
			m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect(std::make_unique<math::effects::NoEffect<math::Real>>(GET_CONFIG_VALUE_GAME("mainMenuNotSelectedEntryBorderEdgeTransitionWidth", 0.1f))),
			m_selectedMenuEntryBorderEdgeTransitionWidthEffect(std::make_unique<math::effects::SmoothTransitionEffect<math::Real>>(
				std::vector<math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidth_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidth_2", 0.1f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidth_3", 0.2f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidth_4", 0.3f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidth_5", 0.4f) }.data(),
				std::vector<math::Real>{
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidthTime_1", 0.0f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidthTime_2", 0.1f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidthTime_3", 0.2f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidthTime_4", 0.3f),
					GET_CONFIG_VALUE_GAME("mainMenuSelectedEntryBorderEdgeTransitionWidthTime_5", 0.4f) }.data(), 5, false)),
			m_mousePosX(REAL_ZERO),
			m_mousePosY(REAL_ZERO),
			m_mousePicker(),
			m_currentMenuEntry(nullptr)
#ifdef PROFILING_GAME_MODULE_ENABLED
			, m_classStats(STATS_STORAGE.GetClassStats("MenuGameState"))
#endif
{
	/**
	* TODO: Calculating the proper locations for the menu entries and updating these locations whenever the window is resized.
	*/
	engine::MenuEntry* mainMenuLoadMenuEntry = new engine::CompositeMenuEntry("Load", mainMenuFont, mainMenuFontSize, nullptr, math::Vector2D(0.25f, 0.34f), math::Angle(0.0f), math::Vector2D(1.0f, 1.0f),
		0.5f, rendering::Color(rendering::color_ids::RED), rendering::Color(rendering::color_ids::GREEN), math::Vector2D(0.005f, 0.005f), true);
	// TODO: Populate "Load" menu entry with children.
	engine::MenuEntry* mainMenuOptionsMenuEntry = new engine::CompositeMenuEntry("Options", mainMenuFont, mainMenuFontSize, nullptr, math::Vector2D(0.25f, 0.51f), math::Angle(0.0f), math::Vector2D(1.0f, 1.0f),
		0.5f, rendering::Color(rendering::color_ids::RED), rendering::Color(rendering::color_ids::GREEN), math::Vector2D(0.005f, 0.005f), true);
	mainMenuOptionsMenuEntry->AddChild(new engine::CompositeMenuEntry("Sound", mainMenuFont, mainMenuFontSize, nullptr, math::Vector2D(0.25f, 0.2f), math::Angle(0.0f), math::Vector2D(1.0f, 1.0f),
		0.5f, rendering::Color(rendering::color_ids::RED), rendering::Color(rendering::color_ids::GREEN), math::Vector2D(0.005f, 0.005f), true));
	mainMenuOptionsMenuEntry->AddChild(new engine::CompositeMenuEntry("Graphics", mainMenuFont, mainMenuFontSize, nullptr, math::Vector2D(0.25f, 0.4f), math::Angle(0.0f), math::Vector2D(1.0f, 1.0f),
		0.5f, rendering::Color(rendering::color_ids::RED), rendering::Color(rendering::color_ids::GREEN), math::Vector2D(0.005f, 0.005f), true));
	mainMenuOptionsMenuEntry->AddChild(new engine::CompositeMenuEntry("Controls", mainMenuFont, mainMenuFontSize, nullptr, math::Vector2D(0.25f, 0.6f), math::Angle(0.0f), math::Vector2D(1.0f, 1.0f),
		0.5f, rendering::Color(rendering::color_ids::RED), rendering::Color(rendering::color_ids::GREEN), math::Vector2D(0.005f, 0.005f), true));
	mainMenuOptionsMenuEntry->AddChild(new engine::ActionMenuEntry(engine::actions::RETURN_TO_PARENT_MENU_ENTRY, "Back", mainMenuFont,
		mainMenuFontSize, nullptr, math::Vector2D(0.25f, 0.8f), math::Angle(0.0f), math::Vector2D(1.0f, 1.0f), 0.5f, rendering::Color(rendering::color_ids::RED), rendering::Color(rendering::color_ids::GREEN), math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChild(new engine::ActionMenuEntry(engine::actions::START_GAME, "New game", mainMenuFont,
		mainMenuFontSize, nullptr /*m_gameManager->GetTextureFactory().GetTexture("check-297273_960_720.png")*/, math::Vector2D(0.25f, 0.17f), math::Angle(0.0f), math::Vector2D(1.0f, 1.0f),
		0.5f, rendering::Color(rendering::color_ids::RED), rendering::Color(rendering::color_ids::GREEN), math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChild(mainMenuLoadMenuEntry);
	m_mainMenuRootEntry.AddChild(mainMenuOptionsMenuEntry);
	m_mainMenuRootEntry.AddChild(new engine::ActionMenuEntry(engine::actions::SHOW_INTRO, "Intro", mainMenuFont,
		mainMenuFontSize, nullptr, math::Vector2D(0.25f, 0.68f), math::Angle(0.0f), math::Vector2D(1.0f, 1.0f), 0.5f, rendering::Color(rendering::color_ids::RED), rendering::Color(rendering::color_ids::GREEN), math::Vector2D(0.005f, 0.005f), true));
	m_mainMenuRootEntry.AddChild(new engine::ActionMenuEntry(engine::actions::QUIT_GAME, "Quit", mainMenuFont,
		mainMenuFontSize, nullptr, math::Vector2D(0.25f, 0.85f), math::Angle(0.0f), math::Vector2D(1.0f, 1.0f), 0.5f, rendering::Color(rendering::color_ids::RED), rendering::Color(rendering::color_ids::GREEN), math::Vector2D(0.005f, 0.005f), true));

	m_gameManager->LoadSoundEffect("bounce.wav");

	DeselectAll();
	m_currentMenuEntry = m_mainMenuRootEntry.SelectChild(0); // TODO: Two assignments to the same variable. Fix that!
	//m_currentMenuEntry = &m_mainMenuRootEntry;

	//m_inputMapping.PushContext("MenuGameStateContext");
	//m_inputMapping.RegisterCallback(MenuGameStateInputCallback, 0);
	//Engine::CoreEngine::GetCoreEngine()->RegisterHandlerForAction(Engine::Input::Actions::ACTION_QUIT_GAME, m_quitGameCommand);
}

game::MenuGameState::~MenuGameState(void)
{
	//SAFE_DELETE(m_particlesSystem);
}

void game::MenuGameState::Entered()
{
	//ParticlesSystemBuilder particlesSystemBuilder(m_gameManager, Particleeffects::FOUNTAIN);
	//Utility::BuilderDirector<Rendering::Particles::ParticlesSystem> particlesSystemBuilderDirector(particlesSystemBuilder);
	//particlesSystemBuilderDirector.Construct();
	//m_particlesSystem = particlesSystemBuilder.Get();
	
	engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);

	INFO_LOG_GAME("Menu game state has been placed in the game state manager");
}

void game::MenuGameState::Leaving()
{
	engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Menu game state is about to be removed from the game state manager");
}

void game::MenuGameState::Obscuring()
{
	engine::CoreEngine::GetCoreEngine()->PopInputContext();
	INFO_LOG_GAME("Another game state is about to stack on top of menu game state");
}

void game::MenuGameState::Revealed()
{
	engine::CoreEngine::GetCoreEngine()->PushInputContext(m_inputMappingContextName);
	INFO_LOG_GAME("Menu game state has become the topmost game state in the game state manager's stack");
}

void game::MenuGameState::Handle(engine::actions::Action action)
{
	switch (action)
	{
	case engine::actions::SELECT_PREVIOUS_MENU_ENTRY:
		m_currentMenuEntry = m_currentMenuEntry->GetParent()->SelectPrevChild(); // TODO: Is it possible that GetParent() == nullptr?
		break;
	case engine::actions::SELECT_NEXT_MENU_ENTRY:
		m_currentMenuEntry = m_currentMenuEntry->GetParent()->SelectNextChild(); // TODO: Is it possible that GetParent() == nullptr?
		break;
	case engine::actions::CHOOSE_CURRENT_MENU_ENTRY:
		m_currentMenuEntry->Dispatch();
		break;
	case engine::actions::GO_TO_CHILD_MENU_ENTRY:
		m_currentMenuEntry = m_currentMenuEntry->GoTo();
		break;
	case engine::actions::RETURN_TO_PARENT_MENU_ENTRY:
		if (m_currentMenuEntry->GetParent()->HasParent())
		{
			m_currentMenuEntry = m_currentMenuEntry->GetParent();
		}
		else
		{
			// TODO: Select the "Quit" menu entry?
		}
		break;
	default:
		DEBUG_LOG_GAME("The action ", action, " is not supported by the MenuGameState");
		break;
	}
}

void game::MenuGameState::Handle(engine::states::State state)
{
	//DELOCUST_LOG_GAME("Handling the state ", state);
	switch (state)
	{
	case engine::states::MOUSE_KEY_LEFT_PRESSED:
		if (m_currentMenuEntry->DoesMouseHoverOver(m_mousePosX, m_mousePosY))
		{
			m_currentMenuEntry->Dispatch();
		}
		break;
	default:
		DEBUG_LOG_GAME("The state ", state, " is not supported by the MenuGameState");
		break;
	}
}

void game::MenuGameState::Handle(engine::ranges::Range range, math::Real value)
{
	switch (range)
	{
	case engine::ranges::AXIS_X:
		m_mousePosX = value;
		break;
	case engine::ranges::AXIS_Y:
		m_mousePosY = value;
		break;
	default:
		DEBUG_LOG_GAME("The range ", range, " is not supported by the MenuGameState");
		break;
	}
	SelectChild(m_currentMenuEntry->GetParent()->GetSelectedChildIndex(m_mousePosX, m_mousePosY));
}

void game::MenuGameState::DeselectAll()
{
	START_PROFILING_GAME(true, "");
	// This function is supposed to apply the "non-selected" effects to all non-selected menu entries. However Effect class can only handle one attribute at a time.
	// It is not possible to modify many attributes using one Effect class instance. As a result only the last non-selected menu entry will have "non-selected" effects applied to it.
	//for (int i = 0; i < m_currentMenuEntry->GetChildrenCount(); ++i)
	//{
	//	m_currentMenuEntry->SelectChildMenuEntry(i, false);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyBorderEdgeTransitionWidthEffect(m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyBorderWidthEffect(m_notSelectedMenuEntryBorderWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyCharacterEdgeTransitionWidthEffect(m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyCharacterWidthEffect(m_notSelectedMenuEntryCharacterWidthEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyColorEffect(m_notSelectedMenuEntryColorEffect);
	//	m_currentMenuEntry->GetSelectedChild()->ApplyOffsetEffect(m_notSelectedMenuEntryOffsetEffect);
	//	//m_currentMenuEntry->GetSelectedChild()->ApplyOutlineColorEffect(m_notSelectedMenuEntryOutlineColorEffect);
	//}
	STOP_PROFILING_GAME("");
}

void game::MenuGameState::SelectChild(size_t childIndex)
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_GAME(m_currentMenuEntry->GetParent()->GetSelectedChildIndex() != childIndex, utility::logging::DELOCUST,
		"Trying to select the child which is already selected (", childIndex, ").");
	m_currentMenuEntry = m_currentMenuEntry->GetParent()->SelectChild(childIndex);
	m_gameManager->PlaySoundEffect("bounce.wav", 1.0f, 1.0f);
	ERROR_LOG_GAME("Selected menu entry changed to ", childIndex);
}

void game::MenuGameState::Render(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	DELOCUST_LOG_GAME("MAIN MENU game state rendering");

	renderer->BindAsRenderTarget();
	renderer->ClearScreen(/* TODO: specify menu game state clear screen color */);
	m_currentMenuEntry->GetParent()->RenderAll(rendering::shader_ids::GUI, renderer);
	RenderParticles(renderer);
	//renderer->FinalizeRenderScene();

	STOP_PROFILING_GAME("");
}

void game::MenuGameState::RenderParticles(rendering::Renderer* renderer) const
{
	START_PROFILING_GAME(true, "");
	//DEBUG_LOG_GAME("Rendering particles started");
	//const Rendering::Shader& particlesShader = m_gameManager->GetShaderFactory().GetShader(Rendering::shader_ids::PARTICLES);
	//renderer->BindShader(particlesShader);
	//renderer->UpdateRendererUniforms(particlesShader);
	//renderer->RenderParticles(particlesShader, *m_particlesSystem);
	STOP_PROFILING_GAME("");
}

//void Game::MenuGameState::MouseButtonEvent(int button, int action, int mods)
//{
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//	{
//		if (m_currentMenuEntry->GetSelectedChild()->DoesMouseHoverOver(m_mousePosX, m_mousePosY))
//		{
//			m_currentMenuEntry = m_currentMenuEntry->Execute();
//		}
//		else
//		{
//			//CRITICAL_LOG_GAME("Does not hover (", m_mousePosX, ", ", m_mousePosY, ")");
//		}
//	}
//}

//void Game::MenuGameState::MousePosEvent(double xPos, double yPos)
//{
//	//const Rendering::Camera& currentCamera = Engine::CoreEngine::GetCoreEngine()->GetRenderer()->GetCurrentCamera();
//	//m_mousePicker.CalculateCurrentRay(xPos, yPos, currentCamera.GetProjection(), currentCamera.GetViewMatrix());
//
//	m_mousePosX = static_cast<math::Real>(xPos);
//	m_mousePosY = static_cast<math::Real>(yPos);
//
//	const int menuEntryChildrenCount = m_currentMenuEntry->GetChildrenCount();
//	//EMERGENCY_LOG_GAME("Menu mouse position event (", m_mousePosX, ", ", m_mousePosY, ")");
//	for (int i = 0; i < menuEntryChildrenCount; ++i)
//	{
//		if (m_currentMenuEntry->DoesMouseHoverOverChild(i, m_mousePosX, m_mousePosY))
//		{
//			if (m_currentMenuEntry->GetSelectedMenuEntryIndex() != i)
//			{
//				INFO_LOG_GAME("Menu entry \"", m_currentMenuEntry->GetChildGuiText(i).GetText(), "\" selected");
//			}
//			SelectChild(i);
//			break;
//		}
//	}
//}

//void MenuGameState::ScrollEvent(double xOffset, double yOffset)
//{
//}

void game::MenuGameState::Update(math::Real deltaTime)
{
	//m_selectedMenuEntryColorEffect->Update(m_currentMenuEntry->GetSelectedChild()->GetGuiText(), deltaTime);
	if (m_notSelectedMenuEntryColorEffect != nullptr)
	{
		m_notSelectedMenuEntryColorEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyColorEffectToAll(*m_notSelectedMenuEntryColorEffect);
	}
	if (m_selectedMenuEntryColorEffect != nullptr)
	{
		m_selectedMenuEntryColorEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyColorEffect(*m_selectedMenuEntryColorEffect);
	}
	if (m_notSelectedMenuEntryOutlineColorEffect != nullptr)
	{
		m_notSelectedMenuEntryOutlineColorEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyOutlineColorEffectToAll(*m_notSelectedMenuEntryOutlineColorEffect);
	}
	if (m_selectedMenuEntryOutlineColorEffect != nullptr)
	{
		m_selectedMenuEntryOutlineColorEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyOutlineColorEffect(*m_selectedMenuEntryOutlineColorEffect);
	}
	if (m_notSelectedMenuEntryOffsetEffect != nullptr)
	{
		m_notSelectedMenuEntryOffsetEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyOffsetEffectToAll(*m_notSelectedMenuEntryOffsetEffect);
	}
	if (m_selectedMenuEntryOffsetEffect != nullptr)
	{
		m_selectedMenuEntryOffsetEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyOffsetEffect(*m_selectedMenuEntryOffsetEffect);
	}
	if (m_notSelectedMenuEntryCharacterWidthEffect != nullptr)
	{
		m_notSelectedMenuEntryCharacterWidthEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyCharacterWidthEffectToAll(*m_notSelectedMenuEntryCharacterWidthEffect);
	}
	if (m_selectedMenuEntryCharacterWidthEffect != nullptr)
	{
		m_selectedMenuEntryCharacterWidthEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyCharacterWidthEffect(*m_selectedMenuEntryCharacterWidthEffect);
	}
	if (m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect != nullptr)
	{
		m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyCharacterEdgeTransitionWidthEffectToAll(*m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect);
	}
	if (m_selectedMenuEntryCharacterEdgeTransitionWidthEffect != nullptr)
	{
		m_selectedMenuEntryCharacterEdgeTransitionWidthEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyCharacterEdgeTransitionWidthEffect(*m_selectedMenuEntryCharacterEdgeTransitionWidthEffect);
	}
	if (m_notSelectedMenuEntryBorderWidthEffect != nullptr)
	{
		m_notSelectedMenuEntryBorderWidthEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyBorderWidthEffectToAll(*m_notSelectedMenuEntryBorderWidthEffect);
	}
	if (m_selectedMenuEntryBorderWidthEffect != nullptr)
	{
		m_selectedMenuEntryBorderWidthEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyBorderWidthEffect(*m_selectedMenuEntryBorderWidthEffect);
	}
	if (m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect != nullptr)
	{
		m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect->Update(deltaTime);
		m_currentMenuEntry->GetParent()->ApplyBorderEdgeTransitionWidthEffectToAll(*m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect);
	}
	if (m_selectedMenuEntryBorderEdgeTransitionWidthEffect != nullptr)
	{
		m_selectedMenuEntryBorderEdgeTransitionWidthEffect->Update(deltaTime);
		m_currentMenuEntry->ApplyBorderEdgeTransitionWidthEffect(*m_selectedMenuEntryBorderEdgeTransitionWidthEffect);
	}
}