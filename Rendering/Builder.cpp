#include "StdAfx.h"
#include "Builder.h"
#include "CoreEngine.h"
#include "Shader.h"
#include "Utility\IConfig.h"

#ifdef BUILD_MESH_RENDERER
#include "MeshRenderer.h"
#endif

/* ==================== Builder implementation begin ==================== */
Rendering::Builder::Builder(void) :
	m_gameNode(NULL)
{
}


Rendering::Builder::~Builder(void)
{
}
/* ==================== Builder implementation end ==================== */

/* ==================== LightBuilder implementation begin ==================== */
Rendering::LightBuilder::LightBuilder(void) :
	Rendering::Builder(),
	m_lightIndex(0),
	m_lightIndexStr("0")
{
}


Rendering::LightBuilder::~LightBuilder(void)
{
}

void Rendering::LightBuilder::BuildPart1()
{
	SetupLightTransform();
}

void Rendering::LightBuilder::BuildPart2()
{
	SetupLightParams();
}

#ifdef BUILD_MESH_RENDERER
void Rendering::LightBuilder::BuildMeshRenderer()
{
}
#endif

void Rendering::LightBuilder::SetLightIndex(int lightIndex)
{
	m_lightIndex = lightIndex;
	std::stringstream ss("");
	ss << (m_lightIndex + 1);
	m_lightIndexStr = ss.str();
}
/* ==================== LightBuilder implementation end ==================== */


/* ==================== DirectionalLightBuilder implementation begin ==================== */
void Rendering::DirectionalLightBuilder::SetupLightTransform()
{
	m_gameNode = new GameNode();

	// Setting position
	const Math::Vector3D defaultDirectionalLightPos(GET_CONFIG_VALUE("defaultDirectionalLightPosX", 0.0f), GET_CONFIG_VALUE("defaultDirectionalLightPosY", 0.0f), GET_CONFIG_VALUE("defaultDirectionalLightPosZ", 0.0f));
	Math::Real xPos = GET_CONFIG_VALUE("directionalLightPosX", defaultDirectionalLightPos.GetX());
	Math::Real yPos = GET_CONFIG_VALUE("directionalLightPosY", defaultDirectionalLightPos.GetY());
	Math::Real zPos = GET_CONFIG_VALUE("directionalLightPosZ", defaultDirectionalLightPos.GetZ());
	Math::Vector3D position(xPos, yPos, zPos);
	m_gameNode->GetTransform().SetPos(position);


	// Setting rotation
	const Math::Angle defaultDirectionalLightRotationX(GET_CONFIG_VALUE("defaultDirectionalLightAngleX", -45.0f));
	const Math::Angle defaultDirectionalLightRotationY(GET_CONFIG_VALUE("defaultDirectionalLightAngleY", REAL_ZERO));
	const Math::Angle defaultDirectionalLightRotationZ(GET_CONFIG_VALUE("defaultDirectionalLightAngleZ", REAL_ZERO));
	Math::Angle angleX(GET_CONFIG_VALUE("directionalLightAngleX", defaultDirectionalLightRotationX.GetAngleInDegrees()));
	Math::Angle angleY(GET_CONFIG_VALUE("directionalLightAngleY", defaultDirectionalLightRotationY.GetAngleInDegrees()));
	Math::Angle angleZ(GET_CONFIG_VALUE("directionalLightAngleZ", defaultDirectionalLightRotationZ.GetAngleInDegrees()));
	Math::Matrix4D rotMatrix(angleX, angleY, angleZ);
	DEBUG_LOG("angleX=%.1f, angleY=%.1f, angleZ=%.1f, rotMatrix =\n%s", angleX.GetAngleInDegrees(), angleY.GetAngleInDegrees(), angleZ.GetAngleInDegrees(), rotMatrix.ToString().c_str());
	Math::Quaternion rot(rotMatrix);
	Math::Quaternion rot2(Math::Vector3D(1, 0, 0), angleX);
	//DEBUG_LOG("rotMatrix =\n%s\n rot =\n%s\n rot.ToRotationMatrix() =\n%s\n rot2.ToRotationMatrix() = \n%s",
	//	rotMatrix.ToString().c_str(),
	//	rot.ToString().c_str(),
	//	rot.ToRotationMatrix().ToString().c_str(),
	//	rot2.ToRotationMatrix().ToString().c_str());
	m_gameNode->GetTransform().SetRot(rot);
	//m_gameNode->GetTransform().SetRot(Quaternion(Vector3D(1, 0, 0), Angle(90.0f)));
	//m_gameNode->GetTransform().Rotate(Vector3D(0, 1, 0), Angle(45.0f));
}

