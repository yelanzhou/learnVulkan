#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipeline.h"
#include "VulkanBuffer.h"
#include "VulkanPipelineLayout.h"
#include "VulkanDescriptorSet.h"

#include <assert.h>

VulkanCommandBuffer::VulkanCommandBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanCommandPool> vulkanCommandPoolPtr)
	:m_vulkanDevicePtr(vulkanDevicePtr),
	m_vulkanCommandPoolPtr(vulkanCommandPoolPtr)
{
	VkCommandBufferAllocateInfo allocateInfo;
	allocateInfo.commandPool = m_vulkanCommandPoolPtr->getHandle();
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.pNext = nullptr;
	allocateInfo.commandBufferCount = 1;

	assert(VK_SUCCESS == vkAllocateCommandBuffers(m_vulkanDevicePtr->getHandle(), &allocateInfo, &m_vulkanCommandBufferHandle));

}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
	vkFreeCommandBuffers(m_vulkanDevicePtr->getHandle(), m_vulkanCommandPoolPtr->getHandle(), 1, &m_vulkanCommandBufferHandle);
}

VkCommandBuffer& VulkanCommandBuffer::getHandle()
{
	return m_vulkanCommandBufferHandle;
}

void VulkanCommandBuffer::beginCommandBuffer(bool isUseOneTime)
{
	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr;
	beginInfo.pInheritanceInfo = nullptr;
	if (isUseOneTime)
	{
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	}

	auto ret = vkBeginCommandBuffer(m_vulkanCommandBufferHandle, &beginInfo);
	assert(ret == VK_SUCCESS);
}

void VulkanCommandBuffer::pushRenderPassBeginCmd(std::shared_ptr<VulkanRenderPass> vulkanRenderPassPtr, std::shared_ptr<VulkanFramebuffer> vulkanFramebufferPtr, VkExtent2D extent)
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

	vkCmdBeginRenderPass(m_vulkanCommandBufferHandle, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}


void VulkanCommandBuffer::pushRenderPassEndCmd()
{
	vkCmdEndRenderPass(m_vulkanCommandBufferHandle);
}

void VulkanCommandBuffer::pushBindPipelineCmd(std::shared_ptr<VulkanPipeline> vulkanPipelinePtr)
{
	vkCmdBindPipeline(m_vulkanCommandBufferHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipelinePtr->getHandle());
}

void VulkanCommandBuffer::pushBindVertexBuffersCmd(uint32_t bindingPoint, uint32_t bindingCount, std::shared_ptr<VulkanBuffer> vulkanBufferPtr, std::vector<uint64_t> offsets)
{
	auto buffer = vulkanBufferPtr->getHandle();
	vkCmdBindVertexBuffers(m_vulkanCommandBufferHandle, bindingPoint, bindingCount, &buffer, offsets.data());
}

void VulkanCommandBuffer::pushBindIndexBufferCmd(std::shared_ptr<VulkanBuffer> vulkanBufferPtr, uint64_t offset, VkIndexType type)
{
	vkCmdBindIndexBuffer(m_vulkanCommandBufferHandle, vulkanBufferPtr->getHandle(), offset, type);
}

void VulkanCommandBuffer::pushBindDescriptorSetsCmd(std::shared_ptr<VulkanPipelineLayout> vulkanPipelineLayoutPtr, uint32_t firstSet, std::vector<std::shared_ptr<VulkanDescriptorSet> > vulkanDesSetPtrs)
{
	std::vector<VkDescriptorSet> sets;
	for (auto vulkanDesSetPtr : vulkanDesSetPtrs)
	{
		sets.push_back(vulkanDesSetPtr->getHandle());
	}

	vkCmdBindDescriptorSets(m_vulkanCommandBufferHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipelineLayoutPtr->getHandle(), firstSet, static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
}

void VulkanCommandBuffer::pushDrawCmd(uint32_t vertexCount)
{
	vkCmdDraw(m_vulkanCommandBufferHandle, vertexCount, 1, 0, 0);
}

void VulkanCommandBuffer::pushDrawIndexedCmd(uint32_t indexCount)
{
	vkCmdDrawIndexed(m_vulkanCommandBufferHandle, indexCount, 1, 0, 0, 0);
}

void VulkanCommandBuffer::endCommandBuffer()
{
	auto ret = vkEndCommandBuffer(m_vulkanCommandBufferHandle);
	assert(ret == VK_SUCCESS);
}
