#pragma once
#include <vulkan/vulkan.h>
#include <memory>

class VulkanDevice;
class VulkanDescriptorSetLayout;
class VulkanDescriptorPool
{
public:
	VulkanDescriptorPool(std::shared_ptr<VulkanDevice> vulkanDevicePtr,std::shared_ptr<VulkanDescriptorSetLayout> vulkanDescriptorSetLayoutPtr,uint64_t descriptorCount);
	~VulkanDescriptorPool();

	VkDescriptorPool getHandle();

private:
	std::shared_ptr<VulkanDevice> m_vulkanDevicePtr;
	VkDescriptorPool              m_vulkanDescriptorPoolHandle;

};

