#ifndef __RENDERING_RENDERER_H__
#define __RENDERING_RENDERER_H__

#include "Rendering.h"
#include "Camera.h"
#include "PointLight.h"
#include "MappedValues.h"
#include "Material.h"
#include "BaseLight.h"
#include "Color.h"
#include "Font.h"
#include "GuiControl.h"
#include "CubeShadowMap.h"
#include "FogInfo.h"
#include "ParticlesContainer.h"
#include "ParticlesSystem.h"
#include "FontFactory.h"
#include "MeshFactory.h"
#include "ShaderFactory.h"
#include "TextureFactory.h"

#include "Math/Vector.h"
#include "Math/Plane.h"
#include "Math/Transform.h"
#ifdef DEBUG_RENDERING_ENABLED
#include "Math/AABB.h"
#include "Math/OBB.h"
#endif
#ifdef PROFILING_RENDERING_MODULE_ENABLED
#include "Math/Statistics.h"
#include "Math/StatisticsStorage.h"
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBar/include/AntTweakBar.h"
#endif

#include <string>
#include <vector>
#include <map>

namespace rendering
{

	class Mesh;

	class Renderer
	{
		/* ==================== Static variables begin ==================== */
	public:
		RENDERING_API static void ClearScreen(const Color& clearColor)
		{
			ClearScreen(clearColor.GetRed(), clearColor.GetGreen(), clearColor.GetBlue(), clearColor.GetAlpha());
		}
		RENDERING_API static void ClearScreen(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
		{
			glClearColor(red, green, blue, alpha);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	private:
		static constexpr int SHADOW_MAPS_COUNT = 11;

		/// <summary>
		/// The matrix that transforms the shadow map coordinates from [-1, 1] range to [0, 1] range.
		/// Thanks to this matrix we don't have to transform the shadow map coordinates in the fragment shader for each light.
		/// </summary>
		static const math::Matrix4D BIAS_MATRIX; // TODO: Make it a constexpr
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
			const std::string& fontsDirectory, aliasing::AntiAliasingMethod antiAliasingMethod);

		/// <summary> Rendering engine destructor. </summary>
		RENDERING_API virtual ~Renderer();

		/// <summary> Rendering engine copy constructor. </summary>
		/// <param name="renderer"> The rendering engine to copy construct from. </param>
		Renderer(const Renderer& renderer) = delete;

		/// <summary> Rendering engine move constructor. </summary>
		/// <param name="renderer"> The rendering engine to move construct from. </param>
		Renderer(Renderer&& renderer) = delete;

		/// <summary> Rendering engine copy assignment operator. </summary>
		/// <param name="renderer"> The rendering engine to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned rendering engine. </returns>
		Renderer& operator=(const Renderer& renderer) = delete;

		/// <summary> Rendering engine move assignment operator. </summary>
		/// <param name="renderer"> The rendering engine to move assign from. </param>
		/// <returns> The reference to the newly move-assigned rendering engine. </returns>
		Renderer& operator=(Renderer&& renderer) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
		RENDERING_API void InitRenderScene(const Color& ambientLightColor, math::Real dayNightMixFactor);
		RENDERING_API void BindDisplayTexture() const;
		RENDERING_API void BindWaterReflectionTexture() const;
		RENDERING_API void BindWaterRefractionTexture() const;
		RENDERING_API void InitWaterNodesRendering();
		//RENDERING_API void RenderWithAmbientLight(const Mesh& mesh, const Material* material, const math::Transform& transform) const;
		RENDERING_API void Render(int meshId, const Material* material, const math::Transform& transform, int shaderId) const;
		RENDERING_API void FinalizeRenderScene(int filterShaderId);
		//RENDERING_API void Render(const GameNode& node);

		RENDERING_API bool InitShadowMap();
		RENDERING_API void FinalizeShadowMapRendering(int filterShaderId);

		RENDERING_API void RenderGuiControl(const controls::GuiControl& guiControl, int guiControlShaderId) const;

		RENDERING_API void RenderParticles(int particleShaderId, const particles::ParticlesSystem& particleSystem) const;

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
		RENDERING_API aliasing::AntiAliasingMethod GetAntiAliasingMethod() const { return m_antiAliasingMethod; }
		RENDERING_API void SetWindowWidth(const int windowWidth) { m_windowWidth = windowWidth; }
		RENDERING_API void SetWindowHeight(const int windowHeight) { m_windowHeight = windowHeight; }

		RENDERING_API const Mesh* CreateMesh(int meshId, const std::string& meshFileName);
		RENDERING_API const Mesh* CreateMeshFromSurface(int meshId, const math::Surface& surface);
		RENDERING_API const Mesh* GetMesh(const int meshId) const { return m_meshFactory.GetMesh(meshId); }

		RENDERING_API const Texture* CreateTexture(int textureID, const std::string& textureFileName)
		{
			return m_textureFactory.CreateTexture(textureID, textureFileName);
		}
		RENDERING_API const Texture* CreateCubeTexture(int textureID, const std::string& cubeMapTextureDirectory)
		{
			return m_textureFactory.CreateCubeTexture(textureID, cubeMapTextureDirectory);
		}
		RENDERING_API const particles::ParticleTexture* CreateParticleTexture(int textureID, const std::string& particleTextureFileName, int rowsCount, bool isAdditive)
		{
			return m_textureFactory.CreateParticleTexture(textureID, particleTextureFileName, rowsCount, isAdditive);
		}
		RENDERING_API const std::string& GetTexturesDirectory() const { return m_textureFactory.GetTexturesDirectory(); }
		RENDERING_API const Texture* GetTexture(const int textureId) const { return m_textureFactory.GetTexture(textureId); }

		RENDERING_API const Shader* CreateShader(int shaderId, const std::string& shaderFileName);
		RENDERING_API const Shader* GetShader(const int shaderId) const { return m_shaderFactory.GetShader(shaderId); }

		RENDERING_API const text::Font* CreateFont(int fontId, const std::string& fontTextureFileName, const std::string& fontMetaDataFileName);
		RENDERING_API const text::Font* GetFont(int fontId) const;

		RENDERING_API void BindShader(const int shaderId) const { m_shaderFactory.GetShader(shaderId)->Bind(); }
		RENDERING_API void UpdateRendererUniforms(const int shaderId) const { m_shaderFactory.GetShader(shaderId)->UpdateRendererUniforms(this); }

		RENDERING_API void ClearScreen() const
		{
			//math::Vector3D fogColor = m_fogColor * m_ambientLight /* TODO: Should ambient light be included here? */;
			ClearScreen(m_fogInfo.IsEnabled() ? m_fogInfo.GetColor() : m_backgroundColor);
		}

		/// <summary>
		/// Enables or disables the depth test in the rendering engine.
		/// </summary>
		RENDERING_API void SetDepthTest(bool enabled)
		{
			enabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		}
		RENDERING_API void SetBlendingEnabled(bool enabled) const { enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND); }
		RENDERING_API void SetBlendFunc(GLenum sFactor, GLenum dFactor) const { glBlendFunc(sFactor, dFactor); }
		RENDERING_API void SetCullFaceFront() const { glCullFace(GL_FRONT); }
		RENDERING_API void SetCullFaceBack() const { glCullFace(GL_BACK); }
		RENDERING_API void SetCullFaceFrontAndBack() const { glCullFace(GL_FRONT_AND_BACK); }
		RENDERING_API void SetCullFaceDefault() const { glCullFace(glCullFaceMode); }
		RENDERING_API void SetDepthFuncNever() const { glDepthFunc(GL_NEVER); }
		RENDERING_API void SetDepthFuncLess() const { glDepthFunc(GL_LESS); }
		RENDERING_API void SetDepthFuncEqual() const { glDepthFunc(GL_EQUAL); }
		RENDERING_API void SetDepthFuncLessOrEqual() const { glDepthFunc(GL_LEQUAL); }
		RENDERING_API void SetDepthFuncGreater() const { glDepthFunc(GL_GREATER); }
		RENDERING_API void SetDepthFuncNotEqual() const { glDepthFunc(GL_NOTEQUAL); }
		RENDERING_API void SetDepthFuncGreaterOrEqual() const { glDepthFunc(GL_GEQUAL); }
		RENDERING_API void SetDepthFuncAlways() const { glDepthFunc(GL_ALWAYS); }
		RENDERING_API void SetDepthFuncDefault() const { glDepthFunc(glDepthTestFunc); }

