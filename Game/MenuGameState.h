#ifndef __MENU_GAME_STATE_H__
#define __MENU_GAME_STATE_H__

#include "Engine\EffectFactory.h"
#include "Engine\GameState.h"
#include "Engine\IRenderable.h"
#include "Engine\IUpdateable.h"
#include "Engine\GameNode.h"
#include "Engine\MenuEntry.h"
#include "Engine\GameCommand.h"
#include "Engine\MousePicker.h"
#include "Engine\QuitGameCommand.h"

#include "Math\Math.h"

#include "Rendering\Renderer.h"

#include "GameStats.h"
#ifdef CALCULATE_GAME_STATS
#include "Math\IStatisticsStorage.h"
#endif

#include <vector>
#include <string>

namespace Game
{

	class Rendering::Shader;

	class MenuGameState : public Engine::GameState, public Engine::IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		MenuGameState(const std::string& inputMappingContextName, const Rendering::Text::Font* mainMenuFont, Math::Real mainMenuFontSize);
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
		Engine::CompositeMenuEntry m_mainMenuRootEntry;

		// TODO: Create a Factory of TextEffects which creates text effects (but not dynamically, on the heap) and stores them under some special enum key.
		// Then pass these variables' address to initialize these following pointers. E.g. textEffectFactory.GetTextEffect(MAIN_MENU_NOT_SELECTED_ENTRY_COLOR_EFFECT).
		// Then remove the SAFE_DELETE(textEffect) from the MenuGameState destructor.
		Engine::Effects::Effect<Math::Vector3D>* m_notSelectedMenuEntryColorEffect;
		Engine::Effects::Effect<Math::Vector3D>* m_selectedMenuEntryColorEffect;
		Engine::Effects::Effect<Math::Vector3D>* m_notSelectedMenuEntryOutlineColorEffect;
		Engine::Effects::Effect<Math::Vector3D>* m_selectedMenuEntryOutlineColorEffect;
		Engine::Effects::Effect<Math::Vector2D>* m_notSelectedMenuEntryOffsetEffect;
		Engine::Effects::Effect<Math::Vector2D>* m_selectedMenuEntryOffsetEffect;
		Engine::Effects::Effect<Math::Real>* m_notSelectedMenuEntryCharacterWidthEffect;
		Engine::Effects::Effect<Math::Real>* m_selectedMenuEntryCharacterWidthEffect;
		Engine::Effects::Effect<Math::Real>* m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect;
		Engine::Effects::Effect<Math::Real>* m_selectedMenuEntryCharacterEdgeTransitionWidthEffect;
		Engine::Effects::Effect<Math::Real>* m_notSelectedMenuEntryBorderWidthEffect;
		Engine::Effects::Effect<Math::Real>* m_selectedMenuEntryBorderWidthEffect;
		Engine::Effects::Effect<Math::Real>* m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect;
		Engine::Effects::Effect<Math::Real>* m_selectedMenuEntryBorderEdgeTransitionWidthEffect;

		Math::Real m_mousePosX, m_mousePosY;
		bool m_mousePosChanged;

		Engine::MousePicker m_mousePicker;
		Engine::MenuEntry* m_currentMenuEntry;
		//Engine::QuitGameCommand m_quitGameCommand;
#ifdef CALCULATE_GAME_STATS
		Math::Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MenuGameState */

} /* end namespace Game */

#endif /* __MENU_GAME_STATE_H__ */