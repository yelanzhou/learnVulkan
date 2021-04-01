#pragma once

#include <vulkan/vulkan.h>
#include <memory>
class VulkanDevice;

class VulkanImageView
{
public:
	VulkanImageView(std::shared_ptr< VulkanDevice>  vulkanDevicePtr,VkImage imageHanle,VkFormat format);
	~VulkanImageView();


	VkImageView getHandle();


private:


	std::shared_ptr< VulkanDevice>  m_vulkanDevicePtr;
	VkImageView m_vulkanImageViewHandle;


};

