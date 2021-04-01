#pragma once
#include <vulkan/vulkan.h>
#include <memory>
class VulkanDevice;
class VulkanFence
{
public:
	VulkanFence(std::shared_ptr<VulkanDevice> vuklanDevicePtr);
	~VulkanFence();
	void wait(uint64_t timeOut = UINT64_MAX);
	void reset();

	VkFence getHandle();

private:
	VkFence m_vulkanFenceHandle;
	std::shared_ptr<VulkanDevice> m_vulkanDevicePtr;
};

