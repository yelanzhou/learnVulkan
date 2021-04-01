#pragma once
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>


class VulkanGpu;
class VulkanSurface;
class VulkanDevice;
class VulkanImageView;
class VulkanSemaphore;
class VulkanQueue;


class VulkanSwapChainKHR
{
public:
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
	VulkanSwapChainKHR(std::shared_ptr<VulkanGpu> vulkanGpuPtr, std::shared_ptr<VulkanSurface> vulkanSurfacePtr, std::shared_ptr<VulkanDevice> vulkanDevicePtr,uint32_t width,uint32_t height);
	~VulkanSwapChainKHR();

	VkExtent2D getExtent();

	VkFormat getFormat();

	std::vector<std::shared_ptr<VulkanImageView>> getImageViews();

	//return index
	uint32_t getNextImageForDrawing(std::shared_ptr<VulkanSemaphore> semaphorePtr);

	void presentKHR(std::shared_ptr<VulkanSemaphore> waitSemaphorePtr,std::shared_ptr<VulkanQueue> presentQueue, uint32_t imageIndex);

	void resize(uint32_t width, uint32_t height);

private:

	void createSwapchain();

	VkSwapchainKHR                                 m_vulkanSwapchainHandle = VK_NULL_HANDLE;
	std::shared_ptr<VulkanGpu>                     m_vulkanGpuPtr;
	std::shared_ptr<VulkanSurface>                 m_vulkanSurfacePtr;
	std::shared_ptr<VulkanDevice>                  m_vulkanDevicePtr;
	std::vector<std::shared_ptr<VulkanImageView>>  m_swapchainImageViewPtrs;

	uint32_t                          m_width;
	uint32_t                          m_height;

	VkExtent2D                        m_swapchainExtent;
	VkFormat                          m_swapchainFormat;

	std::vector<VkImage>              m_swapchainImage;


	SwapChainSupportDetails           m_swapchianDetails;
};

