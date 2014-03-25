#pragma once

#include "Rendering.h"
#include "GameNode.h"
#include "Camera.h"
//#include "Utility\Singleton.h"

#include <string>

namespace Rendering
{

// TODO: Consider creating Singleton template class from which Renderer would inherit
class RENDERING_API Renderer
{
/* ==================== Non-static member variables begin ==================== */
private:
	static const int MAX_NUMBER_OF_CAMERAS;
/* ==================== Non-static member variables end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	const int camerasCount;
	int currentCameraIndex;
	Camera* cameras;
	GLFWwindow* window;
	GLuint vao; // vertex array id
	bool isFullscreen;
	bool isMouseEnabled;
/* ==================== Non-static member variables end ==================== */

public: /* constructors and destructors */
	Renderer(int width, int height, std::string title, unsigned short camerasCount = 3);
	~Renderer(void);

public: /* Non-static, non-virtual member functions */
	//GLFWwindow* GetWindow() const { return this->window; };
	void Render(GameNode& node);
	void SwapBuffers();
	
	inline Camera& GetCurrentCamera() { return cameras[currentCameraIndex]; }
	inline void SetCurrentCamera(const Camera& camera) { cameras[currentCameraIndex] = camera; }
	void NextCamera();
	void PrevCamera();

	std::string GetOpenGLVersion();
protected:
	void SetCurrentCameraIndex(int cameraIndex);
	void Init(int width, int height, std::string title);
	void InitGraphics();
	void InitGlew() const;
	void ClearScreen() const;
public: /* public virtual member functions */
public: /* public static functions */
}; /* end class Renderer */

} /* end namespace Rendering */
