#include "StdAfx.h"
#include "Renderer.h"
#include "Utility\Log.h"

using namespace Rendering;
using namespace Utility;

Renderer::Renderer(void)
{
	stdlog(Debug, LOGPLACE, "Creating Renderer instance");
}


Renderer::~Renderer(void)
{
	stdlog(Debug, LOGPLACE, "Destroying Renderer instance");
}
