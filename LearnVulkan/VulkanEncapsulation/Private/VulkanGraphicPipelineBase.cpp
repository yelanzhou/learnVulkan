#include "VulkanGraphicPipelineBase.h"
#include "VulkanShader.h"
#include "VulkanDevice.h"
#include "VulkanPipelineLayout.h"
#include "VulkanRenderPass.h"

#include <iostream>
#include <assert.h>

VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}
static std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}



VulkanGraphicPipelineBase::VulkanGraphicPipelineBase(
	std::shared_ptr<VulkanDevice> vulkanDevicePtr)
	:m_vulkanDevicePtr(vulkanDevicePtr)
{

}

VulkanGraphicPipelineBase::~VulkanGraphicPipelineBase()
{
	vkDestroyPipeline(
		m_vulkanDevicePtr->getHandle(),
		m_vulkanPipelineHandle,
		nullptr);
}

void VulkanGraphicPipelineBase::createPipeline(
	std::shared_ptr<VulkanPipelineLayout> pipelineLayout,
	std::shared_ptr<VulkanRenderPass> renderpass)
{
	initInputAssemblyStateInfo();
	initVertexInputStateCreateInfo();
	initShaderStageInfo();
	initViewportStateInfo();
	initMultisampleStateInfo();
	initRaseterizationStateInfo();
	initColorBlendStateInfo();
	initDepthStencilStateInfo();
	initDynamicStateInfo();
	
	VkGraphicsPipelineCreateInfo pipelineCreateInfo;
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.flags = 0;
	pipelineCreateInfo.pNext = nullptr;
	pipelineCreateInfo.pInputAssemblyState = &m_inputAssemblyStateInfo;
	pipelineCreateInfo.pVertexInputState = &m_vertexInputStateInfo;
	pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_shaderStageInfo.size());
	pipelineCreateInfo.pStages = m_shaderStageInfo.data();	
	pipelineCreateInfo.pViewportState = &m_viewportStateInfo;
	pipelineCreateInfo.pMultisampleState = &m_multisampleStateInfo;
	pipelineCreateInfo.pDepthStencilState = &m_depthStencilStateInfo;
	pipelineCreateInfo.pColorBlendState = &m_colorBlendStateInfo;
	pipelineCreateInfo.pDynamicState = &m_dynamicStateInfo;
	pipelineCreateInfo.layout = pipelineLayout->getHandle();
	pipelineCreateInfo.renderPass = renderpass->getHandle();
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.pRasterizationState = &m_rasterizationStateInfo;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = -1;
	pipelineCreateInfo.pTessellationState = nullptr;

	auto ret = vkCreateGraphicsPipelines(
		m_vulkanDevicePtr->getHandle(),
		VK_NULL_HANDLE,
		1,
		&pipelineCreateInfo,
		nullptr,
		&m_vulkanPipelineHandle);
	assert(VK_SUCCESS == ret);
}

VkPipeline VulkanGraphicPipelineBase::getHandle()
{
	return m_vulkanPipelineHandle;
}


void VulkanGraphicPipelineBase::createShaderStageInfo(
	const std::vector<ShaderInfo>& shaderInfos)
{

	assert(shaderInfos.size() > 0);
	m_shaderStageInfo.resize(shaderInfos.size());
	for (int i = 0; i < shaderInfos.size(); ++i)
	{
		VulkanShader vulkanShader(shaderInfos[i].shaderSource, m_vulkanDevicePtr);
		m_shaderStageInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStageInfo[i].flags = 0;
		m_shaderStageInfo[i].pNext = nullptr;
		m_shaderStageInfo[i].pName = "main";
		m_shaderStageInfo[i].stage = static_cast<VkShaderStageFlagBits>(shaderInfos[i].type);
		m_shaderStageInfo[i].module = vulkanShader.getShaderModule();
		m_shaderStageInfo[i].pSpecializationInfo = nullptr;
	}

}

void VulkanGraphicPipelineBase::createInputAssemblyStateInfo(
	VkPrimitiveTopology topology,
	VkBool32 enableprimitiveRestart)
{
	m_inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	m_inputAssemblyStateInfo.flags = 0;
	m_inputAssemblyStateInfo.pNext = nullptr;
	m_inputAssemblyStateInfo.topology = topology;
	m_inputAssemblyStateInfo.primitiveRestartEnable = enableprimitiveRestart  ;
}

void VulkanGraphicPipelineBase::createRasterizationStateInfo(
	VkPolygonMode polygonMode,
	VkCullModeFlags cullMode,
	VkFrontFace frontFace)
{
	m_rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	m_rasterizationStateInfo.pNext = nullptr;
	m_rasterizationStateInfo.flags = 0;
	m_rasterizationStateInfo.cullMode = cullMode;
	m_rasterizationStateInfo.depthBiasEnable = VK_FALSE;
	m_rasterizationStateInfo.depthClampEnable = VK_FALSE;
	m_rasterizationStateInfo.depthBiasClamp = 0.0f;
	m_rasterizationStateInfo.depthBiasConstantFactor = 0.0f;
	m_rasterizationStateInfo.depthBiasSlopeFactor = 0.0f;
	m_rasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
	m_rasterizationStateInfo.polygonMode = polygonMode;
	m_rasterizationStateInfo.lineWidth = 1.0f;
	m_rasterizationStateInfo.frontFace = frontFace;
}

