#include "VectorTestGroup.h"
#include "Math\Vector.h"

using namespace MathTest;

VectorTestGroup::VectorTestGroup(void) :
	TestGroup()
{
}


VectorTestGroup::~VectorTestGroup(void)
{
}

void VectorTestGroup::StartTests()
{
	if (!IsTestGroupEnabled())
	{
		return;
	}
}