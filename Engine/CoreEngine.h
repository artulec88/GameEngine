#ifndef __ENGINE_CORE_ENGINE_H__
#define __ENGINE_CORE_ENGINE_H__

#include "Engine.h"
#include "GameManager.h"
#include "GameStateManager.h"
#include "InputMapping.h"

#include "Audio/IAudioEngine.h"

#include "Physics/PhysicsEngine.h"

#include "Rendering/GuiButtonControl.h"
#include "Rendering/Renderer.h"

#include "Math/Math.h"

#include "Utility/Time.h"

#include <string>
#include "GLFW/glfw3.h"

#ifdef PROFILING_ENGINE_MODULE_ENABLED
#include "Math/Statistics.h"
#include "Math/StatisticsStorage.h"
#include "Math/UtmostSamples.h"
#include "Math/UtmostSamples_impl.h"
#endif

#undef STOP_TIMER
#define STOP_TIMER(timerID, countStats, minMaxTime, timeSum) do { if (true) StopTimer(timerID, countStats, minMaxTime, timeSum); } while (0)

// TODO: Consider replacing some of even all of below #defines with CoreEngine class member variables.
// As member variables they could be easily added to the CoreEngine tweak bar and modified in runtime.
//#ifdef _DEBUG

#define COUNT_FPS
#ifdef COUNT_FPS
#define DRAW_FPS
#endif

//#endif

namespace engine
{
	/// <summary>
	/// The core of the game engine. It maintains the whole running game engine.
	/// </summary>
	class CoreEngine
	{
		/* ==================== Static variables and functions begin ==================== */
	protected:
		ENGINE_API static CoreEngine* s_coreEngine;
	public:
		ENGINE_API static CoreEngine* GetCoreEngine();

		static void InitGlew();

		static void ErrorCallback(int errorCode, const char* description);
		static void WindowCloseEventCallback(GLFWwindow* window);
		static void WindowResizeCallback(GLFWwindow* window, int width, int height);
		static void KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		//static void CharEventCallback(GLFWwindow* window, unsigned int codepoint);
		static void MouseEventCallback(GLFWwindow* window, int button, int action, int mods);
		static void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
		static void ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset);

		static void PollEvents();

		static math::Real GetTime();

#ifdef PROFILING_ENGINE_MODULE_ENABLED
	private:
		static void StopTimer(utility::timing::Timer& timer, long& countStats, math::statistics::UtmostSamples<long long>& minMaxTime, double& timeSum)
		{
			if (timer.IsRunning())
			{
				timer.Stop();
			}
			++countStats;
			const auto elapsedTime = timer.GetDuration(utility::timing::MICROSECOND);
			minMaxTime.ProcessSample(elapsedTime);
			timeSum += elapsedTime;
		}
#endif
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Core engine constructor. </summary>
		/// <param name="fullscreensEnabled">
		/// Fullscreen mode flag. If <code>true</code> is specified then the engine will run in fullscreen mode.
		/// Otherwise, the window's size will be set to <paramref name="width"/> x <paramref name="height"/>.
		/// </param>
		/// <param name="width"> The width of the screen. </param>
		/// <param name="height"> The height of the screen. </param>
		/// <param name="title"> The title. </param>
		/// <param name="configDirectory"> The configuration directory. </param>
		/// <param name="shadersDirectory"> The shaders directory. </param>
		/// <param name="modelsDirectory"> The models directory. </param>
		/// <param name="texturesDirectory"> The textures directory. </param>
		/// <param name="fontsDirectory"> The fonts directory. </param>
		/// <param name="audioDirectory"> The audio directory. </param>
		ENGINE_API CoreEngine(bool fullscreenEnabled, int width, int height, const char* title,
			const std::string& configDirectory = "..\\Config\\", const std::string& shadersDirectory = "..\\Shaders\\",
			const std::string& modelsDirectory = "..\\Models\\", const std::string& texturesDirectory = "..\\Textures\\",
			const std::string& fontsDirectory = "..\\Fonts\\", const std::string& audioDirectory = "..\\Sounds\\");
		
		/// <summary> Core engine destructor. </summary>
		ENGINE_API ~CoreEngine();

		/// <summary> Core engine copy constructor. </summary>
		/// <param name="coreEngine"> The core engine to copy construct from. </param>
		CoreEngine(const CoreEngine& coreEngine) = delete;

