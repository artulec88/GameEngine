#pragma once

#include "Shader.h"

namespace Rendering
{

class BasicShader : public Shader
{
public:
	BasicShader(void);
	virtual ~BasicShader(void);

public:
	void UpdateUniforms(const Math::Matrix4D& worldMatrix, const Math::Matrix4D& projectedMatrix, const Material& material); // derived from Shader class
}; /* end class BasicShader */

} /* end namespace Rendering */