#include "VulkanPipeline.h"
#include "VulkanShader.h"
#include "VulkanDevice.h"
#include "VulkanSwapChainKHR.h"
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



VulkanPipeline::VulkanPipeline(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanSwapChainKHR> vulkanSwapchainPtr, std::shared_ptr<VulkanRenderPass> vulkanRenderPassPtr, std::shared_ptr<VulkanPipelineLayout> vulkanPipelineLayoutPtr)
	:m_vulkanDevicePtr(vulkanDevicePtr),
	m_vulkanSwapchainPtr(vulkanSwapchainPtr),
	m_vulkanPipelineLayoutPtr(vulkanPipelineLayoutPtr),
	m_vulkanRenderPassPtr(vulkanRenderPassPtr)
{
	//initPipelinelayout();
	initPipeline();
	//initVerteBuffer();
}

VulkanPipeline::~VulkanPipeline()
{
	vkDestroyPipeline(m_vulkanDevicePtr->getHandle(), m_vulkanPipelineHandle, nullptr);
}

VkPipeline VulkanPipeline::getHandle()
{
	return m_vulkanPipelineHandle;
}


void VulkanPipeline::initPipeline()
{

	//shader
	std::vector<VkPipelineShaderStageCreateInfo> stages;
	VkPipelineShaderStageCreateInfo vertexStageInfo;
	std::string vertexShaderFile("../../shader/vert.spv");
	VulkanShader vertexShader(vertexShaderFile, m_vulkanDevicePtr);
	vertexStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexStageInfo.flags = 0;
	vertexStageInfo.pNext = nullptr;
	vertexStageInfo.pName = "main";
	vertexStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexStageInfo.module = vertexShader.getShaderModule();
	vertexStageInfo.pSpecializationInfo = nullptr;
	stages.push_back(vertexStageInfo);

	VkPipelineShaderStageCreateInfo fragStageInfo;
	std::string fragshaderFile("../../shader/frag.spv");
	VulkanShader fragShader(fragshaderFile, m_vulkanDevicePtr);
	fragStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragStageInfo.flags = 0;
	fragStageInfo.pNext = nullptr;
	fragStageInfo.pName = "main";
	fragStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragStageInfo.module = fragShader.getShaderModule();
	fragStageInfo.pSpecializationInfo = nullptr;
	stages.push_back(fragStageInfo);

	//viewport
	VkViewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = (float)m_vulkanSwapchainPtr->getExtent().width;
	viewport.height = (float)m_vulkanSwapchainPtr->getExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissorPort;
	scissorPort.offset = { 0,0 };
	scissorPort.extent = m_vulkanSwapchainPtr->getExtent();

	VkPipelineViewportStateCreateInfo viewportState;
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.flags = 0;
	viewportState.pNext = nullptr;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissorPort;

	//input state

	VkVertexInputBindingDescription vertexInputBindingDes;
	vertexInputBindingDes.binding = 0;
	vertexInputBindingDes.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertexInputBindingDes.stride = sizeof(vertexs);

	std::vector<VkVertexInputAttributeDescription> attributeDes;
	VkVertexInputAttributeDescription posDes;
	posDes.binding = 0;
	posDes.location = 0;
	posDes.offset = offsetof(vertexs, pos);
	posDes.format = VK_FORMAT_R32G32_SFLOAT;
	attributeDes.push_back(posDes);

	VkVertexInputAttributeDescription colorDes;
	colorDes.binding = 0;
	colorDes.location = 1;
	colorDes.offset = offsetof(vertexs, color);
	colorDes.format = VK_FORMAT_R32G32_SFLOAT;
	attributeDes.push_back(colorDes);

	VkPipelineVertexInputStateCreateInfo vertexInputStateInfo;
	vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateInfo.flags = 0;
	vertexInputStateInfo.pNext = nullptr;
	vertexInputStateInfo.vertexAttributeDescriptionCount = attributeDes.size();
	vertexInputStateInfo.pVertexAttributeDescriptions = attributeDes.data();
	vertexInputStateInfo.vertexBindingDescriptionCount = 1;
	vertexInputStateInfo.pVertexBindingDescriptions = &vertexInputBindingDes;


	//assemblestate
	VkPipelineInputAssemblyStateCreateInfo assemblyInfo;
	assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assemblyInfo.flags = 0;
	assemblyInfo.pNext = nullptr;
	assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	assemblyInfo.primitiveRestartEnable = VK_FALSE;


	//raster
	VkPipelineRasterizationStateCreateInfo rasterState;
	rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterState.pNext = nullptr;
	rasterState.flags = 0;
	rasterState.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterState.depthBiasEnable = VK_FALSE;
	rasterState.depthClampEnable = VK_FALSE;
	rasterState.depthBiasClamp = 0.0f;
	rasterState.depthBiasConstantFactor = 0.0f;
	rasterState.depthBiasSlopeFactor = 0.0f;
	rasterState.rasterizerDiscardEnable = VK_FALSE;
	rasterState.polygonMode = VK_POLYGON_MODE_FILL;
	rasterState.lineWidth = 1.0f;
	rasterState.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	//multisample 
	VkPipelineMultisampleStateCreateInfo multisampleState;
	multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleState.flags = 0;
	multisampleState.pNext = nullptr;
	multisampleState.alphaToCoverageEnable = VK_FALSE;
	multisampleState.alphaToOneEnable = VK_FALSE;
	//multisampleState.minSampleShading = 1;
	multisampleState.pSampleMask = nullptr;
	multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleState.sampleShadingEnable = VK_FALSE;
	
	//color blend
	VkPipelineColorBlendAttachmentState attachmentState;
	//attachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	attachmentState.blendEnable = VK_FALSE;
	//attachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	attachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
/*	attachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE*/;




	VkPipelineColorBlendStateCreateInfo colorBlendStateInfo;
	colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendStateInfo.pNext = 0;
	colorBlendStateInfo.attachmentCount = 1;
	colorBlendStateInfo.pAttachments = &attachmentState;
	colorBlendStateInfo.blendConstants[0] = 0.0f;
	colorBlendStateInfo.blendConstants[1] = 0.0f;
	colorBlendStateInfo.blendConstants[2] = 0.0f;
	colorBlendStateInfo.blendConstants[3] = 0.0f;
	colorBlendStateInfo.flags = 0;
	colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendStateInfo.logicOpEnable = VK_FALSE;

	//pipeline layout
	/*VkPipelineLayoutCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pNext = nullptr;
	createInfo.pPushConstantRanges = nullptr;
	createInfo.pushConstantRangeCount = 0;
	createInfo.setLayoutCount = 0;
	createInfo.pSetLayouts = nullptr;
	vkCreatePipelineLayout(m_device, &createInfo, nullptr, &m_pipelineLayout);*/


	//initPipelinelayout();


	VkGraphicsPipelineCreateInfo pipelineCreateInfo;



	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.flags = 0;
	pipelineCreateInfo.pNext = nullptr;
	pipelineCreateInfo.stageCount =static_cast<uint32_t>(stages.size());
	pipelineCreateInfo.pStages = stages.data();
	pipelineCreateInfo.pVertexInputState = &vertexInputStateInfo;
	pipelineCreateInfo.pInputAssemblyState = &assemblyInfo;
	pipelineCreateInfo.pViewportState = &viewportState;
	pipelineCreateInfo.pMultisampleState = &multisampleState;
	pipelineCreateInfo.pDepthStencilState = nullptr;
	pipelineCreateInfo.pColorBlendState = &colorBlendStateInfo;
	pipelineCreateInfo.pDynamicState = nullptr;
	pipelineCreateInfo.layout = m_vulkanPipelineLayoutPtr->getHandle();
	pipelineCreateInfo.renderPass = m_vulkanRenderPassPtr->getHandle();
	pipelineCreateInfo.subpass = 0;
	pipelineCreateInfo.pRasterizationState = &rasterState;
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineCreateInfo.basePipelineIndex = -1;
	pipelineCreateInfo.pTessellationState = nullptr;

	auto ret = vkCreateGraphicsPipelines(m_vulkanDevicePtr->getHandle(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_vulkanPipelineHandle);

	assert(VK_SUCCESS == ret);

}



VkPipelineDynamicStateCreateInfo VulkanPipeline::initDynamicStateInfo()

{
	VkDynamicState dynamics[] = { VK_DYNAMIC_STATE_VIEWPORT,VK_DYNAMIC_STATE_LINE_WIDTH };
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.pDynamicStates = dynamics;
	dynamicStateInfo.dynamicStateCount = 2;
	dynamicStateInfo.flags = 0;
	dynamicStateInfo.pNext = nullptr;
	return dynamicStateInfo;
}

VkPipelineColorBlendStateCreateInfo VulkanPipeline::initColorBlendStateInfo()
{


	VkPipelineColorBlendAttachmentState attachmentState;
	attachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	attachmentState.blendEnable = VK_TRUE;
	attachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	attachmentState.colorWriteMask = VK_COLOR_COMPONENT_A_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_R_BIT;
	attachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;




	VkPipelineColorBlendStateCreateInfo colorBlendStateInfo;
	colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendStateInfo.pNext = 0;
	colorBlendStateInfo.attachmentCount = 1;
	colorBlendStateInfo.pAttachments = &attachmentState;
	colorBlendStateInfo.blendConstants[0] = 0.0f;
	colorBlendStateInfo.blendConstants[1] = 0.0f;
	colorBlendStateInfo.blendConstants[2] = 0.0f;
	colorBlendStateInfo.blendConstants[3] = 0.0f;

	colorBlendStateInfo.flags = 0;
	colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendStateInfo.logicOpEnable = VK_FALSE;
	


	return colorBlendStateInfo;
}

VkPipelineMultisampleStateCreateInfo VulkanPipeline::initMultisampleStateInfo()
{
	VkPipelineMultisampleStateCreateInfo multisampleState;

	multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleState.flags = 0;
	multisampleState.pNext = nullptr;
	multisampleState.alphaToCoverageEnable = VK_FALSE;
	multisampleState.alphaToOneEnable = VK_FALSE;
	multisampleState.minSampleShading = 1;

	multisampleState.pSampleMask = nullptr;
	multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleState.sampleShadingEnable = VK_FALSE;

	return multisampleState;

}

VkPipelineRasterizationStateCreateInfo VulkanPipeline::initRaseterizationStateInfo()
{
	VkPipelineRasterizationStateCreateInfo ratsterInfo;
	ratsterInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	ratsterInfo.pNext = nullptr;
	ratsterInfo.flags = 0;
	ratsterInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	ratsterInfo.depthBiasEnable = VK_FALSE;
	ratsterInfo.depthClampEnable = VK_FALSE;
	ratsterInfo.depthBiasClamp = 0.0f;
	ratsterInfo.depthBiasConstantFactor = 0.0f;
	ratsterInfo.depthBiasSlopeFactor = 0.0f;

	ratsterInfo.rasterizerDiscardEnable = VK_TRUE;
	ratsterInfo.polygonMode = VK_POLYGON_MODE_FILL;
	ratsterInfo.lineWidth = 1.0f;
	ratsterInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;

	return ratsterInfo;

}

std::shared_ptr<VkPipelineViewportStateCreateInfo> VulkanPipeline::initViewportState()
{
	VkViewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = 800.0f;
	viewport.height = 600.0f;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;


	VkRect2D scissorPort;
	scissorPort.offset = { 0,0 };
	scissorPort.extent.width = 800.0f;
	scissorPort.extent.height = 600.0f;


	std::shared_ptr<VkPipelineViewportStateCreateInfo> viewportState(new  VkPipelineViewportStateCreateInfo());
	viewportState->sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState->flags = 0;
	viewportState->pNext = nullptr;
	viewportState->viewportCount = 1;
	viewportState->pViewports = &viewport;

	viewportState->scissorCount = 1;
	viewportState->pScissors = &scissorPort;

	return viewportState;
}

VkPipelineInputAssemblyStateCreateInfo VulkanPipeline::initInputAssemblyStateInfo()
{
	VkPipelineInputAssemblyStateCreateInfo assemblyInfo;
	assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assemblyInfo.flags = 0;
	assemblyInfo.pNext = nullptr;
	assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	assemblyInfo.primitiveRestartEnable = VK_FALSE;

	return assemblyInfo;
}

std::vector<VkPipelineShaderStageCreateInfo> VulkanPipeline::initShaderStateStateInfo()
{

	std::vector<VkPipelineShaderStageCreateInfo> stages;

	VkPipelineShaderStageCreateInfo vertexStageInfo;
	std::string vertexShaderFile("../../shader/vert.spv");
	VulkanShader vertexShader(vertexShaderFile, m_vulkanDevicePtr);
	vertexStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexStageInfo.flags = 0;
	vertexStageInfo.pNext = nullptr;
	vertexStageInfo.pName = "main";
	vertexStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexStageInfo.module = vertexShader.getShaderModule();
	stages.push_back(vertexStageInfo);

	VkPipelineShaderStageCreateInfo fragStageInfo;
	std::string fragshaderFile("../../shader/frag.spv");
	VulkanShader fragShader(fragshaderFile, m_vulkanDevicePtr);
	fragStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragStageInfo.flags = 0;
	fragStageInfo.pNext = nullptr;
	fragStageInfo.pName = "main";
	fragStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragStageInfo.module = fragShader.getShaderModule();
	stages.push_back(fragStageInfo);

	return stages;
}

VkPipelineVertexInputStateCreateInfo VulkanPipeline::initVertexInputStateInfo()
{
	VkPipelineVertexInputStateCreateInfo vertexInputStateInfo;
	vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateInfo.flags = 0;
	vertexInputStateInfo.pNext = nullptr;
	vertexInputStateInfo.vertexAttributeDescriptionCount = 0;
	vertexInputStateInfo.pVertexAttributeDescriptions = nullptr;

	vertexInputStateInfo.vertexBindingDescriptionCount = 0;
	vertexInputStateInfo.pVertexBindingDescriptions = nullptr;

	return vertexInputStateInfo;
}
