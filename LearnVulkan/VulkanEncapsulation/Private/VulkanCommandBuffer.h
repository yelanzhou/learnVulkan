#pragma once
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

class VulkanDevice;
class VulkanCommandPool;
class VulkanRenderPass;
class VulkanFramebuffer;
class VulkanPipeline;
class VulkanBuffer;
class VulkanPipelineLayout;
class VulkanDescriptorSet;


class VulkanCommandBuffer
{
public:
	VulkanCommandBuffer(std::shared_ptr<VulkanDevice> vulkanDevicePtr, std::shared_ptr<VulkanCommandPool> vulkanCommandPoolPtr);
	~VulkanCommandBuffer();
	VkCommandBuffer& getHandle();

	void beginCommandBuffer(bool isUseOneTime);

	void pushRenderPassBeginCmd(std::shared_ptr<VulkanRenderPass> vulkanRenderPassPtr, std::shared_ptr<VulkanFramebuffer> vulkanFramebufferPtr, VkExtent2D extent);
	void pushRenderPassEndCmd();

	void pushBindPipelineCmd(std::shared_ptr<VulkanPipeline> vulkanPipelinePtr);

	void pushBindVertexBuffersCmd(uint32_t bindingPoint, uint32_t bindingCount, std::shared_ptr<VulkanBuffer> vulkanBufferPtr, std::vector<uint64_t> offsets);

	void pushBindIndexBufferCmd(std::shared_ptr<VulkanBuffer> vulkanBufferPtr, uint64_t offset, VkIndexType type);

	void pushBindDescriptorSetsCmd(std::shared_ptr<VulkanPipelineLayout> vulkanPipelineLayoutPtr, uint32_t firstSet, std::vector<std::shared_ptr<VulkanDescriptorSet>> vulkanDesSetPtrs);

	void pushDrawCmd(uint32_t vertexCount);

	void pushDrawIndexedCmd(uint32_t indexCount);


	void endCommandBuffer();
private:
	std::shared_ptr<VulkanDevice>     m_vulkanDevicePtr;
	std::shared_ptr<VulkanCommandPool> m_vulkanCommandPoolPtr;
	VkCommandBuffer               m_vulkanCommandBufferHandle;
	
};

