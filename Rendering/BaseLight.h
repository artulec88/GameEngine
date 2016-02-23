#ifndef __RENDERING_BASE_LIGHT_H__
#define __RENDERING_BASE_LIGHT_H__

#include "Rendering.h"
#include "ShadowInfo.h"
#include "Color.h"
#include "Math\Vector.h"
#include "Math\Transform.h"
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

class BaseLight
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary>The constructor.</summary>
	/// <param name="transform">The transform of the light.</param>
	/// <param name="color">The color of the light.</param>
	/// <param name="intensity">The intensity of the light.</param>
	/// <remarks><code>explicit</code> keyword is used to prevent implicit conversions between <code>Color</code> objects and <code>BaseLight</code>.</remarks>
	RENDERING_API BaseLight(Math::Transform& transform, const Color& color, Math::Real intensity);
	
	/// <summary>The destructor.</summary>
	RENDERING_API virtual ~BaseLight(void);
private:
	BaseLight(const BaseLight& baseLight); // Copy constructor disabled
	void operator=(BaseLight& baseLight); // Assignment operator disabled
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//virtual void Update(Math::Real delta);
	//virtual void Render(Shader* shader, Renderer* renderer);
	Color GetColor() const { return m_color; }
	Math::Real GetIntensity() const { return m_intensity; }
	inline Shader* GetShader() { return m_shader; }
	inline Shader* GetTerrainShader() { return m_terrainShader; }
	inline Shader* GetNoShadowShader() { return m_noShadowShader; }
	inline Shader* GetNoShadowTerrainShader() { return m_noShadowTerrainShader; }
	inline ShadowInfo* GetShadowInfo() { return m_shadowInfo; }
	virtual bool IsEnabled() const { return m_isEnabled; }
	
	bool IsShadowingEnabled() const { return m_isShadowingEnabled; }

	virtual ShadowCameraTransform CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot);
	Math::Transform& GetTransform() { return m_transform; }
	const Math::Transform& GetTransform() const { return m_transform; }

	RENDERING_API void SetShader(Shader* shader);
	RENDERING_API void SetTerrainShader(Shader* terrainShader);
	RENDERING_API void SetNoShadowShader(Shader* noShadowShader);
	RENDERING_API void SetNoShadowTerrainShader(Shader* noShadowTerrainShader);
	void SetShadowInfo(ShadowInfo* shadowInfo);

#ifdef ANT_TWEAK_BAR_ENABLED
	virtual void InitializeTweakBar(TwBar* lightsBar);
#endif
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	/// <summary> The base light transform. </summary>
	Math::Transform& m_transform;

	/// <summary>The light color.</summary>
	Color m_color;

	/// <summary>The light intensity.</summary>
	Math::Real m_intensity;

	/// <summary>The default shader for the light.</summary>
	Shader* m_shader;

	/// <summary>The terrain shader for the light.</summary>
	Shader* m_terrainShader;

	/// <summary> The default shader with no shadow calculation for the light. </summary>
	Shader* m_noShadowShader;

	/// <summary> The terrain shader with no shadow calculation for the light. </summary>
	Shader* m_noShadowTerrainShader;

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