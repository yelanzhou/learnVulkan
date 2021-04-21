#include "VulkanFence.h"
#include "VulkanDevice.h"
#include <assert.h>

VulkanFence::VulkanFence(std::shared_ptr<VulkanDevice> vuklanDevicePtr)
	:m_vulkanDevicePtr(vuklanDevicePtr)
{
	VkFenceCreateInfo fenceCreateInfo;
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = nullptr;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	assert(vkCreateFence(m_vulkanDevicePtr->getHandle(), &fenceCreateInfo, nullptr, &m_vulkanFenceHandle) == VK_SUCCESS);
}

VulkanFence::~VulkanFence()
{
	vkDestroyFence(m_vulkanDevicePtr->getHandle(), m_vulkanFenceHandle, nullptr);
}

void VulkanFence::wait(uint64_t timeOut)
{
	auto ret = vkWaitForFences(m_vulkanDevicePtr->getHandle(), 1, &m_vulkanFenceHandle, true, timeOut);
	assert(ret == VK_SUCCESS);
}

void VulkanFence::reset()
{
	vkResetFences(m_vulkanDevicePtr->getHandle(), 1, &m_vulkanFenceHandle);
}

VkFence VulkanFence::getHandle()
{
	return m_vulkanFenceHandle;
}
