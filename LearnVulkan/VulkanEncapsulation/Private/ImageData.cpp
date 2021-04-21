#include "ImageData.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>
#include <assert.h>


ImageData::ImageData(std::string imageFileName)
{
	m_pixelPtr = stbi_load(imageFileName.c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);
	assert(m_pixelPtr != nullptr);
}

ImageData::~ImageData()
{
}

uint64_t ImageData::getSize()
{
	return static_cast<uint64_t>(m_height * m_width * 4);
}

uint32_t ImageData::getWidth()
{
	return static_cast<uint32_t>(m_width);
}

uint32_t ImageData::getHeight()
{
	return static_cast<uint32_t>(m_height);
}

const unsigned char* const ImageData::getPixlePtr()
{
	return m_pixelPtr;
}
