#include "Def.h"

#include "Rendering\CameraBuilder.h"
#include "Rendering\LightBuilder.h"
#include "Rendering\ShaderFactory.h"
#include "Rendering\ParticlesSystemBuilder.h"
#include "Rendering\Renderer.h"
#include "Rendering\GuiButtonControl.h"
#include "Rendering\FontFactory.h"
#include "Rendering\Mesh.h"
#include "Rendering\MeshIDs.h"
#include "Rendering\Material.h"
#include "Rendering\Terrain.h"
#include "Rendering\stb_image.h"

#include "Math\Transform.h"
#include "Math\HeightsGenerator.h"
#include "Math\StatisticsStorage.h"
#include "Math\RandomGeneratorFactory.h"

#include "Utility\ICommandLineMapper.h"
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"
#include "Utility\Time.h"

#include <GLFW\glfw3.h>
#include <ctime>
#include <string>
#include <thread>
//#include <xmmintrin.h>
//#include <iostream>
//#include <fstream>

using namespace Rendering;
using namespace Math;
using namespace Utility;
using namespace std;

const string MODULE_NAME = "MathTest";
const string CONFIG_DIR = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Config\\";
const string MODELS_DIR = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Models\\";
const string SHADERS_DIR = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Shaders\\";
const string TEXTURES_DIR = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Textures\\";
const string FONTS_DIR = "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Fonts\\";
constexpr int WINDOW_WIDTH = 1600;
constexpr int WINDOW_HEIGHT = 900;
const Math::Random::RandomGenerator& g_randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::GeneratorIDs::SIMPLE);
GLFWwindow* window = nullptr;
GLFWwindow* threadWindow = nullptr;
std::unique_ptr<Renderer> renderer = nullptr;

bool cameraRotationEnabled = false;
Camera camera;

Particles::ParticlesSystem particlesSystem;

namespace TestMeshIDs
{
	enum TestMeshID
	{
		TERRAIN = MeshIDs::COUNT,
		CUBE
	};
}

namespace TestTextureIDs
{
	enum TestTextureID
	{
		TERRAIN_DIFFUSE = TextureIDs::COUNT,
		TERRAIN_DIFFUSE_2,
		TERRAIN_DIFFUSE_3,
		TERRAIN_DIFFUSE_4,
		TERRAIN_NORMAL_MAP,
		TERRAIN_DISPLACEMENT_MAP,
		TERRAIN_BLEND_MAP,
		CUBE_DIFFUSE
	};
}

constexpr int TERRAIN_VERTEX_COUNT = 128;
constexpr Math::Real TERRAIN_HEIGHT_GENERATOR_AMPLITUDE = 70.0f;
constexpr int TERRAIN_HEIGHT_GENERATOR_OCTAVES_COUNT = 3;
constexpr Math::Real TERRAIN_HEIGHT_GENERATOR_ROUGHNESS = 0.3f;
Transform terrainTransform;
std::unique_ptr<Material> terrainMaterial = nullptr;
const Mesh* terrainMesh = nullptr;
std::unique_ptr<Terrain> terrain;

