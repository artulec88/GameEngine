#include "stdafx.h"
#include "Effect.h"
#include "Effect_impl.h"

template class MATH_API Math::Effects::Effect<Math::Real>;
template class MATH_API Math::Effects::Effect<Math::Vector2D>;
template class MATH_API Math::Effects::Effect<Math::Vector3D>;
template class MATH_API Math::Effects::SmoothTransitionEffect<Math::Real>;
template class MATH_API Math::Effects::SmoothTransitionEffect<Math::Vector2D>;
template class MATH_API Math::Effects::SmoothTransitionEffect<Math::Vector3D>;
template class MATH_API Math::Effects::BlinkEffect<Math::Real>;
template class MATH_API Math::Effects::BlinkEffect<Math::Vector2D>;
template class MATH_API Math::Effects::BlinkEffect<Math::Vector3D>;