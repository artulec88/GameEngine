#include "LightBuilder.h"

#include "Utility\IConfig.h"

/* ==================== DirectionalLightBuilder implementation begin ==================== */
Game::DirectionalLightBuilder::DirectionalLightBuilder(Engine::GameManager* gameManager, const Rendering::ShaderFactory& shaderFactory,
	Rendering::Lighting::DirectionalLight* directionalLight) :
	LightBuilder(gameManager, shaderFactory, directionalLight),
	m_halfShadowArea(40.0f),
	m_shadowMapSizeAsPowerOf2(10),
	m_shadowSoftness(REAL_ONE),
	m_lightBleedingReductionFactor(0.2f),
	m_minVariance(0.00002f)
{
}

Game::DirectionalLightBuilder::~DirectionalLightBuilder()
{
}

void Game::DirectionalLightBuilder::BuildPart1()
{
	// Determining position
	m_pos.Set(GET_CONFIG_VALUE_GAME("directionalLightPosX", GET_CONFIG_VALUE_GAME("defaultDirectionalLightPosX", 0.0f)),
		GET_CONFIG_VALUE_GAME("directionalLightPosY", GET_CONFIG_VALUE_GAME("defaultDirectionalLightPosY", 0.0f)),
		GET_CONFIG_VALUE_GAME("directionalLightPosZ", GET_CONFIG_VALUE_GAME("defaultDirectionalLightPosZ", 0.0f)));

	// Determining rotation
	const Math::Angle defaultDirectionalLightRotationX(GET_CONFIG_VALUE_GAME("defaultDirectionalLightAngleX", -45.0f), Math::Unit::DEGREE);
	const Math::Angle defaultDirectionalLightRotationY(GET_CONFIG_VALUE_GAME("defaultDirectionalLightAngleY", REAL_ZERO), Math::Unit::DEGREE);
	const Math::Angle defaultDirectionalLightRotationZ(GET_CONFIG_VALUE_GAME("defaultDirectionalLightAngleZ", REAL_ZERO), Math::Unit::DEGREE);
	Math::Angle angleX(GET_CONFIG_VALUE_GAME("directionalLightAngleX", defaultDirectionalLightRotationX.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
	Math::Angle angleY(GET_CONFIG_VALUE_GAME("directionalLightAngleY", defaultDirectionalLightRotationY.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
	Math::Angle angleZ(GET_CONFIG_VALUE_GAME("directionalLightAngleZ", defaultDirectionalLightRotationZ.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
	DEBUG_LOG_GAME("angleX=", angleX, ", angleY=", angleY, ", angleZ=", angleZ);
	m_rot = Math::Quaternion(Math::Matrix4D(angleX, angleY, angleZ));
}

void Game::DirectionalLightBuilder::BuildPart2()
{
	// Determining color
	m_color.Set(GET_CONFIG_VALUE_GAME("directionalLightColorRed", GET_CONFIG_VALUE_GAME("defaultDirectionalLightColorRed", REAL_ONE)),
		GET_CONFIG_VALUE_GAME("directionalLightColorGreen", GET_CONFIG_VALUE_GAME("defaultDirectionalLightColorGreen", REAL_ONE)),
		GET_CONFIG_VALUE_GAME("directionalLightColorBlue", GET_CONFIG_VALUE_GAME("defaultDirectionalLightColorBlue", REAL_ONE)),
		REAL_ONE /* TODO: Alpha? */);

	// Determining intensity
	m_intensity = GET_CONFIG_VALUE_GAME("directionalLightIntensity", GET_CONFIG_VALUE_GAME("defaultDirectionalLightIntensity", REAL_ONE));

	// Determining shadow info
	m_halfShadowArea = GET_CONFIG_VALUE_GAME("directionalLightShadowArea", GET_CONFIG_VALUE_GAME("defaultDirectionalLightShadowArea", 80.0f)) / 2.0f;
	m_shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE_GAME("directionalLightShadowMapSizeAsPowerOf2", GET_CONFIG_VALUE_GAME("defaultDirectionalLightShadowMapSizeAsPowerOf2", 10));
	m_shadowSoftness = GET_CONFIG_VALUE_GAME("directionalLightShadowSoftness", GET_CONFIG_VALUE_GAME("defaultDirectionalLightShadowSoftness", 1.0f));
	m_lightBleedingReductionFactor = GET_CONFIG_VALUE_GAME("directionalLightLightBleedingReductionAmount", GET_CONFIG_VALUE_GAME("defaultDirectionalLightLightBleedingReductionAmount", 0.2f));
	m_minVariance = GET_CONFIG_VALUE_GAME("directionalLightMinVariance", GET_CONFIG_VALUE_GAME("defaultDirectionalLightMinVariance", 0.00002f));

	// Setting additional directional light information
	//Math::Real maxIntensity = directionalLight->GetIntensity();
	//const Rendering::Color sunlightDaytimeColor(directionalLight->GetColor());
	//const Rendering::Color sunNearHorizonColor(Rendering::Color(GET_CONFIG_VALUE_GAME("directionalLightNearHorizonColorRed", REAL_ONE), GET_CONFIG_VALUE_GAME("directionalLightNearHorizonColorGreen", 0.2f), GET_CONFIG_VALUE_GAME("directionalLightNearHorizonColorBlue", REAL_ZERO)));
	//const Rendering::Color sunlightNighttimeColor(Rendering::Color(GET_CONFIG_VALUE_GAME("directionalLightNighttimeColorRed", REAL_ZERO), GET_CONFIG_VALUE_GAME("directionalLightNighttimeColorGreen", REAL_ZERO), GET_CONFIG_VALUE_GAME("directionalLightNighttimeColorBlue", REAL_ZERO)));

	//m_object->AddComponent(new DirectionalLightComponent(directionalLight, maxIntensity, sunlightDaytimeColor, sunNearHorizonColor, sunlightNighttimeColor,
	//	Math::Angle(GET_CONFIG_VALUE_GAME("latitude", 52.0f)), Math::Angle(GET_CONFIG_VALUE_GAME("longitude", -16.0f)),
	//	Math::Angle(GET_CONFIG_VALUE_GAME("sunlightFirstElevationLevel", -REAL_ONE)), Math::Angle(GET_CONFIG_VALUE_GAME("sunlightSecondElevationLevel", REAL_ZERO)),
	//	Math::Angle(GET_CONFIG_VALUE_GAME("sunlightThirdElevationLevel", REAL_ONE))));
}

void Game::DirectionalLightBuilder::BuildPart3()
{
	m_object->SetShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::DIRECTIONAL_LIGHT));
	m_object->SetTerrainShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::DIRECTIONAL_LIGHT_TERRAIN));
	m_object->SetNoShadowShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::DIRECTIONAL_LIGHT_NO_SHADOWS));
	m_object->SetNoShadowTerrainShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS));
	// TODO: Add new shaders: "fogShader" and "fogTerrainShader".

	m_object->SetTransform(Math::Transform(m_pos, m_rot, REAL_ONE));

	m_object->SetColor(m_color);
	m_object->SetIntensity(m_intensity);
	m_object->SetShadowInfo(m_halfShadowArea, m_shadowMapSizeAsPowerOf2, m_shadowSoftness, m_lightBleedingReductionFactor, m_minVariance);

	m_object->SetIsEnabled(true);
	m_object->SetIsShadowingEnabled(true);
}

