#ifndef __RENDERING_RENDERER_H__
#define __RENDERING_RENDERER_H__

#include "Rendering.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "MappedValues.h"
#include "Material.h"
#include "BaseLight.h"
#include "Color.h"
#include "Font.h"
#include "GuiControl.h"
#include "CubeShadowMap.h"
#include "Particle.h"
#include "FogInfo.h"
#include "ParticlesContainer.h"
#include "ParticlesSystem.h"
#include "FontFactory.h"
#include "MeshFactory.h"
#include "ShaderFactory.h"
#include "TextureFactory.h"
#include "GuiControl.h"

#include "Math\Angle.h"
#include "Math\Vector.h"
#include "Math\Plane.h"
#include "Math\Transform.h"
#ifdef DEBUG_RENDERING_ENABLED
#include "Math\Sphere.h"
#include "Math\AABB.h"
#include "Math\OBB.h"
#endif
#ifdef PROFILING_RENDERING_MODULE_ENABLED
#include "Math\Statistics.h"
#include "Math\StatisticsStorage.h"
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBar\include\AntTweakBar.h"
#endif

#include <string>
#include <vector>
#include <list>
#include <map>

namespace Rendering
{

	class Mesh;

	class Renderer
	{
		/* ==================== Static variables begin ==================== */
	private:
		static constexpr int SHADOW_MAPS_COUNT = 11;

		/// <summary>
		/// The matrix that transforms the shadow map coordinates from [-1, 1] range to [0, 1] range.
		/// Thanks to this matrix we don't have to transform the shadow map coordinates in the fragment shader for each light.
		/// </summary>
		static const Math::Matrix4D BIAS_MATRIX; // TODO: Make it a constexpr
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Rendering engine constructor. </summary>
		/// <param name="windowWidth"> The width of the window. </param>
		/// <param name="windowHeight"> The height of the window. </param>
		/// <param name="modelsDirectory"> The system directory where all models are stored. </param>
		/// <param name="texturesDirectory"> The system directory where all textures are stored. </param>
		/// <param name="shadersDirectory"> The system directory where all shaders are stored. </param>
		/// <param name="antiAliasingMethod"> The anti-aliasing method to be used by the rendering engine. </param>
		RENDERING_API Renderer(int windowWidth, int windowHeight, const std::string& modelsDirectory,
			const std::string& texturesDirectory, const std::string& shadersDirectory,
			const std::string& fontsDirectory, Aliasing::AntiAliasingMethod antiAliasingMethod);

		/// <summary> Rendering engine destructor. </summary>
		RENDERING_API virtual ~Renderer(void);
		
		/// <summary> Rendering engine copy constructor. </summary>
		/// <param name="renderer"> The rendering engine to copy construct from. </param>
		Renderer(const Renderer& renderer) = delete;

		/// <summary> Rendering engine move constructor. </summary>
		/// <param name="renderer"> The rendering engine to move construct from. </param>
		Renderer(Renderer&& renderer) = delete;

		/// <summary> Rendering engine copy assignment operator. </summary>
		/// <param name="renderer"> The rendering engine to copy assign from. </param>
		Renderer& operator=(const Renderer& renderer) = delete;

		/// <summary> Rendering engine move assignment operator. </summary>
		/// <param name="renderer"> The rendering engine to move assign from. </param>
		Renderer& operator=(Renderer&& renderer) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		RENDERING_API void InitRenderScene(const Color& ambientLightColor, Math::Real dayNightMixFactor);
		RENDERING_API void BindDisplayTexture() const;
		RENDERING_API void BindWaterReflectionTexture() const;
		RENDERING_API void BindWaterRefractionTexture() const;
		RENDERING_API void InitWaterNodesRendering();
		//RENDERING_API void RenderWithAmbientLight(const Mesh& mesh, const Material* material, const Math::Transform& transform) const;
		RENDERING_API void Render(int meshID, const Material* material, const Math::Transform& transform, int shaderID) const;
		RENDERING_API void FinalizeRenderScene(int filterShaderID);
		//RENDERING_API void Render(const GameNode& node);

		RENDERING_API bool InitShadowMap();
		RENDERING_API void FinalizeShadowMapRendering(int filterShaderID);

		RENDERING_API void RenderGuiControl(const Controls::GuiControl& guiControl, int guiControlShaderID) const;

