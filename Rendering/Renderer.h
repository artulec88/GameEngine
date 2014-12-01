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

#include "AntTweakBar\include\AntTweakBar.h"

#include <string>
#include <vector>
#include <map>

namespace Rendering
{

class Mesh;

// TODO: Consider creating Singleton template class from which Renderer would inherit
class RENDERING_API Renderer : public MappedValues
{
/* ==================== Static variables begin ==================== */
private:
	//static const int MAX_NUMBER_OF_CAMERAS;
	static const Math::Matrix4D BIAS_MATRIX;
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Renderer(int width, int height, std::string title);
	virtual ~Renderer(void);
private:
	Renderer(const Renderer& renderer) : altCamera(Math::Matrix4D::Identity()) {} // don't implement
	void operator=(const Renderer& renderer) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	//GLFWwindow* GetWindow() const { return this->window; };
	void Render(GameNode& node);
	void SwapBuffers();
	
	inline void AddLight(BaseLight* light);
	inline void AddCamera(Camera* camera);
	inline BaseLight* GetCurrentLight() { return currentLight; }
	inline Math::Vector3D& GetAmbientLight() { return ambientLight; }
	inline Camera& GetCurrentCamera();
	unsigned int GetCurrentCameraIndex() const { return currentCameraIndex; }
	unsigned int NextCamera();
	unsigned int PrevCamera();
	unsigned int SetCurrentCamera(unsigned int cameraIndex);
	void SetCursorPos(Math::Real xPos, Math::Real yPos) { glfwSetCursorPos(window, xPos, yPos); }
	
	unsigned int GetSamplerSlot(const std::string& samplerName) const;
	void UpdateUniformStruct(const Transform& transform, const Material& material, const Shader& shader, const std::string& uniformName, const std::string& uniformType);

	bool IsCloseRequested() const;

	inline Math::Matrix4D GetLightMatrix() const { return lightMatrix; }

	void PrintGlReport();
protected:
	void InitGraphics(int width, int height, const std::string& title);
	void ClearScreen() const;
	inline void SetSamplerSlot(const std::string& name, unsigned int value) { samplerMap[name] = value; }
private:
	void BindAsRenderTarget();
	void InitGlfw(int width, int height, const std::string& title);
	void InitGlew() const;
	void SetGlfwCallbacks();
	void BlurShadowMap(Texture* shadowMap, Math::Real blurAmount);
	void ApplyFilter(Shader* filterShader, Texture* source, Texture* dest);
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	bool applyFilterEnabled;
	Color backgroundColor;
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

	TwBar* propertiesBar;
/* ==================== Non-static member variables end ==================== */
}; /* end class Renderer */

} /* end namespace Rendering */
