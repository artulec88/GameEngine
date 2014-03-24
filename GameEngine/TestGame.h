#pragma once

#include "Rendering\Game.h"

class TestGame : public Rendering::Game
{
/* ==================== Constructors and destructors begin ==================== */
public:
	TestGame();
	virtual ~TestGame(void);
private:
	TestGame(TestGame& game);
	void operator=(TestGame& game);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Init();
	virtual void Input();
	virtual void Update();
	virtual void Render();

	virtual void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Rendering::Mesh* mesh;
	Rendering::Shader* shader;
	Rendering::Transform* transform;
	Rendering::Material* material;
	Rendering::Camera* camera;
	Rendering::PointLight* pointLights;
	Rendering::SpotLight* spotLights;
/* ==================== Non-static member variables end ==================== */
};

