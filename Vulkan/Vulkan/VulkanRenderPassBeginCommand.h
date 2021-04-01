#pragma once
#include <vulkan/vulkan.h>
#include <memory>
#include "VulkanCommandBase.h"
class VulkanFramebuffer;
class VulkanRenderPass;
class VulkanCommandBuffer;

class VulkanRenderPassBeginCommand :
	public VulkanCommandBase
{
public:
	VulkanRenderPassBeginCommand(std::shared_ptr<VulkanCommandBuffer> vulkanCmdBufferPtr, std::shared_ptr<VulkanRenderPass> vulkanRenderPassPtr, std::shared_ptr<VulkanFramebuffer> vulkanFramebufferPtr, VkExtent2D extent);
	~VulkanRenderPassBeginCommand();
	void end();
private:
};

