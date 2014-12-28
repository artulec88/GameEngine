#pragma once

#ifdef RENDERING_EXPORTS
#define RENDERING_API __declspec(dllexport)
#else
#define RENDERING_API __declspec(dllimport)
#endif

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL\glew.h>
// Include GLFW
#include <GLFW\glfw3.h>
#include <string>

//#define ANT_TWEAK_BAR_ENABLED

namespace Rendering
{
	enum AntiAliasingMethod
	{
		NONE = 0,
		MSAA,
		FXAA
	};

	GLFWwindow* InitGraphics(int width, int height, const std::string& title);
	GLFWwindow* InitGlfw(int width, int height, const std::string& title);
	void InitGlew();
	void CheckErrorCode(const char* functionName, const char* comment);

	extern AntiAliasingMethod antiAliasingMethod;
} /* end namespace Rendering */