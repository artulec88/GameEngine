#pragma once

#include "Shader.h"

namespace Rendering
{

class ForwardAmbientShader : public Shader
{
/* ==================== Static functions begin ==================== */
public:
	static ForwardAmbientShader* GetInstance();
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	virtual ~ForwardAmbientShader(void);
private:
	ForwardAmbientShader(void);
	ForwardAmbientShader(ForwardAmbientShader const&) {}
	void operator=(ForwardAmbientShader const&) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer); // derived from Shader class
/* ==================== Non-static member functions end ==================== */
}; /* end class ForwardAmbientShader */

} /* end namespace Rendering */