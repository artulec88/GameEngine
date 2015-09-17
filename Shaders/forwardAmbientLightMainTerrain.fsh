uniform vec3 R_ambientIntensity;
uniform vec3 C_eyePos;
uniform sampler2D diffuse;
uniform sampler2D diffuse2;
uniform sampler2D terrainMap;
uniform sampler2D displacementMap;

uniform float displacementScale;
uniform float displacementBias;

DeclareFragOutput(0, vec4);
void main()
{
	float distance = CalcFogDistance(C_eyePos, worldPos0, gl_FragCoord);
	float fogFactor = CalcFogFactor(R_ambientFogStart, R_ambientFogEnd, R_ambientFogDensityFactor, distance);
	
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	
	vec2 terrainMapTexColor = normalize(texture2D(terrainMap, texCoords).xy);
	//float terrainMapTexColorSum = terrainMapTexColor.x + terrainMapTexColor.y;
	//terrainMapTexColor
	
	const float minimum = -2.8;
	const float maximum = 1.8;
	const float limit = 0.9;
	vec4 texColor = 0.0;
	
	// This line gives the same results as the if..else clauses, but is slower (around 0.6 ms)
	//texColor = mix(texture2D(diffuse, texCoords), texture2D(diffuse2, texCoords), clamp((worldPos0.y - minimum) / (limit - minimum), 0.0, 1.0));
	if (worldPos0.y < limit)
	{
		texColor = mix(texture2D(diffuse, texCoords), texture2D(diffuse2, texCoords), (worldPos0.y - minimum) / (limit - minimum));
	}
	else
	{
		texColor = texture2D(diffuse2, texCoords);
	}
	//vec4 diffuseTexColor = texture2D(diffuse, texCoords);
	//vec4 diffuse2TexColor = texture2D(diffuse2, texCoords);
	//vec4 texColor = mix(diffuseTexColor, diffuse2TexColor, diffuse2TexColor.r + diffuse2TexColor.g + diffuse2TexColor.b);
	
	SetFragOutput(0, mix(texColor, vec4(R_ambientFogColor, 1.0), fogFactor) * vec4(R_ambientIntensity, 1));
}