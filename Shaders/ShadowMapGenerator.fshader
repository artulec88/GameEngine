#version 330 core

void main()
{
	//gl_FragColor = vec4(gl_FragCoord.z); // This shouldn't be used in basic shadow map generation
	
	/**
	 * For variance shadow mapping first argument will become the mean value.
	 * We are storing the information to calculate the variance in the second argument.
	 */
	 float depth = gl_FragCoord.z;
	 // Introducing depth bias using dx, dy and moment2 calculation.
	 // The math behind the derivative biasing, along with a lot of other math behind variance shadow mapping, is:
	 // http://http.developer.nvidia.com/GPUGems3/gpugems3_ch08.html
	 float dx = dFdx(depth);
	 float dy = dFdy(depth);
	 float moment2 = depth * depth + 0.25 * (dx * dx + dy * dy);
	 gl_FragColor = vec4(gl_FragCoord.z, moment2, 0.0, 0.0);
}