#include "VulkanShader.h"
#include "VulkanDevice.h"
#include <iostream>
#include <assert.h>

VulkanShader::VulkanShader(std::string shaderFile, std::shared_ptr<VulkanDevice> vulkanDevice)
	:m_vulkanDevicePtr(vulkanDevice)
{
	readFile(shaderFile);
	initShader();
}

VkShaderModule VulkanShader::getShaderModule()
{
	return m_shader;
}

VulkanShader::~VulkanShader()
{
	vkDestroyShaderModule(m_vulkanDevicePtr->getHandle(), m_shader, nullptr);
}

void VulkanShader::readFile(std::string shaderFile)
{
	std::ifstream shaderStream(shaderFile, std::ios::ate | std::ios::binary);
	if (!shaderStream.is_open())
	{
		std::cout << "can not open " << shaderFile << std::endl;
		return;
	}

	size_t size = (size_t)shaderStream.tellg();
	shaderStream.seekg(0);
	m_shaderCoder.resize(size);

	shaderStream.read(m_shaderCoder.data(), size);

	shaderStream.close();
}


void VulkanShader::initShader()
{
	VkShaderModuleCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.codeSize = m_shaderCoder.size();
	createInfo.flags = 0;
	createInfo.pCode = reinterpret_cast<const uint32_t*>(m_shaderCoder.data());
	auto ret = vkCreateShaderModule(m_vulkanDevicePtr->getHandle(), &createInfo, nullptr, &m_shader);
	
	assert(VK_SUCCESS == ret);
}