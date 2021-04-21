#pragma once
#include "VulkanDescriptor.h"

#include <memory>
class VulkanBuffer;

class VulkanBufferDescriptor :public VulkanDescriptor
{
public:

	VulkanBufferDescriptor(std::shared_ptr<VulkanBuffer> bufferPtr, uint32_t binding, VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t descriptorCount = 1, VkSampler* pImmutableSamplers = nullptr);
	~VulkanBufferDescriptor();

	std::shared_ptr<VulkanBuffer> getBuffer();

private:
	std::shared_ptr<VulkanBuffer> m_bufferPtr;
};

