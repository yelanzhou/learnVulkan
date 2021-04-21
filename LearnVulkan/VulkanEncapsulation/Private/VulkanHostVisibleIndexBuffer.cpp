#include "VulkanHostVisibleIndexBuffer.h"
#include "VulkanDevice.h"
#include "VulkanGpu.h"

#include <assert.h>

VulkanHostVisibleIndexBuffer::VulkanHostVisibleIndexBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, std::vector<uint16_t> indexData)
	:VulkanBuffer(vulkanDevicePtr,vulkanGpuPtr,sizeof(uint16_t) * indexData.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
	void* data = nullptr;
	auto ret = vkMapMemory(m_vulkanDeviePtr->getHandle(), m_vulkanBufferMemory, 0, sizeof(indexData) * indexData.size(), 0, &data);
	assert(VK_SUCCESS == ret);
	memcpy(data, indexData.data(), (size_t)sizeof(indexData) * indexData.size());
	vkUnmapMemory(m_vulkanDeviePtr->getHandle(), m_vulkanBufferMemory);
}

VulkanHostVisibleIndexBuffer::~VulkanHostVisibleIndexBuffer()
{
}
