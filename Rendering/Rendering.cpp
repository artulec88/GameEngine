#include "stdafx.h"
#include "Rendering.h"
#include "AntTweakBarTypes.h"

#include "Utility/ILogger.h"
#include "Utility/IConfig.h"

int rendering::supportedOpenGLLevel;
std::string rendering::glslVersion;

/* ==================== Blending parameters begin ==================== */
bool rendering::glBlendEnabled, rendering::glBlendEnabledOld;
GLenum rendering::glBlendSfactor, rendering::glBlendSfactorOld;
GLenum rendering::glBlendDfactor, rendering::glBlendDfactorOld;
/* ==================== Blending parameters end ==================== */

/* ==================== Color logic operation parameters begin ==================== */
bool rendering::glColorLogicOperationEnabled, rendering::glColorLogicOperationEnabledOld;
GLenum rendering::glColorLogicOperationCode, rendering::glColorLogicOperationCodeOld;
/* ==================== Color logic operation parameters end ==================== */

/* ==================== Color logic operation parameters begin ==================== */
bool rendering::glCullFaceEnabled, rendering::glCullFaceEnabledOld;
GLenum rendering::glCullFaceMode, rendering::glCullFaceModeOld;
/* ==================== Color logic operation parameters end ==================== */

/* ==================== Depth clamping parameters begin ==================== */
bool rendering::glDepthClampEnabled, rendering::glDepthClampEnabledOld;
/* ==================== Depth clamping parameters end ==================== */

/* ==================== Depth test parameters begin ==================== */
bool rendering::glDepthTestEnabled, rendering::glDepthTestEnabledOld;
GLenum rendering::glDepthTestFunc, rendering::glDepthTestFuncOld;
math::Real rendering::glDepthRangeNearValue, rendering::glDepthRangeNearValueOld;
math::Real rendering::glDepthRangeFarValue, rendering::glDepthRangeFarValueOld;
/* ==================== Depth test parameters end ==================== */

/* ==================== Dithering parameters begin ==================== */
bool rendering::glDitheringEnabled, rendering::glDitheringEnabledOld;
/* ==================== Dithering parameters end ==================== */

/* ==================== Front face parameters begin ==================== */
GLenum rendering::glFrontFaceMode, rendering::glFrontFaceModeOld;
/* ==================== Front face parameters end ==================== */

/* ==================== Scissor test parameters begin ==================== */
bool rendering::glScissorTestEnabled, rendering::glScissorTestEnabledOld;
GLint rendering::glScissorBoxLowerLeftCornerX, rendering::glScissorBoxLowerLeftCornerXOld;
GLint rendering::glScissorBoxLowerLeftCornerY, rendering::glScissorBoxLowerLeftCornerYOld;
GLsizei rendering::glScissorBoxWidth, rendering::glScissorBoxWidthOld;
GLsizei rendering::glScissorBoxHeight, rendering::glScissorBoxHeightOld;
/* ==================== Scissor test parameters end ==================== */

//bool Rendering::FogEffect::FogKey::operator<(const Rendering::FogEffect::FogKey& fog) const
//{
//	if (fallOffType != fog.fallOffType)
//	{
//		return fallOffType < fog.fallOffType;
//	}
//	else
//	{
//		return calculationType < fog.calculationType;
//	}
//	//return (fallOffType & calculationType) < (fog.fallOffType & fog.calculationType);
//}

void rendering::DetermineGlVersion()
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
		CRITICAL_LOG_RENDERING("GLSL Version ", majorVersion, ".", minorVersion, " does not support shaders.");
		exit(EXIT_FAILURE);
	}
}

void rendering::PrintGlReport()
{
	INFO_LOG_RENDERING("OpenGL report:\n\tVendor:\t\t\t\"", glGetString(GL_VENDOR), "\"\n\tRenderer name:\t\t\"", glGetString(GL_RENDERER),
		"\"\n\tVersion:\t\t\"", glGetString(GL_VERSION), "\"\n\tGLSL version:\t\t\"", glGetString(GL_SHADING_LANGUAGE_VERSION), "\"");
	//INFO_LOG_RENDERING("OpenGL extensions: ", glGetString(GL_EXTENSIONS));
}

