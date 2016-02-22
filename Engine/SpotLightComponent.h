#ifndef __ENGINE_SPOT_LIGHT_COMPONENT_H__
#define __ENGINE_SPOT_LIGHT_COMPONENT_H__

#include "Engine.h"
#include "Rendering\SpotLight.h"

namespace Engine
{
	class SpotLightComponent : public Rendering::GameComponent
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		SpotLightComponent(Rendering::Lighting::SpotLight* spotLight);
		virtual ~SpotLightComponent();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		//virtual Math::Transform& GetTransform() { return Rendering::GameComponent::GetTransform(); }
		//virtual const Math::Transform& GetTransform() const { return Rendering::GameComponent::GetTransform(); }
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		Rendering::Lighting::SpotLight* m_spotLight;
	/* ==================== Non-static member variables end ==================== */
	}; // end class SpotLightComponent
} // end namespace Engine

#endif // __ENGINE_SPOT_LIGHT_COMPONENT_H__

