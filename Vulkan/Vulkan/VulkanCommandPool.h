#pragma once

#include <vulkan/vulkan.h>
#include <memory>
class VulkanDevice;

class VulkanCommandPool
{
public:
	VulkanCommandPool(std::shared_ptr<VulkanDevice> vulkanDevicePtr, uint32_t familyIndex);
	
	~VulkanCommandPool();

	VkCommandPool getHandle();

private:

	VkCommandPool     m_vulkanCommandPoolHandle;
	std::shared_ptr<VulkanDevice> m_vulkanDevicePtr;

};

