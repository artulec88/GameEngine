#pragma once

#include "Rendering\Game.h"

class TestGame2 : public Rendering::Game
{
/* ==================== Constructors and destructors begin ==================== */
public:
	TestGame2();
	virtual ~TestGame2(void);
private:
	TestGame2(TestGame2& game);
	void operator=(TestGame2& game);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Init();
	virtual void Input();
	virtual void Update();

	virtual void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Rendering::Mesh* mesh;
	Rendering::Shader* shader;
	Rendering::Transform* transform;
	Rendering::Material* material;
	Rendering::PointLight* pointLights;
	Rendering::SpotLight* spotLights;
/* ==================== Non-static member variables end ==================== */
};

