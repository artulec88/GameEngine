#ifndef __AUDIO_I_AUDIO_DEVICE_H__
#define __AUDIO_I_AUDIO_DEVICE_H__

#include "Audio.h"
#include "IAudioData.h"

#include <string>

namespace Audio
{
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
		AUDIO_API virtual ~IAudioDevice(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		AUDIO_API virtual IAudioData* CreateAudioFromFile(const std::string& fileName) = 0;
		AUDIO_API virtual void ReleaseAudio(IAudioData* audioData) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioDevice */

} /* end namespace Audio */

#endif /* __AUDIO_I_AUDIO_DEVICE_H__ */