void rendering::InitGraphics(int width, int height, aliasing::AntiAliasingMethod antiAliasingMethod)
{
	NOTICE_LOG_RENDERING("Initializing graphics started");

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

	switch (antiAliasingMethod)
	{
	case aliasing::NONE:
		glDisable(GL_MULTISAMPLE); // disable multisampling
		break;
	case aliasing::FXAA:
		glDisable(GL_MULTISAMPLE); // disable multisampling
		break;
	case aliasing::MSAA:
		glEnable(GL_MULTISAMPLE);
		break;
	default:
		WARNING_LOG_RENDERING("Unknown anti-aliasing algorithm chosen");
		glEnable(GL_MULTISAMPLE);
	}

	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP);
	//glEnable(GL_FRAMEBUFFER_SRGB); // Essentialy gives free gamma correction for better contrast. TODO: Test it!
	glEnable(GL_CLIP_DISTANCE0); // Enabled plane clipping // glEnable(GL_CLIP_PLANE0)

	// TODO: Use GL_LINE instead of GL_FILL to make the scene rendered in wireframe. Make it an option in the AntTweakBar to switch between normal and wireframe modes.
	//glEnable(GL_POLYGON_OFFSET_LINE);
	//glPolygonOffset(-1, -1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	NOTICE_LOG_RENDERING("Initializing graphics finished successfully");
}

/**
 * If enabled, do alpha testing. Deprecated in OpenGL 3.*
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glAlphaFunc.xml
 */
//void Rendering::ReadAlphaTestParameter()
//{
//	int alphaTestEnabled = GET_CONFIG_VALUE_RENDERING("GL_ALPHA_TEST_ENABLED", 0);
//	if (alphaTestEnabled == 0)
//	{
//		glDisable(GL_ALPHA_TEST);
//		DEBUG_LOG_RENDERING("GL_ALPHA_TEST disabled");
//		return;
//	}
//
//	glEnable(GL_ALPHA_TEST); // Perform ALPHA testing
//
//	std::string alphaFuncStr = GET_CONFIG_VALUE_STR_RENDERING("GL_ALPHA_TEST_FUNC", "GL_LESS");
//	math::Real alphaFuncRefValue = GET_CONFIG_VALUE_RENDERING("GL_ALPHA_TEST_REF", 0.1f);
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
//		ERROR_LOG_RENDERING("Invalid enum \"", alphaFuncStr, "\" given for the alpha test function. Using default GL_ALWAYS");
//		alphaFuncStr = "GL_ALWAYS";
//		glAlphaFunc(GL_ALWAYS, alphaFuncRefValue);
//	}
//
//	INFO_LOG_RENDERING("GL_ALPHA_TEST enabled with function \"", alphaFuncStr, "\" and reference value = ", alphaFuncRefValue);
//}

/**
 * If enabled, blen the computed fragment color values with the values in the color buffers.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glBlendFunc.xml
 */
void rendering::ReadBlendParameter()
{
	glBlendEnabled = GET_CONFIG_VALUE_RENDERING("GL_BLEND_ENABLED", false);
	glBlendEnabledOld = glBlendEnabled;
	if (!glBlendEnabled)
	{
		glDisable(GL_BLEND);
		DEBUG_LOG_RENDERING("GL_BLEND disabled");
		return;
	}

	glEnable(GL_BLEND);
	std::string blendSFactorStr = GET_CONFIG_VALUE_STR_RENDERING("GL_BLEND_SFACTOR", "GL_ONE");
	std::string blendDFactorStr = GET_CONFIG_VALUE_STR_RENDERING("GL_BLEND_DFACTOR", "GL_ZERO");
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
		ERROR_LOG_RENDERING("Invalid enum \"", blendSFactorStr, "\" given for the blend sFactor. Using default GL_ONE");
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
		ERROR_LOG_RENDERING("Invalid enum \"", blendDFactorStr, "\" given for the blend dFactor. Using default GL_ZERO");
		blendDFactorStr = "GL_ZERO";
		glBlendDfactor = GL_ZERO;
	}

	glBlendSfactorOld = glBlendSfactor;
	glBlendDfactorOld = glBlendDfactor;
	glBlendFunc(glBlendSfactor, glBlendDfactor);
	INFO_LOG_RENDERING("GL_BLEND enabled with sFactor = \"", blendSFactorStr, "\" and dFactor = \"", blendDFactorStr, "\"");
	CheckErrorCode(__FUNCTION__, "Initializing blending parameters");
}