		const lighting::BaseLight* GetCurrentLight() const
		{
			CHECK_CONDITION_EXIT_RENDERING(m_currentLight != nullptr, Utility::Logging::ERR, "Current light is NULL.");
			return m_currentLight;
		}
		const lighting::BaseLight* SetCurrentLight(const lighting::BaseLight* light)
		{
			// TODO: Null check?
			m_currentLight = light;

			m_mappedValues.SetVector3D("directionalLight.direction", m_currentLight->GetTransform().GetTransformedRot().GetForward());
			m_mappedValues.SetVector4D("directionalLight.base.color", m_currentLight->GetColor().GetValues());
			m_mappedValues.SetReal("directionalLight.base.intensity", m_currentLight->GetIntensity());

			return m_currentLight;
		}
		const lighting::PointLight* GetCurrentPointLight() const
		{
			return m_currentPointLight;
		}
		const lighting::PointLight* SetCurrentPointLight(const lighting::PointLight* pointLight)
		{
			// TODO: Index range checking
			m_currentPointLight = pointLight;
			return m_currentPointLight;
		}
		//const Lighting::SpotLight* GetCurrentSpotLight() const { return m_currentSpotLight; }

		const fog_effect::FogInfo& GetFogInfo() const { return m_fogInfo; }

		const Camera& GetCurrentCamera() const
		{
			CHECK_CONDITION_EXIT_RENDERING(m_currentCamera != nullptr, Utility::Logging::CRITICAL, "Current camera is NULL.");
			return *m_currentCamera;
		}

