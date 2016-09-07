#ifndef __MENU_GAME_STATE_H__
#define __MENU_GAME_STATE_H__

#include "Def.h"
#include "Engine\EffectFactory.h"
#include "Engine\GameState.h"
#include "Engine\IRenderable.h"
#include "Engine\IUpdateable.h"
#include "Engine\GameNode.h"
#include "Engine\MenuEntry.h"
#include "Engine\GameCommand.h"
#include "Engine\MousePicker.h"
#include "Engine\QuitGameCommand.h"

#include "Rendering\Renderer.h"

#include "Math\Effect.h"
//#include "Math\Effect_impl.h"

#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math\IStatisticsStorage.h"
#include "Math\Statistics.h"
#endif

#include <vector>
#include <string>

namespace Game
{
	class MenuGameState : public Engine::GameState, public Engine::IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		MenuGameState(Engine::GameManager* gameManager, const std::string& inputMappingContextName, const Rendering::Text::Font* mainMenuFont, Math::Real mainMenuFontSize);
		virtual ~MenuGameState(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Entered();
		virtual void Leaving();
		virtual void Obscuring();
		virtual void Revealed();
		virtual void Handle(Engine::Actions::Action action);
		virtual void Handle(Engine::States::State state);
		virtual void Handle(Engine::Ranges::Range range, Math::Real value);

		virtual void Render(Rendering::Renderer* renderer) const;

		virtual void Update(Math::Real deltaTime);
	private:
		void DeselectAll();
		void SelectChild(size_t childIndex);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Engine::GameManager* m_gameManager;
		Engine::CompositeMenuEntry m_mainMenuRootEntry;

		// TODO: Create a Factory of TextEffects which creates text effects (but not dynamically, on the heap) and stores them under some special enum key.
		// Then pass these variables' address to initialize these following pointers. E.g. textEffectFactory.GetTextEffect(MAIN_MENU_NOT_SELECTED_ENTRY_COLOR_EFFECT).
		// Then remove the SAFE_DELETE(textEffect) from the MenuGameState destructor.
		std::unique_ptr<Math::Effects::Effect<Rendering::Color>> m_notSelectedMenuEntryColorEffect;
		std::unique_ptr<Math::Effects::Effect<Rendering::Color>> m_selectedMenuEntryColorEffect;
		Math::Effects::Effect<Rendering::Color>* m_notSelectedMenuEntryOutlineColorEffect;
		Math::Effects::Effect<Rendering::Color>* m_selectedMenuEntryOutlineColorEffect;
		Math::Effects::Effect<Math::Vector2D>* m_notSelectedMenuEntryOffsetEffect;
		Math::Effects::Effect<Math::Vector2D>* m_selectedMenuEntryOffsetEffect;
		Math::Effects::Effect<Math::Real>* m_notSelectedMenuEntryCharacterWidthEffect;
		Math::Effects::Effect<Math::Real>* m_selectedMenuEntryCharacterWidthEffect;
		Math::Effects::Effect<Math::Real>* m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect;
		Math::Effects::Effect<Math::Real>* m_selectedMenuEntryCharacterEdgeTransitionWidthEffect;
		Math::Effects::Effect<Math::Real>* m_notSelectedMenuEntryBorderWidthEffect;
		Math::Effects::Effect<Math::Real>* m_selectedMenuEntryBorderWidthEffect;
		Math::Effects::Effect<Math::Real>* m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect;
		Math::Effects::Effect<Math::Real>* m_selectedMenuEntryBorderEdgeTransitionWidthEffect;

		Math::Real m_mousePosX, m_mousePosY;
		bool m_mousePosChanged;

		Engine::MousePicker m_mousePicker;
		Engine::MenuEntry* m_currentMenuEntry;
		//Engine::QuitGameCommand m_quitGameCommand;
#ifdef PROFILING_GAME_MODULE_ENABLED
		Math::Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MenuGameState */

} /* end namespace Game */

#endif /* __MENU_GAME_STATE_H__ */