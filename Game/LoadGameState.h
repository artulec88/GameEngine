#ifndef __LOAD_GAME_STATE_H__
#define __LOAD_GAME_STATE_H__

#include "Rendering\Renderer.h"
#include "Engine\GameState.h"
#include "Engine\IRenderable.h"
#include "Engine\IUpdateable.h"
#include "Engine\GameNode.h"
#include "Math\Math.h"
#include "GameStats.h"
#ifdef CALCULATE_GAME_STATS
#include "Math\IStatisticsStorage.h"
#endif

#include <thread>
#include <string>

namespace Game
{

	class Rendering::Shader;

	/* TODO: Consider inheriting from Rendering::IUpdateable class */
	class LoadGameState : public Engine::GameState, public virtual Engine::IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		LoadGameState(Engine::GameManager* gameManager, const std::string& inputMappingContextName);
		virtual ~LoadGameState(void);
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

		virtual void Update(Math::Real elapsedTime);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Engine::GameManager* m_gameManager;
		Math::Real m_loadingProgress;
		std::thread* m_loadingThread;
#ifdef CALCULATE_GAME_STATS
		Math::Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LoadGameState */

} /* end namespace Game */

#endif /* __LOAD_GAME_STATE_H__ */