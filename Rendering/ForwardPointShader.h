#pragma once

#include "Shader.h"

namespace Rendering
{

class ForwardPointShader : public Shader
{
/* ==================== Static functions begin ==================== */
public:
	static ForwardPointShader* GetInstance();
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	virtual ~ForwardPointShader(void);
private:
	ForwardPointShader(void);
	ForwardPointShader(ForwardPointShader const&) {}
	void operator=(ForwardPointShader const&) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer); // derived from Shader class
/* ==================== Non-static member functions end ==================== */
}; /* end class ForwardPointShader */

} /* end namespace Rendering */