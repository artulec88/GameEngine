#include "stdafx.h"
#include "Interpolation.h"
#include "Interpolation_impl.h"
#include "Vector.h"

// TODO: For all template classes perform the steps described in https://anteru.net/2008/11/19/318/.
// The same steps have already been performed for the Utility::Container::Array<T> class.
extern template class math::interpolation::Interpolator<math::Real>;
extern template class MATH_API math::interpolation::Interpolator<math::Vector2D>;
extern template class MATH_API math::interpolation::Interpolator<math::Vector3D>;
//template class math::interpolation::Interpolator<math::Quaternion>;
//template class MATH_API math::interpolation::Interpolator<math::Vector4D>;