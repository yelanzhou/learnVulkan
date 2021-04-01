#pragma once
#include <vulkan/vulkan.h>
#include <memory>
class VulkanCommandBuffer;

class VulkanCommandBase
{
public:
	VulkanCommandBase(std::shared_ptr<VulkanCommandBuffer> vulkanCommandBufferPtr);
	~VulkanCommandBase();

protected:
	std::shared_ptr<VulkanCommandBuffer>  m_vulkanCommandBufferPtr;
};

