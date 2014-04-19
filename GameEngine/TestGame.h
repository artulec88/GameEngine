#pragma once

#include "Rendering\Game.h"
#include "Rendering\GameNode.h"
#include "Rendering\Shader.h"
#include "Rendering\DirectionalLight.h"
#include "Rendering\PointLight.h"
#include "Rendering\SpotLight.h"

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
	virtual void Input(Math::Real delta);
	virtual void Update(Math::Real delta);

	virtual void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	virtual void MousePosEvent(GLFWwindow* window, double xPos, double yPos);

private:
	void AddPointLights();
	void AddSpotLights();
	void AddCameras();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	const int humanCount;
	Rendering::GameNode** humanNodes;
	Rendering::GameNode* directionalLightNode;
	const int pointLightCount;
	Rendering::GameNode** pointLightNodes;
	const int spotLightCount;
	Rendering::GameNode** spotLightNodes;
	const int cameraCount;
	Rendering::GameNode** cameraNodes;
	unsigned int currentCameraIndex; // TODO: this value should always be the same as the Renderer::currentCameraIndex. Match them.
/* ==================== Non-static member variables end ==================== */
};

