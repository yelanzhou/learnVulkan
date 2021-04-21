#include "VulkanImageView.h"
#include "VulkanDevice.h"
#include <assert.h>

VulkanImageView::VulkanImageView(std::shared_ptr<VulkanDevice> vulkanDevicePtr, VkImage imageHanle, VkFormat format)
	:m_vulkanDevicePtr(vulkanDevicePtr)
{
	VkImageViewCreateInfo viewInfo;
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.pNext = nullptr;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.image = imageHanle;
	viewInfo.format = format;

	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;

	viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	auto ret = vkCreateImageView(m_vulkanDevicePtr->getHandle(), &viewInfo, nullptr, &m_vulkanImageViewHandle);
	assert(ret == VK_SUCCESS);
}

VulkanImageView::~VulkanImageView()
{
 	vkDestroyImageView(m_vulkanDevicePtr->getHandle(), m_vulkanImageViewHandle, nullptr);
}

VkImageView VulkanImageView::getHandle()
{
	return m_vulkanImageViewHandle;
}
