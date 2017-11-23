#include "stdafx.h"
#include "Image.h"
#include "stb_image.h"

#include "Utility/ILogger.h"

rendering::Image::Image(const std::string& fileName, int desiredComponentsCount /* = 0 */) :
	m_width(0),
	m_height(0),
	m_componentsCount(0)
{
	CheckErrorCode(__FUNCTION__, "Creating image object");

	INFO_LOG_RENDERING("Loading image data from file \"", fileName, "\".");
	CHECK_CONDITION_RENDERING(stbi_failure_reason() == NULL, Utility::Logging::ERR, "Image data loading failure reason: \"", stbi_failure_reason(), "\".");
	m_data = stbi_load(fileName.c_str(), &m_width, &m_height, &m_componentsCount, desiredComponentsCount);
	ERROR_LOG_RENDERING("Failure reason after = \"", (stbi_failure_reason() == NULL ? "null" : stbi_failure_reason()), "\".");
	CHECK_CONDITION_EXIT_RENDERING(data != NULL, Utility::Logging::ERR, "Unable to load image from the file \"", fileName, "\"");
	DEBUG_LOG_RENDERING("Loading texture from file \"", fileName, "\" finished successfully");
}

rendering::Image::~Image()
{
	if (m_data != nullptr)
	{
		stbi_image_free(m_data);
		m_data = nullptr;
	}
}

rendering::Image::Image(Image&& image) noexcept:
	m_data(image.m_data),
	m_width(image.m_width),
	m_height(image.m_height),
	m_componentsCount(image.m_componentsCount)
{
	image.m_data = nullptr;
}

unsigned char rendering::Image::GetPixelAt(int row, int column) const
{
	CHECK_CONDITION_RETURN_RENDERING(m_data != nullptr, 0, Utility::Logging::ERR,
		"Cannot determine the pixel value on (", x, ", ", z, ") position. Image data is NULL.");
	CHECK_CONDITION_RETURN_RENDERING(row >= 0 && row < m_height && column >= 0 && column < heightMapWidth, REAL_ZERO,
		Utility::Logging::ERR, "Cannot determine the pixel value on (", x, ", ", z, ") position. It is out of range.");
	const int pixelIndex = row * m_width + column;
	CHECK_CONDITION_RENDERING(pixelIndex >= 0 && pixelIndex < m_width * m_height, Utility::Logging::ERR,
		"The pixel index calculation is incorrect. Calculated index (", pixelIndex, ") is out of range [0; ", m_width * m_height, ")");
	//DEBUG_LOG_RENDERING("Heightmap index for [", x, ", ", z, "] = ", heightMapIndex);
	return m_data[pixelIndex];
}