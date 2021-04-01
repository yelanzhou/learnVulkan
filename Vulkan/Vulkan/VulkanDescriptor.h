#pragma once
#include <vulkan/vulkan.h>

class VulkanDescriptor
{
public:
    VulkanDescriptor(uint32_t binding, VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t descriptorCount = 1, VkSampler* pImmutableSamplers = nullptr);
    ~VulkanDescriptor();
    uint32_t getBinding();
    VkDescriptorType getType();
    VkShaderStageFlags getShaderStageFlags();
    VkSampler* getImmutableSamplers();
    uint32_t getCount();

private:
    uint32_t              m_binding;
    VkDescriptorType      m_descriptorType;
    uint32_t              m_descriptorCount;
    VkShaderStageFlags    m_stageFlags;
    VkSampler*            m_pImmutableSamplers;

};

