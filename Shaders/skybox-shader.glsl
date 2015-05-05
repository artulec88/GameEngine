/*
 * Copyright (C) 2014 Artur Osesik
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

varying vec3 texCoord0;

#if defined(VS_BUILD)
attribute vec3 position;
uniform mat4 T_MVP;

void main()
{
	texCoord0 = position;
	//texCoord0 = vec3(position.x, -position.yz);
	//vec4 pos = (T_MVP * vec4(position, 1.0)).xyww;
	vec4 pos = (T_MVP * vec4(position, 1.0)).xyww;
    gl_Position = pos;
}
#elif defined(FS_BUILD)
#include "parallaxMapping.glh"
#include "sampling.glh"

uniform float R_dayNightMixFactor; // within range [0; 1] where 0 means total night and 1 means total day
uniform samplerCube cubeMapDay;
uniform samplerCube cubeMapNight;

DeclareFragOutput(0, vec4);
void main()
{
	const int size = 128;
	const float sizeFactor = 1.0 / size;
	//vec3 fixedTexCoords = clamp(texCoord0, 1.0 / 1024, 1.0 - 1.0 / 1024);
	
	vec3 fixedTexCoords = texCoord0;
	if (texCoord0.x == 0.0)
	{
		fixedTexCoords.x = sizeFactor;
	}
	if (texCoord0.y == 0.0)
	{
		fixedTexCoords.y = sizeFactor;
	}
	if (texCoord0.z == 0.0)
	{
		fixedTexCoords.z = sizeFactor;
	}
	if (texCoord0.x == 1.0)
	{
		fixedTexCoords.x = 1.0 - sizeFactor;
	}
	if (texCoord0.y == 1.0)
	{
		fixedTexCoords.y = 1.0 - sizeFactor;
	}
	if (texCoord0.z == 1.0)
	{
		fixedTexCoords.z = 1.0 - sizeFactor;
	}
	SetFragOutput(0, mix(texture(cubeMapNight, fixedTexCoords), texture(cubeMapDay, fixedTexCoords), R_dayNightMixFactor));
}
#endif
