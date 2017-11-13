#ifndef __RENDERING_IMAGE_H__
#define __RENDERING_IMAGE_H__

#include "Rendering.h"

#include <string>
#include <map>

namespace Rendering
{

	/// <summary>
	/// The representation of the image.
	/// </summary>
	class Image
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API explicit Image(const std::string& fileName, int desiredComponentsCount = 0);
		RENDERING_API ~Image(void);
		Image(const Image& image) = delete;
		/// <summary>
		/// Image move constructor that initializes its member variables using the specified image <paramref name="image"/>.
		/// </summary>
		/// <param name="image"> The r-value reference to the image that will be used to move construct new image. </param>
		RENDERING_API Image(Image&& image);
		Image& operator=(const Image& image) = delete;
		Image& operator=(Image&& image) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API unsigned char GetPixelAt(int row, int column) const;
		RENDERING_API unsigned char* GetData() const { return m_data; }
		RENDERING_API inline int GetWidth() const { return m_width; }
		RENDERING_API inline int GetHeight() const { return m_height; }
		RENDERING_API inline int GetComponentsCount() const { return m_componentsCount; }
	private:
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// Image data. It points to the pixel data, or <code>NULL</code> in case allocation failure or if the specified file is corrupted or invalid.
		/// The pixel data consists of <code>m_height</code> scanlines of <code>m_width</code> pixels, with each pixel consisting of <code>m_componentsCount</code> interleaved 8-bit components.
		/// The first pixel pointed to is top-left-most in the image. There is no padding between image scanlines or between pixels, regardless of format.
		/// An output image with <code>m_componentsCount</code> components has the following components interleaved in this order in each pixel:
		/// N=#comp     components
		///       1           grey
		///       2           grey, alpha
		///       3           red, green, blue
		///       4           red, green, blue, alpha
		/// </summary>
		unsigned char* m_data;

		/// <summary> The image width in pixels. </summary>
		int m_width;

		/// <summary> The image height in pixels. </summary>
		int m_height;

		/// <summary> The number of 8-bit components per pixel in the image. </summary>
		int m_componentsCount;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Image */

} /* end namespace Rendering */

#endif /* __RENDERING_IMAGE_H__ */