constexpr int CUBE_MESHES_ROWS = 20;
constexpr int CUBE_MESHES_COLS = 20;
std::array<Transform, CUBE_MESHES_ROWS * CUBE_MESHES_COLS> cubeTransforms;
const Mesh* cubeMesh = nullptr;
std::unique_ptr<Material> cubeMaterial = nullptr;

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
	Real runModeSpeed = (mods == GLFW_MOD_SHIFT) ? 10.0f : 1.0f;
	const Quaternion rotation = camera.GetTransform().GetRot();
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
	}
	camera.GetTransform().IncreasePos(dirVector * camera.GetSensitivity() * runModeSpeed);
	//ERROR_LOG_RENDERING("camera.Pos = ", camera.GetTransform().GetPos().GetXZ(), "; height = ",
	//	terrain->GetHeightAt(camera.GetTransform().GetPos().GetXZ()));
	NOTICE_LOG_RENDERING_TEST(camera);
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
		cameraRotationEnabled = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		if (cameraRotationEnabled)
		{
			Math::Vector2D centerPosition(static_cast<Math::Real>(WINDOW_WIDTH) / 2, static_cast<Math::Real>(WINDOW_HEIGHT) / 2);
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
		Math::Vector2D centerPosition(static_cast<Math::Real>(WINDOW_WIDTH) / 2, static_cast<Math::Real>(WINDOW_HEIGHT) / 2);
		Math::Vector2D deltaPosition(static_cast<Math::Real>(xPos), static_cast<Math::Real>(yPos));
		deltaPosition -= centerPosition;

		bool rotX = !Math::AlmostEqual(deltaPosition.x, REAL_ZERO);
		bool rotY = !Math::AlmostEqual(deltaPosition.y, REAL_ZERO);

		if (rotX || rotY)
		{
			if (rotX)
			{
				camera.GetTransform().Rotate(Math::Vector3D(0, 1, 0), Math::Angle(deltaPosition.x * camera.GetSensitivity()));
			}
			if (rotY)
			{
				camera.GetTransform().Rotate(camera.GetTransform().GetRot().GetRight(), Math::Angle(deltaPosition.y * camera.GetSensitivity()));
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

void InitGlfw(bool fullscreenEnabled, int width, int height, const std::string& title, Rendering::Aliasing::AntiAliasingMethod antiAliasingMethod)
{
	DEBUG_LOG_RENDERING_TEST("Initializing GLFW started");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(glfwInit(), Utility::Logging::CRITICAL, "Failed to initialize GLFW.");

	const int antiAliasingSamples = 4; // TODO: This parameter belongs in the Rendering module. The config value should also be retrieved from the rendering configuration file.
	switch (antiAliasingMethod)
	{
	case Rendering::Aliasing::NONE:
		/**
		* TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		* Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		*/
		glfwWindowHint(GLFW_SAMPLES, 0);
		INFO_LOG_RENDERING_TEST("No anti-aliasing algorithm chosen");
		break;
	case Rendering::Aliasing::FXAA:
		/**
		* TODO: For this option it seems that when SwapBuffers() is called in Render function the screen blinks from time to time.
		* Why is it so? See http://www.glfw.org/docs/latest/window.html#window_hints
		*/
		glfwWindowHint(GLFW_SAMPLES, 0);
		INFO_LOG_RENDERING_TEST("FXAA anti-aliasing algorithm chosen");
		break;
	case Rendering::Aliasing::MSAA:
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

	GLFWmonitor* monitor = NULL;
	if (fullscreenEnabled)
	{
		monitor = glfwGetPrimaryMonitor();
	}
	window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL); // Open a window and create its OpenGL context
	if (window == NULL)
	{
		CRITICAL_LOG_RENDERING_TEST("Failed to create GLFW main window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	threadWindow = glfwCreateWindow(1, 1, "Thread Window", NULL, window);
	if (threadWindow == NULL)
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

	CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(GLEW_OK == err, Utility::Logging::EMERGENCY, "Error while initializing GLEW: ", glewGetErrorString(err));
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
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(window != NULL, Utility::Logging::CRITICAL, "Setting GLFW callbacks failed. The window is NULL.");
	glfwSetWindowCloseCallback(window, &WindowCloseEventCallback);
	glfwSetWindowSizeCallback(window, &WindowResizeCallback);
	glfwSetKeyCallback(window, &KeyEventCallback);
	//glfwSetCharCallback(window, &CharEventCallback);
	//glfwSetMousePosCallback(window, &MouseMotionCallback);
	glfwSetCursorPosCallback(window, &MousePosCallback);
	glfwSetMouseButtonCallback(window, &MouseEventCallback);
	glfwSetScrollCallback(window, &ScrollEventCallback);
}

void CreateRenderer(bool fullscreenEnabled, int width, int height, const std::string& title, Rendering::Aliasing::AntiAliasingMethod antiAliasingMethod)
{
	InitGlfw(fullscreenEnabled, width, height, title, antiAliasingMethod);
	InitGlew();
	SetCallbacks();

	Rendering::InitGraphics(width, height, antiAliasingMethod);

	glfwSetErrorCallback(&ErrorCallback);
	//DEBUG_LOG_ENGINE("Thread window address: ", threadWindow);
	NOTICE_LOG_RENDERING_TEST("Creating Renderer instance started");
	renderer = std::make_unique<Rendering::Renderer>(width, height, MODELS_DIR, TEXTURES_DIR, SHADERS_DIR, FONTS_DIR, antiAliasingMethod);
	NOTICE_LOG_RENDERING_TEST("Creating Renderer instance finished");

	CHECK_CONDITION_EXIT_RENDERING_TEST(renderer != NULL, Utility::Logging::CRITICAL, "Failed to create a renderer.");
}

void ReportError(const std::string& reportStr)
{
	ERROR_LOG_RENDERING_TEST("Test #", testNumber, " FAILED. ", reportStr);
}

void TestReport(bool statusCode /* false if error */, const std::string& reportErrorStr)
{
	testNumber++;
	if (statusCode == false)
	{
		ReportError(reportErrorStr);
	}
	INFO_LOG_RENDERING_TEST("Test #", testNumber, " passed");
}

void TimeReport(const std::string& reportStr, Timing::Timer& timer, Timing::TimeUnit timeUnit, const unsigned int NUMBER_OF_ITERATIONS = 1)
{
	//CHECK_CONDITION_EXIT_ALWAYS_MATH_TEST(!timer.IsRunning(), Logging::ERR, "Timer is still running");
	double elapsedTime = static_cast<double>(timer.GetDuration(timeUnit));
	elapsedTime /= NUMBER_OF_ITERATIONS;
	//timeSpan.AdjustUnitToValue();
	NOTICE_LOG_RENDERING_TEST(reportStr, ":\t", elapsedTime, " ", Timing::DateTime::ConvertTimeUnitToString(timeUnit));
}

void CameraBuilderTest()
{
	if (!cameraBuilderTestEnabled)
	{
		return;
	}

	OrthoCameraBuilder orthoCameraBuilder;
	BuilderDirector<Camera> cameraBuilderDirector(&orthoCameraBuilder);
	camera = cameraBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(camera);
	camera = orthoCameraBuilder.SetPos(4.0f, 4.0f, 0.0f).SetNearPlane(0.1f).SetFarPlane(10000.0f).Get();
	//NOTICE_LOG_RENDERING_TEST(camera);

	PerspectiveCameraBuilder perspectiveCameraBuilder;
	perspectiveCameraBuilder.SetAspectRatio(4.0f / 3.0f).SetFieldOfView(Math::Angle(110.0f)).
		SetFarPlane(1000.0f).SetNearPlane(0.1f).SetPos(0.0f, 0.0f, -3.0f);
	cameraBuilderDirector.SetBuilder(&perspectiveCameraBuilder);
	camera = cameraBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(camera);
}

void LightBuilderTest()
{
	if (!lightBuilderTestEnabled)
	{
		return;
	}

	DirectionalLightBuilder directionalLightBuilder;
	BuilderDirector<Lighting::DirectionalLight> directionalLightBuilderDirector(&directionalLightBuilder);
	Lighting::DirectionalLight directionalLight = directionalLightBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(directionalLight);
}

void ParticlesSystemBuilderTest()
{
	if (!particlesSystemBuilderTestEnabled)
	{
		return;
	}

	Particles::ParticlesSystemBuilder particlesSystemBuilder;
	BuilderDirector<Particles::ParticlesSystem> particlesSystemBuilderDirector(&particlesSystemBuilder);
	particlesSystem = particlesSystemBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(particlesSystem);

	particlesSystemBuilder.SetMaxCount(10).SetAttributesMask(Particles::Attributes::POSITION | Particles::Attributes::COLOR).
		SetTextureID(TextureIDs::INVALID).SetShaderID(ShaderIDs::PARTICLES_COLORS);
	particlesSystem = particlesSystemBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(particlesSystem);
}

Math::Real GetTime()
{
	return static_cast<Math::Real>(glfwGetTime());
	//return Time(glfwGetTime());

	//return Time::Now();
}

#ifdef ANT_TWEAK_BAR_ENABLED
void InitializeTestTweakBars()
{
	Rendering::AntTweakBarTypes::InitializeTweakBarTypes();

	TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	TwBar* coreEnginePropertiesBar = TwNewBar("RenderingTestPropertiesBar");
	TwAddVarRO(coreEnginePropertiesBar, "windowWidth", TW_TYPE_INT32, &WINDOW_WIDTH, " label='Window width' ");
	TwAddVarRO(coreEnginePropertiesBar, "windowHeight", TW_TYPE_INT32, &WINDOW_HEIGHT, " label='Window height' ");
	//TwAddVarRW(coreEnginePropertiesBar, "clockSpeed", TW_TYPE_REAL, &m_clockSpeed, " label='Clock speed' ");
	//TwAddVarRW(coreEnginePropertiesBar, "timeOfDay", TW_TYPE_REAL, &m_timeOfDay, " label='Time of day' ");

	//TwEnumVal daytimeEV[] = { { Utility::Timing::NIGHT, "Night" }, { Utility::Timing::BEFORE_DAWN, "Before dawn" }, { Utility::Timing::SUNRISE, "Sunrise" },
	//	{ Utility::Timing::DAY, "Day" }, { Utility::Timing::SUNSET, "Sunset" }, { Utility::Timing::AFTER_DUSK, "After dusk" } };
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

Real CalculateHeightAt(int x, int z, const Image& heightMapImage, Math::Real heightMapMaxHeight)
{
	constexpr Math::Real MAX_PIXEL_COLOR = 255.0f; // The maximum value for color of the single pixel in the height map.

	Math::Real height = static_cast<Math::Real>(heightMapImage.GetPixelAt(z, x)) / MAX_PIXEL_COLOR;
	//CRITICAL_LOG_RENDERING("Height[", x, "][", z, "] = ", height);
	height = (height - 0.5f) * 2.0f * heightMapMaxHeight; // rescaling the height so that it is within range [-heightMapMaxHeight; heightMapMaxHeight].
	return height;
}

void CreateTerrain()
{
	constexpr Real heightMapMaxHeight = 0.5f;
	Image heightMapImage(renderer->GetTexturesDirectory() + "terrainHeightMapDebug7.png", STBI_grey /* we only care about one RED component for now (the heightmap is grayscale, so we could use GREEN or BLUE components as well) */);
	std::vector<Math::Real> heights;
	heights.reserve(heightMapImage.GetWidth() * heightMapImage.GetHeight());
	for (int z = heightMapImage.GetHeight() - 1; z >= 0; --z)
	{
		const Math::Real zReal = static_cast<Math::Real>(z);
		for (int x = 0; x < heightMapImage.GetWidth(); ++x)
		{
			const Math::Real xReal = static_cast<Math::Real>(x);
			Math::Real terrainHeight = CalculateHeightAt(x, z, heightMapImage, heightMapMaxHeight);
			//CRITICAL_LOG_RENDERING("Height[", x, "][", z, "] = ", terrainHeight);
			heights.push_back(terrainHeight);
		}
	}
	Surface surface(Vector2D(REAL_ZERO, REAL_ZERO), 1, 1, heightMapImage.GetWidth(), heightMapImage.GetHeight(), heights.data());

	//terrainMesh = renderer->CreateMesh(TestMeshIDs::TERRAIN, "plane.obj");
	terrainMesh = renderer->CreateMeshFromSurface(TestMeshIDs::TERRAIN, surface);
	//terrainTransform.SetScale(2.0f);
}

void CreateScene()
{
	OrthoCameraBuilder orthoCameraBuilder;
	orthoCameraBuilder.SetBottom(-10.0f).SetTop(10.0f).SetLeft(-10.0f).SetRight(10.0f).SetNearPlane(0.1f).SetFarPlane(100.0f).SetPos(0.0f, 0.0f, -3.0f).
		SetRot(Math::Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE)).SetSensitivity(0.05f);
	PerspectiveCameraBuilder perspectiveCameraBuilder;
	perspectiveCameraBuilder.SetAspectRatio(WINDOW_WIDTH / WINDOW_HEIGHT).SetFieldOfView(Math::Angle(70.0f)).
		SetFarPlane(1000.0f).SetNearPlane(0.1f).SetPos(0.0f, 0.0f, -3.0f).SetRot(Math::Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE)).SetSensitivity(0.05f);
	BuilderDirector<Camera> cameraBuilderDirector(&perspectiveCameraBuilder);
	//BuilderDirector<Camera> cameraBuilderDirector(&orthoCameraBuilder);
	camera = cameraBuilderDirector.Construct();
	NOTICE_LOG_RENDERING_TEST(camera);

	terrainMaterial = std::make_unique<Material>(renderer->CreateTexture(TestTextureIDs::TERRAIN_DIFFUSE, "grass4.jpg"), 1.0f,
		8.0f, renderer->CreateTexture(TestTextureIDs::TERRAIN_NORMAL_MAP, "grass_normal.jpg"),
		renderer->CreateTexture(TestTextureIDs::TERRAIN_DISPLACEMENT_MAP, "grass_disp.jpg"), 0.02f, -0.5f);
	terrainMaterial->SetAdditionalTexture(renderer->CreateTexture(TestTextureIDs::TERRAIN_BLEND_MAP, "terrainBlendMap.png"), "blendMap");
	terrainMaterial->SetAdditionalTexture(renderer->CreateTexture(TestTextureIDs::TERRAIN_DIFFUSE_2, "rocks2.jpg"), "diffuse2");
	terrainMaterial->SetAdditionalTexture(renderer->CreateTexture(TestTextureIDs::TERRAIN_DIFFUSE_3, "mud.png"), "diffuse3");
	terrainMaterial->SetAdditionalTexture(renderer->CreateTexture(TestTextureIDs::TERRAIN_DIFFUSE_4, "path.png"), "diffuse4");

	CreateTerrain();

	int bufferEntriesCount;
	void* data = terrainMesh->GetBufferData(MeshBufferTypes::POSITIONS, &bufferEntriesCount);

	terrain = std::make_unique<Terrain>(static_cast<Real*>(data), bufferEntriesCount, terrainTransform);
	constexpr int NUMBER_OF_TERRAIN_HEIGHT_TEST_POSITIONS = 11;
	std::array<Real, NUMBER_OF_TERRAIN_HEIGHT_TEST_POSITIONS> xPositions = { -8, 8, -2.0f, 2.0f, 0.0f, -1.0f, 1.0f, -0.5f, 0.5f, -0.2f, 0.2f };
	std::array<Real, NUMBER_OF_TERRAIN_HEIGHT_TEST_POSITIONS> zPositions = { -3.0f, 3.0f, -1.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.2f, 0.2f, -0.25f, 0.25f };
	for (int i = 0; i < NUMBER_OF_TERRAIN_HEIGHT_TEST_POSITIONS; ++i)
	{
		for (int j = 0; j < NUMBER_OF_TERRAIN_HEIGHT_TEST_POSITIONS; ++j)
		{
			Real height = terrain->GetHeightAt(xPositions[i], zPositions[j]);
			ERROR_LOG_RENDERING_TEST("Height at position [", xPositions[i], "; ", zPositions[j], "] = ", height);
		}
	}

	renderer->CreateTexture(TestTextureIDs::CUBE_DIFFUSE, "chessboard3.jpg");
	cubeMaterial = std::make_unique<Material>(renderer->GetTexture(TestTextureIDs::CUBE_DIFFUSE), 8.0f, 1.0f,
		renderer->GetTexture(TextureIDs::DEFAULT_NORMAL_MAP), renderer->GetTexture(TextureIDs::DEFAULT_DISPLACEMENT_MAP));
	cubeMesh = renderer->CreateMesh(TestMeshIDs::CUBE, "cube.obj");
	for (int i = 0; i < CUBE_MESHES_ROWS; ++i)
	{
		for (int j = 0; j < CUBE_MESHES_COLS; ++j)
		{
			cubeTransforms[i * CUBE_MESHES_ROWS + j].SetPos(2.5f * static_cast<Math::Real>(j), 0.0f, 2.5f * static_cast<Math::Real>(i));
			cubeTransforms[i * CUBE_MESHES_ROWS + j].SetRot(Math::Quaternion(REAL_ZERO, sqrtf(2.0f) / 2, sqrtf(2.0f) / 2, REAL_ZERO)
				/* to make the plane face towards the camera.
				See "OpenGL Game Rendering Tutorial: Shadow Mapping Preparations"
				https://www.youtube.com/watch?v=kyjDP68s9vM&index=8&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl (starts around 14:10) */);
			cubeTransforms[i * CUBE_MESHES_ROWS + j].SetScale(0.5f + i * j * 0.01f);
		}
	}

	//int bufferEntriesCount;
	//void* data = renderer->GetMesh(TestMeshIDs::CUBE)->GetBufferData(MeshBufferTypes::TEXTURE_COORDINATES, &bufferEntriesCount);
	data = renderer->GetMesh(TestMeshIDs::CUBE)->GetBufferData(MeshBufferTypes::TEXTURE_COORDINATES, &bufferEntriesCount);
	CHECK_CONDITION_ALWAYS_RENDERING_TEST(data != nullptr, Utility::Logging::ERR, "Data is nullptr.");
	Math::Vector2D* dataValues = static_cast<Math::Vector2D*>(data);
	for (int i = 0; i < bufferEntriesCount; ++i)
	{
		ERROR_LOG_RENDERING("DataValues[", i, "] = ", dataValues[i], ".");
	}

	//Particles::ParticlesSystemBuilder particlesSystemBuilder;
	//particlesSystemBuilder.SetAttributesMask(Particles::Attributes::POSITION | Particles::Attributes::COLOR).SetMaxCount(10).
	//	SetShaderID(ShaderIDs::PARTICLES_COLORS);
	//Utility::BuilderDirector<Particles::ParticlesSystem> particlesSystemBuilderDirector(&particlesSystemBuilder);
	//particlesSystem = particlesSystemBuilderDirector.Construct();
}

//Math::Angle angleStep(1.0f);

void UpdateScene(Math::Real frameTime)
{
	//camera.GetTransform().Rotate(Math::Vector3D(0.0f, 1.0f, 0.0f), angleStep);
	//NOTICE_LOG_RENDERING_TEST(camera);

	//camera.GetTransform().SetPosY(terrain->GetHeightAt(camera.GetTransform().GetPos().GetXZ()) + 0.01f);
}

void RenderScene()
{
	renderer->InitRenderScene(Color(ColorIDs::GREY), 0.5f);
	renderer->SetCurrentCamera(&camera);

	renderer->BindDisplayTexture();
	renderer->ClearScreen();

	//renderer->BindShader(ShaderIDs::AMBIENT);
	//renderer->UpdateRendererUniforms(ShaderIDs::AMBIENT);
	//for (int i = 0; i < CUBE_MESHES_ROWS * CUBE_MESHES_COLS; ++i)
	//{
	//	renderer->Render(TestMeshIDs::CUBE, cubeMaterial.get(), cubeTransforms[i], ShaderIDs::AMBIENT);
	//}
	renderer->BindShader(ShaderIDs::AMBIENT_TERRAIN);
	renderer->UpdateRendererUniforms(ShaderIDs::AMBIENT_TERRAIN);
	renderer->Render(TestMeshIDs::TERRAIN, terrainMaterial.get(), terrainTransform, ShaderIDs::AMBIENT_TERRAIN);

	renderer->FinalizeRenderScene((renderer->GetAntiAliasingMethod() == Rendering::Aliasing::FXAA) ?
		Rendering::ShaderIDs::FILTER_FXAA :
		Rendering::ShaderIDs::FILTER_NULL);
}

void Run()
{
	constexpr int THREAD_SLEEP_TIME = 10;
	constexpr Math::Real MAX_FPS = 500.0f;
	constexpr Math::Real FRAME_TIME = 1.0f / MAX_FPS;

	CreateScene();

	Rendering::Controls::GuiButtonControl fpsGuiButton("text", renderer->GetFont(Text::FontIDs::CANDARA), 1.25f, NULL,
		ZERO_VECTOR_2D, Math::Angle(45.0f), Math::Vector2D(1.0f, 1.0f), 0.25f, Color(ColorIDs::RED),
		Color(ColorIDs::GREEN), Math::Vector2D(0.0f, 0.005f), false, 0.5f, 0.1f, 0.4f, 0.2f);

#ifdef ANT_TWEAK_BAR_ENABLED
	Rendering::InitializeTweakBars();
	InitializeTestTweakBars();
#endif

	Math::Real fpsSample = 1.0f; // represents the time after which FPS value is calculated and logged
	int framesCount = 0;
	Math::Real frameTimeCounter = REAL_ZERO;
	int fps = 0;
	Math::Real spf = REAL_ZERO;

	Math::Real unprocessingTime = REAL_ZERO; // used to cap the FPS when it gets too high
	Math::Real previousTime = GetTime();

	while (true)
	{
		/* ==================== REGION #1 begin ====================*/
		bool isRenderRequired = false;

		// flCurrentTime will be lying around from last frame. It's now the previous time.
		Math::Real currentTime = GetTime();
		Math::Real passedTime = currentTime - previousTime;
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
			Rendering::CheckChangesAndUpdateGLState();
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
			std::stringstream ss;
			ss << "FPS = " << fps << " SPF[ms] = " << std::setprecision(4) << spf; // TODO: This allocates memory which seemes unneccessary.
			fpsGuiButton.SetText(ss.str());
			renderer->RenderGuiControl(fpsGuiButton, Rendering::ShaderIDs::GUI);
#ifdef ANT_TWEAK_BAR_ENABLED
			int resultCode = TwDraw();
			CHECK_CONDITION_EXIT_ALWAYS_RENDERING_TEST(resultCode == 1, Utility::Logging::ERR, "TwDraw() function failed with message: \"", TwGetLastError(), "\".");
#endif
			glfwSwapBuffers(window);
		}
		else
		{
			//INFO_LOG_ENGINE("Rendering is not required. Moving on...");
			std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_SLEEP_TIME)); // Sleep for some time to prevent the thread from constant looping
		}
		/* ==================== REGION #3 end ====================*/
		//CRITICAL_LOG_ENGINE("STOP");
	}
}

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	std::unique_ptr<ICommandLineMapper> commandLineMapper = ICommandLineMapper::CreateCommandLineMapper(argc, argv);
	//if (commandLineMapper.IsPresent("-help"))
	//{
	//	PrintHelp();
	//	system("pause");
	//	return 0;
	//}
	Logging::ILogger::GetLogger(MODULE_NAME).Fill(commandLineMapper->Get("-log", ""), Logging::INFO);
	Logging::ILogger::GetLogger("Rendering").Fill(commandLineMapper->Get("-logRendering", "Info"), Logging::INFO);
	Logging::ILogger::GetLogger("Math").Fill(commandLineMapper->Get("-logMath", "Info"), Logging::INFO);
	Logging::ILogger::GetLogger("Utility").Fill(commandLineMapper->Get("-logUtility", "Info"), Logging::INFO);

	IConfig::CreateConfig("Rendering", commandLineMapper->Get("-configRendering", "C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Config\\ConfigRendering.cfg"));

	STATS_STORAGE.StartTimer();

	CreateRenderer(false, WINDOW_WIDTH, WINDOW_HEIGHT, "3D rendering tests", Rendering::Aliasing::NONE);
	Run();

	//MeshTest();
	CameraBuilderTest();
	//TextureTest();
	LightBuilderTest();
	ParticlesSystemBuilderTest();
	//OtherTests();

	STATS_STORAGE.StopTimer();
	STATS_STORAGE.PrintReport();

	Logging::ILogger::GetLogger(MODULE_NAME).ResetConsoleColor();
	std::cout << "Bye!" << std::endl;

	glfwTerminate(); // Terminate GLFW

	return 0;
}
