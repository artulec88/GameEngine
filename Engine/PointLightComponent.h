#ifndef __ENGINE_POINT_LIGHT_COMPONENT_H__
#define __ENGINE_POINT_LIGHT_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"

#include "Rendering\PointLight.h"

namespace engine
{
	class PointLightComponent : public GameComponent
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit PointLightComponent(rendering::lighting::PointLight* pointLight);
		virtual ~PointLightComponent();
		PointLightComponent(const PointLightComponent& pointLightComponent) = delete;
		PointLightComponent(PointLightComponent&& pointLightComponent) noexcept;
		PointLightComponent& operator=(const PointLightComponent& pointLightComponent) = delete;
		PointLightComponent& operator=(PointLightComponent&& pointLightComponent) noexcept;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
		//virtual math::Transform& GetTransform() { return GameComponent::GetTransform(); }
		//virtual const math::Transform& GetTransform() const { return GameComponent::GetTransform(); }
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		rendering::lighting::PointLight* m_pointLight;
	/* ==================== Non-static member variables end ==================== */
	}; // end class PointLightComponent
} // end namespace engine

#endif // __ENGINE_POINT_LIGHT_COMPONENT_H__