/**
 * If enabled, apply the currently selected logical operation to the computed fragment color and color buffer values.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glLogicOp.xml
 */
void rendering::ReadColorLogicOperationParameter()
{
	glColorLogicOperationEnabled = GET_CONFIG_VALUE_RENDERING("GL_COLOR_LOGIC_OP_ENABLED", false);
	glColorLogicOperationEnabledOld = glColorLogicOperationEnabled;
	if (!glColorLogicOperationEnabled)
	{
		glDisable(GL_COLOR_LOGIC_OP);
		DEBUG_LOG_RENDERING("GL_COLOR_LOGIC_OP disabled");
		return;
	}

	glEnable(GL_COLOR_LOGIC_OP);
	std::string logicalOperationStr = GET_CONFIG_VALUE_STR_RENDERING("GL_COLOR_LOGIC_OPERATION", "GL_COPY");
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
		ERROR_LOG_RENDERING("Invalid enum \"", logicalOperationStr, "\" given for the color logic operation parameter. Using default GL_COPY");
		logicalOperationStr = "GL_COPY";
		glColorLogicOperationCode = GL_COPY;
	}

	glColorLogicOperationCodeOld = glColorLogicOperationCode;

	glLogicOp(glColorLogicOperationCode);

	INFO_LOG_RENDERING("GL_COLOR_LOGIC_OP enabled in \"", logicalOperationStr, "\" mode");
	CheckErrorCode(__FUNCTION__, "Initializing color logic parameters");
}

/**
 * If enabled, cull polygons based on their winding in window coordinates.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glCullFace.xml
 */
void rendering::ReadCullFaceParameter()
{
	glCullFaceEnabled = GET_CONFIG_VALUE_RENDERING("GL_CULL_FACE_ENABLED", false);
	glCullFaceEnabledOld = glCullFaceEnabled;
	if (!glCullFaceEnabled)
	{
		glDisable(GL_CULL_FACE);
		DEBUG_LOG_RENDERING("GL_CULL_FACE disabled");
	}
	else
	{
		glEnable(GL_CULL_FACE); // culling faces enabled. Cull triangles which normal is not towards the camera
	}

	std::string cullFaceModeStr = GET_CONFIG_VALUE_STR_RENDERING("GL_CULL_FACE_MODE", "GL_BACK");
	if (cullFaceModeStr == "GL_FRONT") { glCullFaceMode = GL_FRONT; } // cull the front face
	else if (cullFaceModeStr == "GL_BACK") { glCullFaceMode = GL_BACK; } // cull the back face
	else if (cullFaceModeStr == "GL_FRONT_AND_BACK ") { glCullFaceMode = GL_FRONT_AND_BACK; } // cull both back and front faces (only lines, points are rendered)
	else /* GL_BACK is default */
	{
		ERROR_LOG_RENDERING("Invalid enum \"", cullFaceModeStr, "\" given for the face culling parameter. Using default GL_BACK");
		cullFaceModeStr = "GL_BACK";
		glCullFaceMode = GL_BACK;
	}

	glCullFaceModeOld = glCullFaceMode;

	glCullFace(glCullFaceMode);

	INFO_LOG_RENDERING("GL_CULL_FACE enabled in \"", cullFaceModeStr, "\" mode");
	CheckErrorCode(__FUNCTION__, "Initializing culling face parameters");
}

/**
 * If enabled, the -wc <= zc <= wc plane equation is ignored by view volume clipping.
 * This effectively means that there is no near or far plane clipping.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glDepthRange.xml
 */
void rendering::ReadDepthClampParameter()
{
	glDepthClampEnabled = GET_CONFIG_VALUE_RENDERING("GL_DEPTH_CLAMP_ENABLED", false);
	glDepthClampEnabledOld = glDepthClampEnabled;
	if (!glDepthClampEnabled)
	{
		glDisable(GL_DEPTH_CLAMP);
		DEBUG_LOG_RENDERING("GL_DEPTH_CLAMP disabled");
		return;
	}

	glEnable(GL_DEPTH_CLAMP); // prevents the camera to clip through the mesh
	INFO_LOG_RENDERING("GL_DEPTH_CLAMP enabled");
}

