#pragma once

#include <GLFW\glfw3.h>

namespace Rendering
{

class Input
{
/* ==================== Constructors and destructors begin ==================== */
public:
	Input(void);
	~Input(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Static member functions begin ==================== */
public:
	static bool IsLeftAltPressed(GLFWwindow* window);
	static bool IsRightAltPressed(GLFWwindow* window);
	static bool IsLeftShiftPressed(GLFWwindow* window);
	static bool IsRightShiftPressed(GLFWwindow* window);
	static bool IsLeftCtrlPressed(GLFWwindow* window);
	static bool IsRightCtrlPressed(GLFWwindow* window);
/* ==================== Static member functions end ==================== */

}; /* end class Input */

} /* end namespace Rendering */
