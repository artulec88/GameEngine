#pragma once

#include "Rendering.h"
#include "GameNode.h"
#include "Camera.h"
#include "BaseLight.h"
#include "PointLight.h"
#include "MappedValues.h"
#include "Material.h"
#include "Transform.h"
#include "Shader.h"
#include "Color.h"
#include "MenuEntry.h"
#include "CubeShadowMap.h"

#include "Math\Vector.h"

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
	Renderer(const Renderer& renderer) : altCamera(Math::Matrix4D::Identity(), Transform()) {} // don't implement
	void operator=(const Renderer& renderer) {} // don't implement
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

	RENDERING_API GLFWwindow* GetThreadWindow() { return m_threadWindow; }

#ifdef ANT_TWEAK_BAR_ENABLED
	RENDERING_API void InitializeTweakBars();
	RENDERING_API void CheckCameraIndexChange();
#endif
	
	RENDERING_API inline void AddLight(Lighting::BaseLight* light);
	RENDERING_API inline void AddCamera(CameraBase* camera);
	RENDERING_API inline Lighting::BaseLight* GetCurrentLight() { return m_currentLight; }
	RENDERING_API inline Lighting::PointLight* GetPointLight() { return m_pointLight; }
	//RENDERING_API inline Lighting::SpotLight* GetSpotLight() { return m_spotLight; }
	RENDERING_API inline const Math::Vector3D& GetAmbientDayLight() const { return m_ambientDaytimeColor; }
	RENDERING_API inline const Math::Vector3D& GetAmbientNightLight() const { return m_ambientNighttimeColor; }
	
	RENDERING_API inline CameraBase& GetCurrentCamera()
	{
		if (m_currentCamera == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Current camera is NULL");
			exit(EXIT_FAILURE);
		}
		return *m_currentCamera;
	}

	RENDERING_API unsigned int GetCurrentCameraIndex() const { return currentCameraIndex; }
	RENDERING_API unsigned int NextCamera();
	RENDERING_API unsigned int PrevCamera();
	RENDERING_API unsigned int SetCurrentCamera(unsigned int cameraIndex);
	RENDERING_API void SetCursorPos(Math::Real xPos, Math::Real yPos) const { glfwSetCursorPos(m_window, xPos, yPos); }
	
	RENDERING_API unsigned int GetSamplerSlot(const std::string& samplerName) const;
	RENDERING_API void UpdateUniformStruct(const Transform& transform, const Material& material, const Shader& shader, const std::string& uniformName, const std::string& uniformType);

	RENDERING_API inline bool IsCloseRequested() const
	{
		return glfwWindowShouldClose(m_window) != 0;
	}

	RENDERING_API inline Math::Matrix4D GetLightMatrix() const { return lightMatrix; }

	RENDERING_API void PrintGlReport();

	void RequestWindowClose() const { glfwSetWindowShouldClose(m_window, GL_TRUE); }
	void RegisterTerrainNode(GameNode* terrainNode);
	void BindCubeShadowMap(unsigned int textureUnit) const;

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

	inline void SetSamplerSlot(const std::string& name, unsigned int value) { samplerMap[name] = value; }
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
	const Math::Vector3D m_ambientDaytimeColor;
	const Math::Vector3D m_ambientSunNearHorizonColor;
	const Math::Vector3D m_ambientNighttimeColor;
	Math::Vector3D m_ambientLight;
	Lighting::BaseLight* m_currentLight;
	Lighting::PointLight* m_pointLight;

	// TODO: Start refactoring from here

	unsigned int currentCameraIndex;
	CameraBase* m_currentCamera;
	
	/// <summary> The main menu camera. This camera will be used
	/// in the main menu rendering when there are no game cameras set up. </summary>
	CameraBase* m_mainMenuCamera;
	
	Camera altCamera; // alternative camera for shadow mapping, rendering to texture etc.
	Material* filterMaterial;
	Transform filterTransform;
	Mesh* filterMesh;
	Texture* filterTarget;

	GameNode* m_terrainNode;
	Shader* m_defaultShaderTerrain;
	Shader* m_defaultShaderFogEnabledTerrain;

	GameNode* cubeMapNode;
	Shader* cubeMapShader;
	Texture* cubeMapTextureDay;
	Texture* cubeMapTextureNight;

	Shader* m_cubeMapShader; // for use by the point lights
	CubeShadowMap* m_cubeShadowMap; // for use by the point lights
	//Texture* m_cubeShadowMap; // for use by the point lights
	Texture* shadowMaps[SHADOW_MAPS_COUNT];
	Texture* shadowMapTempTargets[SHADOW_MAPS_COUNT];

	Shader* defaultShader;
	Shader* defaultShaderFogEnabled;
	Shader* shadowMapShader;
	Shader* nullFilterShader;
	Shader* gaussBlurFilterShader;
	Shader* fxaaFilterShader;

	std::vector<Lighting::BaseLight*> m_lights;
	std::vector<Lighting::BaseLight*> m_directionalAndSpotLights;
	std::vector<Lighting::PointLight*> m_pointLights;

	std::vector<CameraBase*> cameras;
	std::map<std::string, unsigned int> samplerMap;
	Math::Matrix4D lightMatrix;

	Math::Real fxaaSpanMax;
	Math::Real fxaaReduceMin;
	Math::Real fxaaReduceMul;

	Texture* fontTexture;
	TextRenderer* textRenderer;

#ifdef ANT_TWEAK_BAR_ENABLED
	unsigned int m_cameraCountMinusOne;
//#ifdef CARTOON_SHADING_ENABLED // cartoon shading is included in the Lighting.glh file (probably commented out)
//	bool cartoonShadingEnabled;
//	Shader* cartoonShader;
//#endif
	unsigned int previousFrameCameraIndex;
	TwBar* propertiesBar;
	TwBar* cameraBar;
	TwBar* lightsBar;
	TwStructMember cameraMembers[5];
	TwType cameraType;
	bool renderToTextureTestingEnabled;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class Renderer */

} /* end namespace Rendering */