/**
 * If enabled, do depth comparisons and update the depth buffer. Note that even if the depth buffer
 * exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glDepthFunc.xml
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glDepthRange.xml
 */
void rendering::ReadDepthTestParameter()
{
	glDepthTestEnabled = GET_CONFIG_VALUE_RENDERING("GL_DEPTH_TEST_ENABLED", false);
	glDepthTestEnabledOld = glDepthTestEnabled;
	if (!glDepthTestEnabled)
	{
		glDisable(GL_DEPTH_TEST);
		DEBUG_LOG_RENDERING("GL_DEPTH_TEST disabled");
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
	}
	

	std::string depthTestFuncStr = GET_CONFIG_VALUE_STR_RENDERING("GL_DEPTH_TEST_FUNC", "GL_LESS");
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
		ERROR_LOG_RENDERING("Invalid enum \"", depthTestFuncStr, "\" given for the depth test parameter. Using default GL_LESS");
		depthTestFuncStr = "GL_LESS";
		glDepthTestFunc = GL_LESS;
	}

	glDepthTestFuncOld = glDepthTestFunc;
	glDepthFunc(glDepthTestFunc);


	glDepthRangeNearValue = GET_CONFIG_VALUE_RENDERING("GL_DEPTH_RANGE_NEAR", REAL_ZERO);
	glDepthRangeFarValue = GET_CONFIG_VALUE_RENDERING("GL_DEPTH_RANGE_FAR", REAL_ONE);
	glDepthRangeNearValueOld = glDepthRangeNearValue;
	glDepthRangeFarValueOld = glDepthRangeFarValue;
	glDepthRange(glDepthRangeNearValue, glDepthRangeFarValue);
	INFO_LOG_RENDERING("GL_DEPTH_TEST enabled with function \"", depthTestFuncStr, "\" and the range [", glDepthRangeNearValue, "; ", glDepthRangeFarValue, "]");
	CheckErrorCode(__FUNCTION__, "Initializing depth clamping parameters");
}

/**
 * If enabled, dither color components or indices before they are written to the color buffer.
 */
void rendering::ReadDitherParameter()
{
	glDitheringEnabled = GET_CONFIG_VALUE_RENDERING("GL_DITHER_ENABLED", false);
	glDitheringEnabledOld = glDitheringEnabled;
	if (!glDitheringEnabled)
	{
		glDisable(GL_DITHER);
		DEBUG_LOG_RENDERING("GL_DITHER disabled");
		return;
	}

	glEnable(GL_DITHER);
	INFO_LOG_RENDERING("GL_DITHER enabled");
	CheckErrorCode(__FUNCTION__, "Initializing dithering parameters");
}

/**
 * Defines front- and back-facing polygons
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glFrontFace.xml
 */
void rendering::ReadFrontFaceParameter()
{
	std::string frontFaceStr = GET_CONFIG_VALUE_STR_RENDERING("GL_FRONT_FACE", "GL_CW");
	if (frontFaceStr == "GL_CW") { glFrontFaceMode = GL_CW; } // every face I draw in clockwise order is a front face
	else if (frontFaceStr == "GL_CCW") { glFrontFaceMode = GL_CCW; } // every face I draw in counter-clockwise order is a front face
	else /* GL_CCW is default */
	{
		ERROR_LOG_RENDERING("Invalid enum \"", frontFaceStr, "\" given for the front face mode. Using default GL_CCW");
		frontFaceStr = "GL_CCW";
		glFrontFaceMode = GL_CCW;
	}

	glFrontFaceModeOld = glFrontFaceMode;
	glFrontFace(glFrontFaceMode);
	INFO_LOG_RENDERING("\"", frontFaceStr, "\" mode specified for the glFrontFace");
	CheckErrorCode(__FUNCTION__, "Initializing front face parameters");
}

/**
 * If enabled, histogram incoming RGBA color values.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glHistogram.xml
 */
