#include "stdafx.h"
#include "Effect.h"
#include "Effect_impl.h"

template class RENDERING_API Rendering::Effects::Effect<Math::Real>;
template class RENDERING_API Rendering::Effects::Effect<Math::Vector2D>;
template class RENDERING_API Rendering::Effects::Effect<Math::Vector3D>;
template class RENDERING_API Rendering::Effects::SmoothTransitionEffect<Math::Real>;
template class RENDERING_API Rendering::Effects::SmoothTransitionEffect<Math::Vector2D>;
template class RENDERING_API Rendering::Effects::SmoothTransitionEffect<Math::Vector3D>;
template class RENDERING_API Rendering::Effects::BlinkEffect<Math::Real>;
template class RENDERING_API Rendering::Effects::BlinkEffect<Math::Vector2D>;
template class RENDERING_API Rendering::Effects::BlinkEffect<Math::Vector3D>;