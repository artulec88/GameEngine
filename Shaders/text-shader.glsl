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

#if defined(VS_BUILD)
attribute vec2 vVertex; // in screen space
attribute vec2 vTexCoord;

//uniform float screenWidth;
//uniform float screenHeight;
//uniform mat4 MVP;

void main()
{
	// Output position of the vertex, in clip space
	// map [0..screenWidth][0..screenHeight] to [-1..1][-1..1]
	const vec2 screenHalfSize = vec2(512,300); // TODO: Pass the screen width and height here
	vec2 vertexPosition_homoneneousspace = vVertex - screenHalfSize; // [0..screenWidth][0..screenHeight] -> [-400..400][-300..300]
	vertexPosition_homoneneousspace /= screenHalfSize;
	gl_Position =  vec4(vertexPosition_homoneneousspace, 0, 1);
	
	texCoord0 = vTexCoord;
	
	//texCoord0 = vTexCoord;
    //gl_Position = MVP * vec4(vVertex, 0.0, 1.0);
}

#elif defined(FS_BUILD)
uniform sampler2D diffuse; // font texture
//uniform bool selected;

DeclareFragOutput(0, vec4);
void main()
{
	vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
	//vec4 vFragColor = color;
	vec4 vFragColor = texture(diffuse, texCoord0.st) * color;
	if (vFragColor.rgb == vec3(0.0, 0.0, 0.0))
	{
		//SetFragOutput(0, vec4(0.0, 1.0, 0.0, 1.0));
		discard;
	}
	SetFragOutput(0, vFragColor);
}
#endif
