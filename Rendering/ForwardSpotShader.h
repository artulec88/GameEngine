#pragma once

#include "Shader.h"

namespace Rendering
{

class ForwardSpotShader : public Shader
{
/* ==================== Static functions begin ==================== */
public:
	static ForwardSpotShader* GetInstance();
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	virtual ~ForwardSpotShader(void);
private:
	ForwardSpotShader(void);
	ForwardSpotShader(ForwardSpotShader const&) {}
	void operator=(ForwardSpotShader const&) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer); // derived from Shader class
/* ==================== Non-static member functions end ==================== */
}; /* end class ForwardSpotShader */

} /* end namespace Rendering */