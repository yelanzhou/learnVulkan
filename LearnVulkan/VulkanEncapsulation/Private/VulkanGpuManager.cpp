#include "VulkanGpuManager.h"
#include <vector>

VulkanGpuManager::VulkanGpuManager(std::shared_ptr<VulkanInstance> vulkanInstancePtr)
	:m_vulkanInstancePtr(vulkanInstancePtr)
{
	uint32_t physcialDeviceCount = 0;
	vkEnumeratePhysicalDevices(m_vulkanInstancePtr->getHandle(), &physcialDeviceCount, nullptr);
	
	std::vector<VkPhysicalDevice> physcialDevices(physcialDeviceCount);
	vkEnumeratePhysicalDevices(m_vulkanInstancePtr->getHandle(), &physcialDeviceCount, physcialDevices.data());

	for (auto physicalDevice : physcialDevices)
	{
		m_physcialDevices.push_back(std::make_shared<VulkanGpu>(physicalDevice));
	}

}

VulkanGpuManager::~VulkanGpuManager()
{
}

std::shared_ptr<VulkanGpu> VulkanGpuManager::getSuitableGpu()
{
	if (m_physcialDevices.size() != 0)
	{
		return m_physcialDevices.front();
	}
	return nullptr;
}
