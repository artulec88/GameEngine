#pragma once

#include "Rendering.h"
#include "GameNode.h"
//#include "Utility\Singleton.h"

#include <string>

namespace Rendering
{

// TODO: Consider creating Singleton template class from which Renderer would inherit
class RENDERING_API Renderer
{
/* ==================== Non-static member variables begin ==================== */
private:
	GLFWwindow* window;
	GLuint vao; // vertex array id
	bool isFullscreen;
	bool isMouseEnabled;
/* ==================== Non-static member variables end ==================== */

public: /* constructors and destructors */
	Renderer(int width, int height, std::string title);
	~Renderer(void);

public: /* Non-static, non-virtual member functions */
	//GLFWwindow* GetWindow() const { return this->window; };
	void InitGraphics();
	void Render(GameNode& node);
	void SwapBuffers();

	std::string GetOpenGLVersion();
protected:
	void Init(int width, int height, std::string title);
	void InitGlew() const;
public: /* public virtual member functions */
public: /* public static functions */
}; /* end class Renderer */

} /* end namespace Rendering */
