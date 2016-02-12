#ifndef __GAME_TEST_GAME_MANAGER_H__
#define __GAME_TEST_GAME_MANAGER_H__

#include <Rendering\GameManager.h>
#include <Rendering\GameNode.h>
#include <Rendering\DirectionalLight.h>
#include <Rendering\PointLight.h>
#include <Rendering\SpotLight.h>

#include "GameStats.h"
#ifdef CALCULATE_GAME_STATS
#include "Math\IStatisticsStorage.h"
#endif

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
	virtual void Update(Math::Real delta);

	virtual Math::Real GetLoadingProgress() const;

	virtual void WindowResizeEvent(int width, int height);
	virtual void KeyEvent(int key, int scancode, int action, int mods);
	virtual void MouseButtonEvent(int button, int action, int mods);
	virtual void MousePosEvent(double xPos, double yPos);
#ifdef ANT_TWEAK_BAR_ENABLED
	virtual void InitializeTweakBars();
#endif
private:
	void AddLights();
	void AddDirectionalLight();
	void AddPointLights();
	void AddSpotLights();
	void AddCameras(Rendering::GameNode* entityToFollow);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	const int RESOURCES_TO_LOAD;
	const Math::Real CAMERA_HEIGHT_UPDATE_INTERVAL;
	int m_resourcesLoaded;
	
	Rendering::TerrainMesh* m_terrainMesh;
	Math::Real m_timeToUpdateCameraHeight;
	
	Rendering::GameNode* m_boxNode;
#ifdef ANT_TWEAK_BAR_ENABLED
	Rendering::Material* terrainMaterial;
	Rendering::Material* boxMaterial;

	Math::Real terrainSpecularIntensity, terrainSpecularPower;
	Math::Real terrainDisplacementScale, terrainDisplacementOffset;
#endif

	const int HUMAN_NODES_COUNT;
	Rendering::GameNode** humanNodes;
	const int pointLightCount;
	const int spotLightCount;
	const int cameraCount;
	Rendering::GameNode** cameraNodes;
	bool m_heightMapCalculationEnabled;
	//Rendering::GameNode* castleNode;
#ifdef CALCULATE_GAME_STATS
	Math::Statistics::ClassStats& m_classStats;
#endif

/* ==================== Non-static member variables end ==================== */
}; /* end class TestGameManager */

} /* end namespace Game */

#endif /* __GAME_TEST_GAME_MANAGER_H__ */