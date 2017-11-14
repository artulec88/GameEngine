#ifndef __AUDIO_I_AUDIO_CONTEXT_H__
#define __AUDIO_I_AUDIO_CONTEXT_H__

#include "Audio.h"
#include "AudioObject.h"

namespace audio
{
	/// <summary>
	/// The interface for basic audio functionality, e.g. playing, pausing and stopping audio.
	/// </summary>
	class IAudioContext
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API IAudioContext();
		AUDIO_API virtual ~IAudioContext(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		AUDIO_API virtual void PlayAudio(AudioObject& audioObject) = 0;
		AUDIO_API virtual void PauseAudio(AudioObject& audioObject) = 0;
		AUDIO_API virtual void StopAudio(AudioObject& audioObject) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioContext */

} /* end namespace Audio */

#endif /* __AUDIO_I_AUDIO_CONTEXT_H__ */