void Rendering::DirectionalLightBuilder::SetupLightParams()
{
	// Setting light
	const Color defaultDirectionalLightColor(GET_CONFIG_VALUE("defaultDirectionalLightColorRed", REAL_ONE), GET_CONFIG_VALUE("defaultDirectionalLightColorGreen", REAL_ONE), GET_CONFIG_VALUE("defaultDirectionalLightColorBlue", REAL_ONE));
	const Math::Real defaultDirectionalLightIntensity(GET_CONFIG_VALUE("defaultDirectionalLightIntensity", REAL_ONE));

	Math::Real red = GET_CONFIG_VALUE("directionalLightColorRed", defaultDirectionalLightColor.GetRed());
	Math::Real green = GET_CONFIG_VALUE("directionalLightColorGreen", defaultDirectionalLightColor.GetGreen());
	Math::Real blue = GET_CONFIG_VALUE("directionalLightColorBlue", defaultDirectionalLightColor.GetBlue());
	Color color(red, green, blue);

	Math::Real intensity = GET_CONFIG_VALUE("directionalLightIntensity", defaultDirectionalLightIntensity);

	// Setting shadow info
	const Math::Real defaultDirectionalLightShadowArea = GET_CONFIG_VALUE("defaultDirectionalLightShadowArea", 80.0f);
	const int defaultDirectionalLightShadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE("defaultDirectionalLightShadowMapSizeAsPowerOf2", 10);
	const Math::Real defaultDirectionalLightShadowSoftness = GET_CONFIG_VALUE("defaultDirectionalLightShadowSoftness", 1.0f);
	const Math::Real defaultDirectionalLightLightBleedingReductionAmount = GET_CONFIG_VALUE("defaultDirectionalLightLightBleedingReductionAmount", 0.2f);
	const Math::Real defaultDirectionalLightMinVariance = GET_CONFIG_VALUE("defaultDirectionalLightMinVariance", 0.00002f);
	Math::Real halfShadowArea = GET_CONFIG_VALUE("directionalLightShadowArea", defaultDirectionalLightShadowArea) / 2.0f;
	int shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE("directionalLightShadowMapSizeAsPowerOf2", defaultDirectionalLightShadowMapSizeAsPowerOf2);
	Math::Real shadowSoftness = GET_CONFIG_VALUE("directionalLightShadowSoftness", defaultDirectionalLightShadowSoftness);
	Math::Real lightBleedingReductionAmount = GET_CONFIG_VALUE("directionalLightLightBleedingReductionAmount", defaultDirectionalLightLightBleedingReductionAmount);
	Math::Real minVariance = GET_CONFIG_VALUE("directionalLightMinVariance", defaultDirectionalLightMinVariance);

	Lighting::DirectionalLight* directionalLight = new Lighting::DirectionalLight(color, intensity, halfShadowArea, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance);

	// Setting shaders
	directionalLight->SetShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShader", "forward-directional")));
	directionalLight->SetTerrainShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightTerrainShader", "forward-directional-terrain")));
	directionalLight->SetNoShadowShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightNoShadowShader", "forward-directional-no-shadows")));
	directionalLight->SetNoShadowTerrainShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightNoShadowTerrainShader", "forward-directional-terrain-no-shadows")));
	// TODO: Add new shaders: "fogShader" and "fogTerrainShader".

	// Setting additional directional light information
	directionalLight->SetMaxIntensity(directionalLight->GetIntensity());
	directionalLight->SetSunlightDaytimeColor(directionalLight->GetColor());
	directionalLight->SetSunNearHorizonColor(Color(GET_CONFIG_VALUE("directionalLightNearHorizonColorRed", REAL_ONE), GET_CONFIG_VALUE("directionalLightNearHorizonColorGreen", 0.2f), GET_CONFIG_VALUE("directionalLightNearHorizonColorBlue", REAL_ZERO)));
	directionalLight->SetSunlightNighttimeColor(Color(GET_CONFIG_VALUE("directionalLightNighttimeColorRed", REAL_ZERO), GET_CONFIG_VALUE("directionalLightNighttimeColorGreen", REAL_ZERO), GET_CONFIG_VALUE("directionalLightNighttimeColorBlue", REAL_ZERO)));

	m_gameNode->AddComponent(directionalLight);

	CoreEngine::GetCoreEngine()->GetRenderer()->AddLight(directionalLight);
}

