#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <glm/glm.hpp>


class VulkanPipelineLayout;
class VulkanDevice;
class VulkanSwapChainKHR;
class VulkanRenderPass;

class VulkanPipeline
{
public:
	struct vertexs
	{
		glm::vec2 pos;
		glm::vec3 color;
	};
	VulkanPipeline(std::shared_ptr<VulkanDevice> vulkanDevicePtr,std::shared_ptr<VulkanSwapChainKHR> vulkanSwapchainPtr,std::shared_ptr<VulkanRenderPass> vulkanRenderPassPtr, std::shared_ptr<VulkanPipelineLayout> vulkanPipelineLayoutPtr);
	~VulkanPipeline();

	VkPipeline getHandle();

private:
	void initPipeline();

	void initPipelinelayout();
	VkPipelineDynamicStateCreateInfo initDynamicStateInfo();

	VkPipelineColorBlendStateCreateInfo initColorBlendStateInfo();
	VkPipelineMultisampleStateCreateInfo initMultisampleStateInfo();

	VkPipelineRasterizationStateCreateInfo initRaseterizationStateInfo();
	
	std::shared_ptr<VkPipelineViewportStateCreateInfo> initViewportState();
	VkPipelineInputAssemblyStateCreateInfo initInputAssemblyStateInfo();
	std::vector<VkPipelineShaderStageCreateInfo> initShaderStateStateInfo();
	VkPipelineVertexInputStateCreateInfo initVertexInputStateInfo();


	VkPipeline                              m_vulkanPipelineHandle = VK_NULL_HANDLE;
	std::shared_ptr<VulkanDevice>           m_vulkanDevicePtr;
	std::shared_ptr<VulkanSwapChainKHR>     m_vulkanSwapchainPtr;
	std::shared_ptr<VulkanPipelineLayout>   m_vulkanPipelineLayoutPtr;

	std::shared_ptr<VulkanRenderPass>      m_vulkanRenderPassPtr;

};

