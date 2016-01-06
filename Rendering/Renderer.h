#pragma once

#include "Rendering.h"
#include "GameNode.h"
#include "Camera.h"
#include "BaseLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "MappedValues.h"
#include "Material.h"
#include "Transform.h"
#include "Shader.h"
#include "Color.h"
#include "MenuEntry.h"
#include "CubeShadowMap.h"

#include "Math\Angle.h"
#include "Math\Vector.h"
#ifdef DEBUG_RENDERING_ENABLED
#include "Math\Sphere.h"
#include "Math\AABB.h"
#include "Math\OBB.h"
#endif
#ifdef CALCULATE_RENDERING_STATS
#include "Math\Statistics.h"
#include "Math\IStatisticsStorage.h"
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBar\include\AntTweakBar.h"
#endif

#include <string>
#include <vector>
#include <map>

namespace Rendering
{

class Mesh;
class TextRenderer;

// TODO: Consider creating Singleton template class from which Renderer would inherit
class Renderer : public MappedValues
{
/* ==================== Static variables begin ==================== */
private:
	static const int SHADOW_MAPS_COUNT = 11;
	static const Math::Matrix4D BIAS_MATRIX;
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API Renderer(GLFWwindow* window, GLFWwindow* threadWindow);
	RENDERING_API virtual ~Renderer(void);
private:
	Renderer(const Renderer& renderer);
	void operator=(const Renderer& renderer);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	RENDERING_API void Render(const GameNode& node);
	RENDERING_API void RenderMainMenu(const MenuEntry& menuEntry);
	RENDERING_API void RenderLoadingScreen(Math::Real loadingProgress);
	RENDERING_API inline void SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	RENDERING_API GLFWwindow* GetThreadWindow()
	{
		return m_threadWindow;
	}

#ifdef ANT_TWEAK_BAR_ENABLED
	RENDERING_API void InitializeTweakBars();
	RENDERING_API void CheckCameraIndexChange();
#endif
	
	RENDERING_API inline void AddLight(Lighting::BaseLight* light);
	RENDERING_API inline void AddCamera(CameraBase* camera);
	RENDERING_API inline const Lighting::BaseLight* GetCurrentLight() const
	{
		CHECK_CONDITION_EXIT(m_currentLight != NULL, Utility::Error, "Current light is NULL.");
		return m_currentLight;
	}
	RENDERING_API inline const Lighting::PointLight* GetPointLight() const
	{
		return m_pointLight;
	}
	//RENDERING_API inline const Lighting::SpotLight* GetSpotLight() const { return m_spotLight; }
	RENDERING_API inline const Math::Vector3D& GetAmbientDayLight() const
	{
		return m_ambientDaytimeColor;
	}
	RENDERING_API inline const Math::Vector3D& GetAmbientNightLight() const
	{
		return m_ambientNighttimeColor;
	}
	
	RENDERING_API inline CameraBase& GetCurrentCamera()
	{
		CHECK_CONDITION_EXIT(m_currentCamera != NULL, Utility::Emergency, "Current camera is NULL.");
		return *m_currentCamera;
	}

	RENDERING_API size_t GetCurrentCameraIndex() const
	{
		return m_currentCameraIndex;
	}
	RENDERING_API unsigned int NextCamera();
	RENDERING_API unsigned int PrevCamera();
	RENDERING_API unsigned int SetCurrentCamera(unsigned int cameraIndex);
	RENDERING_API void SetCursorPos(Math::Real xPos, Math::Real yPos) const
	{
		glfwSetCursorPos(m_window, xPos, yPos);
	}
	
	RENDERING_API inline unsigned int GetSamplerSlot(const std::string& samplerName) const
	{
		std::map<std::string, unsigned int>::const_iterator samplerItr = m_samplerMap.find(samplerName);
		CHECK_CONDITION_EXIT(samplerItr != m_samplerMap.end(), Utility::Error, "Sampler name \"%s\" has not been found in the sampler map.", samplerName.c_str());
		return samplerItr->second;
	}
	RENDERING_API void UpdateUniformStruct(const Transform& transform, const Material& material, const Shader& shader, const std::string& uniformName, const std::string& uniformType);

