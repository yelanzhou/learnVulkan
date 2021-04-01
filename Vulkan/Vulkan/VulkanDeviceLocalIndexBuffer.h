#pragma once
#include "VulkanBuffer.h"
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class VulkanDevice;
class VulkanGpu;

class VulkanDeviceLocalIndexBuffer
	:public VulkanBuffer
{
public:
	VulkanDeviceLocalIndexBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, uint64_t size);
	~VulkanDeviceLocalIndexBuffer();
};

