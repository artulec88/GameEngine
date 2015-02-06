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
#include "sampling.glh"

uniform float R_timeOfDay; // current daytime in seconds [0; 86400)
uniform samplerCube cubeMapDay;
uniform samplerCube cubeMapNight;

DeclareFragOutput(0, vec4);
void main()
{
	const float sunriseStartsAt = 21600; // the exact time of the moment when the sun starts rising in seconds [0; 86400)
	const float sunriseFinishesAt = 22800; // the exact time of the moment when the sun finishes rising in seconds [0; 86400)
	const float sunsetStartsAt = 64800; // the exact time of the moment when the sun starts setting in seconds [0; 86400)
	const float sunsetFinishesAt = 66000; // the exact time of the moment when the sun finishes setting in seconds [0; 86400)

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
	
	float mixFactor;
	if ((R_timeOfDay >= sunriseFinishesAt) && (R_timeOfDay < sunsetStartsAt))
	{
		mixFactor = 1.0;
	}
	else if ((R_timeOfDay >= sunsetFinishesAt) || (R_timeOfDay < sunriseStartsAt))
	{
		mixFactor = 0.0;
	}
	else if ((R_timeOfDay >= sunriseStartsAt) && (R_timeOfDay < sunriseFinishesAt)) // night to day conversion
	{
		mixFactor = mix(0.0, 1.0, (R_timeOfDay - sunriseStartsAt) / (sunriseFinishesAt - sunriseStartsAt));
	}
	else /* day to night conversion */
	{
		mixFactor = mix(1.0, 0.0, (R_timeOfDay - sunsetStartsAt) / (sunsetFinishesAt - sunsetStartsAt));
	}
	SetFragOutput(0, mix(texture(cubeMapNight, fixedTexCoords), texture(cubeMapDay, fixedTexCoords), mixFactor));
}
#endif
