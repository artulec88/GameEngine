#ifndef __PLAY_MENU_GAME_STATE_H__
#define __PLAY_MENU_GAME_STATE_H__

#include "Def.h"
#include "Rendering/Renderer.h"
#include "Engine/GameState.h"
#include "Math/Math.h"
#include "Engine/GameNode.h"
#include "Engine/MenuEntry.h"
#include "Engine/GameCommand.h"

#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math/StatisticsStorage.h"
#include "Math/Statistics.h"
#endif

#include <string>

namespace game
{
	class PlayMenuGameState : public engine::GameState
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Loading game state constructor. </summary>
		/// <param name="gameManager"> The pointer to game manager this game state will operate on. </param>
		/// <param name="inputMappingContextName"> The <code>string</code> representing the name of the input mapping context. </param>
		/// <param name="playMainMenuFont"> The font that play main menu game state is supposed to use. </param>
		/// <param name="playMainMenuFontSize"> The font size that that play main menu game state is supposed to use. </param>
		PlayMenuGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName, const rendering::text::Font* playMainMenuFont, math::Real playMainMenuFontSize);

		/// <summary> Load game state destructor. </summary>
		virtual ~PlayMenuGameState();

		/// <summary> Play main menu game state copy constructor. </summary>
		/// <param name="playMenuGameState"> The reference to play main menu game state to copy construct from. </param>
		PlayMenuGameState(const PlayMenuGameState& playMenuGameState) = delete;

		/// <summary> Play main menu game state move constructor. </summary>
		/// <param name="playMenuGameState"> The r-value reference to play main menu game state to move construct from. </param>
		PlayMenuGameState(PlayMenuGameState&& playMenuGameState) = delete;

		/// <summary> Play main menu game state copy assignment operator. </summary>
		/// <param name="playMenuGameState"> The reference to play main menu game state to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned play main menu game state. </returns>
		PlayMenuGameState& operator=(const PlayMenuGameState& playMenuGameState) = delete;

		/// <summary> Play main menu game state move assignment operator. </summary>
		/// <param name="playMenuGameState"> The r-value reference to play main menu game state to move assign from. </param>
		/// <returns> The reference to the newly move-assigned play main menu game state. </returns>
		PlayMenuGameState& operator=(PlayMenuGameState&& playMenuGameState) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Entered() override;
		void Leaving() override;
		void Obscuring() override;
		void Revealed() override;
		void Handle(engine::actions::Action action) override;
		void Handle(engine::states::State state) override;
		void Handle(engine::ranges::Range range, math::Real value) override;

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

} /* end namespace game */

#endif /* __PLAY_MENU_GAME_STATE_H__ */