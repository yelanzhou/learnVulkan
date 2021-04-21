#pragma once
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class VulkanCommandBuffer;
class VulkanSemaphore;
class VulkanDevice;
class VulkanFence;

class VulkanQueue
{
public:

	VulkanQueue(std::shared_ptr<VulkanDevice> vulkanDevicePtr, uint32_t familyIndex);
	~VulkanQueue();

	uint32_t getFamilyIndex();
	void waitIdle();

	VkQueue getHandle();

	void submit(std::shared_ptr<VulkanCommandBuffer> commandBufferPtr, std::vector<std::shared_ptr<VulkanSemaphore>> waitSemaphorePtr, std::vector<std::shared_ptr<VulkanSemaphore>> signalSemaphorePtr, std::shared_ptr<VulkanFence> fence, VkPipelineStageFlags waitStages = 0);

private:
	std::shared_ptr<VulkanDevice> m_vulkanDevicePtr;
	VkQueue                       m_vulkanQueueHandle;
	uint32_t                      m_familyIndex;

};

