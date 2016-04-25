#include "stdafx.h"
#include "Effect.h"
#include "Effect_impl.h"

template class ENGINE_API Engine::Effects::Effect<Math::Real>;
template class ENGINE_API Engine::Effects::Effect<Math::Vector2D>;
template class ENGINE_API Engine::Effects::Effect<Math::Vector3D>;
template class ENGINE_API Engine::Effects::SmoothTransitionEffect<Math::Real>;
template class ENGINE_API Engine::Effects::SmoothTransitionEffect<Math::Vector2D>;
template class ENGINE_API Engine::Effects::SmoothTransitionEffect<Math::Vector3D>;
template class ENGINE_API Engine::Effects::BlinkEffect<Math::Real>;
template class ENGINE_API Engine::Effects::BlinkEffect<Math::Vector2D>;
template class ENGINE_API Engine::Effects::BlinkEffect<Math::Vector3D>;