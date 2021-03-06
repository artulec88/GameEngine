#include "Def.h"

#include "Rendering/AntTweakBarTypes.h"
#include "Rendering/CameraBuilder.h"
#include "Rendering/LightBuilder.h"
#include "Rendering/ParticlesSystemBuilder.h"
#include "Rendering/Renderer.h"
#include "Rendering/GuiButtonControl.h"
#include "Rendering/FontFactory.h"
#include "Rendering/Mesh.h"
#include "Rendering/MeshIDs.h"
#include "Rendering/TextureIDs.h"
#include "Rendering/Material.h"
#include "Rendering/Terrain.h"
#include "Rendering/Image.h"
#include "Rendering/stb_image.h"
#include "Rendering/ShaderIDs.h"
#include "Rendering/ParticlePositionGenerator.h"
#include "Rendering/ParticleColorGenerator.h"
#include "Rendering/ParticleColorUpdater.h"
#include "Rendering/ParticlePositionUpdater.h"
#include "Rendering/ParticleScaleGenerator.h"
#include "Rendering/ParticleRotationGenerator.h"
#include "Rendering/ParticleRotationUpdater.h"

#include "Math/Transform.h"
#include "Math/StatisticsStorage.h"
#include "Math/RandomGeneratorFactory.h"

#include "Utility/ICommandLineMapper.h"
#include "Utility/ILogger.h"
#include "Utility/IConfig.h"
#include "Utility/Time.h"
#include "Utility/BuilderDirector.h"

#include <GLFW/glfw3.h>
#include <ctime>
#include <string>
#include <thread>
//#include <xmmintrin.h>
//#include <iostream>
//#include <fstream>

using namespace rendering;
using namespace math;
using namespace utility;
using namespace std;

const string MODULE_NAME = "MathTest";
const string CONFIG_DIR = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Config\\";
const string MODELS_DIR = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Models\\";
const string SHADERS_DIR = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Shaders\\";
const string TEXTURES_DIR = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Textures\\";
const string FONTS_DIR = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Fonts\\";
constexpr int WINDOW_WIDTH = 1600;
constexpr int WINDOW_HEIGHT = 900;
const random::RandomGenerator& g_randomGenerator = random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(random::generator_ids::SIMPLE);
GLFWwindow* window = nullptr;
GLFWwindow* threadWindow = nullptr;
unique_ptr<Renderer> renderer = nullptr;

bool cameraRotationEnabled = false;
unique_ptr<Camera> camera;

unique_ptr<particles::ParticlesSystem> particlesSystem = nullptr;
unique_ptr<particles::ParticlesKiller> particlesKiller = nullptr;

namespace test_mesh_ids
{
	enum TestMeshId
	{
		TERRAIN = mesh_ids::COUNT,
		SKYBOX,
		CUBE
	};
}

namespace test_texture_ids
{
	enum TestTextureId
	{
		TERRAIN_DIFFUSE = texture_ids::COUNT,
		TERRAIN_DIFFUSE_2,
		TERRAIN_DIFFUSE_3,
		TERRAIN_DIFFUSE_4,
		TERRAIN_NORMAL_MAP,
		TERRAIN_DISPLACEMENT_MAP,
		TERRAIN_BLEND_MAP,
		SKYBOX_DAY,
		SKYBOX_NIGHT,
		CUBE_DIFFUSE
	};
}

constexpr int TERRAIN_VERTEX_COUNT = 128;
constexpr Real TERRAIN_HEIGHT_GENERATOR_AMPLITUDE = 70.0f;
constexpr int TERRAIN_HEIGHT_GENERATOR_OCTAVES_COUNT = 3;
constexpr Real TERRAIN_HEIGHT_GENERATOR_ROUGHNESS = 0.3f;

Transform terrainTransform;
unique_ptr<Material> terrainMaterial = nullptr;
const Mesh* terrainMesh = nullptr;
unique_ptr<Terrain> terrain;

Transform skyboxTransform;
unique_ptr<Material> skyboxMaterial = nullptr;
const Mesh* skyboxMesh = nullptr;

constexpr int CUBE_MESHES_ROWS = 20;
constexpr int CUBE_MESHES_COLS = 20;
array<Transform, CUBE_MESHES_ROWS * CUBE_MESHES_COLS> cubeTransforms;
const Mesh* cubeMesh = nullptr;
unique_ptr<Material> cubeMaterial = nullptr;

unsigned int testNumber = 0;
bool meshTestEnabled = true;
bool textureTestEnabled = true;
bool cameraBuilderTestEnabled = true;
bool lightBuilderTestEnabled = true;
bool particlesSystemBuilderTestEnabled = true;
bool otherTestsEnabled = true;

void WindowCloseEventCallback(GLFWwindow* window)
{
	NOTICE_LOG_RENDERING_TEST("Window close event callback");
	glfwSetWindowShouldClose(window, GL_TRUE);
}

void WindowResizeEvent(GLFWwindow* window, int width, int height)
{
	renderer->SetWindowWidth(width);
	renderer->SetWindowHeight(height);
}

void WindowResizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwWindowSize(width, height))
	{
		WindowResizeEvent(window, width, height);
	}
#else
	WindowResizeEvent(window, width, height);
#endif
}

void KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	const auto& rotation = camera->GetRot();
	Vector3D dirVector;
	switch (key)
	{
	case GLFW_KEY_A:
		dirVector = rotation.GetLeft();
		break;
	case GLFW_KEY_W:
		dirVector = rotation.GetForward();
		break;
	case GLFW_KEY_S:
		dirVector = rotation.GetBack();
		break;
	case GLFW_KEY_D:
		dirVector = rotation.GetRight();
		break;
	case GLFW_KEY_LEFT_CONTROL:
		dirVector = rotation.GetDown();
		break;
	case GLFW_KEY_SPACE:
		dirVector = rotation.GetUp();
		break;
	default:
		break;
	}
	camera->GetPos().Increase(dirVector * camera->GetSensitivity() * (mods == GLFW_MOD_SHIFT ? 10.0f : 1.0f));
	//ERROR_LOG_RENDERING("camera.Pos = ", camera.GetTransform().GetPos().GetXZ(), "; height = ",
	//	terrain->GetHeightAt(camera.GetTransform().GetPos().GetXZ()));
	camera->GetPos().y = terrain->GetHeightAt(camera->GetPos().GetXz()) + 0.02f;
	//NOTICE_LOG_RENDERING_TEST(camera);
}

void KeyEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	INFO_LOG_RENDERING_TEST("Key event callback (key = ", key, "; scancode = ", scancode, "; action = ", action, "; mods = ", mods);
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventKeyGLFW(key, action))  // send event to AntTweakBar
	{
		// event has not been handled by AntTweakBar
		KeyEvent(window, key, scancode, action, mods);
	}
#else
	KeyEvent(window, key, scancode, action, mods);
#endif
}

void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		cameraRotationEnabled = action == GLFW_PRESS || action == GLFW_REPEAT;
		if (cameraRotationEnabled)
		{
			const Vector2D centerPosition(static_cast<Real>(WINDOW_WIDTH) / 2, static_cast<Real>(WINDOW_HEIGHT) / 2);
			glfwSetCursorPos(window, centerPosition.x, centerPosition.y);
		}
	}
}

void MouseEventCallback(GLFWwindow* window, int button, int action, int mods)
{
	INFO_LOG_RENDERING_TEST("Mouse event callback");
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventMouseButtonGLFW(button, action))
	{
		// event has not been handled by AntTweakBar
		MouseButtonEvent(window, button, action, mods);
	}
#else
	MouseButtonEvent(window, button, action, mods);
#endif
}

void MousePosEvent(GLFWwindow* window, double xPos, double yPos)
{
	if (cameraRotationEnabled)
	{
		const Vector2D centerPosition(static_cast<Real>(WINDOW_WIDTH) / 2, static_cast<Real>(WINDOW_HEIGHT) / 2);
		Vector2D deltaPosition(static_cast<Real>(xPos), static_cast<Real>(yPos));
		deltaPosition -= centerPosition;

		const auto rotX = !AlmostEqual(deltaPosition.x, REAL_ZERO);
		const auto rotY = !AlmostEqual(deltaPosition.y, REAL_ZERO);

		if (rotX || rotY)
		{
			if (rotX)
			{
				camera->Rotate(Vector3D(0, 1, 0), Angle(deltaPosition.x * camera->GetSensitivity()));
			}
			if (rotY)
			{
				camera->Rotate(camera->GetRot().GetRight(), Angle(deltaPosition.y * camera->GetSensitivity()));
			}
			//NOTICE_LOG_RENDERING_TEST("Camera's rotation: ", camera.GetTransform().GetRot());
			glfwSetCursorPos(window, centerPosition.x, centerPosition.y);
		}
	}
}

void MousePosCallback(GLFWwindow* window, double xPos, double yPos)
{
	DEBUG_LOG_RENDERING_TEST("Mouse event callback");
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventMousePosGLFW(static_cast<int>(xPos), static_cast<int>(yPos)))
	{
		// event has not been handled by AntTweakBar
		MousePosEvent(window, xPos, yPos);
	}
#else
	MousePosEvent(window, xPos, yPos);
#endif
	//GetCoreEngine()->CentralizeCursor();
}

void ScrollEvent(GLFWwindow* window, double xOffset, double yOffset)
{

}

void ScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	INFO_LOG_RENDERING_TEST("Scroll event callback");
#ifdef ANT_TWEAK_BAR_ENABLED
	if (!TwEventMouseWheelGLFW(static_cast<int>(yOffset))) // TODO: Check if yOffset here is ok
	{
		// event has not been handled by AntTweakBar
		ScrollEvent(window, xOffset, yOffset);
	}
#else
	ScrollEvent(window, xOffset, yOffset);
#endif
}

void ErrorCallback(int errorCode, const char* description)
{
	switch (errorCode)
	{
	case GLFW_NOT_INITIALIZED:
		ERROR_LOG_RENDERING_TEST("GLFW has not been initialized. Error description: ", description);
		break;
	case GLFW_NO_CURRENT_CONTEXT:
		ERROR_LOG_RENDERING_TEST("No context is current for this thread. Error description: ", description);
		break;
	case GLFW_INVALID_ENUM:
		ERROR_LOG_RENDERING_TEST("One of the arguments to the function was an invalid enum value. Error description: ", description);
		break;
	case GLFW_INVALID_VALUE:
		ERROR_LOG_RENDERING_TEST("One of the arguments to the function was an invalid value. Error description: ", description);
		break;
	case GLFW_OUT_OF_MEMORY:
		ERROR_LOG_RENDERING_TEST("A memory allocation failed. Error description: ", description);
		break;
	case GLFW_API_UNAVAILABLE:
		ERROR_LOG_RENDERING_TEST("GLFW could not find support for the requested client API on the system. Error description: ", description);
		break;
	case GLFW_VERSION_UNAVAILABLE:
		ERROR_LOG_RENDERING_TEST("The requested OpenGL or OpenGL ES version is not available. Error description: ", description);
		break;
	case GLFW_PLATFORM_ERROR:
		ERROR_LOG_RENDERING_TEST("A platform-specific error occurred that does not match any of the more specific categories. Error description: ", description);
		break;
	case GLFW_FORMAT_UNAVAILABLE:
		ERROR_LOG_RENDERING_TEST("The requested format is not supported or available. Error description: ", description);
		break;
	default:
		CRITICAL_LOG_RENDERING_TEST("Unknown GLFW error event occurred with code ", errorCode, " and message: Error description: ", description);
	}
	exit(EXIT_FAILURE);
}