#ifdef BUILD_MESH_RENDERER
void Rendering::DirectionalLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around point light node position to let the user see the source
	//Material directionalLightMaterial("directionalLight_material", Texture("DirectionalLight.png"), 1, 8);
	//Material directionalLightLineMaterial("directionalLightLine_material", Texture("DirectionalLight.png"), 1, 8);

	m_gameNode->GetTransform().SetScale(0.4f); /* TODO: Don't use hardcoded values! Ever! */
	m_gameNode->AddComponent(new MeshRenderer(
		new Mesh("DirectionalLight.obj"),
		new Material(new Texture("DirectionalLight.png"), 1.0f, 8.0f)));
		
	Math::Vector3D forwardVec = m_gameNode->GetTransform().GetTransformedRot().GetForward().Normalized();
	Math::Vector3D rayEndPosition = forwardVec * 2.0f;
	//DELOCUST_LOG("light position = %s;\t light rotation = %s;\t light forward vector = %s;\t light end pos = %s", position.ToString().c_str(),
	//	directionalLightNode->GetTransform().GetTransformedRot().ToString().c_str(), forwardVec.ToString().c_str(), (position + rayEndPosition).ToString().c_str());
	Vertex vertices [] = { Vertex(Math::Vector3D()), Vertex(rayEndPosition) };
	int indices [] = { 0, 1 };
	m_gameNode->AddComponent(new MeshRenderer(
		new Mesh(vertices, 2, indices, 2, false, GL_LINES),
		new Material(new Texture("DirectionalLight.png"), 1.0f, 8.0f)));
}
#endif
/* ==================== DirectionalLightBuilder implementation end ==================== */

