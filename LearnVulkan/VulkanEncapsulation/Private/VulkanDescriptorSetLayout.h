#pragma once
#include "VulkanBaseType.h"

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class VulkanDevice;



class VulkanDescriptorSetLayout
{
public:

	VulkanDescriptorSetLayout(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<std::vector<VkDescriptorSetLayoutBinding>> bindings);
	~VulkanDescriptorSetLayout();
	VkDescriptorSetLayout getHandle();
	std::shared_ptr<std::vector<VkDescriptorSetLayoutBinding>> getLayoutBindings();

private:

	std::shared_ptr<VulkanDevice> m_vulkanDevicePtr;
	VkDescriptorSetLayout m_vulkanSetLayoutHandle;

	std::shared_ptr<std::vector<VkDescriptorSetLayoutBinding>> m_bindings;
		//VkDescriptorType              m_type;
		//VkShaderStageFlags            m_shaderStages;

};
