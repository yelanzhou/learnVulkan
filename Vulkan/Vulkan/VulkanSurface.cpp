#include "VulkanSurface.h"
#include "VulkanInstance.h"
#include "VulkanGpu.h"
#include <GLFW/glfw3.h>
#include <assert.h>


VulkanSurface::VulkanSurface(GLFWwindow* window, std::shared_ptr<VulkanInstance> vulkanInstancePtr/*,uint32_t width, uint32_t height*/)
	:m_vulkanInstancePtr(vulkanInstancePtr)
	//m_width(width),
	//m_height(height)
{
	VkResult ret = glfwCreateWindowSurface(m_vulkanInstancePtr->getHandle(), window, nullptr, &m_vulkanSurface);
	assert(ret == VK_SUCCESS);
}

VulkanSurface::~VulkanSurface()
{
	vkDestroySurfaceKHR(m_vulkanInstancePtr->getHandle(), m_vulkanSurface, nullptr);
}

VkSurfaceKHR VulkanSurface::getHandel()
{
	return m_vulkanSurface;
}

bool VulkanSurface::isSupportByQueueFamily(std::shared_ptr<VulkanGpu> gpuPtr,uint32_t index)
{
	VkBool32 presentSupport = 0;
	vkGetPhysicalDeviceSurfaceSupportKHR(gpuPtr->getHandle(), index, m_vulkanSurface, &presentSupport);
	return presentSupport == 0;
}

//uint32_t VulkanSurface::getWidth()
//{
//	return m_width;
//}
//
//uint32_t VulkanSurface::getHeight()
//{
//	return m_height;
//}
