#ifndef __AUDIO_I_AUDIO_CONTEXT_H__
#define __AUDIO_I_AUDIO_CONTEXT_H__

#include "Audio.h"
#include "AudioObject.h"

namespace Audio
{
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
		virtual void PlayAudio(AudioObject& audioObject) = 0;
		virtual void PauseAudio(AudioObject& audioObject) = 0;
		virtual void StopAudio(AudioObject& audioObject) = 0;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
	/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioContext */

} /* end namespace Audio */

#endif /* __AUDIO_I_AUDIO_CONTEXT_H__ */
