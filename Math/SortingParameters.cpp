#include "stdafx.h"
#include "SortingParameters.h"
#include "Utility\ILogger.h"

using namespace Math;

SortingParametersChain::SortingParametersChain() :
	m_sortingParametersChain(NULL),
	m_sortingKey(COMPONENT_X),
	m_sortingDirection(ASCENDING)
{
}

SortingParametersChain::SortingParametersChain(SortingKey sortingKey, SortingDirection sortingDirection) :
	m_sortingParametersChain(NULL),
	m_sortingKey(sortingKey),
	m_sortingDirection(sortingDirection)
{
}

SortingParametersChain::~SortingParametersChain(void)
{
	SAFE_DELETE(m_sortingParametersChain);
}

void SortingParametersChain::AddChainLink(SortingParametersChain* sortingParametersChain)
{
	if (m_sortingParametersChain != NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Replacing the already set-up sorting parameters chain");
		SAFE_DELETE(m_sortingParametersChain);
	}
	m_sortingParametersChain = sortingParametersChain;
}