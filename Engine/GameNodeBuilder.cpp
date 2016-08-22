#include "StdAfx.h"
#include "GameNodeBuilder.h"
#include "CoreEngine.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "MeshRendererComponent.h"
#include "ParticleGeneratorComponent.h"
#include "Utility\IConfig.h"
#include "Utility\FileManager.h"

/* ==================== LightBuilder implementation begin ==================== */
Engine::LightBuilder::LightBuilder(GameManager* gameManager) :
	Utility::Builder<GameNode>(),
	m_lightIndex(0),
	m_lightIndexStr("0"),
	m_gameManager(gameManager)
{
}


Engine::LightBuilder::~LightBuilder(void)
{
}

void Engine::LightBuilder::BuildPart1()
{
	SetupLightTransform();
}

void Engine::LightBuilder::BuildPart2()
{
	SetupLightParams();
}

void Engine::LightBuilder::SetLightIndex(int lightIndex)
{
	m_lightIndex = lightIndex;
	std::stringstream ss("");
	ss << (m_lightIndex + 1);
	m_lightIndexStr = ss.str();
}
/* ==================== LightBuilder implementation end ==================== */


/* ==================== DirectionalLightBuilder implementation begin ==================== */
void Engine::DirectionalLightBuilder::SetupLightTransform()
{
	//m_object = std::make_unique<GameNode>();
	m_object = new GameNode();

	// Setting position
	const Math::Vector3D defaultDirectionalLightPos(GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightPosX", 0.0f), GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightPosY", 0.0f), GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightPosZ", 0.0f));
	Math::Real xPos = GET_CONFIG_VALUE_ENGINE("directionalLightPosX", defaultDirectionalLightPos.GetX());
	Math::Real yPos = GET_CONFIG_VALUE_ENGINE("directionalLightPosY", defaultDirectionalLightPos.GetY());
	Math::Real zPos = GET_CONFIG_VALUE_ENGINE("directionalLightPosZ", defaultDirectionalLightPos.GetZ());
	Math::Vector3D position(xPos, yPos, zPos);
	m_object->GetTransform().SetPos(position);


	// Setting rotation
	const Math::Angle defaultDirectionalLightRotationX(GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightAngleX", -45.0f), Math::Unit::DEGREE);
	const Math::Angle defaultDirectionalLightRotationY(GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightAngleY", REAL_ZERO), Math::Unit::DEGREE);
	const Math::Angle defaultDirectionalLightRotationZ(GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightAngleZ", REAL_ZERO), Math::Unit::DEGREE);
	Math::Angle angleX(GET_CONFIG_VALUE_ENGINE("directionalLightAngleX", defaultDirectionalLightRotationX.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
	Math::Angle angleY(GET_CONFIG_VALUE_ENGINE("directionalLightAngleY", defaultDirectionalLightRotationY.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
	Math::Angle angleZ(GET_CONFIG_VALUE_ENGINE("directionalLightAngleZ", defaultDirectionalLightRotationZ.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
	Math::Matrix4D rotMatrix(angleX, angleY, angleZ);
	DEBUG_LOG_ENGINE("angleX=", angleX.ToString(), ", angleY=", angleY.ToString(), ", angleZ=", angleZ.ToString(), ", rotMatrix =\n", rotMatrix.ToString());
	Math::Quaternion rot(rotMatrix);
	Math::Quaternion rot2(Math::Vector3D(1, 0, 0), angleX);
	//DEBUG_LOG_ENGINE("rotMatrix =\n", rotMatrix.ToString(), "\n rot =\n", rot.ToString(), "\n rot.ToRotationMatrix() =\n", rot.ToRotationMatrix().ToString(),
	//	"\n rot2.ToRotationMatrix() = \n", rot2.ToRotationMatrix().ToString());
	m_object->GetTransform().SetRot(rot);
	//m_gameNode->GetTransform().SetRot(Quaternion(Vector3D(1, 0, 0), Angle(90.0f)));
	//m_gameNode->GetTransform().Rotate(Vector3D(0, 1, 0), Angle(45.0f));
}

void Engine::DirectionalLightBuilder::SetupLightParams()
{
	// Setting light
	const Rendering::Color defaultDirectionalLightColor(GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightColorRed", REAL_ONE), GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightColorGreen", REAL_ONE), GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightColorBlue", REAL_ONE));
	const Math::Real defaultDirectionalLightIntensity(GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightIntensity", REAL_ONE));

	Math::Real red = GET_CONFIG_VALUE_ENGINE("directionalLightColorRed", defaultDirectionalLightColor.GetRed());
	Math::Real green = GET_CONFIG_VALUE_ENGINE("directionalLightColorGreen", defaultDirectionalLightColor.GetGreen());
	Math::Real blue = GET_CONFIG_VALUE_ENGINE("directionalLightColorBlue", defaultDirectionalLightColor.GetBlue());
	Rendering::Color color(red, green, blue);

	Math::Real intensity = GET_CONFIG_VALUE_ENGINE("directionalLightIntensity", defaultDirectionalLightIntensity);

	// Setting shadow info
	const Math::Real defaultDirectionalLightShadowArea = GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightShadowArea", 80.0f);
	const int defaultDirectionalLightShadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightShadowMapSizeAsPowerOf2", 10);
	const Math::Real defaultDirectionalLightShadowSoftness = GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightShadowSoftness", 1.0f);
	const Math::Real defaultDirectionalLightLightBleedingReductionAmount = GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightLightBleedingReductionAmount", 0.2f);
	const Math::Real defaultDirectionalLightMinVariance = GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightMinVariance", 0.00002f);
	Math::Real halfShadowArea = GET_CONFIG_VALUE_ENGINE("directionalLightShadowArea", defaultDirectionalLightShadowArea) / 2.0f;
	int shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE_ENGINE("directionalLightShadowMapSizeAsPowerOf2", defaultDirectionalLightShadowMapSizeAsPowerOf2);
	Math::Real shadowSoftness = GET_CONFIG_VALUE_ENGINE("directionalLightShadowSoftness", defaultDirectionalLightShadowSoftness);
	Math::Real lightBleedingReductionAmount = GET_CONFIG_VALUE_ENGINE("directionalLightLightBleedingReductionAmount", defaultDirectionalLightLightBleedingReductionAmount);
	Math::Real minVariance = GET_CONFIG_VALUE_ENGINE("directionalLightMinVariance", defaultDirectionalLightMinVariance);

	Rendering::Lighting::DirectionalLight* directionalLight = new Rendering::Lighting::DirectionalLight(m_object->GetTransform(), color, intensity, halfShadowArea, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance,
		m_gameManager->GetShader(ShaderTypes::DIRECTIONAL_LIGHT), m_gameManager->GetShader(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN), m_gameManager->GetShader(ShaderTypes::DIRECTIONAL_LIGHT_NO_SHADOWS),
		m_gameManager->GetShader(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS));

	// TODO: Add new shaders: "fogShader" and "fogTerrainShader".

	// Setting additional directional light information
	Math::Real maxIntensity = directionalLight->GetIntensity();
	const Rendering::Color sunlightDaytimeColor(directionalLight->GetColor());
	const Rendering::Color sunNearHorizonColor(Rendering::Color(GET_CONFIG_VALUE_ENGINE("directionalLightNearHorizonColorRed", REAL_ONE), GET_CONFIG_VALUE_ENGINE("directionalLightNearHorizonColorGreen", 0.2f), GET_CONFIG_VALUE_ENGINE("directionalLightNearHorizonColorBlue", REAL_ZERO)));
	const Rendering::Color sunlightNighttimeColor(Rendering::Color(GET_CONFIG_VALUE_ENGINE("directionalLightNighttimeColorRed", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("directionalLightNighttimeColorGreen", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("directionalLightNighttimeColorBlue", REAL_ZERO)));

	m_object->AddComponent(new DirectionalLightComponent(directionalLight, maxIntensity, sunlightDaytimeColor, sunNearHorizonColor, sunlightNighttimeColor,
		Math::Angle(GET_CONFIG_VALUE_ENGINE("latitude", 52.0f)), Math::Angle(GET_CONFIG_VALUE_ENGINE("longitude", -16.0f)),
		Math::Angle(GET_CONFIG_VALUE_ENGINE("sunlightFirstElevationLevel", -REAL_ONE)), Math::Angle(GET_CONFIG_VALUE_ENGINE("sunlightSecondElevationLevel", REAL_ZERO)),
		Math::Angle(GET_CONFIG_VALUE_ENGINE("sunlightThirdElevationLevel", REAL_ONE))));

	m_gameManager->AddLight(directionalLight);
}

#ifdef BUILD_MESH_RENDERER
void Engine::DirectionalLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around point light node position to let the user see the source
	//Material directionalLightMaterial("directionalLight_material", Texture("DirectionalLight.png"), 1, 8);
	//Material directionalLightLineMaterial("directionalLightLine_material", Texture("DirectionalLight.png"), 1, 8);

	m_gameNode->GetTransform().SetScale(0.4f); /* TODO: Don't use hardcoded values! Ever! */
	m_gameNode->AddComponent(new MeshRendererComponent(new Rendering::Mesh("DirectionalLight.obj"), new Rendering::Material(new Rendering::Texture("DirectionalLight.png"), 1.0f, 8.0f)));
		
	Math::Vector3D forwardVec = m_gameNode->GetTransform().GetTransformedRot().GetForward().Normalized();
	Math::Vector3D rayEndPosition = forwardVec * 2.0f;
	//DELOCUST_LOG_ENGINE("light position = ", m_gameNode->GetTransform().GetTransformedPos().ToString(), ";\t light rotation = ",
	//	m_gameNode->GetTransform().GetTransformedRot().ToString(), ";\t light forward vector = ", forwardVec.ToString(), ";\t light end pos = ",
	//	(m_gameNode->GetTransform().GetTransformedPos() + rayEndPosition).ToString());

	//Vertex vertices [] = { Vertex(Math::Vector3D()), Vertex(rayEndPosition) };
	//int indices [] = { 0, 1 };
	//m_gameNode->AddComponent(new MeshRenderer(
	//	new Mesh(vertices, 2, indices, 2, false, GL_LINES),
	//	new Material(new Texture("DirectionalLight.png"), 1.0f, 8.0f)));
}
#endif
/* ==================== DirectionalLightBuilder implementation end ==================== */

/* ==================== PointLightBuilder implementation begin ==================== */
Engine::PointLightBuilder::PointLightBuilder(GameManager* gameManager) :
	LightBuilder(gameManager),
	M_DEFAULT_POINT_LIGHT_POS(GET_CONFIG_VALUE_ENGINE("defaultPointLightPosX", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultPointLightPosY", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultPointLightPosZ", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X(GET_CONFIG_VALUE_ENGINE("defaultPointLightAngleX", -45.0f)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y(GET_CONFIG_VALUE_ENGINE("defaultPointLightAngleY", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z(GET_CONFIG_VALUE_ENGINE("defaultPointLightAngleZ", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_COLOR(GET_CONFIG_VALUE_ENGINE("defaultPointLightColorRed", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultPointLightColorGreen", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultPointLightColorBlue", REAL_ONE)),
	M_DEFAULT_POINT_LIGHT_INTENSITY(GET_CONFIG_VALUE_ENGINE("defaultPointLightIntensity", 10.0f)),
	M_DEFAULT_POINT_LIGHT_ATTENUATION(GET_CONFIG_VALUE_ENGINE("defaultPointLightAttenuationConstant", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultPointLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE_ENGINE("defaultPointLightAttenuationExponent", REAL_ZERO))
{
}

void Engine::PointLightBuilder::SetupLightTransform()
{
	//m_object = std::make_unique<GameNode>();
	m_object = new GameNode();

	// Setting position
	Math::Real xPos = GET_CONFIG_VALUE_ENGINE("pointLightPosX_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetX());
	Math::Real yPos = GET_CONFIG_VALUE_ENGINE("pointLightPosY_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetY());
	Math::Real zPos = GET_CONFIG_VALUE_ENGINE("pointLightPosZ_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetZ());
	m_object->GetTransform().SetPos(xPos, yPos, zPos);

	// Setting rotation
	Math::Angle angleX(GET_CONFIG_VALUE_ENGINE("pointLightAngleX_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X.Get(Math::Unit::DEGREE)));
	Math::Angle angleY(GET_CONFIG_VALUE_ENGINE("pointLightAngleY_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y.Get(Math::Unit::DEGREE)));
	Math::Angle angleZ(GET_CONFIG_VALUE_ENGINE("pointLightAngleZ_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z.Get(Math::Unit::DEGREE)));
	Math::Matrix4D rotMatrix(angleX, angleY, angleZ);
	Math::Quaternion rot(rotMatrix);
	m_object->GetTransform().SetRot(rot);
}

void Engine::PointLightBuilder::SetupLightParams()
{
	// Setting color
	Math::Real red = GET_CONFIG_VALUE_ENGINE("pointLightColorRed_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetRed());
	Math::Real green = GET_CONFIG_VALUE_ENGINE("pointLightColorGreen_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetGreen());
	Math::Real blue = GET_CONFIG_VALUE_ENGINE("pointLightColorBlue_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetBlue());
	Rendering::Color color(red, green, blue);

	// Setting intensity
	Math::Real intensity = GET_CONFIG_VALUE_ENGINE("pointLightIntensity_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_INTENSITY);

	// Setting attenuation
	Math::Real constant = GET_CONFIG_VALUE_ENGINE("pointLightAttenuationConstant_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetConstant());
	Math::Real linear = GET_CONFIG_VALUE_ENGINE("pointLightAttenuationLinear_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetLinear());
	Math::Real exponent = GET_CONFIG_VALUE_ENGINE("pointLightAttenuationExponent_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetExponent());
	Rendering::Attenuation attenuation(constant, linear, exponent);

	Rendering::Lighting::PointLight* pointLight = new Rendering::Lighting::PointLight(m_object->GetTransform(), color, intensity, attenuation,
		m_gameManager->GetShader(ShaderTypes::POINT_LIGHT), m_gameManager->GetShader(ShaderTypes::POINT_LIGHT_TERRAIN),
		m_gameManager->GetShader(ShaderTypes::POINT_LIGHT_NO_SHADOWS), m_gameManager->GetShader(ShaderTypes::POINT_LIGHT_TERRAIN_NO_SHADOWS));

	// Setting shadow info
	// Setting additional point light information

	m_object->AddComponent(new PointLightComponent(pointLight));

	m_gameManager->AddLight(pointLight);
}

#ifdef BUILD_MESH_RENDERER
void Engine::PointLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around point light node position to let the user see the source
	m_gameNode->AddComponent(new MeshRendererComponent(/* new Mesh("Bulb\\Bulb.obj") */ new Rendering::Mesh("PointLight.obj"), new Rendering::Material(new Rendering::Texture("PointLight.png"), 1.0f, 8.0f)));
	m_gameNode->GetTransform().SetScale(0.005f); /* TODO: Don't use hard-coded values! Ever! */
}
#endif
/* ==================== PointLightBuilder implementation end ==================== */

/* ==================== SpotLightBuilder implementation begin ==================== */
Engine::SpotLightBuilder::SpotLightBuilder(GameManager* gameManager) :
	LightBuilder(gameManager),
	M_DEFAULT_SPOT_LIGHT_POS(GET_CONFIG_VALUE_ENGINE("defaultSpotLightPosX", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultSpotLightPosY", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultSpotLightPosZ", REAL_ZERO)),
	M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_X(GET_CONFIG_VALUE_ENGINE("defaultSpotLightAngleX", -45.0f)),
	M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Y(GET_CONFIG_VALUE_ENGINE("defaultSpotLightAngleY", REAL_ZERO)),
	M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Z(GET_CONFIG_VALUE_ENGINE("defaultSpotLightAngleZ", REAL_ZERO)),
	M_DEFAULT_SPOT_LIGHT_COLOR(GET_CONFIG_VALUE_ENGINE("defaultSpotLightColorRed", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultSpotLightColorGreen", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultSpotLightColorBlue", REAL_ONE)),
	M_DEFAULT_SPOT_LIGHT_INTENSITY(GET_CONFIG_VALUE_ENGINE("defaultSpotLightIntensity", 4.0f)),
	M_DEFAULT_SPOT_LIGHT_ATTENUATION(GET_CONFIG_VALUE_ENGINE("defaultSpotLightAttenuationConstant", 0.5f), GET_CONFIG_VALUE_ENGINE("defaultSpotLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE_ENGINE("defaultSpotLightAttenuationExponent", 0.05f)),
	M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE(GET_CONFIG_VALUE_ENGINE("defaultSpotLightViewAngle", 120.0f), Math::Unit::DEGREE),
	M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2(GET_CONFIG_VALUE_ENGINE("defaultSpotLightShadowMapSizeAsPowerOf2", 10)), // 2 ^ 10 = 1024
	M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS(GET_CONFIG_VALUE_ENGINE("defaultSpotLightShadowSoftness", REAL_ONE)),
	M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT(GET_CONFIG_VALUE_ENGINE("defaultSpotLightLightBleedingReductionAmount", 0.2f)),
	M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE(GET_CONFIG_VALUE_ENGINE("defaultSpotLightMinVariance", 0.00002f))
{
}

void Engine::SpotLightBuilder::SetupLightTransform()
{
	//m_object = std::make_unique<GameNode>();
	m_object = new GameNode();

	// Setting position
	Math::Real xPos = GET_CONFIG_VALUE_ENGINE("spotLightPosX_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetX());
	Math::Real yPos = GET_CONFIG_VALUE_ENGINE("spotLightPosY_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetY());
	Math::Real zPos = GET_CONFIG_VALUE_ENGINE("spotLightPosZ_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetZ());
	m_object->GetTransform().SetPos(xPos, yPos, zPos);

	// Setting rotation
	Math::Angle angleX(GET_CONFIG_VALUE_ENGINE("spotLightAngleX_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_X.Get(Math::Unit::DEGREE)));
	Math::Angle angleY(GET_CONFIG_VALUE_ENGINE("spotLightAngleY_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Y.Get(Math::Unit::DEGREE)));
	Math::Angle angleZ(GET_CONFIG_VALUE_ENGINE("spotLightAngleZ_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Z.Get(Math::Unit::DEGREE)));
	Math::Matrix4D rotMatrix(angleX, angleY, angleZ);
	Math::Quaternion rot(rotMatrix);
	m_object->GetTransform().SetRot(rot);
}

void Engine::SpotLightBuilder::SetupLightParams()
{
	// Setting color
	Math::Real red = GET_CONFIG_VALUE_ENGINE("spotLightColorRed_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetRed());
	Math::Real green = GET_CONFIG_VALUE_ENGINE("spotLightColorGreen_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetGreen());
	Math::Real blue = GET_CONFIG_VALUE_ENGINE("spotLightColorBlue_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetBlue());
	Rendering::Color color(red, green, blue);

	// Setting intensity
	Math::Real intensity = GET_CONFIG_VALUE_ENGINE("spotLightIntensity_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_INTENSITY);

	// Setting attenuation
	Math::Real constant = GET_CONFIG_VALUE_ENGINE("spotLightAttenuationConstant_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetConstant());
	Math::Real linear = GET_CONFIG_VALUE_ENGINE("spotLightAttenuationLinear_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetLinear());
	Math::Real exponent = GET_CONFIG_VALUE_ENGINE("spotLightAttenuationExponent_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetExponent());
	Rendering::Attenuation attenuation(constant, linear, exponent);

	// Setting view angle
	Math::Angle viewAngle(GET_CONFIG_VALUE_ENGINE("spotLightViewAngle_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);

	// Setting shadow info
	int shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE_ENGINE("spotLightShadowMapSizeAsPowerOf2_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2); // 2 ^ 10 = 1024
	Math::Real shadowSoftness = GET_CONFIG_VALUE_ENGINE("spotLightShadowSoftness_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS);
	Math::Real lightBleedingReductionAmount = GET_CONFIG_VALUE_ENGINE("spotLightLightBleedingReductionAmount_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT);
	Math::Real minVariance = GET_CONFIG_VALUE_ENGINE("spotLightMinVariance_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE);

	Rendering::Lighting::SpotLight* spotLight = new Rendering::Lighting::SpotLight(m_object->GetTransform(), color, intensity, attenuation, viewAngle,
		shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance, m_gameManager->GetShader(ShaderTypes::SPOT_LIGHT),
		m_gameManager->GetShader(ShaderTypes::SPOT_LIGHT_TERRAIN), m_gameManager->GetShader(ShaderTypes::SPOT_LIGHT_NO_SHADOWS),
		m_gameManager->GetShader(ShaderTypes::POINT_LIGHT_TERRAIN_NO_SHADOWS));

	m_object->AddComponent(new SpotLightComponent(spotLight));

	m_gameManager->AddLight(spotLight);
}

#ifdef BUILD_MESH_RENDERER
void Engine::SpotLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around spot light node position to let the user see the source
	m_gameNode->AddComponent(new MeshRendererComponent(new Rendering::Mesh("SpotLight.obj"), new Rendering::Material(new Rendering::Texture("SpotLight.png"), 1.0f, 8.0f)));
	m_gameNode->GetTransform().SetScale(0.1f); /* TODO: Don't use hard-coded values! Ever! */
}
#endif
/* ==================== SpotLightBuilder implementation end ==================== */

/* ==================== CameraBuilder implementation begin ==================== */
Engine::CameraBuilder::CameraBuilder(GameManager* gameManager) :
	Utility::Builder<GameNode>(),
	m_gameManager(gameManager),
	M_DEFAULT_CAMERA_POS(GET_CONFIG_VALUE_ENGINE("defaultCameraPosX", 0.0f), GET_CONFIG_VALUE_ENGINE("defaultCameraPosY", 0.0f), GET_CONFIG_VALUE_ENGINE("defaultCameraPosZ", 0.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_X(GET_CONFIG_VALUE_ENGINE("defaultCameraAngleX", -45.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_Y(GET_CONFIG_VALUE_ENGINE("defaultCameraAngleY", 0.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_Z(GET_CONFIG_VALUE_ENGINE("defaultCameraAngleZ", 0.0f)),
	M_DEFAULT_CAMERA_FIELD_OF_VIEW(GET_CONFIG_VALUE_ENGINE("defaultCameraFoV", 70.0f)),
	M_DEFAULT_CAMERA_ASPECT_RATIO(GET_CONFIG_VALUE_ENGINE("defaultCameraAspectRatio", static_cast<Math::Real>(800) / 600)),
	M_DEFAULT_CAMERA_NEAR_PLANE(GET_CONFIG_VALUE_ENGINE("defaultCameraNearPlane", 0.1f)),
	M_DEFAULT_CAMERA_FAR_PLANE(GET_CONFIG_VALUE_ENGINE("defaultCameraFarPlane", 1000.0f)),
	M_DEFAULT_CAMERA_SENSITIVITY(GET_CONFIG_VALUE_ENGINE("defaultCameraSensitivity", 0.005f)),
	M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY(GET_CONFIG_VALUE_ENGINE("defaultCameraFollowEntityInitialDistance", 0.25f)),
	M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED(GET_CONFIG_VALUE_ENGINE("defaultCameraFollowAngleAroundEntitySpeed", 0.24f)),
	M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED(GET_CONFIG_VALUE_ENGINE("defaultCameraFollowPitchRotationSpeed", 0.1f)),
	M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE(GET_CONFIG_VALUE_ENGINE("defaultCameraFollowInitialPitchAngle", 30.0f)),
	m_cameraIndex(0),
	m_cameraIndexStr("0")
{
}

Engine::CameraBuilder::~CameraBuilder()
{
}

void Engine::CameraBuilder::SetCameraIndex(int cameraIndex)
{
	m_cameraIndex = cameraIndex;
	std::stringstream ss("");
	ss << (m_cameraIndex + 1);
	m_cameraIndexStr = ss.str();
}

void Engine::CameraBuilder::SetEntityToFollow(GameNode* gameNodeToFollow)
{
	m_gameNodeToFollow = gameNodeToFollow;
}

void Engine::CameraBuilder::BuildPart1()
{
	SetupCameraTransform();
}

void Engine::CameraBuilder::BuildPart2()
{
	SetupCameraParams();
}

void Engine::CameraBuilder::SetupCameraTransform()
{
	//m_object = std::make_unique<GameNode>();
	m_object = new GameNode();

	// Setting position
	Math::Real xPos = GET_CONFIG_VALUE_ENGINE("cameraPosX_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetX());
	Math::Real yPos = GET_CONFIG_VALUE_ENGINE("cameraPosY_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetY());
	Math::Real zPos = GET_CONFIG_VALUE_ENGINE("cameraPosZ_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetZ());
	m_object->GetTransform().SetPos(xPos, yPos, zPos);

	// Setting rotation
	Math::Angle angleX(GET_CONFIG_VALUE_ENGINE("cameraAngleX_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_X.Get(Math::Unit::DEGREE)));
	Math::Angle angleY(GET_CONFIG_VALUE_ENGINE("cameraAngleY_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Y.Get(Math::Unit::DEGREE)));
	Math::Angle angleZ(GET_CONFIG_VALUE_ENGINE("cameraAngleZ_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Z.Get(Math::Unit::DEGREE)));
	Math::Matrix4D rotMatrix(angleX, angleY, angleZ);
	DELOCUST_LOG_ENGINE("angleX=", angleX.ToString(), ", angleY=", angleY.ToString(), ", angleZ=", angleZ.ToString());
	Math::Quaternion rot(rotMatrix);
	m_object->GetTransform().SetRot(rot);
}

void Engine::CameraBuilder::SetupCameraParams()
{
	// Setting camera parameters
	Math::Angle fov(GET_CONFIG_VALUE_ENGINE("cameraFoV_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FIELD_OF_VIEW.Get(Math::Unit::DEGREE)));
	Math::Real aspectRatio = GET_CONFIG_VALUE_ENGINE("cameraAspectRatio_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ASPECT_RATIO);
	Math::Real zNearPlane = GET_CONFIG_VALUE_ENGINE("cameraNearPlane_" + m_cameraIndexStr, M_DEFAULT_CAMERA_NEAR_PLANE);
	Math::Real zFarPlane = GET_CONFIG_VALUE_ENGINE("cameraFarPlane_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FAR_PLANE);
	Math::Real sensitivity = GET_CONFIG_VALUE_ENGINE("cameraSensitivity_" + m_cameraIndexStr, M_DEFAULT_CAMERA_SENSITIVITY);

	Math::Real initialDistanceFromEntity = GET_CONFIG_VALUE_ENGINE("cameraFollowEntityInitialDistance_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY);
	Math::Real angleAroundEntitySpeed = GET_CONFIG_VALUE_ENGINE("cameraFollowAngleAroundEntitySpeed_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED);
	Math::Real pitchRotationSpeed = GET_CONFIG_VALUE_ENGINE("cameraFollowPitchRotationSpeed_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED);
	Math::Angle initialPitchAngle(GET_CONFIG_VALUE_ENGINE("cameraFollowInitialPitchAngle_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE.Get(Math::Unit::DEGREE)));

	Rendering::Camera* camera = new Rendering::Camera(m_object->GetTransform().GetTransformedPos(), m_object->GetTransform().GetTransformedRot(), fov, aspectRatio, zNearPlane, zFarPlane, sensitivity);
	Engine::CameraComponent* cameraComponent = new Engine::CameraFollowComponent(camera, m_gameNodeToFollow, initialDistanceFromEntity, angleAroundEntitySpeed, pitchRotationSpeed, initialPitchAngle);
	//Engine::CameraComponent* camera = new Engine::CameraMoveComponent(fov, aspectRatio, zNearPlane, zFarPlane, sensitivity);
	m_object->AddComponent(cameraComponent);
	m_gameManager->AddCamera(camera);
}

#ifdef BUILD_MESH_RENDERER
void Engine::CameraBuilder::BuildMeshRenderer()
{
	// Rendering a small box around camera node position to let the user see the camera
	//m_gameNode->AddComponent(new MeshRenderer(
	//	new Mesh("Camera.obj"),
	//	new Material(new Texture("Camera.png"), 1.0f, 8.0f)));
	//m_gameNode->GetTransform().SetScale(0.1f); /* TODO: Don't use hard-coded values! Ever! */
}
#endif
/* ==================== CameraBuilder implementation end ==================== */


/* ==================== SkyboxBuilder implementation begin ==================== */
Engine::SkyboxBuilder::SkyboxBuilder() :
	Builder()
{
}

Engine::SkyboxBuilder::~SkyboxBuilder()
{
}

void Engine::SkyboxBuilder::BuildPart1()
{
	std::string cubeMapDayDirectory = GET_CONFIG_VALUE_STR_ENGINE("skyboxDayDirectory", "SkyboxDebug");
	std::string cubeMapNightDirectory = GET_CONFIG_VALUE_STR_ENGINE("skyboxNightDirectory", "SkyboxDebug");
	Rendering::Texture* skyboxTextureDay = InitializeCubeMapTexture(cubeMapDayDirectory);
	Rendering::Texture* skyboxTextureNight = InitializeCubeMapTexture(cubeMapNightDirectory);

	//SetTexture("cubeMapDay", m_skyboxTextureDay);
	//SetTexture("cubeMapNight", m_skyboxTextureNight);

	Rendering::Material* skyboxMaterial = new Rendering::Material(skyboxTextureDay, "cubeMapDay");
	skyboxMaterial->SetAdditionalTexture(skyboxTextureNight, "cubeMapNight");

	//m_object = std::make_unique<GameNode>();
	m_object = new GameNode();
	m_object->GetTransform().SetPos(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	m_object->GetTransform().SetScale(5.0f); /* TODO: Don't use hardcoded values! Ever! */
	m_object->AddComponent(new MeshRendererComponent(new Rendering::Mesh(GET_CONFIG_VALUE_STR_ENGINE("skyboxModel", "cube.obj")), skyboxMaterial));

	CoreEngine::GetCoreEngine()->AddSkyboxNode(m_object);
}

void Engine::SkyboxBuilder::BuildPart2()
{
}

#ifdef BUILD_MESH_RENDERER
void Engine::SkyboxBuilder::BuildMeshRenderer()
{
}
#endif

Rendering::Texture* Engine::SkyboxBuilder::InitializeCubeMapTexture(const std::string& cubeMapTextureDirectory)
{
	//START_PROFILING("");
	const std::string DIRECTORY_PATH_SEPARATOR = "\\"; // for Windows it's "\", but for Unix it's "/"
	const std::string EXPECTED_POS_X_FACE_FILENAME = "right";
	const std::string EXPECTED_NEG_X_FACE_FILENAME = "left";
	const std::string EXPECTED_POS_Y_FACE_FILENAME = "up";
	const std::string EXPECTED_NEG_Y_FACE_FILENAME = "down";
	const std::string EXPECTED_POS_Z_FACE_FILENAME = "front";
	const std::string EXPECTED_NEG_Z_FACE_FILENAME = "back";

	Utility::FileManager fileManager;
	std::vector<std::string> filenames = fileManager.ListAllFilesInDirectory(CoreEngine::GetCoreEngine()->GetTexturesDirectory() + cubeMapTextureDirectory);
	bool cubeMapPosXFaceFileFound = false; std::string cubeMapPosXFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegXFaceFileFound = false; std::string cubeMapNegXFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapPosYFaceFileFound = false; std::string cubeMapPosYFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegYFaceFileFound = false; std::string cubeMapNegYFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapPosZFaceFileFound = false; std::string cubeMapPosZFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegZFaceFileFound = false; std::string cubeMapNegZFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	for (std::vector<std::string>::const_iterator filenameItr = filenames.begin(); filenameItr != filenames.end(); ++filenameItr)
	{
		if ((!cubeMapPosXFaceFileFound) && ((*filenameItr).find(EXPECTED_POS_X_FACE_FILENAME) != std::string::npos))
		{
			cubeMapPosXFaceFileFound = true;
			cubeMapPosXFaceFileName += (*filenameItr);
		}
		if ((!cubeMapNegXFaceFileFound) && ((*filenameItr).find(EXPECTED_NEG_X_FACE_FILENAME) != std::string::npos))
		{
			cubeMapNegXFaceFileFound = true;
			cubeMapNegXFaceFileName += (*filenameItr);
		}
		if ((!cubeMapPosYFaceFileFound) && ((*filenameItr).find(EXPECTED_POS_Y_FACE_FILENAME) != std::string::npos))
		{
			cubeMapPosYFaceFileFound = true;
			cubeMapPosYFaceFileName += (*filenameItr);
		}
		if ((!cubeMapNegYFaceFileFound) && ((*filenameItr).find(EXPECTED_NEG_Y_FACE_FILENAME) != std::string::npos))
		{
			cubeMapNegYFaceFileFound = true;
			cubeMapNegYFaceFileName += (*filenameItr);
		}
		if ((!cubeMapPosZFaceFileFound) && ((*filenameItr).find(EXPECTED_POS_Z_FACE_FILENAME) != std::string::npos))
		{
			cubeMapPosZFaceFileFound = true;
			cubeMapPosZFaceFileName += (*filenameItr);
		}
		if ((!cubeMapNegZFaceFileFound) && ((*filenameItr).find(EXPECTED_NEG_Z_FACE_FILENAME) != std::string::npos))
		{
			cubeMapNegZFaceFileFound = true;
			cubeMapNegZFaceFileName += (*filenameItr);
		}
	}
	CHECK_CONDITION_EXIT_ENGINE(cubeMapPosXFaceFileFound, Utility::ERR, "Cannot locate the right face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_ENGINE(cubeMapNegXFaceFileFound, Utility::ERR, "Cannot locate the left face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_ENGINE(cubeMapPosYFaceFileFound, Utility::ERR, "Cannot locate the up face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_ENGINE(cubeMapNegYFaceFileFound, Utility::ERR, "Cannot locate the down face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_ENGINE(cubeMapPosZFaceFileFound, Utility::ERR, "Cannot locate the front face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_ENGINE(cubeMapNegZFaceFileFound, Utility::ERR, "Cannot locate the back face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	Rendering::Texture* cubeMapTexture = new Rendering::Texture(cubeMapPosXFaceFileName, cubeMapNegXFaceFileName, cubeMapPosYFaceFileName, cubeMapNegYFaceFileName, cubeMapPosZFaceFileName, cubeMapNegZFaceFileName);
	if (cubeMapTexture == NULL)
	{
		ERROR_LOG_ENGINE("Cube map texture is NULL");
		exit(EXIT_FAILURE);
	}
	//STOP_PROFILING("");
	return cubeMapTexture;
}
/* ==================== CameraBuilder implementation end ==================== */