		/// <summary> Core engine move constructor. </summary>
		/// <param name="coreEngine"> The core engine to move construct from. </param>
		CoreEngine(CoreEngine&& coreEngine) = delete;

		/// <summary> Core engine copy assignment operator. </summary>
		/// <param name="coreEngine"> The core engine to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned core engine. </returns>
		CoreEngine& operator=(const CoreEngine& coreEngine) = delete;

		/// <summary> Core engine move assignment operator. </summary>
		/// <param name="coreEngine"> The core engine to move assign from. </param>
		/// <returns> The reference to the newly move-assigned core engine. </returns>
		CoreEngine& operator=(CoreEngine&& coreEngine) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void Start(GameManager* gameManager);
		ENGINE_API void Stop();
		void RequestWindowClose() const
		{
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		//ENGINE_API void MakeMainWindowCurrentContext() const { glfwMakeContextCurrent(m_window); }
		GLFWwindow* GetThreadWindow() const { return m_threadWindow; }


		void WindowResizeEvent(GLFWwindow* window, int width, int height) const;

		/// <summary>
		/// Error callback.
		/// </summary>
		/// <param name="errorCode"> The error code. See http://www.glfw.org/docs/latest/group__errors.html. </param>
		/// <param name="description"> The description of the error. </param>
		void ErrorCallbackEvent(int errorCode, const char* description);
		void CloseWindowEvent(GLFWwindow* window) const;

		/// <summary>
		/// Main entry for the key event handling.
		/// </summary>
		/// <param name="key">The keyboard key that was pressed or released.</param>
		/// <param name="scancode">The system-specific scancode of the key.</param>
		/// <param name="action">GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.</param>
		/// <param name="mods">Bit field describing which modifier keys were held down when key event was dispatched.</param>
		void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
		void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
		void MousePosEvent(GLFWwindow* window, double xPos, double yPos);
		void ScrollEvent(GLFWwindow* window, double xOffset, double yOffset) const;

		/// <summary> Returns the width of the window used by the core and rendering engines. </summary>
		/// <returns> The width of the window used by the core and rendering engines. </returns>
		int GetWindowWidth() const { return m_windowWidth; }

		/// <summary> Returns the height of the window used by the core and rendering engines. </summary>
		/// <returns> The height of the window used by the core and rendering engines. </returns>
		int GetWindowHeight() const { return m_windowHeight; }

		void SetCursorPos(math::Real xPos, math::Real yPos) const;
		ENGINE_API void CentralizeCursor();

		void ClearScreen() const;
		//void ConvertTimeOfDay(int& inGameHours, int& inGameMinutes, int& inGameSeconds) const;
		//void ConvertTimeOfDay(math::Real timeOfDay, int& inGameHours, int& inGameMinutes, int& inGameSeconds) const;

		audio::IAudioEngine& GetAudioEngine() const { return *m_audioEngine; }

		const std::string& GetShadersDirectory() const { return m_shadersDirectory; }
		const std::string& GetModelsDirectory() const { return m_modelsDirectory; }
		const std::string& GetTexturesDirectory() const { return m_texturesDirectory; }
		const std::string& GetFontsDirectory() const { return m_fontsDirectory; }
		const std::string& GetAudioDirectory() const { return m_audioDirectory; }

		ENGINE_API void AddBillboardNode(GameNode* billboardNode);
		ENGINE_API void AddPhysicsObject(physics::PhysicsObject* physicsObject) const; // TODO: In the future only the specialized Builder should call this function (the PhysicsObjectBuilder). Once it is done remove ENGINE_API from here.
		
		ENGINE_API const rendering::Mesh* GetMesh(int meshId) const { return m_renderer->GetMesh(meshId); }
		ENGINE_API const rendering::Mesh* AddMesh(int meshId, const std::string& meshFileName) const
		{
			return m_renderer->CreateMesh(meshId, meshFileName);
		}

		ENGINE_API const rendering::Texture* AddTexture(int textureId, const std::string& textureFileName) const
		{
			return m_renderer->CreateTexture(textureId, textureFileName);
		}
		ENGINE_API const rendering::Texture* AddCubeTexture(int textureId, const std::string& cubeMapTextureDirectory) const
		{
			return m_renderer->CreateCubeTexture(textureId, cubeMapTextureDirectory);
		}
		ENGINE_API const rendering::Texture* GetTexture(int textureId) const
		{
			return m_renderer->GetTexture(textureId);
		}
		ENGINE_API const rendering::Shader* AddShader(int shaderId, const std::string& shaderFileName) const
		{
			return m_renderer->CreateShader(shaderId, shaderFileName);
		}
		ENGINE_API const rendering::Shader* GetShader(int shaderId) const
		{
			return m_renderer->GetShader(shaderId);
		}
		ENGINE_API const rendering::text::Font* CreateFont(int fontId, const std::string& fontTextureFileName,
			const std::string& fontMetaDataFileName) const
		{
			return m_renderer->CreateFont(fontId, fontTextureFileName, fontMetaDataFileName);
		}
		ENGINE_API const rendering::text::Font* GetFont(int fontId) const
		{
			return m_renderer->GetFont(fontId);
		}

		ENGINE_API void PushInputContext(const std::string& inputContextName);
		ENGINE_API void PopInputContext();
		//const Rendering::CameraBase& GetCurrentCamera() const { return m_renderer->GetCurrentCamera(); }
	private:
		void CreateAudioEngine();
		void CreatePhysicsEngine();
		void CreateRenderer(bool fullscreenEnabled, int width, int height, const std::string& title, rendering::aliasing::AntiAliasingMethod antiAliasingMethod);
		void Run();

#ifdef PROFILING_ENGINE_MODULE_ENABLED
	public:
		void StartSamplingSpf() const { m_isSamplingSpf = true; }
		void StopSamplingSpf() const { m_isSamplingSpf = false; }
	private:
		void InitGraphics(bool fullscreenEnabled, int width, int height, const std::string& title, rendering::aliasing::AntiAliasingMethod antiAliasingMethod);
		void InitGlfw(bool fullscreenEnabled, int width, int height, const std::string& title, rendering::aliasing::AntiAliasingMethod antiAliasingMethod);
		void SetCallbacks() const;
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
		void InitializeTweakBars();
	public:
		ENGINE_API void InitializeGameTweakBars() const;
#endif
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		GLFWwindow* m_window;
		GLFWwindow* m_threadWindow;
		bool m_isRunning;
		int m_windowWidth;
		int m_windowHeight;
		const char* m_windowTitle;
		const math::Real m_frameTime;
		GameManager* m_game;
		std::unique_ptr<audio::IAudioEngine> m_audioEngine;
		physics::PhysicsEngine* m_physicsEngine;
		std::unique_ptr<rendering::Renderer> m_renderer; // TODO: Replace unique_ptr with a simple instance of Rendering::Renderer.

		/// <summary> Specifies where to look for the configuration files. </summary>
		const std::string m_configDirectory;
		/// <summary> Specifies where to look for the shader files. </summary>
		const std::string m_shadersDirectory;
		/// <summary> Specifies where to look for the model files. </summary>
		const std::string m_modelsDirectory;
		/// <summary> Specifies where to look for the texture files. </summary>
		const std::string m_texturesDirectory;
		/// <summary> Specifies where to look for the fonts files. </summary>
		const std::string m_fontsDirectory;
		/// <summary> Specifies where to look for the audio files. </summary>
		const std::string m_audioDirectory;

		const std::map<int, input::raw_input_keys::RawInputKey> m_glfwKeysToRawInputKeysMap;
		input::InputMapping m_inputMapping;

#ifdef PROFILING_ENGINE_MODULE_ENABLED
		long m_countStats1;
		math::statistics::UtmostSamples<long long> m_minMaxTime1;
		double m_timeSum1;

		long m_countStats2;
		math::statistics::UtmostSamples<long long> m_minMaxTime2;
		double m_timeSum2;

		long m_countStats2_1;
		math::statistics::UtmostSamples<long long> m_minMaxTime2_1;
		double m_timeSum2_1;

		long m_countStats2_2;
		math::statistics::UtmostSamples<long long> m_minMaxTime2_2;
		double m_timeSum2_2;

		long m_countStats2_3;
		math::statistics::UtmostSamples<long long> m_minMaxTime2_3;
		double m_timeSum2_3;

		long m_countStats3;
		math::statistics::UtmostSamples<long long> m_minMaxTime3;
		double m_timeSum3;

		mutable int m_renderingRequiredCount;
		mutable int m_renderingNotRequiredCount;
		mutable bool m_isSamplingSpf;
		math::statistics::ClassStats& m_classStats;
		mutable math::statistics::Stats<math::Real> m_stats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CoreEngine */

} /* end namespace engine */

#endif /* __ENGINE_CORE_ENGINE_H__ */