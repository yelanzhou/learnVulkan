#pragma once
#include <vulkan/vulkan.h>
#include <memory>

class VulkanDevice;
class VulkanGpu;
class VulkanBuffer
{
public:
	VulkanBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanGpu> vulkanGpuPtr,uint64_t size, VkBufferUsageFlags flags, VkMemoryPropertyFlags memoryProperttFlags);
	~VulkanBuffer();

	VkBuffer getHandle();

	uint64_t getSize();

	VkDeviceMemory getMemoryHandle();

	static void Copy(std::shared_ptr<VulkanBuffer> srcBufferPtr, std::shared_ptr<VulkanBuffer> dstBuffer, uint64_t size);

protected:
	std::shared_ptr<VulkanDevice>   m_vulkanDeviePtr;
	VkBuffer                        m_vulkanBufferHandle;
	VkDeviceMemory                  m_vulkanBufferMemory;
	uint64_t                        m_size;
};

