#include "MainMenuGameState.h"

using namespace Game;

MainMenuGameState::MainMenuGameState(void) :
	Rendering::GameState()
{
}

MainMenuGameState::~MainMenuGameState(void)
{
}

void MainMenuGameState::Entered()
{
}

void MainMenuGameState::Leaving()
{
}

void MainMenuGameState::Obscuring()
{
}

void MainMenuGameState::Revealed()
{
}

void MainMenuGameState::KeyEvent(int key, int scancode, int action, int mods)
{
	LOG(Utility::Critical, LOGPLACE, "Key event");
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		LOG(Utility::Critical, LOGPLACE, "To exit the game click \"QUIT\"");
		break;
	default:
		LOG(Utility::Critical, LOGPLACE, "To start the game click \"START\"");
		break;
	}
}

void MainMenuGameState::Input(Math::Real elapsedTime, Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "MAIN MENU game state input processing");
}

void MainMenuGameState::Render(Rendering::Renderer* renderer, const Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "MAIN MENU game state rendering");
	renderer->RenderMainMenu();
}