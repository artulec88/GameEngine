#pragma once

#include "Shader.h"

namespace Rendering
{

class RENDERING_API BasicShader : public Shader
{
/* ==================== Static functions begin ==================== */
public:
	static BasicShader* GetInstance();
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	virtual ~BasicShader(void);
private:
	BasicShader(void);
	BasicShader(BasicShader const&) {}
	void operator=(BasicShader const&) {}
/* ==================== Constructors and destructors end ==================== */

public:
	void UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer); // derived from Shader class
}; /* end class BasicShader */

} /* end namespace Rendering */