void InitGlfw(bool fullscreenEnabled, int width, int height, const string& title, aliasing::AntiAliasingMethod antiAliasingMethod)
{
	DEBUG_LOG_RENDERING_TEST("Initializing GLFW started");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(glfwInit(), utility::logging::CRITICAL, "Failed to initialize GLFW.");

	const auto antiAliasingSamples = 4; // TODO: This parameter belongs in the Rendering module. The config value should also be retrieved from the rendering configuration file.
	switch (antiAliasingMethod)
	{
	case aliasing::NONE:
		/**
		* TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		* Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		*/
		glfwWindowHint(GLFW_SAMPLES, 0);
		INFO_LOG_RENDERING_TEST("No anti-aliasing algorithm chosen");
		break;
	case aliasing::FXAA:
		/**
		* TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		* Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		*/
		glfwWindowHint(GLFW_SAMPLES, 0);
		INFO_LOG_RENDERING_TEST("FXAA anti-aliasing algorithm chosen");
		break;
	case aliasing::MSAA:
		glfwWindowHint(GLFW_SAMPLES, antiAliasingSamples);
		INFO_LOG_RENDERING_TEST(antiAliasingSamples, "xMSAA anti-aliasing algorithm chosen");
		break;
	default:
		WARNING_LOG_RENDERING_TEST("Unknown anti-aliasing algorithm chosen. Default ", antiAliasingSamples, "xMSAA algorithm chosen");
		glfwWindowHint(GLFW_SAMPLES, antiAliasingSamples);
	}
	glfwWindowHint(GLFW_VERSION_MAJOR, 3); // TODO: Do not hard-code any values
	glfwWindowHint(GLFW_VERSION_MINOR, 3); // TODO: Do not hard-code any values
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // So that glBegin / glEnd etc. work
#else
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GL_TRUE);

	//int monitorCount;
	//GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	GLFWmonitor* monitor = nullptr;
	if (fullscreenEnabled)
	{
		monitor = glfwGetPrimaryMonitor();
	}
	window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr); // Open a window and create its OpenGL context
	if (window == nullptr)
	{
		CRITICAL_LOG_RENDERING_TEST("Failed to create GLFW main window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	threadWindow = glfwCreateWindow(1, 1, "Thread Window", nullptr, window);
	if (threadWindow == nullptr)
	{
		CRITICAL_LOG_RENDERING_TEST("Failed to create GLFW thread window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we can capture the escape key being pressed below
	glfwSetCursorPos(window, width / 2, height / 2); // Set cursor position to the middle point
													 //glfwSwapInterval(1);
	glfwSetTime(REAL_ZERO);
	DEBUG_LOG_RENDERING_TEST("Initializing GLFW finished successfully");
}

void InitGlew()
{
	INFO_LOG_RENDERING_TEST("Initializing GLEW started");
	glewExperimental = true; // Needed in core profile
	GLenum err = glewInit();

	CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(GLEW_OK == err, utility::logging::EMERGENCY, "Error while initializing GLEW: ", glewGetErrorString(err));
	if (GLEW_VERSION_2_0)
	{
		DEBUG_LOG_RENDERING_TEST("OpenGL 2.0 supported");
	}
	else
	{
		ERROR_LOG_RENDERING_TEST("Initializing GLEW failed. OpenGL 2.0 NOT supported");
		exit(EXIT_FAILURE);
	}

	INFO_LOG_RENDERING_TEST("Using GLEW version ", glewGetString(GLEW_VERSION));
	//CheckErrorCode(__FUNCTION__, "Initializing GLEW");
}

void SetCallbacks()
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(window != NULL, utility::logging::CRITICAL, "Setting GLFW callbacks failed. The window is NULL.");
	glfwSetWindowCloseCallback(window, &WindowCloseEventCallback);
	glfwSetWindowSizeCallback(window, &WindowResizeCallback);
	glfwSetKeyCallback(window, &KeyEventCallback);
	//glfwSetCharCallback(window, &CharEventCallback);
	//glfwSetMousePosCallback(window, &MouseMotionCallback);
	glfwSetCursorPosCallback(window, &MousePosCallback);
	glfwSetMouseButtonCallback(window, &MouseEventCallback);
	glfwSetScrollCallback(window, &ScrollEventCallback);
}

void CreateRenderer(bool fullscreenEnabled, int width, int height, const string& title, aliasing::AntiAliasingMethod antiAliasingMethod)
{
	InitGlfw(fullscreenEnabled, width, height, title, antiAliasingMethod);
	InitGlew();
	SetCallbacks();

	InitGraphics(width, height, antiAliasingMethod);

	glfwSetErrorCallback(&ErrorCallback);
	//DEBUG_LOG_ENGINE("Thread window address: ", threadWindow);
	NOTICE_LOG_RENDERING_TEST("Creating Renderer instance started");
	renderer = std::make_unique<Renderer>(width, height, MODELS_DIR, TEXTURES_DIR, SHADERS_DIR, FONTS_DIR, antiAliasingMethod);
	NOTICE_LOG_RENDERING_TEST("Creating Renderer instance finished");

	CHECK_CONDITION_EXIT_RENDERING_TEST(renderer != NULL, utility::logging::CRITICAL, "Failed to create a renderer.");
}

void ReportError(const string& reportStr)
{
	ERROR_LOG_RENDERING_TEST("Test #", testNumber, " FAILED. ", reportStr);
}

void TestReport(bool statusCode /* false if error */, const string& reportErrorStr)
{
	testNumber++;
	if (statusCode == false)
	{
		ReportError(reportErrorStr);
	}
	INFO_LOG_RENDERING_TEST("Test #", testNumber, " passed");
}

void TimeReport(const string& reportStr, timing::Timer& timer, timing::TimeUnit timeUnit, const unsigned int NUMBER_OF_ITERATIONS = 1)
{
	//CHECK_CONDITION_EXIT_ALWAYS_MATH_TEST(!timer.IsRunning(), Logging::ERR, "Timer is still running");
	const auto elapsedTime = static_cast<double>(timer.GetDuration(timeUnit)) / NUMBER_OF_ITERATIONS;
	//timeSpan.AdjustUnitToValue();
	NOTICE_LOG_RENDERING_TEST(reportStr, ":\t", elapsedTime, " ", timing::DateTime::ConvertTimeUnitToString(timeUnit));
}

void CameraBuilderTest()
{
	if (!cameraBuilderTestEnabled)
	{
		return;
	}

	OrthoCameraBuilder orthoCameraBuilder;
	BuilderDirector<Camera> cameraBuilderDirector(&orthoCameraBuilder);
	camera = std::make_unique<Camera>(cameraBuilderDirector.Construct());
	NOTICE_LOG_RENDERING_TEST(*camera);
	camera = std::make_unique<Camera>(orthoCameraBuilder.SetPos(4.0f, 4.0f, 0.0f).SetNearPlane(0.1f).SetFarPlane(10000.0f));
	//NOTICE_LOG_RENDERING_TEST(camera);

	PerspectiveCameraBuilder perspectiveCameraBuilder;
	perspectiveCameraBuilder.SetAspectRatio(4.0f / 3.0f).SetFieldOfView(Angle(110.0f)).
		SetFarPlane(1000.0f).SetNearPlane(0.1f).SetPos(0.0f, 0.0f, -3.0f);
	cameraBuilderDirector.SetBuilder(&perspectiveCameraBuilder);
	camera = std::make_unique<Camera>(cameraBuilderDirector.Construct());
	NOTICE_LOG_RENDERING_TEST(*camera);
}

void LightBuilderTest()
{
	if (!lightBuilderTestEnabled)
	{
		return;
	}

	lighting::DirectionalLightBuilder directionalLightBuilder;
	BuilderDirector<lighting::DirectionalLight> directionalLightBuilderDirector(&directionalLightBuilder);
	const auto directionalLight = directionalLightBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(directionalLight);
}

Real GetTime()
{
	return static_cast<Real>(glfwGetTime());
	//return Time(glfwGetTime());

	//return timing::DateTime::Now();
}

void ParticlesSystemBuilderTest()
{
	if (!particlesSystemBuilderTestEnabled)
	{
		return;
	}

	particles::ParticlesSystemBuilder particlesSystemBuilder;
	BuilderDirector<particles::ParticlesSystem> particlesSystemBuilderDirector(&particlesSystemBuilder);
	particlesSystem = make_unique<particles::ParticlesSystem>(particlesSystemBuilderDirector.Construct());
	NOTICE_LOG_RENDERING_TEST(*particlesSystem);

	particlesSystemBuilder.SetMaxCount(10).SetAttributesMask(particles::attributes::POSITION | particles::attributes::COLOR).
		SetTextureId(texture_ids::INVALID).SetShaderId(shader_ids::PARTICLES_COLORS);

	particles::ParticlesEmitter particlesEmitter(2.0f);
	particlesEmitter.AddGenerator(make_unique<particles::generators::ConstantPositionGenerator>(REAL_ZERO, REAL_ZERO, REAL_ZERO));
	const auto colorsSet = { Color(color_ids::RED), Color(color_ids::GREEN), Color(color_ids::CYAN) };
	particlesEmitter.AddGenerator(make_unique<particles::generators::FromSetColorGenerator>(colorsSet));
	particlesSystemBuilder.AddEmitter(particlesEmitter);

	particlesSystemBuilder.AddUpdater(make_shared<particles::updaters::ConstantColorUpdater>(Color(color_ids::WHITE)));

	particlesKiller = make_unique<particles::TimerParticlesKiller>(1.2f);
	particlesSystemBuilder.SetKiller(particlesKiller.get());

	particlesSystem = make_unique<particles::ParticlesSystem>(particlesSystemBuilderDirector.Construct());

	NOTICE_LOG_RENDERING_TEST(*particlesSystem);

	auto previousTime = GetTime();
	for (auto i = 0; i < 6; ++i)
	{
		this_thread::sleep_for(chrono::milliseconds(400)); // Sleep for some time to prevent the thread from constant looping
		const auto currentTime = GetTime();
		const auto passedTime = currentTime - previousTime;
		previousTime = currentTime;
		CRITICAL_LOG_RENDERING_TEST("Passed time = ", passedTime);
		particlesSystem->Update(passedTime);
		NOTICE_LOG_RENDERING_TEST(*particlesSystem);
	}
}

#ifdef ANT_TWEAK_BAR_ENABLED
void InitializeTestTweakBars()
{
	AntTweakBarTypes::InitializeTweakBarTypes();

	TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	TwBar* coreEnginePropertiesBar = TwNewBar("RenderingTestPropertiesBar");
	TwAddVarRO(coreEnginePropertiesBar, "windowWidth", TW_TYPE_INT32, &WINDOW_WIDTH, " label='Window width' ");
	TwAddVarRO(coreEnginePropertiesBar, "windowHeight", TW_TYPE_INT32, &WINDOW_HEIGHT, " label='Window height' ");
	//TwAddVarRW(coreEnginePropertiesBar, "clockSpeed", TW_TYPE_REAL, &m_clockSpeed, " label='Clock speed' ");
	//TwAddVarRW(coreEnginePropertiesBar, "timeOfDay", TW_TYPE_REAL, &m_timeOfDay, " label='Time of day' ");

	//TwEnumVal daytimeEV[] = { { Utility::timing::NIGHT, "Night" }, { Utility::timing::BEFORE_DAWN, "Before dawn" }, { Utility::timing::SUNRISE, "Sunrise" },
	//	{ Utility::timing::DAY, "Day" }, { Utility::timing::SUNSET, "Sunset" }, { Utility::timing::AFTER_DUSK, "After dusk" } };
	//TwType daytimeType = TwDefineEnum("Daytime", daytimeEV, 6);
	//TwAddVarRW(coreEnginePropertiesBar, "daytime", daytimeType, &m_daytime, " label='Daytime' ");

	//TwAddVarRW(coreEnginePropertiesBar, "sunElevation", Rendering::angleType, &m_sunElevation, " label='Sun elevation' ");
	//TwAddVarRW(coreEnginePropertiesBar, "sunAzimuth", Rendering::angleType, &m_sunAzimuth, " label='Sun azimuth' ");
	//TwAddVarRW(coreEnginePropertiesBar, "sunFirstElevationLevel", Rendering::angleType, &M_FIRST_ELEVATION_LEVEL, " label='First elevation level' ");
	//TwAddVarRW(coreEnginePropertiesBar, "sunSecondElevationLevel", Rendering::angleType, &M_SECOND_ELEVATION_LEVEL, " label='Second elevation level' ");
	//TwAddVarRW(coreEnginePropertiesBar, "sunThirdElevationLevel", Rendering::angleType, &M_THIRD_ELEVATION_LEVEL, " label='Third elevation level' ");

	TwDefine(" RenderingTestPropertiesBar refresh=0.5 ");
	//double refreshRate = 0.2;
	//TwSetParam(coreEnginePropertiesBar, NULL, "refresh", TW_PARAM_DOUBLE, 1, &refreshRate);

	//TwSetParam(coreEnginePropertiesBar, NULL, "visible", TW_PARAM_CSTRING, 1, "false"); // Hide the bar at startup
}
#endif

Real CalculateHeightAt(int x, int z, const Image& heightMapImage, Real heightMapMaxHeight)
{
	constexpr Real MAX_PIXEL_COLOR = 255.0f; // The maximum value for color of the single pixel in the height map.

	Real height = static_cast<Real>(heightMapImage.GetPixelAt(z, x)) / MAX_PIXEL_COLOR;
	//CRITICAL_LOG_RENDERING("Height[", x, "][", z, "] = ", height);
	height = (height - 0.5f) * 2.0f * heightMapMaxHeight; // rescaling the height so that it is within range [-heightMapMaxHeight; heightMapMaxHeight].
	return height;
}

void CreateTerrain()
{
	terrainMaterial = std::make_unique<Material>(renderer->CreateTexture(test_texture_ids::TERRAIN_DIFFUSE, "grass4.jpg"), 1.0f,
		8.0f, renderer->CreateTexture(test_texture_ids::TERRAIN_NORMAL_MAP, "grass_normal.jpg"),
		renderer->CreateTexture(test_texture_ids::TERRAIN_DISPLACEMENT_MAP, "grass_disp.jpg"), 0.02f, -0.5f);
	terrainMaterial->SetAdditionalTexture(renderer->CreateTexture(test_texture_ids::TERRAIN_BLEND_MAP, "terrainBlendMap.png"), "blendMap");
	terrainMaterial->SetAdditionalTexture(renderer->CreateTexture(test_texture_ids::TERRAIN_DIFFUSE_2, "rocks2.jpg"), "diffuse2");
	terrainMaterial->SetAdditionalTexture(renderer->CreateTexture(test_texture_ids::TERRAIN_DIFFUSE_3, "mud.png"), "diffuse3");
	terrainMaterial->SetAdditionalTexture(renderer->CreateTexture(test_texture_ids::TERRAIN_DIFFUSE_4, "path.png"), "diffuse4");

	Image heightMapImage(renderer->GetTexturesDirectory() + "terrainHeightMapDebug7.png", STBI_grey /* we only care about one RED component for now (the heightmap is grayscale, so we could use GREEN or BLUE components as well) */);
	const Real heightMapMaxHeight = (heightMapImage.GetWidth() + heightMapImage.GetHeight()) / 40.0f;
	vector<Real> heights;
	heights.reserve(heightMapImage.GetWidth() * heightMapImage.GetHeight());
	for (auto z = heightMapImage.GetHeight() - 1; z >= 0; --z)
	{
		for (auto x = 0; x < heightMapImage.GetWidth(); ++x)
		{
			const auto terrainHeight = CalculateHeightAt(x, z, heightMapImage, heightMapMaxHeight);
			//CRITICAL_LOG_RENDERING("Height[", x, "][", z, "] = ", terrainHeight);
			heights.push_back(terrainHeight);
		}
	}
	Surface surface(Vector2D(REAL_ZERO, REAL_ZERO), heightMapImage.GetWidth() - 1, heightMapImage.GetHeight() - 1, heightMapImage.GetWidth(), heightMapImage.GetHeight(), heights.data());

	//terrainMesh = renderer->CreateMesh(TestMeshIDs::TERRAIN, "plane.obj");
	terrainMesh = renderer->CreateMeshFromSurface(test_mesh_ids::TERRAIN, surface);
	//terrainTransform.SetScale(2.0f);

	terrain = std::make_unique<Terrain>(surface, terrainTransform);
	//constexpr int NUMBER_OF_TERRAIN_HEIGHT_TEST_POSITIONS = 11;
	//std::array<Real, NUMBER_OF_TERRAIN_HEIGHT_TEST_POSITIONS> xPositions = { -8, 8, -2.0f, 2.0f, 0.0f, -1.0f, 1.0f, -0.5f, 0.5f, -0.2f, 0.2f };
	//std::array<Real, NUMBER_OF_TERRAIN_HEIGHT_TEST_POSITIONS> zPositions = { -3.0f, 3.0f, -1.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.2f, 0.2f, -0.25f, 0.25f };
	//for (int i = 0; i < NUMBER_OF_TERRAIN_HEIGHT_TEST_POSITIONS; ++i)
	//{
	//	for (int j = 0; j < NUMBER_OF_TERRAIN_HEIGHT_TEST_POSITIONS; ++j)
	//	{
	//		Real height = terrain->GetHeightAt(xPositions[i], zPositions[j]);
	//		ERROR_LOG_RENDERING_TEST("Height at position [", xPositions[i], "; ", zPositions[j], "] = ", height);
	//	}
	//}
}

void CreateSkybox()
{
	const auto cubeMapDayDirectory = "Skyboxes\\Skybox12";
	const auto cubeMapNightDirectory = "Skyboxes\\Skybox13";
	const auto skyboxTextureDay = renderer->CreateCubeTexture(test_texture_ids::SKYBOX_DAY, cubeMapDayDirectory);
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(skyboxTextureDay != nullptr, utility::logging::ERR, "Skybox day texture \"", cubeMapDayDirectory, "\" is NULL");
	const auto skyboxTextureNight = renderer->CreateCubeTexture(test_texture_ids::SKYBOX_NIGHT, cubeMapNightDirectory);
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(skyboxTextureNight != nullptr, utility::logging::ERR, "Skybox night texture \"", cubeMapNightDirectory, "\" is NULL");

	//SetTexture("cubeMapDay", m_skyboxTextureDay);
	//SetTexture("cubeMapNight", m_skyboxTextureNight);

	skyboxMaterial = std::make_unique<Material>(skyboxTextureDay, "cubeMapDay");
	skyboxMaterial->SetAdditionalTexture(skyboxTextureNight, "cubeMapNight");
	skyboxMesh = renderer->CreateMesh(test_mesh_ids::SKYBOX, "cube.obj");
}

void CreateCamera()
{
	OrthoCameraBuilder orthoCameraBuilder;
	orthoCameraBuilder.SetBottom(-10.0f).SetTop(10.0f).SetLeft(-10.0f).SetRight(10.0f).SetNearPlane(0.1f).SetFarPlane(100.0f).SetPos(0.0f, 0.0f, -3.0f).
		SetRot(Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE)).SetSensitivity(0.05f);
	PerspectiveCameraBuilder perspectiveCameraBuilder;
	perspectiveCameraBuilder.SetAspectRatio(WINDOW_WIDTH / WINDOW_HEIGHT).SetFieldOfView(Angle(70.0f)).
		SetFarPlane(1000.0f).SetNearPlane(0.1f).SetPos(1.0f, 0.0f, 0.0f).SetRot(Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE)).SetSensitivity(0.05f);
	BuilderDirector<Camera> cameraBuilderDirector(&perspectiveCameraBuilder);
	//BuilderDirector<Camera> cameraBuilderDirector(&orthoCameraBuilder);
	camera = std::make_unique<Camera>(cameraBuilderDirector.Construct());
	camera->GetPos().y = terrain->GetHeightAt(camera->GetPos().GetXz()) + 0.02f;
	NOTICE_LOG_RENDERING_TEST(*camera);
}

