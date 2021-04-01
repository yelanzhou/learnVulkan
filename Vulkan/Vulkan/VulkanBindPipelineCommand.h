#pragma once
#include "VulkanCommandBase.h"
#include <vulkan/vulkan.h>
#include <memory>

class VulkanCommandbuffer;
class VulkanPipeline;

class VulkanBindPipelineCommand :
	public VulkanCommandBase
{
public:
	VulkanBindPipelineCommand(std::shared_ptr<VulkanCommandBuffer> vulkanCommandbufferPtr, std::shared_ptr<VulkanPipeline> vulkanPipelinePtr);
	~VulkanBindPipelineCommand();

private:

};