void rendering::ReadHistogramParameter()
{
	if (supportedOpenGLLevel > 300)
	{
		INFO_LOG_RENDERING("Histogram not supported in OpenGL version higher than 3.0");
		CheckErrorCode(__FUNCTION__, "Initializing histogram parameters");
		return;
	}
	const auto histogramEnabled = GET_CONFIG_VALUE_RENDERING("GL_HISTOGRAM_ENABLED", 0);
	if (histogramEnabled == 0)
	{
		glDisable(GL_HISTOGRAM);
		DEBUG_LOG_RENDERING("GL_HISTOGRAM disabled");
		CheckErrorCode(__FUNCTION__, "Initializing histogram parameters");
		return;
	}

	glEnable(GL_HISTOGRAM);

	std::string histogramTargetStr = GET_CONFIG_VALUE_STR_RENDERING("GL_HISTOGRAM_TARGET", "GL_HISTOGRAM");
	GLenum histogramTarget;
	if (histogramTargetStr == "GL_HISTOGRAM") { histogramTarget = GL_HISTOGRAM; }
	else if (histogramTargetStr == "GL_PROXY_HISTOGRAM") { histogramTarget = GL_PROXY_HISTOGRAM; }
	else /* GL_HISTOGRAM is default */
	{
		ERROR_LOG_RENDERING("Invalid enum \"", histogramTargetStr, "\" given for the histogram target parameter. Using default GL_HISTOGRAM");
		histogramTargetStr = "GL_HISTOGRAM";
		histogramTarget = GL_HISTOGRAM;
	}

	int histogramWidth = GET_CONFIG_VALUE_RENDERING("GL_HISTOGRAM_WIDTH", 16); // must be a power of 2

	std::string histogramInternalFormatStr = GET_CONFIG_VALUE_STR_RENDERING("GL_HISTOGRAM_INTERNAL_FORMAT", "GL_ALPHA");
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
		ERROR_LOG_RENDERING("Invalid enum \"", histogramInternalFormatStr, "\" given for the histogram internal format. Using default GL_RGBA");
		histogramInternalFormatStr = "GL_RGBA";
		histogramInternalFormat = GL_RGBA;
	}

	const auto histogramSink = GET_CONFIG_VALUE_RENDERING("GL_HISTOGRAM_SINK", false);
	glHistogram(histogramTarget, histogramWidth, histogramInternalFormat, histogramSink);

	if (histogramSink)
	{
		INFO_LOG_RENDERING("GL_HISTOGRAM enabled with target = \"", histogramTargetStr, "\", width = ", histogramWidth, ", internal format = \"", histogramInternalFormatStr, "\" and enabled sink");
	}
	else
	{
		INFO_LOG_RENDERING("GL_HISTOGRAM enabled with target = \"", histogramTargetStr, "\", width = ", histogramWidth, ", internal format = \"", histogramInternalFormatStr, "\" and disabled sink");
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
void rendering::ReadScissorTestParameter(int width, int height)
{
	glScissorTestEnabled = GET_CONFIG_VALUE_RENDERING("GL_SCISSOR_TEST_ENABLED", false);
	glScissorTestEnabledOld = glScissorTestEnabled;
	if (glScissorTestEnabled)
	{
		glEnable(GL_SCISSOR_TEST);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
		DEBUG_LOG_RENDERING("GL_SCISSOR_TEST disabled");
	}
	
	glScissorBoxLowerLeftCornerX = GET_CONFIG_VALUE_RENDERING("GL_SCISSOR_BOX_LOWER_LEFT_CORNER_X", 0);
	glScissorBoxLowerLeftCornerY = GET_CONFIG_VALUE_RENDERING("GL_SCISSOR_BOX_LOWER_LEFT_CORNER_Y", 0);
	glScissorBoxWidth = GET_CONFIG_VALUE_RENDERING("GL_SCISSOR_BOX_WIDTH", width);
	glScissorBoxHeight = GET_CONFIG_VALUE_RENDERING("GL_SCISSOR_BOX_HEIGHT", height);
	
	glScissorBoxLowerLeftCornerXOld = glScissorBoxLowerLeftCornerX;
	glScissorBoxLowerLeftCornerYOld = glScissorBoxLowerLeftCornerY;
	glScissorBoxWidthOld = glScissorBoxWidth;
	glScissorBoxHeightOld = glScissorBoxHeight;
	if (glScissorTestEnabled)
	{
		//glScissor(glScissorBoxLowerLeftCornerX, glScissorBoxLowerLeftCornerY, glScissorBoxWidth, glScissorBoxHeight);
		INFO_LOG_RENDERING("GL_SCISSOR_TEST enabled with lower left corner position = (", glScissorBoxLowerLeftCornerX, "; ",
			glScissorBoxLowerLeftCornerY, "), width = ", glScissorBoxWidth, " and height = ", glScissorBoxHeight);
	}
	CheckErrorCode(__FUNCTION__, "Initializing scissor test parameters");
}

/**
 * If enabled, do stencil testing and update the stencil buffer.
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glStencilFunc.xml
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glStencilFuncSeparate.xml
 * See https://www.opengl.org/sdk/docs/man3/xhtml/glStencilOp.xml
 */
void rendering::ReadStencilTestParameter()
{
	const auto stencilTestEnabled = GET_CONFIG_VALUE_RENDERING("GL_STENCIL_TEST_ENABLED", 0);
	if (stencilTestEnabled == 0)
	{
		glDisable(GL_STENCIL_TEST);
		DEBUG_LOG_RENDERING("GL_STENCIL_TEST_ENABLED disabled");
		return;
	}

	glEnable(GL_STENCIL_TEST);

	std::string stencilTestFuncStr = GET_CONFIG_VALUE_STR_RENDERING("GL_STENCIL_TEST_FUNC", "GL_ALWAYS");
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
		ERROR_LOG_RENDERING("Invalid enum \"", stencilTestFuncStr, "\" given for the stencil test function. Using default GL_ALWAYS");
		stencilTestFuncStr = "GL_ALWAYS";
		stencilTestFunc = GL_ALWAYS;
	}

	GLint stencilTestRefValue = GET_CONFIG_VALUE_RENDERING("GL_STENCIL_TEST_REF", 0);
	GLuint stencilTestMask = GET_CONFIG_VALUE_RENDERING("GL_STENCIL_TEST_MASK", 1);

	glStencilFunc(stencilTestFunc, stencilTestRefValue, stencilTestMask);
	INFO_LOG_RENDERING("GL_STENCIL_TEST enabled with function \"", stencilTestFuncStr, "\", reference value = ", stencilTestRefValue, " and the mask = ", stencilTestMask);
	CheckErrorCode(__FUNCTION__, "Initializing stencil test parameters");
}

