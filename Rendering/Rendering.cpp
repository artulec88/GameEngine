#include "stdafx.h"
#include "Rendering.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"
#include "Math\Math.h"

Rendering::AntiAliasingMethod Rendering::antiAliasingMethod = Rendering::AntiAliasingMethod::FXAA;

GLFWwindow* Rendering::InitGraphics(int width, int height, const std::string& title)
{
	LOG(Utility::Info, LOGPLACE, "Initializing graphics started");
	GLFWwindow* window = InitGlfw(width, height, title);
	InitGlew();


	ReadAlphaTestParameter();
	ReadBlendParameter();
	ReadCullFaceParameter();
	ReadDepthTestParameter();
	ReadDitherParameter();
	ReadFrontFaceParameter();
	ReadHistogramParameter();
	//ReadIndexLogicOperationParameter();
	//ReadMinmaxParameter();
	//ReadMultisampleParameter();
	//ReadScissorTestParameter();
	//ReadStencilTestParameter();
	//ReadTextureParameters();

	glFrontFace(GL_CW); // every face I draw in clockwise order is a front face
	//glCullFace(GL_BACK); // cull the back face
	//glEnable(GL_DEPTH_CLAMP); // prevents the camera to clip through the mesh

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
		LOG(Utility::Warning, LOGPLACE, "Unknown anti-aliasing algorithm chosen");
		glEnable(GL_MULTISAMPLE);
	}

	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_FRAMEBUFFER_SRGB); // Essentialy gives free gamma correction for better contrast. TODO: Test it!

	LOG(Utility::Info, LOGPLACE, "Initializing graphics finished successfully");
	return window;
}

