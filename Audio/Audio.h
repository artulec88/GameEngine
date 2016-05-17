#ifndef __AUDIO_AUDIO_H__
#define __AUDIO_AUDIO_H__

#ifdef AUDIO_EXPORTS
#define AUDIO_API __declspec(dllexport)
#else
#define AUDIO_API __declspec(dllimport)
#endif

namespace Audio
{
} /* end namespace Audio */

#endif /* __AUDIO_AUDIO_H__ */