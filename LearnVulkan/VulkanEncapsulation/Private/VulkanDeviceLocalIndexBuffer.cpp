#include "VulkanDeviceLocalIndexBuffer.h"

VulkanDeviceLocalIndexBuffer::VulkanDeviceLocalIndexBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, uint64_t size)
	:VulkanBuffer(vulkanDevicePtr, vulkanGpuPtr, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{
}

VulkanDeviceLocalIndexBuffer::~VulkanDeviceLocalIndexBuffer()
{
}
