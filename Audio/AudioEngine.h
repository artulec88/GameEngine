#ifndef __AUDIO_AUDIO_ENGINE_H__
#define __AUDIO_AUDIO_ENGINE_H__

#include "Audio.h"
#include "OpenAL\include\al.h"
#include "OpenAL\include\alc.h"

namespace Audio
{

	class AudioEngine
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API AudioEngine();
		AUDIO_API virtual ~AudioEngine(void);
	private:
		AudioEngine(const AudioEngine& audioEngine);
		void operator=(const AudioEngine& audioEngine);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		AUDIO_API void Play() const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
	/* ==================== Non-static member variables end ==================== */
	}; /* end class AudioEngine */

} /* end namespace Audio */

#endif /* __AUDIO_AUDIO_ENGINE_H__ */
