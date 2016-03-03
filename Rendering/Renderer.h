#pragma once

#include "Rendering.h"
#include "Shader.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "MappedValues.h"
#include "Material.h"
#include "BaseLight.h"
#include "Color.h"
//#include "MenuEntry.h"
#include "CubeShadowMap.h"

#include "Math\Angle.h"
#include "Math\Vector.h"
#include "Math\Plane.h"
#include "Math\Transform.h"
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

class Renderer
{
/* ==================== Static variables begin ==================== */
private:
	static const int SHADOW_MAPS_COUNT = 11;
	static const Math::Matrix4D BIAS_MATRIX;
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API Renderer(int windowWidth, int windowHeight);
	RENDERING_API virtual ~Renderer(void);
private:
	Renderer(const Renderer& renderer);
	void operator=(const Renderer& renderer);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	RENDERING_API void InitRenderScene();
	RENDERING_API void BindDisplayTexture();
	//RENDERING_API void RenderWithAmbientLight(const Mesh& mesh, const Material* material, const Math::Transform& transform) const;
	RENDERING_API void Render(const Mesh& mesh, const Material* material, const Math::Transform& transform, const Shader* shader) const;
	RENDERING_API void FinalizeRenderScene();
	//RENDERING_API void Render(const GameNode& node);
	RENDERING_API void RenderLoadingScreen(Math::Real loadingProgress) const;

	RENDERING_API bool InitShadowMap();
	RENDERING_API void FinalizeShadowMapRendering();

	RENDERING_API void RenderString(Text::Alignment alignment, int y, const std::string& str) const;
	RENDERING_API void RenderString(Text::Alignment alignment, int y, const std::string& str, Math::Real fontSize) const;
	RENDERING_API void RenderString(Text::Alignment alignment, int y, const std::string& str, const Math::Vector4D& fontColor) const;
	RENDERING_API void RenderString(Text::Alignment alignment, int y, const std::string& str, Math::Real fontSize, const Math::Vector4D& fontColor) const;
	RENDERING_API void RenderString(int x, int y, const std::string& str) const;
	RENDERING_API void RenderString(int x, int y, const std::string& str, Math::Real fontSize) const;
	RENDERING_API void RenderString(int x, int y, const std::string& str, const Math::Vector4D& fontColor) const;
	RENDERING_API void RenderString(int x, int y, const std::string& str, Math::Real fontSize, const Math::Vector4D& fontColor) const;

#ifdef ANT_TWEAK_BAR_ENABLED
	/// <summary>
	/// Initializes the rendering engine tweak bar.
	/// </summary>
	/// <remarks>
	/// This function must be called after AntTweakBar library is initialized. See Rendering::InitializeTweakBars().
	/// </remarks>
	RENDERING_API void InitializeTweakBars();
	void CheckCameraIndexChange();
#endif
	int GetWindowWidth() const { return m_windowWidth; }
	int GetWindowHeight() const { return m_windowHeight; }
	RENDERING_API void SetWindowWidth(int windowWidth) { m_windowWidth = windowWidth; }
	RENDERING_API void SetWindowHeight(int windowHeight) { m_windowHeight = windowHeight; }
	
	RENDERING_API inline void AddLight(Lighting::BaseLight* light);
	RENDERING_API inline void AddCamera(CameraBase* camera);
	//RENDERING_API void AddTerrainNode(GameNode* terrainNode);
	//RENDERING_API void AddWaterNode(GameNode* waterNode);
	//RENDERING_API void AddBillboardNode(GameNode* billboardNode);
	//RENDERING_API void AddSkyboxNode(GameNode* skyboxNode);

