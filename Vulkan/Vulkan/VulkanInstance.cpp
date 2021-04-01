#include "VulkanInstance.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <GLFW/glfw3.h>

#include <assert.h>



PFN_vkCreateDebugUtilsMessengerEXT pvkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pvkDestroyDebugUtilsMessengerEXT;


VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	// Select prefix depending on flags passed to the callback
	std::string prefix("");

	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
		prefix = "VERBOSE: ";
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
		prefix = "INFO: ";
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		prefix = "WARNING: ";
	}
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		prefix = "ERROR: ";
	}
	// Display message to default output (console/logcat)
	std::stringstream debugMessage;
	debugMessage << prefix << "[" << pCallbackData->messageIdNumber << "][" << pCallbackData->pMessageIdName << "] : " << pCallbackData->pMessage;

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		std::cerr << debugMessage.str() << "\n";
	}
	else {
		std::cout << debugMessage.str() << "\n";
	}
	fflush(stdout);



	// The return value of this callback controls whether the Vulkan call that caused the validation message will be aborted or not
	// We return VK_FALSE as we DON'T want Vulkan calls that cause a validation message to abort
	// If you instead want to have calls abort, pass in VK_TRUE and the function will return VK_ERROR_VALIDATION_FAILED_EXT 
	return VK_FALSE;
}

struct LayerExtension
{
	VkLayerProperties layer;
	std::vector<VkExtensionProperties> extensions;
};

VulkanInstance::VulkanInstance(bool enableValidation)
	:m_enableValidation(enableValidation)
{
	initInstance();

}

void VulkanInstance::getInstanceLayersAndExtensions(std::vector<const char*>& layers, std::vector<const char*>& extensions)
{
	layers.clear();
	extensions.clear();

	std::vector<LayerExtension> layerExtensions;
	uint32_t count = 0;
	vkEnumerateInstanceLayerProperties(&count,nullptr);
	std::vector<VkLayerProperties> layerProperties(count);
	vkEnumerateInstanceLayerProperties(&count, layerProperties.data());


	for (int i = 0; i < layerProperties.size(); ++i)
	{
		layers.push_back(layerProperties[i].layerName);

		vkEnumerateInstanceExtensionProperties(layerProperties[i].layerName, &count, nullptr);
		std::vector<VkExtensionProperties> extensionPropertys(count);
		vkEnumerateInstanceExtensionProperties(layerProperties[i].layerName, &count, extensionPropertys.data());

		LayerExtension layerExtension;
		layerExtension.layer = layerProperties[i];
		layerExtension.extensions = extensionPropertys;
		layerExtensions.push_back(layerExtension);

	}

}

VulkanInstance::~VulkanInstance()
{
	pvkDestroyDebugUtilsMessengerEXT(m_vulkanInstance, m_debugUtilsMessenger, nullptr);
	vkDestroyInstance(m_vulkanInstance, nullptr);
}


inline void VulkanInstance::initInstance()
{
	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.apiVersion = VK_API_VERSION_1_1;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "lanzhou";
	appInfo.pApplicationName = "lanzhou11";

	uint32_t count = 0;
	const char** extensions = glfwGetRequiredInstanceExtensions(&count);

	std::vector<const char*> extensionVecs(count);
	for (int i = 0; i < count; ++i)
	{
		extensionVecs[i] = extensions[i];
	}

	if (m_enableValidation)
	{
		extensionVecs.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		extensionVecs.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	VkInstanceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = extensionVecs.size();
	createInfo.ppEnabledExtensionNames = extensionVecs.data();


	const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
	if (m_enableValidation)
	{
		// Check if this layer is available at instance level
		uint32_t instanceLayerCount;
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
		std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
		bool validationLayerPresent = false;
		for (VkLayerProperties layer : instanceLayerProperties) {
			if (strcmp(layer.layerName, validationLayerName) == 0) {
				validationLayerPresent = true;
				break;
			}
		}
		if (validationLayerPresent) {
			createInfo.ppEnabledLayerNames = &validationLayerName;
			createInfo.enabledLayerCount = 1;
		}
		else {
			std::cerr << "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled";
		}
	}

	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = nullptr;

	assert(vkCreateInstance(&createInfo, nullptr, &m_vulkanInstance) == VK_SUCCESS);

	if (m_enableValidation)
	{
		pvkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_vulkanInstance, "vkCreateDebugUtilsMessengerEXT"));
		pvkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_vulkanInstance, "vkDestroyDebugUtilsMessengerEXT"));

		VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCI{};
		debugUtilsMessengerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugUtilsMessengerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugUtilsMessengerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debugUtilsMessengerCI.pfnUserCallback = debugUtilsMessengerCallback;
		assert(VK_SUCCESS == pvkCreateDebugUtilsMessengerEXT(m_vulkanInstance, &debugUtilsMessengerCI, nullptr, &m_debugUtilsMessenger));
	}
}


