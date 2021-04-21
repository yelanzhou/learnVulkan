#pragma once
#include "VulkanCommandBase.h"

class VulkanCommandBuffer;
class VulkanDrawCommand :
	public VulkanCommandBase
{
public:
	VulkanDrawCommand(std::shared_ptr<VulkanCommandBuffer> vulkanCommandBufferPtr,uint32_t vertexCount);
	~VulkanDrawCommand();

private:

};

