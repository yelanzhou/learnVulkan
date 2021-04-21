#include "VulkanPipelineLayout.h"
#include "VulkanDevice.h"
#include "VulkanDescriptorSetLayout.h"

#include <assert.h>

VulkanPipelineLayout::VulkanPipelineLayout(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanDescriptorSetLayout> vulkanDesLayoutPtr)
	:m_vulkanDevicePtr(vulkanDevicePtr)
{


	VkPipelineLayoutCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pNext = nullptr;
	createInfo.pPushConstantRanges = nullptr;
	createInfo.pushConstantRangeCount = 0;

	auto layout = vulkanDesLayoutPtr->getHandle();
	createInfo.setLayoutCount = 1;
	createInfo.pSetLayouts = &layout;


	auto ret = vkCreatePipelineLayout(m_vulkanDevicePtr->getHandle(), &createInfo, nullptr, &m_pipelineLayoutHandle);
	assert(ret == VK_SUCCESS);
}

VulkanPipelineLayout::~VulkanPipelineLayout()
{
	vkDestroyPipelineLayout(m_vulkanDevicePtr->getHandle(), m_pipelineLayoutHandle, nullptr);
}

VkPipelineLayout VulkanPipelineLayout::getHandle()
{
	return m_pipelineLayoutHandle;
}
