#pragma once

#include "Rendering.h"
#include "GameNode.h"
#include "Camera.h"
#include "BaseLight.h"
#include "MappedValues.h"
#include "Material.h"
#include "Transform.h"
#include "Shader.h"
#include "Color.h"

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
public:
	enum RendererState
	{
		RENDERING_MENU,
		RENDERING_GAME
	};

/* ==================== Static variables begin ==================== */
private:
	static const int SHADOW_MAPS_COUNT = 11;
	static const Math::Matrix4D BIAS_MATRIX;
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API Renderer(GLFWwindow* window);
	RENDERING_API virtual ~Renderer(void);
private:
	Renderer(const Renderer& renderer) : altCamera(Math::Matrix4D::Identity()) {} // don't implement
	void operator=(const Renderer& renderer) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	//GLFWwindow* GetWindow() const { return this->window; };
	RENDERING_API void Render(GameNode& node);
	RENDERING_API void SwapBuffers();

#ifdef ANT_TWEAK_BAR_ENABLED
	RENDERING_API void InitializeTweakBars();
	RENDERING_API void CheckCameraIndexChange();
#endif
	
	RENDERING_API inline void AddLight(BaseLight* light);
	RENDERING_API inline void AddCamera(Camera* camera);
	RENDERING_API inline BaseLight* GetCurrentLight() { return currentLight; }
	RENDERING_API inline Math::Vector3D& GetAmbientLight() { return m_ambientLight; }
	RENDERING_API inline const Math::Vector3D& GetAmbientDayLight() const { return m_ambientDaytimeColor; }
	RENDERING_API inline const Math::Vector3D& GetAmbientNightLight() const { return m_ambientNighttimeColor; }
	RENDERING_API inline Camera& GetCurrentCamera();
	RENDERING_API unsigned int GetCurrentCameraIndex() const { return currentCameraIndex; }
	RENDERING_API unsigned int NextCamera();
	RENDERING_API unsigned int PrevCamera();
	RENDERING_API unsigned int SetCurrentCamera(unsigned int cameraIndex);
	RENDERING_API void SetCursorPos(Math::Real xPos, Math::Real yPos) const { glfwSetCursorPos(window, xPos, yPos); }
	
	RENDERING_API unsigned int GetSamplerSlot(const std::string& samplerName) const;
	RENDERING_API void UpdateUniformStruct(const Transform& transform, const Material& material, const Shader& shader, const std::string& uniformName, const std::string& uniformType);

	RENDERING_API bool IsCloseRequested() const;

	RENDERING_API inline Math::Matrix4D GetLightMatrix() const { return lightMatrix; }

	RENDERING_API void PrintGlReport();
protected:
	void RenderSkybox();
	void SetCallbacks();
	void ClearScreen() const;
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
	unsigned int cameraCount;
	bool applyFilterEnabled;
	Color backgroundColor;
	bool shadowsEnabled;
	GLFWwindow* window;
	GLuint framebuffer;
	GLuint vao; // vertex array id
	bool isMouseEnabled;
	bool ambientLightFogEnabled;
	Math::Vector3D ambientLightFogColor;
	Math::Real ambientLightFogStart;
	Math::Real ambientLightFogEnd;
	const Math::Vector3D m_ambientDaytimeColor;
	const Math::Vector3D m_ambientSunNearHorizonColor;
	const Math::Vector3D m_ambientNighttimeColor;
	Math::Vector3D m_ambientLight;
	BaseLight* currentLight;
	unsigned int currentCameraIndex;
	Camera* currentCamera;
	
	Camera	altCamera; // alternative camera for shadow mapping, rendering to texture etc.
	GameNode* altCameraNode;
	Material* planeMaterial;
	Transform planeTransform;
	Mesh* planeMesh;
	Texture* tempTarget;

	GameNode* cubeMapNode;
	Shader* cubeMapShader;
	Texture* cubeMapTextureDay;
	Texture* cubeMapTextureNight;

	Texture* shadowMaps[SHADOW_MAPS_COUNT];
	Texture* shadowMapTempTargets[SHADOW_MAPS_COUNT];

	Shader* defaultShader;
	Shader* defaultShaderFogEnabled;
	Shader* shadowMapShader;
	Shader* nullFilterShader;
	Shader* gaussBlurFilterShader;
	Shader* fxaaFilterShader;
	std::vector<BaseLight*> lights;
	std::vector<Camera*> cameras;
	std::map<std::string, unsigned int> samplerMap;
	Math::Matrix4D lightMatrix;

	Math::Real fxaaSpanMax;
	Math::Real fxaaReduceMin;
	Math::Real fxaaReduceMul;

	Texture* fontTexture;
	TextRenderer* textRenderer;

#ifdef ANT_TWEAK_BAR_ENABLED
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
