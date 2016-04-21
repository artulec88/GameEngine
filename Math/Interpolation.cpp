#include "stdafx.h"
#include "Interpolation.h"
#include "Interpolation_impl.h"

// TODO: For all template classes perform the steps described in https://anteru.net/2008/11/19/318/.
// The same steps have already been performed for the Utility::Container::Array<T> class.
extern template class Math::Interpolation::Interpolator<Math::Real>;
extern template class MATH_API Math::Interpolation::Interpolator<Math::Vector2D>;
extern template class MATH_API Math::Interpolation::Interpolator<Math::Vector3D>;
//template class Math::Interpolation::Interpolator<Math::Quaternion>;
//template class MATH_API Math::Interpolation::Interpolator<Math::Vector4D>;