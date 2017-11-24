#ifndef __PLAY_GAME_STATE_H__
#define __PLAY_GAME_STATE_H__

#include "Def.h"

#include "Engine/GameManager.h"
#include "Engine/GameState.h"
//#include "Engine/IInputableKeyboard.h"
#include "Engine/IInputableMouse.h"
#include "Engine/IUpdateable.h"
#include "Engine/MousePicker.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderIDs.h"
#include "Rendering/Renderer.h"
#include "Rendering/Terrain.h"
#include "Rendering/DirectionalLight.h"
#include "Rendering/SpotLight.h"

#include "Math/Math.h"
#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math/StatisticsStorage.h"
#include "Math/Statistics.h"
#endif

#include <string>

#define DRAW_GAME_TIME
#define SIMULATE_SUN_BEHAVIOR // TODO: Move all #define's to one place

namespace Game
{

	class PlayGameState : public engine::GameState, public engine::Input::IInputableMouse, public engine::IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		static constexpr math::Real TROPIC_OF_CANCER_SINUS = static_cast<math::Real>(0.39794863131);
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		PlayGameState(engine::GameManager* gameManager, const std::string& inputMappingContextName);
		virtual ~PlayGameState();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Entered() override;
		void Leaving() override;
		void Obscuring() override;
		void Revealed() override;
		void Handle(engine::Actions::Action action) override;
		void Handle(engine::States::State state) override;
		void Handle(engine::Ranges::Range range, math::Real value) override;

		void MouseButtonEvent(int button, int action, int mods) override;
		void MousePosEvent(double xPos, double yPos) override;
		void ScrollEvent(double xOffset, double yOffset) override;
		void Render(rendering::Renderer* renderer) const override;
		void Update(math::Real elapsedTime) override;
	private:
		void AddShaders();
		void AddTerrainNode();
		void AddWaterNodes();
		void AddSkyboxNode();
		void AddPlayerNode();
		void AddBillboards(unsigned int billboardsCount, rendering::Material* billboardsMaterial);
		void AddCameras();
		void AddLights();
		void AddDirectionalLight();
		void AddPointLights();
		void AddSpotLights();

		unsigned int SetCurrentCamera(unsigned int cameraIndex);
		unsigned int NextCamera();
		unsigned int PrevCamera();

		int GetAmbientShaderID(const rendering::fog_effect::FogInfo& fogInfo) const;
		int GetAmbientTerrainShaderID(const rendering::fog_effect::FogInfo& fogInfo) const;
		int GetWaterShaderID(rendering::Renderer* renderer) const
		{
			return ((m_directionalLightsCount > 0) && (renderer->IsWaterLightReflectionEnabled())) ?
				rendering::shader_ids::WATER : rendering::shader_ids::WATER_NO_DIRECTIONAL_LIGHT;
		}

		void RenderSceneWithAmbientLight(rendering::Renderer* renderer) const;
		void RenderSceneWithPointLights(rendering::Renderer* renderer) const;
		void RenderSceneWithDirectionalAndSpotLights(rendering::Renderer* renderer) const;
		void RenderWaterNodes(rendering::Renderer* renderer) const;
		void RenderSkybox(rendering::Renderer* renderer) const;

		/// <summary>
		/// Water textures (reflection, refraction) rendering pass.
		/// </summary>
		/// <remarks>
		/// The implementation is based on the tutorial: https://www.youtube.com/watch?v=0NH9k4zTAqk.
		/// </remarks>
		void RenderWaterTextures(rendering::Renderer* renderer) const;
		void RenderWaterReflectionTexture(rendering::Renderer* renderer) const;
		void RenderWaterRefractionTexture(rendering::Renderer* renderer) const;
		void RenderBillboardNodes(rendering::Renderer* renderer) const;
		void RenderParticles(rendering::Renderer* renderer) const;

		//void AdjustAmbientLightAccordingToCurrentTime();

		int GetSpotLightsCount() const
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
		engine::GameNode m_rootGameNode;
		engine::GameNode m_terrainNode;
		rendering::Terrain* m_terrain;
		rendering::Material* m_terrainMaterial;

		engine::GameNode m_waterNode;
		engine::GameNode m_skyboxNode;
		engine::GameNode m_playerNode;
		std::vector<engine::GameNode> m_nodes;
		std::vector<engine::GameNode> m_billboardsNodes;
		
		std::vector<rendering::BaseCamera*> m_cameras;
		unsigned int m_currentCameraIndex;
		//std::vector<Engine::GameNode> m_cameraNodes;
		engine::GameNode m_camerasNode;

		bool m_isMouseLocked;
		engine::GameManager* m_gameManager;
		engine::MousePicker m_mousePicker;
		CONST_IF_TWEAK_BAR_DISABLED math::Real m_clockSpeed;
		utility::timing::DateTime m_inGameDateTime;
#ifdef DRAW_GAME_TIME
		mutable rendering::controls::GuiButtonControl m_inGameTimeGuiButton;
#endif

		math::Real m_dayNightMixFactor;

		rendering::Color m_ambientDaytimeColor;
		rendering::Color m_ambientSunNearHorizonColor;
		rendering::Color m_ambientNighttimeColor;
		rendering::Color m_ambientLightColor;

		// TODO: We should store each type of available light in different vector. The PlayGameState should not know about possible light types.
		// Program to interface (BaseLight) instead of concrete implementations (DirectionalLight, PointLight, SpotLight, ...).
		/// <summary> The number of directional lights currently being used in the game. </summary>
		int m_directionalLightsCount;
		/// <summary> The vector of all lights that are used by the game engine. </summary>
		//std::vector<Rendering::Lighting::BaseLight*> m_lights;
		/// <summary> The vector of directional lights that are used by the game engine. </summary>
		std::vector<rendering::lighting::DirectionalLight> m_directionalLights;
		/// <summary> The vector of spot lights that are used by the game engine. </summary>
		std::vector<rendering::lighting::SpotLight> m_spotLights;
		/// <summary> The vector of point lights that are used by the game engine. </summary>
		std::vector<rendering::lighting::PointLight> m_pointLights;
		//std::vector<Lighting::SpotLight*> m_spotLights;

		math::Vector2D m_previousMousePos, m_mousePos;
		bool m_mousePosChanged;

#ifdef PROFILING_GAME_MODULE_ENABLED
		math::statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PlayGameState */

} /* end namespace Game */

#endif /* __PLAY_GAME_STATE_H__ */
