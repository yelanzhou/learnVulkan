#pragma once
#include "VulkanBuffer.h"
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class VulkanDevice;
class VulkanGpu;

class VulkanHostVisibleIndexBuffer :
	public VulkanBuffer
{
public:
	VulkanHostVisibleIndexBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, std::vector<uint16_t> indexData);
	~VulkanHostVisibleIndexBuffer();
};

