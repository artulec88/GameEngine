#pragma once

#include "Rendering\Game.h"
#include "Rendering\GameNode.h"
#include "Rendering\Shader.h"

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

	virtual void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
/* ==================== Non-static member variables end ==================== */
};