		//RENDERING_API void SetMenuCameraAsCurrent();
		RENDERING_API void SetCurrentCamera(const Camera* camera);

		RENDERING_API void BindAsRenderTarget() const;
		RENDERING_API void InitLightRendering() const;
		RENDERING_API void FinalizeLightRendering() const;

		unsigned int GetSamplerSlot(const std::string& samplerName) const
		{
			const auto samplerItr = m_samplerMap.find(samplerName);
			CHECK_CONDITION_EXIT_ALWAYS_RENDERING(samplerItr != m_samplerMap.end(), utility::logging::ERR,
				"Sampler name \"", samplerName, "\" has not been found in the sampler map.");
			return samplerItr->second;
		}

		const math::Matrix4D& GetLightMatrix() const
		{
			return m_lightMatrix;
		}

		RENDERING_API bool IsAmbientLightEnabled() const { return m_ambientLightEnabled; }
		RENDERING_API bool IsWaterLightReflectionEnabled() const { return m_waterLightReflectionEnabled; }

		void BindCubeShadowMap(unsigned int textureUnit) const;

		math::Real GetReal(const std::string& realValueName) const { return m_mappedValues.GetReal(realValueName); }
		const math::Vector2D& GetVec2D(const std::string& vectorName) const { return m_mappedValues.GetVec2D(vectorName); }
		const math::Vector3D& GetVec3D(const std::string& vectorName) const { return m_mappedValues.GetVec3D(vectorName); }
		const math::Vector4D& GetVec4D(const std::string& vectorName) const { return m_mappedValues.GetVec4D(vectorName); }
		const Texture* GetTexture(const std::string& textureName) const { return m_mappedValues.GetTexture(textureName); }
		const Texture* GetTexture(const std::string& textureName, unsigned int* multitextureIndex) const { return m_mappedValues.GetTexture(textureName, multitextureIndex); }

		//RENDERING_API void SetClippingPlane(const math::Vector4D& clippingPlane);
		RENDERING_API void EnableWaterReflectionClippingPlane(math::Real height);
		RENDERING_API void EnableWaterRefractionClippingPlane(math::Real height);
		RENDERING_API void DisableClippingPlanes();

#ifdef DEBUG_RENDERING_ENABLED
		/// <summary>
		/// Rendering debug nodes, i.e. the nodes that do not belong to the scene, but can be used to see the intermediate results of some other processing (e.g. how shadow map looks).
		/// </summary>
		RENDERING_API void RenderDebugGuiControls(const Shader* guiShader);

