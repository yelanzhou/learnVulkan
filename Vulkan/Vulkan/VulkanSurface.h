#pragma once
#include <vulkan/vulkan.h>
#include <memory>
#include <GLFW/glfw3.h>

class VulkanInstance;
class VulkanGpu;
class VulkanSurface
{
public:
	VulkanSurface(GLFWwindow* window, std::shared_ptr<VulkanInstance> vulkanInstancePtr/*,uint32_t width,uint32_t height*/);
	VulkanSurface() = delete;
	VulkanSurface(VulkanSurface&) = delete;
	~VulkanSurface();

	VkSurfaceKHR getHandel();
	bool isSupportByQueueFamily(std::shared_ptr<VulkanGpu> gpuPtr,uint32_t index);

private:

	VkSurfaceKHR                        m_vulkanSurface = VK_NULL_HANDLE;
	std::shared_ptr<VulkanInstance>     m_vulkanInstancePtr;
};

