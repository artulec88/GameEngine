#include "stdafx.h"
#include "Rendering.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

int Rendering::supportedOpenGLLevel;
std::string Rendering::glslVersion;
Rendering::AntiAliasingMethod Rendering::antiAliasingMethod = Rendering::NONE;

/* ==================== Blending parameters begin ==================== */
bool Rendering::glBlendEnabled, Rendering::glBlendEnabledOld;
GLenum Rendering::glBlendSfactor, Rendering::glBlendSfactorOld;
GLenum Rendering::glBlendDfactor, Rendering::glBlendDfactorOld;
/* ==================== Blending parameters end ==================== */

/* ==================== Color logic operation parameters begin ==================== */
bool Rendering::glColorLogicOperationEnabled, Rendering::glColorLogicOperationEnabledOld;
GLenum Rendering::glColorLogicOperationCode, Rendering::glColorLogicOperationCodeOld;
/* ==================== Color logic operation parameters end ==================== */

/* ==================== Color logic operation parameters begin ==================== */
bool Rendering::glCullFaceEnabled, Rendering::glCullFaceEnabledOld;
GLenum Rendering::glCullFaceMode, Rendering::glCullFaceModeOld;
/* ==================== Color logic operation parameters end ==================== */

/* ==================== Depth clamping parameters begin ==================== */
bool Rendering::glDepthClampEnabled, Rendering::glDepthClampEnabledOld;
/* ==================== Depth clamping parameters end ==================== */

/* ==================== Depth test parameters begin ==================== */
bool Rendering::glDepthTestEnabled, Rendering::glDepthTestEnabledOld;
GLenum Rendering::glDepthTestFunc, Rendering::glDepthTestFuncOld;
Math::Real Rendering::glDepthRangeNearValue, Rendering::glDepthRangeNearValueOld;
Math::Real Rendering::glDepthRangeFarValue, Rendering::glDepthRangeFarValueOld;
/* ==================== Depth test parameters end ==================== */

/* ==================== Dithering parameters begin ==================== */
bool Rendering::glDitheringEnabled, Rendering::glDitheringEnabledOld;
/* ==================== Dithering parameters end ==================== */

/* ==================== Front face parameters begin ==================== */
GLenum Rendering::glFrontFaceMode, Rendering::glFrontFaceModeOld;
/* ==================== Front face parameters end ==================== */

/* ==================== Scissor test parameters begin ==================== */
bool Rendering::glScissorTestEnabled, Rendering::glScissorTestEnabledOld;
GLint Rendering::glScissorBoxLowerLeftCornerX, Rendering::glScissorBoxLowerLeftCornerXOld;
GLint Rendering::glScissorBoxLowerLeftCornerY, Rendering::glScissorBoxLowerLeftCornerYOld;
GLsizei Rendering::glScissorBoxWidth, Rendering::glScissorBoxWidthOld;
GLsizei Rendering::glScissorBoxHeight, Rendering::glScissorBoxHeightOld;
/* ==================== Scissor test parameters end ==================== */

using namespace Math;

bool Rendering::FogEffect::Fog::operator<(const Rendering::FogEffect::Fog& fog) const
{
	if (fallOffType != fog.fallOffType)
	{
		return fallOffType < fog.fallOffType;
	}
	else
	{
		return calculationType < fog.calculationType;
	}
	//return (fallOffType & calculationType) < (fog.fallOffType & fog.calculationType);
}

void Rendering::DetermineGlVersion()
{
	int majorVersion;
	int minorVersion;

	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	supportedOpenGLLevel = majorVersion * 100 + minorVersion * 10;

	if (supportedOpenGLLevel >= 330)
	{
		std::ostringstream convert;
		convert << supportedOpenGLLevel;

		glslVersion = convert.str();
	}
	else if (supportedOpenGLLevel >= 320)
	{
		glslVersion = "150";
	}
	else if (supportedOpenGLLevel >= 310)
	{
		glslVersion = "140";
	}
	else if (supportedOpenGLLevel >= 300)
	{
		glslVersion = "130";
	}
	else if (supportedOpenGLLevel >= 210)
	{
		glslVersion = "120";
	}
	else if (supportedOpenGLLevel >= 200)
	{
		glslVersion = "110";
	}
	else
	{
		CRITICAL_LOG("GLSL Version %d.%d does not support shaders.\n", majorVersion, minorVersion);
		exit(EXIT_FAILURE);
	}
}

