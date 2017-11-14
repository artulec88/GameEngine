#ifndef __AUDIO_FMOD_AUDIO_CONTEXT_H__
#define __AUDIO_FMOD_AUDIO_CONTEXT_H__

#include "Audio.h"
#include "IAudioContext.h"
#include "fmod.hpp"

#include <vector>

namespace audio
{
	namespace Fmod
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
			AUDIO_API virtual void PlayAudio(AudioObject& audioObject);
			AUDIO_API virtual void PauseAudio(AudioObject& audioObject);
			AUDIO_API virtual void StopAudio(AudioObject& audioObject);
			AUDIO_API void GenerateSamples(int* stream, int streamLength);
		private:
			/// <summary>
			/// Removes the audio object from the vector of audio objects currently being played.
			/// </summary>
			bool RemoveAudio(AudioObject& audioObject);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			FMOD::System* m_system;
			std::vector<float> m_stream;
			/// <summary>
			/// A vector of audio objects currently being played.
			/// </summary>
			std::vector<AudioObject*> m_playingAudioObjects;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class FmodAudioContext */
	
	} /* end namespace Fmod */

} /* end namespace Audio */

#endif /* __AUDIO_FMOD_AUDIO_CONTEXT_H__ */
