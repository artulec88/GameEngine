#pragma once

#include "Rendering.h"
#include "GameComponent.h"
#include "ShadowInfo.h"
#include "Color.h"
#include "Math\Vector.h"
#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBarTypes.h"
#include "AntTweakBar\include\AntTweakBar.h"
#endif

namespace Rendering
{

// TODO: Move this to GameComponent class declaration when BaseLight becomes a child of GameComponent class
class Shader;
class Renderer;

namespace Lighting
{
/// <summary>
/// Possible types of lights
/// </summary>
enum LightType
{
	DIRECTIONAL = 0,
	POINT,
	SPOT
};

class RENDERING_API BaseLight : public GameComponent
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	explicit BaseLight(const Color& color = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE), Math::Real intensity = REAL_ZERO);
	virtual ~BaseLight(void);
private:
	BaseLight(const BaseLight& baseLight) {} // Copy constructor disabled
	void operator=(BaseLight& baseLight) {} // Assignment operator disabled
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//virtual void Update(Math::Real delta);
	//virtual void Render(Shader* shader, Renderer* renderer);
	Color GetColor() const { return m_color; }
	Math::Real GetIntensity() const { return m_intensity; }
	inline Shader* GetShader() { return m_shader; }
	inline Shader* GetTerrainShader() { return m_terrainShader; }
	inline ShadowInfo* GetShadowInfo() { return m_shadowInfo; }
	virtual bool IsEnabled() const { return m_isEnabled; }
	
	bool IsShadowingEnabled() const { return m_isShadowingEnabled; }

	virtual ShadowCameraTransform CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot);

	virtual LightType GetLightType() const = 0;

	void SetShader(Shader* shader);
	void SetTerrainShader(Shader* terrainShader);
	void SetShadowInfo(ShadowInfo* shadowInfo);

#ifdef ANT_TWEAK_BAR_ENABLED
	virtual void InitializeTweakBar(TwBar* lightsBar);
#endif
protected:
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Color m_color;
	Math::Real m_intensity;
	Shader* m_shader;
	Shader* m_terrainShader;
	ShadowInfo* m_shadowInfo;
	bool m_isEnabled;
	bool m_isShadowingEnabled;
/* ==================== Non-static member variables end ==================== */
}; /* end class BaseLight */

} /* end namespace Lighting */

} /* end namespace Rendering */