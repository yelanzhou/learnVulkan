#include "VulkanDevice.h"
#include "VulkanGpu.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanQueue.h"

#include <assert.h>


//#include "VulkanShader.h"
//#include "VulkanGraphicPipelineBase.h"

VulkanDevice::VulkanDevice(std::shared_ptr<VulkanInstance> vulkanInstancePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr)
	:enable_shared_from_this<VulkanDevice>(),
	m_vulkanInstancePtr(vulkanInstancePtr),
	m_vulkanGpuPtr(vulkanGpuPtr)
	//m_vulkanSurfacePtr(vulkanSurfacePtr)
{
}

void VulkanDevice::initDevice(VkQueueFlags queueFlags)
{
	std::vector<VkDeviceQueueCreateInfo> queueInfos;
	VkDeviceQueueCreateInfo queueInfo;
	float pro = 1.0f;

	uint32_t grahicIndex = -1;
	uint32_t computeIndex = -1;
	uint32_t transferIndex = -1;

	grahicIndex = getQueueIndex(VK_QUEUE_GRAPHICS_BIT);
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.flags = 0;
	queueInfo.pNext = nullptr;
	queueInfo.pQueuePriorities = &pro;
	queueInfo.queueCount = 1;
	queueInfo.queueFamilyIndex = grahicIndex;
	queueInfos.push_back(queueInfo);

	computeIndex = getQueueIndex(VK_QUEUE_COMPUTE_BIT);
	if (computeIndex != -1 && computeIndex != grahicIndex)
	{
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.flags = 0;
		queueInfo.pNext = nullptr;
		queueInfo.pQueuePriorities = &pro;
		queueInfo.queueCount = 1;
		queueInfo.queueFamilyIndex = computeIndex;
		queueInfos.push_back(queueInfo);
	}
	else
	{
		computeIndex = grahicIndex;
	}

	if (transferIndex != -1 && transferIndex != grahicIndex && computeIndex != transferIndex)
	{
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.flags = 0;
		queueInfo.pNext = nullptr;
		queueInfo.pQueuePriorities = &pro;
		queueInfo.queueCount = 1;
		queueInfo.queueFamilyIndex = transferIndex;
		queueInfos.push_back(queueInfo);
	}
	else
	{
		transferIndex = grahicIndex;
	}


	VkPhysicalDeviceFeatures features = {};
	VkDeviceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pNext = nullptr;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
	createInfo.ppEnabledLayerNames = nullptr;
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();
	createInfo.pEnabledFeatures = &features;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
	createInfo.pQueueCreateInfos = queueInfos.data();
	assert(vkCreateDevice(m_vulkanGpuPtr->getHandle(), &createInfo, nullptr, &m_vulkanDeviceHandle) == VK_SUCCESS);

	m_queueTypeMap[QueueType::GRAPHIC] = std::make_shared<VulkanQueue>(shared_from_this(), grahicIndex);
	m_queueTypeMap.insert(std::make_pair(QueueType::PRESENT, std::make_shared<VulkanQueue>(shared_from_this(), grahicIndex)));
	m_queueTypeMap.insert(std::make_pair(QueueType::COMPUTE, std::make_shared<VulkanQueue>(shared_from_this(), computeIndex)));
	//m_queueTypeMap.insert(std::make_pair(QueueType::TRANSFER, std::make_shared<VulkanQueue>(shared_from_this(), transferIndex)));
}

std::shared_ptr<VulkanQueue> VulkanDevice::getPresentQueue()
{
	auto iter = m_queueTypeMap.find(QueueType::PRESENT);
	if (iter != m_queueTypeMap.end())
	{
		return iter->second;
	}
	return nullptr;

}

std::shared_ptr<VulkanQueue> VulkanDevice::getGraphicQueue()
{
	auto iter = m_queueTypeMap.find(QueueType::GRAPHIC);
	if (iter != m_queueTypeMap.end())
	{
		return iter->second;
	}
	return nullptr;
}

std::shared_ptr<VulkanQueue> VulkanDevice::getQueue(QueueType type)
{
	auto iter = m_queueTypeMap.find(type);
	if (iter != m_queueTypeMap.end())
	{
		return iter->second;
	}
	return nullptr;
}

VulkanDevice::~VulkanDevice()
{
	vkDestroyDevice(m_vulkanDeviceHandle, nullptr);
}

VkDevice VulkanDevice::getHandle()
{
	return m_vulkanDeviceHandle;
}

void VulkanDevice::setupPresentQueue(std::shared_ptr<VulkanSurface> vulkanSurfacePtr)
{
	VkBool32  presentSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(m_vulkanGpuPtr->getHandle(), m_queueTypeMap[QueueType::GRAPHIC]->getFamilyIndex(), vulkanSurfacePtr->getHandel(), &presentSupport);
	if (!presentSupport)
	{
		std::cerr << "Cannot find a compatible Vulkan device that supports surface presentation";
		assert(false);
	}
}




uint32_t VulkanDevice::getQueueIndex(VkQueueFlagBits flags)
{
	auto& queueFamilyProperties = m_vulkanGpuPtr->getQueueFamilyProperties();
	if (flags & VK_QUEUE_COMPUTE_BIT)
	{		
		for (int i = 0; i < queueFamilyProperties.size(); ++i)
		{
			if ((queueFamilyProperties[i].queueFlags & flags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
			{
				return i;
			}
		}
	}

	if (flags & VK_QUEUE_TRANSFER_BIT)
	{
		//auto& queueFamilyProperties = m_vulkanGpuPtr->getQueueFamilyProperties();
		for (int i = 0; i < queueFamilyProperties.size(); ++i)
		{
			if ((queueFamilyProperties[i].queueFlags & flags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) & ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
			{
				return i;
			}
		}
	}
	// For other queue types or if no separate compute queue is present, return the first one to support the requested flags
	for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
	{
		if (queueFamilyProperties[i].queueFlags & flags)
		{
			return i;
		}
	}
	std::cerr << "can not find suitablel graphic";
	//assert(false);
}


