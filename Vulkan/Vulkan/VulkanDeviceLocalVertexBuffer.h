#pragma once
#include "VulkanVertexBufferBase.h"
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class VulkanDevice;
class VulkanGpu;

class VulkanDeviceLocalVertexBuffer :
	public VulkanVertexBufferBase
{
public:
	VulkanDeviceLocalVertexBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, uint64_t size);
	~VulkanDeviceLocalVertexBuffer();

};

