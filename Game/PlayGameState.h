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
#include "Rendering\Terrain.h"

#include "Math\Math.h"
#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math\StatisticsStorage.h"
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
		void AddShaders();
		void AddTerrainNode();
		void AddWaterNodes();
		void AddSkyboxNode();
		void AddPlayerNode();
		void AddBillboards(unsigned int billboardsCount, Rendering::Material* billboardsMaterial);
		void AddCameras();
		void AddLights();
		void AddDirectionalLight();
		void AddPointLights();
		void AddSpotLights();

		unsigned int SetCurrentCamera(unsigned int cameraIndex);
		unsigned int NextCamera();
		unsigned int PrevCamera();

		int GetAmbientShaderID(const Rendering::FogEffect::FogInfo& fogInfo) const;
		int GetAmbientTerrainShaderID(const Rendering::FogEffect::FogInfo& fogInfo) const;
		int GetWaterShaderID(Rendering::Renderer* renderer) const
		{
			return ((m_directionalLightsCount > 0) && (renderer->IsWaterLightReflectionEnabled())) ?
				Rendering::ShaderIDs::WATER : Rendering::ShaderIDs::WATER_NO_DIRECTIONAL_LIGHT;
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
			return static_cast<int>(m_spotLights.size());
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
		Engine::GameNode m_rootGameNode;
		Engine::GameNode m_terrainNode;
		Rendering::Terrain* m_terrain;
		Rendering::Material* m_terrainMaterial;

		Engine::GameNode m_waterNode;
		Engine::GameNode m_skyboxNode;
		Engine::GameNode m_playerNode;
		std::vector<Engine::GameNode> m_nodes;
		std::vector<Engine::GameNode> m_billboardsNodes;
		
		std::vector<Rendering::BaseCamera*> m_cameras;
		unsigned int m_currentCameraIndex;
		//std::vector<Engine::GameNode> m_cameraNodes;
		Engine::GameNode m_camerasNode;

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

		// TODO: We should store each type of available light in different vector. The PlayGameState should not know about possible light types.
		// Program to interface (BaseLight) instead of concrete implementations (DirectionalLight, PointLight, SpotLight, ...).
		/// <summary> The number of directional lights currently being used in the game. </summary>
		int m_directionalLightsCount;
		/// <summary> The vector of all lights that are used by the game engine. </summary>
		//std::vector<Rendering::Lighting::BaseLight*> m_lights;
		/// <summary> The vector of directional lights that are used by the game engine. </summary>
		std::vector<Rendering::Lighting::DirectionalLight> m_directionalLights;
		/// <summary> The vector of spot lights that are used by the game engine. </summary>
		std::vector<Rendering::Lighting::SpotLight> m_spotLights;
		/// <summary> The vector of point lights that are used by the game engine. </summary>
		std::vector<Rendering::Lighting::PointLight> m_pointLights;
		//std::vector<Lighting::SpotLight*> m_spotLights;

		Math::Vector2D m_previousMousePos, m_mousePos;
		bool m_mousePosChanged;

#ifdef PROFILING_GAME_MODULE_ENABLED
		Math::Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PlayGameState */

} /* end namespace Game */

#endif /* __PLAY_GAME_STATE_H__ */
