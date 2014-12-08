#pragma once

#include "Rendering.h"
#include "GameComponent.h"
#include "Math\Vector.h"

namespace Rendering
{

// TODO: Move this to GameComponent class declaration when BaseLight becomes a child of GameComponent class
class Shader;
class ShadowInfo;
class Renderer;

class RENDERING_API BaseLight : public GameComponent
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	BaseLight(const Math::Vector3D& color = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Real intensity = REAL_ZERO);
	virtual ~BaseLight(void);
private:
	BaseLight(const BaseLight& baseLight) {} // Copy constructor disabled
	void operator=(BaseLight& baseLight) {} // Assignment operator disabled
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//virtual void Update(Math::Real delta);
	//virtual void Render(Shader* shader, Renderer* renderer);
	Math::Vector3D GetColor() const { return this->color; }
	Math::Real GetIntensity() const { return this->intensity; }
	inline Shader* GetShader() { return this->shader; }
	inline ShadowInfo* GetShadowInfo() { return this->shadowInfo; }
	virtual void AddToEngine(CoreEngine* coreEngine);
	virtual bool IsEnabled() const { return isEnabled; }
protected:
	void SetShader(Shader* shader);
	void SetShadowInfo(ShadowInfo* shadowInfo);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Math::Vector3D color;
	Math::Real intensity;
	Shader* shader;
	ShadowInfo* shadowInfo;
	bool isEnabled;
/* ==================== Non-static member variables end ==================== */
}; /* end class BaseLight */

} /* end namespace Rendering */