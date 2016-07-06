#ifndef __ENGINE_ACTION_CONSTANTS_H__
#define __ENGINE_ACTION_CONSTANTS_H__

namespace Engine
{
	namespace Actions
	{
		enum Action
		{
			INVALID = 0,
			EMPTY,
			START_GAME, // game state action
			SHOW_INTRO, // game state action
			SHOW_MENU, // game state action
			SHOW_PLAY_MENU, // game state action
			QUIT_GAME, // game state action
			QUIT_INTRO, // game state action
			RESUME_GAME, // game state action
			SAVE_GAME, // game state action
			LOAD_GAME, // game state action
			SELECT_NEXT_MENU_ENTRY, // game state action
			SELECT_PREVIOUS_MENU_ENTRY, // game state action
			CHOOSE_CURRENT_MENU_ENTRY, // game state action
			RETURN_TO_PARENT_MENU_ENTRY, // game state action
			MOVE,
			ONE,
			TWO,
			THREE,
			FOUR,
			FIVE,
			SIX,
			SEVEN
		}; /* end enum Action */
	} /* end namespace Actions */

	namespace States
	{
		enum State
		{
			INVALID = 0,
			MOUSE_KEY_LEFT_PRESSED,
			MOUSE_KEY_MIDDLE_PRESSED,
			MOUSE_KEY_RIGHT_PRESSED
		}; /* end enum State */
	} /* end namespace States */

	namespace Ranges
	{
		enum Range
		{
			INVALID = 0,
			AXIS_X,
			AXIS_Y
		}; /* end enum Range */
	} /* end namespace Ranges */
} /* end namespace Engine */

#endif /* __ENGINE_ACTION_CONSTANTS_H__ */