#include "StdAfx.h"
#include "Input.h"
#include "Utility\ILogger.h"

using namespace Rendering;
using namespace Utility;

///* static */ bool Input::inputs[NUM_KEYCODES];
///* static */ std::list<int> Input::downKeys;
///* static */ std::list<int> Input::upKeys;
///* static */ std::list<int> Input::repeatedKeys;
//
///* static */ bool Input::mouseInputs[NUM_MOUSEBUTTONS];
///* static */ std::list<int> Input::downMouse;
///* static */ std::list<int> Input::upMouse;
///* static */ std::list<int> Input::repeatedMouse;
//
//Input::Input(void)
//{
//}
//
//
//Input::~Input(void)
//{
//}
//
//void Input::Update()
//{
//	downKeys.clear();
//	upKeys.clear();
//	repeatedKeys.clear();
//	downMouse.clear();
//	upMouse.clear();
//}
//
//void Input::UpdateKey(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (key == GLFW_KEY_ESCAPE)
//	{
//		glfwSetWindowShouldClose(window, GL_TRUE);
//		return;
//	}
//
//	//if (lastKeys[key])
//	//{
//		// the key was pressed and is still pressed
//	//}
//	//lastKeys[key] = ! lastKeys[key];
//
//	switch (action)
//	{
//	case GLFW_PRESS:
//		LOG(Delocust, LOGPLACE, "Pressed key %d", key);
//		//inputs[key] = true;
//		downKeys.push_back(key);
//		break;
//	case GLFW_RELEASE:
//		LOG(Delocust, LOGPLACE, "Released key %d", key);
//		//inputs[key] = false;
//		upKeys.push_back(key);
//		break;
//	case GLFW_REPEAT:
//		LOG(Delocust, LOGPLACE, "Repeated key %d", key);
//		//inputs[key] = true;
//		repeatedKeys.push_back(key);
//		break;
//	default:
//		// should not happen
//		break;
//	}
//}
//
///* static */ Input::KeyValue Input::GetKey(int keyCode)
//{
//	//LOG(Debug, LOGPLACE, "Number of pressed, released, repeated keys = %d; %d; %d", downKeys.size(), upKeys.size(), repeatedKeys.size());
//	for (std::list<int>::const_iterator it = downKeys.begin(); it != downKeys.end(); ++it)
//	{
//		if ((*it) == keyCode)
//			return Input::KeyPressed;
//	}
//	for (std::list<int>::const_iterator it = upKeys.begin(); it != upKeys.end(); ++it)
//	{
//		if ((*it) == keyCode)
//			return Input::KeyReleased;
//	}
//	for (std::list<int>::const_iterator it = repeatedKeys.begin(); it != repeatedKeys.end(); ++it)
//	{
//		if ((*it) == keyCode)
//			return Input::KeyRepeated;
//	}
//	return Input::KeyNone;
//}
//
///* static */ bool Input::IsKeyPressed(int keyCode)
//{
//	Input::KeyValue keyValue = Input::GetKey(keyCode);
//	return ( (keyValue == Input::KeyPressed) || (keyValue == Input::KeyRepeated) );
//}
//
/////*static*/ bool Input::IsLeftAltPressed(GLFWwindow* window)
////{
////	return (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS);
////}
////
/////*static*/ bool Input::IsRightAltPressed(GLFWwindow* window)
////{
////	return (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS);
////}
////
/////*static*/ bool Input::IsLeftShiftPressed(GLFWwindow* window)
////{
////	return (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
////}
////
/////*static*/ bool Input::IsRightShiftPressed(GLFWwindow* window)
////{
////	return (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
////}
////
/////*static*/ bool Input::IsLeftCtrlPressed(GLFWwindow* window)
////{
////	return (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);
////}
////
/////*static*/ bool Input::IsRightCtrlPressed(GLFWwindow* window)
////{
////	return (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
////}