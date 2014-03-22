#pragma once

#include "Shader.h"

namespace Rendering
{

class PhongShader : public Shader
{
public:
	PhongShader(void);
	virtual ~PhongShader(void);

public:
	void UpdateUniforms(const Math::Matrix4D& worldMatrix, const Math::Matrix4D& projectedMatrix, const Material& material); // derived from Shader class
}; /* end class BasicShader */

} /* end namespace Rendering */