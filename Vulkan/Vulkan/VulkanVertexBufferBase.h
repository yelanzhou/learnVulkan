#pragma once
#include "VulkanBuffer.h"

#include <vulkan/vulkan.h>
#include <memory>

class VulkanDevice;
class VulkanVertexBufferBase:public VulkanBuffer
{
public:
	VulkanVertexBufferBase(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, uint64_t size, VkBufferUsageFlags flags, VkMemoryPropertyFlags memoryPropertyFlags);
	~VulkanVertexBufferBase();

};

