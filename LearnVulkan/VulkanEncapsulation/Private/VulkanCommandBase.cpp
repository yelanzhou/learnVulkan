#include "VulkanCommandBase.h"

VulkanCommandBase::VulkanCommandBase(std::shared_ptr<VulkanCommandBuffer> vulkanCommandBufferPtr)
	:m_vulkanCommandBufferPtr(vulkanCommandBufferPtr)
{
}

VulkanCommandBase::~VulkanCommandBase()
{
}
