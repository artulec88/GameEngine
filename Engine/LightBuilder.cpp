#include "StdAfx.h"
#include "LightBuilder.h"
#include "LightBuilder_impl.h"

#include "Utility\IConfig.h"

/* ==================== DirectionalLightBuilder implementation begin ==================== */
void Engine::DirectionalLightBuilder::SetupLightShaders()
{
	m_object = new Rendering::Lighting::DirectionalLight(m_shaderFactory.GetShader(ShaderTypes::DIRECTIONAL_LIGHT),
		m_shaderFactory.GetShader(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN), m_shaderFactory.GetShader(ShaderTypes::DIRECTIONAL_LIGHT_NO_SHADOWS),
		m_shaderFactory.GetShader(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS));
	// TODO: Add new shaders: "fogShader" and "fogTerrainShader".
}

void Engine::DirectionalLightBuilder::SetupLightParams()
{
	// Determining position
	const Math::Vector3D defaultDirectionalLightPos(GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightPosX", 0.0f), GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightPosY", 0.0f), GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightPosZ", 0.0f));
	Math::Real xPos = GET_CONFIG_VALUE_ENGINE("directionalLightPosX", defaultDirectionalLightPos.GetX());
	Math::Real yPos = GET_CONFIG_VALUE_ENGINE("directionalLightPosY", defaultDirectionalLightPos.GetY());
	Math::Real zPos = GET_CONFIG_VALUE_ENGINE("directionalLightPosZ", defaultDirectionalLightPos.GetZ());

	// Determining rotation
	const Math::Angle defaultDirectionalLightRotationX(GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightAngleX", -45.0f), Math::Unit::DEGREE);
	const Math::Angle defaultDirectionalLightRotationY(GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightAngleY", REAL_ZERO), Math::Unit::DEGREE);
	const Math::Angle defaultDirectionalLightRotationZ(GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightAngleZ", REAL_ZERO), Math::Unit::DEGREE);
	Math::Angle angleX(GET_CONFIG_VALUE_ENGINE("directionalLightAngleX", defaultDirectionalLightRotationX.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
	Math::Angle angleY(GET_CONFIG_VALUE_ENGINE("directionalLightAngleY", defaultDirectionalLightRotationY.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
	Math::Angle angleZ(GET_CONFIG_VALUE_ENGINE("directionalLightAngleZ", defaultDirectionalLightRotationZ.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
	DEBUG_LOG_ENGINE("angleX=", angleX.ToString(), ", angleY=", angleY.ToString(), ", angleZ=", angleZ.ToString());
	m_object->SetTransform(Math::Transform(Math::Vector3D(xPos, yPos, zPos), Math::Quaternion(Math::Matrix4D(angleX, angleY, angleZ))));

	// Determining color
	Math::Real red = GET_CONFIG_VALUE_ENGINE("directionalLightColorRed", GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightColorRed", REAL_ONE));
	Math::Real green = GET_CONFIG_VALUE_ENGINE("directionalLightColorGreen", GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightColorGreen", REAL_ONE));
	Math::Real blue = GET_CONFIG_VALUE_ENGINE("directionalLightColorBlue", GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightColorBlue", REAL_ONE));
	// TODO: Alpha?
	m_object->SetColor(Rendering::Color(red, green, blue));

	// Determining intensity
	m_object->SetIntensity(GET_CONFIG_VALUE_ENGINE("directionalLightIntensity", GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightIntensity", REAL_ONE)));

	// Determining shadow info
	m_object->SetShadowInfo(GET_CONFIG_VALUE_ENGINE("directionalLightShadowArea", GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightShadowArea", 80.0f)) / 2.0f,
		GET_CONFIG_VALUE_ENGINE("directionalLightShadowMapSizeAsPowerOf2", GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightShadowMapSizeAsPowerOf2", 10)),
		GET_CONFIG_VALUE_ENGINE("directionalLightShadowSoftness", GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightShadowSoftness", 1.0f)),
		GET_CONFIG_VALUE_ENGINE("directionalLightLightBleedingReductionAmount", GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightLightBleedingReductionAmount", 0.2f)),
		GET_CONFIG_VALUE_ENGINE("directionalLightMinVariance", GET_CONFIG_VALUE_ENGINE("defaultDirectionalLightMinVariance", 0.00002f)));	

	m_object->SetIsEnabled(true);

	// Setting additional directional light information
	//Math::Real maxIntensity = directionalLight->GetIntensity();
	//const Rendering::Color sunlightDaytimeColor(directionalLight->GetColor());
	//const Rendering::Color sunNearHorizonColor(Rendering::Color(GET_CONFIG_VALUE_ENGINE("directionalLightNearHorizonColorRed", REAL_ONE), GET_CONFIG_VALUE_ENGINE("directionalLightNearHorizonColorGreen", 0.2f), GET_CONFIG_VALUE_ENGINE("directionalLightNearHorizonColorBlue", REAL_ZERO)));
	//const Rendering::Color sunlightNighttimeColor(Rendering::Color(GET_CONFIG_VALUE_ENGINE("directionalLightNighttimeColorRed", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("directionalLightNighttimeColorGreen", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("directionalLightNighttimeColorBlue", REAL_ZERO)));

	//m_object->AddComponent(new DirectionalLightComponent(directionalLight, maxIntensity, sunlightDaytimeColor, sunNearHorizonColor, sunlightNighttimeColor,
	//	Math::Angle(GET_CONFIG_VALUE_ENGINE("latitude", 52.0f)), Math::Angle(GET_CONFIG_VALUE_ENGINE("longitude", -16.0f)),
	//	Math::Angle(GET_CONFIG_VALUE_ENGINE("sunlightFirstElevationLevel", -REAL_ONE)), Math::Angle(GET_CONFIG_VALUE_ENGINE("sunlightSecondElevationLevel", REAL_ZERO)),
	//	Math::Angle(GET_CONFIG_VALUE_ENGINE("sunlightThirdElevationLevel", REAL_ONE))));
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
Engine::PointLightBuilder::PointLightBuilder(const ShaderFactory& shaderFactory) :
	LightBuilder(shaderFactory),
	M_DEFAULT_POINT_LIGHT_POS(GET_CONFIG_VALUE_ENGINE("defaultPointLightPosX", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultPointLightPosY", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultPointLightPosZ", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X(GET_CONFIG_VALUE_ENGINE("defaultPointLightAngleX", -45.0f)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y(GET_CONFIG_VALUE_ENGINE("defaultPointLightAngleY", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z(GET_CONFIG_VALUE_ENGINE("defaultPointLightAngleZ", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_COLOR(GET_CONFIG_VALUE_ENGINE("defaultPointLightColorRed", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultPointLightColorGreen", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultPointLightColorBlue", REAL_ONE)),
	M_DEFAULT_POINT_LIGHT_INTENSITY(GET_CONFIG_VALUE_ENGINE("defaultPointLightIntensity", 10.0f)),
	M_DEFAULT_POINT_LIGHT_ATTENUATION(GET_CONFIG_VALUE_ENGINE("defaultPointLightAttenuationConstant", REAL_ZERO), GET_CONFIG_VALUE_ENGINE("defaultPointLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE_ENGINE("defaultPointLightAttenuationExponent", REAL_ZERO))
{
}

void Engine::PointLightBuilder::SetupLightShaders()
{
	m_object = new Rendering::Lighting::PointLight(m_shaderFactory.GetShader(ShaderTypes::POINT_LIGHT),
		m_shaderFactory.GetShader(ShaderTypes::POINT_LIGHT_TERRAIN), m_shaderFactory.GetShader(ShaderTypes::POINT_LIGHT_NO_SHADOWS),
		m_shaderFactory.GetShader(ShaderTypes::POINT_LIGHT_TERRAIN_NO_SHADOWS));
	// TODO: Add new shaders: "fogShader" and "fogTerrainShader".
}

void Engine::PointLightBuilder::SetupLightParams()
{
	// Determining position
	Math::Real xPos = GET_CONFIG_VALUE_ENGINE("pointLightPosX_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetX());
	Math::Real yPos = GET_CONFIG_VALUE_ENGINE("pointLightPosY_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetY());
	Math::Real zPos = GET_CONFIG_VALUE_ENGINE("pointLightPosZ_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetZ());

	// // Determining rotation
	Math::Angle angleX(GET_CONFIG_VALUE_ENGINE("pointLightAngleX_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X.Get(Math::Unit::DEGREE)));
	Math::Angle angleY(GET_CONFIG_VALUE_ENGINE("pointLightAngleY_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y.Get(Math::Unit::DEGREE)));
	Math::Angle angleZ(GET_CONFIG_VALUE_ENGINE("pointLightAngleZ_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z.Get(Math::Unit::DEGREE)));
	m_object->SetTransform(Math::Transform(Math::Vector3D(xPos, yPos, zPos), Math::Quaternion(Math::Matrix4D(angleX, angleY, angleZ))));

	// Determining color
	Math::Real red = GET_CONFIG_VALUE_ENGINE("pointLightColorRed_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetRed());
	Math::Real green = GET_CONFIG_VALUE_ENGINE("pointLightColorGreen_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetGreen());
	Math::Real blue = GET_CONFIG_VALUE_ENGINE("pointLightColorBlue_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetBlue());
	// TODO: Alpha?
	m_object->SetColor(Rendering::Color(red, green, blue));

	// Determining intensity
	m_object->SetIntensity(GET_CONFIG_VALUE_ENGINE("pointLightIntensity_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_INTENSITY));

	// Determining attenuation
	Math::Real constant = GET_CONFIG_VALUE_ENGINE("pointLightAttenuationConstant_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetConstant());
	Math::Real linear = GET_CONFIG_VALUE_ENGINE("pointLightAttenuationLinear_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetLinear());
	Math::Real exponent = GET_CONFIG_VALUE_ENGINE("pointLightAttenuationExponent_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetExponent());
	m_object->SetAttenuation(Rendering::Attenuation(constant, linear, exponent));
	
	m_object->SetIsEnabled(true);

	// TODO: Setting shadow info
	// TODO: Setting additional point light information
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
Engine::SpotLightBuilder::SpotLightBuilder(const ShaderFactory& shaderFactory) :
	LightBuilder(shaderFactory),
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

void Engine::SpotLightBuilder::SetupLightShaders()
{
	m_object = new Rendering::Lighting::SpotLight(m_shaderFactory.GetShader(ShaderTypes::SPOT_LIGHT),
		m_shaderFactory.GetShader(ShaderTypes::SPOT_LIGHT_TERRAIN), m_shaderFactory.GetShader(ShaderTypes::SPOT_LIGHT_NO_SHADOWS),
		m_shaderFactory.GetShader(ShaderTypes::SPOT_LIGHT_TERRAIN_NO_SHADOWS));
	// TODO: Add new shaders: "fogShader" and "fogTerrainShader".
}

void Engine::SpotLightBuilder::SetupLightParams()
{
	// Determining position
	Math::Real xPos = GET_CONFIG_VALUE_ENGINE("spotLightPosX_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetX());
	Math::Real yPos = GET_CONFIG_VALUE_ENGINE("spotLightPosY_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetY());
	Math::Real zPos = GET_CONFIG_VALUE_ENGINE("spotLightPosZ_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetZ());

	// Determining rotation
	Math::Angle angleX(GET_CONFIG_VALUE_ENGINE("spotLightAngleX_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_X.Get(Math::Unit::DEGREE)));
	Math::Angle angleY(GET_CONFIG_VALUE_ENGINE("spotLightAngleY_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Y.Get(Math::Unit::DEGREE)));
	Math::Angle angleZ(GET_CONFIG_VALUE_ENGINE("spotLightAngleZ_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Z.Get(Math::Unit::DEGREE)));
	m_object->SetTransform(Math::Transform(Math::Vector3D(xPos, yPos, zPos), Math::Quaternion(Math::Matrix4D(angleX, angleY, angleZ))));

	// Determining color
	Math::Real red = GET_CONFIG_VALUE_ENGINE("spotLightColorRed_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetRed());
	Math::Real green = GET_CONFIG_VALUE_ENGINE("spotLightColorGreen_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetGreen());
	Math::Real blue = GET_CONFIG_VALUE_ENGINE("spotLightColorBlue_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetBlue());
	// TODO: Alpha?
	m_object->SetColor(Rendering::Color(red, green, blue));

	// Setting intensity
	m_object->SetIntensity(GET_CONFIG_VALUE_ENGINE("spotLightIntensity_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_INTENSITY));

	// Setting attenuation
	Math::Real constant = GET_CONFIG_VALUE_ENGINE("spotLightAttenuationConstant_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetConstant());
	Math::Real linear = GET_CONFIG_VALUE_ENGINE("spotLightAttenuationLinear_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetLinear());
	Math::Real exponent = GET_CONFIG_VALUE_ENGINE("spotLightAttenuationExponent_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetExponent());
	m_object->SetAttenuation(Rendering::Attenuation(constant, linear, exponent));

	// Setting shadow info
	m_object->SetShadowInfo(Math::Angle(GET_CONFIG_VALUE_ENGINE("spotLightViewAngle_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE),
		GET_CONFIG_VALUE_ENGINE("spotLightShadowMapSizeAsPowerOf2_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2),
		GET_CONFIG_VALUE_RENDERING("spotLightProjectionNearPlane", 0.1f), GET_CONFIG_VALUE_ENGINE("spotLightShadowSoftness_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS),
		GET_CONFIG_VALUE_ENGINE("spotLightLightBleedingReductionAmount_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT),
		GET_CONFIG_VALUE_ENGINE("spotLightMinVariance_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE));

	m_object->SetIsEnabled(true);

	// TODO: Setting additional spot light information
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