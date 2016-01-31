#include "common.glh"

varying vec2 texCoord0;
varying vec3 worldPos0;
varying mat3 tbnMatrix;

#if defined(VS_BUILD)
#include "forwardAmbientLighting.vsh"
#elif defined(FS_BUILD)
#include "parallaxMapping.glh"
#include "tiling-terrain.glh"

uniform vec3 R_ambientIntensity;
uniform vec3 C_eyePos;

uniform sampler2D diffuse;
uniform sampler2D diffuse2;
uniform sampler2D diffuse3;
uniform sampler2D diffuse4;
uniform sampler2D blendMap;

uniform sampler2D displacementMap;
uniform float displacementScale;
uniform float displacementBias;

DeclareFragOutput(0, vec4);
void main()
{
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec4 blendMapColor = texture2D(blendMap, texCoord0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	vec2 tiledCoords = texCoords * TILING;
	
	float backTextureAmount = 1.0 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec4 diffuseTextureColor = texture2D(diffuse, tiledCoords) * backTextureAmount;
	vec4 diffuse2TextureColor = texture2D(diffuse2, tiledCoords) * blendMapColor.r;
	vec4 diffuse3TextureColor = texture2D(diffuse3, tiledCoords) * blendMapColor.g;
	vec4 diffuse4TextureColor = texture2D(diffuse4, tiledCoords) * blendMapColor.b;
	
	vec4 totalColor = diffuseTextureColor + diffuse2TextureColor + diffuse3TextureColor + diffuse4TextureColor;
	
	SetFragOutput(0, totalColor * vec4(R_ambientIntensity, 1));
}
#endif
