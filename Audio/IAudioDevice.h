#ifndef __AUDIO_I_AUDIO_DEVICE_H__
#define __AUDIO_I_AUDIO_DEVICE_H__

#include "Audio.h"

#include <string>

namespace audio
{
	class IAudioData;

	/// <summary>
	/// It pretty much serves as a factory pattern for creating the audio data.
	/// </summary>
	class IAudioDevice
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API IAudioDevice();
		AUDIO_API virtual ~IAudioDevice();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		AUDIO_API virtual IAudioData* CreateAudioFromFile(const std::string& fileName) = 0;
		AUDIO_API virtual void ReleaseAudio(IAudioData* audioData) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioDevice */

} /* end namespace audio */

#endif /* __AUDIO_I_AUDIO_DEVICE_H__ */
