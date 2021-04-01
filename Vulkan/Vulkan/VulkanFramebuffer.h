#pragma once
#include <vulkan/vulkan.h>
#include <memory>
class VulkanDevice;
class VulkanImageView;
class VulkanRenderPass;


class VulkanFramebuffer
{
public:
	VulkanFramebuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanRenderPass> vulkanRenderPassPtr, std::shared_ptr<VulkanImageView> vulkanImageViewPtr, VkExtent2D extent);
	~VulkanFramebuffer();

	VkFramebuffer getHandle();
private:
	std::shared_ptr<VulkanDevice>  m_vulkanDevicePtr;
	VkFramebuffer                  m_vulkanFramebufferHandle;
};

