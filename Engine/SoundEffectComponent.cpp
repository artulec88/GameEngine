#include "stdafx.h"
#include "SoundEffectComponent.h"
#include "CoreEngine.h"

engine::SoundEffectComponent::SoundEffectComponent(const std::string& soundEffectPath) :
	GameComponent(),
	m_soundEffectPath(soundEffectPath)
{
	//CoreEngine::GetCoreEngine()->GetAudioEngine()->LoadSoundEffect3D(m_soundEffectPath);
}


engine::SoundEffectComponent::~SoundEffectComponent()
{
}
