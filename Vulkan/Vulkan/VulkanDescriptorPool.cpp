#include "VulkanDescriptorPool.h"
#include "VulkanDevice.h"
#include "VulkanDescriptorSetLayout.h"

#include <assert.h>
#include <vector>

VulkanDescriptorPool::VulkanDescriptorPool(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanDescriptorSetLayout> vulkanDescriptorSetLayoutPtr,uint64_t descriptorCount)
	:m_vulkanDevicePtr(vulkanDevicePtr)
{
	auto layoutBindings = vulkanDescriptorSetLayoutPtr->getLayoutBindings();
	auto count = layoutBindings->size();

	std::vector<VkDescriptorPoolSize> poolSizes(count);
	
	for (int i = 0; i < count; ++i)
	{
		poolSizes[i].type = layoutBindings->at(i).descriptorType;
		poolSizes[i].descriptorCount = descriptorCount;
	}


	VkDescriptorPoolCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.maxSets = descriptorCount;
	createInfo.poolSizeCount = count;
	createInfo.pPoolSizes = poolSizes.data();
	createInfo.pNext = nullptr;
	createInfo.flags = 0;

	auto ret = vkCreateDescriptorPool(m_vulkanDevicePtr->getHandle(), &createInfo, nullptr, &m_vulkanDescriptorPoolHandle);
	assert(ret == VK_SUCCESS);
}

VulkanDescriptorPool::~VulkanDescriptorPool()
{
	vkDestroyDescriptorPool(m_vulkanDevicePtr->getHandle(), m_vulkanDescriptorPoolHandle, nullptr);
}

VkDescriptorPool VulkanDescriptorPool::getHandle()
{
	return m_vulkanDescriptorPoolHandle;
}