void Rendering::PrintGlReport()
{
	INFO_LOG("OpenGL report:\n\tVendor:\t\t\t\"%s\"\n\tRenderer name:\t\t\"%s\"\n\tVersion:\t\t\"%s\"\n\tGLSL version:\t\t\"%s\"",
		(const char*)glGetString(GL_VENDOR),
		(const char*)glGetString(GL_RENDERER),
		(const char*)glGetString(GL_VERSION),
		(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	//INFO_LOG("OpenGL extensions: ", (const char*)glGetString(GL_EXTENSIONS));
}

GLFWwindow* Rendering::InitGraphics(int width, int height, const std::string& title, GLFWwindow*& threadWindow)
{
	INFO_LOG("Initializing graphics started");

	GLFWwindow* window = InitGlfw(width, height, title, threadWindow);
	InitGlew();

	DetermineGlVersion();
	PrintGlReport();

	//ReadAlphaTestParameter();
	ReadBlendParameter();
	ReadColorLogicOperationParameter();
	ReadCullFaceParameter();
	ReadDepthClampParameter();
	ReadDepthTestParameter();
	ReadDitherParameter();
	ReadFrontFaceParameter();
	ReadHistogramParameter();
	//ReadMinmaxParameter();
	//ReadMultisampleParameter();
	ReadScissorTestParameter(width, height);
	ReadStencilTestParameter();
	ReadTextureParameters();

	//glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	//glEnable(GL_DEPTH_CLAMP);

	switch (Rendering::antiAliasingMethod)
	{
	case NONE:
		glDisable(GL_MULTISAMPLE); // disable multisampling
		break;
	case FXAA:
		glDisable(GL_MULTISAMPLE); // disable multisampling
		break;
	case MSAA:
		glEnable(GL_MULTISAMPLE);
		break;
	default:
		WARNING_LOG("Unknown anti-aliasing algorithm chosen");
		glEnable(GL_MULTISAMPLE);
	}

	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP);
	//glEnable(GL_FRAMEBUFFER_SRGB); // Essentialy gives free gamma correction for better contrast. TODO: Test it!
	glEnable(GL_CLIP_PLANE0); // Enables plane clipping
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	INFO_LOG("Initializing graphics finished successfully");
	return window;
}

GLFWwindow* Rendering::InitGlfw(int width, int height, const std::string& title, GLFWwindow*& threadWindow)
{
	DEBUG_LOG("Initializing GLFW started");
	if( !glfwInit() )
	{
		CRITICAL_LOG("Failed to initalize GLFW");
		exit(EXIT_FAILURE);
		// throw FileNotFoundException(); // TODO: throw another exception in the future
	}
	
	glfwWindowHint( GLFW_VISIBLE, GL_FALSE );
    threadWindow = glfwCreateWindow( 1, 1, "Thread Window", NULL, NULL );
	if (threadWindow == NULL)
	{
		CRITICAL_LOG("Failed to create GLFW thread window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	int antiAliasingSamples = GET_CONFIG_VALUE("antiAliasingSamples", 4); /* 4x anti-aliasing by default */
	switch (Rendering::antiAliasingMethod)
	{
	case NONE:
		/**
		 * TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		 * Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		 */
		glfwWindowHint(GLFW_SAMPLES, 0);
		NOTICE_LOG("No anti-aliasing algorithm chosen");
		break;
	case FXAA:
		/**
		 * TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		 * Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		 */
		glfwWindowHint(GLFW_SAMPLES, 0);
		NOTICE_LOG("FXAA anti-aliasing algorithm chosen");
		break;
	case MSAA:
		glfwWindowHint(GLFW_SAMPLES, antiAliasingSamples);
		NOTICE_LOG("%dxMSAA anti-aliasing algorithm chosen", antiAliasingSamples);
		break;
	default:
		WARNING_LOG("Unknown anti-aliasing algorithm chosen. Default %dxMSAA algorithm chosen", antiAliasingSamples);
		glfwWindowHint(GLFW_SAMPLES, antiAliasingSamples);
	}
	glfwWindowHint(GLFW_VERSION_MAJOR, 3); // TODO: Do not hard-code any values
	glfwWindowHint(GLFW_VERSION_MINOR, 3); // TODO: Do not hard-code any values
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // So that glBegin / glEnd etc. work
#else
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GL_TRUE);

	GLFWmonitor* monitor = NULL;
	bool fullscreenEnabled = GET_CONFIG_VALUE("fullscreenEnabled", false);
	if (fullscreenEnabled)
	{
		monitor = glfwGetPrimaryMonitor();
	}
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), monitor, threadWindow); // Open a window and create its OpenGL context
	if (window == NULL)
	{
		CRITICAL_LOG("Failed to create GLFW main window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we can capture the escape key being pressed below
	glfwSetCursorPos(window, width / 2, height / 2); // Set cursor position to the middle point
	//glfwSwapInterval(1);
	glfwSetTime(REAL_ZERO);
	DEBUG_LOG("Initializing GLFW finished successfully");
	return window;
}

void Rendering::InitGlew()
{
	INFO_LOG("Initializing GLEW started");
	glewExperimental = true; // Needed in core profile
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		ERROR_LOG("Error while initializing GLEW: %s", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	if (GLEW_VERSION_2_0)
	{
		DEBUG_LOG("OpenGL 2.0 supported");
	}
	else
	{
		ERROR_LOG("Initializing GLEW failed. OpenGL 2.0 NOT supported");
		exit(EXIT_FAILURE);
	}

	NOTICE_LOG("Using GLEW version %s", glewGetString(GLEW_VERSION));
	CheckErrorCode(__FUNCTION__, "Initializing GLEW");
}

/**
 * If enabled, do alpha testing. Deprecated in OpenGL 3.*
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glAlphaFunc.xml
 */
//void Rendering::ReadAlphaTestParameter()
//{
//	int alphaTestEnabled = GET_CONFIG_VALUE("GL_ALPHA_TEST_ENABLED", 0);
//	if (alphaTestEnabled == 0)
//	{
//		glDisable(GL_ALPHA_TEST);
//		DEBUG_LOG("GL_ALPHA_TEST disabled");
//		return;
//	}
//
//	glEnable(GL_ALPHA_TEST); // Perform ALPHA testing
//
//	std::string alphaFuncStr = GET_CONFIG_VALUE_STR("GL_ALPHA_TEST_FUNC", "GL_LESS");
//	Math::Real alphaFuncRefValue = GET_CONFIG_VALUE("GL_ALPHA_TEST_REF", 0.1f);
//	if (alphaFuncStr == "GL_NEVER") { glAlphaFunc(GL_NEVER, alphaFuncRefValue); }
//	else if (alphaFuncStr == "GL_LESS") { glAlphaFunc(GL_LESS, alphaFuncRefValue); }
//	else if (alphaFuncStr == "GL_EQUAL") { glAlphaFunc(GL_EQUAL, alphaFuncRefValue); }
//	else if (alphaFuncStr == "GL_LEQUAL") { glAlphaFunc(GL_LEQUAL, alphaFuncRefValue); }
//	else if (alphaFuncStr == "GL_GREATER") { glAlphaFunc(GL_GREATER, alphaFuncRefValue); }
//	else if (alphaFuncStr == "GL_NOTEQUAL") { glAlphaFunc(GL_NOTEQUAL, alphaFuncRefValue); }
//	else if (alphaFuncStr == "GL_GEQUAL") { glAlphaFunc(GL_GEQUAL, alphaFuncRefValue); }
//	else if (alphaFuncStr == "GL_ALWAYS") { glAlphaFunc(GL_ALWAYS, alphaFuncRefValue); }
//	else /* GL_ALWAYS is default */
//	{
//		ERROR_LOG("Invalid enum \"%s\" given for the alpha test function. Using default GL_ALWAYS", alphaFuncStr.c_str());
//		alphaFuncStr = "GL_ALWAYS";
//		glAlphaFunc(GL_ALWAYS, alphaFuncRefValue);
//	}
//
//	INFO_LOG("GL_ALPHA_TEST enabled with function \"%s\" and reference value = %.2f", alphaFuncStr.c_str(), alphaFuncRefValue);
//}

/**
 * If enabled, blen the computed fragment color values with the values in the color buffers.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glBlendFunc.xml
 */
void Rendering::ReadBlendParameter()
{
	glBlendEnabled = GET_CONFIG_VALUE("GL_BLEND_ENABLED", false);
	glBlendEnabledOld = glBlendEnabled;
	if (!glBlendEnabled)
	{
		glDisable(GL_BLEND);
		DEBUG_LOG("GL_BLEND disabled");
		return;
	}

	glEnable(GL_BLEND);
	std::string blendSFactorStr = GET_CONFIG_VALUE_STR("GL_BLEND_SFACTOR", "GL_ONE");
	std::string blendDFactorStr = GET_CONFIG_VALUE_STR("GL_BLEND_DFACTOR", "GL_ZERO");
	if (blendSFactorStr == "GL_ZERO") { glBlendSfactor = GL_ZERO; }
	else if (blendSFactorStr == "GL_ONE") { glBlendSfactor = GL_ONE; }
	else if (blendSFactorStr == "GL_SRC_COLOR") { glBlendSfactor = GL_SRC_COLOR; }
	else if (blendSFactorStr == "GL_ONE_MINUS_SRC_COLOR") { glBlendSfactor = GL_ONE_MINUS_SRC_COLOR; }
	else if (blendSFactorStr == "GL_DST_COLOR") { glBlendSfactor = GL_DST_COLOR; }
	else if (blendSFactorStr == "GL_ONE_MINUS_DST_COLOR") { glBlendSfactor = GL_ONE_MINUS_DST_COLOR; }
	else if (blendSFactorStr == "GL_SRC_ALPHA") { glBlendSfactor = GL_SRC_ALPHA; }
	else if (blendSFactorStr == "GL_ONE_MINUS_SRC_ALPHA") { glBlendSfactor = GL_ONE_MINUS_SRC_ALPHA; }
	else if (blendSFactorStr == "GL_DST_ALPHA") { glBlendSfactor = GL_DST_ALPHA; }
	else if (blendSFactorStr == "GL_ONE_MINUS_DST_ALPHA") { glBlendSfactor = GL_ONE_MINUS_DST_ALPHA; }
	else if (blendSFactorStr == "GL_CONSTANT_COLOR") { glBlendSfactor = GL_CONSTANT_COLOR; }
	else if (blendSFactorStr == "GL_ONE_MINUS_CONSTANT_COLOR") { glBlendSfactor = GL_ONE_MINUS_CONSTANT_COLOR; }
	else if (blendSFactorStr == "GL_CONSTANT_ALPHA") { glBlendSfactor = GL_CONSTANT_ALPHA; }
	else if (blendSFactorStr == "GL_ONE_MINUS_CONSTANT_ALPHA") { glBlendSfactor = GL_ONE_MINUS_CONSTANT_ALPHA; }
	else if (blendSFactorStr == "GL_SRC_ALPHA_SATURATE") { glBlendSfactor = GL_SRC_ALPHA_SATURATE; }
	else if (blendSFactorStr == "GL_SRC1_COLOR") { glBlendSfactor = GL_SRC1_COLOR; }
	else if (blendSFactorStr == "GL_ONE_MINUS_SRC1_COLOR") { glBlendSfactor = GL_ONE_MINUS_SRC1_COLOR; }
	else if (blendSFactorStr == "GL_SRC1_ALPHA") { glBlendSfactor = GL_SRC1_ALPHA; }
	else if (blendSFactorStr == "GL_ONE_MINUS_SRC1_ALPHA") { glBlendSfactor = GL_ONE_MINUS_SRC1_ALPHA; }
	else /* GL_ONE is default */
	{
		ERROR_LOG("Invalid enum \"%s\" given for the blend sFactor. Using default GL_ONE", blendSFactorStr.c_str());
		blendSFactorStr = "GL_ONE";
		glBlendSfactor = GL_ONE;
	}

	if (blendDFactorStr == "GL_ZERO") { glBlendDfactor = GL_ZERO; }
	else if (blendDFactorStr == "GL_ONE") { glBlendDfactor = GL_ONE; }
	else if (blendDFactorStr == "GL_SRC_COLOR") { glBlendDfactor = GL_SRC_COLOR; }
	else if (blendDFactorStr == "GL_ONE_MINUS_SRC_COLOR") { glBlendDfactor = GL_ONE_MINUS_SRC_COLOR; }
	else if (blendDFactorStr == "GL_DST_COLOR") { glBlendDfactor = GL_DST_COLOR; }
	else if (blendDFactorStr == "GL_ONE_MINUS_DST_COLOR") { glBlendDfactor = GL_ONE_MINUS_DST_COLOR; }
	else if (blendDFactorStr == "GL_SRC_ALPHA") { glBlendDfactor = GL_SRC_ALPHA; }
	else if (blendDFactorStr == "GL_ONE_MINUS_SRC_ALPHA") { glBlendDfactor = GL_ONE_MINUS_SRC_ALPHA; }
	else if (blendDFactorStr == "GL_DST_ALPHA") { glBlendDfactor = GL_DST_ALPHA; }
	else if (blendDFactorStr == "GL_ONE_MINUS_DST_ALPHA") { glBlendDfactor = GL_ONE_MINUS_DST_ALPHA; }
	else if (blendDFactorStr == "GL_CONSTANT_COLOR") { glBlendDfactor = GL_CONSTANT_COLOR; }
	else if (blendDFactorStr == "GL_ONE_MINUS_CONSTANT_COLOR") { glBlendDfactor = GL_ONE_MINUS_CONSTANT_COLOR; }
	else if (blendDFactorStr == "GL_CONSTANT_ALPHA") { glBlendDfactor = GL_CONSTANT_ALPHA; }
	else if (blendDFactorStr == "GL_ONE_MINUS_CONSTANT_ALPHA") { glBlendDfactor = GL_ONE_MINUS_CONSTANT_ALPHA; }
	else /* GL_ZERO is default */
	{
		ERROR_LOG("Invalid enum \"%s\" given for the blend dFactor. Using default GL_ZERO", blendDFactorStr.c_str());
		blendDFactorStr = "GL_ZERO";
		glBlendDfactor = GL_ZERO;
	}

	glBlendSfactorOld = glBlendSfactor;
	glBlendDfactorOld = glBlendDfactor;
	glBlendFunc(glBlendSfactor, glBlendDfactor);
	INFO_LOG("GL_BLEND enabled with sFactor = \"%s\" and dFactor = \"%s\"", blendSFactorStr.c_str(), blendDFactorStr.c_str());
	CheckErrorCode(__FUNCTION__, "Initializing blending parameters");
}

/**
 * If enabled, apply the currently selected logical operation to the computed fragment color and color buffer values.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glLogicOp.xml
 */
void Rendering::ReadColorLogicOperationParameter()
{
	glColorLogicOperationEnabled = GET_CONFIG_VALUE("GL_COLOR_LOGIC_OP_ENABLED", false);
	glColorLogicOperationEnabledOld = glColorLogicOperationEnabled;
	if (!glColorLogicOperationEnabled)
	{
		glDisable(GL_COLOR_LOGIC_OP);
		DEBUG_LOG("GL_COLOR_LOGIC_OP disabled");
		return;
	}

	glEnable(GL_COLOR_LOGIC_OP);
	std::string logicalOperationStr = GET_CONFIG_VALUE_STR("GL_COLOR_LOGIC_OPERATION", "GL_COPY");
	if (logicalOperationStr == "GL_CLEAR") { glColorLogicOperationCode = GL_CLEAR; }
	else if (logicalOperationStr == "GL_SET") { glColorLogicOperationCode = GL_SET; }
	else if (logicalOperationStr == "GL_COPY") { glColorLogicOperationCode = GL_COPY; }
	else if (logicalOperationStr == "GL_COPY_INVERTED") { glColorLogicOperationCode = GL_COPY_INVERTED; }
	else if (logicalOperationStr == "GL_NOOP") { glColorLogicOperationCode = GL_NOOP; }
	else if (logicalOperationStr == "GL_INVERT") { glColorLogicOperationCode = GL_INVERT; }
	else if (logicalOperationStr == "GL_AND") { glColorLogicOperationCode = GL_AND; }
	else if (logicalOperationStr == "GL_NAND") { glColorLogicOperationCode = GL_NAND; }
	else if (logicalOperationStr == "GL_OR") { glColorLogicOperationCode = GL_OR; }
	else if (logicalOperationStr == "GL_NOR") { glColorLogicOperationCode = GL_NOR; }
	else if (logicalOperationStr == "GL_XOR") { glColorLogicOperationCode = GL_XOR; }
	else if (logicalOperationStr == "GL_EQUIV") { glColorLogicOperationCode = GL_EQUIV; }
	else if (logicalOperationStr == "GL_AND_REVERSE") { glColorLogicOperationCode = GL_AND_REVERSE; }
	else if (logicalOperationStr == "GL_AND_INVERTED") { glColorLogicOperationCode = GL_AND_INVERTED; }
	else if (logicalOperationStr == "GL_OR_REVERSE") { glColorLogicOperationCode = GL_OR_REVERSE; }
	else if (logicalOperationStr == "GL_OR_INVERTED") { glColorLogicOperationCode = GL_OR_INVERTED; }
	else /* GL_COPY is default */
	{
		ERROR_LOG("Invalid enum \"%s\" given for the color logic operation parameter. Using default GL_COPY", logicalOperationStr.c_str());
		logicalOperationStr = "GL_COPY";
		glColorLogicOperationCode = GL_COPY;
	}

	glColorLogicOperationCodeOld = glColorLogicOperationCode;

	glLogicOp(glColorLogicOperationCode);

	INFO_LOG("GL_COLOR_LOGIC_OP enabled in \"%s\" mode", logicalOperationStr.c_str());
	CheckErrorCode(__FUNCTION__, "Initializing color logic parameters");
}

/**
 * If enabled, cull polygons based on their winding in window coordinates.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glCullFace.xml
 */
void Rendering::ReadCullFaceParameter()
{
	glCullFaceEnabled = GET_CONFIG_VALUE("GL_CULL_FACE_ENABLED", false);
	glCullFaceEnabledOld = glCullFaceEnabled;
	if (!glCullFaceEnabled)
	{
		glDisable(GL_CULL_FACE);
		DEBUG_LOG("GL_CULL_FACE disabled");
	}
	else
	{
		glEnable(GL_CULL_FACE); // culling faces enabled. Cull triangles which normal is not towards the camera
	}

	std::string cullFaceModeStr = GET_CONFIG_VALUE_STR("GL_CULL_FACE_MODE", "GL_BACK");
	if (cullFaceModeStr == "GL_FRONT") { glCullFaceMode = GL_FRONT; } // cull the front face
	else if (cullFaceModeStr == "GL_BACK") { glCullFaceMode = GL_BACK; } // cull the back face
	else if (cullFaceModeStr == "GL_FRONT_AND_BACK ") { glCullFaceMode = GL_FRONT_AND_BACK; } // cull both back and front faces (only lines, points are rendered)
	else /* GL_BACK is default */
	{
		ERROR_LOG("Invalid enum \"%s\" given for the face culling parameter. Using default GL_BACK", cullFaceModeStr.c_str());
		cullFaceModeStr = "GL_BACK";
		glCullFaceMode = GL_BACK;
	}

	glCullFaceModeOld = glCullFaceMode;

	glCullFace(glCullFaceMode);

	INFO_LOG("GL_CULL_FACE enabled in \"%s\" mode", cullFaceModeStr.c_str());
	CheckErrorCode(__FUNCTION__, "Initializing culling face parameters");
}

/**
 * If enabled, the -wc <= zc <= wc plane equation is ignored by view volume clipping.
 * This effectively means that there is no near or far plane clipping.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glDepthRange.xml
 */
void Rendering::ReadDepthClampParameter()
{
	glDepthClampEnabled = GET_CONFIG_VALUE("GL_DEPTH_CLAMP_ENABLED", false);
	glDepthClampEnabledOld = glDepthClampEnabled;
	if (!glDepthClampEnabled)
	{
		glDisable(GL_DEPTH_CLAMP);
		DEBUG_LOG("GL_DEPTH_CLAMP disabled");
		return;
	}

	glEnable(GL_DEPTH_CLAMP); // prevents the camera to clip through the mesh
	INFO_LOG("GL_DEPTH_CLAMP enabled");
}

/**
 * If enabled, do depth comparisons and update the depth buffer. Note that even if the depth buffer
 * exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glDepthFunc.xml
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glDepthRange.xml
 */
void Rendering::ReadDepthTestParameter()
{
	glDepthTestEnabled = GET_CONFIG_VALUE("GL_DEPTH_TEST_ENABLED", false);
	glDepthTestEnabledOld = glDepthTestEnabled;
	if (!glDepthTestEnabled)
	{
		glDisable(GL_DEPTH_TEST);
		DEBUG_LOG("GL_DEPTH_TEST disabled");
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
	}
	

	std::string depthTestFuncStr = GET_CONFIG_VALUE_STR("GL_DEPTH_TEST_FUNC", "GL_LESS");
	if (depthTestFuncStr == "GL_NEVER") { glDepthTestFunc = GL_NEVER; }
	else if (depthTestFuncStr == "GL_LESS") { glDepthTestFunc = GL_LESS; } // Accept fragment if it closer to the camera than the former one
	else if (depthTestFuncStr == "GL_EQUAL") { glDepthTestFunc = GL_EQUAL; }
	else if (depthTestFuncStr == "GL_LEQUAL") { glDepthTestFunc = GL_LEQUAL; }
	else if (depthTestFuncStr == "GL_GREATER") { glDepthTestFunc = GL_GREATER; }
	else if (depthTestFuncStr == "GL_NOTEQUAL") { glDepthTestFunc = GL_NOTEQUAL; }
	else if (depthTestFuncStr == "GL_GEQUAL") { glDepthTestFunc = GL_GEQUAL; }
	else if (depthTestFuncStr == "GL_ALWAYS ") { glDepthTestFunc = GL_ALWAYS; }
	else /* GL_LESS is default */
	{
		ERROR_LOG("Invalid enum \"%s\" given for the depth test parameter. Using default GL_LESS", depthTestFuncStr.c_str());
		depthTestFuncStr = "GL_LESS";
		glDepthTestFunc = GL_LESS;
	}

	glDepthTestFuncOld = glDepthTestFunc;
	glDepthFunc(glDepthTestFunc);


	glDepthRangeNearValue = GET_CONFIG_VALUE("GL_DEPTH_RANGE_NEAR", REAL_ZERO);
	glDepthRangeFarValue = GET_CONFIG_VALUE("GL_DEPTH_RANGE_FAR", REAL_ONE);
	glDepthRangeNearValueOld = glDepthRangeNearValue;
	glDepthRangeFarValueOld = glDepthRangeFarValue;
	glDepthRange(glDepthRangeNearValue, glDepthRangeFarValue);
	INFO_LOG("GL_DEPTH_TEST enabled with function \"%s\" and the range [%.2f; %.2f]", depthTestFuncStr.c_str(), glDepthRangeNearValue, glDepthRangeFarValue);
	CheckErrorCode(__FUNCTION__, "Initializing depth clamping parameters");
}

/**
 * If enabled, dither color components or indices before they are written to the color buffer.
 */
void Rendering::ReadDitherParameter()
{
	glDitheringEnabled = GET_CONFIG_VALUE("GL_DITHER_ENABLED", false);
	glDitheringEnabledOld = glDitheringEnabled;
	if (!glDitheringEnabled)
	{
		glDisable(GL_DITHER);
		DEBUG_LOG("GL_DITHER disabled");
		return;
	}

	glEnable(GL_DITHER);
	INFO_LOG("GL_DITHER enabled");
	CheckErrorCode(__FUNCTION__, "Initializing dithering parameters");
}

/**
 * Defines front- and back-facing polygons
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glFrontFace.xml
 */
void Rendering::ReadFrontFaceParameter()
{
	std::string frontFaceStr = GET_CONFIG_VALUE_STR("GL_FRONT_FACE", "GL_CW");
	if (frontFaceStr == "GL_CW") { glFrontFaceMode = GL_CW; } // every face I draw in clockwise order is a front face
	else if (frontFaceStr == "GL_CCW") { glFrontFaceMode = GL_CCW; } // every face I draw in counter-clockwise order is a front face
	else /* GL_CCW is default */
	{
		ERROR_LOG("Invalid enum \"%s\" given for the front face mode. Using default GL_CCW", frontFaceStr.c_str());
		frontFaceStr = "GL_CCW";
		glFrontFaceMode = GL_CCW;
	}

	glFrontFaceModeOld = glFrontFaceMode;
	glFrontFace(glFrontFaceMode);
	INFO_LOG("\"%s\" mode specified for the glFrontFace", frontFaceStr.c_str());
	CheckErrorCode(__FUNCTION__, "Initializing front face parameters");
}

/**
 * If enabled, histogram incoming RGBA color values.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glHistogram.xml
 */
void Rendering::ReadHistogramParameter()
{
	if (supportedOpenGLLevel > 300)
	{
		INFO_LOG("Histogram not supported in OpenGL version higher than 3.0");
		CheckErrorCode(__FUNCTION__, "Initializing histogram parameters");
		return;
	}
	int histogramEnabled = GET_CONFIG_VALUE("GL_HISTOGRAM_ENABLED", 0);
	if (histogramEnabled == 0)
	{
		glDisable(GL_HISTOGRAM);
		DEBUG_LOG("GL_HISTOGRAM disabled");
		CheckErrorCode(__FUNCTION__, "Initializing histogram parameters");
		return;
	}

	glEnable(GL_HISTOGRAM);

	std::string histogramTargetStr = GET_CONFIG_VALUE_STR("GL_HISTOGRAM_TARGET", "GL_HISTOGRAM");
	GLenum histogramTarget;
	if (histogramTargetStr == "GL_HISTOGRAM") { histogramTarget = GL_HISTOGRAM; }
	else if (histogramTargetStr == "GL_PROXY_HISTOGRAM") { histogramTarget = GL_PROXY_HISTOGRAM; }
	else /* GL_HISTOGRAM is default */
	{
		ERROR_LOG("Invalid enum \"%s\" given for the histogram target parameter. Using default GL_HISTOGRAM", histogramTargetStr.c_str());
		histogramTargetStr = "GL_HISTOGRAM";
		histogramTarget = GL_HISTOGRAM;
	}

	int histogramWidth = GET_CONFIG_VALUE("GL_HISTOGRAM_WIDTH", 16); // must be a power of 2

	std::string histogramInternalFormatStr = GET_CONFIG_VALUE_STR("GL_HISTOGRAM_INTERNAL_FORMAT", "GL_ALPHA");
	GLenum histogramInternalFormat;
	if (histogramInternalFormatStr == "GL_ALPHA") { histogramInternalFormat = GL_ALPHA; }
	else if (histogramInternalFormatStr == "GL_ALPHA4") { histogramInternalFormat = GL_ALPHA4; }
	else if (histogramInternalFormatStr == "GL_ALPHA8") { histogramInternalFormat = GL_ALPHA8; }
	else if (histogramInternalFormatStr == "GL_ALPHA12") { histogramInternalFormat = GL_ALPHA12; }
	else if (histogramInternalFormatStr == "GL_ALPHA16") { histogramInternalFormat = GL_ALPHA16; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE") { histogramInternalFormat = GL_LUMINANCE; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE4") { histogramInternalFormat = GL_LUMINANCE4; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE8") { histogramInternalFormat = GL_LUMINANCE8; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE12") { histogramInternalFormat = GL_LUMINANCE12; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE16") { histogramInternalFormat = GL_LUMINANCE16; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE_ALPHA") { histogramInternalFormat = GL_LUMINANCE_ALPHA; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE4_ALPHA4") { histogramInternalFormat = GL_LUMINANCE4_ALPHA4; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE6_ALPHA2") { histogramInternalFormat = GL_LUMINANCE6_ALPHA2; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE8_ALPHA8") { histogramInternalFormat = GL_LUMINANCE8_ALPHA8; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE12_ALPHA4") { histogramInternalFormat = GL_LUMINANCE12_ALPHA4; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE12_ALPHA12") { histogramInternalFormat = GL_LUMINANCE12_ALPHA12; }
	else if (histogramInternalFormatStr == "GL_LUMINANCE16_ALPHA16") { histogramInternalFormat = GL_LUMINANCE16_ALPHA16; }
	else if (histogramInternalFormatStr == "GL_R3_G3_B2") { histogramInternalFormat = GL_R3_G3_B2; }
	else if (histogramInternalFormatStr == "GL_RGB") { histogramInternalFormat = GL_RGB; }
	else if (histogramInternalFormatStr == "GL_RGB4") { histogramInternalFormat = GL_RGB4; }
	else if (histogramInternalFormatStr == "GL_RGB5") { histogramInternalFormat = GL_RGB5; }
	else if (histogramInternalFormatStr == "GL_RGB8") { histogramInternalFormat = GL_RGB8; }
	else if (histogramInternalFormatStr == "GL_RGB10") { histogramInternalFormat = GL_RGB10; }
	else if (histogramInternalFormatStr == "GL_RGB12") { histogramInternalFormat = GL_RGB12; }
	else if (histogramInternalFormatStr == "GL_RGB16") { histogramInternalFormat = GL_RGB16; }
	else if (histogramInternalFormatStr == "GL_RGBA") { histogramInternalFormat = GL_RGBA; }
	else if (histogramInternalFormatStr == "GL_RGBA2") { histogramInternalFormat = GL_RGBA2; }
	else if (histogramInternalFormatStr == "GL_RGBA4") { histogramInternalFormat = GL_RGBA4; }
	else if (histogramInternalFormatStr == "GL_RGB5_A1") { histogramInternalFormat = GL_RGB5_A1; }
	else if (histogramInternalFormatStr == "GL_RGBA8") { histogramInternalFormat = GL_RGBA8; }
	else if (histogramInternalFormatStr == "GL_RGB10_A2") { histogramInternalFormat = GL_RGB10_A2; }
	else if (histogramInternalFormatStr == "GL_RGBA12") { histogramInternalFormat = GL_RGBA12; }
	else if (histogramInternalFormatStr == "GL_RGBA16") { histogramInternalFormat = GL_RGBA16; }
	else /* GL_RGBA is default */
	{
		ERROR_LOG("Invalid enum \"%s\" given for the histogram internal format. Using default GL_RGBA", histogramInternalFormatStr.c_str());
		histogramInternalFormatStr = "GL_RGBA";
		histogramInternalFormat = GL_RGBA;
	}

	bool histogramSink = GET_CONFIG_VALUE("GL_HISTOGRAM_SINK", false);
	glHistogram(histogramTarget, histogramWidth, histogramInternalFormat, histogramSink);

	if (histogramSink)
	{
		INFO_LOG("GL_HISTOGRAM enabled with target = \"%s\", width = %d, internal format = \"%s\" and enabled sink",
			histogramTargetStr.c_str(), histogramWidth, histogramInternalFormatStr.c_str());
	}
	else
	{
		INFO_LOG("GL_HISTOGRAM enabled with target = \"%s\", width = %d, internal format = \"%s\" and disabled sink",
			histogramTargetStr.c_str(), histogramWidth, histogramInternalFormatStr.c_str());
	}
	CheckErrorCode(__FUNCTION__, "Initializing histogram parameters");
}

/**
 * If enabled, compute the minimum and maximum values of incoming RGBA color values.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glMinmax.xml
 */
//void Rendering::ReadMinmaxParameter()
//{
//	// TODO: Not yet implemented
//}

/**
 * If enabled, use multiple fragment samples in computing the final color of a pixel.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glSampleCoverage.xml
 */
//void Rendering::ReadMultisampleParameter()
//{
//	// TODO: Not yet implemented
//}

/**
 * If enabled, discard fragments that are outside the scissor rectangle.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glScissor.xml
 */
void Rendering::ReadScissorTestParameter(int width, int height)
{
	glScissorTestEnabled = GET_CONFIG_VALUE("GL_SCISSOR_TEST_ENABLED", false);
	glScissorTestEnabledOld = glScissorTestEnabled;
	if (glScissorTestEnabled)
	{
		glEnable(GL_SCISSOR_TEST);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
		DEBUG_LOG("GL_SCISSOR_TEST disabled");
	}
	
	glScissorBoxLowerLeftCornerX = GET_CONFIG_VALUE("GL_SCISSOR_BOX_LOWER_LEFT_CORNER_X", 0);
	glScissorBoxLowerLeftCornerY = GET_CONFIG_VALUE("GL_SCISSOR_BOX_LOWER_LEFT_CORNER_Y", 0);
	glScissorBoxWidth = GET_CONFIG_VALUE("GL_SCISSOR_BOX_WIDTH", width);
	glScissorBoxHeight = GET_CONFIG_VALUE("GL_SCISSOR_BOX_HEIGHT", height);
	
	glScissorBoxLowerLeftCornerXOld = glScissorBoxLowerLeftCornerX;
	glScissorBoxLowerLeftCornerYOld = glScissorBoxLowerLeftCornerY;
	glScissorBoxWidthOld = glScissorBoxWidth;
	glScissorBoxHeightOld = glScissorBoxHeight;
	if (glScissorTestEnabled)
	{
		//glScissor(glScissorBoxLowerLeftCornerX, glScissorBoxLowerLeftCornerY, glScissorBoxWidth, glScissorBoxHeight);
		INFO_LOG("GL_SCISSOR_TEST enabled with lower left corner position = (%d; %d), width = %d and height = %d",
			glScissorBoxLowerLeftCornerX, glScissorBoxLowerLeftCornerY, glScissorBoxWidth, glScissorBoxHeight);
	}
	CheckErrorCode(__FUNCTION__, "Initializing scissor test parameters");
}

/**
 * If enabled, do stencil testing and update the stencil buffer.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glStencilFunc.xml
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glStencilFuncSeparate.xml
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glStencilOp.xml
 */
void Rendering::ReadStencilTestParameter()
{
	int stencilTestEnabled = GET_CONFIG_VALUE("GL_STENCIL_TEST_ENABLED", 0);
	if (stencilTestEnabled == 0)
	{
		glDisable(GL_STENCIL_TEST);
		DEBUG_LOG("GL_STENCIL_TEST_ENABLED disabled");
		return;
	}

	glEnable(GL_STENCIL_TEST);

	std::string stencilTestFuncStr = GET_CONFIG_VALUE_STR("GL_STENCIL_TEST_FUNC", "GL_ALWAYS");
	GLenum stencilTestFunc;
	if (stencilTestFuncStr == "GL_NEVER") { stencilTestFunc = GL_NEVER; }
	else if (stencilTestFuncStr == "GL_LESS") { stencilTestFunc = GL_LESS; }
	else if (stencilTestFuncStr == "GL_LEQUAL") { stencilTestFunc = GL_LEQUAL; }
	else if (stencilTestFuncStr == "GL_GREATER") { stencilTestFunc = GL_GREATER; }
	else if (stencilTestFuncStr == "GL_GEQUAL") { stencilTestFunc = GL_GEQUAL; }
	else if (stencilTestFuncStr == "GL_EQUAL") { stencilTestFunc = GL_EQUAL; }
	else if (stencilTestFuncStr == "GL_NOTEQUAL") { stencilTestFunc = GL_NOTEQUAL; }
	else if (stencilTestFuncStr == "GL_ALWAYS") { stencilTestFunc = GL_ALWAYS; }
	else /* GL_ALWAYS is default */
	{
		ERROR_LOG("Invalid enum \"%s\" given for the stencil test function. Using default GL_ALWAYS", stencilTestFuncStr.c_str());
		stencilTestFuncStr = "GL_ALWAYS";
		stencilTestFunc = GL_ALWAYS;
	}

	GLint stencilTestRefValue = GET_CONFIG_VALUE("GL_STENCIL_TEST_REF", 0);
	GLuint stencilTestMask = GET_CONFIG_VALUE("GL_STENCIL_TEST_MASK", 1);

	glStencilFunc(stencilTestFunc, stencilTestRefValue, stencilTestMask);
	INFO_LOG("GL_STENCIL_TEST enabled with function \"%s\", reference value = %d and the mask = %d",
		stencilTestFuncStr.c_str(), stencilTestRefValue, stencilTestMask);
	CheckErrorCode(__FUNCTION__, "Initializing stencil test parameters");
}

/**
 * Reads different kinds of OpenGL capabilities regarding textures.
 */
void Rendering::ReadTextureParameters()
{
	// TODO: Not yet implemented
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_TEXTURE_CUBE_MAP_EXT);
	glEnable(GL_TEXTURE_3D);
	CheckErrorCode(__FUNCTION__, "Initializing texture parameters");
}

/**
 * See the link: https://www.opengl.org/wiki/GLAPI/glGetError
 */
void Rendering::CheckErrorCode(const char* functionName, const char* comment)
{
	GLenum errCode = glGetError();
	if (errCode == GL_NO_ERROR)
	{
		return;
	}
	switch (errCode)
	{
	case GL_INVALID_ENUM:
		ERROR_LOG("Error occured in function \"%s\". %s failed with error code = GL_INVALID_ENUM", functionName, comment);
		break;
	case GL_INVALID_VALUE:
		ERROR_LOG("Error occured in function \"%s\". %s failed with error code = GL_INVALID_VALUE", functionName, comment);
		break;
	case GL_INVALID_OPERATION:
		ERROR_LOG("Error occured in function \"%s\". %s failed with error code = GL_INVALID_OPERATION", functionName, comment);
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		ERROR_LOG("Error occured in function \"%s\". %s failed with error code = GL_INVALID_FRAMEBUFFER_OPERATION", functionName, comment);
		break;
	case GL_OUT_OF_MEMORY:
		CRITICAL_LOG("Error occured in function \"%s\". %s failed with error code = GL_OUT_OF_MEMORY", functionName, comment);
		exit(EXIT_FAILURE);
		break;
	case GL_STACK_UNDERFLOW:
		CRITICAL_LOG("Error occured in function \"%s\". %s failed with error code = GL_STACK_UNDERFLOW", functionName, comment);
		exit(EXIT_FAILURE);
		break;
	case GL_STACK_OVERFLOW:
		CRITICAL_LOG("Error occured in function \"%s\". %s failed with error code = GL_STACK_OVERFLOW", functionName, comment);
		exit(EXIT_FAILURE);
		break;
	default:
		CRITICAL_LOG("Error occured in function \"%s\". %s failed with error code = %d", functionName, comment, errCode);
		exit(EXIT_FAILURE);
	}
}

/**
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glCheckFramebufferStatus.xml
 */
void Rendering::CheckFramebufferStatus()
{
	GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (framebufferStatus)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		DEBUG_LOG("Framebuffer is in status GL_FRAMEBUFFER_COMPLETE");
		break;
	case GL_FRAMEBUFFER_UNDEFINED:
		ERROR_LOG("Framebuffer is in status GL_FRAMEBUFFER_UNDEFINED");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		ERROR_LOG("Framebuffer is in status GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		ERROR_LOG("Framebuffer is in status GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		ERROR_LOG("Framebuffer is in status GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		ERROR_LOG("Framebuffer is in status GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		ERROR_LOG("Framebuffer is in status GL_FRAMEBUFFER_UNSUPPORTED");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		ERROR_LOG("Framebuffer is in status GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		ERROR_LOG("Framebuffer is in status GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
		break;
	default:
		EMERGENCY_LOG("Framebuffer is in unknown status 0x%x", framebufferStatus);
		break;
	}
}

#ifdef ANT_TWEAK_BAR_ENABLED
void TW_CALL Rendering::TweakBarErrorHandler(const char* errorMessage)
{
	ERROR_LOG("TweakBar error: \"%s\"", errorMessage);
}

void Rendering::InitializeTweakBars()
{
	TwInit(TW_OPENGL_CORE, NULL); // Initializing AntTweakBar library
	TwHandleErrors(Rendering::TweakBarErrorHandler);

#ifdef OPEN_GL_PROPERTIES_TWEAK_BAR
	TwEnumVal blendSFactorEV[] = { { GL_ZERO, "GL_ZERO" }, { GL_ONE, "GL_ONE" }, { GL_SRC_COLOR, "GL_SRC_COLOR" }, { GL_ONE_MINUS_SRC_COLOR, "GL_ONE_MINUS_SRC_COLOR" },
		{ GL_DST_COLOR, "GL_DST_COLOR" }, { GL_ONE_MINUS_DST_COLOR, "GL_ONE_MINUS_DST_COLOR" }, { GL_SRC_ALPHA, "GL_SRC_ALPHA" }, { GL_ONE_MINUS_SRC_ALPHA, "GL_ONE_MINUS_SRC_ALPHA" },
		{ GL_DST_ALPHA, "GL_DST_ALPHA" }, { GL_ONE_MINUS_DST_ALPHA, "GL_ONE_MINUS_DST_ALPHA" }, { GL_CONSTANT_COLOR, "GL_CONSTANT_COLOR" }, { GL_ONE_MINUS_CONSTANT_COLOR, "GL_ONE_MINUS_CONSTANT_COLOR" },
		{ GL_CONSTANT_ALPHA, "GL_CONSTANT_ALPHA" }, { GL_ONE_MINUS_CONSTANT_ALPHA, "GL_ONE_MINUS_CONSTANT_ALPHA" }, { GL_SRC_ALPHA_SATURATE, "GL_SRC_ALPHA_SATURATE" },
		{ GL_SRC1_COLOR, "GL_SRC1_COLOR" }, { GL_ONE_MINUS_SRC1_COLOR, "GL_ONE_MINUS_SRC1_COLOR" }, { GL_SRC1_ALPHA, "GL_SRC1_ALPHA" }, { GL_ONE_MINUS_SRC1_ALPHA, "GL_ONE_MINUS_SRC1_ALPHA" }};
	TwType glBlendSFactorEnumType = TwDefineEnum("Blending S Factor", blendSFactorEV, 19);

	TwEnumVal blendDFactorEV[] = { { GL_ZERO, "GL_ZERO" }, { GL_ONE, "GL_ONE" }, { GL_SRC_COLOR, "GL_SRC_COLOR" }, { GL_ONE_MINUS_SRC_COLOR, "GL_ONE_MINUS_SRC_COLOR" },
		{ GL_DST_COLOR, "GL_DST_COLOR" }, { GL_ONE_MINUS_DST_COLOR, "GL_ONE_MINUS_DST_COLOR" }, { GL_SRC_ALPHA, "GL_SRC_ALPHA" }, { GL_ONE_MINUS_SRC_ALPHA, "GL_ONE_MINUS_SRC_ALPHA" },
		{ GL_DST_ALPHA, "GL_DST_ALPHA" }, { GL_ONE_MINUS_DST_ALPHA, "GL_ONE_MINUS_DST_ALPHA" }, { GL_CONSTANT_COLOR, "GL_CONSTANT_COLOR" }, { GL_ONE_MINUS_CONSTANT_COLOR, "GL_ONE_MINUS_CONSTANT_COLOR" },
		{ GL_CONSTANT_ALPHA, "GL_CONSTANT_ALPHA" }, { GL_ONE_MINUS_CONSTANT_ALPHA, "GL_ONE_MINUS_CONSTANT_ALPHA" } };
	TwType glBlendDFactorEnumType = TwDefineEnum("Blending D Factor", blendDFactorEV, 14);

	TwEnumVal colorLogicOperationCodeEV[] = { { GL_CLEAR, "GL_CLEAR" }, { GL_SET, "GL_SET" }, { GL_COPY, "GL_COPY" }, { GL_COPY_INVERTED, "GL_COPY_INVERTED" },
		{ GL_NOOP, "GL_NOOP" }, { GL_INVERT, "GL_INVERT" }, { GL_AND, "GL_AND" }, { GL_NAND, "GL_NAND" }, { GL_OR, "GL_OR" }, { GL_NOR, "GL_NOR" },
		{ GL_XOR, "GL_XOR" }, { GL_EQUIV, "GL_EQUIV" }, { GL_AND_REVERSE, "GL_AND_REVERSE" }, { GL_AND_INVERTED, "GL_AND_INVERTED" },
		{ GL_OR_REVERSE, "GL_OR_REVERSE" }, { GL_OR_INVERTED, "GL_OR_INVERTED" } };
	TwType glColorLogicOperationCodeEnumType = TwDefineEnum("Logic operation code", colorLogicOperationCodeEV, 16);

	TwEnumVal cullFaceModeEV[] = { { GL_FRONT, "GL_FRONT" }, { GL_BACK, "GL_BACK" }, { GL_FRONT_AND_BACK, "GL_FRONT_AND_BACK" } };
	TwType glCullFaceModeEnumType = TwDefineEnum("Cull face mode", cullFaceModeEV, 3);

	TwEnumVal depthTestFuncEV[] = { { GL_NEVER, "GL_NEVER" }, { GL_LESS, "GL_LESS" }, { GL_EQUAL, "GL_EQUAL" }, { GL_LEQUAL, "GL_LEQUAL" },
		{ GL_GREATER, "GL_GREATER" }, { GL_NOTEQUAL, "GL_NOTEQUAL" }, { GL_GEQUAL, "GL_GEQUAL" }, { GL_ALWAYS, "GL_ALWAYS" } };
	TwType glDepthTestFuncEnumType = TwDefineEnum("Depth test function", depthTestFuncEV, 8);

	TwEnumVal frontFaceEV[] = { { GL_CW, "GL_CW" }, { GL_CCW, "GL_CCW" } };
	TwType glFrontFaceEnumType = TwDefineEnum("Front face", frontFaceEV, 2);


	TwBar* glPropertiesBar = TwNewBar("OpenGLPropertiesBar");
	TwAddVarRW(glPropertiesBar, "blendEnabled", TW_TYPE_BOOLCPP, &glBlendEnabled, " label='Enabled' group='Blending' ");
	TwAddVarRW(glPropertiesBar, "blendSFactor", glBlendSFactorEnumType, &glBlendSfactor, " label='S factor' group='Blending' ");
	TwAddVarRW(glPropertiesBar, "blendDFactor", glBlendDFactorEnumType, &glBlendDfactor, " label='D factor' group='Blending' ");

	TwAddVarRW(glPropertiesBar, "logicOperationEnabled", TW_TYPE_BOOLCPP, &glColorLogicOperationEnabled, " label='Enabled' group='Color logic operation' ");
	TwAddVarRW(glPropertiesBar, "logicOperationCode", glColorLogicOperationCodeEnumType, &glColorLogicOperationCode, " label='Code' group='Color logic operation' ");

	TwAddVarRW(glPropertiesBar, "cullFaceEnabled", TW_TYPE_BOOLCPP, &glCullFaceEnabled, " label='Enabled' group='Face culling' ");
	TwAddVarRW(glPropertiesBar, "cullFaceMode", glCullFaceModeEnumType, &glCullFaceMode, " label='Mode' group='Face culling' ");

	TwAddVarRW(glPropertiesBar, "depthClampingEnabled", TW_TYPE_BOOLCPP, &glDepthClampEnabled, " label='Enabled' group='Depth clamp' ");

	TwAddVarRW(glPropertiesBar, "depthTestEnabled", TW_TYPE_BOOLCPP, &glDepthTestEnabled, " label='Enabled' group='Depth test' ");
	TwAddVarRW(glPropertiesBar, "depthTestFunction", glDepthTestFuncEnumType, &glDepthTestFunc, " label='Function' group='Depth test' ");
	TwAddVarRW(glPropertiesBar, "depthRangeNearValue", TW_TYPE_REAL, &glDepthRangeNearValue, " label='Range near value' group='Depth test' step=0.01 min=0.0 max=1.0 ");
	TwAddVarRW(glPropertiesBar, "depthRangeFarValue", TW_TYPE_REAL, &glDepthRangeFarValue, " label='Range far value' group='Depth test' step=0.01 min=0.0 max=1.0 ");

	TwAddVarRW(glPropertiesBar, "ditheringEnabled", TW_TYPE_BOOLCPP, &glDitheringEnabled, " label='Enabled' group='Dithering' ");

	TwAddVarRW(glPropertiesBar, "frontFace", glFrontFaceEnumType, &glFrontFaceMode, " label='Front face' ");

	TwAddVarRW(glPropertiesBar, "scissorTestEnabled", TW_TYPE_BOOLCPP, &glScissorTestEnabled, " label='Enabled' group='Scissor test' ");
	TwAddVarRW(glPropertiesBar, "scissorTestLeftCornerX", TW_TYPE_INT32, &glScissorBoxLowerLeftCornerX, " label='Left corner X' group='Scissor test' ");
	TwAddVarRW(glPropertiesBar, "scissorTestLeftCornerY", TW_TYPE_INT32, &glScissorBoxLowerLeftCornerY, " label='Left corner Y' group='Scissor test' ");
	TwAddVarRW(glPropertiesBar, "scissorTestBoxWidth", TW_TYPE_INT32, &glScissorBoxWidth, " label='Box width' group='Scissor test' min=50");
	TwAddVarRW(glPropertiesBar, "scissorTestBoxHeight", TW_TYPE_INT32, &glScissorBoxHeight, " label='Box height' group='Scissor test' min=50");

	TwSetParam(glPropertiesBar, NULL, "visible", TW_PARAM_CSTRING, 1, "false"); // Hide the bar at startup
#endif
}

void Rendering::CheckChangesAndUpdateGLState()
{
#ifdef OPEN_GL_PROPERTIES_TWEAK_BAR
	UpdateBlendParameters(); /* ==================== Checking blending parameters ==================== */
	UpdateColorLogicOperationParameters(); /* ==================== Checking color logic operation parameters ==================== */
	UpdateCullFaceParameters(); /* ==================== Checking face culling parameters ==================== */
	UpdateDepthClampParameters(); /* ==================== Depth clamp parameters ==================== */
	UpdateDepthTestParameters(); /* ==================== Depth test parameters ==================== */
	UpdateDitheringParameters(); /* ==================== Dithering parameters ==================== */
	UpdateFrontFaceParameters(); /* ==================== Front face parameters ==================== */


	UpdateScissorTestParameters(); /* ==================== Checking scissor test parameters ==================== */
#endif
}

void Rendering::UpdateBlendParameters()
{
	if ((glBlendEnabled == glBlendEnabledOld) && (glBlendSfactor == glBlendSfactorOld) && (glBlendDfactor == glBlendDfactorOld))
	{
		return;
	}

	const GLenum glPropertyEnum = GL_BLEND;
	if (glBlendEnabled)
	{
		glEnable(glPropertyEnum);
		NOTICE_LOG("GL_BLEND is now enabled");
	}
	else
	{
		glDisable(glPropertyEnum);
		NOTICE_LOG("GL_BLEND is now disabled");
	}
	glBlendEnabledOld = glBlendEnabled;

	// Now we check if any of blending factors have been changed.
	if ((glBlendSfactor != glBlendSfactorOld) || (glBlendDfactor != glBlendDfactorOld))
	{
		glBlendFunc(glBlendSfactor, glBlendDfactor);
		NOTICE_LOG("Blending is now performed with the following parameters: sFactor = %d, dFactor = %d", glBlendSfactor, glBlendDfactor);
		glBlendSfactorOld = glBlendSfactor;
		glBlendDfactorOld = glBlendDfactor;
	}
}

void Rendering::UpdateColorLogicOperationParameters()
{
	if ((glColorLogicOperationEnabled == glColorLogicOperationEnabledOld) && (glColorLogicOperationCode == glColorLogicOperationCodeOld))
	{
		return;
	}

	const GLenum glPropertyEnum = GL_COLOR_LOGIC_OP;
	if (glColorLogicOperationEnabled)
	{
		glEnable(glPropertyEnum);
		NOTICE_LOG("GL_COLOR_LOGIC_OP is now enabled");
	}
	else
	{
		glDisable(glPropertyEnum);
		NOTICE_LOG("GL_COLOR_LOGIC_OP is now disabled");
	}
	glColorLogicOperationEnabledOld = glColorLogicOperationEnabled;

	// Now we check if logic operation code has been changed
	if (glColorLogicOperationCode != glColorLogicOperationCodeOld)
	{
		glLogicOp(glColorLogicOperationCode);
		NOTICE_LOG("Color logic operation %d is chosen", glColorLogicOperationCode);
		glColorLogicOperationCodeOld = glColorLogicOperationCode;
	}
}

void Rendering::UpdateCullFaceParameters()
{
	if ((glCullFaceEnabled == glCullFaceEnabledOld) && (glCullFaceMode == glCullFaceModeOld))
	{
		return;
	}

	const GLenum glPropertyEnum = GL_CULL_FACE;
	if (glCullFaceEnabled)
	{
		glEnable(glPropertyEnum);
		CheckErrorCode("Rendering::UpdateCullFaceParameters", "Enabling GL_CULL_FACE");
		NOTICE_LOG("GL_CULL_FACE is now enabled");
	}
	else
	{
		glDisable(glPropertyEnum);
		CheckErrorCode("Rendering::UpdateCullFaceParameters", "Disabling GL_CULL_FACE");
		NOTICE_LOG("GL_CULL_FACE is now disabled");
	}
	glCullFaceEnabledOld = glCullFaceEnabled;

	// Now we must check if cull face mode has been changed
	if (glCullFaceMode != glCullFaceModeOld)
	{
		glCullFace(glCullFaceMode);
		CheckErrorCode("Rendering::UpdateCullFaceParameters", "Setting the cull face mode");
		NOTICE_LOG("Face culling mode is %d", glCullFaceMode);
		glCullFaceModeOld = glCullFaceMode;
	}

}

void Rendering::UpdateDepthClampParameters()
{
	if (glDepthClampEnabled == glDepthClampEnabledOld)
	{
		return;
	}

	const GLenum glPropertyEnum = GL_DEPTH_CLAMP;
	glDepthClampEnabledOld = glDepthClampEnabled;
	if (glDepthClampEnabled)
	{
		glEnable(glPropertyEnum);
		CheckErrorCode("Rendering::UpdateDepthClampParameters", "Enabling GL_DEPTH_CLAMP");
		NOTICE_LOG("GL_DEPTH_CLAMP is now enabled");
	}
	else
	{
		glDisable(glPropertyEnum);
		CheckErrorCode("Rendering::UpdateDepthClampParameters", "Disabling GL_DEPTH_CLAMP");
		NOTICE_LOG("GL_DEPTH_CLAMP is now disabled");
	}
}

void Rendering::UpdateDepthTestParameters()
{
	if (glDepthTestEnabled != glDepthTestEnabledOld)
	{
		if (glDepthTestEnabled)
		{
			glEnable(GL_DEPTH_TEST);
			CheckErrorCode("Rendering::UpdateDepthTestParameters", "Enabling GL_DEPTH_TEST");
			NOTICE_LOG("GL_DEPTH_TEST is now enabled");
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			CheckErrorCode("Rendering::UpdateDepthTestParameters", "Disabling GL_DEPTH_TEST");
			NOTICE_LOG("GL_DEPTH_TEST is now disabled");
		}
		glDepthTestEnabledOld = glDepthTestEnabled;
	}

	if (glDepthTestFunc != glDepthTestFuncOld)
	{
		glDepthFunc(glDepthTestFunc);
		CheckErrorCode("Rendering::UpdateDepthTestParameters", "Depth function change");
		NOTICE_LOG("Depth function changed from %d to %d", glDepthTestFuncOld, glDepthTestFunc);
		glDepthTestFuncOld = glDepthTestFunc;
	}

	//TODO: if (glDepthRangeNearValue
}

void Rendering::UpdateDitheringParameters()
{
	if (glDitheringEnabled == glDitheringEnabledOld)
	{
		return;
	}

	glDitheringEnabledOld = glDitheringEnabled;
	const GLenum glPropertyEnum = GL_DITHER;
	if (glDitheringEnabled)
	{
		glEnable(glPropertyEnum);
		NOTICE_LOG("GL_DITHER is now enabled");
	}
	else
	{
		glDisable(glPropertyEnum);
		NOTICE_LOG("GL_DITHER is now disabled");
	}
}

void Rendering::UpdateFrontFaceParameters()
{
	if (glFrontFaceMode == glFrontFaceModeOld)
	{
		return;
	}

	glFrontFaceModeOld = glFrontFaceMode;
	glFrontFace(glFrontFaceMode);
	CheckErrorCode("Rendering::UpdateFrontFaceParameters", "Change front face parameter");
	NOTICE_LOG("%d value has been chosen for the front face parameter", glFrontFaceMode);
}

void Rendering::UpdateScissorTestParameters()
{
	if (glScissorTestEnabled != glScissorTestEnabledOld)
	{
		const GLenum glPropertyEnum = GL_SCISSOR_TEST;
		if (glScissorTestEnabled)
		{
			glEnable(glPropertyEnum);
			NOTICE_LOG("GL_SCISSOR_TEST is now enabled");
		}
		else
		{
			glDisable(glPropertyEnum);
			NOTICE_LOG("GL_SCISSOR_TEST is now disabled");
		}
		glScissorTestEnabledOld = glScissorTestEnabled;
	}

	// Now we check if any of scissor test parameters have been changed.
	if ((glScissorBoxLowerLeftCornerX != glScissorBoxLowerLeftCornerXOld) || (glScissorBoxLowerLeftCornerY != glScissorBoxLowerLeftCornerYOld) ||
		(glScissorBoxWidth != glScissorBoxWidthOld) || (glScissorBoxHeight != glScissorBoxHeightOld))
	{
		// TODO: Clear whole screen
		//glClearColor(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE);
		glScissor(glScissorBoxLowerLeftCornerX, glScissorBoxLowerLeftCornerY, glScissorBoxWidth, glScissorBoxHeight);
		NOTICE_LOG("Scissor test is now performed with the following parameters: left corner position = (%d; %d), box width = %d, box height = %d",
			glScissorBoxLowerLeftCornerX, glScissorBoxLowerLeftCornerY, glScissorBoxWidth, glScissorBoxHeight);

		glScissorBoxLowerLeftCornerXOld = glScissorBoxLowerLeftCornerX;
		glScissorBoxLowerLeftCornerYOld = glScissorBoxLowerLeftCornerY;
		glScissorBoxWidthOld = glScissorBoxWidth;
		glScissorBoxHeightOld = glScissorBoxHeight;
	}
}

#endif