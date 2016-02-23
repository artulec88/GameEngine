#ifndef __ENGINE_DIRECTIONAL_LIGHT_COMPONENT_H__
#define __ENGINE_DIRECTIONAL_LIGHT_COMPONENT_H__

#include "Engine.h"
#include "Rendering\DirectionalLight.h"
#include "GameComponent.h"

namespace Engine
{
	class DirectionalLightComponent : public GameComponent
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		DirectionalLightComponent(Rendering::Lighting::DirectionalLight* directionalLight);
		virtual ~DirectionalLightComponent();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		//virtual Math::Transform& GetTransform() { return GameComponent::GetTransform(); }
		//virtual const Math::Transform& GetTransform() const { return GameComponent::GetTransform(); }
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		Rendering::Lighting::DirectionalLight* m_directionalLight;
	/* ==================== Non-static member variables end ==================== */
	}; // end class DirectionalLightComponent
} // end namespace Engine

#endif // __ENGINE_DIRECTIONAL_LIGHT_COMPONENT_H__

	