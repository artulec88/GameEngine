#ifndef __LOAD_GAME_STATE_H__
#define __LOAD_GAME_STATE_H__

#include "Def.h"

#include "Engine/GameState.h"
#include "Engine/IRenderable.h"
#include "Engine/IUpdateable.h"
#include "Engine/GameNode.h"
#include "Engine/GameManager.h"

#include "Rendering/Renderer.h"

#include "Math/Math.h"
#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math/StatisticsStorage.h"
#include "Math/Statistics.h"
#endif

#include <thread>
#include <string>

namespace Game
{
	/* TODO: Consider inheriting from Rendering::IUpdateable class */
	class LoadGameState : public engine::GameState, public virtual engine::IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		LoadGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName);
		virtual ~LoadGameState();
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

} /* end namespace Game */

#endif /* __LOAD_GAME_STATE_H__ */