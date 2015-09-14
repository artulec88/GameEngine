#pragma once

#include <Rendering\GameManager.h>
#include <Rendering\GameNode.h>
#include <Rendering\Shader.h>
#include <Rendering\DirectionalLight.h>
#include <Rendering\PointLight.h>
#include <Rendering\SpotLight.h>

namespace Game
{

class TestGameManager : public Rendering::GameManager
{
/* ==================== Constructors and destructors begin ==================== */
public:
	TestGameManager();
	virtual ~TestGameManager(void);
private:
	TestGameManager(TestGameManager& gameManager);
	void operator=(TestGameManager& gameManager);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Load();
	virtual void Input(Math::Real delta);
	virtual void Update(Math::Real delta);

	virtual Math::Real GetLoadingProgress() const;

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
	const int m_resourcesToLoad;
	int m_resourcesLoaded;
	
	Rendering::TerrainMesh* terrainMesh;
	Rendering::Material* terrainMaterial;
	const Math::Real CAMERA_HEIGHT_UPDATE_INTERVAL;
	Math::Real timeToUpdateCameraHeight;
	
	Rendering::GameNode* boxNode;
#ifdef ANT_TWEAK_BAR_ENABLED
	Rendering::Material* boxMaterial;

	Math::Real terrainSpecularIntensity, terrainSpecularPower;
	Math::Real terrainDisplacementScale, terrainDisplacementOffset;
#endif

	const int humanCount;
	Rendering::GameNode** humanNodes;
	const int pointLightCount;
	const int spotLightCount;
	const int cameraCount;
	Rendering::GameNode** cameraNodes;
	bool m_heightMapCalculationEnabled;
	//Rendering::GameNode* castleNode;
#ifdef CALCULATE_STATS
	mutable Math::Statistics::ClassStats& m_classStats;
#endif

/* ==================== Non-static member variables end ==================== */
}; /* end class TestGameManager */

} /* end namespace Game */