#ifdef BUILD_MESH_RENDERER
void Game::DirectionalLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around point light node position to let the user see the source
	//Material directionalLightMaterial("directionalLight_material", Texture("DirectionalLight.png"), 1, 8);
	//Material directionalLightLineMaterial("directionalLightLine_material", Texture("DirectionalLight.png"), 1, 8);

	m_gameNode->GetTransform().SetScale(0.4f); /* TODO: Don't use hardcoded values! Ever! */
	m_gameNode->AddComponent(new MeshRendererComponent(m_gameManager->GetMesh("DirectionalLight.obj"), new Rendering::Material(m_textureFactory.GetTexture(TextureIDs::DIRECTIONAL_LIGHT), 1.0f, 8.0f)));

	Math::Vector3D forwardVec = m_gameNode->GetTransform().GetTransformedRot().GetForward().Normalized();
	Math::Vector3D rayEndPosition = forwardVec * 2.0f;
	//DELOCUST_LOG_ENGINE("light position = ", m_gameNode->GetTransform().GetTransformedPos().ToString(), ";\t light rotation = ",
	//	m_gameNode->GetTransform().GetTransformedRot().ToString(), ";\t light forward vector = ", forwardVec.ToString(), ";\t light end pos = ",
	//	(m_gameNode->GetTransform().GetTransformedPos() + rayEndPosition).ToString());

	//Vertex vertices [] = { Vertex(Math::Vector3D()), Vertex(rayEndPosition) };
	//int indices [] = { 0, 1 };
	//m_gameNode->AddComponent(new MeshRenderer(
	//	new Mesh(vertices, 2, indices, 2, false, GL_LINES),
	//	new Material(m_gameManager->GetTextureFactory().GetTexture(TextureIDs::DIRECTIONAL_LIGHT), 1.0f, 8.0f)));
}
#endif
/* ==================== DirectionalLightBuilder implementation end ==================== */

