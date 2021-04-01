#pragma once
#include <vulkan/vulkan.h>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

class VulkanDevice;
class VulkanShader
{
public:
	VulkanShader(std::string shaderFile, std::shared_ptr<VulkanDevice> vulkanDevicePtr);
	~VulkanShader();
	VkShaderModule getShaderModule();
private:

	void readFile(std::string shaderFile);
	void initShader();
	std::shared_ptr<VulkanDevice> m_vulkanDevicePtr;
	VkShaderModule m_shader;
	std::vector<char> m_shaderCoder;


};
