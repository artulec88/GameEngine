#include "stdafx.h"
#include "GameCommandFactory.h"

Engine::GameCommandFactory::GameCommandFactory()
{
	m_commands[GameCommandTypes::EMPTY] = new EmptyGameCommand();
}


Engine::GameCommandFactory::~GameCommandFactory()
{
	//for (GameCommandsMap::iterator gameCommandItr = m_commands.begin(); gameCommandItr != m_commands.end(); ++gameCommandItr)
	//{
	//	SAFE_DELETE(gameCommandItr->second);
	//}
	//m_commands.clear();
}