/* ==================== PointLightBuilder implementation begin ==================== */
Rendering::PointLightBuilder::PointLightBuilder() :
	LightBuilder(),
	M_DEFAULT_POINT_LIGHT_POS(GET_CONFIG_VALUE("defaultPointLightPosX", REAL_ZERO), GET_CONFIG_VALUE("defaultPointLightPosY", REAL_ZERO), GET_CONFIG_VALUE("defaultPointLightPosZ", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X(GET_CONFIG_VALUE("defaultPointLightAngleX", -45.0f)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y(GET_CONFIG_VALUE("defaultPointLightAngleY", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z(GET_CONFIG_VALUE("defaultPointLightAngleZ", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_COLOR(GET_CONFIG_VALUE("defaultPointLightColorRed", REAL_ZERO), GET_CONFIG_VALUE("defaultPointLightColorGreen", REAL_ZERO), GET_CONFIG_VALUE("defaultPointLightColorBlue", REAL_ONE)),
	M_DEFAULT_POINT_LIGHT_INTENSITY(GET_CONFIG_VALUE("defaultPointLightIntensity", 10.0f)),
	M_DEFAULT_POINT_LIGHT_ATTENUATION(GET_CONFIG_VALUE("defaultPointLightAttenuationConstant", REAL_ZERO), GET_CONFIG_VALUE("defaultPointLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE("defaultPointLightAttenuationExponent", REAL_ZERO))
{
}

void Rendering::PointLightBuilder::SetupLightTransform()
{
	m_gameNode = new GameNode();

	// Setting position
	Math::Real xPos = GET_CONFIG_VALUE("pointLightPosX_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetX());
	Math::Real yPos = GET_CONFIG_VALUE("pointLightPosY_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetY());
	Math::Real zPos = GET_CONFIG_VALUE("pointLightPosZ_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetZ());
	m_gameNode->GetTransform().SetPos(xPos, yPos, zPos);

	// Setting rotation
	Math::Angle angleX(GET_CONFIG_VALUE("pointLightAngleX_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X.GetAngleInDegrees()));
	Math::Angle angleY(GET_CONFIG_VALUE("pointLightAngleY_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y.GetAngleInDegrees()));
	Math::Angle angleZ(GET_CONFIG_VALUE("pointLightAngleZ_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z.GetAngleInDegrees()));
	Math::Matrix4D rotMatrix(angleX, angleY, angleZ);
	Math::Quaternion rot(rotMatrix);
	m_gameNode->GetTransform().SetRot(rot);
}

void Rendering::PointLightBuilder::SetupLightParams()
{
	// Setting color
	Math::Real red = GET_CONFIG_VALUE("pointLightColorRed_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetRed());
	Math::Real green = GET_CONFIG_VALUE("pointLightColorGreen_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetGreen());
	Math::Real blue = GET_CONFIG_VALUE("pointLightColorBlue_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetBlue());
	Color color(red, green, blue);

	// Setting intensity
	Math::Real intensity = GET_CONFIG_VALUE("pointLightIntensity_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_INTENSITY);

	// Setting attenuation
	Math::Real constant = GET_CONFIG_VALUE("pointLightAttenuationConstant_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetConstant());
	Math::Real linear = GET_CONFIG_VALUE("pointLightAttenuationLinear_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetLinear());
	Math::Real exponent = GET_CONFIG_VALUE("pointLightAttenuationExponent_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetExponent());
	Attenuation attenuation(constant, linear, exponent);

	Lighting::PointLight* pointLight = new Lighting::PointLight(color, intensity, attenuation);

	// Setting shaders
	pointLight->SetShader(new Shader(GET_CONFIG_VALUE_STR("pointLightShader", "forward-point")));
	pointLight->SetTerrainShader(new Shader(GET_CONFIG_VALUE_STR("pointLightShaderTerrain", "forward-point-terrain")));
	pointLight->SetNoShadowShader(new Shader(GET_CONFIG_VALUE_STR("pointLightNoShadowShader", "forward-point-no-shadows")));
	pointLight->SetNoShadowTerrainShader(new Shader(GET_CONFIG_VALUE_STR("pointLightNoShadowTerrainShader", "forward-point-terrain-no-shadows")));

	// Setting shadow info
	// Setting additional point light information

	m_gameNode->AddComponent(pointLight);

	CoreEngine::GetCoreEngine()->GetRenderer()->AddLight(pointLight);
}

#ifdef BUILD_MESH_RENDERER
void Rendering::PointLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around point light node position to let the user see the source
	m_gameNode->AddComponent(new MeshRenderer(
		/* new Mesh("Bulb\\Bulb.obj") */ new Mesh("PointLight.obj"),
		new Material(new Texture("PointLight.png"), 1.0f, 8.0f)));
	m_gameNode->GetTransform().SetScale(0.005f); /* TODO: Don't use hard-coded values! Ever! */
}
#endif
/* ==================== PointLightBuilder implementation end ==================== */

/* ==================== SpotLightBuilder implementation begin ==================== */
Rendering::SpotLightBuilder::SpotLightBuilder() :
	LightBuilder(),
	M_DEFAULT_SPOT_LIGHT_POS(GET_CONFIG_VALUE("defaultSpotLightPosX", REAL_ZERO), GET_CONFIG_VALUE("defaultSpotLightPosY", REAL_ZERO), GET_CONFIG_VALUE("defaultSpotLightPosZ", REAL_ZERO)),
	M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_X(GET_CONFIG_VALUE("defaultSpotLightAngleX", -45.0f)),
	M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Y(GET_CONFIG_VALUE("defaultSpotLightAngleY", REAL_ZERO)),
	M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Z(GET_CONFIG_VALUE("defaultSpotLightAngleZ", REAL_ZERO)),
	M_DEFAULT_SPOT_LIGHT_COLOR(GET_CONFIG_VALUE("defaultSpotLightColorRed", REAL_ZERO), GET_CONFIG_VALUE("defaultSpotLightColorGreen", REAL_ZERO), GET_CONFIG_VALUE("defaultSpotLightColorBlue", REAL_ONE)),
	M_DEFAULT_SPOT_LIGHT_INTENSITY(GET_CONFIG_VALUE("defaultSpotLightIntensity", 4.0f)),
	M_DEFAULT_SPOT_LIGHT_ATTENUATION(GET_CONFIG_VALUE("defaultSpotLightAttenuationConstant", 0.5f), GET_CONFIG_VALUE("defaultSpotLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE("defaultSpotLightAttenuationExponent", 0.05f)),
	M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE(GET_CONFIG_VALUE("defaultSpotLightViewAngle", 120.0f), Math::Unit::DEGREE),
	M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2(GET_CONFIG_VALUE("defaultSpotLightShadowMapSizeAsPowerOf2", 10)), // 2 ^ 10 = 1024
	M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS(GET_CONFIG_VALUE("defaultSpotLightShadowSoftness", REAL_ONE)),
	M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT(GET_CONFIG_VALUE("defaultSpotLightLightBleedingReductionAmount", 0.2f)),
	M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE(GET_CONFIG_VALUE("defaultSpotLightMinVariance", 0.00002f))
{
}

void Rendering::SpotLightBuilder::SetupLightTransform()
{
	m_gameNode = new GameNode();

	// Setting position
	Math::Real xPos = GET_CONFIG_VALUE("spotLightPosX_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetX());
	Math::Real yPos = GET_CONFIG_VALUE("spotLightPosY_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetY());
	Math::Real zPos = GET_CONFIG_VALUE("spotLightPosZ_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetZ());
	m_gameNode->GetTransform().SetPos(xPos, yPos, zPos);

	// Setting rotation
	Math::Angle angleX(GET_CONFIG_VALUE("spotLightAngleX_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_X.GetAngleInDegrees()));
	Math::Angle angleY(GET_CONFIG_VALUE("spotLightAngleY_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Y.GetAngleInDegrees()));
	Math::Angle angleZ(GET_CONFIG_VALUE("spotLightAngleZ_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Z.GetAngleInDegrees()));
	Math::Matrix4D rotMatrix(angleX, angleY, angleZ);
	Math::Quaternion rot(rotMatrix);
	m_gameNode->GetTransform().SetRot(rot);
}

void Rendering::SpotLightBuilder::SetupLightParams()
{
	// Setting color
	Math::Real red = GET_CONFIG_VALUE("spotLightColorRed_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetRed());
	Math::Real green = GET_CONFIG_VALUE("spotLightColorGreen_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetGreen());
	Math::Real blue = GET_CONFIG_VALUE("spotLightColorBlue_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetBlue());
	Color color(red, green, blue);

	// Setting intensity
	Math::Real intensity = GET_CONFIG_VALUE("spotLightIntensity_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_INTENSITY);

	// Setting attenuation
	Math::Real constant = GET_CONFIG_VALUE("spotLightAttenuationConstant_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetConstant());
	Math::Real linear = GET_CONFIG_VALUE("spotLightAttenuationLinear_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetLinear());
	Math::Real exponent = GET_CONFIG_VALUE("spotLightAttenuationExponent_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetExponent());
	Attenuation attenuation(constant, linear, exponent);

	// Setting view angle
	Math::Angle viewAngle(GET_CONFIG_VALUE("spotLightViewAngle_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE.GetAngleInRadians()), Math::Unit::RADIAN);

	// Setting shadow info
	int shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE("spotLightShadowMapSizeAsPowerOf2_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2); // 2 ^ 10 = 1024
	Math::Real shadowSoftness = GET_CONFIG_VALUE("spotLightShadowSoftness_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS);
	Math::Real lightBleedingReductionAmount = GET_CONFIG_VALUE("spotLightLightBleedingReductionAmount_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT);
	Math::Real minVariance = GET_CONFIG_VALUE("spotLightMinVariance_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE);

	Lighting::SpotLight* spotLight = new Lighting::SpotLight(color, intensity, attenuation, viewAngle, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance);

	// Setting shaders
	spotLight->SetShader(new Shader(GET_CONFIG_VALUE_STR("spotLightShader", "forward-spot")));
	spotLight->SetTerrainShader(new Shader(GET_CONFIG_VALUE_STR("spotLightShader", "forward-spot-terrain")));
	spotLight->SetNoShadowShader(new Shader(GET_CONFIG_VALUE_STR("spotLightNoShadowShader", "forward-spot-no-shadows")));
	spotLight->SetNoShadowTerrainShader(new Shader(GET_CONFIG_VALUE_STR("spotLightNoShadowTerrainShader", "forward-spot-terrain-no-shadows")));

	m_gameNode->AddComponent(spotLight);

	CoreEngine::GetCoreEngine()->GetRenderer()->AddLight(spotLight);
}

#ifdef BUILD_MESH_RENDERER
void Rendering::SpotLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around spot light node position to let the user see the source
	m_gameNode->AddComponent(new MeshRenderer(
		new Mesh("SpotLight.obj"),
		new Material(new Texture("SpotLight.png"), 1.0f, 8.0f)));
	m_gameNode->GetTransform().SetScale(0.1f); /* TODO: Don't use hard-coded values! Ever! */
}
#endif
/* ==================== SpotLightBuilder implementation end ==================== */

/* ==================== CameraBuilder implementation begin ==================== */
Rendering::CameraBuilder::CameraBuilder() :
	Builder(),
	M_DEFAULT_CAMERA_POS(GET_CONFIG_VALUE("defaultCameraPosX", 0.0f), GET_CONFIG_VALUE("defaultCameraPosY", 0.0f), GET_CONFIG_VALUE("defaultCameraPosZ", 0.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_X(GET_CONFIG_VALUE("defaultCameraAngleX", -45.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_Y(GET_CONFIG_VALUE("defaultCameraAngleY", 0.0f)),
	M_DEFAULT_CAMERA_ROTATION_ANGLE_Z(GET_CONFIG_VALUE("defaultCameraAngleZ", 0.0f)),
	M_DEFAULT_CAMERA_FIELD_OF_VIEW(GET_CONFIG_VALUE("defaultCameraFoV", 70.0f)),
	M_DEFAULT_CAMERA_ASPECT_RATIO(GET_CONFIG_VALUE("defaultCameraAspectRatio", static_cast<Math::Real>(800) / 600)),
	M_DEFAULT_CAMERA_NEAR_PLANE(GET_CONFIG_VALUE("defaultCameraNearPlane", 0.1f)),
	M_DEFAULT_CAMERA_FAR_PLANE(GET_CONFIG_VALUE("defaultCameraFarPlane", 1000.0f)),
	M_DEFAULT_CAMERA_SENSITIVITY(GET_CONFIG_VALUE("defaultCameraSensitivity", 0.005f)),
	m_cameraIndex(0),
	m_cameraIndexStr("0")
{
}

Rendering::CameraBuilder::~CameraBuilder()
{
}

void Rendering::CameraBuilder::SetCameraIndex(int cameraIndex)
{
	m_cameraIndex = cameraIndex;
	std::stringstream ss("");
	ss << (m_cameraIndex + 1);
	m_cameraIndexStr = ss.str();
}

void Rendering::CameraBuilder::SetEntityToFollow(Rendering::GameNode* gameNodeToFollow)
{
	m_gameNodeToFollow = gameNodeToFollow;
}

void Rendering::CameraBuilder::BuildPart1()
{
	SetupCameraTransform();
}

void Rendering::CameraBuilder::BuildPart2()
{
	SetupCameraParams();
}

void Rendering::CameraBuilder::SetupCameraTransform()
{
	m_gameNode = new GameNode();

	// Setting position
	Math::Real xPos = GET_CONFIG_VALUE("cameraPosX_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetX());
	Math::Real yPos = GET_CONFIG_VALUE("cameraPosY_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetY());
	Math::Real zPos = GET_CONFIG_VALUE("cameraPosZ_" + m_cameraIndexStr, M_DEFAULT_CAMERA_POS.GetZ());
	m_gameNode->GetTransform().SetPos(xPos, yPos, zPos);

	// Setting rotation
	Math::Angle angleX(GET_CONFIG_VALUE("cameraAngleX_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_X.GetAngleInDegrees()));
	Math::Angle angleY(GET_CONFIG_VALUE("cameraAngleY_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Y.GetAngleInDegrees()));
	Math::Angle angleZ(GET_CONFIG_VALUE("cameraAngleZ_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ROTATION_ANGLE_Z.GetAngleInDegrees()));
	Math::Matrix4D rotMatrix(angleX, angleY, angleZ);
	DELOCUST_LOG("angleX=%.1f, angleY=%.1f, angleZ=%.1f", angleX.GetAngleInDegrees(), angleY.GetAngleInDegrees(), angleZ.GetAngleInDegrees());
	Math::Quaternion rot(rotMatrix);
	m_gameNode->GetTransform().SetRot(rot);
}

void Rendering::CameraBuilder::SetupCameraParams()
{
	// Setting camera parameters
	Math::Angle fov(GET_CONFIG_VALUE("cameraFoV_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FIELD_OF_VIEW.GetAngleInDegrees()));
	Math::Real aspectRatio = GET_CONFIG_VALUE("cameraAspectRatio_" + m_cameraIndexStr, M_DEFAULT_CAMERA_ASPECT_RATIO);
	Math::Real zNearPlane = GET_CONFIG_VALUE("cameraNearPlane_" + m_cameraIndexStr, M_DEFAULT_CAMERA_NEAR_PLANE);
	Math::Real zFarPlane = GET_CONFIG_VALUE("cameraFarPlane_" + m_cameraIndexStr, M_DEFAULT_CAMERA_FAR_PLANE);
	Math::Real sensitivity = GET_CONFIG_VALUE("cameraSensitivity_" + m_cameraIndexStr, M_DEFAULT_CAMERA_SENSITIVITY);

	Rendering::CameraComponent* camera = new Rendering::CameraFollowComponent(fov, aspectRatio, zNearPlane, zFarPlane, sensitivity, m_gameNodeToFollow);
	m_gameNode->AddComponent(camera);
	CoreEngine::GetCoreEngine()->GetRenderer()->AddCamera(camera);
}

#ifdef BUILD_MESH_RENDERER
void Rendering::CameraBuilder::BuildMeshRenderer()
{
	// Rendering a small box around camera node position to let the user see the camera
	//m_gameNode->AddComponent(new MeshRenderer(
	//	new Mesh("Camera.obj"),
	//	new Material(new Texture("Camera.png"), 1.0f, 8.0f)));
	//m_gameNode->GetTransform().SetScale(0.1f); /* TODO: Don't use hard-coded values! Ever! */
}
#endif
/* ==================== CameraBuilder implementation end ==================== */