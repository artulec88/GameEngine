#include "StdAfx.h"
#include "GameState.h"

using namespace Rendering;

GameState::GameState(void)
{
}


GameState::~GameState(void)
{
}

void IntroGameState::Input(Math::Real elapsedTime, GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "INTRO input");
}

void IntroGameState::Render(Renderer* renderer, const GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "Rendering INTRO");
}

void MainMenuGameState::Input(Math::Real elapsedTime, GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "MAIN MENU input");
}

void MainMenuGameState::Render(Renderer* renderer, const GameNode& gameNode)
{
	LOG(Utility::Debug, LOGPLACE, "Rendering MAIN MENU");

	//renderer->RenderMainMenu();

	renderer->Render(gameNode);
}