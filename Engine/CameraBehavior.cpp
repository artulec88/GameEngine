#include "stdafx.h"
#include "CameraBehavior.h"

#include "Utility/ILogger.h"

/* ==================== CameraBehavior class implementation begin ==================== */
engine::CameraBehavior::CameraBehavior()
{
}


engine::CameraBehavior::~CameraBehavior()
{
}
/* ==================== CameraBehavior class implementation end ==================== */

/* ==================== StaticCameraBehavior class implementation begin ==================== */
engine::StaticCameraBehavior::StaticCameraBehavior() :
	CameraBehavior()
{
}


engine::StaticCameraBehavior::~StaticCameraBehavior()
{
}

void engine::StaticCameraBehavior::Handle(Rendering::BaseCamera* camera, Actions::Action action)
{
	WARNING_LOG_ENGINE("Handling action ", action, " by the static camera.");
}

void engine::StaticCameraBehavior::Handle(Rendering::BaseCamera* camera, States::State state)
{
	WARNING_LOG_ENGINE("Handling state ", state, " by the static camera.");
}

void engine::StaticCameraBehavior::Handle(Rendering::BaseCamera* camera, Ranges::Range range, math::Real value)
{
	WARNING_LOG_ENGINE("Handling range ", range, " with value ", value, " by the static camera.");
}
/* ==================== StaticCameraBehavior class implementation end ==================== */

/* ==================== RotationCameraBehavior class implementation begin ==================== */
engine::RotationCameraBehavior::RotationCameraBehavior() :
	CameraBehavior()
{
}


engine::RotationCameraBehavior::~RotationCameraBehavior()
{
}

void engine::RotationCameraBehavior::Handle(Rendering::BaseCamera* camera, Actions::Action action)
{
	WARNING_LOG_ENGINE("Handling action ", action, " by the rotation camera.");
}

void engine::RotationCameraBehavior::Handle(Rendering::BaseCamera* camera, States::State state)
{
	WARNING_LOG_ENGINE("Handling state ", state, " by the rotation camera.");
}

void engine::RotationCameraBehavior::Handle(Rendering::BaseCamera* camera, Ranges::Range range, math::Real value)
{
	WARNING_LOG_ENGINE("Handling range ", range, " with value ", value, " by the rotation camera.");
}
/* ==================== RotationCameraBehavior class implementation end ==================== */

/* ==================== MovementCameraBehavior class implementation begin ==================== */
engine::MovementCameraBehavior::MovementCameraBehavior() :
	CameraBehavior()
{
}


engine::MovementCameraBehavior::~MovementCameraBehavior()
{
}

void engine::MovementCameraBehavior::Handle(Rendering::BaseCamera* camera, Actions::Action action)
{
	WARNING_LOG_ENGINE("Handling action ", action, " by the movement camera.");
}

void engine::MovementCameraBehavior::Handle(Rendering::BaseCamera* camera, States::State state)
{
	WARNING_LOG_ENGINE("Handling state ", state, " by the movement camera.");
}

void engine::MovementCameraBehavior::Handle(Rendering::BaseCamera* camera, Ranges::Range range, math::Real value)
{
	WARNING_LOG_ENGINE("Handling range ", range, " with value ", value, " by the movement camera.");
}
/* ==================== MovementCameraBehavior class implementation end ==================== */