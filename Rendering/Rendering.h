#ifndef __RENDERING_RENDERING_H__
#define __RENDERING_RENDERING_H__

#ifdef RENDERING_EXPORTS
#define RENDERING_API __declspec(dllexport)
#else
#define RENDERING_API __declspec(dllimport)
#endif

#include "stdafx.h"
#include <string>
#include "Math\Math.h"
// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL\glew.h>

#define CALCULATE_RENDERING_STATS
#define ANT_TWEAK_BAR_ENABLED
#ifdef ANT_TWEAK_BAR_ENABLED
#define CONST_IF_TWEAK_BAR_DISABLED
#else
#define CONST_IF_TWEAK_BAR_DISABLED const
#endif

#define GET_CONFIG_VALUE_RENDERING(cfgName, defValue) GET_CONFIG_VALUE("Rendering", cfgName, defValue)
#define GET_CONFIG_VALUE_STR_RENDERING(cfgName, defValue) GET_CONFIG_VALUE_STR("Rendering", cfgName, defValue)

#define CRITICAL_LOG_RENDERING(...) CRITICAL_LOG("Rendering", ##__VA_ARGS__)
#define EMERGENCY_LOG_RENDERING(...) EMERGENCY_LOG("Rendering", ##__VA_ARGS__)
#define ERROR_LOG_RENDERING(...) ERROR_LOG("Rendering", ##__VA_ARGS__)
#define WARNING_LOG_RENDERING(...) WARNING_LOG("Rendering", ##__VA_ARGS__)
#define NOTICE_LOG_RENDERING(...) NOTICE_LOG("Rendering", ##__VA_ARGS__)
#define INFO_LOG_RENDERING(...) INFO_LOG("Rendering", ##__VA_ARGS__)
#define DEBUG_LOG_RENDERING(...) DEBUG_LOG("Rendering", ##__VA_ARGS__)
#define DELOCUST_LOG_RENDERING(...) DELOCUST_LOG("Rendering", ##__VA_ARGS__)

#define CHECK_CONDITION_RENDERING(expr, logLevel, ...) CHECK_CONDITION(expr, "Rendering", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_ALWAYS_RENDERING(expr, logLevel, ...) CHECK_CONDITION_ALWAYS(expr, "Rendering", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_RENDERING(expr, logLevel, ...) CHECK_CONDITION_EXIT(expr, "Rendering", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_EXIT_ALWAYS_RENDERING(expr, logLevel, ...) CHECK_CONDITION_EXIT_ALWAYS(expr, "Rendering", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_RENDERING(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID(expr, "Rendering", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(expr, logLevel, ...) CHECK_CONDITION_RETURN_VOID_ALWAYS(expr, "Rendering", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_RENDERING(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN(expr, returnValue, "Rendering", logLevel, ##__VA_ARGS__)
#define CHECK_CONDITION_RETURN_ALWAYS_RENDERING(expr, returnValue, logLevel, ...) CHECK_CONDITION_RETURN_ALWAYS(expr, returnValue, "Rendering", logLevel, ##__VA_ARGS__)

/// <summary>
/// If enabled the Rendering engine will be able to make simple debug drawing commands
/// (see section "Debug Drawing Facilities" in "Game Engine Architecture" by Jason Gregory).
/// </summary>
#define DEBUG_RENDERING_ENABLED

#ifndef CALCULATE_RENDERING_STATS
#define START_PROFILING_STATIC(param)
#define STOP_PROFILING_STATIC(param)
#define START_PROFILING(param)
#define STOP_PROFILING(param)
#endif

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
	namespace Text
	{
		enum Alignment
		{
			LEFT = 0,
			RIGHT,
			CENTER,
			//JUSTIFY // TODO: Implement JUSTIFY alignment
		}; /* end enum Alignment */
	} /* end namespace Text */

	namespace FogEffect
	{
		// must match the values in the FogCalculationType
		enum FogFallOffType
		{
			LINEAR = 0,
			EXPONENTIAL = 1
		};

		// must match the values in the FogFallOffType
		enum FogCalculationType
		{
			PLANE_BASED = 0,
			RANGE_BASED = 4
		};
	} /* end namespace FogEffect */

	namespace WaterEffect
	{
		enum WaterDistortionType
		{
			DISTORTION_X = 0,
			DISTORTION_Z = 1,
			DISTORTION_XZ = 2,
		};

		enum WaterReflectionEffect
		{
			REFLECTION_EFFECT_ENABLED = 0,
			REFLECTION_EFFECT_DISABLED = 1
		};

		enum WaterDepthEffect
		{
			DEPTH_EFFECT_ENABLED = 0,
			DEPTH_EFFECT_DISABLED = 1
		};
	} /* end namespace WaterEffect */

	namespace Aliasing
	{
		enum AntiAliasingMethod
		{
			NONE = 0,
			MSAA,
			FXAA
		};
	} /* end namespace Aliasing */

	void DetermineGlVersion();
	void PrintGlReport();

	RENDERING_API void InitGraphics(int width, int height, Aliasing::AntiAliasingMethod antiAliasingMethod);
	void CheckErrorCode(const char* functionName, const char* comment);
	void CheckFramebufferStatus();

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


	extern int supportedOpenGLLevel;
	extern std::string glslVersion;

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

	RENDERING_API void InitializeTweakBars();
	RENDERING_API void CheckChangesAndUpdateGLState();
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

#endif /* __RENDERING_RENDERING_H__ */