/**
 * Reads different kinds of OpenGL capabilities regarding textures.
 */
void rendering::ReadTextureParameters()
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
void rendering::CheckErrorCode(const char* functionName, const char* comment)
{
	GLenum errCode = glGetError();
	if (errCode == GL_NO_ERROR)
	{
		return;
	}
	switch (errCode)
	{
	case GL_INVALID_ENUM:
		ERROR_LOG_RENDERING("Error occured in function \"", functionName, "\". ", comment, " failed with error code = GL_INVALID_ENUM");
		break;
	case GL_INVALID_VALUE:
		ERROR_LOG_RENDERING("Error occured in function \"", functionName, "\". ", comment, " failed with error code = GL_INVALID_VALUE");
		break;
	case GL_INVALID_OPERATION:
		ERROR_LOG_RENDERING("Error occured in function \"", functionName, "\". ", comment, " failed with error code = GL_INVALID_OPERATION");
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		ERROR_LOG_RENDERING("Error occured in function \"", functionName, "\". ", comment, " failed with error code = GL_INVALID_FRAMEBUFFER_OPERATION");
		break;
	case GL_OUT_OF_MEMORY:
		CRITICAL_LOG_RENDERING("Error occured in function \"", functionName, "\". ", comment, " failed with error code = GL_OUT_OF_MEMORY");
		exit(EXIT_FAILURE);
		break;
	case GL_STACK_UNDERFLOW:
		CRITICAL_LOG_RENDERING("Error occured in function \"", functionName, "\". ", comment, " failed with error code = GL_STACK_UNDERFLOW");
		exit(EXIT_FAILURE);
		break;
	case GL_STACK_OVERFLOW:
		CRITICAL_LOG_RENDERING("Error occured in function \"", functionName, "\". ", comment, " failed with error code = GL_STACK_OVERFLOW");
		exit(EXIT_FAILURE);
		break;
	default:
		CRITICAL_LOG_RENDERING("Error occured in function \"", functionName, "\". ", comment, " failed with error code = ", errCode);
		exit(EXIT_FAILURE);
	}
}

