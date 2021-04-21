#pragma once
#include <vulkan/vulkan.h>
#include <vector>
class VulkanGpu
{
public:
	VulkanGpu(VkPhysicalDevice physicalDevice);
	~VulkanGpu();
	VkPhysicalDevice getHandle();
	const std::vector< VkQueueFamilyProperties>& getQueueFamilyProperties();

	VkPhysicalDeviceMemoryProperties getMemoryProperties();

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags  propertyFlags);
	

private:
	VkPhysicalDevice                            m_physicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties                  m_physicalDeviceProperties;
	std::vector<VkQueueFamilyProperties>        m_queueFamilyProperties;
	VkPhysicalDeviceMemoryProperties            m_physicalDeviceMemoryProperties;

};

