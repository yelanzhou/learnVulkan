#include "VulkanImageHostVisibleBuffer.h"

#include "VulkanDevice.h"
#include "VulkanGpu.h"
#include "ImageData.h"

#include <assert.h>

VulkanImageHostVisibleBuffer::VulkanImageHostVisibleBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, std::shared_ptr<ImageData> imageDataPtr)
	:VulkanBuffer(vulkanDevicePtr,vulkanGpuPtr,imageDataPtr->getSize(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{

	void* data = nullptr;
	auto ret = vkMapMemory(vulkanDevicePtr->getHandle(), m_vulkanBufferMemory, 0, imageDataPtr->getSize(), 0, &data);
	assert(VK_SUCCESS == ret);
	memcpy(data, imageDataPtr->getPixlePtr(), imageDataPtr->getSize());
	vkUnmapMemory(vulkanDevicePtr->getHandle(), m_vulkanBufferMemory);

}

VulkanImageHostVisibleBuffer::~VulkanImageHostVisibleBuffer()
{
}
