#include "VulkanGpu.h"

#include <assert.h>
VulkanGpu::VulkanGpu(VkPhysicalDevice physicalDevice)
	:m_physicalDevice(physicalDevice)
{
	vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
	m_queueFamilyProperties.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount,m_queueFamilyProperties.data());

	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice,&m_physicalDeviceMemoryProperties);
}

VulkanGpu::~VulkanGpu()
{
}

VkPhysicalDevice VulkanGpu::getHandle()
{
	return m_physicalDevice;
}

const std::vector<VkQueueFamilyProperties>& VulkanGpu::getQueueFamilyProperties()
{
	return m_queueFamilyProperties;
}

VkPhysicalDeviceMemoryProperties VulkanGpu::getMemoryProperties()
{
	return m_physicalDeviceMemoryProperties;
}

uint32_t VulkanGpu::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags  propertyFlags)
{
	for (int i = 0; i < m_physicalDeviceMemoryProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & 1 << i) && (m_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)
		{
			return i;
		}
	}

	assert(false);
}
