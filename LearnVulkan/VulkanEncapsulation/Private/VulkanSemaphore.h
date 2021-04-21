#pragma once
#include <vulkan/vulkan.h>
#include <memory>
class VulkanDevice;

class VulkanSemaphore
{
public:
	VulkanSemaphore(std::shared_ptr<VulkanDevice> vulkanDevicePtr);
	~VulkanSemaphore();
	VkSemaphore getHandle();

private:
	std::shared_ptr<VulkanDevice> m_vulkanDevicePtr;
	VkSemaphore m_vulkanSemaphoreHandle;
};

