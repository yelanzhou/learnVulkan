#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>


class VulkanPipelineLayout;
class VulkanDevice;
class VulkanRenderPass;

class VulkanGraphicPipelineBase
{
public:
	enum class ShaderType
	{
		Vertex = VK_SHADER_STAGE_VERTEX_BIT,
		Frag = VK_SHADER_STAGE_FRAGMENT_BIT,
	};
	struct vertexs
	{
		glm::vec2 pos;
		glm::vec3 color;
	};
	struct ShaderInfo
	{
		std::string shaderSource;
		ShaderType type;
	};

	struct AttachmentState
	{
		VkColorComponentFlags colorWriteMask;
		VkBool32 blendEnable;
	};
	VulkanGraphicPipelineBase(std::shared_ptr<VulkanDevice> vulkanDevicePtr);
	~VulkanGraphicPipelineBase();

	void createPipeline(
		std::shared_ptr<VulkanPipelineLayout> pipelineLayout,
		std::shared_ptr<VulkanRenderPass>     renderpass
		);

	VkPipeline getHandle();

	virtual void initShaderStageInfo() = 0;
	virtual void initInputAssemblyStateInfo();
	virtual void initRaseterizationStateInfo();
	//创建blendstate过程需要优化
	virtual void initColorBlendStateInfo();
	virtual void initDepthStencilStateInfo();
	virtual void initViewportStateInfo();
	virtual void initMultisampleStateInfo();
	virtual void initDynamicStateInfo();

	virtual void initVertexInputBindingDesciption() = 0;
	virtual void initVertexInputAttributeDescription() = 0;
	void initVertexInputStateCreateInfo();

protected:

	void createShaderStageInfo(
		const std::vector<ShaderInfo>& shaderInfos
		);

	void createInputAssemblyStateInfo(
		VkPrimitiveTopology topology,
		VkBool32 enableprimitiveRestart);

	void createRasterizationStateInfo(
		VkPolygonMode polygonMode,
		VkCullModeFlags cullMode,
		VkFrontFace frontFace);

	void createColorBlendAttachmentState(
		const std::vector<AttachmentState>& states
		);

	void createColorBlendStateInfo();

	void createDepthStencilStateInfo(
		VkBool32 depthTestEnable,
		VkBool32 depthWriteEnable,
		VkCompareOp depthCompareOp);

	void createViewportStateInfo(
		uint32_t viewportCount,
		uint32_t scissorCount);

	void createMultisampleStateInfo(
		VkSampleCountFlagBits rasterizationSamples);

	void createDynamicStateInfo(
		const std::vector<VkDynamicState>& states);

	VkPipelineInputAssemblyStateCreateInfo                        m_inputAssemblyStateInfo;
	std::vector<VkVertexInputBindingDescription>                  m_vertexInputBindingDescriptions;
	std::vector<VkVertexInputAttributeDescription>                m_vertexInputAttributeDescriptions;
	VkPipelineVertexInputStateCreateInfo                          m_vertexInputStateInfo;

	std::vector<VkPipelineShaderStageCreateInfo>                  m_shaderStageInfo;
	VkPipelineRasterizationStateCreateInfo                        m_rasterizationStateInfo;
	std::vector<VkPipelineColorBlendAttachmentState>              m_colorBlendAttachmentStates;
	VkPipelineColorBlendStateCreateInfo                           m_colorBlendStateInfo;
	VkPipelineDepthStencilStateCreateInfo                         m_depthStencilStateInfo;
	VkPipelineViewportStateCreateInfo                             m_viewportStateInfo;
	VkPipelineMultisampleStateCreateInfo                          m_multisampleStateInfo;
	std::vector<VkDynamicState>                                   m_dynamicStates;
	VkPipelineDynamicStateCreateInfo                              m_dynamicStateInfo;

	VkPipeline                                                    m_vulkanPipelineHandle = VK_NULL_HANDLE;
	std::shared_ptr<VulkanDevice>                                 m_vulkanDevicePtr;
};