void CreateCubes()
{
	renderer->CreateTexture(test_texture_ids::CUBE_DIFFUSE, "chessboard3.jpg");
	cubeMaterial = std::make_unique<Material>(renderer->GetTexture(test_texture_ids::CUBE_DIFFUSE), 8.0f, 1.0f,
		renderer->GetTexture(texture_ids::DEFAULT_NORMAL_MAP), renderer->GetTexture(texture_ids::DEFAULT_DISPLACEMENT_MAP));
	cubeMesh = renderer->CreateMesh(test_mesh_ids::CUBE, "cube.obj");
	for (auto i = 0; i < CUBE_MESHES_ROWS; ++i)
	{
		for (auto j = 0; j < CUBE_MESHES_COLS; ++j)
		{
			cubeTransforms[i * CUBE_MESHES_ROWS + j].SetPos(0.1f * static_cast<Real>(j), 0.0f, 0.1f * static_cast<Real>(i));
			cubeTransforms[i * CUBE_MESHES_ROWS + j].SetPosY(terrain->GetHeightAt(cubeTransforms[i * CUBE_MESHES_ROWS + j].GetPos().GetXz()));
			cubeTransforms[i * CUBE_MESHES_ROWS + j].SetRot(Quaternion(REAL_ZERO, sqrtf(2.0f) / 2, sqrtf(2.0f) / 2, REAL_ZERO)
				/* to make the plane face towards the camera.
				See "OpenGL Game Rendering Tutorial: Shadow Mapping Preparations"
				https://www.youtube.com/watch?v=kyjDP68s9vM&index=8&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl (starts around 14:10) */);
			cubeTransforms[i * CUBE_MESHES_ROWS + j].SetScale(0.005f + i * j * 0.00001f);
		}
	}
}

