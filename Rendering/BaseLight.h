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
	BaseLight(const Math::Vector3D& color = Math::Vector3D(0.0, 0.0, 0.0), Math::Real intensity = 0.0);
	virtual ~BaseLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//virtual void Update(Math::Real delta);
	//virtual void Render(Shader* shader, Renderer* renderer);
	Math::Vector3D GetColor() const;
	void SetColor(const Math::Vector3D& color);
	Math::Real GetIntensity() const;
	void SetIntensity(Math::Real intensity);
	inline Shader* GetShader() { return this->shader; }
	inline ShadowInfo* GetShadowInfo() { return this->shadowInfo; }
	virtual void AddToEngine(CoreEngine* coreEngine);
	virtual bool IsEnabled() const { return isEnabled; }
protected:
	inline void SetShader(Shader* shader);
	inline void SetShadowInfo(ShadowInfo* shadowInfo);
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