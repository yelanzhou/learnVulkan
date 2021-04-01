#include "VulkanQueue.h"
#include "VulkanDevice.h"
#include "VulkanSemaphore.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFence.h"

#include <assert.h>

VulkanQueue::VulkanQueue(std::shared_ptr<VulkanDevice> vulkanDevicePtr, uint32_t familyIndex)
	:m_vulkanDevicePtr(vulkanDevicePtr),
	m_familyIndex(familyIndex)
{
	vkGetDeviceQueue(m_vulkanDevicePtr->getHandle(), m_familyIndex, 0, &m_vulkanQueueHandle);
}

VulkanQueue::~VulkanQueue()
{
}

uint32_t VulkanQueue::getFamilyIndex()
{
	return m_familyIndex;
}

void VulkanQueue::waitIdle()
{
    auto ret = vkQueueWaitIdle(m_vulkanQueueHandle);
    assert(ret == VK_SUCCESS);
}

VkQueue VulkanQueue::getHandle()
{
    return m_vulkanQueueHandle;
}

void VulkanQueue::submit(std::shared_ptr<VulkanCommandBuffer> commandBufferPtr, std::vector<std::shared_ptr<VulkanSemaphore>> waitSemaphorePtr, std::vector<std::shared_ptr<VulkanSemaphore>> signalSemaphorePtr, std::shared_ptr<VulkanFence> fencePtr, VkPipelineStageFlags waitStages)
{

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    std::vector<VkSemaphore> waitSemaphores;
    for (auto semaphore : waitSemaphorePtr)
    {
        waitSemaphores.push_back(semaphore->getHandle());
    }

    std::vector<VkSemaphore> signalSemaphores;
    for (auto semaphore : signalSemaphorePtr)
    {
        signalSemaphores.push_back(semaphore->getHandle());
    }

    submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
    submitInfo.pWaitSemaphores = waitSemaphores.size() == 0 ? nullptr: waitSemaphores.data();
    submitInfo.pWaitDstStageMask = &waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBufferPtr->getHandle();
    submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
    submitInfo.pSignalSemaphores = signalSemaphores.size() == 0? nullptr : signalSemaphores.data();
    submitInfo.pNext = nullptr;

    VkFence fence = fencePtr == nullptr ? VK_NULL_HANDLE : fencePtr->getHandle();

    auto ret = vkQueueSubmit(m_vulkanQueueHandle, 1, &submitInfo, fence);
    assert(ret == VK_SUCCESS);
}
