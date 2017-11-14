#include "stdafx.h"
#include "AudioObject.h"
#include "Math\Math.h"
#include "Utility\ILogger.h"

audio::AudioObject::AudioObject(const SampleInfo& sampleInfo, IAudioData* audioData) :
	m_audioPos(0),
	m_audioLength(audioData->GetAudioLength()),
	m_sampleInfo(sampleInfo),
	m_audioData(audioData)
{
}


audio::AudioObject::~AudioObject()
{
}

bool audio::AudioObject::GenerateSamples(float* stream, size_t streamLength)
{
	m_audioPos = m_audioData->GenerateSamples(stream, streamLength, m_audioPos, m_sampleInfo);

	if (m_audioPos == static_cast<size_t>(-1)) // have we reached the end of the file?
	{
		m_audioPos = 0;
		return false;
	}
	return true;
}

void audio::AudioObject::SetPos(double pos)
{
	CHECK_CONDITION_AUDIO(!((pos < 0.0) || (pos > 1.0)), Utility::Logging::ERR,
		"Cannot easily set the audio data position. The normalized position's value (", pos, ") lies outside of range [0; 1]");
	double clampedPos = Math::Clamp(pos, 0.0, 1.0);
	m_audioPos = ConvertPosToAbsolutePos(clampedPos);
}

size_t audio::AudioObject::ConvertPosToAbsolutePos(double pos)
{
	CHECK_CONDITION_AUDIO(!((pos < 0.0) || (pos > 1.0)), Utility::Logging::ERR,
		"Cannot convert the audio data normalized position to absolute position. The normalized position's value (", pos, ") lies outside of range [0; 1]");
	return static_cast<size_t>(pos * m_audioLength);
}