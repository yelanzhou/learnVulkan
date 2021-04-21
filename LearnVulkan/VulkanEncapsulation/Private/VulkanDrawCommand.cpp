#include "VulkanDrawCommand.h"
#include "VulkanCommandBuffer.h"

VulkanDrawCommand::VulkanDrawCommand(std::shared_ptr<VulkanCommandBuffer> vulkanCommandBufferPtr, uint32_t vertexCount)
	:VulkanCommandBase(vulkanCommandBufferPtr)
{
	vkCmdDraw(m_vulkanCommandBufferPtr->getHandle(), vertexCount, 1, 0,0);
}

VulkanDrawCommand::~VulkanDrawCommand()
{
}