void CreateParticlesSystem()
{
	particles::ParticlesSystemBuilder particlesSystemBuilder;
	particlesSystemBuilder.SetMaxCount(100000).SetAttributesMask(particles::attributes::POSITION | particles::attributes::COLOR | particles::attributes::SCALE | particles::attributes::ROTATION).
		SetMeshId(mesh_ids::PARTICLE_COLOR).SetTextureId(texture_ids::INVALID).SetShaderId(shader_ids::PARTICLES_COLORS);

	particles::ParticlesEmitter particlesEmitter(1501.0f);
	particlesEmitter.AddGenerator(make_unique<particles::generators::CirclePositionGenerator>(math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), 0.15f, 0.15f));
	const auto colorsSet = { Color(color_ids::RED), Color(color_ids::GREEN), Color(color_ids::CYAN), Color(color_ids::YELLOW), Color(color_ids::MAGENTA) };
	particlesEmitter.AddGenerator(make_unique<particles::generators::FromSetColorGenerator>(colorsSet));
	particlesEmitter.AddGenerator(make_unique<particles::generators::ConstantScaleGenerator>(0.01f));
	particlesEmitter.AddGenerator(make_unique<particles::generators::RangeRotationGenerator>(Angle(0.0f), Angle(90.0f)));
	particlesSystemBuilder.AddEmitter(particlesEmitter);

	particlesSystemBuilder.AddUpdater(make_shared<particles::updaters::ConstantMovementParticlesUpdater>(math::Vector3D(0.0f, 0.0f, 0.2f)));
	particlesSystemBuilder.AddUpdater(make_shared<particles::updaters::ConstantRotationParticlesUpdater>(math::Angle(180.0f)));
	//particlesSystemBuilder.AddUpdater(make_shared<particles::updaters::ConstantColorUpdater>(Color(color_ids::WHITE)));

	particlesKiller = make_unique<particles::TimerParticlesKiller>(86.2f);
	particlesSystemBuilder.SetKiller(particlesKiller.get());

	BuilderDirector<particles::ParticlesSystem> particlesSystemBuilderDirector(&particlesSystemBuilder);
	particlesSystem = make_unique<particles::ParticlesSystem>(particlesSystemBuilderDirector.Construct());

	NOTICE_LOG_RENDERING_TEST(*particlesSystem);
}

