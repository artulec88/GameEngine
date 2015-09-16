#include "StdAfx.h"
#include "Resource.h"

using namespace Rendering;

Resource::Resource(const std::string& filename) :
	m_isLoaded(false),
	m_guid(0),
	m_filename(filename)
{
	// TODO: Calculate hash value based on the filename
}


Resource::~Resource(void)
{
}
