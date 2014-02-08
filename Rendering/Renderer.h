#pragma once

#include "Rendering.h"
//#include "Utility\Singleton.h"

namespace Rendering
{
// TODO: Consider creating Singleton template class from which Renderer would inherit
class RENDERING_API Renderer
{
public:
	Renderer(void);
	~Renderer(void);
}; /* end class Renderer */

} /* end namespace Rendering */
