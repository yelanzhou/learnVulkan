#include "VulkanHostVisibleVertexBuffer.h"
#include "VulkanDevice.h"
#include "VulkanGpu.h"
#include <assert.h>

VulkanHostVisibleVertexBuffer::VulkanHostVisibleVertexBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, std::vector<VertexData> vertexDatas)
	:VulkanVertexBufferBase(vulkanDevicePtr,vulkanGpuPtr, sizeof(VertexData)* vertexDatas.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
	void* data = nullptr;
	auto ret = vkMapMemory(m_vulkanDeviePtr->getHandle(), m_vulkanBufferMemory, 0, sizeof(VertexData) * vertexDatas.size(), 0, &data);
	assert(VK_SUCCESS == ret);
	memcpy(data, vertexDatas.data(), (size_t)sizeof(VertexData) * vertexDatas.size());
	vkUnmapMemory(m_vulkanDeviePtr->getHandle(), m_vulkanBufferMemory);
}

VulkanHostVisibleVertexBuffer::~VulkanHostVisibleVertexBuffer()
{

}