void CreateScene()
{
	CreateTerrain();
	//CreateSkybox();
	CreateCamera();
	CreateCubes();

	//int bufferEntriesCount;
	//void* data = renderer->GetMesh(TestMeshIDs::CUBE)->GetBufferData(MeshBufferTypes::TEXTURE_COORDINATES, &bufferEntriesCount);
	//CHECK_CONDITION_ALWAYS_RENDERING_TEST(data != nullptr, utility::logging::ERR, "Data is nullptr.");
	//math::Vector2D* dataValues = static_cast<math::Vector2D*>(data);
	//for (int i = 0; i < bufferEntriesCount; ++i)
	//{
	//	ERROR_LOG_RENDERING("DataValues[", i, "] = ", dataValues[i], ".");
	//}

	CreateParticlesSystem();
}

//math::Angle angleStep(1.0f);

void UpdateScene(Real frameTime)
{
	//camera.GetTransform().Rotate(math::Vector3D(0.0f, 1.0f, 0.0f), angleStep);
	//NOTICE_LOG_RENDERING_TEST(camera);
	//camera.GetTransform().SetPosY(terrain->GetHeightAt(camera.GetTransform().GetPos().GetXZ()) + 0.01f);

	//skyboxTransform.SetPos(camera->GetPos());

	particlesSystem->Update(frameTime);
}

