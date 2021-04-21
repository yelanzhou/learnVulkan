#pragma once

#include <memory>
#include<vulkan/vulkan.h>

class VulkanDevice;
class VulkanPipeline;
class VulkanDescriptorSetLayout;
class VulkanDescriptorPool;

class FirstTriangleRenderPass
{

public:
	FirstTriangleRenderPass(std::shared_ptr<VulkanDevice> devicePtr);

private:

	void initRenderPass();
	
	VkRenderPass                                      m_renderPassHandle;
	std::shared_ptr<VulkanDevice>                     m_device;
	std::shared_ptr<VulkanPipeline>                   m_pipeline;

	std::shared_ptr<VulkanDescriptorSetLayout>        m_DesSetLayout;


};