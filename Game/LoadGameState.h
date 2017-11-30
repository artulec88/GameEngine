#ifndef __LOAD_GAME_STATE_H__
#define __LOAD_GAME_STATE_H__

#include "Def.h"

#include "Engine/GameState.h"
#include "Engine/IUpdateable.h"
#include "Engine/GameNode.h"
#include "Engine/GameManager.h"

#include "Math/Math.h"
#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math/StatisticsStorage.h"
#include "Math/Statistics.h"
#endif

#include <thread>
#include <string>

namespace game
{
	/* TODO: Consider inheriting from Rendering::IUpdateable class */
	class LoadGameState : public engine::GameState, public virtual engine::IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Loading game state constructor. </summary>
		/// <param name="gameManager"> The pointer to game manager this game state will operate on. </param>
		/// <param name="inputMappingContextName"> The <code>string</code> representing the name of the input mapping context. </param>
		LoadGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName);

		/// <summary> Load game state destructor. </summary>
		virtual ~LoadGameState();

		/// <summary> Load game state copy constructor. </summary>
		/// <param name="loadGameState"> The reference to load game state to copy construct from. </param>
		LoadGameState(const LoadGameState& loadGameState) = delete;

		/// <summary> Load game state move constructor. </summary>
		/// <param name="loadGameState"> The r-value reference to load game state to move construct from. </param>
		LoadGameState(LoadGameState&& loadGameState) = delete;

		/// <summary> Load game state copy assignment operator. </summary>
		/// <param name="loadGameState"> The reference to load game state to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned load game state. </returns>
		LoadGameState& operator=(const LoadGameState& loadGameState) = delete;

		/// <summary> Load game state move assignment operator. </summary>
		/// <param name="loadGameState"> The r-value reference to load game state to move assign from. </param>
		/// <returns> The reference to the newly move-assigned load game state. </returns>
		LoadGameState& operator=(LoadGameState&& loadGameState) = delete;
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

		void Update(math::Real elapsedTime) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		engine::GameManager* m_gameManager;
		math::Real m_loadingProgress;
		std::thread* m_loadingThread;
#ifdef PROFILING_GAME_MODULE_ENABLED
		math::statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LoadGameState */

} /* end namespace game */

#endif /* __LOAD_GAME_STATE_H__ */