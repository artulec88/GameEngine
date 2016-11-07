#ifndef __RENDERING_TEXTURE_IDS_H__
#define __RENDERING_TEXTURE_IDS_H__

namespace Rendering
{
	namespace TextureIDs
	{
		enum TextureID
		{
			INVALID = 0,
			DEFAULT_TEXTURE,
			DEFAULT_NORMAL_MAP,
			DEFAULT_DISPLACEMENT_MAP,
			COUNT, // This value must always stay as the last one in the enum. It only represents the number of available basic texture IDs.
		}; /* end enum TextureID */
	} /* end namespace TextureIDs */
} /* end namespace Rendering */

#endif /* __RENDERING_TEXTURE_IDS_H__ */