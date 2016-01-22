#ifndef __GAME_DEBUG_GAME_MANAGER_H__
#define __GAME_DEBUG_GAME_MANAGER_H__

#include <Rendering\GameManager.h>
#include <Rendering\GameNode.h>

#include "GameStats.h"
#ifdef CALCULATE_GAME_STATS
#include "Math\IStatisticsStorage.h"
#endif

namespace Game
{

class DebugGameManager : public Rendering::GameManager
{
/* ==================== Constructors and destructors begin ==================== */
public:
	DebugGameManager();
	virtual ~DebugGameManager(void);
private:
	DebugGameManager(DebugGameManager& gameManager);
	void operator=(DebugGameManager& gameManager);
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
	void AddCameras();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	const int M_RESOURCES_TO_LOAD;
	int m_resourcesLoaded;

	const int m_cameraCount;
	Rendering::GameNode** m_cameraNodes;
#ifdef CALCULATE_GAME_STATS
	Math::Statistics::ClassStats& m_classStats;
#endif

/* ==================== Non-static member variables end ==================== */
}; /* end class DebugGameManager */

} /* end namespace Game */

#endif /* __GAME_DEBUG_GAME_MANAGER_H__ */