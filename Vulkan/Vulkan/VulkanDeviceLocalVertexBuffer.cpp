#include "VulkanDeviceLocalVertexBuffer.h"
#include "VulkanDevice.h"
#include "VulkanGpu.h"
#include <assert.h>


VulkanDeviceLocalVertexBuffer::VulkanDeviceLocalVertexBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, uint64_t size)
	:VulkanVertexBufferBase(vulkanDevicePtr,vulkanGpuPtr,size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{
}

VulkanDeviceLocalVertexBuffer::~VulkanDeviceLocalVertexBuffer()
{
}
