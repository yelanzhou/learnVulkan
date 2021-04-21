#include "VulkanGlobal.h"
#include "VulkanInstance.h"
#include "VulkanGpuManager.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanSwapChainKHR.h"

void VulkanGlobal::init(uint32_t width, uint32_t height)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(width, height, "Vulkan window", nullptr, nullptr);
	m_vulkanInstancePtr = std::make_shared<VulkanInstance>(true);
	m_vulkanGpuManagerPtr = std::make_shared<VulkanGpuManager>(m_vulkanInstancePtr);

	m_vulkanSurface= std::make_shared<VulkanSurface>(m_window, VulkanGlobal::m_vulkanInstancePtr);

	m_vulkanDevice= std::make_shared<VulkanDevice>(VulkanGlobal::m_vulkanInstancePtr, m_vulkanGpuManagerPtr->getSuitableGpu());
	m_vulkanDevice->initDevice();
	m_vulkanSwapChain = std::make_shared<VulkanSwapChainKHR>(m_vulkanGpuManagerPtr->getSuitableGpu(), m_vulkanSurface, m_vulkanDevice,  width, height);

}

VulkanGlobal& VulkanGlobal::getSingleton()
{
	static VulkanGlobal instance;
	return instance;
}

std::shared_ptr<VulkanInstance> VulkanGlobal::getVulkanInstance()
{
	return m_vulkanInstancePtr;
}

std::shared_ptr<VulkanGpuManager> VulkanGlobal::getGpuManager()
{
	return m_vulkanGpuManagerPtr;
}

std::shared_ptr<VulkanSurface> VulkanGlobal::getVulkanSurface()
{
	return m_vulkanSurface;
}

std::shared_ptr<VulkanDevice> VulkanGlobal::getVulkanDevice()
{
	return m_vulkanDevice;
}

std::shared_ptr<VulkanSwapChainKHR> VulkanGlobal::getVulkanSwapChain()
{
	return m_vulkanSwapChain;
}

bool VulkanGlobal::renderLoop()
{
	return !glfwWindowShouldClose(m_window);
}

VulkanGlobal::VulkanGlobal()
{
}

VulkanGlobal::~VulkanGlobal()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
