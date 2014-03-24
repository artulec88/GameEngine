#pragma once

#include "Shader.h"

namespace Rendering
{

class RENDERING_API BasicShader : public Shader
{
public:
	BasicShader(void);
	virtual ~BasicShader(void);

public:
	void UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer); // derived from Shader class
}; /* end class BasicShader */

} /* end namespace Rendering */