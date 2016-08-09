#ifndef __PLAY_GAME_STATE_H__
#define __PLAY_GAME_STATE_H__

#include "GameStats.h"

#include "Engine\GameManager.h"
#include "Engine\GameState.h"
#include "Engine\IInputableKeyboard.h"
#include "Engine\IInputableMouse.h"
#include "Engine\IUpdateable.h"
#include "Engine\MousePicker.h"

#include "Rendering\Shader.h"
#include "Rendering\Renderer.h"

#include "Math\Math.h"
#ifdef CALCULATE_GAME_STATS
#include "Math\IStatisticsStorage.h"
#endif

#include <string>

namespace Game
{

	class PlayGameState : public Engine::GameState, public Engine::Input::IInputableMouse, public Engine::IUpdateable
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		PlayGameState(Engine::GameManager* gameManager, const std::string& inputMappingContextName);
		virtual ~PlayGameState(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Entered();
		virtual void Leaving();
		virtual void Obscuring();
		virtual void Revealed();
		virtual void Handle(Engine::Actions::Action action);
		virtual void Handle(Engine::States::State state);
		virtual void Handle(Engine::Ranges::Range range, Math::Real value);

		bool IsInGameTimeCalculationEnabled() const { return true; }

		virtual void MouseButtonEvent(int button, int action, int mods);
		virtual void MousePosEvent(double xPos, double yPos);
		virtual void ScrollEvent(double xOffset, double yOffset);
		virtual void Render(Rendering::Renderer* renderer) const;
		virtual void Update(Math::Real elapsedTime);
	private:
		void RenderSceneWithAmbientLight(Rendering::Renderer* renderer) const;
		void RenderSceneWithPointLights(Rendering::Renderer* renderer) const;
		void RenderSceneWithDirectionalAndSpotLights(Rendering::Renderer* renderer) const;
		void RenderWaterNodes(Rendering::Renderer* renderer) const;
		void RenderSkybox(Rendering::Renderer* renderer) const;

		/// <summary>
		/// Water textures (reflection, refraction) rendering pass.
		/// </summary>
		/// <remarks>
		/// The implementation is based on the tutorial: https://www.youtube.com/watch?v=0NH9k4zTAqk.
		/// </remarks>
		void RenderWaterTextures(Rendering::Renderer* renderer) const;
		void RenderWaterReflectionTexture(Rendering::Renderer* renderer) const;
		void RenderWaterRefractionTexture(Rendering::Renderer* renderer) const;
		void RenderBillboardNodes(Rendering::Renderer* renderer) const;
		void RenderParticles(Rendering::Renderer* renderer) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		//Rendering::GameNode& m_rootGameNode;
		bool m_isMouseLocked;
		Engine::GameManager* m_gameManager;
		Engine::MousePicker m_mousePicker;
#ifdef CALCULATE_GAME_STATS
		Math::Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PlayGameState */

} /* end namespace Game */

#endif /* __PLAY_GAME_STATE_H__ */
