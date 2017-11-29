#ifndef __ENGINE_GAME_COMMAND_FACTORY_H__
#define __ENGINE_GAME_COMMAND_FACTORY_H__

//#include "Engine.h"
//#include "GameCommand.h"
//#include "InputConstants.h"
//
//#include "Math/StatisticsStorage.h"
//
//#include "Utility/ILogger.h"
//
//#include <map>
//
//namespace engine
//{
//	class GameCommandFactory
//	{
//		typedef std::map<Input::Actions::Action, GameCommand*> GameCommandsMap;
//	/* ==================== Static variables and functions begin ==================== */
//	/* ==================== Static variables and functions end ==================== */
//
//	/* ==================== Constructors and destructors begin ==================== */
//	public:
//		GameCommandFactory();
//		virtual ~GameCommandFactory(void);
//	/* ==================== Constructors and destructors end ==================== */
//
//	/* ==================== Non-static member functions begin ==================== */
//	public:
//		/// <summary>
//		/// Retrieves the game command registered for the specified action.
//		/// </summary>
//		/// <param name="action"> The action we want to instantiate the game command for. </param>
//		ENGINE_API virtual GameCommand& GetCommand(Input::Actions::Action action)
//		{
//			if (m_commands[action] != NULL)
//			{
//				return *m_commands[action];
//			}
//			else
//			{
//				ERROR_LOG_ENGINE("No game command registered for the specified action ", action);
//				return *m_commands[Input::Actions::Action::EMPTY];
//			}
//		}
//		/// <summary>
//		/// Creates game command based on the specified game command type.
//		/// </summary>
//		/// <param name="action"> The action we want to the game command to be registered for. </param>
//		/// <param name="gameCommand"> The game command that will be registered as the handler for the specified action. </param>
//		ENGINE_API void CreateCommand(Input::Actions::Action action, GameCommand* gameCommand)
//		{
//			if (m_commands[action] != NULL)
//			{
//				WARNING_LOG_ENGINE("Replacing the concrete implementation for the specified action ", action, " with a new one.");
//				SAFE_DELETE(m_commands[action]);
//			}
//			m_commands[action] = gameCommand;
//		}
//	/* ==================== Non-static member functions end ==================== */
//
//	/* ==================== Non-static member variables begin ==================== */
//	private:
//		GameCommandsMap m_commands;
//	/* ==================== Non-static member variables end ==================== */
//	}; /* end class GameCommandFactory */
//
//} /* end namespace engine */

#endif /* __ENGINE_GAME_COMMAND_FACTORY_H__ */