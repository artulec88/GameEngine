#ifndef __AUDIO_AUDIO_ENGINE_H__
#define __AUDIO_AUDIO_ENGINE_H__

#include "Audio.h"

#include "Math\Math.h"

#include <string>
#include <vector>

namespace Audio
{

	class AudioEngine
	{
		typedef std::vector<FMOD::Sound*> Sounds;
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API explicit AudioEngine(int maxChannelsCount);
		AUDIO_API ~AudioEngine(void);
	private:
		AudioEngine(const AudioEngine& audioEngine);
		void operator=(const AudioEngine& audioEngine);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		AUDIO_API void Update(Math::Real deltaTime);
		AUDIO_API void CreateSound(const std::string& soundFileName);
		AUDIO_API void Play();
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		FMOD::System* m_system;
		int m_maxChannelsCount;
		Sounds m_sounds;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class AudioEngine */

} /* end namespace Audio */

#endif /* __AUDIO_AUDIO_ENGINE_H__ */
