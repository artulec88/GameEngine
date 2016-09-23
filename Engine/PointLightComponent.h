#ifndef __ENGINE_POINT_LIGHT_COMPONENT_H__
#define __ENGINE_POINT_LIGHT_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"

#include "Rendering\PointLight.h"

namespace Engine
{
	class PointLightComponent : public GameComponent
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		PointLightComponent(Rendering::Lighting::PointLight* pointLight);
		virtual ~PointLightComponent();
		PointLightComponent(const PointLightComponent& pointLightComponent) = delete;
		PointLightComponent(PointLightComponent&& pointLightComponent);
		PointLightComponent& operator=(const PointLightComponent& pointLightComponent) = delete;
		PointLightComponent& operator=(PointLightComponent&& pointLightComponent);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		//virtual Math::Transform& GetTransform() { return GameComponent::GetTransform(); }
		//virtual const Math::Transform& GetTransform() const { return GameComponent::GetTransform(); }
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		Rendering::Lighting::PointLight* m_pointLight;
	/* ==================== Non-static member variables end ==================== */
	}; // end class PointLightComponent
} // end namespace Engine

#endif // __ENGINE_POINT_LIGHT_COMPONENT_H__

