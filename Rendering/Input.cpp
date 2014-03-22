#include "StdAfx.h"
#include "Input.h"

using namespace Rendering;

Input::Input(void)
{
}


Input::~Input(void)
{
}

/*static*/ bool Input::IsLeftAltPressed(GLFWwindow* window)
{
	return (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS);
}

/*static*/ bool Input::IsRightAltPressed(GLFWwindow* window)
{
	return (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS);
}

/*static*/ bool Input::IsLeftShiftPressed(GLFWwindow* window)
{
	return (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
}

/*static*/ bool Input::IsRightShiftPressed(GLFWwindow* window)
{
	return (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
}

/*static*/ bool Input::IsLeftCtrlPressed(GLFWwindow* window)
{
	return (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);
}

/*static*/ bool Input::IsRightCtrlPressed(GLFWwindow* window)
{
	return (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
}