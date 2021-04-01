#pragma once
#include "VertexData.h"
#include "VulkanVertexBufferBase.h"
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class VulkanDevice;
class VulkanGpu;
class VulkanHostVisibleVertexBuffer:public VulkanVertexBufferBase
{
public:

	VulkanHostVisibleVertexBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr,std::shared_ptr<VulkanGpu> vulkanGpuPtr, std::vector<VertexData> vertexDatas);
	~VulkanHostVisibleVertexBuffer();

};