	RENDERING_API inline bool IsCloseRequested() const
	{
		return glfwWindowShouldClose(m_window) != 0;
	}

	RENDERING_API inline Math::Matrix4D GetLightMatrix() const
	{
		return m_lightMatrix;
	}

	void RequestWindowClose() const
	{
		glfwSetWindowShouldClose(m_window, GL_TRUE);
	}
	void RegisterTerrainNode(GameNode* terrainNode);
	void BindCubeShadowMap(unsigned int textureUnit) const;

#ifdef DEBUG_RENDERING_ENABLED
	/// <summary>
	/// Adds a line segment to the debug drawing queue.
	/// </summary>
	RENDERING_API void AddLine(const Math::Vector3D& fromPosition, const Math::Vector3D& toPosition, const Color& color,
		Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds a wireframe sphere to the debug drawing queue.
	/// </summary>
	RENDERING_API void AddSphere(const Math::Sphere& sphere, const Color& color,
		Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds an axis-aligned cross (3 lines converging at a point) to the debug drawing queue.
	/// </summary>
	RENDERING_API void AddCross(const Math::Vector3D& position, const Color& color, Math::Real size,
		Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds a circle to the debug drawing queue.
	/// </summary>
	RENDERING_API void AddCircle(const Math::Vector3D& centerPosition, const Math::Vector3D& planeNormal, Math::Real radius,
		const Color& color, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds a set of coordinate axes depicting the position and orientation of the given transformation to the debug drawing queue.
	/// </summary>
	RENDERING_API void AddAxes(const Transform& transform, const Color& color, Math::Real size,
		Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds a wireframe triangle to the debug drawing queue.
	/// </summary>
	RENDERING_API void AddTriangle(const Math::Vector3D& v0, const Math::Vector3D& v1, const Math::Vector3D& v2,
		const Color& color, Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds an axis-aligned bounding box to the debug drawing queue.
	/// </summary>
	RENDERING_API void AddAABB(const Math::AABB& aabb, const Color& color, Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds an oriented bounding box to the debug drawing queue.
	/// </summary>
	RENDERING_API void AddOBB(const Math::OBB& obb, const Color& color, Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds a text string to the debug drawing queue.
	/// </summary>
	RENDERING_API void AddString(const Math::Vector3D& pos, const char* text, const Color& color, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);
#endif

protected:
	void RenderSkybox();
	void RenderSceneWithAmbientLight(const GameNode& gameNode);
	void RenderSceneWithPointLights(const GameNode& gameNode);
	void RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode);
	void SetCallbacks();

	inline void ClearScreen() const
	{
		if (m_ambientLightFogEnabled)
		{
			Math::Vector3D fogColor = m_ambientLightFogColor * m_ambientLight;
			glClearColor(fogColor.GetX(), fogColor.GetY(), fogColor.GetZ(), REAL_ONE);
		}
		else
		{
			glClearColor(m_backgroundColor.GetRed(), m_backgroundColor.GetGreen(), m_backgroundColor.GetBlue(), m_backgroundColor.GetAlpha());
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	inline void ClearScreen(const Color& clearColor) const
	{
		glClearColor(clearColor.GetRed(), clearColor.GetGreen(), clearColor.GetBlue(), clearColor.GetAlpha());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	inline void SetSamplerSlot(const std::string& name, unsigned int value)
	{
		m_samplerMap[name] = value;
	}
private:
	void InitializeCubeMap();
	Texture* InitializeCubeMapTexture(const std::string& cubeMapTextureDirectory);
	void AdjustAmbientLightAccordingToCurrentTime();
	void BindAsRenderTarget();
	void BlurShadowMap(int shadowMapIndex, Math::Real blurAmount);
	void ApplyFilter(Shader* filterShader, Texture* source, Texture* dest);
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	bool m_applyFilterEnabled;
	Color m_backgroundColor;
	//bool m_shadowsEnabled;
	//bool m_pointLightShadowsEnabled;
	GLFWwindow* m_window;
	GLFWwindow* m_threadWindow;
	//GLuint framebuffer;
	GLuint m_vao; // vertex array id
	bool m_ambientLightFogEnabled;
	Math::Vector3D m_ambientLightFogColor;
	Math::Real m_ambientLightFogStart;
	Math::Real m_ambientLightFogEnd;
	Math::Real m_ambientLightFogDensityFactor;
	FogEffect::FogFallOffType m_ambientLightFogFallOffType;
	FogEffect::FogCalculationType m_ambientLightFogCalculationType;
	const Math::Vector3D m_ambientDaytimeColor;
	const Math::Vector3D m_ambientSunNearHorizonColor;
	const Math::Vector3D m_ambientNighttimeColor;
	Math::Vector3D m_ambientLight;
	Lighting::BaseLight* m_currentLight;
	Lighting::PointLight* m_pointLight; // current point light
	Lighting::SpotLight* m_spotLight; // current spot light

	size_t m_currentCameraIndex;
	CameraBase* m_currentCamera;
	
	/// <summary> The main menu camera. This camera will be used
	/// in the main menu rendering when there are no game cameras set up. </summary>
	CameraBase* m_mainMenuCamera;
	
	/// <summary> The alternative camera for shadow mapping, rendering to texture etc. </summary>
	Camera m_altCamera;
	Texture* m_filterTexture;
	Material* m_filterMaterial;
	Transform m_filterTransform;
	Mesh* m_filterMesh;

	GameNode* m_terrainNode;
	Shader* m_ambientShaderTerrain;
	std::map<FogEffect::Fog, Shader*> m_ambientShadersFogEnabledMap;
	std::map<FogEffect::Fog, Shader*> m_ambientShadersFogEnabledTerrainMap;

	Shader* m_ambientShader;
	Shader* m_shadowMapShader;
	Shader* m_nullFilterShader;
	Shader* m_gaussBlurFilterShader;
	Shader* m_fxaaFilterShader;

	Math::Real m_fxaaSpanMax;
	Math::Real m_fxaaReduceMin;
	Math::Real m_fxaaReduceMul;

	Math::Angle m_skyboxAngle;
	const Math::Angle m_skyboxAngleStep;
	GameNode* m_skyboxNode;
	Shader* m_skyboxShader;
	Shader* m_skyboxShaderProcedural;
	Texture* m_skyboxTextureDay;
	Texture* m_skyboxTextureNight;

	const Math::Real m_defaultShadowMinVariance;
	Shader* m_cubeMapShader; // for use by the point lights
	CubeShadowMap* m_cubeShadowMap; // for use by the point lights
	//Texture* m_cubeShadowMap; // for use by the point lights
	Texture* m_shadowMaps[SHADOW_MAPS_COUNT];
	Texture* m_shadowMapTempTargets[SHADOW_MAPS_COUNT];

	std::vector<Lighting::BaseLight*> m_lights;
	std::vector<Lighting::BaseLight*> m_directionalAndSpotLights;
	std::vector<Lighting::PointLight*> m_pointLights;
	//std::vector<Lighting::SpotLight*> m_spotLights;

	std::vector<CameraBase*> m_cameras;
	std::map<std::string, unsigned int> m_samplerMap;
	Math::Matrix4D m_lightMatrix;

	Texture* m_fontTexture;
	TextRenderer* m_textRenderer;

	Math::Vector3D m_waterRefractionClippingPlaneNormal;
	Math::Vector3D m_waterReflectionClippingPlaneNormal;
	float m_waterRefractionClippingPlaneOriginDistance;
	float m_waterReflectionClippingPlaneOriginDistance;
	Texture* m_waterRefractionTexture;
	Texture* m_waterReflectionTexture;
	Shader* m_waterShader;

#ifdef ANT_TWEAK_BAR_ENABLED
	unsigned int m_cameraCountMinusOne;
//#ifdef CARTOON_SHADING_ENABLED // cartoon shading is included in the Lighting.glh file (probably commented out)
//	bool cartoonShadingEnabled;
//	Shader* cartoonShader;
//#endif
	unsigned int m_previousFrameCameraIndex;
	TwBar* m_propertiesBar;
	TwBar* m_cameraBar;
	TwBar* m_lightsBar;
	TwStructMember m_cameraMembers[5];
	TwType m_cameraType;
#endif

#ifdef CALCULATE_RENDERING_STATS
	Math::Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class Renderer */

} /* end namespace Rendering */
