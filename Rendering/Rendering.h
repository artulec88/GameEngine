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

#define ANT_TWEAK_BAR_ENABLED

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


	//void ReadAlphaTestParameter();
	void ReadBlendParameter();
	void ReadColorLogicOperationParameter();
	void ReadCullFaceParameter();
	void ReadDepthClampParameter();
	void ReadDepthTestParameter();
	void ReadDitherParameter();
	void ReadFrontFaceParameter();
	void ReadHistogramParameter();
	//void ReadMinmaxParameter();
	//void ReadMultisampleParameter();
	void ReadScissorTestParameter(int width, int height);
	void ReadStencilTestParameter();
	void ReadTextureParameters();


	extern AntiAliasingMethod antiAliasingMethod;
	
	/**
	 * TODO: In the future all OpenGL paramaters declared below (e.g. glBlendEnabled, glBlendSFactor, glScissorTestEnabled etc.)
	 * should be declared inside the #ifdef ANT_TWEAK_BAR_ENABLED ... #endif, because they are only useful when tweak bars are rendered.
	 * When tweak bars are not rendered all global variables below should be replaced in the code with the local variables.
	 * All those local variables will be used to initialize graphics and then they will be deallocated.
	 *
	 * This way we should be able to store global variables in memory only when necessary (tweak bars enabled).
	 */
	/* ==================== Blending parameters begin ==================== */
	extern bool glBlendEnabled, glBlendEnabledOld;
	extern GLenum glBlendSfactor, glBlendDfactor, glBlendSfactorOld, glBlendDfactorOld;
	/* ==================== Blending parameters end ==================== */

	/* ==================== Color logic operation parameters begin ==================== */
	extern bool glColorLogicOperationEnabled, glColorLogicOperationEnabledOld;
	extern GLenum glColorLogicOperationCode, glColorLogicOperationCodeOld;
	/* ==================== Color logic operation parameters end ==================== */

	/* ==================== Cull face parameters begin ==================== */
	extern bool glCullFaceEnabled, glCullFaceEnabledOld;
	extern GLenum glCullFaceMode, glCullFaceModeOld;
	/* ==================== Cull face parameters end ==================== */

	/* ==================== Scissor test parameters begin ==================== */
	extern bool glScissorTestEnabled, glScissorTestEnabledOld;
	extern GLint glScissorBoxLowerLeftCornerX, glScissorBoxLowerLeftCornerXOld;
	extern GLint glScissorBoxLowerLeftCornerY, glScissorBoxLowerLeftCornerYOld;
	extern GLsizei glScissorBoxWidth, glScissorBoxWidthOld;
	extern GLsizei glScissorBoxHeight, glScissorBoxHeightOld;
	/* ==================== Scissor test parameters end ==================== */

#ifdef ANT_TWEAK_BAR_ENABLED
	void InitializeTweakBars();
	void CheckChangesAndUpdateGLState();
	void UpdateBlendParameters();
	void UpdateColorLogicOperationParameters();
	void UpdateCullFaceParameters();

	void UpdateScissorTestParameters();
#endif
} /* end namespace Rendering */