#ifndef __PLAY_GAME_STATE_H__
#define __PLAY_GAME_STATE_H__

#include "Def.h"

#include "Engine\GameManager.h"
#include "Engine\GameState.h"
#include "Engine\IInputableKeyboard.h"
#include "Engine\IInputableMouse.h"
#include "Engine\IUpdateable.h"
#include "Engine\MousePicker.h"

#include "Rendering\Shader.h"
#include "Rendering\Renderer.h"

#include "Math\Math.h"
#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math\IStatisticsStorage.h"
#include "Math\Statistics.h"
#endif

#include <string>

#define DRAW_GAME_TIME
#define SIMULATE_SUN_BEHAVIOR // TODO: Move all #define's to one place

namespace Game
{

	class PlayGameState : public Engine::GameState, public Engine::Input::IInputableMouse, public Engine::IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
	private:
		static constexpr Math::Real TROPIC_OF_CANCER_SINUS = static_cast<Math::Real>(0.39794863131);
		/* ==================== Static variables and functions end ==================== */

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

		virtual void MouseButtonEvent(int button, int action, int mods);
		virtual void MousePosEvent(double xPos, double yPos);
		virtual void ScrollEvent(double xOffset, double yOffset);
		virtual void Render(Rendering::Renderer* renderer) const;
		virtual void Update(Math::Real elapsedTime);
	private:
		void AddLights();
		void AddShaders();
		void AddDirectionalLight();
		void AddPointLights();
		void AddSpotLights();

		const Rendering::Shader& GetAmbientShader(const Rendering::FogEffect::FogInfo& fogInfo) const;
		const Rendering::Shader& GetAmbientTerrainShader(const Rendering::FogEffect::FogInfo& fogInfo) const;
		const Rendering::Shader& GetWaterShader(Rendering::Renderer* renderer) const
		{
			return ((m_directionalLightsCount > 0) && (renderer->IsWaterLightReflectionEnabled())) ?
				m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::WATER) :
				m_gameManager->GetShaderFactory().GetShader(Engine::ShaderTypes::WATER_NO_DIRECTIONAL_LIGHT);
		}

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

		//void AdjustAmbientLightAccordingToCurrentTime();

		inline int GetSpotLightsCount() const
		{
			return static_cast<int>(m_directionalAndSpotLights.size()) - m_directionalLightsCount;
		}

		/// <summary>
		/// Calculates the elevation and azimuth angles for the directional light (sun).
		/// </summary>
		/// <remarks>
		/// See http://pveducation.org/pvcdrom/properties-of-sunlight/sun-position-calculator for details.
		/// </remarks>
		void CalculateSunElevationAndAzimuth();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		//Rendering::GameNode& m_rootGameNode;
		bool m_isMouseLocked;
		Engine::GameManager* m_gameManager;
		Engine::MousePicker m_mousePicker;
		CONST_IF_TWEAK_BAR_DISABLED Math::Real m_clockSpeed;
		Utility::Timing::DateTime m_inGameDateTime;
#ifdef DRAW_GAME_TIME
		mutable Rendering::Controls::GuiButtonControl m_inGameTimeGuiButton;
#endif

		Math::Real m_dayNightMixFactor;

		Rendering::Color m_ambientDaytimeColor;
		Rendering::Color m_ambientSunNearHorizonColor;
		Rendering::Color m_ambientNighttimeColor;
		Rendering::Color m_ambientLightColor;

		/// <summary> The number of directional lights currently being used in the game. </summary>
		int m_directionalLightsCount;
		/// <summary> The vector of all lights that are used by the game engine. </summary>
		std::vector<Rendering::Lighting::BaseLight*> m_lights;
		/// <summary> The vector of directional and spot lights that are used by the game engine. </summary>
		std::vector<Rendering::Lighting::BaseLight*> m_directionalAndSpotLights;
		/// <summary> The vector of point lights that are used by the game engine. </summary>
		std::vector<Rendering::Lighting::PointLight*> m_pointLights;
		//std::vector<Lighting::SpotLight*> m_spotLights;

#ifdef PROFILING_GAME_MODULE_ENABLED
		Math::Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PlayGameState */

} /* end namespace Game */

#endif /* __PLAY_GAME_STATE_H__ */
