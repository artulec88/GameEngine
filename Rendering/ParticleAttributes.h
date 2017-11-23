#ifndef __RENDERING_PARTICLE_ATTRIBUTES_H__
#define __RENDERING_PARTICLE_ATTRIBUTES_H__

#include "Rendering.h"

namespace rendering
{
	namespace particles
	{
		namespace attributes
		{
			enum Attribute
			{
				POSITION = 1,
				VELOCITY = 1 << 1,
				ACCELERATION = 1 << 2,
				GRAVITY_EFFECT_FACTOR = 1 << 3,
				LIFE_SPAN = 1 << 4,
				ROTATION = 1 << 5,
				SCALE = 1 << 6,
				TEXTURE_OFFSET = 1 << 7,
				COLOR = 1 << 8,
				MASS = 1 << 9,
				ALIVE = 1 << 10,
				ID = 1 << 11,
				//VISIBLE = 1 << 12,
				//ELASTICITY or BOUNCE_FACTOR
			}; /* end enum Attribute */

			class AttributesMask
			{
				/* ==================== Static variables and functions begin ==================== */
				/// <summary> The number of possible particle attributes. </summary>
				static constexpr int COUNT = 12;
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				/// <summary> The constructor of the attributes mask. </summary>
				/// <param name="attributesMask">
				/// The simple integer value whose binary representation indicates which particle attributes
				/// are supposed to be enabled and which not.
				/// </param>
				constexpr explicit AttributesMask(int attributesMask) :
					m_attributesMask(attributesMask)
				{
				}

				/// <summary> Attributes mask copy constructor. </summary>
				/// <param name="attributesMask"> The attributes mask to copy-construct from. </param>
				AttributesMask(const AttributesMask& attributesMask) = default;

				/// <summary> Attributes mask move constructor. </summary>
				/// <param name="attributesMask"> The attributes mask to move-construct from. </param>
				AttributesMask(AttributesMask&& attributesMask) noexcept = default;

				/// <summary> Attributes mask copy assignment operator. </summary>
				/// <param name="attributesMask"> The attributes mask to copy-assign from. </param>
				AttributesMask& operator=(const AttributesMask& attributesMask) = default;

				/// <summary> Attributes mask move assignment operator. </summary>
				/// <param name="attributesMask"> The attributes mask to move-assign from. </param>
				AttributesMask& operator=(AttributesMask&& attributesMask) noexcept = default;
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				/// <summary>
				/// Simple function returning <code>true</code> when specified <paramref name="attribute"/>
				/// is enabled in the attributes mask and <code>false</code> otherwise.
				/// </summary>
				/// <param name="attribute">The attribute to be checked if it is enabled or not. </param>
				/// <returns>
				/// <code>true</code> when specified <paramref name="attribute"/> is enabled in the attributes mask
				/// and <code>false</code> otherwise.
				/// </returns>
				constexpr bool IsAttributeEnabled(Attribute attribute) const
				{
					return ((m_attributesMask & attribute) == attribute);
				}

				/// <summary>
				/// Performs bitwise AND operation on two possible particle attributes.
				/// </summary>
				/// See: http://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c.
				AttributesMask operator&(Attribute attr) const
				{
					return AttributesMask(m_attributesMask & static_cast<int>(attr));
				}
				/// <summary>
				/// Performs bitwise OR operation on two possible particle attributes.
				/// </summary>
				/// See: http://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c.
				AttributesMask operator|(Attribute attr) const
				{
					return AttributesMask(m_attributesMask | static_cast<int>(attr));
				}
				AttributesMask& operator&=(Attribute attr)
				{
					m_attributesMask &= static_cast<int>(attr);
					return *this;
				}
				AttributesMask& operator|=(Attribute attr)
				{
					m_attributesMask |= static_cast<int>(attr);
					return *this;
				}

				friend std::ostream& operator<<(std::ostream& out, const AttributesMask& attributesMask)
				{
					out << attributesMask.m_attributesMask;
					return out;
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
				int m_attributesMask;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class AttributesMask */
		} /* end namespace attributes */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_ATTRIBUTES_H__ */