		/// <summary>
		/// Adds a line segment to the debug drawing queue.
		/// </summary>
		void AddLine(const math::Vector3D& fromPosition, const math::Vector3D& toPosition, const Color& color,
			math::Real lineWidth = REAL_ONE, math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds a wireframe sphere to the debug drawing queue.
		/// </summary>
		void AddSphere(const math::Sphere& sphere, const Color& color,
			math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds an axis-aligned cross (3 lines converging at a point) to the debug drawing queue.
		/// </summary>
		void AddCross(const math::Vector3D& position, const Color& color, math::Real size,
			math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds a circle to the debug drawing queue.
		/// </summary>
		void AddCircle(const math::Vector3D& centerPosition, const math::Vector3D& planeNormal, math::Real radius,
			const Color& color, math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds a set of coordinate axes depicting the position and orientation of the given transformation to the debug drawing queue.
		/// </summary>
		void AddAxes(const math::Transform& transform, const Color& color, math::Real size,
			math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds a wireframe triangle to the debug drawing queue.
		/// </summary>
		void AddTriangle(const math::Vector3D& v0, const math::Vector3D& v1, const math::Vector3D& v2,
			const Color& color, math::Real lineWidth = REAL_ONE, math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds an axis-aligned bounding box to the debug drawing queue.
		/// </summary>
		void AddAabb(const math::Aabb& aabb, const Color& color, math::Real lineWidth = REAL_ONE, math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds an oriented bounding box to the debug drawing queue.
		/// </summary>
		void AddObb(const math::Obb& obb, const Color& color, math::Real lineWidth = REAL_ONE, math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);

		/// <summary>
		/// Adds a text string to the debug drawing queue.
		/// </summary>
		void AddString(const math::Vector3D& pos, const char* text, const Color& color, math::Real duration = REAL_ZERO, bool isDepthTestEnabled = true);
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

		void SetSamplerSlot(const std::string& name, const unsigned int value)
		{
			m_samplerMap[name] = value;
		}
	private:
		//Texture* InitializeCubeMapTexture(const std::string& cubeMapTextureDirectory);
		void BlurShadowMap(const Shader* filterShader, int shadowMapIndex, math::Real blurAmount);
		void ApplyFilter(const Shader* filterShader, const Texture* source, const Texture* dest);
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		int m_windowWidth, m_windowHeight;
		const aliasing::AntiAliasingMethod m_antiAliasingMethod;
		// TODO: In the future, before shipping the game engine, remove variables (or declare them as const) that are only used when ANT_TWEAK_BAR_ENABLED is defined.
		CONST_IF_TWEAK_BAR_DISABLED bool m_applyFilterEnabled;
		CONST_IF_TWEAK_BAR_DISABLED Color m_backgroundColor;
		//bool m_shadowsEnabled;
		//bool m_pointLightShadowsEnabled;

		CONST_IF_TWEAK_BAR_DISABLED fog_effect::FogInfo m_fogInfo;

		CONST_IF_TWEAK_BAR_DISABLED bool m_ambientLightEnabled;
		const lighting::BaseLight* m_currentLight;
		/// <summary>
		/// Current point light.
		/// </summary>
		const lighting::PointLight* m_currentPointLight;
		//Lighting::SpotLight* m_currentSpotLight; // current spot light

		const Camera* m_currentCamera;
		const Camera* m_tempCamera;

		ShaderFactory m_shaderFactory;
		text::FontFactory m_fontFactory;
		MeshFactory m_meshFactory;
		TextureFactory m_textureFactory;

		Texture m_displayTexture;

		/// <summary> The filter camera always facing the screen. </summary>
		Camera m_filterCamera; // TODO: This camera should be marked as const
		/// <summary> The alternative camera for shadow mapping, rendering to texture etc. </summary>
		Camera m_altCamera;
		math::Transform m_filterTransform;
		const Mesh* m_filterMesh;

		CONST_IF_TWEAK_BAR_DISABLED math::Real m_fxaaSpanMax;
		CONST_IF_TWEAK_BAR_DISABLED math::Real m_fxaaReduceMin;
		CONST_IF_TWEAK_BAR_DISABLED math::Real m_fxaaReduceMul;

		const math::Real m_defaultShadowMinVariance; // TODO: Should be removed, probably.
		CubeShadowMap m_cubeShadowMap; // for use by the point lights
		//Texture m_cubeShadowMap; // for use by the point lights
		std::array<Texture, SHADOW_MAPS_COUNT> m_shadowMaps;
		std::array<Texture, SHADOW_MAPS_COUNT> m_shadowMapTempTargets;

		std::map<std::string, unsigned int> m_samplerMap;

		/// <summary> The matrix represents the way the world looks from the light perspective. </summary>
		math::Matrix4D m_lightMatrix;

		/// <summary>
		/// The default clip plane which is used for the normal scene rendering pass.
		/// The variable could be removed if GL_CLIP_DISTANCE0 could be disabled.
		/// </summary>
		const math::Vector4D m_defaultClipPlane;
		math::Real m_waterWaveStrength;
		math::Real m_waterShineDamper;
		math::Real m_waterReflectivity;
		math::Real m_waterWaveSpeed;
		math::Real m_waterMoveFactor;
		math::Vector4D m_waterRefractionClippingPlane; // TODO: Consider using math::Plane instead
		math::Vector4D m_waterReflectionClippingPlane; // TODO: Consider using math::Plane instead
		/// <summary>
		/// A map holding the distortion of the water creating a wrippling effect on its surface.
		/// </summary>
		Texture m_waterDuDvTexture;
		Texture m_waterNormalMap;
		Texture m_waterRefractionTexture;
		Texture m_waterReflectionTexture;
		bool m_waterLightReflectionEnabled;
		/// <summary>
		/// The greater the more reflective water will be.
		/// </summary>
		CONST_IF_TWEAK_BAR_DISABLED math::Real m_waterFresnelEffectFactor;
		/// <summary>
		/// The greater the more vertical water normal values will be.
		/// </summary>
		CONST_IF_TWEAK_BAR_DISABLED math::Real m_waterNormalVerticalFactor;

		int m_maxParticlesCount;
		const InstanceMesh* m_particleQuad;
		mutable std::vector<math::Real> m_particleInstanceVboData;

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
		std::vector<std::unique_ptr<controls::GuiControl>> m_guiControls; // https://www.youtube.com/watch?v=vOmJ1lyiJ4A&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=24
#endif

#ifdef PROFILING_RENDERING_MODULE_ENABLED
		math::statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Renderer */

} /* end namespace rendering */

#endif /* __RENDERING_RENDERER_H__ */