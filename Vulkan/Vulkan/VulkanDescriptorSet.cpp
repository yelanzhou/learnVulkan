#include "VulkanDescriptorSet.h"
#include "VulkanDevice.h"
#include "VulkanUniformBuffer.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"
#include <assert.h>


/*********************************
同时分配多个能提升多高的效率
*****************************************/
VulkanDescriptorSet::VulkanDescriptorSet(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanDescriptorPool> vulkanDescriptorPoolPtr, std::shared_ptr<VulkanDescriptorSetLayout> vulkanDesSetLayoutPtr)
	:m_vulkanDevicePtr(vulkanDevicePtr)
{
	auto setLayout = vulkanDesSetLayoutPtr->getHandle();

	VkDescriptorSetAllocateInfo allocateInfo;
	allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.descriptorPool = vulkanDescriptorPoolPtr->getHandle();
	allocateInfo.descriptorSetCount = 1;
	allocateInfo.pSetLayouts = &setLayout;

	auto ret = vkAllocateDescriptorSets(m_vulkanDevicePtr->getHandle(), &allocateInfo, &m_vulkanDescriptorSetHandle);
}

VulkanDescriptorSet::~VulkanDescriptorSet()
{
}

VkDescriptorSet VulkanDescriptorSet::getHandle()
{
	return m_vulkanDescriptorSetHandle;
}

void VulkanDescriptorSet::update(std::shared_ptr<VulkanUniformBuffer> vulkanUniformBufferPtr)
{
	VkDescriptorBufferInfo bufferInfo;
	bufferInfo.buffer = vulkanUniformBufferPtr->getHandle();
	bufferInfo.offset = 0;
	bufferInfo.range = vulkanUniformBufferPtr->getSize();


	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = m_vulkanDescriptorSetHandle;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo  = &bufferInfo;

	vkUpdateDescriptorSets(m_vulkanDevicePtr->getHandle(), 1, &descriptorWrite, 0, nullptr);

}
