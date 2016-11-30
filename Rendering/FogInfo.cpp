#include "stdafx.h"
#include "FogInfo.h"

Rendering::FogEffect::FogInfo::FogInfo(Color fogColor /* = Color(ColorIDs::GAINSBORO) */, Math::Real fogStartDistance /* = 8.0f */, Math::Real fogEndDistance /* = 50.0f */, Math::Real fogDensityFactor /* = 0.2f */,
	Math::Real fogGradient /* = 0.005f */, FogFallOffType fogFallOffType /* = EXPONENTIAL */, FogCalculationType fogCalculationType /* = RANGE_BASED */, bool isEnabled /* = true */) noexcept :
	m_color(fogColor),
	m_startDistance(fogStartDistance),
	m_endDistance(fogEndDistance),
	m_densityFactor(fogDensityFactor),
	m_gradient(fogGradient),
	m_fallOffType(fogFallOffType),
	m_calculationType(fogCalculationType),
	m_isEnabled(isEnabled)
{
}

bool Rendering::FogEffect::FogInfo::operator<(const FogInfo& fogInfo) const
{
	if (m_fallOffType != fogInfo.m_fallOffType)
	{
		return m_fallOffType < fogInfo.m_fallOffType;
	}
	else
	{
		return m_calculationType < fogInfo.m_calculationType;
	}
	//return (fallOffType & calculationType) < (fog.fallOffType & fog.calculationType);
}