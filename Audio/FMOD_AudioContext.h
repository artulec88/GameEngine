#ifndef __AUDIO_FMOD_AUDIO_CONTEXT_H__
#define __AUDIO_FMOD_AUDIO_CONTEXT_H__

#include "Audio.h"
#include "IAudioContext.h"
#include "fmod.hpp"

#include <vector>

namespace Audio
{
	class FmodAudioContext : public IAudioContext
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API FmodAudioContext();
		AUDIO_API virtual ~FmodAudioContext(void);
	private:
		FmodAudioContext(FmodAudioContext& fmodAudioContext) { (void)fmodAudioContext; }
		void operator=(const FmodAudioContext& fmodAudioContext) { (void)fmodAudioContext; }
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void PlayAudio(AudioObject& audioObject);
		virtual void PauseAudio(AudioObject& audioObject);
		virtual void StopAudio(AudioObject& audioObject);
		void GenerateSamples(UINT8* stream, int streamLength);
	private:
		bool RemoveAudio(AudioObject& audioObject);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		FMOD::System* m_system;
		std::vector<float> m_stream;
		std::vector<AudioObject*> m_playingAudioObjects;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class FmodAudioContext */

} /* end namespace Audio */

#endif /* __AUDIO_FMOD_AUDIO_CONTEXT_H__ */