/* ==================== PointLightBuilder implementation begin ==================== */
Game::PointLightBuilder::PointLightBuilder(Engine::GameManager* gameManager, const Rendering::ShaderFactory& shaderFactory,
	Rendering::Lighting::PointLight* pointLight) :
	LightBuilder(gameManager, shaderFactory, pointLight),
	M_DEFAULT_POINT_LIGHT_POS(GET_CONFIG_VALUE_GAME("defaultPointLightPosX", REAL_ZERO), GET_CONFIG_VALUE_GAME("defaultPointLightPosY", REAL_ZERO), GET_CONFIG_VALUE_GAME("defaultPointLightPosZ", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X(GET_CONFIG_VALUE_GAME("defaultPointLightAngleX", -45.0f)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y(GET_CONFIG_VALUE_GAME("defaultPointLightAngleY", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z(GET_CONFIG_VALUE_GAME("defaultPointLightAngleZ", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_COLOR(GET_CONFIG_VALUE_GAME("defaultPointLightColorRed", REAL_ZERO), GET_CONFIG_VALUE_GAME("defaultPointLightColorGreen", REAL_ZERO), GET_CONFIG_VALUE_GAME("defaultPointLightColorBlue", REAL_ONE)),
	M_DEFAULT_POINT_LIGHT_INTENSITY(GET_CONFIG_VALUE_GAME("defaultPointLightIntensity", 10.0f)),
	M_DEFAULT_POINT_LIGHT_ATTENUATION(GET_CONFIG_VALUE_GAME("defaultPointLightAttenuationConstant", REAL_ZERO), GET_CONFIG_VALUE_GAME("defaultPointLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE_GAME("defaultPointLightAttenuationExponent", REAL_ZERO)),
	m_attenuation(M_DEFAULT_POINT_LIGHT_ATTENUATION)
{
}

void Game::PointLightBuilder::BuildPart1()
{
	// Determining position
	m_pos.Set(GET_CONFIG_VALUE_GAME("pointLightPosX_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetX()),
		GET_CONFIG_VALUE_GAME("pointLightPosY_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetY()),
		GET_CONFIG_VALUE_GAME("pointLightPosZ_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetZ()));

	// Determining rotation
	Math::Angle angleX(GET_CONFIG_VALUE_GAME("pointLightAngleX_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X.Get(Math::Unit::DEGREE)));
	Math::Angle angleY(GET_CONFIG_VALUE_GAME("pointLightAngleY_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y.Get(Math::Unit::DEGREE)));
	Math::Angle angleZ(GET_CONFIG_VALUE_GAME("pointLightAngleZ_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z.Get(Math::Unit::DEGREE)));
	DEBUG_LOG_GAME("angleX=", angleX, ", angleY=", angleY, ", angleZ=", angleZ);
	m_rot = Math::Quaternion(Math::Matrix4D(angleX, angleY, angleZ));
}

void Game::PointLightBuilder::BuildPart2()
{
	// Determining color
	m_color.Set(GET_CONFIG_VALUE_GAME("pointLightColorRed_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetRed()),
		GET_CONFIG_VALUE_GAME("pointLightColorGreen_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetGreen()),
		GET_CONFIG_VALUE_GAME("pointLightColorBlue_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetBlue()),
		REAL_ONE /* TODO: Alpha? */);

	// Determining intensity
	m_intensity = GET_CONFIG_VALUE_GAME("pointLightIntensity_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_INTENSITY);

	// Determining attenuation
	Math::Real constant = GET_CONFIG_VALUE_GAME("pointLightAttenuationConstant_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetConstant());
	Math::Real linear = GET_CONFIG_VALUE_GAME("pointLightAttenuationLinear_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetLinear());
	Math::Real exponent = GET_CONFIG_VALUE_GAME("pointLightAttenuationExponent_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetExponent());
	m_attenuation = std::move(Rendering::Attenuation(constant, linear, exponent));

	// TODO: Setting shadow info
	// TODO: Setting additional point light information
}

#ifdef BUILD_MESH_RENDERER
void Game::PointLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around point light node position to let the user see the source
	m_gameNode->AddComponent(new MeshRendererComponent(/* new Mesh("Bulb\\Bulb.obj") */ m_gameManager->GetMesh("PointLight.obj"), new Rendering::Material(m_textureFactory.GetTexture(TextureIDs::POINT_LIGHT), 1.0f, 8.0f)));
	m_gameNode->GetTransform().SetScale(0.005f); /* TODO: Don't use hard-coded values! Ever! */
}
#endif

void Game::PointLightBuilder::BuildPart3()
{
	m_object->SetShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::POINT_LIGHT));
	m_object->SetTerrainShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::POINT_LIGHT_TERRAIN));
	m_object->SetNoShadowShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::POINT_LIGHT_NO_SHADOWS));
	m_object->SetNoShadowTerrainShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::POINT_LIGHT_TERRAIN_NO_SHADOWS));
	// TODO: Add new shaders: "fogShader" and "fogTerrainShader".

	m_object->SetTransform(Math::Transform(m_pos, m_rot, REAL_ONE));

	m_object->SetColor(m_color);
	m_object->SetIntensity(m_intensity);
	m_object->SetAttenuation(m_attenuation);
	//m_object->SetShadowInfo(m_halfShadowArea, m_shadowMapSizeAsPowerOf2, m_shadowSoftness, m_lightBleedingReductionFactor, m_minVariance);

	m_object->SetIsEnabled(true);
	//m_object->SetIsShadowingEnabled(true);
}
/* ==================== PointLightBuilder implementation end ==================== */

/* ==================== SpotLightBuilder implementation begin ==================== */
Game::SpotLightBuilder::SpotLightBuilder(Engine::GameManager* gameManager, const Rendering::ShaderFactory& shaderFactory,
	Rendering::Lighting::SpotLight* spotLight) :
	LightBuilder(gameManager, shaderFactory, spotLight),
	M_DEFAULT_SPOT_LIGHT_POS(GET_CONFIG_VALUE_GAME("defaultSpotLightPosX", REAL_ZERO), GET_CONFIG_VALUE_GAME("defaultSpotLightPosY", REAL_ZERO), GET_CONFIG_VALUE_GAME("defaultSpotLightPosZ", REAL_ZERO)),
	M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_X(GET_CONFIG_VALUE_GAME("defaultSpotLightAngleX", -45.0f)),
	M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Y(GET_CONFIG_VALUE_GAME("defaultSpotLightAngleY", REAL_ZERO)),
	M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Z(GET_CONFIG_VALUE_GAME("defaultSpotLightAngleZ", REAL_ZERO)),
	M_DEFAULT_SPOT_LIGHT_COLOR(GET_CONFIG_VALUE_GAME("defaultSpotLightColorRed", REAL_ZERO), GET_CONFIG_VALUE_GAME("defaultSpotLightColorGreen", REAL_ZERO), GET_CONFIG_VALUE_GAME("defaultSpotLightColorBlue", REAL_ONE)),
	M_DEFAULT_SPOT_LIGHT_INTENSITY(GET_CONFIG_VALUE_GAME("defaultSpotLightIntensity", 4.0f)),
	M_DEFAULT_SPOT_LIGHT_ATTENUATION(GET_CONFIG_VALUE_GAME("defaultSpotLightAttenuationConstant", 0.5f), GET_CONFIG_VALUE_GAME("defaultSpotLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE_GAME("defaultSpotLightAttenuationExponent", 0.05f)),
	M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE(GET_CONFIG_VALUE_GAME("defaultSpotLightViewAngle", 120.0f), Math::Unit::DEGREE),
	M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2(GET_CONFIG_VALUE_GAME("defaultSpotLightShadowMapSizeAsPowerOf2", 10)), // 2 ^ 10 = 1024
	M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS(GET_CONFIG_VALUE_GAME("defaultSpotLightShadowSoftness", REAL_ONE)),
	M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT(GET_CONFIG_VALUE_GAME("defaultSpotLightLightBleedingReductionAmount", 0.2f)),
	M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE(GET_CONFIG_VALUE_GAME("defaultSpotLightMinVariance", 0.00002f)),
	m_attenuation(M_DEFAULT_SPOT_LIGHT_ATTENUATION),
	m_viewAngle(M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE),
	m_shadowMapSizeAsPowerOf2(M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2),
	m_nearPlane(0.1f),
	m_shadowSoftness(M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS),
	m_lightBleedingReductionFactor(M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT),
	m_minVariance(M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE)
{
}

void Game::SpotLightBuilder::BuildPart1()
{
	// Determining position
	m_pos.Set(GET_CONFIG_VALUE_GAME("spotLightPosX_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetX()),
		GET_CONFIG_VALUE_GAME("spotLightPosY_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetY()),
		GET_CONFIG_VALUE_GAME("spotLightPosZ_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetZ()));

	// Determining rotation
	Math::Angle angleX(GET_CONFIG_VALUE_GAME("spotLightAngleX_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_X.Get(Math::Unit::DEGREE)));
	Math::Angle angleY(GET_CONFIG_VALUE_GAME("spotLightAngleY_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Y.Get(Math::Unit::DEGREE)));
	Math::Angle angleZ(GET_CONFIG_VALUE_GAME("spotLightAngleZ_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Z.Get(Math::Unit::DEGREE)));
	DEBUG_LOG_GAME("angleX=", angleX, ", angleY=", angleY, ", angleZ=", angleZ);
	m_rot = Math::Quaternion(Math::Matrix4D(angleX, angleY, angleZ));
}

void Game::SpotLightBuilder::BuildPart2()
{
	// Determining color
	m_color.Set(GET_CONFIG_VALUE_GAME("spotLightColorRed_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetRed()),
		GET_CONFIG_VALUE_GAME("spotLightColorGreen_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetGreen()),
		GET_CONFIG_VALUE_GAME("spotLightColorBlue_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetBlue()),
		REAL_ONE /* TODO: Alpha channel? */);

	// Setting intensity
	m_intensity = GET_CONFIG_VALUE_GAME("spotLightIntensity_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_INTENSITY);

	// Setting attenuation
	Math::Real constant = GET_CONFIG_VALUE_GAME("spotLightAttenuationConstant_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetConstant());
	Math::Real linear = GET_CONFIG_VALUE_GAME("spotLightAttenuationLinear_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetLinear());
	Math::Real exponent = GET_CONFIG_VALUE_GAME("spotLightAttenuationExponent_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetExponent());
	m_attenuation = Rendering::Attenuation(constant, linear, exponent);

	// Setting shadow info
	m_viewAngle.Set(GET_CONFIG_VALUE_GAME("spotLightViewAngle_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE.Get(Math::Unit::DEGREE)), Math::Unit::DEGREE);
	m_shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE_GAME("spotLightShadowMapSizeAsPowerOf2_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2);
	m_nearPlane = GET_CONFIG_VALUE_GAME("spotLightProjectionNearPlane", 0.1f);
	m_shadowSoftness = GET_CONFIG_VALUE_GAME("spotLightShadowSoftness_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS);
	m_lightBleedingReductionFactor = GET_CONFIG_VALUE_GAME("spotLightLightBleedingReductionAmount_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT);
	m_minVariance = GET_CONFIG_VALUE_GAME("spotLightMinVariance_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE);

	// TODO: Setting additional spot light information
}

#ifdef BUILD_MESH_RENDERER
void Game::SpotLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around spot light node position to let the user see the source
	m_gameNode->AddComponent(new MeshRendererComponent(m_gameManager->GetMesh("SpotLight.obj"), new Rendering::Material(m_textureFactory.GetTexture(TextureIDs::SPOT_LIGHT), 1.0f, 8.0f)));
	m_gameNode->GetTransform().SetScale(0.1f); /* TODO: Don't use hard-coded values! Ever! */
}
#endif

void Game::SpotLightBuilder::BuildPart3()
{
	m_object->SetShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::SPOT_LIGHT));
	m_object->SetTerrainShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::SPOT_LIGHT_TERRAIN));
	m_object->SetNoShadowShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::SPOT_LIGHT_NO_SHADOWS));
	m_object->SetNoShadowTerrainShader(m_shaderFactory.GetShader(Rendering::ShaderIDs::SPOT_LIGHT_TERRAIN_NO_SHADOWS));
	// TODO: Add new shaders: "fogShader" and "fogTerrainShader".

	m_object->SetTransform(Math::Transform(m_pos, m_rot));
	m_object->SetColor(m_color);
	m_object->SetIntensity(m_intensity);
	m_object->SetAttenuation(m_attenuation);

	m_object->SetShadowInfo(m_viewAngle, m_shadowMapSizeAsPowerOf2, m_nearPlane, m_shadowSoftness, m_lightBleedingReductionFactor, m_minVariance);

	m_object->SetIsEnabled(true);
	m_object->SetIsShadowingEnabled(true);
}
/* ==================== SpotLightBuilder implementation end ==================== */