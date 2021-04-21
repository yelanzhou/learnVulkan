#include "VulkanCommandPool.h"
#include "VulkanDevice.h"
#include <iostream>
#include <assert.h>



VulkanCommandPool::VulkanCommandPool(std::shared_ptr<VulkanDevice> vulkanDevicePtr, uint32_t familyIndex)
	:m_vulkanDevicePtr(vulkanDevicePtr)
{
	VkCommandPoolCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex = familyIndex;
	createInfo.flags = 0;
	createInfo.pNext = nullptr;

	VkResult ret = vkCreateCommandPool(m_vulkanDevicePtr->getHandle(), &createInfo, nullptr, &m_vulkanCommandPoolHandle);

	assert(ret == VK_SUCCESS);

}

VulkanCommandPool::~VulkanCommandPool()
{
	vkDestroyCommandPool(m_vulkanDevicePtr->getHandle(), m_vulkanCommandPoolHandle, nullptr);
}

VkCommandPool VulkanCommandPool::getHandle()
{
	// TODO: 在此处插入 return 语句
	return m_vulkanCommandPoolHandle;
}
