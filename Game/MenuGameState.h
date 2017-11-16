#ifndef __MENU_GAME_STATE_H__
#define __MENU_GAME_STATE_H__

#include "Def.h"
#include "Engine/GameState.h"
#include "Engine/IUpdateable.h"
#include "Engine/GameNode.h"
#include "Engine/MenuEntry.h"
#include "Engine/MousePicker.h"

#include "Rendering/Renderer.h"
#include "Rendering/ParticlesSystem.h"

#include "Math/EffectFactory.h"
#include "Math/Effect.h"
//#include "Math/Effect_impl.h"

#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math/StatisticsStorage.h"
#include "Math/Statistics.h"
#endif

#include <string>

namespace Game
{
	class MenuGameState : public engine::GameState, public engine::IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		MenuGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName, const Rendering::Text::Font* mainMenuFont, math::Real mainMenuFontSize);
		virtual ~MenuGameState();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Entered() override;
		void Leaving() override;
		void Obscuring() override;
		void Revealed() override;
		void Handle(engine::Actions::Action action) override;
		void Handle(engine::States::State state) override;
		void Handle(engine::Ranges::Range range, math::Real value) override;

		void Render(Rendering::Renderer* renderer) const override;

		void Update(math::Real deltaTime) override;
	private:
		void DeselectAll();
		void SelectChild(size_t childIndex);
		void RenderParticles(Rendering::Renderer* renderer) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		engine::GameManager* m_gameManager;
		//Rendering::Particles::ParticlesSystem* m_particlesSystem;
		engine::CompositeMenuEntry m_mainMenuRootEntry;

		// TODO: Create a Factory of TextEffects which creates text effects (but not dynamically, on the heap) and stores them under some special enum key.
		// Then pass these variables' address to initialize these following pointers. E.g. textEffectFactory.GetTextEffect(MAIN_MENU_NOT_SELECTED_ENTRY_COLOR_EFFECT).
		// Then remove the SAFE_DELETE(textEffect) from the MenuGameState destructor.
		std::unique_ptr<math::effects::Effect<Rendering::Color>> m_notSelectedMenuEntryColorEffect;
		std::unique_ptr<math::effects::Effect<Rendering::Color>> m_selectedMenuEntryColorEffect;
		std::unique_ptr<math::effects::Effect<Rendering::Color>> m_notSelectedMenuEntryOutlineColorEffect;
		std::unique_ptr<math::effects::Effect<Rendering::Color>> m_selectedMenuEntryOutlineColorEffect;
		std::unique_ptr<math::effects::Effect<math::Vector2D>> m_notSelectedMenuEntryOffsetEffect;
		std::unique_ptr<math::effects::Effect<math::Vector2D>> m_selectedMenuEntryOffsetEffect;
		std::unique_ptr<math::effects::Effect<math::Real>> m_notSelectedMenuEntryCharacterWidthEffect;
		std::unique_ptr<math::effects::Effect<math::Real>> m_selectedMenuEntryCharacterWidthEffect;
		std::unique_ptr<math::effects::Effect<math::Real>> m_notSelectedMenuEntryCharacterEdgeTransitionWidthEffect;
		std::unique_ptr<math::effects::Effect<math::Real>> m_selectedMenuEntryCharacterEdgeTransitionWidthEffect;
		std::unique_ptr<math::effects::Effect<math::Real>> m_notSelectedMenuEntryBorderWidthEffect;
		std::unique_ptr<math::effects::Effect<math::Real>> m_selectedMenuEntryBorderWidthEffect;
		std::unique_ptr<math::effects::Effect<math::Real>> m_notSelectedMenuEntryBorderEdgeTransitionWidthEffect;
		std::unique_ptr<math::effects::Effect<math::Real>> m_selectedMenuEntryBorderEdgeTransitionWidthEffect;

		math::Real m_mousePosX, m_mousePosY;

		engine::MousePicker m_mousePicker;
		engine::MenuEntry* m_currentMenuEntry;
#ifdef PROFILING_GAME_MODULE_ENABLED
		math::statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MenuGameState */

} /* end namespace Game */

#endif /* __MENU_GAME_STATE_H__ */