#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <iostream>
#include <memory>
class VulkanInstance
{
private:
	VkInstance       m_vulkanInstance;
	VkDebugUtilsMessengerEXT m_debugUtilsMessenger;
	bool             m_enableValidation = false;
public:
	VulkanInstance(bool enableValidation = false);
	~VulkanInstance();

	void getInstanceLayersAndExtensions(std::vector<const char*> &layers, std::vector<const char*> &extensions);
	VkInstance getHandle();
	void initInstance();
};

inline VkInstance VulkanInstance::getHandle()
{
	return m_vulkanInstance;
}







