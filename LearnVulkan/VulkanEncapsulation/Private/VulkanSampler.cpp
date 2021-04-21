#include "VulkanSampler.h"
#include "VulkanDevice.h"

#include <assert.h>

VulkanSampler::VulkanSampler(std::shared_ptr<VulkanDevice> vulkanDevicePtr)
	:m_vulkanDevicePtr(vulkanDevicePtr)
{
	VkSamplerCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	createInfo.magFilter = VK_FILTER_LINEAR;
	createInfo.minFilter = VK_FILTER_LINEAR;
	createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.anisotropyEnable = VK_TRUE;
	createInfo.maxAnisotropy = 16;
	createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	createInfo.compareEnable = VK_FALSE;
	createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	createInfo.flags = 0;
	createInfo.maxLod = 0.0f;
	createInfo.mipLodBias = 0.0f;
	createInfo.minLod = 0.0f;
	createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	auto ret = vkCreateSampler(m_vulkanDevicePtr->getHandle(), &createInfo, nullptr, &m_vulkanSamplerHandle);
	assert(ret == VK_SUCCESS);
}

VulkanSampler::~VulkanSampler()
{
	vkDestroySampler(m_vulkanDevicePtr->getHandle(), m_vulkanSamplerHandle, nullptr);
}

VkSampler VulkanSampler::getHandle()
{
	return m_vulkanSamplerHandle;
}
