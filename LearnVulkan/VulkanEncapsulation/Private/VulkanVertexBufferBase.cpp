#include "VulkanVertexBufferBase.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanQueue.h"

#include <assert.h>

VulkanVertexBufferBase::VulkanVertexBufferBase(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, uint64_t size, VkBufferUsageFlags flags, VkMemoryPropertyFlags memoryPropertyFlags)
	:VulkanBuffer(vulkanDevicePtr, vulkanGpuPtr,size,flags, memoryPropertyFlags)
{
}

VulkanVertexBufferBase::~VulkanVertexBufferBase()
{
}

