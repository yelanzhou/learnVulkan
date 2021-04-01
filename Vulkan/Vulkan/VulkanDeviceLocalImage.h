#pragma once
#include <vulkan/vulkan.h>
#include <memory>

class VulkanDevice;
class VulkanGpu;
class ImageData;
class VulkanImageHostVisibleBuffer;
class VulkanCommanBuffer;

class VulkanDeviceLocalImage
{
public:
	VulkanDeviceLocalImage(std::shared_ptr<VulkanDevice> vulkanDevicePtr,std::shared_ptr<VulkanGpu> vulkanGpuptr,std::shared_ptr<ImageData> imageDataPtr);
	~VulkanDeviceLocalImage();

	void setImageLayout(VkImageLayout layout);

	VkImage getHandle();

	static void copyBufferToImage(std::shared_ptr<VulkanImageHostVisibleBuffer> srcBufferPtr, std::shared_ptr<VulkanDeviceLocalImage> dstImagePtr);
private:
	std::shared_ptr<VulkanDevice>    m_vulkanDevicePtr;
	VkImage                          m_vulkanImageHandle;
	VkDeviceMemory                   m_vulkanDeviceMemoryHandle;
	VkImageLayout                    m_imageLayout;
	uint32_t                         m_imageHeight;
	uint32_t                         m_imageWidth;
	uint32_t                         m_imageDepth;
};

