#include "stdafx.h"
#include "ReferenceCounter.h"

using namespace Utility;

ReferenceCounter::ReferenceCounter() :
	m_refCount(1)
{
}

void ReferenceCounter::AddReference()
{
	++m_refCount;
}

void ReferenceCounter::RemoveReference()
{
	--m_refCount;
}

bool ReferenceCounter::IsReferenced() const
{
	return m_refCount > 0;
}