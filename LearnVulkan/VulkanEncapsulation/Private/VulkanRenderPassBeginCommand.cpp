#include "VulkanRenderPassBeginCommand.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFramebuffer.h"
#include "VulkanRenderPass.h"

#include <assert.h>


VulkanRenderPassBeginCommand::VulkanRenderPassBeginCommand(std::shared_ptr<VulkanCommandBuffer> vulkanCmdBufferPtr, std::shared_ptr<VulkanRenderPass> vulkanRenderPassPtr, std::shared_ptr<VulkanFramebuffer> vulkanFramebufferPtr, VkExtent2D extent)
	:VulkanCommandBase(vulkanCmdBufferPtr)
{
	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.pNext = nullptr;
	renderPassBeginInfo.renderPass = vulkanRenderPassPtr->getHandle();
	renderPassBeginInfo.renderArea.offset = { 0,0 };
	renderPassBeginInfo.renderArea.extent = extent;
	renderPassBeginInfo.framebuffer = vulkanFramebufferPtr->getHandle();

	VkClearValue clearValue = { 0.0f,0.0f,0.0f,1.0f };
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearValue;

	vkCmdBeginRenderPass(vulkanCmdBufferPtr->getHandle(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

}

VulkanRenderPassBeginCommand::~VulkanRenderPassBeginCommand()
{
}

void VulkanRenderPassBeginCommand::end()
{
	vkCmdEndRenderPass(m_vulkanCommandBufferPtr->getHandle());
}