void rendering::CheckFramebufferStatus()
{
	/**
	* See https://www.opengl.org/sdk/docs/man3/xhtml/glCheckFramebufferStatus.xml
	*/
	GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (framebufferStatus)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		DELOCUST_LOG_RENDERING("Framebuffer is in status: GL_FRAMEBUFFER_COMPLETE");
		break;
	case GL_FRAMEBUFFER_UNDEFINED:
		ERROR_LOG_RENDERING("Framebuffer is in status: GL_FRAMEBUFFER_UNDEFINED");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		ERROR_LOG_RENDERING("Framebuffer is in status: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		ERROR_LOG_RENDERING("Framebuffer is in status: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		ERROR_LOG_RENDERING("Framebuffer is in status: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		ERROR_LOG_RENDERING("Framebuffer is in status: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		ERROR_LOG_RENDERING("Framebuffer is in status: GL_FRAMEBUFFER_UNSUPPORTED");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		ERROR_LOG_RENDERING("Framebuffer is in status: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		ERROR_LOG_RENDERING("Framebuffer is in status: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
		break;
	default:
		EMERGENCY_LOG_RENDERING("Framebuffer is in unknown status: ", framebufferStatus);
		break;
	}
}

#ifdef ANT_TWEAK_BAR_ENABLED
void TW_CALL rendering::TweakBarErrorHandler(const char* errorMessage)
{
	ERROR_LOG_RENDERING("TweakBar error: \"", errorMessage, "\"");
}

void rendering::InitializeTweakBars()
{
	TwInit(TW_OPENGL_CORE, nullptr); // Initializing AntTweakBar library
	TwHandleErrors(TweakBarErrorHandler);

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

	TwSetParam(glPropertiesBar, nullptr, "visible", TW_PARAM_CSTRING, 1, "false"); // Hide the bar at startup
#endif
}

void rendering::CheckChangesAndUpdateGLState()
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

void rendering::UpdateBlendParameters()
{
	if ((glBlendEnabled == glBlendEnabledOld) && (glBlendSfactor == glBlendSfactorOld) && (glBlendDfactor == glBlendDfactorOld))
	{
		return;
	}

	const GLenum glPropertyEnum = GL_BLEND;
	if (glBlendEnabled)
	{
		glEnable(glPropertyEnum);
		NOTICE_LOG_RENDERING("GL_BLEND is now enabled");
	}
	else
	{
		glDisable(glPropertyEnum);
		NOTICE_LOG_RENDERING("GL_BLEND is now disabled");
	}
	glBlendEnabledOld = glBlendEnabled;

	// Now we check if any of blending factors have been changed.
	if ((glBlendSfactor != glBlendSfactorOld) || (glBlendDfactor != glBlendDfactorOld))
	{
		glBlendFunc(glBlendSfactor, glBlendDfactor);
		NOTICE_LOG_RENDERING("Blending is now performed with the following parameters: sFactor = ", glBlendSfactor, ", dFactor = ", glBlendDfactor);
		glBlendSfactorOld = glBlendSfactor;
		glBlendDfactorOld = glBlendDfactor;
	}
}

void rendering::UpdateColorLogicOperationParameters()
{
	if ((glColorLogicOperationEnabled == glColorLogicOperationEnabledOld) && (glColorLogicOperationCode == glColorLogicOperationCodeOld))
	{
		return;
	}

	const GLenum glPropertyEnum = GL_COLOR_LOGIC_OP;
	if (glColorLogicOperationEnabled)
	{
		glEnable(glPropertyEnum);
		NOTICE_LOG_RENDERING("GL_COLOR_LOGIC_OP is now enabled");
	}
	else
	{
		glDisable(glPropertyEnum);
		NOTICE_LOG_RENDERING("GL_COLOR_LOGIC_OP is now disabled");
	}
	glColorLogicOperationEnabledOld = glColorLogicOperationEnabled;

	// Now we check if logic operation code has been changed
	if (glColorLogicOperationCode != glColorLogicOperationCodeOld)
	{
		glLogicOp(glColorLogicOperationCode);
		NOTICE_LOG_RENDERING("Color logic operation ", glColorLogicOperationCode, " is chosen");
		glColorLogicOperationCodeOld = glColorLogicOperationCode;
	}
}

void rendering::UpdateCullFaceParameters()
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
		NOTICE_LOG_RENDERING("GL_CULL_FACE is now enabled");
	}
	else
	{
		glDisable(glPropertyEnum);
		CheckErrorCode("Rendering::UpdateCullFaceParameters", "Disabling GL_CULL_FACE");
		NOTICE_LOG_RENDERING("GL_CULL_FACE is now disabled");
	}
	glCullFaceEnabledOld = glCullFaceEnabled;

	// Now we must check if cull face mode has been changed
	if (glCullFaceMode != glCullFaceModeOld)
	{
		glCullFace(glCullFaceMode);
		CheckErrorCode("Rendering::UpdateCullFaceParameters", "Setting the cull face mode");
		NOTICE_LOG_RENDERING("Face culling mode is ", glCullFaceMode);
		glCullFaceModeOld = glCullFaceMode;
	}

}

