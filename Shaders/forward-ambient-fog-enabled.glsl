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

#include "common.glh"

varying vec2 texCoord0;
varying vec3 worldPos0;
varying mat3 tbnMatrix;

#if defined(VS_BUILD)
attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 tangent;

uniform mat4 T_model;
uniform mat4 T_MVP;

void main()
{
    gl_Position = T_MVP * vec4(position, 1.0);
    texCoord0 = texCoord; 
    worldPos0 = (T_model * vec4(position, 1.0)).xyz;
    
    vec3 n = normalize((T_model * vec4(normal, 0.0)).xyz);
    vec3 t = normalize((T_model * vec4(tangent, 0.0)).xyz);
    t = normalize(t - dot(t, n) * n);
    
    vec3 biTangent = cross(t, n);
    tbnMatrix = mat3(t, biTangent, n);
}
#elif defined(FS_BUILD)
#include "sampling.glh"

uniform vec3 R_ambientFogColor;
uniform float R_ambientFogStart;
uniform float R_ambientFogEnd;
uniform vec3 R_ambientIntensity;
uniform vec3 C_eyePos;
uniform sampler2D diffuse;
uniform sampler2D diffuse2;
uniform sampler2D displacementMap;

uniform float displacementScale;
uniform float displacementBias;

DeclareFragOutput(0, vec4);
void main()
{
	float distance = length(C_eyePos - worldPos0);
	float fogFactor = 1.0 - clamp((R_ambientFogEnd - distance) / (R_ambientFogEnd - R_ambientFogStart), 0.0, 1.0);
	
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = CalcParallaxTexCoords(displacementMap, tbnMatrix, directionToEye, texCoord0, displacementScale, displacementBias);
	
	const float minimum = -1.8;
	const float maximum = 0.8;
	const float limit = 0.5;
	vec4 texColor = 0.0;
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
#endif