		RENDERING_API void RenderParticles(int particleShaderID, const Particles::ParticlesSystem& particleSystem) const;

#ifdef ANT_TWEAK_BAR_ENABLED
		/// <summary>
		/// Initializes the rendering engine tweak bar.
		/// </summary>
		/// <remarks>
		/// This function must be called after AntTweakBar library is initialized. See Rendering::InitializeTweakBars().
		/// </remarks>
		RENDERING_API void InitializeTweakBars();
#endif
		RENDERING_API int GetWindowWidth() const { return m_windowWidth; }
		RENDERING_API int GetWindowHeight() const { return m_windowHeight; }
		RENDERING_API Rendering::Aliasing::AntiAliasingMethod GetAntiAliasingMethod() const { return m_antiAliasingMethod; }
		RENDERING_API void SetWindowWidth(int windowWidth) { m_windowWidth = windowWidth; }
		RENDERING_API void SetWindowHeight(int windowHeight) { m_windowHeight = windowHeight; }

		RENDERING_API const Mesh* CreateMesh(int meshID, const std::string& meshFileName);
		RENDERING_API const Mesh* GetMesh(int meshID) const { return m_meshFactory.GetMesh(meshID); }

		RENDERING_API const Rendering::Texture* CreateTexture(int textureID, const std::string& textureFileName)
		{
			return m_textureFactory.CreateTexture(textureID, textureFileName);
		}
		RENDERING_API const Rendering::Texture* CreateCubeTexture(int textureID, const std::string& cubeMapTextureDirectory)
		{
			return m_textureFactory.CreateCubeTexture(textureID, cubeMapTextureDirectory);
		}
		RENDERING_API const Rendering::Particles::ParticleTexture* CreateParticleTexture(int textureID, const std::string& particleTextureFileName, int rowsCount, bool isAdditive)
		{
			return m_textureFactory.CreateParticleTexture(textureID, particleTextureFileName, rowsCount, isAdditive);
		}
		RENDERING_API const Texture* GetTexture(int textureID) const { return m_textureFactory.GetTexture(textureID); }

		RENDERING_API const Shader* CreateShader(int shaderID, const std::string& shaderFileName);
		RENDERING_API const Shader* GetShader(int shaderID) const { return m_shaderFactory.GetShader(shaderID); }

		RENDERING_API const Rendering::Text::Font* CreateFont(int fontID, const std::string& fontTextureFileName, const std::string& fontMetaDataFileName);
		RENDERING_API const Rendering::Text::Font* GetFont(int fontID) const;

		RENDERING_API inline void BindShader(int shaderID) { m_shaderFactory.GetShader(shaderID)->Bind(); }
		RENDERING_API inline void UpdateRendererUniforms(int shaderID) { m_shaderFactory.GetShader(shaderID)->UpdateRendererUniforms(this); }

