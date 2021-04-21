#pragma once
#include <memory>
#include <GLFW/glfw3.h>

class VulkanInstance;
class VulkanGpuManager;
class VulkanSurface;
class VulkanDevice;
class VulkanSwapChainKHR;
class VulkanGlobal
{
public:
	
	VulkanGlobal(const VulkanGlobal& rhs) = delete;
	VulkanGlobal& operator=(const VulkanGlobal& rhs) = delete;
	~VulkanGlobal();
	void init(uint32_t width = 800, uint32_t height = 600);
	static VulkanGlobal& getSingleton();

	std::shared_ptr<VulkanInstance>    getVulkanInstance();
	std::shared_ptr<VulkanGpuManager>  getGpuManager();

	std::shared_ptr<VulkanSurface>      getVulkanSurface();
	std::shared_ptr<VulkanDevice>       getVulkanDevice();
	std::shared_ptr<VulkanSwapChainKHR> getVulkanSwapChain();

	bool renderLoop();
private:
	VulkanGlobal();

	std::shared_ptr<VulkanInstance>     m_vulkanInstancePtr;
	std::shared_ptr<VulkanGpuManager>   m_vulkanGpuManagerPtr;
	std::shared_ptr<VulkanSurface>      m_vulkanSurface;
	std::shared_ptr<VulkanDevice>       m_vulkanDevice;
	std::shared_ptr<VulkanSwapChainKHR> m_vulkanSwapChain;

	GLFWwindow*                         m_window;


};

