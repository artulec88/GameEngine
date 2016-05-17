#ifndef __AUDIO_SAMPLE_INFO_H__
#define __AUDIO_SAMPLE_INFO_H__

#include "Audio.h"

namespace Audio
{
	class SampleInfo
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API explicit SampleInfo(double volume);
		AUDIO_API ~SampleInfo(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		double m_volume;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class SampleInfo */

} /* end namespace Audio */

#endif /* __AUDIO_SAMPLE_INFO_H__ */
