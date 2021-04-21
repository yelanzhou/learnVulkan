#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

class VulkanDevice;
class VulkanDescriptorSetLayout;
class VulkanPipelineLayout
{
public:
	VulkanPipelineLayout(std::shared_ptr<VulkanDevice> vulkanDevicePtr,std::shared_ptr<VulkanDescriptorSetLayout> vulkanDesLayoutPtr);
	~VulkanPipelineLayout();

	VkPipelineLayout getHandle();
private:

	std::shared_ptr<VulkanDevice> m_vulkanDevicePtr;
	VkPipelineLayout              m_pipelineLayoutHandle;

};

