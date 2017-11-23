#ifndef __PLAY_MENU_GAME_STATE_H__
#define __PLAY_MENU_GAME_STATE_H__

#include "Def.h"
#include "Rendering\Renderer.h"
#include "Engine\GameState.h"
#include "Engine\IRenderable.h"
#include "Math\Math.h"
#include "Engine\GameNode.h"
#include "Engine\MenuEntry.h"
#include "Engine\GameCommand.h"

#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math/StatisticsStorage.h"
#include "Math/Statistics.h"
#endif

#include <vector>
#include <string>

namespace Game
{

	class rendering::Shader;

	class PlayMenuGameState : public engine::GameState
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		PlayMenuGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName, const rendering::text::Font* playMainMenuFont, math::Real playMainMenuFontSize);
		virtual ~PlayMenuGameState();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Entered() override;
		void Leaving() override;
		void Obscuring() override;
		void Revealed() override;
		void Handle(engine::Actions::Action action) override;
		void Handle(engine::States::State state) override;
		void Handle(engine::Ranges::Range range, math::Real value) override;

		void Render(rendering::Renderer* renderer) const override;

		//void MouseButtonEvent(int button, int action, int mods) override;
		//void MousePosEvent(double xPos, double yPos) override;
		//void ScrollEvent(double xOffset, double yOffset) override;

	private:
		void DeselectAll();
		void SelectChild(int childIndex);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		engine::GameManager* m_gameManager;
		engine::CompositeMenuEntry m_playMainMenuRootEntry;
		//Rendering::effects::Effect<math::Vector3D>* m_notSelectedMenuEntryColorEffect;
		//Rendering::effects::Effect<math::Vector3D>* m_selectedMenuEntryColorEffect;
		//Rendering::effects::Effect<math::Vector3D>* m_notSelectedMenuEntryOutlineColorEffect;
		//Rendering::effects::Effect<math::Vector3D>* m_selectedMenuEntryOutlineColorEffect;
		//Rendering::effects::Effect<math::Vector2D>* m_notSelectedMenuEntryOffsetEffect;
		//Rendering::effects::Effect<math::Vector2D>* m_selectedMenuEntryOffsetEffect;
		math::Real m_mousePosX, m_mousePosY;
		engine::MenuEntry* m_currentMenuEntry;
#ifdef PROFILING_GAME_MODULE_ENABLED
		math::statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PlayMenuGameState */

} /* end namespace Game */

#endif /* __PLAY_MENU_GAME_STATE_H__ */