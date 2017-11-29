#ifndef __ENGINE_ACTION_CONSTANTS_H__
#define __ENGINE_ACTION_CONSTANTS_H__

namespace engine
{
	namespace actions
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
			GO_TO_CHILD_MENU_ENTRY, // game state action
			MOVE_CAMERA_UP,
			MOVE_CAMERA_DOWN,
			MOVE_CAMERA_LEFT,
			MOVE_CAMERA_RIGHT,
			MOVE_CAMERA_FORWARD,
			MOVE_CAMERA_BACKWARD
		}; /* end enum Action */
	} /* end namespace actions */

	namespace states
	{
		enum State
		{
			INVALID = 0,
			MOUSE_KEY_LEFT_PRESSED,
			MOUSE_KEY_MIDDLE_PRESSED,
			MOUSE_KEY_RIGHT_PRESSED,
			MOVE_CAMERA_UP,
			MOVE_CAMERA_DOWN,
			MOVE_CAMERA_LEFT,
			MOVE_CAMERA_RIGHT,
			MOVE_CAMERA_FORWARD,
			MOVE_CAMERA_BACKWARD
		}; /* end enum State */
	} /* end namespace states */

	namespace ranges
	{
		enum Range
		{
			INVALID = 0,
			AXIS_X,
			AXIS_Y
		}; /* end enum Range */
	} /* end namespace ranges */
} /* end namespace engine */

#endif /* __ENGINE_ACTION_CONSTANTS_H__ */