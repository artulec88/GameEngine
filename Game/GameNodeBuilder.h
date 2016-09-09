#ifndef __GAME_GAME_NODE_BUILDER_H__
#define __GAME_GAME_NODE_BUILDER_H__

#include "Def.h"
#include "Engine\GameNode.h"
#include "Engine\GameManager.h"

#include "Rendering\Texture.h"

#include "Utility\Builder.h"
#include "Utility\Builder_impl.h"

#include <string>

namespace Game
{

	class CameraBuilder : public Utility::Builder<Engine::GameNode>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		CameraBuilder(Engine::GameManager* gameManager);
		virtual ~CameraBuilder(void);
		CameraBuilder(CameraBuilder& cameraBuilder) = delete;
		CameraBuilder(CameraBuilder&& cameraBuilder) = delete;
		CameraBuilder& operator=(const CameraBuilder& cameraBuilder) = delete;
		CameraBuilder& operator=(CameraBuilder&& cameraBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
		void SetCameraIndex(int cameraIndex);
		void SetEntityToFollow(Engine::GameNode* gameNodeToFollow);
	protected:
		void SetupCameraTransform();
		void SetupCameraParams();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Engine::GameManager* m_gameManager;
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
		Engine::GameNode* m_gameNodeToFollow;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraBuilder */

	class SkyboxBuilder : public Utility::Builder<Engine::GameNode>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		SkyboxBuilder(Engine::GameManager* gameManager);
		virtual ~SkyboxBuilder(void);
		SkyboxBuilder(SkyboxBuilder& skyboxBuilder) = delete;
		SkyboxBuilder(SkyboxBuilder&& skyboxBuilder) = delete;
		SkyboxBuilder& operator=(const SkyboxBuilder& skyboxBuilder) = delete;
		SkyboxBuilder& operator=(SkyboxBuilder&& skyboxBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Engine::GameManager* m_gameManager;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SkyboxBuilder */

} /* end namespace Game */

#endif // __GAME_GAME_NODE_BUILDER_H__
