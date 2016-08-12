#ifndef __ENGINE_BUILDER_H__
#define __ENGINE_BUILDER_H__

#include "Engine.h"
#include "GameNode.h"
#include "GameManager.h"

#include "Rendering\SpotLight.h"
#include "Rendering\Texture.h"

#include "Utility\Builder.h"
#include "Utility\Builder_impl.h"

#include <string>

namespace Engine
{
	class LightBuilder : public Utility::Builder<GameNode>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API LightBuilder(GameManager* gameManager);
		ENGINE_API virtual ~LightBuilder(void);

		LightBuilder(const LightBuilder& lightBuilder) = delete;
		LightBuilder(LightBuilder&& lightBuilder) = delete;
		LightBuilder& operator=(const LightBuilder& lightBuilder) = delete;
		LightBuilder& operator=(LightBuilder&& lightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();

		ENGINE_API void SetLightIndex(int lightIndex);
	protected:
		virtual void SetupLightTransform() = 0;
		virtual void SetupLightParams() = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		int m_lightIndex;
		std::string m_lightIndexStr;
		GameManager* m_gameManager;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LightBuilder */

	class DirectionalLightBuilder : public LightBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API DirectionalLightBuilder(GameManager* gameManager) : LightBuilder(gameManager) { };
		ENGINE_API virtual ~DirectionalLightBuilder(void) { };
		DirectionalLightBuilder(DirectionalLightBuilder& directionalLightBuilder) = delete;
		DirectionalLightBuilder(DirectionalLightBuilder&& directionalLightBuilder) = delete;
		DirectionalLightBuilder& operator=(const DirectionalLightBuilder& directionalLightBuilder) = delete;
		DirectionalLightBuilder& operator=(DirectionalLightBuilder&& directionalLightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void SetupLightTransform();
		virtual void SetupLightParams();
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
		ENGINE_API PointLightBuilder(GameManager* gameManager);
		ENGINE_API virtual ~PointLightBuilder(void) { };
		PointLightBuilder(PointLightBuilder& pointLightBuilder) = delete;
		PointLightBuilder(PointLightBuilder&& pointLightBuilder) = delete;
		PointLightBuilder& operator=(const PointLightBuilder& pointLightBuilder) = delete;
		PointLightBuilder& operator=(PointLightBuilder&& pointLightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void SetupLightTransform();
		virtual void SetupLightParams();
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
		ENGINE_API SpotLightBuilder(GameManager* gameManager);
		ENGINE_API virtual ~SpotLightBuilder(void) { };
		SpotLightBuilder(SpotLightBuilder& spotLightBuilder) = delete;
		SpotLightBuilder(SpotLightBuilder&& spotLightBuilder) = delete;
		SpotLightBuilder& operator=(const SpotLightBuilder& spotLightBuilder) = delete;
		SpotLightBuilder& operator=(SpotLightBuilder&& spotLightBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void SetupLightTransform();
		virtual void SetupLightParams();
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

	class CameraBuilder : public Utility::Builder<GameNode>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API CameraBuilder(GameManager* gameManager);
		ENGINE_API virtual ~CameraBuilder(void);
		CameraBuilder(CameraBuilder& cameraBuilder) = delete;
		CameraBuilder(CameraBuilder&& cameraBuilder) = delete;
		CameraBuilder& operator=(const CameraBuilder& cameraBuilder) = delete;
		CameraBuilder& operator=(CameraBuilder&& cameraBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
		ENGINE_API void SetCameraIndex(int cameraIndex);
		ENGINE_API void SetEntityToFollow(GameNode* gameNodeToFollow);
	protected:
		void SetupCameraTransform();
		void SetupCameraParams();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		GameManager* m_gameManager;
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
		GameNode* m_gameNodeToFollow;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraBuilder */

	class SkyboxBuilder : public Utility::Builder<GameNode>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API SkyboxBuilder();
		ENGINE_API virtual ~SkyboxBuilder(void);
		SkyboxBuilder(SkyboxBuilder& skyboxBuilder) = delete;
		SkyboxBuilder(SkyboxBuilder&& skyboxBuilder) = delete;
		SkyboxBuilder& operator=(const SkyboxBuilder& skyboxBuilder) = delete;
		SkyboxBuilder& operator=(SkyboxBuilder&& skyboxBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
	private:
		Rendering::Texture* InitializeCubeMapTexture(const std::string& cubeMapTextureDirectory);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SkyboxBuilder */

} /* end namespace Engine */

#endif // __ENGINE_BUILDER_H__
