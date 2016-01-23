#include "common.glh"

#if defined(VS_BUILD)
attribute vec3 position;

void main()
{
    gl_Position = vec4(position, 1.0);
}

#elif defined(GS_BUILD)
layout(points) in; // The point list is the incoming topology
layout(triangle_strip) out; // The triangle strip is the outgoing topology
layout(max_vertices = 4) out; // We will emit no more than four vertices (we know we want to emit a quad)

uniform mat4 T_VP;
uniform vec3 C_eyePos;

out vec2 texCoord0;

void main()
{
	/* 
	 * Since GS is executed on a complete primitive (e.g. points list, triangle) we actually have access to each of the vertices that comprise it.
	 * This is done using the built-in variable 'gl_in'.
	 */
	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 directionToCamera = normalize(C_eyePos - pos);
	vec3 upVec = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(directionToCamera, upVec);
	//vec3 right = cross(upVec, directionToCamera);
	
	pos -= (right * 0.5);
	gl_Position = T_VP * vec4(pos, 1.0);
	texCoord0 = vec2(0.0, 1.0);
	EmitVertex();
	
	pos.y += 1.0;
	gl_Position = T_VP * vec4(pos, 1.0);
	texCoord0 = vec2(0.0, 0.0);
	EmitVertex();
	
	pos.y -= 1.0;
	pos += right;
	gl_Position = T_VP * vec4(pos, 1.0);
	texCoord0 = vec2(1.0, 1.0);
	EmitVertex();
	
	pos.y += 1.0;
	gl_Position = T_VP * vec4(pos, 1.0);
	texCoord0 = vec2(1.0, 0.0);
	EmitVertex();
	
	EndPrimitive();
}

#elif defined(FS_BUILD)
uniform sampler2D diffuse;

in vec2 texCoord0;

DeclareFragOutput(0, vec4);
void main()
{
	vec4 fragColor = texture2D(diffuse, texCoord0);
	if (fragColor.r > 0.5 && fragColor.g > 0.5 && fragColor.b > 0.5)
	{
		discard;
	}
	SetFragOutput(0, fragColor);
}
#endif