#ifndef __ENGINE_BUILDER_H__
#define __ENGINE_BUILDER_H__

#include "Engine.h"
#include "Rendering\GameNode.h"
#include "Rendering\SpotLight.h"
#include <string>

#define BUILD_MESH_RENDERER

namespace Engine
{

	class Builder
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API Builder();
		ENGINE_API virtual ~Builder(void);
	private:
		Builder(Builder& builder) {}
		void operator=(Builder& builder) {}
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1() = 0;
		virtual void BuildPart2() = 0;
#ifdef BUILD_MESH_RENDERER
		virtual void BuildMeshRenderer() = 0;
#endif

		Rendering::GameNode* GetGameNode() { return m_gameNode; }
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Rendering::GameNode* m_gameNode;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Builder */

	class LightBuilder : public Builder
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API LightBuilder();
		ENGINE_API virtual ~LightBuilder(void);
	private:
		LightBuilder(LightBuilder& lightBuilder) {}
		void operator=(LightBuilder& lightBuilder) {}
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
#ifdef BUILD_MESH_RENDERER
		virtual void BuildMeshRenderer();
#endif

		ENGINE_API void SetLightIndex(int lightIndex);
	protected:
		virtual void SetupLightTransform() = 0;
		virtual void SetupLightParams() = 0;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		int m_lightIndex;
		std::string m_lightIndexStr;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class LightBuilder */

	class DirectionalLightBuilder : public LightBuilder
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API DirectionalLightBuilder() : LightBuilder() { };
		ENGINE_API virtual ~DirectionalLightBuilder(void) { };
	private:
		DirectionalLightBuilder(DirectionalLightBuilder& directionalLightBuilder) {}
		void operator=(DirectionalLightBuilder& directionalLightBuilder) {}
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void SetupLightTransform();
		virtual void SetupLightParams();
#ifdef BUILD_MESH_RENDERER
		virtual void BuildMeshRenderer();
#endif
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	/* ==================== Non-static member variables end ==================== */
	}; /* end class DirectionalLightBuilder */

	class PointLightBuilder : public LightBuilder
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API PointLightBuilder();
		ENGINE_API virtual ~PointLightBuilder(void) { };
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void SetupLightTransform();
		virtual void SetupLightParams();
#ifdef BUILD_MESH_RENDERER
		virtual void BuildMeshRenderer();
#endif
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		const Math::Vector3D M_DEFAULT_POINT_LIGHT_POS;
		const Math::Angle M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X;
		const Math::Angle M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y;
		const Math::Angle M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z;
		const Rendering::Color M_DEFAULT_POINT_LIGHT_COLOR;
		const Math::Real M_DEFAULT_POINT_LIGHT_INTENSITY;
		const Rendering::Attenuation M_DEFAULT_POINT_LIGHT_ATTENUATION;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class PointLightBuilder */

	class SpotLightBuilder : public LightBuilder
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API SpotLightBuilder();
		ENGINE_API virtual ~SpotLightBuilder(void) { };
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void SetupLightTransform();
		virtual void SetupLightParams();
#ifdef BUILD_MESH_RENDERER
		virtual void BuildMeshRenderer();
#endif
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		const Math::Vector3D M_DEFAULT_SPOT_LIGHT_POS;
		const Math::Angle M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_X;
		const Math::Angle M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Y;
		const Math::Angle M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Z;
		const Rendering::Color M_DEFAULT_SPOT_LIGHT_COLOR;
		const Math::Real M_DEFAULT_SPOT_LIGHT_INTENSITY;
		const Rendering::Attenuation M_DEFAULT_SPOT_LIGHT_ATTENUATION;
		const Math::Angle M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE;
		const int M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2;
		const Math::Real M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS;
		const Math::Real M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT;
		const Math::Real M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class SpotLightBuilder */

	class CameraBuilder : public Builder
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API CameraBuilder();
		ENGINE_API virtual ~CameraBuilder(void);
	private:
		CameraBuilder(CameraBuilder& cameraBuilder);
		void operator=(CameraBuilder& cameraBuilder) {}
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
#ifdef BUILD_MESH_RENDERER
		virtual void BuildMeshRenderer();
#endif
		ENGINE_API void SetCameraIndex(int cameraIndex);
		ENGINE_API void SetEntityToFollow(Rendering::GameNode* gameNodeToFollow);
	protected:
		void SetupCameraTransform();
		void SetupCameraParams();
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		const Math::Vector3D M_DEFAULT_CAMERA_POS;
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_X;
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_Y;
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_Z;
		const Math::Angle M_DEFAULT_CAMERA_FIELD_OF_VIEW;
		const Math::Real M_DEFAULT_CAMERA_ASPECT_RATIO;
		const Math::Real M_DEFAULT_CAMERA_NEAR_PLANE;
		const Math::Real M_DEFAULT_CAMERA_FAR_PLANE;
		const Math::Real M_DEFAULT_CAMERA_SENSITIVITY;
		const Math::Real M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY;
		const Math::Real M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED;
		const Math::Real M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED;
		const Math::Angle M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE;
		int m_cameraIndex;
		std::string m_cameraIndexStr;
		Rendering::GameNode* m_gameNodeToFollow;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraBuilder */

} /* end namespace Engine */

#endif // __ENGINE_BUILDER_H__