void RenderParticles()
{
	DEBUG_LOG_RENDERING_TEST("Rendering particles started");
	//if (!particlesSystem.GetTexture()->IsAdditive())
	//{
	//particlesSystem.SortParticles(renderer->GetCurrentCamera().GetPos());
	//}
	renderer->RenderParticles(*particlesSystem);
}

void RenderSkybox()
{
	//if (m_fogEnabled)
	//{
	//	STOP_PROFILING_GAME("");
	//	return;
	//}

	//glDisable(GL_DEPTH_TEST);
	renderer->SetCullFaceFront();
	/**
	* By default (GL_LESS) we tell OpenGL that an incoming fragment wins the depth test if its Z value is less than the stored one.
	* However, in the case of a skybox the Z value is always the far Z. The far Z is clipped when the depth test function is set to "less than".
	* To make it part of the scene we change the depth function to "less than or equal".
	*/
	renderer->SetDepthFuncLessOrEqual();
	const auto skyboxShaderId = shader_ids::SKYBOX;
	renderer->BindShader(skyboxShaderId);
	renderer->UpdateRendererUniforms(skyboxShaderId);

	renderer->Render(test_mesh_ids::SKYBOX, skyboxMaterial.get(), skyboxTransform, skyboxShaderId);

	renderer->SetDepthFuncDefault();
	renderer->SetCullFaceDefault();
	//glEnable(GL_DEPTH_TEST);
	//Rendering::CheckErrorCode("Renderer::Render", "Rendering skybox");
}

void RenderScene()
{
	renderer->InitRenderScene(Color(color_ids::GREY), 0.5f);
	renderer->SetCurrentCamera(camera.get());

	renderer->BindDisplayTexture();
	renderer->ClearScreen();

	renderer->BindShader(shader_ids::AMBIENT);
	renderer->UpdateRendererUniforms(shader_ids::AMBIENT);
	for (auto i = 0; i < CUBE_MESHES_ROWS * CUBE_MESHES_COLS; ++i)
	{
		renderer->Render(test_mesh_ids::CUBE, cubeMaterial.get(), cubeTransforms[i], shader_ids::AMBIENT);
	}
	renderer->BindShader(shader_ids::AMBIENT_TERRAIN);
	renderer->UpdateRendererUniforms(shader_ids::AMBIENT_TERRAIN);
	renderer->Render(test_mesh_ids::TERRAIN, terrainMaterial.get(), terrainTransform, shader_ids::AMBIENT_TERRAIN);

	RenderParticles();

	//RenderSkybox();

	renderer->FinalizeRenderScene(renderer->GetAntiAliasingMethod() == aliasing::FXAA ?
		shader_ids::FILTER_FXAA :
		shader_ids::FILTER_NULL);
}

