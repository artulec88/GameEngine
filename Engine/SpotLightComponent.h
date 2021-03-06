#ifndef __ENGINE_SPOT_LIGHT_COMPONENT_H__
#define __ENGINE_SPOT_LIGHT_COMPONENT_H__

#include "Engine.h"
#include "Rendering/SpotLight.h"
#include "GameComponent.h"

namespace engine
{
	class SpotLightComponent : public GameComponent
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit SpotLightComponent(rendering::lighting::SpotLight* spotLight);
		virtual ~SpotLightComponent();
		SpotLightComponent(const SpotLightComponent& spotLightComponent) = delete;
		SpotLightComponent(SpotLightComponent&& spotLightComponent) noexcept;
		SpotLightComponent& operator=(const SpotLightComponent& spotLightComponent) = delete;
		SpotLightComponent& operator=(SpotLightComponent&& spotLightComponent) noexcept;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
		//virtual math::Transform& GetTransform() { return GameComponent::GetTransform(); }
		//virtual const math::Transform& GetTransform() const { return GameComponent::GetTransform(); }
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		rendering::lighting::SpotLight* m_spotLight;
	/* ==================== Non-static member variables end ==================== */
	}; // end class SpotLightComponent
} // end namespace engine

#endif // __ENGINE_SPOT_LIGHT_COMPONENT_H__

