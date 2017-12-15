/**
 * http://http.developer.nvidia.com/GPUGems3/gpugems3_ch08.html
 * https://www.youtube.com/watch?v=F5QAkUloGOs&index=13&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl
 */

#include "common.glh"

#if defined(VS_BUILD)
attribute vec3 position;

uniform mat4 T_MVP;

void main()
{
    gl_Position = T_MVP * vec4(position, 1.0);
}
#elif defined(FS_BUILD)
DeclareFragOutput(0, vec4);
void main()
{
	const float OFFSET = 0.001; // see http://www.sunandblackcat.com/tipFullView.php?l=eng&topicid=35 (section "Additional offset to depth values in shadow map")
	float depth = gl_FragCoord.z + OFFSET;

	float dx = dFdx(depth);
	float dy = dFdy(depth);
	float moment2 = depth * depth + 0.25 * (dx * dx + dy * dy);

	SetFragOutput(0, vec4(depth, moment2, 0.0, 0.0));
}
#endif
