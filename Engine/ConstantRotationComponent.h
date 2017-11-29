#ifndef __ENGINE_CONSTANT_ROTATION_COMPONENT_H__
#define __ENGINE_CONSTANT_ROTATION_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "GameNode.h"
#include "IUpdateable.h"

#include "Math/Angle.h"

namespace engine
{

	class ConstantRotationComponent : public GameComponent, public IUpdateable
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ConstantRotationComponent(const math::Vector3D& rotationAxis, const math::Angle& rotationSpeedAngle);
		ENGINE_API virtual ~ConstantRotationComponent();
		ConstantRotationComponent(const ConstantRotationComponent& constantRotationComponent) = delete;
		ConstantRotationComponent(ConstantRotationComponent&& constantRotationComponent) = default;
		ConstantRotationComponent& operator=(const ConstantRotationComponent& constantRotationComponent) = delete;
		ConstantRotationComponent& operator=(ConstantRotationComponent&& constantRotationComponent) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void Update(math::Real elapsedTime) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		math::Vector3D m_rotationAxis;
		math::Angle m_rotationSpeedAngle;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ConstantRotationComponent */

} /* end namespace engine */

#endif /* __ENGINE_CONSTANT_ROTATION_COMPONENT_H__ */