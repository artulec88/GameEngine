#include "stdafx.h"
#include "CameraBehavior.h"

#include "Utility\ILogger.h"

/* ==================== CameraBehavior class implementation begin ==================== */
Engine::CameraBehavior::CameraBehavior()
{
}


Engine::CameraBehavior::~CameraBehavior()
{
}
/* ==================== CameraBehavior class implementation end ==================== */

/* ==================== StaticCameraBehavior class implementation begin ==================== */
Engine::StaticCameraBehavior::StaticCameraBehavior() :
	CameraBehavior()
{
}


Engine::StaticCameraBehavior::~StaticCameraBehavior()
{
}

void Engine::StaticCameraBehavior::Handle(Rendering::BaseCamera* camera, Actions::Action action)
{
	WARNING_LOG_ENGINE("Handling action ", action, " by the static camera.");
}

void Engine::StaticCameraBehavior::Handle(Rendering::BaseCamera* camera, States::State state)
{
	WARNING_LOG_ENGINE("Handling state ", state, " by the static camera.");
}

void Engine::StaticCameraBehavior::Handle(Rendering::BaseCamera* camera, Ranges::Range range, Math::Real value)
{
	WARNING_LOG_ENGINE("Handling range ", range, " with value ", value, " by the static camera.");
}
/* ==================== StaticCameraBehavior class implementation end ==================== */

/* ==================== RotationCameraBehavior class implementation begin ==================== */
Engine::RotationCameraBehavior::RotationCameraBehavior() :
	CameraBehavior()
{
}


Engine::RotationCameraBehavior::~RotationCameraBehavior()
{
}

void Engine::RotationCameraBehavior::Handle(Rendering::BaseCamera* camera, Actions::Action action)
{
	WARNING_LOG_ENGINE("Handling action ", action, " by the rotation camera.");
}

void Engine::RotationCameraBehavior::Handle(Rendering::BaseCamera* camera, States::State state)
{
	WARNING_LOG_ENGINE("Handling state ", state, " by the rotation camera.");
}

void Engine::RotationCameraBehavior::Handle(Rendering::BaseCamera* camera, Ranges::Range range, Math::Real value)
{
	WARNING_LOG_ENGINE("Handling range ", range, " with value ", value, " by the rotation camera.");
}
/* ==================== RotationCameraBehavior class implementation end ==================== */

/* ==================== MovementCameraBehavior class implementation begin ==================== */
Engine::MovementCameraBehavior::MovementCameraBehavior() :
	CameraBehavior()
{
}


Engine::MovementCameraBehavior::~MovementCameraBehavior()
{
}

void Engine::MovementCameraBehavior::Handle(Rendering::BaseCamera* camera, Actions::Action action)
{
	WARNING_LOG_ENGINE("Handling action ", action, " by the movement camera.");
}

void Engine::MovementCameraBehavior::Handle(Rendering::BaseCamera* camera, States::State state)
{
	WARNING_LOG_ENGINE("Handling state ", state, " by the movement camera.");
}

void Engine::MovementCameraBehavior::Handle(Rendering::BaseCamera* camera, Ranges::Range range, Math::Real value)
{
	WARNING_LOG_ENGINE("Handling range ", range, " with value ", value, " by the movement camera.");
}
/* ==================== MovementCameraBehavior class implementation end ==================== */