#include "MenuGameState.h"

using namespace Game;

/* static */ Rendering::MenuEntry MenuGameState::s_mainMenuEntry;

/* static */ void MenuGameState::InitializeMainMenu()
{
	//s_mainMenuEntry.AddChildren(new Rendering::MenuEntry("Start"));
	//s_mainMenuEntry.AddChildren(new Rendering::MenuEntry("Options"));
	//s_mainMenuEntry.AddChildren(new Rendering::MenuEntry("Exit"));
}

MenuGameState::MenuGameState(void) :
	Rendering::GameState(),
	m_currentMenuEntry(NULL)
{
	m_currentMenuEntry = new Rendering::MenuEntry("Main menu");
	m_currentMenuEntry->AddChildren(new Rendering::MenuEntry("Start"));
	m_currentMenuEntry->AddChildren(new Rendering::MenuEntry("Options"));
	m_currentMenuEntry->AddChildren(new Rendering::MenuEntry("Exit"));

	s_mainMenuEntry = *m_currentMenuEntry;
}

MenuGameState::~MenuGameState(void)
{
	SAFE_DELETE(m_currentMenuEntry);
}

void MenuGameState::Entered()
{
}

void MenuGameState::Leaving()
{
}

void MenuGameState::Obscuring()
{
}

void MenuGameState::Revealed()
{
}

void MenuGameState::KeyEvent(int key, int scancode, int action, int mods)
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

void MenuGameState::Input(Math::Real elapsedTime, Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "MAIN MENU game state input processing");
}

void MenuGameState::Render(Rendering::Renderer* renderer, const Rendering::GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "MAIN MENU game state rendering");
	renderer->RenderMainMenu(*m_currentMenuEntry);
}