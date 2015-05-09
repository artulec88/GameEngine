#ifndef __RENDERING_LIGHT_BUILDER_H__
#define __RENDERING_LIGHT_BUILDER_H__

#include "Rendering.h"
#include "GameNode.h"
#include "SpotLight.h"
#include <string>

#define RENDER_LIGHT_MESHES

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
#ifdef RENDER_LIGHT_MESHES
	virtual void BuildMeshRenderer() = 0;
#endif
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
#ifdef RENDER_LIGHT_MESHES
	virtual void BuildMeshRenderer();
#endif

	GameNode* GetLightNode() { return m_directionalLightNode; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	GameNode* m_directionalLightNode;
/* ==================== Non-static member variables end ==================== */
}; /* end class DirectionalLightBuilder */

class PointLightBuilder : public LightBuilder
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API PointLightBuilder();
	RENDERING_API virtual ~PointLightBuilder(void) { };
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void BuildPart1();
	virtual void BuildPart2();
#ifdef RENDER_LIGHT_MESHES
	virtual void BuildMeshRenderer();
#endif

	RENDERING_API void SetPointLightIndex(int pointLightIndex);

	GameNode* GetLightNode() { return m_pointLightNode; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	const Math::Vector3D M_DEFAULT_POINT_LIGHT_POS;
	const Math::Angle M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X;
	const Math::Angle M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y;
	const Math::Angle M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z;
	const Color M_DEFAULT_POINT_LIGHT_COLOR;
	const Math::Real M_DEFAULT_POINT_LIGHT_INTENSITY;
	const Attenuation M_DEFAULT_POINT_LIGHT_ATTENUATION;

	int m_pointLightIndex;
	std::string m_pointLightIndexStr;
	GameNode* m_pointLightNode;
/* ==================== Non-static member variables end ==================== */
}; /* end class PointLightBuilder */

class SpotLightBuilder : public LightBuilder
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API SpotLightBuilder();
	RENDERING_API virtual ~SpotLightBuilder(void) { };
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void BuildPart1();
	virtual void BuildPart2();
#ifdef RENDER_LIGHT_MESHES
	virtual void BuildMeshRenderer();
#endif

	RENDERING_API void SetSpotLightIndex(int spotLightIndex);

	GameNode* GetLightNode() { return m_spotLightNode; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	const Math::Vector3D M_DEFAULT_SPOT_LIGHT_POS;
	const Math::Angle M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_X;
	const Math::Angle M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Y;
	const Math::Angle M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Z;
	const Color M_DEFAULT_SPOT_LIGHT_COLOR;
	const Math::Real M_DEFAULT_SPOT_LIGHT_INTENSITY;
	const Attenuation M_DEFAULT_SPOT_LIGHT_ATTENUATION;
	const Math::Angle M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE;
	const int M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2;
	const Math::Real M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS;
	const Math::Real M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT;
	const Math::Real M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE;

	int m_spotLightIndex;
	std::string m_spotLightIndexStr;
	GameNode* m_spotLightNode;
/* ==================== Non-static member variables end ==================== */
}; /* end class SpotLightBuilder */

} /* end namespace Rendering */

#endif // __RENDERING_LIGHT_BUILDER_H__
