#pragma once

#include <Rendering\Game.h>
#include <Rendering\GameNode.h>
#include <Rendering\Shader.h>
#include <Rendering\DirectionalLight.h>
#include <Rendering\PointLight.h>
#include <Rendering\SpotLight.h>

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

	virtual void WindowResizeEvent(GLFWwindow* window, int width, int height);
	virtual void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	virtual void MousePosEvent(GLFWwindow* window, double xPos, double yPos);
#ifdef ANT_TWEAK_BAR_ENABLED
	virtual void InitializeTweakBars();
#endif
private:
	void AddLights();
	void AddDirectionalLight();
	void AddPointLights();
	void AddSpotLights();
	void AddCameras();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Rendering::GameNode* planeNode;
#ifdef ANT_TWEAK_BAR_ENABLED
	Rendering::Material* planeMaterial;

	Math::Real planeSpecularIntensity, planeSpecularPower;
	Math::Real planeDisplacementScale, planeDisplacementOffset;
#endif

	const int humanCount;
	Rendering::GameNode** humanNodes;
	Rendering::GameNode* directionalLightNode;
	const int pointLightCount;
	Rendering::GameNode** pointLightNodes;
	const int spotLightCount;
	Rendering::GameNode** spotLightNodes;
	const int cameraCount;
	Rendering::GameNode** cameraNodes;
	//Rendering::GameNode* castleNode;
/* ==================== Non-static member variables end ==================== */
};