	RENDERING_API inline void ClearScreen() const
	{
		if (m_fogEnabled)
		{
			Math::Vector3D fogColor = m_fogColor * m_ambientLight /* TODO: Should ambient light be included here? */;
			glClearColor(fogColor.GetX(), fogColor.GetY(), fogColor.GetZ(), REAL_ONE);
		}
		else
		{
			glClearColor(m_backgroundColor.GetRed(), m_backgroundColor.GetGreen(), m_backgroundColor.GetBlue(), m_backgroundColor.GetAlpha());
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	RENDERING_API inline void ClearScreen(const Color& clearColor) const
	{
		glClearColor(clearColor.GetRed(), clearColor.GetGreen(), clearColor.GetBlue(), clearColor.GetAlpha());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	RENDERING_API inline void ClearScreen(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{
		glClearColor(red, green, blue, alpha);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	inline size_t GetDirectionalAndSpotLightsCount() const
	{
		return m_directionalAndSpotLights.size();
	}
	inline const Lighting::BaseLight* GetCurrentLight() const
	{
		CHECK_CONDITION_EXIT(m_currentLight != NULL, Utility::Error, "Current light is NULL.");
		return m_currentLight;
	}
	inline const Lighting::BaseLight* SetCurrentLight(size_t index)
	{
		// TODO: Index range checking
		m_currentLight = m_directionalAndSpotLights[index];
		return m_currentLight;
	}
	inline size_t GetPointLightsCount() const
	{
		return m_pointLights.size();
	}
	inline const Lighting::PointLight* GetCurrentPointLight() const
	{
		return m_currentPointLight;
	}
	inline const Lighting::PointLight* SetCurrentPointLight(size_t index)
	{
		// TODO: Index range checking
		m_currentPointLight = m_pointLights[index];
		return m_currentPointLight;
	}
	//inline const Lighting::SpotLight* GetSpotLight() const { return m_spotLight; }
	inline const Math::Vector3D& GetAmbientDayLight() const
	{
		return m_ambientDaytimeColor;
	}
	inline const Math::Vector3D& GetAmbientNightLight() const
	{
		return m_ambientNighttimeColor;
	}
	
	inline const CameraBase& GetCurrentCamera() const
	{
		CHECK_CONDITION_EXIT(m_currentCamera != NULL, Utility::Emergency, "Current camera is NULL.");
		return *m_currentCamera;
	}

	size_t GetCurrentCameraIndex() const
	{
		return m_currentCameraIndex;
	}
	RENDERING_API size_t NextCamera();
	RENDERING_API size_t PrevCamera();
	//RENDERING_API void SetMenuCameraAsCurrent();
	RENDERING_API void SetCurrentCamera();
	size_t SetCurrentCamera(size_t cameraIndex);

	RENDERING_API void BindAsRenderTarget() const;
	RENDERING_API void InitLightRendering() const;
	RENDERING_API void FinalizeLightRendering() const;
	
	inline unsigned int GetSamplerSlot(const std::string& samplerName) const
	{
		std::map<std::string, unsigned int>::const_iterator samplerItr = m_samplerMap.find(samplerName);
		CHECK_CONDITION_EXIT_ALWAYS(samplerItr != m_samplerMap.end(), Utility::Error, "Sampler name \"%s\" has not been found in the sampler map.", samplerName.c_str());
		return samplerItr->second;
	}

	inline const Math::Matrix4D& GetLightMatrix() const
	{
		return m_lightMatrix;
	}

	RENDERING_API const Shader* GetAmbientShader() const;
	RENDERING_API const Shader* GetAmbientTerrainShader() const;
	RENDERING_API const Shader* GetShadowMapShader() const
	{
		return m_shadowMapShader;
	}
	RENDERING_API void AdjustAmbientLightAccordingToCurrentTime(Utility::Timing::Daytime dayTime, Math::Real dayTimeTransitionFactor);

	void BindCubeShadowMap(unsigned int textureUnit) const;

	Math::Real GetReal(const std::string& realValueName) const { return m_mappedValues.GetReal(realValueName); }
	const Math::Vector3D& GetVec3D(const std::string& vectorName) const { return m_mappedValues.GetVec3D(vectorName); }
	const Math::Vector4D& GetVec4D(const std::string& vectorName) const { return m_mappedValues.GetVec4D(vectorName); }
	const Texture* GetTexture(const std::string& textureName) const { return m_mappedValues.GetTexture(textureName); }
	const Texture* GetTexture(const std::string& textureName, unsigned int* multitextureIndex) const { return m_mappedValues.GetTexture(textureName, multitextureIndex); }

	RENDERING_API void DisableClippingPlanes();

#ifdef DEBUG_RENDERING_ENABLED
	/// <summary>
	/// Rendering debug nodes, i.e. the nodes that do not belong to the scene, but can be used to see the intermediate results of some other processing (e.g. how shadow map looks).
	/// </summary>
	RENDERING_API void RenderDebugNodes();

	/// <summary>
	/// Adds a line segment to the debug drawing queue.
	/// </summary>
	void AddLine(const Math::Vector3D& fromPosition, const Math::Vector3D& toPosition, const Color& color,
		Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds a wireframe sphere to the debug drawing queue.
	/// </summary>
	void AddSphere(const Math::Sphere& sphere, const Color& color,
		Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds an axis-aligned cross (3 lines converging at a point) to the debug drawing queue.
	/// </summary>
	void AddCross(const Math::Vector3D& position, const Color& color, Math::Real size,
		Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds a circle to the debug drawing queue.
	/// </summary>
	void AddCircle(const Math::Vector3D& centerPosition, const Math::Vector3D& planeNormal, Math::Real radius,
		const Color& color, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds a set of coordinate axes depicting the position and orientation of the given transformation to the debug drawing queue.
	/// </summary>
	void AddAxes(const Math::Transform& transform, const Color& color, Math::Real size,
		Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds a wireframe triangle to the debug drawing queue.
	/// </summary>
	void AddTriangle(const Math::Vector3D& v0, const Math::Vector3D& v1, const Math::Vector3D& v2,
		const Color& color, Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds an axis-aligned bounding box to the debug drawing queue.
	/// </summary>
	void AddAABB(const Math::AABB& aabb, const Color& color, Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds an oriented bounding box to the debug drawing queue.
	/// </summary>
	void AddOBB(const Math::OBB& obb, const Color& color, Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

	/// <summary>
	/// Adds a text string to the debug drawing queue.
	/// </summary>
	void AddString(const Math::Vector3D& pos, const char* text, const Color& color, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);
#endif

protected:
	/// <summary>
	/// Water textures (reflection, refraction) rendering pass.
	/// </summary>
	/// <remarks>
	/// The implementation is based on the tutorial: https://www.youtube.com/watch?v=0NH9k4zTAqk.
	/// </remarks>
	//void RenderWaterTextures(const GameNode& gameNode);
	//void RenderWaterReflectionTexture(const GameNode& gameNode);
	//void RenderWaterRefractionTexture(const GameNode& gameNode);

	/// <summary>
	/// Renders the registered water nodes using the reflection and refraction textures created during the earlier stage of rendering pass.
	/// </summary>
	//void RenderWaterNodes();
	//void RenderBillboardNodes();
	//void RenderSkybox();
	//void RenderSceneWithAmbientLight(const GameNode& gameNode);
	//void RenderSceneWithPointLights(const GameNode& gameNode);
	//void RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode, bool isCastingShadowsEnabled = true);

	inline void SetSamplerSlot(const std::string& name, unsigned int value)
	{
		m_samplerMap[name] = value;
	}
private:
	Texture* InitializeCubeMapTexture(const std::string& cubeMapTextureDirectory);
	void BlurShadowMap(int shadowMapIndex, Math::Real blurAmount);
	void ApplyFilter(const Shader* filterShader, const Texture* source, const Texture* dest);
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	int m_windowWidth, m_windowHeight;
	// TODO: In the future, before shipping the game engine, remove variables (or declare them as const) that are only used when ANT_TWEAK_BAR_ENABLED is defined.
	CONST_IF_TWEAK_BAR_DISABLED bool m_applyFilterEnabled;
	CONST_IF_TWEAK_BAR_DISABLED Color m_backgroundColor;
	//bool m_shadowsEnabled;
	//bool m_pointLightShadowsEnabled;
	//GLuint framebuffer;
	GLuint m_vao; // vertex array id

	CONST_IF_TWEAK_BAR_DISABLED bool m_fogEnabled;
	CONST_IF_TWEAK_BAR_DISABLED Math::Vector3D m_fogColor;
	CONST_IF_TWEAK_BAR_DISABLED Math::Real m_fogStart;
	CONST_IF_TWEAK_BAR_DISABLED Math::Real m_fogEnd;
	/// <summary> Represent the thickness of the fog. Increasing this value will generally decrease the visibility of the scene. </summary>
	CONST_IF_TWEAK_BAR_DISABLED Math::Real m_fogDensityFactor;
	/// <summary>
	/// Represents how fast the transition between no-fog and complete fog is performed.
	/// In other words it determines how quickly the visibility of the scene decreases with distance.
	/// </summary>
	CONST_IF_TWEAK_BAR_DISABLED Math::Real m_fogGradient;
	CONST_IF_TWEAK_BAR_DISABLED FogEffect::FogFallOffType m_fogFallOffType;
	CONST_IF_TWEAK_BAR_DISABLED FogEffect::FogCalculationType m_fogCalculationType;

	CONST_IF_TWEAK_BAR_DISABLED bool m_ambientLightEnabled;
	CONST_IF_TWEAK_BAR_DISABLED Math::Vector3D m_ambientDaytimeColor;
	CONST_IF_TWEAK_BAR_DISABLED Math::Vector3D m_ambientSunNearHorizonColor;
	CONST_IF_TWEAK_BAR_DISABLED Math::Vector3D m_ambientNighttimeColor;
	Math::Vector3D m_ambientLight;
	Lighting::BaseLight* m_currentLight;
	/// <summary>
	/// Current point light.
	/// </summary>
	Lighting::PointLight* m_currentPointLight;
	Lighting::SpotLight* m_spotLight; // current spot light

	size_t m_currentCameraIndex;
	CameraBase* m_currentCamera;
	CameraBase* m_tempCamera;
	
	/// <summary> The main menu camera. This camera will be used
	/// in the main menu rendering when there are no game cameras set up. </summary>
	CameraBase* m_mainMenuCamera;
	
	/// <summary> The alternative camera for shadow mapping, rendering to texture etc. </summary>
	Camera m_altCamera;
	Texture* m_filterTexture;
	Material* m_filterMaterial;
	Math::Transform m_filterTransform;
	Mesh* m_filterMesh;

	//std::vector<GameNode*> m_terrainNodes;
	Shader* m_ambientShaderTerrain;
	std::map<FogEffect::FogKey, Shader*> m_ambientShadersFogEnabledMap;
	std::map<FogEffect::FogKey, Shader*> m_ambientShadersFogEnabledTerrainMap;

	Shader* m_ambientShader;
	Shader* m_shadowMapShader;
	Shader* m_nullFilterShader;
	Shader* m_gaussBlurFilterShader;
	Shader* m_fxaaFilterShader;

	CONST_IF_TWEAK_BAR_DISABLED Math::Real m_fxaaSpanMax;
	CONST_IF_TWEAK_BAR_DISABLED Math::Real m_fxaaReduceMin;
	CONST_IF_TWEAK_BAR_DISABLED Math::Real m_fxaaReduceMul;

	Math::Angle m_skyboxAngle;
	const Math::Angle m_skyboxAngleStep;
	//GameNode* m_skyboxNode;
	Shader* m_skyboxShader;
	Shader* m_skyboxProceduralShader;

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

	Material* m_fontMaterial;
	Math::Real m_defaultFontSize;
	Math::Vector4D m_defaultFontColor;
	Shader* m_textShader;
	GLuint m_textVertexBuffer;
	GLuint m_textTextureCoordBuffer;

	/// <summary>
	/// The default clip plane which is used for the normal scene rendering pass.
	/// The variable could be removed if GL_CLIP_DISTANCE0 could be disabled.
	/// </summary>
	const Math::Vector4D m_defaultClipPlane;
	Math::Real m_waterWaveStrength;
	Math::Real m_waterShineDamper;
	Math::Real m_waterReflectivity;
	Math::Real m_waterWaveSpeed;
	Math::Real m_waterMoveFactor;
	//std::vector<GameNode*> m_waterNodes;
	Math::Vector4D m_waterRefractionClippingPlane; // TODO: Consider using Math::Plane instead
	Math::Vector4D m_waterReflectionClippingPlane; // TODO: Consider using Math::Plane instead
	/// <summary>
	/// A map holding the distortion of the water creating a wrippling effect on its surface.
	/// </summary>
	Texture* m_waterDUDVTexture;
	Texture* m_waterNormalMap;
	Texture* m_waterRefractionTexture;
	Texture* m_waterReflectionTexture;
	bool m_waterLightReflectionEnabled;
	/// <summary>
	/// The greater the more reflective water will be.
	/// </summary>
	CONST_IF_TWEAK_BAR_DISABLED Math::Real m_waterFresnelEffectFactor;
	/// <summary>
	/// The greater the more vertical water normal values will be.
	/// </summary>
	CONST_IF_TWEAK_BAR_DISABLED Math::Real m_waterNormalVerticalFactor;
	Shader* m_waterShader;
	Shader* m_waterNoDirectionalLightShader;

	Shader* m_billboardShader;
	//std::vector<GameNode*> m_billboardNodes;

	MappedValues m_mappedValues;

#ifdef ANT_TWEAK_BAR_ENABLED
	unsigned int m_cameraCountMinusOne;
//#ifdef CARTOON_SHADING_ENABLED // cartoon shading is included in the Lighting.glh file (probably commented out)
//	bool cartoonShadingEnabled;
//	Shader* cartoonShader;
//#endif
	size_t m_previousFrameCameraIndex;
	TwBar* m_propertiesBar;
	TwBar* m_cameraBar;
	TwBar* m_lightsBar;
	TwStructMember m_cameraMembers[5];
	TwType m_cameraType;
#endif

#ifdef DEBUG_RENDERING_ENABLED
	std::vector<GuiTexture*> m_guiTextures; // https://www.youtube.com/watch?v=vOmJ1lyiJ4A&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=24
	Mesh* m_debugQuad;
	Shader* m_debugShader;
#endif

#ifdef CALCULATE_RENDERING_STATS
	Math::Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class Renderer */

} /* end namespace Rendering */
