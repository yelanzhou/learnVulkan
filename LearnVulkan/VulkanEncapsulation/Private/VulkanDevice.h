#pragma once
//#include "VulkanInstance.h"

//#include "VulkanCommandPool.h"
//#include "VulkanCommandBuffer.h"
//#include "VulkanPipeline.h"
#include <vulkan/vulkan.h>
//#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <map>



class VulkanInstance;
class VulkanGpu;
class VulkanSurface;
class VulkanQueue;
class VulkanDevice:public std::enable_shared_from_this<VulkanDevice>
{

public:

	enum class QueueType
	{
		GRAPHIC,
		COMPUTE,
		TRANSFER,
		PRESENT,
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};


	VulkanDevice(std::shared_ptr<VulkanInstance> vulkanInstancePtr,std::shared_ptr<VulkanGpu> vulkanGpuPtr);
	
	~VulkanDevice();

	VkDevice getHandle();

	void initDevice(VkQueueFlags queueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

	std::shared_ptr<VulkanQueue> getPresentQueue();
	std::shared_ptr<VulkanQueue> getGraphicQueue();
	std::shared_ptr<VulkanQueue> getQueue(QueueType type);

	void setupPresentQueue(std::shared_ptr<VulkanSurface> vulkanSurfacePtr);


private:

	uint32_t getQueueIndex(VkQueueFlagBits flags);

	VkDevice                         m_vulkanDeviceHandle;
	std::shared_ptr<VulkanInstance>  m_vulkanInstancePtr;
	std::shared_ptr<VulkanGpu>       m_vulkanGpuPtr;

	std::map<QueueType, std::shared_ptr<VulkanQueue>>  m_queueTypeMap;

	const std::vector<const char*> m_deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

};

