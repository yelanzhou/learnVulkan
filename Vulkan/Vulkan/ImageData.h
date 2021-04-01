#pragma once
#include <string>
class ImageData
{
public:
	ImageData(std::string imageFileName);
	~ImageData();
	
	//bytes
	uint64_t getSize();

	uint32_t getWidth();
	uint32_t getHeight();

	const unsigned char* const getPixlePtr();

private:
	int m_width;
	int m_height;
	int m_channels;
	unsigned char* m_pixelPtr;
};

