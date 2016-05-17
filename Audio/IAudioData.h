#ifndef __AUDIO_I_AUDIO_DATA_H__
#define __AUDIO_I_AUDIO_DATA_H__

#include "Audio.h"
#include "SampleInfo.h"

namespace Audio
{
	class IAudioData
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API IAudioData();
		AUDIO_API virtual ~IAudioData(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		virtual size_t GenerateSamples(float* stream, size_t streamLength, size_t pos, const SampleInfo& sampleInfo) = 0;
		virtual size_t GetAudioLength() = 0;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
	/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioData */

} /* end namespace Audio */

#endif /* __AUDIO_I_AUDIO_DATA_H__ */
