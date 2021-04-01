#include "VulkanFramebuffer.h"
#include "VulkanDevice.h"
#include "VulkanImageView.h"
#include "VulkanRenderPass.h"

#include <assert.h>


VulkanFramebuffer::VulkanFramebuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanRenderPass> vulkanRenderPassPtr, std::shared_ptr<VulkanImageView> vulkanImageViewPtr, VkExtent2D extent)
	:m_vulkanDevicePtr(vulkanDevicePtr)
{
	VkImageView attachments[] = { vulkanImageViewPtr->getHandle()};

	VkFramebufferCreateInfo framebufferCreateInfo;
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferCreateInfo.flags = 0;
	framebufferCreateInfo.pNext = nullptr;
	framebufferCreateInfo.height = extent.height;
	framebufferCreateInfo.width = extent.width;
	framebufferCreateInfo.attachmentCount = 1;
	framebufferCreateInfo.pAttachments = attachments;
	framebufferCreateInfo.layers = 1;
	framebufferCreateInfo.renderPass = vulkanRenderPassPtr->getHandle();
	assert(vkCreateFramebuffer(m_vulkanDevicePtr->getHandle(), &framebufferCreateInfo, nullptr, &m_vulkanFramebufferHandle) == VK_SUCCESS);
}

VulkanFramebuffer::~VulkanFramebuffer()
{
	vkDestroyFramebuffer(m_vulkanDevicePtr->getHandle(), m_vulkanFramebufferHandle, nullptr);
}

VkFramebuffer VulkanFramebuffer::getHandle()
{
	return m_vulkanFramebufferHandle;
}
