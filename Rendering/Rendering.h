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
#include "Math\Math.h"

#define ANT_TWEAK_BAR_ENABLED

#define OPEN_GL_PROPERTIES_TWEAK_BAR
#define RENDERER_PROPERTIES_BAR
#define CAMERA_TWEAK_BAR
//#define LIGHTS_TWEAK_BAR
#define GAME_PROPERTIES_TWEAK_BAR

#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBarTypes.h"
#include "AntTweakBar\include\AntTweakBar.h"
#else
#undef OPEN_GL_PROPERTIES_TWEAK_BAR
#undef RENDERER_PROPERTIES_BAR
#undef CAMERA_TWEAK_BAR
#undef LIGHTS_TWEAK_BAR
#undef GAME_PROPERTIES_TWEAK_BAR
#endif

namespace Rendering
{
	namespace GameTime
	{
		enum Daytime
		{
			NIGHT = 0,
			BEFORE_DAWN,
			SUNRISE,
			DAY,
			SUNSET,
			AFTER_DUSK
		};
	}

	enum AntiAliasingMethod
	{
		NONE = 0,
		MSAA,
		FXAA
	};

	GLFWwindow* InitGraphics(int width, int height, const std::string& title, GLFWwindow*& threadWindow);
	GLFWwindow* InitGlfw(int width, int height, const std::string& title, GLFWwindow*& threadWindow);
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

	/* ==================== Depth clamping parameters begin ==================== */
	extern bool glDepthClampEnabled, glDepthClampEnabledOld;
	/* ==================== Depth clamping parameters end ==================== */

	/* ==================== Depth test parameters begin ==================== */
	extern bool glDepthTestEnabled, glDepthTestEnabledOld;
	extern GLenum glDepthTestFunc, glDepthTestFuncOld;
	extern Math::Real glDepthRangeNearValue, glDepthRangeNearValueOld;
	extern Math::Real glDepthRangeFarValue, glDepthRangeFarValueOld;
	/* ==================== Depth test parameters end ==================== */

	/* ==================== Dithering parameters begin ==================== */
	extern bool glDitheringEnabled, glDitheringEnabledOld;
	/* ==================== Dithering parameters end ==================== */

	/* ==================== Front face parameters begin ==================== */
	extern GLenum glFrontFaceMode, glFrontFaceModeOld;
	/* ==================== Front face parameters end ==================== */

	/* ==================== Scissor test parameters begin ==================== */
	extern bool glScissorTestEnabled, glScissorTestEnabledOld;
	extern GLint glScissorBoxLowerLeftCornerX, glScissorBoxLowerLeftCornerXOld;
	extern GLint glScissorBoxLowerLeftCornerY, glScissorBoxLowerLeftCornerYOld;
	extern GLsizei glScissorBoxWidth, glScissorBoxWidthOld;
	extern GLsizei glScissorBoxHeight, glScissorBoxHeightOld;
	/* ==================== Scissor test parameters end ==================== */

#ifdef ANT_TWEAK_BAR_ENABLED
	void TW_CALL TweakBarErrorHandler(const char* errorMessage);

	void InitializeTweakBars();
	void CheckChangesAndUpdateGLState();
	void UpdateBlendParameters();
	void UpdateColorLogicOperationParameters();
	void UpdateCullFaceParameters();
	void UpdateDepthClampParameters();
	void UpdateDepthTestParameters();
	void UpdateDitheringParameters();
	void UpdateFrontFaceParameters();

	void UpdateScissorTestParameters();
#endif
} /* end namespace Rendering */