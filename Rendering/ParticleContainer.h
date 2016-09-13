#ifndef __RENDERING_PARTICLE_CONTAINER_H__
#define __RENDERING_PARTICLE_CONTAINER_H__

#include "Rendering.h"

#include "Math\Vector.h"
#include "Math\Angle.h"

#include <memory>

namespace Rendering
{
	/// <summary>
	/// Particles container.
	/// </summary>
	class ParticleContainer
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API explicit ParticleContainer(size_t maxCount);
		RENDERING_API ~ParticleContainer();

		/// <summary> Particle container copy constructor. </summary>
		RENDERING_API ParticleContainer(const ParticleContainer& particleContainer) = delete;
		/// <summary> Particle container move constructor. </summary>
		RENDERING_API ParticleContainer(ParticleContainer&& particleContainer) = delete;
		/// <summary> Particle container copy assignment operator. </summary>
		RENDERING_API ParticleContainer& operator=(const ParticleContainer& particleContainer) = delete;
		/// <summary> Particle container move assignment operator. </summary>
		RENDERING_API ParticleContainer& operator=(ParticleContainer&& particleContainer) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API inline void Reset() { m_countAlive = 0; }
		RENDERING_API inline size_t GetCount() const { return m_count; }
		RENDERING_API inline size_t GetAliveCount() const { return m_countAlive; }
		RENDERING_API inline bool IsAlive(size_t i) const { return m_lifeSpans[i] < m_lifeSpanLimits[i]; }
		RENDERING_API inline const Math::Vector3D& GetPosition(size_t i) const { return m_positions[i]; }
		RENDERING_API inline const Math::Vector3D& GetVelocity(size_t i) const { return m_velocities[i]; }
		RENDERING_API inline const Math::Vector3D& GetAcceleration(size_t i) const { return m_accelerations[i]; }
		RENDERING_API inline Math::Real GetGravityEffectFactor(size_t i) const { return m_gravityEffectFactors[i]; }
		RENDERING_API inline Math::Real GetLifeSpanLimit(size_t i) const { return m_lifeSpanLimits[i]; }
		RENDERING_API inline Math::Real GetLifeSpan(size_t i) const { return m_lifeSpans[i]; }
		RENDERING_API inline const Math::Angle& GetRotation(size_t i) const { return m_rotations[i]; }
		RENDERING_API inline Math::Real GetScale(size_t i) const { return m_scales[i]; }
		RENDERING_API inline void SetPosition(size_t i, const Math::Vector3D& newPosition) { m_positions[i] = newPosition; }
		RENDERING_API inline void SetVelocity(size_t i, const Math::Vector3D& newVelocity) { m_velocities[i] = newVelocity; }
		RENDERING_API inline void SetAcceleration(size_t i, const Math::Vector3D& newAcceleration) { m_accelerations[i] = newAcceleration; }
		RENDERING_API inline void SetGravityEffectFactor(size_t i, Math::Real newGravityEffectFactor) { m_gravityEffectFactors[i] = newGravityEffectFactor; }
		RENDERING_API inline void SetLifeSpanLimit(size_t i, Math::Real newLifeSpanLimit) { m_lifeSpanLimits[i] = newLifeSpanLimit; }
		RENDERING_API inline void SetLifeSpan(size_t i, Math::Real newLifeSpan) { m_lifeSpans[i] = newLifeSpan; }
		RENDERING_API inline void SetRotation(size_t i, const Math::Angle& newRotation) { m_rotations[i] = newRotation; }
		RENDERING_API inline void SetScale(size_t i, Math::Real newScale) { m_scales[i] = newScale; }
		RENDERING_API inline void IncreasePosition(size_t i, const Math::Vector3D& positionIncrease) { m_positions[i] += positionIncrease; }
		RENDERING_API inline void IncreaseVelocity(size_t i, const Math::Vector3D& velocityIncrease) { m_velocities[i] += velocityIncrease; }
		RENDERING_API inline void IncreaseAcceleration(size_t i, const Math::Vector3D& accelerationIncrease) { m_accelerations[i] += accelerationIncrease; }
		RENDERING_API inline void IncreaseGravityEffectFactor(size_t i, Math::Real gravityEffectFactorIncrease) { m_gravityEffectFactors[i] += gravityEffectFactorIncrease; }
		RENDERING_API inline void IncreaseLifeSpanLimit(size_t i, Math::Real lifeSpanLimitIncrease) { m_lifeSpanLimits[i] += lifeSpanLimitIncrease; }
		RENDERING_API inline void IncreaseLifeSpan(size_t i, Math::Real lifeSpanIncrease) { m_lifeSpans[i] += lifeSpanIncrease; }
		RENDERING_API inline void IncreaseRotation(size_t i, const Math::Angle& rotationIncrease) { m_rotations[i] += rotationIncrease; }
		RENDERING_API inline void IncreaseScale(size_t i, Math::Real scaleIncrease) { m_scales[i] += scaleIncrease; }
		RENDERING_API void Kill(size_t id);
		RENDERING_API void Revive(size_t id);
		RENDERING_API void SwapData(size_t a, size_t b);
		RENDERING_API Math::Real CalculateLifeStageFactor(size_t i) const { return m_lifeSpans[i] / m_lifeSpanLimits[i]; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		std::unique_ptr<Math::Vector3D[]> m_positions;
		std::unique_ptr<Math::Vector3D[]> m_velocities;
		std::unique_ptr<Math::Vector3D[]> m_accelerations;
		std::unique_ptr<Math::Real[]> m_gravityEffectFactors;
		std::unique_ptr<Math::Real[]> m_lifeSpanLimits;
		std::unique_ptr<Math::Real[]> m_lifeSpans;
		std::unique_ptr<Math::Angle[]> m_rotations;
		std::unique_ptr<Math::Real[]>  m_scales;
		std::unique_ptr<Math::Vector2D[]> m_textureOffsets;

		size_t m_count;
		size_t m_countAlive;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticleContainer */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_CONTAINER_H__ */