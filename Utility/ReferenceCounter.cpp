#include "stdafx.h"
#include "ReferenceCounter.h"

using namespace Utility;

ReferenceCounter::ReferenceCounter() :
	refCount(1)
{
}

//int ReferenceCounter::GetReferenceCount() const
//{
//	return refCount;
//}

void ReferenceCounter::AddReference()
{
	++refCount;
}

void ReferenceCounter::RemoveReference()
{
	--refCount;
}

bool ReferenceCounter::IsReferenced() const
{
	return refCount > 0;
}