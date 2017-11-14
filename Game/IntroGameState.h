#ifndef __INTRO_GAME_STATE_H__
#define __INTRO_GAME_STATE_H__

#include "Def.h"
#include "Rendering\Renderer.h"
#include "Engine\IRenderable.h"
#include "Engine\GameState.h"
//#include "Rendering\GameCommand.h"
#include "Math\Math.h"
#include "Engine\GameNode.h"
#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math\StatisticsStorage.h"
#include "Math\Statistics.h"
#endif

#include <string>

namespace Game
{

	class IntroGameState : public engine::GameState
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		IntroGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName);
		virtual ~IntroGameState(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Entered();
		virtual void Leaving();
		virtual void Obscuring();
		virtual void Revealed();
		virtual void Handle(engine::Actions::Action action);
		virtual void Handle(engine::States::State state);
		virtual void Handle(engine::Ranges::Range range, Math::Real value);

		virtual void Render(Rendering::Renderer* renderer) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		engine::GameManager* m_gameManager;
#ifdef PROFILING_GAME_MODULE_ENABLED
		Math::Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IntroGameState */

} /* end namespace Game */

#endif /* __INTRO_GAME_STATE_H__ */