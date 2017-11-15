#ifndef __LOAD_GAME_STATE_H__
#define __LOAD_GAME_STATE_H__

#include "Def.h"
#include "Rendering\Renderer.h"
#include "Engine\GameState.h"
#include "Engine\IRenderable.h"
#include "Engine\IUpdateable.h"
#include "Engine\GameNode.h"
#include "Math\Math.h"
#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math\StatisticsStorage.h"
#include "Math\Statistics.h"
#endif

#include <thread>
#include <string>

namespace Game
{

	class Rendering::Shader;

	/* TODO: Consider inheriting from Rendering::IUpdateable class */
	class LoadGameState : public engine::GameState, public virtual engine::IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		LoadGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName);
		virtual ~LoadGameState(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Entered();
		virtual void Leaving();
		virtual void Obscuring();
		virtual void Revealed();
		virtual void Handle(engine::Actions::Action action);
		virtual void Handle(engine::States::State state);
		virtual void Handle(engine::Ranges::Range range, math::Real value);

		virtual void Render(Rendering::Renderer* renderer) const;

		virtual void Update(math::Real elapsedTime);
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