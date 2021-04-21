#include "VulkanBufferDescriptor.h"

VulkanBufferDescriptor::VulkanBufferDescriptor(std::shared_ptr<VulkanBuffer> bufferPtr, uint32_t binding, VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t descriptorCount, VkSampler* pImmutableSamplers)
	:VulkanDescriptor(binding,type,stageFlags,descriptorCount,pImmutableSamplers)
	,m_bufferPtr(bufferPtr)
{
}

VulkanBufferDescriptor::~VulkanBufferDescriptor()
{
}

std::shared_ptr<VulkanBuffer> VulkanBufferDescriptor::getBuffer()
{
	return m_bufferPtr;
}
