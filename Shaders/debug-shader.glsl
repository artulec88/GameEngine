#include "common.glh"

varying vec2 texCoord0;

#if defined(VS_BUILD)
attribute vec2 position;

uniform mat4 guiTransformationMatrix;

void main()
{
	gl_Position = guiTransformationMatrix * vec4(position, 0.0, 1.0);
	
	// The calculation of texture coordinates (see https://www.youtube.com/watch?v=vOmJ1lyiJ4A&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=24 at exactly 9:36)
	texCoord0 = vec2((position.x+1.0)/2.0, 1 - (position.y+1.0)/2.0);
}
#elif defined(FS_BUILD)

uniform sampler2D guiTexture;

DeclareFragOutput(0, vec4);
void main()
{
	SetFragOutput(0, texture(guiTexture, texCoord0));
}
#endif
