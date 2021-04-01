#include "VulkanSemaphore.h"
#include "VulkanDevice.h"

#include <assert.h>

VulkanSemaphore::VulkanSemaphore(std::shared_ptr<VulkanDevice> vulkanDevicePtr)
	:m_vulkanDevicePtr(vulkanDevicePtr)
{
	VkSemaphoreCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pNext = nullptr;

	auto ret = vkCreateSemaphore(m_vulkanDevicePtr->getHandle(), &createInfo, nullptr, &m_vulkanSemaphoreHandle);
	assert(ret == VK_SUCCESS);
}

VulkanSemaphore::~VulkanSemaphore()
{
	vkDestroySemaphore(m_vulkanDevicePtr->getHandle(), m_vulkanSemaphoreHandle, nullptr);
}

VkSemaphore VulkanSemaphore::getHandle()
{
	return m_vulkanSemaphoreHandle;
}
