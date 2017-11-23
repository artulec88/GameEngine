#ifndef __RENDERING_TEXTURE_IDS_H__
#define __RENDERING_TEXTURE_IDS_H__

namespace rendering
{
	namespace texture_ids
	{
		enum TextureId
		{
			INVALID = 0,
			DEFAULT_TEXTURE,
			DEFAULT_NORMAL_MAP,
			DEFAULT_DISPLACEMENT_MAP,
			COUNT, // This value must always stay as the last one in the enum. It only represents the number of available basic texture IDs.
		}; /* end enum TextureId */
	} /* end namespace texture_ids */
} /* end namespace rendering */

#endif /* __RENDERING_TEXTURE_IDS_H__ */