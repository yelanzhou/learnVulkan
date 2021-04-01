#pragma once
#include <vulkan/vulkan.h>
#include <memory>


class VulkanDevice;

class VulkanSampler
{
public:
	VulkanSampler(std::shared_ptr<VulkanDevice> vulkanDevicePtr);
	~VulkanSampler();
	VkSampler getHandle();


private:
	VkSampler     m_vulkanSamplerHandle;
	std::shared_ptr<VulkanDevice>  m_vulkanDevicePtr;
};

