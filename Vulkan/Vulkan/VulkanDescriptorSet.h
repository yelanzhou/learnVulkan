#pragma once
#include <vulkan/vulkan.h>
#include <memory>


class VulkanDevice;
class VulkanDescriptorPool;
class VulkanUniformBuffer;
class VulkanDescriptorSetLayout;

class VulkanUniformBuffer;

class VulkanDescriptorSet
{
public:
	VulkanDescriptorSet(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanDescriptorPool> vulkanDescriptorPoolPtr,std::shared_ptr<VulkanDescriptorSetLayout> vulkanDesSetLayoutPtr);
	~VulkanDescriptorSet();

	VkDescriptorSet getHandle();

	void update(std::shared_ptr<VulkanUniformBuffer> vulkanUniformBufferPtr);

private:
	std::shared_ptr<VulkanDevice> m_vulkanDevicePtr;
	VkDescriptorSet  m_vulkanDescriptorSetHandle;
};

