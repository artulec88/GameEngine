#ifndef __INTRO_GAME_STATE_H__
#define __INTRO_GAME_STATE_H__

#include "Rendering\Renderer.h"
#include "Engine\IRenderable.h"
#include "Engine\GameState.h"
//#include "Rendering\GameCommand.h"
#include "Math\Math.h"
#include "Engine\GameNode.h"
#include "GameStats.h"
#ifdef CALCULATE_GAME_STATS
#include "Math\IStatisticsStorage.h"
#endif

#include <string>

namespace Game
{

class IntroGameState : public Engine::GameState, public virtual Engine::IRenderable
{
/* ==================== Constructors and destructors begin ==================== */
public:
	IntroGameState(const std::string& inputMappingContextName);
	virtual ~IntroGameState(void);
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

	virtual void Render(const Rendering::Shader* shader, Rendering::Renderer* renderer) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
#ifdef CALCULATE_GAME_STATS
	Math::Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class IntroGameState */

} /* end namespace Game */

#endif /* __INTRO_GAME_STATE_H__ */