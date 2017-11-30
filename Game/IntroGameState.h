#ifndef __INTRO_GAME_STATE_H__
#define __INTRO_GAME_STATE_H__

#include "Def.h"

#include "Engine/GameState.h"
#include "Engine/GameManager.h"
#include "Engine/GameNode.h"

#include "Math/Math.h"

#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math/StatisticsStorage.h"
#include "Math/Statistics.h"
#endif

#include <string>

namespace game
{

	class IntroGameState : public engine::GameState
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Intro game state constructor. </summary>
		/// <param name="gameManager"> The pointer to game manager this game state will operate on. </param>
		/// <param name="inputMappingContextName"> The <code>string</code> representing the name of the input mapping context. </param>
		IntroGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName);

		/// <summary> Intro game state destructor. </summary>
		virtual ~IntroGameState();

		/// <summary> Intro game state copy constructor. </summary>
		/// <param name="introGameState"> The reference to intro game state to copy construct from. </param>
		IntroGameState(const IntroGameState& introGameState) = delete;

		/// <summary> Intro game state move constructor. </summary>
		/// <param name="introGameState"> The r-value reference to intro game state to move construct from. </param>
		IntroGameState(IntroGameState&& introGameState) = delete;

		/// <summary> Intro game state copy assignment operator. </summary>
		/// <param name="introGameState"> The reference to intro game state to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned intro game state. </returns>
		IntroGameState& operator=(const IntroGameState& introGameState) = delete;

		/// <summary> Intro game state move assignment operator. </summary>
		/// <param name="introGameState"> The r-value reference to intro game state to move assign from. </param>
		/// <returns> The reference to the newly move-assigned intro game state. </returns>
		IntroGameState& operator=(IntroGameState&& introGameState) = delete;
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
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		engine::GameManager* m_gameManager;
#ifdef PROFILING_GAME_MODULE_ENABLED
		math::statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IntroGameState */

} /* end namespace game */

#endif /* __INTRO_GAME_STATE_H__ */