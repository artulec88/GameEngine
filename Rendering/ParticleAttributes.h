#ifndef __RENDERING_PARTICLE_ATTRIBUTES_H__
#define __RENDERING_PARTICLE_ATTRIBUTES_H__

#include "Rendering.h"

#include "Math\Vector.h"
#include "Math\Angle.h"

#include <memory>

namespace Rendering
{
	namespace Particles
	{
		namespace Attributes
		{
			enum Attribute
			{
				POSITION = 1 >> 0,
				VELOCITY = 1 >> 1,
				ACCELERATION = 1 >> 2,
				GRAVITY_EFFECT_FACTOR = 1 >> 3,
				LIFE_SPAN = 1 >> 4,
				LIFE_SPAN_LIMIT = 1 >> 5,
				ROTATION = 1 >> 6,
				SCALE = 1 >> 7,
				TEXTURE_OFFSET = 1 >> 8,
				COLOR = 1 >> 9,
				MASS = 1 >> 10,
				ALIVE = 1 >> 11
			}; /* end enum Attribute */

			class AttributesMask
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				AttributesMask(int attributesMask) :
					m_attributesMask(attributesMask)
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				/// <summary>
				/// Performs bitwise AND operation on two possible particle attributes.
				/// </summary>
				/// See: http://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c.
				inline AttributesMask operator&(Attribute attr)
				{
					return AttributesMask(m_attributesMask & static_cast<int>(attr));
				}
				/// <summary>
				/// Performs bitwise OR operation on two possible particle attributes.
				/// </summary>
				/// See: http://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c.
				inline AttributesMask operator|(Attribute attr)
				{
					return AttributesMask(m_attributesMask | static_cast<int>(attr));
				}
				inline AttributesMask& operator&=(Attribute attr)
				{
					m_attributesMask &= static_cast<int>(attr);
					return *this;
				}
				inline AttributesMask& operator|=(Attribute attr)
				{
					m_attributesMask |= static_cast<int>(attr);
					return *this;
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
				int m_attributesMask;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class AttributesMask */
		} /* end namespace Attributes */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_ATTRIBUTES_H__ */