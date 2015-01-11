#include "common.glh"

#if defined(VS_BUILD)
attribute vec3 position;

void main()
{
    gl_Position = vec4(position, 1.0);
}

#elif defined(GS_BUILD)
layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 T_VP; // View-Projection matrix
uniform vec3 C_eyePos;

out vec2 texCoord0;

void main()
{
	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 directionToCamera = normalize(C_eyePos - pos);
	vec3 upVec = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(directionToCamera, upVec);
	
	pos -= (right * 0.5);
	gl_Position = T_VP * vec4(pos, 1.0);
	texCoord0 = vec2(0.0, 0.0);
	EmitVertex();
	
	pos.y += 1.0;
	gl_Position = T_VP * vec4(pos, 1.0);
	texCoord0 = vec2(0.0, 1.0);
	EmitVertex();
	
	pos.y -= 1.0;
	pos += right;
	gl_Position = T_VP * vec4(pos, 1.0);
	texCoord0 = vec2(1.0, 0.0);
	EmitVertex();
	
	pos.y += 1.0;
	gl_Position = T_VP * vec4(pos, 1.0);
	texCoord0 = vec2(1.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}

#elif defined(FS_BUILD)
uniform sampler2D diffuse; // color map

in vec2 texCoord0;

DeclareFragOutput(0, vec4);
void main()
{
	FragColor = texture2D(diffuse, texCoord0);
	if (FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0)
	{
		discard;
	}
	SetFragOutput(0, FragColor);
}
#endif