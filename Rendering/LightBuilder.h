#ifndef __RENDERING_LIGHT_BUILDER_H__
#define __RENDERING_LIGHT_BUILDER_H__

#include "Rendering.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace Rendering
{

class LightBuilder
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API LightBuilder();
	RENDERING_API virtual ~LightBuilder(void);
private:
	LightBuilder(LightBuilder& lightBuilder) {}
	void operator=(LightBuilder& lightBuilder) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void BuildPart1() = 0;
	virtual void BuildPart2() = 0;
	virtual void AddToEngine() = 0;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class LightBuilder */

class DirectionalLightBuilder : public LightBuilder
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API DirectionalLightBuilder() { };
	RENDERING_API virtual ~DirectionalLightBuilder(void) { };
private:
	DirectionalLightBuilder(DirectionalLightBuilder& directionalLightBuilder) {}
	void operator=(DirectionalLightBuilder& directionalLightBuilder) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void BuildPart1();
	virtual void BuildPart2();
	virtual void AddToEngine();
	DirectionalLight* GetLight() { return m_directionalLight; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	DirectionalLight* m_directionalLight;
/* ==================== Non-static member variables end ==================== */
}; /* end class DirectionalLightBuilder */

class PointLightBuilder : public LightBuilder
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API PointLightBuilder() { };
	RENDERING_API virtual ~PointLightBuilder(void) { };
private:
	PointLightBuilder(PointLightBuilder& pointLightBuilder) {}
	void operator=(PointLightBuilder& pointLightBuilder) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void BuildPart1();
	virtual void BuildPart2();
	virtual void AddToEngine();
	PointLight* GetLight() { return m_pointLight; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	PointLight* m_pointLight;
/* ==================== Non-static member variables end ==================== */
}; /* end class PointLightBuilder */

class SpotLightBuilder
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API SpotLightBuilder() { };
	RENDERING_API virtual ~SpotLightBuilder(void) { };
private:
	SpotLightBuilder(SpotLightBuilder& spotLightBuilder) {}
	void operator=(SpotLightBuilder& spotLightBuilder) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void BuildPart1();
	virtual void BuildPart2();
	virtual void AddToEngine();
	SpotLight* GetLight() { return m_spotLight; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	SpotLight* m_spotLight;
/* ==================== Non-static member variables end ==================== */
}; /* end class SpotLightBuilder */

} /* end namespace Rendering */

#endif // __RENDERING_LIGHT_BUILDER_H__
