#include "stdafx.h"
#include "FogInfo.h"

rendering::fog_effect::FogInfo::FogInfo(Color fogColor /* = Color(ColorIDs::GAINSBORO) */, math::Real fogStartDistance /* = 8.0f */, math::Real fogEndDistance /* = 50.0f */, math::Real fogDensityFactor /* = 0.2f */,
	math::Real fogGradient /* = 0.005f */, FogFallOffType fogFallOffType /* = EXPONENTIAL */, FogCalculationType fogCalculationType /* = RANGE_BASED */, bool isEnabled /* = true */) noexcept :
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

bool rendering::fog_effect::FogInfo::operator<(const FogInfo& fogInfo) const
{
	return m_fallOffType != fogInfo.m_fallOffType ? m_fallOffType < fogInfo.m_fallOffType : m_calculationType < fogInfo.m_calculationType;
	//return (fallOffType & calculationType) < (fog.fallOffType & fog.calculationType);
}