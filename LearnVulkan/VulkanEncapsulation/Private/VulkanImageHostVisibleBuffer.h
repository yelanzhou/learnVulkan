#pragma once

#include "VulkanBuffer.h"

#include <vulkan/vulkan.h>
#include <memory>

class ImageData;
class VulkanImageHostVisibleBuffer :
	public VulkanBuffer
{
public:
	VulkanImageHostVisibleBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, std::shared_ptr<ImageData> imageDataPtr);
	~VulkanImageHostVisibleBuffer();


};