		RENDERING_API inline void ClearScreen() const
		{
			if (m_fogInfo.IsEnabled())
			{
				//Math::Vector3D fogColor = m_fogColor * m_ambientLight /* TODO: Should ambient light be included here? */;
				glClearColor(m_fogInfo.GetColor().GetRed(), m_fogInfo.GetColor().GetGreen(), m_fogInfo.GetColor().GetBlue(), m_fogInfo.GetColor().GetAlpha());
			}
			else
			{
				glClearColor(m_backgroundColor.GetRed(), m_backgroundColor.GetGreen(), m_backgroundColor.GetBlue(), m_backgroundColor.GetAlpha());
			}
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		RENDERING_API inline void ClearScreen(const Color& clearColor) const
		{
			glClearColor(clearColor.GetRed(), clearColor.GetGreen(), clearColor.GetBlue(), clearColor.GetAlpha());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		RENDERING_API inline void ClearScreen(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) const
		{
			glClearColor(red, green, blue, alpha);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		/// <summary>
		/// Enables or disables the depth test in the rendering engine.
		/// </summary>
		RENDERING_API inline void SetDepthTest(bool enabled)
		{
			enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		}
		RENDERING_API inline void SetBlendingEnabled(bool enabled) const { (enabled) ? glEnable(GL_BLEND) : glDisable(GL_BLEND); }
		RENDERING_API inline void SetBlendFunc(GLenum sFactor, GLenum dFactor) const { glBlendFunc(sFactor, dFactor); }
		RENDERING_API inline void SetCullFaceFront() const { glCullFace(GL_FRONT); }
		RENDERING_API inline void SetCullFaceBack() const { glCullFace(GL_BACK); }
		RENDERING_API inline void SetCullFaceFrontAndBack() const { glCullFace(GL_FRONT_AND_BACK); }
		RENDERING_API inline void SetCullFaceDefault() const { glCullFace(Rendering::glCullFaceMode); }
		RENDERING_API inline void SetDepthFuncNever() const { glDepthFunc(GL_NEVER); }
		RENDERING_API inline void SetDepthFuncLess() const { glDepthFunc(GL_LESS); }
		RENDERING_API inline void SetDepthFuncEqual() const { glDepthFunc(GL_EQUAL); }
		RENDERING_API inline void SetDepthFuncLessOrEqual() const { glDepthFunc(GL_LEQUAL); }
		RENDERING_API inline void SetDepthFuncGreater() const { glDepthFunc(GL_GREATER); }
		RENDERING_API inline void SetDepthFuncNotEqual() const { glDepthFunc(GL_NOTEQUAL); }
		RENDERING_API inline void SetDepthFuncGreaterOrEqual() const { glDepthFunc(GL_GEQUAL); }
		RENDERING_API inline void SetDepthFuncAlways() const { glDepthFunc(GL_ALWAYS); }
		RENDERING_API inline void SetDepthFuncDefault() const { glDepthFunc(Rendering::glDepthTestFunc); }

		inline const Lighting::BaseLight* GetCurrentLight() const
		{
			CHECK_CONDITION_EXIT_RENDERING(m_currentLight != NULL, Utility::Logging::ERR, "Current light is NULL.");
			return m_currentLight;
		}
		inline const Lighting::BaseLight* SetCurrentLight(const Lighting::BaseLight* light)
		{
			// TODO: Null check?
			m_currentLight = light;

			m_mappedValues.SetVector3D("directionalLight.direction", m_currentLight->GetTransform().GetTransformedRot().GetForward());
			m_mappedValues.SetVector4D("directionalLight.base.color", m_currentLight->GetColor().GetValues());
			m_mappedValues.SetReal("directionalLight.base.intensity", m_currentLight->GetIntensity());

			return m_currentLight;
		}
		inline const Lighting::PointLight* GetCurrentPointLight() const
		{
			return m_currentPointLight;
		}
		inline const Lighting::PointLight* SetCurrentPointLight(const Lighting::PointLight* pointLight)
		{
			// TODO: Index range checking
			m_currentPointLight = pointLight;
			return m_currentPointLight;
		}
		//inline const Lighting::SpotLight* GetCurrentSpotLight() const { return m_currentSpotLight; }

		inline const FogEffect::FogInfo& GetFogInfo() const { return m_fogInfo; }

		inline const BaseCamera& GetCurrentCamera() const
		{
			CHECK_CONDITION_EXIT_RENDERING(m_currentCamera != NULL, Utility::Logging::CRITICAL, "Current camera is NULL.");
			return *m_currentCamera;
		}

		//RENDERING_API void SetMenuCameraAsCurrent();
		RENDERING_API void SetCurrentCamera(const BaseCamera* camera);

		RENDERING_API void BindAsRenderTarget() const;
		RENDERING_API void InitLightRendering() const;
		RENDERING_API void FinalizeLightRendering() const;

		inline unsigned int GetSamplerSlot(const std::string& samplerName) const
		{
			std::map<std::string, unsigned int>::const_iterator samplerItr = m_samplerMap.find(samplerName);
			CHECK_CONDITION_EXIT_ALWAYS_RENDERING(samplerItr != m_samplerMap.end(), Utility::Logging::ERR, "Sampler name \"", samplerName, "\" has not been found in the sampler map.");
			return samplerItr->second;
		}

		inline const Math::Matrix4D& GetLightMatrix() const
		{
			return m_lightMatrix;
		}

		RENDERING_API bool IsAmbientLightEnabled() const { return m_ambientLightEnabled; }
		RENDERING_API bool IsWaterLightReflectionEnabled() const { return m_waterLightReflectionEnabled; }

		void BindCubeShadowMap(unsigned int textureUnit) const;

		Math::Real GetReal(const std::string& realValueName) const { return m_mappedValues.GetReal(realValueName); }
		const Math::Vector2D& GetVec2D(const std::string& vectorName) const { return m_mappedValues.GetVec2D(vectorName); }
		const Math::Vector3D& GetVec3D(const std::string& vectorName) const { return m_mappedValues.GetVec3D(vectorName); }
		const Math::Vector4D& GetVec4D(const std::string& vectorName) const { return m_mappedValues.GetVec4D(vectorName); }
		const Texture* GetTexture(const std::string& textureName) const { return m_mappedValues.GetTexture(textureName); }
		const Texture* GetTexture(const std::string& textureName, unsigned int* multitextureIndex) const { return m_mappedValues.GetTexture(textureName, multitextureIndex); }

		//RENDERING_API void SetClippingPlane(const Math::Vector4D& clippingPlane);
		RENDERING_API void EnableWaterReflectionClippingPlane(Math::Real height);
		RENDERING_API void EnableWaterRefractionClippingPlane(Math::Real height);
		RENDERING_API void DisableClippingPlanes();

#ifdef DEBUG_RENDERING_ENABLED
		/// <summary>
		/// Rendering debug nodes, i.e. the nodes that do not belong to the scene, but can be used to see the intermediate results of some other processing (e.g. how shadow map looks).
		/// </summary>
		RENDERING_API void RenderDebugGuiControls(const Shader* guiShader);

		/// <summary>
		/// Adds a line segment to the debug drawing queue.
		/// </summary>
		void AddLine(const Math::Vector3D& fromPosition, const Math::Vector3D& toPosition, const Color& color,
			Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds a wireframe sphere to the debug drawing queue.
		/// </summary>
		void AddSphere(const Math::Sphere& sphere, const Color& color,
			Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds an axis-aligned cross (3 lines converging at a point) to the debug drawing queue.
		/// </summary>
		void AddCross(const Math::Vector3D& position, const Color& color, Math::Real size,
			Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds a circle to the debug drawing queue.
		/// </summary>
		void AddCircle(const Math::Vector3D& centerPosition, const Math::Vector3D& planeNormal, Math::Real radius,
			const Color& color, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds a set of coordinate axes depicting the position and orientation of the given transformation to the debug drawing queue.
		/// </summary>
		void AddAxes(const Math::Transform& transform, const Color& color, Math::Real size,
			Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds a wireframe triangle to the debug drawing queue.
		/// </summary>
		void AddTriangle(const Math::Vector3D& v0, const Math::Vector3D& v1, const Math::Vector3D& v2,
			const Color& color, Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds an axis-aligned bounding box to the debug drawing queue.
		/// </summary>
		void AddAABB(const Math::AABB& aabb, const Color& color, Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds an oriented bounding box to the debug drawing queue.
		/// </summary>
		void AddOBB(const Math::OBB& obb, const Color& color, Math::Real lineWidth = REAL_ONE, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds a text string to the debug drawing queue.
		/// </summary>
		void AddString(const Math::Vector3D& pos, const char* text, const Color& color, Math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);
#endif

	protected:

		/// <summary>
		/// Renders the registered water nodes using the reflection and refraction textures created during the earlier stage of rendering pass.
		/// </summary>
		//void RenderWaterNodes();
		//void RenderBillboardNodes();
		//void RenderSkybox();
		//void RenderSceneWithAmbientLight(const GameNode& gameNode);
		//void RenderSceneWithPointLights(const GameNode& gameNode);
		//void RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode, bool isCastingShadowsEnabled = true);

		inline void SetSamplerSlot(const std::string& name, unsigned int value)
		{
			m_samplerMap[name] = value;
		}
	private:
		//Texture* InitializeCubeMapTexture(const std::string& cubeMapTextureDirectory);
		void BlurShadowMap(const Shader* filterShader, int shadowMapIndex, Math::Real blurAmount);
		void ApplyFilter(const Shader* filterShader, const Texture* source, const Texture* dest);
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		int m_windowWidth, m_windowHeight;
		const Rendering::Aliasing::AntiAliasingMethod m_antiAliasingMethod;
		// TODO: In the future, before shipping the game engine, remove variables (or declare them as const) that are only used when ANT_TWEAK_BAR_ENABLED is defined.
		CONST_IF_TWEAK_BAR_DISABLED bool m_applyFilterEnabled;
		CONST_IF_TWEAK_BAR_DISABLED Color m_backgroundColor;
		//bool m_shadowsEnabled;
		//bool m_pointLightShadowsEnabled;

		CONST_IF_TWEAK_BAR_DISABLED FogEffect::FogInfo m_fogInfo;

		CONST_IF_TWEAK_BAR_DISABLED bool m_ambientLightEnabled;
		const Lighting::BaseLight* m_currentLight;
		/// <summary>
		/// Current point light.
		/// </summary>
		const Lighting::PointLight* m_currentPointLight;
		//Lighting::SpotLight* m_currentSpotLight; // current spot light

		const BaseCamera* m_currentCamera;
		const BaseCamera* m_tempCamera;

		ShaderFactory m_shaderFactory;
		Text::FontFactory m_fontFactory;
		MeshFactory m_meshFactory;
		TextureFactory m_textureFactory;

		Texture m_displayTexture;

		/// <summary> The filter camera always facing the screen. </summary>
		Camera m_filterCamera; // TODO: This camera should be marked as const
		/// <summary> The alternative camera for shadow mapping, rendering to texture etc. </summary>
		Camera m_altCamera;
		Math::Transform m_filterTransform;
		const Mesh* m_filterMesh;

		CONST_IF_TWEAK_BAR_DISABLED Math::Real m_fxaaSpanMax;
		CONST_IF_TWEAK_BAR_DISABLED Math::Real m_fxaaReduceMin;
		CONST_IF_TWEAK_BAR_DISABLED Math::Real m_fxaaReduceMul;

		const Math::Real m_defaultShadowMinVariance; // TODO: Should be removed, probably.
		CubeShadowMap m_cubeShadowMap; // for use by the point lights
		//Texture m_cubeShadowMap; // for use by the point lights
		std::array<Texture, SHADOW_MAPS_COUNT> m_shadowMaps;
		std::array<Texture, SHADOW_MAPS_COUNT> m_shadowMapTempTargets;

		std::map<std::string, unsigned int> m_samplerMap;

		/// <summary> The matrix represents the way the world looks from the light perspective. </summary>
		Math::Matrix4D m_lightMatrix;

		/// <summary>
		/// The default clip plane which is used for the normal scene rendering pass.
		/// The variable could be removed if GL_CLIP_DISTANCE0 could be disabled.
		/// </summary>
		const Math::Vector4D m_defaultClipPlane;
		Math::Real m_waterWaveStrength;
		Math::Real m_waterShineDamper;
		Math::Real m_waterReflectivity;
		Math::Real m_waterWaveSpeed;
		Math::Real m_waterMoveFactor;
		Math::Vector4D m_waterRefractionClippingPlane; // TODO: Consider using Math::Plane instead
		Math::Vector4D m_waterReflectionClippingPlane; // TODO: Consider using Math::Plane instead
		/// <summary>
		/// A map holding the distortion of the water creating a wrippling effect on its surface.
		/// </summary>
		Texture m_waterDUDVTexture;
		Texture m_waterNormalMap;
		Texture m_waterRefractionTexture;
		Texture m_waterReflectionTexture;
		bool m_waterLightReflectionEnabled;
		/// <summary>
		/// The greater the more reflective water will be.
		/// </summary>
		CONST_IF_TWEAK_BAR_DISABLED Math::Real m_waterFresnelEffectFactor;
		/// <summary>
		/// The greater the more vertical water normal values will be.
		/// </summary>
		CONST_IF_TWEAK_BAR_DISABLED Math::Real m_waterNormalVerticalFactor;

		int m_maxParticlesCount;
		const InstanceMesh* m_particleQuad;
		mutable std::vector<Math::Real> m_particleInstanceVboData;

		MappedValues m_mappedValues;

#ifdef ANT_TWEAK_BAR_ENABLED
		//#ifdef CARTOON_SHADING_ENABLED // cartoon shading is included in the Lighting.glh file (probably commented out)
		//	bool cartoonShadingEnabled;
		//	Shader* cartoonShader;
		//#endif
		TwBar* m_propertiesBar;
		TwBar* m_cameraBar;
		TwBar* m_lightsBar;
		TwStructMember m_cameraMembers[5];
		TwType m_cameraType;
#endif


#ifdef DEBUG_RENDERING_ENABLED
		std::vector<std::unique_ptr<Controls::GuiControl>> m_guiControls; // https://www.youtube.com/watch?v=vOmJ1lyiJ4A&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=24
#endif

#ifdef PROFILING_RENDERING_MODULE_ENABLED
		Math::Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Renderer */

} /* end namespace Rendering */

#endif /* __RENDERING_RENDERER_H__ */