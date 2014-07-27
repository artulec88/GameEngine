#pragma once

#include "Rendering.h"
#include "GameNode.h"
#include "Camera.h"
#include "BaseLight.h"
#include "MappedValues.h"
#include "Material.h"
#include "Transform.h"
#include "Shader.h"
//#include "Utility\Singleton.h"

#include "Math\Vector.h"

#include <string>
#include <vector>
#include <map>

namespace Rendering
{

// TODO: Consider creating Singleton template class from which Renderer would inherit
class RENDERING_API Renderer : public MappedValues
{
/* ==================== Static variables begin ==================== */
private:
	//static const int MAX_NUMBER_OF_CAMERAS;
/* ==================== Static variables end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	GLFWwindow* window;
	GLuint framebuffer;
	GLuint vao; // vertex array id
	bool isFullscreen;
	bool isMouseEnabled;
	Math::Vector3D ambientLight;
	BaseLight* currentLight;
	unsigned int currentCameraIndex;
	Camera* currentCamera;
	
	Shader* defaultShader;
	std::vector<BaseLight*> lights;
	std::vector<Camera*> cameras;
	std::map<std::string, unsigned int> samplerMap;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Renderer(int width, int height, std::string title);
	virtual ~Renderer(void);
private:
	Renderer(const Renderer& renderer) {} // don't implement
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
	unsigned int NextCamera();
	unsigned int PrevCamera();
	unsigned int SetCurrentCamera(unsigned int cameraIndex);
	void SetCursorPos(Math::Real xPos, Math::Real yPos) { glfwSetCursorPos(window, xPos, yPos); }
	
	unsigned int GetSamplerSlot(const std::string& samplerName) { /* TODO: Add assertions and checks */ return samplerMap[samplerName]; }
	void UpdateUniformStruct(const Transform& transform, const Material& material, Shader* shader, const std::string& uniformName, const std::string& uniformType);

	bool IsCloseRequested() const;

	std::string GetOpenGLVersion();
protected:
	void Init(int width, int height, std::string title);
	void InitGraphics();
	void InitGlew() const;
	void ClearScreen() const;
private:
	void BindAsRenderTarget();
/* ==================== Non-static, non-virtual member functions end ==================== */
}; /* end class Renderer */

} /* end namespace Rendering */
