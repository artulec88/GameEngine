#include "StdAfx.h"
#include "Attenuation.h"

using namespace Rendering;

Attenuation::Attenuation(Math::Real constant /* = 0.0 */, Math::Real linear /* = 0.0 */, Math::Real exponent /* = 1.0 */) :
	constant(constant),
	linear(linear),
	exponent(exponent)
{
}


Attenuation::~Attenuation(void)
{
}