void VulkanGraphicPipelineBase::createColorBlendAttachmentState(
	const std::vector<AttachmentState>& states)
{
	m_colorBlendAttachmentStates.resize(states.size());
	for (int i = 0; i < states.size(); ++i)
	{//color blend
	//attachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
		m_colorBlendAttachmentStates[i].blendEnable = states[i].blendEnable;
		//attachmentState.colorBlendOp = VK_BLEND_OP_ADD;
		m_colorBlendAttachmentStates[i].colorWriteMask = states[i].colorWriteMask;
		/*attachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		attachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		attachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		attachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE*/;
	}
}

void VulkanGraphicPipelineBase::createColorBlendStateInfo()
{
	m_colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	m_colorBlendStateInfo.pNext = 0;
	m_colorBlendStateInfo.attachmentCount = m_colorBlendAttachmentStates.size();
	m_colorBlendStateInfo.pAttachments = m_colorBlendAttachmentStates.data();
	m_colorBlendStateInfo.blendConstants[0] = 0.0f;
	m_colorBlendStateInfo.blendConstants[1] = 0.0f;
	m_colorBlendStateInfo.blendConstants[2] = 0.0f;
	m_colorBlendStateInfo.blendConstants[3] = 0.0f;
	m_colorBlendStateInfo.flags = 0;
	m_colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
	m_colorBlendStateInfo.logicOpEnable = VK_FALSE;
}

void VulkanGraphicPipelineBase::createDepthStencilStateInfo(
	VkBool32 depthTestEnable,
	VkBool32 depthWriteEnable,
	VkCompareOp depthCompareOp)
{
	m_depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	m_depthStencilStateInfo.depthTestEnable = depthTestEnable;
	m_depthStencilStateInfo.depthWriteEnable = depthWriteEnable;
	m_depthStencilStateInfo.depthCompareOp = depthCompareOp;
	m_depthStencilStateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
}

void VulkanGraphicPipelineBase::createViewportStateInfo(
	uint32_t viewportCount,
	uint32_t scissorCount)
{
	m_viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	m_viewportStateInfo.viewportCount = viewportCount;
	m_viewportStateInfo.scissorCount = scissorCount;
	m_viewportStateInfo.flags = 0;
}

void VulkanGraphicPipelineBase::createMultisampleStateInfo(
	VkSampleCountFlagBits rasterizationSamples)
{
	m_multisampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	m_multisampleStateInfo.flags = 0;
	m_multisampleStateInfo.pNext = nullptr;
	m_multisampleStateInfo.alphaToCoverageEnable = VK_FALSE;
	m_multisampleStateInfo.alphaToOneEnable = VK_FALSE;
	//multisampleState.minSampleShading = 1;
	m_multisampleStateInfo.pSampleMask = nullptr;
	m_multisampleStateInfo.rasterizationSamples = rasterizationSamples;
	m_multisampleStateInfo.sampleShadingEnable = VK_FALSE;
}

void VulkanGraphicPipelineBase::createDynamicStateInfo(
	const std::vector<VkDynamicState>& states)
{
	m_dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	m_dynamicStateInfo.pDynamicStates = states.data();
	m_dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(states.size());
	m_dynamicStateInfo.flags = 0;
}


void VulkanGraphicPipelineBase::initDynamicStateInfo()
{
	m_dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	createDynamicStateInfo(m_dynamicStates);
}

void VulkanGraphicPipelineBase::initVertexInputStateCreateInfo()
{
	initVertexInputBindingDesciption();
	initVertexInputAttributeDescription();

	m_vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_vertexInputStateInfo.vertexBindingDescriptionCount = 
		static_cast<uint32_t>(m_vertexInputBindingDescriptions.size());
	m_vertexInputStateInfo.pVertexBindingDescriptions = m_vertexInputBindingDescriptions.data();
	m_vertexInputStateInfo.vertexAttributeDescriptionCount = 
		static_cast<uint32_t>(m_vertexInputAttributeDescriptions.size());
	m_vertexInputStateInfo.pVertexAttributeDescriptions = m_vertexInputAttributeDescriptions.data();
}

void VulkanGraphicPipelineBase::initColorBlendStateInfo()
{
	std::vector<AttachmentState> states=
	{
		{VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		VK_FALSE }
	};
	createColorBlendAttachmentState(states);
	createColorBlendStateInfo();
}

void VulkanGraphicPipelineBase::initDepthStencilStateInfo()
{
	createDepthStencilStateInfo(
		VK_TRUE,
		VK_TRUE,
		VK_COMPARE_OP_LESS_OR_EQUAL);
}

void VulkanGraphicPipelineBase::initViewportStateInfo()
{
	createViewportStateInfo(1, 1);
}

void VulkanGraphicPipelineBase::initMultisampleStateInfo()
{
	createMultisampleStateInfo(
		VK_SAMPLE_COUNT_1_BIT);
}

void VulkanGraphicPipelineBase::initRaseterizationStateInfo()
{
	createRasterizationStateInfo(
		VK_POLYGON_MODE_FILL,
		VK_CULL_MODE_BACK_BIT,
		VK_FRONT_FACE_CLOCKWISE);
}


void VulkanGraphicPipelineBase::initInputAssemblyStateInfo()
{
	createInputAssemblyStateInfo(
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		VK_FALSE);
}

