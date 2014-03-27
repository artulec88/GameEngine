#pragma once

//#include "Shader.h"
#include "Math\Vector.h"
#include "GameComponent.h"

namespace Rendering
{

// TODO: Move this to GameComponent class declaration when BaseLight becomes a child of GameComponent class
// class Shader;

class RENDERING_API BaseLight : public GameComponent
{
/* ==================== Non-static member variables begin ==================== */
protected:
	Math::Vector3D color;
	Math::Real intensity;
	Shader* shader;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	BaseLight(const Math::Vector3D& color = Math::Vector3D(0.0, 0.0, 0.0), Math::Real intensity = 0.0);
	virtual ~BaseLight(void);
protected:
	BaseLight(Shader* shader, const Math::Vector3D& color = Math::Vector3D(0.0, 0.0, 0.0), Math::Real intensity = 0.0);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Math::Vector3D GetColor() const;
	void SetColor(const Math::Vector3D& color);
	Math::Real GetIntensity() const;
	void SetIntensity(Math::Real intensity);
	//inline void SetShader(Shader* shader) { this->shader = shader; }
	inline Shader* GetShader() { return this->shader; }
	virtual void AddToRenderingEngine(Renderer* renderer);
/* ==================== Non-static member functions end ==================== */
}; /* end class BaseLight */

} /* end namespace Rendering */