void Run()
{
	constexpr auto THREAD_SLEEP_TIME = 10;
	constexpr auto MAX_FPS = 500.0f;
	constexpr auto FRAME_TIME = 1.0f / MAX_FPS;

	CreateScene();

	controls::GuiButtonControl fpsGuiButton("text", renderer->GetFont(text::font_ids::CANDARA), 1.25f, nullptr,
		ZERO_VECTOR_2D, Angle(45.0f), Vector2D(1.0f, 1.0f), 0.25f, Color(color_ids::RED),
		Color(color_ids::GREEN), Vector2D(0.0f, 0.005f), false, 0.5f, 0.1f, 0.4f, 0.2f);

#ifdef ANT_TWEAK_BAR_ENABLED
	InitializeTweakBars();
	InitializeTestTweakBars();
#endif

	const auto fpsSample = 1.0f; // represents the time after which FPS value is calculated and logged
	auto framesCount = 0;
	auto frameTimeCounter = REAL_ZERO;
	auto fps = 0;
	auto spf = REAL_ZERO;

	auto unprocessingTime = REAL_ZERO; // used to cap the FPS when it gets too high
	auto previousTime = GetTime();

	while (true)
	{
		/* ==================== REGION #1 begin ====================*/
		auto isRenderRequired = false;

		// flCurrentTime will be lying around from last frame. It's now the previous time.
		const auto currentTime = GetTime();
		const auto passedTime = currentTime - previousTime;
		DELOCUST_LOG_RENDERING_TEST("Passed time: ", passedTime * 1000.0f, " [ms]");

		previousTime = currentTime;
		unprocessingTime += passedTime;

		frameTimeCounter += passedTime;
		if (frameTimeCounter >= fpsSample)
		{
			fps = static_cast<int>(framesCount / fpsSample); // Frames Per Second
			spf = 1000 * frameTimeCounter / framesCount; // Seconds Per Frame
			//DEBUG_LOG_RENDERING_TEST("FPS = ", fps, "\t Average time per frame = ", spf, " [ms]");
			framesCount = 0;
			frameTimeCounter = REAL_ZERO;
		}
		/* ==================== REGION #1 end ====================*/

		/* ==================== REGION #2 begin ====================*/
		while (unprocessingTime > FRAME_TIME)
		{
			isRenderRequired = true;
			if (glfwWindowShouldClose(window) != 0)
			{
				return;
			}
			/* ==================== REGION #2_1 begin ====================*/
			glfwPollEvents();
			/* ==================== REGION #2_1 end ====================*/

			/* ==================== REGION #2_2 begin ====================*/
			//m_game->Input(m_inputMapping.GetMappedInput());
			//m_inputMapping.ClearActions();
			//m_inputMapping.ClearRanges();
			//m_game->Update(FRAME_TIME);
			UpdateScene(FRAME_TIME);
			fpsGuiButton.Update(FRAME_TIME);
			/* ==================== REGION #2_2 end ====================*/
#ifdef ANT_TWEAK_BAR_ENABLED
			CheckChangesAndUpdateGLState();
#endif
			unprocessingTime -= FRAME_TIME;
		}
		/* ==================== REGION #2 end ====================*/

		/* ==================== REGION #3 begin ====================*/
		if (isRenderRequired)
		{
			//m_renderer->SetReal("timeOfDay", m_timeOfDay);
			//m_game->Render(renderer.get());
			RenderScene();

			++framesCount;
			stringstream ss;
			ss << "FPS = " << fps << " SPF[ms] = " << setprecision(4) << spf; // TODO: This allocates memory which seemes unneccessary.
			fpsGuiButton.SetText(ss.str());
			renderer->RenderGuiControl(fpsGuiButton, shader_ids::GUI);
#ifdef ANT_TWEAK_BAR_ENABLED
			const auto resultCode = TwDraw();
			CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(resultCode == 1, utility::logging::ERR, "TwDraw() function failed with message: \"", TwGetLastError(), "\".");
#endif
			glfwSwapBuffers(window);
		}
		else
		{
			//INFO_LOG_ENGINE("Rendering is not required. Moving on...");
			this_thread::sleep_for(chrono::milliseconds(THREAD_SLEEP_TIME)); // Sleep for some time to prevent the thread from constant looping
		}
		/* ==================== REGION #3 end ====================*/
		//CRITICAL_LOG_ENGINE("STOP");
	}
}

int main(int argc, char* argv[])
{
	srand(static_cast<unsigned int>(time(nullptr)));
	const auto commandLineMapper = ICommandLineMapper::CreateCommandLineMapper(argc, argv);
	//if (commandLineMapper.IsPresent("-help"))
	//{
	//	PrintHelp();
	//	system("pause");
	//	return 0;
	//}
	logging::ILogger::GetLogger(MODULE_NAME).Fill(commandLineMapper->Get("-log", ""), logging::INFO);
	logging::ILogger::GetLogger("Rendering").Fill(commandLineMapper->Get("-logRendering", "Info"), logging::INFO);
	logging::ILogger::GetLogger("Math").Fill(commandLineMapper->Get("-logMath", "Info"), logging::INFO);
	logging::ILogger::GetLogger("Utility").Fill(commandLineMapper->Get("-logUtility", "Info"), logging::INFO);

	IConfig::CreateConfig("Rendering", commandLineMapper->Get("-configRendering", "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Config\\ConfigRendering.cfg"));

	STATS_STORAGE.StartTimer();

	CreateRenderer(false, WINDOW_WIDTH, WINDOW_HEIGHT, "3D rendering tests", aliasing::NONE);
	Run();

	//MeshTest();
	CameraBuilderTest();
	//TextureTest();
	LightBuilderTest();
	//ParticlesSystemBuilderTest();
	//OtherTests();

	STATS_STORAGE.StopTimer();
	STATS_STORAGE.PrintReport();

	logging::ILogger::GetLogger(MODULE_NAME).ResetConsoleColor();
	cout << "Bye!" << endl;

	glfwTerminate(); // Terminate GLFW

	return 0;
}
