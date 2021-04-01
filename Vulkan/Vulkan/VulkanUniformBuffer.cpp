#include "VulkanUniformBuffer.h"
#include "VulkanDevice.h"

VulkanUniformBuffer::VulkanUniformBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, UniformData uniformData)
	:VulkanBuffer(vulkanDevicePtr, vulkanGpuPtr, sizeof(uniformData), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
	void* data = nullptr;
	auto ret = vkMapMemory(m_vulkanDeviePtr->getHandle(), m_vulkanBufferMemory, 0, sizeof(uniformData), 0, &data);
	assert(VK_SUCCESS == ret);
	memcpy(data, &uniformData, (size_t)sizeof(uniformData));
	vkUnmapMemory(m_vulkanDeviePtr->getHandle(), m_vulkanBufferMemory);
}

VulkanUniformBuffer::~VulkanUniformBuffer()
{
}

void VulkanUniformBuffer::updateData(UniformData uniformData)
{
	void* data = nullptr;
	auto ret = vkMapMemory(m_vulkanDeviePtr->getHandle(), m_vulkanBufferMemory, 0, sizeof(uniformData), 0, &data);
	assert(VK_SUCCESS == ret);
	memcpy(data, &uniformData, (size_t)sizeof(uniformData));
	vkUnmapMemory(m_vulkanDeviePtr->getHandle(), m_vulkanBufferMemory);
}
