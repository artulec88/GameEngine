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

#define BASIC_SHADOW_MAPPING_ENABLED
#define VARIANCE_SHADOW_MAPPING_ENABLED

#ifdef VARIANCE_SHADOW_MAPPING_ENABLED
#undef BASIC_SHADOW_MAPPING_ENABLED
#endif

namespace Rendering
{
} /* end namespace Rendering */