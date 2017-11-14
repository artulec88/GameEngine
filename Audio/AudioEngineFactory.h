#ifndef __AUDIO_AUDIO_ENGINE_FACTORY_H__
#define __AUDIO_AUDIO_ENGINE_FACTORY_H__

#include "Audio.h"

#include <memory>
#include <string>

namespace audio
{
	class IAudioEngine;

	namespace AudioEngineTypes
	{
		enum AudioEngineType
		{
			FMOD = 0,
			IRR_KLANG,
			COUNT // the number of audio engine types supported
		}; /* end enum AudioEngineType */
	}; /* end namespace AudioEngineTypes */

	class AudioEngineFactory
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API AudioEngineFactory(const std::string& audioDirectory);
		AUDIO_API ~AudioEngineFactory(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		AUDIO_API std::unique_ptr<IAudioEngine> CreateAudioEngine(AudioEngineTypes::AudioEngineType audioEngineType = AudioEngineTypes::IRR_KLANG) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const std::string& m_audioDirectory;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioEngine */

} /* end namespace Audio */

#endif /* __AUDIO_AUDIO_ENGINE_FACTORY_H__ */
