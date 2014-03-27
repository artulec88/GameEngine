#pragma once

#include "Shader.h"

namespace Rendering
{

class ForwardDirectionalShader : public Shader
{
/* ==================== Static functions begin ==================== */
public:
	static ForwardDirectionalShader* GetInstance();
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	virtual ~ForwardDirectionalShader(void);
private:
	ForwardDirectionalShader(void);
	ForwardDirectionalShader(ForwardDirectionalShader const&) {}
	void operator=(ForwardDirectionalShader const&) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer); // derived from Shader class
/* ==================== Non-static member functions end ==================== */
}; /* end class ForwardDirectionalShader */

} /* end namespace Rendering */