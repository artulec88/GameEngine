#ifndef __ENGINE_GAME_COMMAND_FACTORY_H__
#define __ENGINE_GAME_COMMAND_FACTORY_H__

#include "Engine\Engine.h"
#include "Engine\GameCommand.h"

#include "Math\IStatisticsStorage.h"

#include "Utility\ILogger.h"

#include <map>

namespace Engine
{
	namespace GameCommandTypes
	{
		enum GameCommandType
		{
			EMPTY = 0,
			START,
			QUIT,
			RESUME,
			SAVE,
			LOAD,
			SHOW_INTRO
		}; /* end enum GameCommandType */
	} /* end namespace GameCommandTypes */

	class GameCommandFactory
	{
		typedef std::map<GameCommandTypes::GameCommandType, GameCommand*> GameCommandsMap;
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		GameCommandFactory();
		virtual ~GameCommandFactory(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>
		/// Creates game command based on the specified game command type.
		/// </summary>
		/// <param name="gameCommandType> Game comand type we want to instantiate </param>
		ENGINE_API virtual GameCommand& GetCommand(GameCommandTypes::GameCommandType gameCommandType)
		{
			if (m_commands[gameCommandType] != NULL)
			{
				return *m_commands[gameCommandType];
			}
			else
			{
				ERROR_LOG_ENGINE("No game command registered for the specified game command type %d", gameCommandType);
				return *m_commands[GameCommandTypes::EMPTY];
			}
		}
		ENGINE_API void CreateCommand(GameCommandTypes::GameCommandType gameCommandType, GameCommand* gameCommand)
		{
			if (m_commands[gameCommandType] != NULL)
			{
				WARNING_LOG_ENGINE("Replacing the concrete implementation for the specified %d game command type with a new one.", gameCommandType);
				SAFE_DELETE(m_commands[gameCommandType]);
			}
			m_commands[gameCommandType] = gameCommand;
		}
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		GameCommandsMap m_commands;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class GameCommandFactory */

} /* end namespace Engine */

#endif /* __ENGINE_GAME_COMMAND_FACTORY_H__ */