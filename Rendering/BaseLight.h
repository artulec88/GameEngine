#ifndef __RENDERING_BASE_LIGHT_H__
#define __RENDERING_BASE_LIGHT_H__

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

class BaseLight : public GameComponent
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary>The constructor.</summary>
	/// <param name="color">The color of the light.</param>
	/// <param name="intensity">The intensity of the light.</param>
	/// <remarks><code>explicit</code> keyword is used to prevent implicit conversions between <code>Color</code> objects and <code>BaseLight</code>.</remarks>
	explicit BaseLight(const Color& color = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE), Math::Real intensity = REAL_ZERO);
	
	/// <summary>The destructor.</summary>
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

	void SetShader(Shader* shader);
	void SetTerrainShader(Shader* terrainShader);
	void SetShadowInfo(ShadowInfo* shadowInfo);

#ifdef ANT_TWEAK_BAR_ENABLED
	virtual void InitializeTweakBar(TwBar* lightsBar);
#endif
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	/// <summary>The light color.</summary>
	Color m_color;

	/// <summary>The light intensity.</summary>
	Math::Real m_intensity;

	/// <summary>The default shader for the light.</summary>
	Shader* m_shader;

	/// <summary>The terrain shader for the light.</summary>
	Shader* m_terrainShader;

	/// <summary>The information about the shadow that the light casts.</summary>
	ShadowInfo* m_shadowInfo;

	/// <summary>The information whether the light is enabled or not.</summary>
	bool m_isEnabled;

	/// <summary>The information about whether the light casts shadows or not.</summary>
	bool m_isShadowingEnabled;
/* ==================== Non-static member variables end ==================== */
}; /* end class BaseLight */

} /* end namespace Lighting */

} /* end namespace Rendering */

#endif /* __RENDERING_BASE_LIGHT_H__ */