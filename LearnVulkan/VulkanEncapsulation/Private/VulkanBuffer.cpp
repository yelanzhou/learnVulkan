#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanQueue.h"
#include "VulkanGpu.h"

#include <assert.h>

VulkanBuffer::VulkanBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, uint64_t size, VkBufferUsageFlags flags, VkMemoryPropertyFlags memoryPropertyFlags)
	:m_vulkanDeviePtr(vulkanDevicePtr),
	m_size(size)
{
	VkBufferCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pNext = nullptr;
	createInfo.pQueueFamilyIndices = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	createInfo.usage = flags;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.size = m_size;

	auto ret = vkCreateBuffer(m_vulkanDeviePtr->getHandle(), &createInfo, nullptr, &m_vulkanBufferHandle);
	assert(VK_SUCCESS == ret);

	VkMemoryRequirements requre;

	vkGetBufferMemoryRequirements(m_vulkanDeviePtr->getHandle(), m_vulkanBufferHandle, &requre);

	auto memoryProperties = vulkanGpuPtr->getMemoryProperties();
	uint32_t index = -1;
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
	{

		if ((requre.memoryTypeBits & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags))
		{
			index = i;
			break;
		}
	}

	assert(-1 != index);

	VkMemoryAllocateInfo allocateInfo;
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.allocationSize = requre.size;
	allocateInfo.pNext = nullptr;
	allocateInfo.memoryTypeIndex = index;

	ret = vkAllocateMemory(m_vulkanDeviePtr->getHandle(), &allocateInfo, nullptr, &m_vulkanBufferMemory);
	assert(ret == VK_SUCCESS);

	ret = vkBindBufferMemory(m_vulkanDeviePtr->getHandle(), m_vulkanBufferHandle, m_vulkanBufferMemory, 0);
	assert(ret == VK_SUCCESS);
}



VulkanBuffer::~VulkanBuffer()
{
	vkDestroyBuffer(m_vulkanDeviePtr->getHandle(), m_vulkanBufferHandle, nullptr);
	vkFreeMemory(m_vulkanDeviePtr->getHandle(), m_vulkanBufferMemory, nullptr);
}

VkBuffer VulkanBuffer::getHandle()
{
	return m_vulkanBufferHandle;
}

uint64_t VulkanBuffer::getSize()
{
	return m_size;
}

VkDeviceMemory VulkanBuffer::getMemoryHandle()
{
	return m_vulkanBufferMemory;
}

void VulkanBuffer::Copy(std::shared_ptr<VulkanBuffer> srcBufferPtr, std::shared_ptr<VulkanBuffer> dstBuffer, uint64_t size)
{
	auto vulkanDevicePtr = srcBufferPtr->m_vulkanDeviePtr;
	auto graphicQueue = vulkanDevicePtr->getGraphicQueue();

	auto commandPoolPtr = std::make_shared<VulkanCommandPool>(vulkanDevicePtr, graphicQueue->getFamilyIndex());
	auto commandBufferPtr = std::make_shared<VulkanCommandBuffer>(vulkanDevicePtr, commandPoolPtr);

	commandBufferPtr->beginCommandBuffer(true);
	VkBufferCopy bufferCopy = {};
	bufferCopy.size = size;
	vkCmdCopyBuffer(commandBufferPtr->getHandle(), srcBufferPtr->getHandle(), dstBuffer->getHandle(), 1, &bufferCopy);

	commandBufferPtr->endCommandBuffer();

	std::vector<std::shared_ptr<VulkanSemaphore>> waitSemaphorePtr;
	std::vector<std::shared_ptr<VulkanSemaphore>> signalSemaphorePtr;
	std::shared_ptr<VulkanFence> fencePtr;

	graphicQueue->submit(commandBufferPtr, waitSemaphorePtr, signalSemaphorePtr, fencePtr);

	graphicQueue->waitIdle();


}
