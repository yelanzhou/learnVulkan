#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "VulkanBuffer.h"
struct UniformData {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};



class VulkanDevice;
class VulkanGpu;

/********************************
后期优化可以使用template-
*********************************/
class VulkanUniformBuffer :
	public VulkanBuffer
{
public:
    VulkanUniformBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr, UniformData uniformData);
    ~VulkanUniformBuffer();

    void updateData(UniformData uniformData);

};

