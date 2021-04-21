#include "FirstTrianglePipeline.h"
#include "Private/VertexData.h"


void FirstTrianglePipeline::initShaderStageInfo()
{
	std::vector<ShaderInfo> shaderInfos = {
		 {"../../shader/vert.spv",ShaderType::Vertex},
		 {"../../shader/frag.spv",ShaderType::Frag}
	 };

	createShaderStageInfo(shaderInfos);
}

void FirstTrianglePipeline::initVertexInputBindingDesciption()
{
	m_vertexInputBindingDescriptions.resize(1);
	m_vertexInputBindingDescriptions[0].binding = 0;
	m_vertexInputBindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	m_vertexInputBindingDescriptions[0].stride = sizeof(VertexData);
}

void FirstTrianglePipeline::initVertexInputAttributeDescription()
{
	m_vertexInputAttributeDescriptions.resize(2);
	m_vertexInputAttributeDescriptions[0].binding = 0;
	m_vertexInputAttributeDescriptions[0].location = 0;
	m_vertexInputAttributeDescriptions[0].offset = offsetof(vertexs, pos);
	m_vertexInputAttributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;

	m_vertexInputAttributeDescriptions[0].binding = 0;
	m_vertexInputAttributeDescriptions[0].location = 1;
	m_vertexInputAttributeDescriptions[1].offset = offsetof(vertexs, color);
	m_vertexInputAttributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;


}
