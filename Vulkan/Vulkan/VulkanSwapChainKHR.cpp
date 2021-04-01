#include "VulkanSwapChainKHR.h"
#include "VulkanDevice.h"
#include "VulkanGpu.h"
#include "VulkanSurface.h"
#include "VulkanImageView.h"
#include "VulkanSemaphore.h"
#include "VulkanQueue.h"

#include <assert.h>

VulkanSwapChainKHR::VulkanSwapChainKHR(std::shared_ptr<VulkanGpu> vulkanGpuPtr, std::shared_ptr<VulkanSurface> vulkanSurfacePtr, std::shared_ptr<VulkanDevice> vulkanDevicePtr,uint32_t width,uint32_t height)
	:m_vulkanGpuPtr(vulkanGpuPtr),
	m_vulkanSurfacePtr(vulkanSurfacePtr),
	m_vulkanDevicePtr(vulkanDevicePtr),
	m_width(width),
	m_height(height)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanGpuPtr->getHandle(), m_vulkanSurfacePtr->getHandel(), &m_swapchianDetails.capabilities);

	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanGpuPtr->getHandle(), m_vulkanSurfacePtr->getHandel(), &formatCount, nullptr);
	if (formatCount != 0) {
		m_swapchianDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanGpuPtr->getHandle(), m_vulkanSurfacePtr->getHandel(), &formatCount, m_swapchianDetails.formats.data());
	}

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanGpuPtr->getHandle(), m_vulkanSurfacePtr->getHandel(), &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		m_swapchianDetails.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanGpuPtr->getHandle(), m_vulkanSurfacePtr->getHandel(), &presentModeCount, m_swapchianDetails.presentModes.data());
	}

	createSwapchain();
}

VulkanSwapChainKHR::~VulkanSwapChainKHR()
{

	vkDestroySwapchainKHR(m_vulkanDevicePtr->getHandle(), m_vulkanSwapchainHandle, nullptr);
}

VkExtent2D VulkanSwapChainKHR::getExtent()
{
	return m_swapchainExtent;
}

VkFormat VulkanSwapChainKHR::getFormat()
{
	return m_swapchainFormat;
}

std::vector<std::shared_ptr<VulkanImageView>> VulkanSwapChainKHR::getImageViews()
{
	return m_swapchainImageViewPtrs;
}

uint32_t VulkanSwapChainKHR::getNextImageForDrawing(std::shared_ptr<VulkanSemaphore> semaphorePtr)
{
	uint32_t index = -1;

	vkAcquireNextImageKHR(m_vulkanDevicePtr->getHandle(), m_vulkanSwapchainHandle, UINT64_MAX, semaphorePtr->getHandle(), VK_NULL_HANDLE, &index);
	assert(index != -1);

	return index;
}

void VulkanSwapChainKHR::presentKHR(std::shared_ptr<VulkanSemaphore> waitSemaphorePtr, std::shared_ptr<VulkanQueue> presentQueue, uint32_t imageIndex)
{
	VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	auto semaphore = waitSemaphorePtr->getHandle();
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &semaphore;

	VkSwapchainKHR swapChains[] = { m_vulkanSwapchainHandle };

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(presentQueue->getHandle(), &presentInfo);
}

void VulkanSwapChainKHR::resize(uint32_t width, uint32_t height)
{

	m_width = width;
	m_height = height;

	createSwapchain();
}

void VulkanSwapChainKHR::createSwapchain()
{
	VkExtent2D extent;
	if (m_swapchianDetails.capabilities.currentExtent.height != 0xFFFFFFFF)
	{
		extent.width = m_width;
		extent.height = m_height;

		extent.width = extent.width > m_swapchianDetails.capabilities.minImageExtent.width ? m_swapchianDetails.capabilities.minImageExtent.width : extent.width;
		extent.height = extent.height > m_swapchianDetails.capabilities.minImageExtent.height ? m_swapchianDetails.capabilities.minImageExtent.height : extent.height;

		extent.width = extent.width > m_swapchianDetails.capabilities.maxImageExtent.width ? m_swapchianDetails.capabilities.maxImageExtent.width : extent.width;
		extent.height = extent.height > m_swapchianDetails.capabilities.maxImageExtent.height ? m_swapchianDetails.capabilities.maxImageExtent.height : extent.height;
	}

	uint32_t imageCount = m_swapchianDetails.capabilities.minImageCount + 1 < m_swapchianDetails.capabilities.maxImageCount ? m_swapchianDetails.capabilities.minImageCount + 1 : m_swapchianDetails.capabilities.maxImageCount;

	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (int i = 0; i < m_swapchianDetails.presentModes.size(); ++i)
	{
		if (m_swapchianDetails.presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
	}

	VkSurfaceFormatKHR format = m_swapchianDetails.formats[0];

	for (int i = 0; i < m_swapchianDetails.formats.size(); ++i)
	{

		if (m_swapchianDetails.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && m_swapchianDetails.formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			format = m_swapchianDetails.formats[i];
		}

	}

	m_swapchainExtent = extent;
	m_swapchainFormat = format.format;

	VkSwapchainCreateInfoKHR createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.flags = 0;
	createInfo.surface = m_vulkanSurfacePtr->getHandel();
	createInfo.imageExtent = extent;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = format.format;
	createInfo.imageColorSpace = format.colorSpace;
	createInfo.imageArrayLayers = 1;//always 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	if (m_vulkanDevicePtr->getGraphicQueue()->getFamilyIndex() != m_vulkanDevicePtr->getPresentQueue()->getFamilyIndex())
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;

		uint32_t queues[] = { m_vulkanDevicePtr->getGraphicQueue()->getFamilyIndex(), m_vulkanDevicePtr->getPresentQueue()->getFamilyIndex() };
		createInfo.pQueueFamilyIndices = queues;

	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}
	createInfo.preTransform = m_swapchianDetails.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.oldSwapchain = VK_NULL_HANDLE;
	createInfo.clipped = true;
	createInfo.presentMode = presentMode;

	createInfo.pNext = nullptr;

	if (m_vulkanSwapchainHandle != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(m_vulkanDevicePtr->getHandle(), m_vulkanSwapchainHandle, nullptr);
	}
	assert(vkCreateSwapchainKHR(m_vulkanDevicePtr->getHandle(), &createInfo, nullptr, &m_vulkanSwapchainHandle) == VK_SUCCESS);

	//get swap chain image
	imageCount = 0;

	vkGetSwapchainImagesKHR(m_vulkanDevicePtr->getHandle(), m_vulkanSwapchainHandle, &imageCount, nullptr);
	m_swapchainImage.resize(imageCount);
	vkGetSwapchainImagesKHR(m_vulkanDevicePtr->getHandle(), m_vulkanSwapchainHandle, &imageCount, m_swapchainImage.data());

	//init iamge view;
	m_swapchainImageViewPtrs.clear();
	for (int i = 0; i < imageCount; ++i)
	{
		m_swapchainImageViewPtrs.push_back(std::make_shared<VulkanImageView>(m_vulkanDevicePtr, m_swapchainImage[i], m_swapchainFormat));
	}
}

