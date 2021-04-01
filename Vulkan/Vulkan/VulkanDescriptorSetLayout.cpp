#include "VulkanDescriptorSetLayout.h"
#include "VulkanDevice.h"

#include <assert.h>




VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<std::vector<VkDescriptorSetLayoutBinding>> bindings)
	:m_vulkanDevicePtr(vulkanDevicePtr)
	,m_bindings(bindings)
{

	VkDescriptorSetLayoutCreateInfo layoutCreateInfo;
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutCreateInfo.bindingCount = bindings->size();
	layoutCreateInfo.pNext = nullptr;
	layoutCreateInfo.flags = 0;
	layoutCreateInfo.pBindings = bindings->data();;

	auto ret = vkCreateDescriptorSetLayout(m_vulkanDevicePtr->getHandle(), &layoutCreateInfo, nullptr, &m_vulkanSetLayoutHandle);
	assert(ret == VK_SUCCESS);

}

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
{
	vkDestroyDescriptorSetLayout(m_vulkanDevicePtr->getHandle(), m_vulkanSetLayoutHandle, nullptr);
}

VkDescriptorSetLayout VulkanDescriptorSetLayout::getHandle()
{
	return m_vulkanSetLayoutHandle;
}

std::shared_ptr<std::vector<VkDescriptorSetLayoutBinding>> VulkanDescriptorSetLayout::getLayoutBindings()
{
	return m_bindings;
}
