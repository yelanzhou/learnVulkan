#pragma once
#include <memory>
#include <vulkan/vulkan.h>

class VulkanSwapChainKHR;
class VulkanDevice;


class VulkanRenderPass
{
public:
	VulkanRenderPass(std::shared_ptr<VulkanDevice> vulkanDevicePtr,std::shared_ptr<VulkanSwapChainKHR> vulkanSwapchainKHRPtr);
	~VulkanRenderPass();

	VkRenderPass getHandle();
private:
	std::shared_ptr<VulkanDevice>         m_vulkanDevicePtr;
	std::shared_ptr<VulkanSwapChainKHR>   m_vulkanSwapchainKHR;


	VkRenderPass                          m_vulkanRenderPassHandle;

};