GLFWwindow* Rendering::InitGlfw(int width, int height, const std::string& title)
{
	LOG(Utility::Debug, LOGPLACE, "Initializing GLFW started");
	if( !glfwInit() )
	{
		LOG(Utility::Critical, LOGPLACE, "Failed to initalize GLFW");
		exit(EXIT_FAILURE);
		// throw FileNotFoundException(); // TODO: throw another exception in the future
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
		LOG(Utility::Notice, LOGPLACE, "No anti-aliasing algorithm chosen");
		break;
	case FXAA:
		/**
		 * TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		 * Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		 */
		glfwWindowHint(GLFW_SAMPLES, 0);
		LOG(Utility::Notice, LOGPLACE, "FXAA anti-aliasing algorithm chosen");
		break;
	case MSAA:
		glfwWindowHint(GLFW_SAMPLES, antiAliasingSamples);
		LOG(Utility::Notice, LOGPLACE, "%dxMSAA anti-aliasing algorithm chosen", antiAliasingSamples);
		break;
	default:
		LOG(Utility::Warning, LOGPLACE, "Unknown anti-aliasing algorithm chosen. Default %dxMSAA algorithm chosen", antiAliasingSamples);
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
	//glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL /* glfwGetPrimaryMonitor()- for fullscreen */, NULL); // Open a window and create its OpenGL context
	if (window == NULL)
	{
		LOG(Utility::Critical, LOGPLACE, "Failed to open GLFW window.  If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we can capture the escape key being pressed below
	glfwSetCursorPos(window, width / 2, height / 2); // Set cursor position to the middle point
	//glfwSwapInterval(1);
	glfwSetTime(0.0);
	LOG(Utility::Debug, LOGPLACE, "Initializing GLFW finished successfully");
	return window;
}

void Rendering::InitGlew()
{
	LOG(Utility::Info, LOGPLACE, "Initializing GLEW started");
	glewExperimental = true; // Needed in core profile
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		LOG(Utility::Error, LOGPLACE, "Error while initializing GLEW: %s", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	if (GLEW_VERSION_2_0)
	{
		LOG(Utility::Info, LOGPLACE, "OpenGL 2.0 supported");
	}
	else
	{
		LOG(Utility::Info, LOGPLACE, "OpenGL 2.0 NOT supported");
		exit(EXIT_FAILURE);
	}

	LOG(Utility::Notice, LOGPLACE, "Using GLEW version %s", glewGetString(GLEW_VERSION));
}

/**
 * If enabled, do alpha testing.
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glAlphaFunc.xml
 */
void Rendering::ReadAlphaTestParameter()
{
	int alphaTestEnabled = GET_CONFIG_VALUE("GL_ALPHA_TEST_ENABLED", 0);
	if (alphaTestEnabled == 0)
	{
		glDisable(GL_ALPHA_TEST);
		LOG(Utility::Debug, LOGPLACE, "GL_ALPHA_TEST disabled");
		return;
	}

	glEnable(GL_ALPHA_TEST); // Perform ALPHA testing

	std::string alphaFuncStr = GET_CONFIG_VALUE_STR("GL_ALPHA_TEST_FUNC", "GL_LESS");
	Math::Real alphaFuncRefValue = GET_CONFIG_VALUE("GL_ALPHA_TEST_REF", 0.1f);
	if (alphaFuncStr == "GL_NEVER") { glAlphaFunc(GL_NEVER, alphaFuncRefValue); }
	else if (alphaFuncStr == "GL_LESS") { glAlphaFunc(GL_LESS, alphaFuncRefValue); }
	else if (alphaFuncStr == "GL_EQUAL") { glAlphaFunc(GL_EQUAL, alphaFuncRefValue); }
	else if (alphaFuncStr == "GL_LEQUAL") { glAlphaFunc(GL_LEQUAL, alphaFuncRefValue); }
	else if (alphaFuncStr == "GL_GREATER") { glAlphaFunc(GL_GREATER, alphaFuncRefValue); }
	else if (alphaFuncStr == "GL_NOTEQUAL") { glAlphaFunc(GL_NOTEQUAL, alphaFuncRefValue); }
	else if (alphaFuncStr == "GL_GEQUAL") { glAlphaFunc(GL_GEQUAL, alphaFuncRefValue); }
	else if (alphaFuncStr == "GL_ALWAYS") { glAlphaFunc(GL_ALWAYS, alphaFuncRefValue); }
	else /* GL_ALWAYS is default */
	{
		LOG(Utility::Error, LOGPLACE, "Invalid enum \"%s\" given for the alpha test function. Using default GL_ALWAYS", alphaFuncStr.c_str());
		alphaFuncStr = "GL_ALWAYS";
		glAlphaFunc(GL_ALWAYS, alphaFuncRefValue);
	}

	LOG(Utility::Info, LOGPLACE, "GL_ALPHA_TEST enabled with function \"%s\" and reference value = %.2f", alphaFuncStr.c_str(), alphaFuncRefValue);
}

/**
 * If enabled, blen the computed fragment color values with the values in the color buffers.
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glBlendFunc.xml
 */
void Rendering::ReadBlendParameter()
{
	int blendingEnabled = GET_CONFIG_VALUE("GL_BLEND_ENABLED", 0);
	if (blendingEnabled == 0)
	{
		glDisable(GL_BLEND);
		LOG(Utility::Debug, LOGPLACE, "GL_BLEND disabled");
		return;
	}

	glEnable(GL_BLEND);
	std::string blendSFactorStr = GET_CONFIG_VALUE_STR("GL_BLEND_SFACTOR", "GL_ONE");
	std::string blendDFactorStr = GET_CONFIG_VALUE_STR("GL_BLEND_DFACTOR", "GL_ZERO");
	GLenum sFactor, dFactor;
	if (blendSFactorStr == "GL_ZERO") { sFactor = GL_ZERO; }
	else if (blendSFactorStr == "GL_ONE") { sFactor = GL_ONE; }
	else if (blendSFactorStr == "GL_SRC_COLOR") { sFactor = GL_SRC_COLOR; }
	else if (blendSFactorStr == "GL_ONE_MINUS_SRC_COLOR") { sFactor = GL_ONE_MINUS_SRC_COLOR; }
	else if (blendSFactorStr == "GL_DST_COLOR") { sFactor = GL_DST_COLOR; }
	else if (blendSFactorStr == "GL_ONE_MINUS_DST_COLOR") { sFactor = GL_ONE_MINUS_DST_COLOR; }
	else if (blendSFactorStr == "GL_SRC_ALPHA") { sFactor = GL_SRC_ALPHA; }
	else if (blendSFactorStr == "GL_ONE_MINUS_SRC_ALPHA") { sFactor = GL_ONE_MINUS_SRC_ALPHA; }
	else if (blendSFactorStr == "GL_DST_ALPHA") { sFactor = GL_DST_ALPHA; }
	else if (blendSFactorStr == "GL_ONE_MINUS_DST_ALPHA") { sFactor = GL_ONE_MINUS_DST_ALPHA; }
	else if (blendSFactorStr == "GL_CONSTANT_COLOR") { sFactor = GL_CONSTANT_COLOR; }
	else if (blendSFactorStr == "GL_ONE_MINUS_CONSTANT_COLOR") { sFactor = GL_ONE_MINUS_CONSTANT_COLOR; }
	else if (blendSFactorStr == "GL_CONSTANT_ALPHA") { sFactor = GL_CONSTANT_ALPHA; }
	else if (blendSFactorStr == "GL_ONE_MINUS_CONSTANT_ALPHA") { sFactor = GL_ONE_MINUS_CONSTANT_ALPHA; }
	else if (blendSFactorStr == "GL_SRC_ALPHA_SATURATE") { sFactor = GL_SRC_ALPHA_SATURATE; }
	else /* GL_ONE is default */
	{
		LOG(Utility::Error, LOGPLACE, "Invalid enum \"%s\" given for the blend sFactor. Using default GL_ONE", blendSFactorStr.c_str());
		blendSFactorStr = "GL_ONE";
		sFactor = GL_ONE;
	}

	if (blendDFactorStr == "GL_ZERO") { dFactor = GL_ZERO; }
	else if (blendDFactorStr == "GL_ONE") { dFactor = GL_ONE; }
	else if (blendDFactorStr == "GL_SRC_COLOR") { dFactor = GL_SRC_COLOR; }
	else if (blendDFactorStr == "GL_ONE_MINUS_SRC_COLOR") { dFactor = GL_ONE_MINUS_SRC_COLOR; }
	else if (blendDFactorStr == "GL_DST_COLOR") { dFactor = GL_DST_COLOR; }
	else if (blendDFactorStr == "GL_ONE_MINUS_DST_COLOR") { dFactor = GL_ONE_MINUS_DST_COLOR; }
	else if (blendDFactorStr == "GL_SRC_ALPHA") { dFactor = GL_SRC_ALPHA; }
	else if (blendDFactorStr == "GL_ONE_MINUS_SRC_ALPHA") { dFactor = GL_ONE_MINUS_SRC_ALPHA; }
	else if (blendDFactorStr == "GL_DST_ALPHA") { dFactor = GL_DST_ALPHA; }
	else if (blendDFactorStr == "GL_ONE_MINUS_DST_ALPHA") { dFactor = GL_ONE_MINUS_DST_ALPHA; }
	else if (blendDFactorStr == "GL_CONSTANT_COLOR") { dFactor = GL_CONSTANT_COLOR; }
	else if (blendDFactorStr == "GL_ONE_MINUS_CONSTANT_COLOR") { dFactor = GL_ONE_MINUS_CONSTANT_COLOR; }
	else if (blendDFactorStr == "GL_CONSTANT_ALPHA") { dFactor = GL_CONSTANT_ALPHA; }
	else if (blendDFactorStr == "GL_ONE_MINUS_CONSTANT_ALPHA") { dFactor = GL_ONE_MINUS_CONSTANT_ALPHA; }
	else if (blendDFactorStr == "GL_SRC_ALPHA_SATURATE") { LOG(Utility::Error, LOGPLACE, "GL_SRC_ALPHA_SATURATE is not a valid dFactor value for blending operation. Using default GL_ZERO instead"); dFactor = GL_ZERO; }
	else /* GL_ZERO is default */
	{
		LOG(Utility::Error, LOGPLACE, "Invalid enum \"%s\" given for the blend dFactor. Using default GL_ZERO", blendDFactorStr.c_str());
		blendDFactorStr = "GL_ZERO";
		dFactor = GL_ZERO;
	}

	glBlendFunc(sFactor, dFactor);
	LOG(Utility::Info, LOGPLACE, "GL_BLEND enabled with sFactor = \"%s\" and dFactor = \"%s\"", blendSFactorStr.c_str(), blendDFactorStr.c_str());
}

/**
 * If enabled, cull polygons based on their winding in window coordinates.
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glCullFace.xml
 */
void Rendering::ReadCullFaceParameter()
{
	int cullFaceEnabled = GET_CONFIG_VALUE("GL_CULL_FACE_ENABLED", 0);
	if (cullFaceEnabled == 0)
	{
		glDisable(GL_CULL_FACE);
		LOG(Utility::Debug, LOGPLACE, "GL_CULL_FACE disabled");
		return;
	}

	glEnable(GL_CULL_FACE); // culling faces enabled. Cull triangles which normal is not towards the camera
	std::string cullFaceModeStr = GET_CONFIG_VALUE_STR("GL_CULL_FACE_MODE", "GL_BACK");
	if (cullFaceModeStr == "GL_FRONT") { glCullFace(GL_FRONT); }
	else if (cullFaceModeStr == "GL_BACK") { glCullFace(GL_BACK); }
	else if (cullFaceModeStr == "GL_FRONT_AND_BACK ") { glCullFace(GL_FRONT_AND_BACK ); }
	else /* GL_BACK is default */
	{
		LOG(Utility::Error, LOGPLACE, "Invalid enum \"%s\" given for the face culling parameter. Using default GL_BACK", cullFaceModeStr.c_str());
		cullFaceModeStr = "GL_BACK";
		glCullFace(GL_BACK);
	}

	LOG(Utility::Info, LOGPLACE, "GL_CULL_FACE enabled in \"%s\" mode", cullFaceModeStr.c_str());
}

/**
 * If enabled, do depth comparisons and update the depth buffer. Note that even if the depth buffer
 * exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glDepthFunc.xml
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glDepthRange.xml
 */
void Rendering::ReadDepthTestParameter()
{
	int depthTestEnabled = GET_CONFIG_VALUE("GL_DEPTH_TEST_ENABLED", 0);
	if (depthTestEnabled == 0)
	{
		glDisable(GL_DEPTH_TEST);
		LOG(Utility::Debug, LOGPLACE, "GL_DEPTH_TEST disabled");
		return;
	}

	glEnable(GL_DEPTH_TEST);

	std::string depthTestFuncStr = GET_CONFIG_VALUE_STR("GL_DEPTH_TEST_FUNC", "GL_LESS");
	if (depthTestFuncStr == "GL_NEVER") { glDepthFunc(GL_NEVER); }
	else if (depthTestFuncStr == "GL_LESS") { glDepthFunc(GL_LESS); } // Accept fragment if it closer to the camera than the former one
	else if (depthTestFuncStr == "GL_EQUAL") { glDepthFunc(GL_EQUAL); }
	else if (depthTestFuncStr == "GL_LEQUAL") { glDepthFunc(GL_LEQUAL); }
	else if (depthTestFuncStr == "GL_GREATER") { glDepthFunc(GL_GREATER); }
	else if (depthTestFuncStr == "GL_NOTEQUAL") { glDepthFunc(GL_NOTEQUAL); }
	else if (depthTestFuncStr == "GL_GEQUAL") { glDepthFunc(GL_GEQUAL); }
	else if (depthTestFuncStr == "GL_ALWAYS ") { glDepthFunc(GL_ALWAYS ); }
	else /* GL_LESS is default */
	{
		LOG(Utility::Error, LOGPLACE, "Invalid enum \"%s\" given for the depth test parameter. Using default GL_LESS", depthTestFuncStr.c_str());
		depthTestFuncStr = "GL_LESS";
		glDepthFunc(GL_LESS);
	}

	Math::Real depthTestRangeNearValue = GET_CONFIG_VALUE("GL_DEPTH_TEST_RANGE_NEAR", REAL_ZERO);
	Math::Real depthTestRangeFarValue = GET_CONFIG_VALUE("GL_DEPTH_TEST_RANGE_FAR", REAL_ONE);
	glDepthRange(depthTestRangeNearValue, depthTestRangeFarValue);
	LOG(Utility::Info, LOGPLACE, "GL_DEPTH_TEST enabled with function \"%s\" and the range [%.2f; %.2f]",
		depthTestFuncStr.c_str(), depthTestRangeNearValue, depthTestRangeFarValue);
}

/**
 * If enabled, dither color components or indices before they are written to the color buffer.
 */
void Rendering::ReadDitherParameter()
{
	int ditheringEnabled = GET_CONFIG_VALUE("GL_DITHER_ENABLED", 0);
	if (ditheringEnabled == 0)
	{
		glDisable(GL_DITHER);
		LOG(Utility::Debug, LOGPLACE, "GL_DITHER disabled");
		return;
	}

	glEnable(GL_DITHER);
	LOG(Utility::Info, LOGPLACE, "GL_DITHER enabled");
}

/**
 * Defines front- and back-facing polygons
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glFrontFace.xml
 */
void Rendering::ReadFrontFaceParameter()
{
	std::string frontFaceStr = GET_CONFIG_VALUE_STR("GL_FRONT_FACE", "GL_CW");
	GLenum frontFaceMode;
	if (frontFaceStr == "GL_CW") { frontFaceMode = GL_CW; }
	else if (frontFaceStr == "GL_CCW") { frontFaceMode = GL_CCW; }
	else /* GL_CCW is default */
	{
		LOG(Utility::Error, LOGPLACE, "Invalid enum \"%s\" given for the front face mode. Using default GL_CCW", frontFaceStr.c_str());
		frontFaceStr = "GL_CCW";
		frontFaceMode = GL_CCW;
	}

	glFrontFace(frontFaceMode);
	LOG(Utility::Info, LOGPLACE, "\"%s\" mode specified for the glFrontFace", frontFaceStr.c_str());
}

/**
 * If enabled, histogram incoming RGBA color values.
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glHistogram.xml
 */
void Rendering::ReadHistogramParameter()
{
	int histogramEnabled = GET_CONFIG_VALUE("GL_HISTOGRAM_ENABLED", 0);
	if (histogramEnabled == 0)
	{
		glDisable(GL_HISTOGRAM);
		LOG(Utility::Debug, LOGPLACE, "GL_HISTOGRAM disabled");
		return;
	}

	glEnable(GL_HISTOGRAM);

	std::string histogramTargetStr = GET_CONFIG_VALUE_STR("GL_HISTOGRAM_TARGET", "GL_HISTOGRAM");
	GLenum histogramTarget;
	if (histogramTargetStr == "GL_HISTOGRAM") { histogramTarget = GL_HISTOGRAM; }
	else if (histogramTargetStr == "GL_PROXY_HISTOGRAM") { histogramTarget = GL_PROXY_HISTOGRAM; }
	else /* GL_HISTOGRAM is default */
	{
		LOG(Utility::Error, LOGPLACE, "Invalid enum \"%s\" given for the histogram target parameter. Using default GL_HISTOGRAM", histogramTargetStr.c_str());
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
		LOG(Utility::Error, LOGPLACE, "Invalid enum \"%s\" given for the histogram internal format. Using default GL_RGBA", histogramInternalFormatStr.c_str());
		histogramInternalFormatStr = "GL_RGBA";
		histogramInternalFormat = GL_RGBA;
	}

	bool histogramSink = GET_CONFIG_VALUE("GL_HISTOGRAM_SINK", false);
	glHistogram(histogramTarget, histogramWidth, histogramInternalFormat, histogramSink);

	if (histogramSink)
	{
		LOG(Utility::Info, LOGPLACE, "GL_HISTOGRAM enabled with target = \"%s\", width = %d, internal format = \"%s\" and enabled sink",
			histogramTargetStr.c_str(), histogramWidth, histogramInternalFormatStr.c_str());
	}
	else
	{
		LOG(Utility::Info, LOGPLACE, "GL_HISTOGRAM enabled with target = \"%s\", width = %d, internal format = \"%s\" and disabled sink",
			histogramTargetStr.c_str(), histogramWidth, histogramInternalFormatStr.c_str());
	}
}

/**
 * If enabled, apply the currently selected logical operation to the incoming index and color buffer indices.
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glLogicOp.xml
 */
void Rendering::ReadIndexLogicOperationParameter()
{
	// TODO: Not yet implemented
}

/**
 * If enabled, compute the minimum and maximum values of incoming RGBA color values.
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glMinmax.xml
 */
void Rendering::ReadMinmaxParameter()
{
	// TODO: Not yet implemented
}

/**
 * If enabled, use multiple fragment samples in computing the final color of a pixel.
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glSampleCoverage.xml
 */
void Rendering::ReadMultisampleParameter()
{
	// TODO: Not yet implemented
}

/**
 * If enabled, discard fragments that are outside the scissor rectangle.
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glScissor.xml
 */
void Rendering::ReadScissorTestParameter()
{
	// TODO: Not yet implemented
}

/**
 * If enabled, do stencil testing and update the stencil buffer.
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glStencilFunc.xml
 * See https://www.opengl.org/sdk/docs/man2/xhtml/glStencilOp.xml
 */
void Rendering::ReadStencilTestParameter()
{
	// TODO: Not yet implemented
}

/**
 * Reads different kinds of OpenGL capabilities regarding textures.
 */
void Rendering::ReadTextureParameters()
{
	// TODO: Not yet implemented
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
		LOG(Utility::Error, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_INVALID_ENUM", functionName, comment);
		break;
	case GL_INVALID_VALUE:
		LOG(Utility::Error, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_INVALID_VALUE", functionName, comment);
		break;
	case GL_INVALID_OPERATION:
		LOG(Utility::Error, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_INVALID_OPERATION", functionName, comment);
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		LOG(Utility::Error, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_INVALID_FRAMEBUFFER_OPERATION", functionName, comment);
		break;
	case GL_OUT_OF_MEMORY:
		LOG(Utility::Critical, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_OUT_OF_MEMORY", functionName, comment);
		exit(EXIT_FAILURE);
		break;
	case GL_STACK_UNDERFLOW:
		LOG(Utility::Critical, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_STACK_UNDERFLOW", functionName, comment);
		exit(EXIT_FAILURE);
		break;
	case GL_STACK_OVERFLOW:
		LOG(Utility::Critical, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = GL_STACK_OVERFLOW", functionName, comment);
		exit(EXIT_FAILURE);
		break;
	default:
		LOG(Utility::Critical, LOGPLACE, "Error occured in function \"%s\". %s failed with error code = %d", functionName, comment, errCode);
		exit(EXIT_FAILURE);
	}
}