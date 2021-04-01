#include "VulkanDeviceLocalImage.h"
#include "VulkanBuffer.h"
#include "ImageData.h"
#include "VulkanDevice.h"
#include "VulkanGpu.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"
#include "VulkanQueue.h"
#include "VulkanImageHostVisibleBuffer.h"

#include <assert.h>

VulkanDeviceLocalImage::VulkanDeviceLocalImage(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, std::shared_ptr<ImageData> imageDataPtr)
	:m_vulkanDevicePtr(vulkanDevicePtr)
	,m_imageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
	,m_imageWidth(imageDataPtr->getWidth())
	,m_imageHeight(imageDataPtr->getHeight())
	,m_imageDepth(1)
{
	VkImageCreateInfo imageCreateInfo;
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.extent.height = imageDataPtr->getHeight();
	imageCreateInfo.extent.width = imageDataPtr->getWidth();
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.flags = 0;
	imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.initialLayout = m_imageLayout;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.pNext = nullptr;
	//imageCreateInfo.queueFamilyIndexCount = 
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

	auto ret = vkCreateImage(m_vulkanDevicePtr->getHandle(), &imageCreateInfo, nullptr, &m_vulkanImageHandle);

	assert(ret == VK_SUCCESS);

	VkMemoryRequirements memoryReq;
	vkGetImageMemoryRequirements(m_vulkanDevicePtr->getHandle(), m_vulkanImageHandle, &memoryReq);

	VkMemoryAllocateInfo allocateInfo;
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.memoryTypeIndex = vulkanGpuPtr->findMemoryType(memoryReq.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	allocateInfo.allocationSize = memoryReq.size;
	allocateInfo.pNext = nullptr;

	ret = vkAllocateMemory(vulkanDevicePtr->getHandle(), &allocateInfo, nullptr, &m_vulkanDeviceMemoryHandle);
	assert(ret == VK_SUCCESS);

	vkBindImageMemory(vulkanDevicePtr->getHandle(), m_vulkanImageHandle, m_vulkanDeviceMemoryHandle, 0);
}

VulkanDeviceLocalImage::~VulkanDeviceLocalImage()
{
	vkDestroyImage(m_vulkanDevicePtr->getHandle(), m_vulkanImageHandle, nullptr);
	vkFreeMemory(m_vulkanDevicePtr->getHandle(), m_vulkanDeviceMemoryHandle, nullptr);
}

void VulkanDeviceLocalImage::setImageLayout(VkImageLayout layout)
{
	//auto devicePtr = dstImagePtr->m_vulkanDevicePtr;
	auto graphicQueuePtr = m_vulkanDevicePtr->getGraphicQueue();

	auto commandPoolPtr = std::make_shared<VulkanCommandPool>(m_vulkanDevicePtr, graphicQueuePtr->getFamilyIndex());
	auto commandBufferPtr = std::make_shared<VulkanCommandBuffer>(m_vulkanDevicePtr, commandPoolPtr);

	VkImageMemoryBarrier barrier;
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.pNext = nullptr;
	barrier.oldLayout = m_imageLayout;
	barrier.newLayout = layout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = m_vulkanImageHandle;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (m_imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (m_imageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}

	commandBufferPtr->beginCommandBuffer(true);
	vkCmdPipelineBarrier(
		commandBufferPtr->getHandle(),
		sourceStage,
		destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	commandBufferPtr->endCommandBuffer();
	graphicQueuePtr->submit(commandBufferPtr, {}, {}, nullptr);
	graphicQueuePtr->waitIdle();

	m_imageLayout = layout;
}

VkImage VulkanDeviceLocalImage::getHandle()
{
	return m_vulkanImageHandle;
}

void VulkanDeviceLocalImage::copyBufferToImage(std::shared_ptr<VulkanImageHostVisibleBuffer> srcBufferPtr, std::shared_ptr<VulkanDeviceLocalImage> dstImagePtr)
{
	auto devicePtr = dstImagePtr->m_vulkanDevicePtr;
	auto graphicQueuePtr = devicePtr->getGraphicQueue();

	auto commandPoolPtr = std::make_shared<VulkanCommandPool>(devicePtr, graphicQueuePtr->getFamilyIndex());
	auto commandBufferPtr = std::make_shared<VulkanCommandBuffer>(devicePtr, commandPoolPtr);

	commandBufferPtr->beginCommandBuffer(true);

	VkBufferImageCopy copyInfo;
	copyInfo.bufferOffset = 0;
	copyInfo.bufferRowLength = 0;
	copyInfo.bufferImageHeight = 0;

	copyInfo.imageOffset = { 0,0,0 };
	copyInfo.imageExtent = { dstImagePtr->m_imageWidth,dstImagePtr->m_imageHeight,dstImagePtr->m_imageDepth };
	copyInfo.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyInfo.imageSubresource.baseArrayLayer = 0;
	copyInfo.imageSubresource.layerCount = 1;
	copyInfo.imageSubresource.mipLevel = 0;

	vkCmdCopyBufferToImage(
		commandBufferPtr->getHandle(),
		srcBufferPtr->getHandle(),
		dstImagePtr->getHandle(),
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&copyInfo
	);

	commandBufferPtr->endCommandBuffer();
	graphicQueuePtr->submit(commandBufferPtr, {}, {}, nullptr);
	graphicQueuePtr->waitIdle();
}
