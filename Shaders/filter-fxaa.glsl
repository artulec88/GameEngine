// FXAA shader, GLSL code adapted from:
// http://horde3d.org/wiki/index.php5?title=Shading_Technique_-_FXAA
// Whitepaper describing the technique:
// http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf


#include "common.glh"

varying vec2 texCoord0;

#if defined(VS_BUILD)
#include "filter.vsh"
#elif defined(FS_BUILD)

uniform sampler2D R_filterTexture;
uniform vec3 R_inverseFilterTextureSize;
uniform float R_fxaaSpanMax; // controls the magnitude of the blurDir object
uniform float R_fxaaReduceMin;
uniform float R_fxaaReduceMul;

DeclareFragOutput(0, vec4);
void main()
{
	vec2 texCoordOffset = R_inverseFilterTextureSize.xy;
	
	// Edge detection using the luminosity
	// We are going to sample in an X-shape, so we need 5 values:
	// 1. top left pixel 2. top right pixel 3. middle pixel 4. bottom left pixel 5. bottom right pixel
	vec3 luma = vec3(0.299, 0.587, 0.114); // luminosity vector (if you take a dot product of a color and this vector you will get luminosity of that color)
	float lumaTopLeft = dot(luma, texture2D(R_filterTexture, texCoord0.xy + (vec2(-1.0, -1.0) * texCoordOffset)).xyz);
	float lumaTopRight = dot(luma, texture2D(R_filterTexture, texCoord0.xy + (vec2(1.0, -1.0) * texCoordOffset)).xyz);
	float lumaBottomLeft = dot(luma, texture2D(R_filterTexture, texCoord0.xy + (vec2(-1.0, 1.0) * texCoordOffset)).xyz);
	float lumaBottomRight = dot(luma, texture2D(R_filterTexture, texCoord0.xy + (vec2(1.0, 1.0) * texCoordOffset)).xyz);
	float lumaMiddle  = dot(luma, texture2D(R_filterTexture, texCoord0.xy).xyz);

	vec2 blurDir;
	blurDir.x = -((lumaTopLeft + lumaTopRight) - (lumaBottomLeft + lumaBottomRight));
	blurDir.y = ((lumaTopLeft + lumaBottomLeft) - (lumaTopRight + lumaBottomRight));
	
	float dirReduce = max((lumaTopLeft + lumaTopRight + lumaBottomLeft + lumaBottomRight) * (R_fxaaReduceMul * 0.25), R_fxaaReduceMin);
	float inverseDirAdjustment = 1.0/(min(abs(blurDir.x), abs(blurDir.y)) + dirReduce);
	
	blurDir = min(vec2(R_fxaaSpanMax, R_fxaaSpanMax), 
		max(vec2(-R_fxaaSpanMax, -R_fxaaSpanMax), blurDir * inverseDirAdjustment));
	
	blurDir.x = blurDir.x * step(1.0, abs(blurDir.x));
	blurDir.y = blurDir.y * step(1.0, abs(blurDir.y));
	
	//float dirStep = max(step(1.0, abs(blurDir.x)), step(1.0, abs(blurDir.y)));
	//blurDir.x = blurDir.x * dirStep;
	//blurDir.y = blurDir.y * dirStep;
	
	// Perform blurring now
	blurDir = blurDir * texCoordOffset; // to get back to texel space (now blurDir represent the number of texels)

	vec3 result1 = (1.0/2.0) * (
		texture2D(R_filterTexture, texCoord0.xy + (blurDir * vec2(1.0/3.0 - 0.5))).xyz +
		texture2D(R_filterTexture, texCoord0.xy + (blurDir * vec2(2.0/3.0 - 0.5))).xyz);

	vec3 result2 = result1 * (1.0/2.0) + (1.0/4.0) * (
		texture2D(R_filterTexture, texCoord0.xy + (blurDir * vec2(0.0/3.0 - 0.5))).xyz +
		texture2D(R_filterTexture, texCoord0.xy + (blurDir * vec2(3.0/3.0 - 0.5))).xyz);

	float lumaMin = min(lumaMiddle, min(min(lumaTopLeft, lumaTopRight), min(lumaBottomLeft, lumaBottomRight)));
	float lumaMax = max(lumaMiddle, max(max(lumaTopLeft, lumaTopRight), max(lumaBottomLeft, lumaBottomRight)));
	float lumaResult2 = dot(luma, result2); // if lumaResult2 > any of the 5 texels' luminosities then we sampled too much and should return result1
	
	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax) // if lumaResult2 is outside the luminosity range
		SetFragOutput(0, vec4(result1, 1.0));
	else
		SetFragOutput(0, vec4(result2, 1.0));
}
#endif
