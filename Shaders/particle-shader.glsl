#include "common.glh"

varying vec2 texCoord0; // texture coordinates for the current life stage the particle is in
varying vec2 texCoord1; // texture coordinates for the next life stage the particle will be in (if "current stage" is the latest one then "next stage" == "current stage")
varying float particleLifeStageBlendFactor; // the blend factor of 0 means the particle has just switched the state to "current stage" and the closer it is to 1 the more "next stage" will be blended into the final output color.

#if defined(VS_BUILD)
layout(location = 0) attribute vec2 position;
layout(location = 1) attribute mat4 mvp; // instanced attribute
layout(location = 5) attribute vec4 textureOffsets; // instanced attribute; two texture offsets (first one is in XY components and the second in ZW components)
layout(location = 6) attribute float lifeStageBlendFactor; // instanced attribute

uniform float textureAtlasRowsCount;

void main()
{
	vec2 texCoords = position + vec2(0.5, 0.5);
	texCoords.y = 1.0 - texCoords.y;
	texCoords /= textureAtlasRowsCount;
	texCoord0 = texCoords + textureOffsets.xy;
	texCoord1 = texCoords + textureOffsets.zw;
	particleLifeStageBlendFactor = lifeStageBlendFactor;
	
    gl_Position = mvp * vec4(position, 0.0, 1.0);
}

#elif defined(FS_BUILD)

uniform sampler2D particleTexture;

DeclareFragOutput(0, vec4);
void main()
{
	vec4 currentParticleStageColor = texture(particleTexture, texCoord0);
	vec4 nextParticleStageColor = texture(particleTexture, texCoord1);
	SetFragOutput(0, mix(currentParticleStageColor, nextParticleStageColor, particleLifeStageBlendFactor));
}
#endif