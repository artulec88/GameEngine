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
//#include "Utility\Singleton.h"

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

// TODO: Consider creating Singleton template class from which Renderer would inherit
class Renderer : public MappedValues
{
/* ==================== Static variables begin ==================== */
private:
	//static const int MAX_NUMBER_OF_CAMERAS;
	static const Math::Matrix4D BIAS_MATRIX;
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API Renderer(int width, int height, std::string title);
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
	RENDERING_API inline Math::Vector3D& GetAmbientLight() { return ambientLight; }
	RENDERING_API inline Camera& GetCurrentCamera();
	RENDERING_API unsigned int GetCurrentCameraIndex() const { return currentCameraIndex; }
	RENDERING_API unsigned int NextCamera();
	RENDERING_API unsigned int PrevCamera();
	RENDERING_API unsigned int SetCurrentCamera(unsigned int cameraIndex);
	RENDERING_API void SetCursorPos(Math::Real xPos, Math::Real yPos) { glfwSetCursorPos(window, xPos, yPos); }
	
	RENDERING_API unsigned int GetSamplerSlot(const std::string& samplerName) const;
	RENDERING_API void UpdateUniformStruct(const Transform& transform, const Material& material, const Shader& shader, const std::string& uniformName, const std::string& uniformType);

	RENDERING_API bool IsCloseRequested() const;

	RENDERING_API inline Math::Matrix4D GetLightMatrix() const { return lightMatrix; }

	RENDERING_API void PrintGlReport();
protected:
	void InitGraphics(int width, int height, const std::string& title);
	void ClearScreen() const;
	inline void SetSamplerSlot(const std::string& name, unsigned int value) { samplerMap[name] = value; }
private:
	void BindAsRenderTarget();
	void InitGlfw(int width, int height, const std::string& title);
	void InitGlew() const;
	void SetCallbacks();
	void BlurShadowMap(Texture* shadowMap, Math::Real blurAmount);
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
	bool isFullscreen;
	bool isMouseEnabled;
	Math::Vector3D ambientLight;
	BaseLight* currentLight;
	unsigned int currentCameraIndex;
	Camera* currentCamera;
	
	Camera	altCamera; // alternative camera for shadow mapping, rendering to texture etc.
	GameNode* altCameraNode;
	Material* planeMaterial;
	Transform planeTransform;
	Mesh* planeMesh;
	Texture* tempTarget;
	
	Shader* defaultShader;
	Shader* shadowMapShader;
	Shader* nullFilterShader;
	Shader* gaussBlurFilterShader;
	int shadowMapWidth;
	int shadowMapHeight;
	std::vector<BaseLight*> lights;
	std::vector<Camera*> cameras;
	std::map<std::string, unsigned int> samplerMap;
	Math::Matrix4D lightMatrix;

#ifdef ANT_TWEAK_BAR_ENABLED
	unsigned int previousFrameCameraIndex;
	TwBar* propertiesBar;
	TwBar* cameraBar;
	TwStructMember cameraMembers[5];
	TwType cameraType;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class Renderer */

} /* end namespace Rendering */
