#include "VulkanDescriptor.h"

VulkanDescriptor::VulkanDescriptor(uint32_t binding, VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t descriptorCount, VkSampler* pImmutableSamplers)
	:m_binding(binding)
	,m_descriptorType(type)
	,m_stageFlags(stageFlags)
	,m_descriptorCount(descriptorCount)
	,m_pImmutableSamplers(pImmutableSamplers)
{
}

VulkanDescriptor::~VulkanDescriptor()
{
}

uint32_t VulkanDescriptor::getBinding()
{
	return m_binding;
}

VkDescriptorType VulkanDescriptor::getType()
{
	return m_descriptorType;
}

VkShaderStageFlags VulkanDescriptor::getShaderStageFlags()
{
	return m_stageFlags;
}

VkSampler* VulkanDescriptor::getImmutableSamplers()
{
	return m_pImmutableSamplers;
}

uint32_t VulkanDescriptor::getCount()
{
	return m_descriptorCount;
}

