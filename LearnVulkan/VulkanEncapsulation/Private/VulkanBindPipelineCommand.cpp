#include "VulkanBindPipelineCommand.h"

#include "VulkanCommandBuffer.h"
#include "VulkanGraphicPipelineBase.h"

VulkanBindPipelineCommand::VulkanBindPipelineCommand(std::shared_ptr<VulkanCommandBuffer> vulkanCommandbufferPtr, std::shared_ptr<VulkanGraphicPipelineBase> vulkanPipelinePtr)
	:VulkanCommandBase(vulkanCommandbufferPtr)
{
	vkCmdBindPipeline(vulkanCommandbufferPtr->getHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipelinePtr->getHandle());
}

VulkanBindPipelineCommand::~VulkanBindPipelineCommand()
{
}
