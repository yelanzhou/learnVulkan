#pragma once
#include "VulkanInstance.h"
#include "VulkanGpu.h"
#include <memory>

class VulkanGpuManager
{

public:
	VulkanGpuManager(std::shared_ptr<VulkanInstance> vulkanInstancePtr);
	~VulkanGpuManager();

	std::shared_ptr<VulkanGpu> getSuitableGpu();

private:
	std::shared_ptr<VulkanInstance>          m_vulkanInstancePtr;
	std::vector <std::shared_ptr<VulkanGpu>> m_physcialDevices;

};

