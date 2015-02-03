/*
 * Copyright (C) 2014 Benny Bobaganoosh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sampling.glh"

uniform sampler2D diffuse;
uniform sampler2D diffuse2;
uniform sampler2D normalMap;
uniform sampler2D displacementMap;

uniform float displacementScale;
uniform float displacementBias;

uniform sampler2D R_shadowMap;
uniform float R_shadowVarianceMin;
uniform float R_shadowLightBleedingReductionFactor;

bool InRange(float val)
{
	return val >= 0.0 && val <= 1.0;
}

float CalcShadowAmount(sampler2D shadowMap, vec4 initialShadowMapCoords)
{
	vec3 shadowMapCoords = (initialShadowMapCoords.xyz/initialShadowMapCoords.w);
	
	if(InRange(shadowMapCoords.z) && InRange(shadowMapCoords.x) && InRange(shadowMapCoords.y))
	{
		return SampleVarianceShadowMap(shadowMap, shadowMapCoords.xy, shadowMapCoords.z, R_shadowVarianceMin, R_shadowLightBleedingReductionFactor);
	}
	else
	{
		return 1.0; // point is outside the range, so we render it in light
	}
}

DeclareFragOutput(0, vec4);
void main()
{
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	vec3 normal = normalize(tbnMatrix * (255.0/128.0 * texture2D(normalMap, texCoords).xyz - 1));
    
    vec4 lightingAmt = CalcLightingEffect(normal, worldPos0) * CalcShadowAmount(R_shadowMap, shadowMapCoords0);
	
	vec4 diffuseTexColor = texture2D(diffuse, texCoords);
	vec4 diffuse2TexColor = texture2D(diffuse2, texCoords);
	vec4 texColor = mix(diffuseTexColor, diffuse2TexColor, diffuse2TexColor.r + diffuse2TexColor.g + diffuse2TexColor.b);
	
    SetFragOutput(0, texColor * lightingAmt);
}
