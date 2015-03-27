#include "IntroGameState.h"

using namespace Game;

IntroGameState::IntroGameState(void) :
	Rendering::GameState()
{
}

IntroGameState::~IntroGameState(void)
{
}

void IntroGameState::Entered()
{
}

void IntroGameState::Leaving()
{
}

void IntroGameState::Obscuring()
{
}

void IntroGameState::Revealed()
{
}

void IntroGameState::KeyEvent(int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		LOG(Utility::Info, LOGPLACE, "To skip the intro click ESC twice.");
		break;
	}
}

void IntroGameState::Input(Math::Real elapsedTime, Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "INTRO game state input processing");
}

void IntroGameState::Render(Rendering::Renderer* renderer, const Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "INTRO game state rendering");
}