void rendering::UpdateDepthClampParameters()
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
		NOTICE_LOG_RENDERING("GL_DEPTH_CLAMP is now enabled");
	}
	else
	{
		glDisable(glPropertyEnum);
		CheckErrorCode("Rendering::UpdateDepthClampParameters", "Disabling GL_DEPTH_CLAMP");
		NOTICE_LOG_RENDERING("GL_DEPTH_CLAMP is now disabled");
	}
}

void rendering::UpdateDepthTestParameters()
{
	if (glDepthTestEnabled != glDepthTestEnabledOld)
	{
		if (glDepthTestEnabled)
		{
			glEnable(GL_DEPTH_TEST);
			CheckErrorCode("Rendering::UpdateDepthTestParameters", "Enabling GL_DEPTH_TEST");
			NOTICE_LOG_RENDERING("GL_DEPTH_TEST is now enabled");
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			CheckErrorCode("Rendering::UpdateDepthTestParameters", "Disabling GL_DEPTH_TEST");
			NOTICE_LOG_RENDERING("GL_DEPTH_TEST is now disabled");
		}
		glDepthTestEnabledOld = glDepthTestEnabled;
	}

	if (glDepthTestFunc != glDepthTestFuncOld)
	{
		glDepthFunc(glDepthTestFunc);
		CheckErrorCode("Rendering::UpdateDepthTestParameters", "Depth function change");
		NOTICE_LOG_RENDERING("Depth function changed from ", glDepthTestFuncOld, " to ", glDepthTestFunc);
		glDepthTestFuncOld = glDepthTestFunc;
	}

	//TODO: if (glDepthRangeNearValue
}

void rendering::UpdateDitheringParameters()
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
		NOTICE_LOG_RENDERING("GL_DITHER is now enabled");
	}
	else
	{
		glDisable(glPropertyEnum);
		NOTICE_LOG_RENDERING("GL_DITHER is now disabled");
	}
}

void rendering::UpdateFrontFaceParameters()
{
	if (glFrontFaceMode == glFrontFaceModeOld)
	{
		return;
	}

	glFrontFaceModeOld = glFrontFaceMode;
	glFrontFace(glFrontFaceMode);
	CheckErrorCode("Rendering::UpdateFrontFaceParameters", "Change front face parameter");
	NOTICE_LOG_RENDERING(glFrontFaceMode, " value has been chosen for the front face parameter");
}

void rendering::UpdateScissorTestParameters()
{
	if (glScissorTestEnabled != glScissorTestEnabledOld)
	{
		const GLenum glPropertyEnum = GL_SCISSOR_TEST;
		if (glScissorTestEnabled)
		{
			glEnable(glPropertyEnum);
			NOTICE_LOG_RENDERING("GL_SCISSOR_TEST is now enabled");
		}
		else
		{
			glDisable(glPropertyEnum);
			NOTICE_LOG_RENDERING("GL_SCISSOR_TEST is now disabled");
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
		NOTICE_LOG_RENDERING("Scissor test is now performed with the following parameters: left corner position = (", glScissorBoxLowerLeftCornerX,
			"; ", glScissorBoxLowerLeftCornerY, "), box width = ", glScissorBoxWidth, ", box height = ", glScissorBoxHeight);

		glScissorBoxLowerLeftCornerXOld = glScissorBoxLowerLeftCornerX;
		glScissorBoxLowerLeftCornerYOld = glScissorBoxLowerLeftCornerY;
		glScissorBoxWidthOld = glScissorBoxWidth;
		glScissorBoxHeightOld = glScissorBoxHeight;
	}
}

#endif