#include "stdafx.h"
#include "Effect.h"
#include "Effect_impl.h"

#include "Vector.h"

template class MATH_API math::effects::Effect<math::Real>;
template class MATH_API math::effects::Effect<math::Vector2D>;
template class MATH_API math::effects::Effect<math::Vector3D>;
template class MATH_API math::effects::SmoothTransitionEffect<math::Real>;
template class MATH_API math::effects::SmoothTransitionEffect<math::Vector2D>;
template class MATH_API math::effects::SmoothTransitionEffect<math::Vector3D>;
template class MATH_API math::effects::BlinkEffect<math::Real>;
template class MATH_API math::effects::BlinkEffect<math::Vector2D>;
template class MATH_API math::effects::BlinkEffect<math::Vector3D>;