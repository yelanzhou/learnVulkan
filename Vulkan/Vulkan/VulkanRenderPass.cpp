#include "VulkanRenderPass.h"
#include "VulkanDevice.h"
#include "VulkanSwapChainKHR.h"

#include <assert.h>

VulkanRenderPass::VulkanRenderPass(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanSwapChainKHR> vulkanSwapchainKHRPtr)
	:m_vulkanDevicePtr(vulkanDevicePtr),
	m_vulkanSwapchainKHR(vulkanSwapchainKHRPtr)
{
	VkAttachmentDescription attachmentDescription;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachmentDescription.flags = 0;
	attachmentDescription.format = m_vulkanSwapchainKHR->getFormat();
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;


	VkAttachmentReference attachmentRef;
	attachmentRef.attachment = 0;
	attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


	VkSubpassDescription subpassDescriptioin;
	subpassDescriptioin.colorAttachmentCount = 1;
	subpassDescriptioin.pColorAttachments = &attachmentRef;
	subpassDescriptioin.flags = 0;
	subpassDescriptioin.inputAttachmentCount = 0;
	subpassDescriptioin.pInputAttachments = nullptr;
	subpassDescriptioin.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescriptioin.pDepthStencilAttachment = nullptr;
	subpassDescriptioin.preserveAttachmentCount = 0;
	subpassDescriptioin.pPreserveAttachments = nullptr;
	subpassDescriptioin.pResolveAttachments = nullptr;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &attachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &attachmentDescription;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpassDescriptioin;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	auto ret = vkCreateRenderPass(m_vulkanDevicePtr->getHandle(), &renderPassInfo, nullptr, &m_vulkanRenderPassHandle);

	assert(ret == VK_SUCCESS);

}

VulkanRenderPass::~VulkanRenderPass()
{
	vkDestroyRenderPass(m_vulkanDevicePtr->getHandle(), m_vulkanRenderPassHandle, nullptr);
}

VkRenderPass VulkanRenderPass::getHandle()
{
	return m_vulkanRenderPassHandle;
}
