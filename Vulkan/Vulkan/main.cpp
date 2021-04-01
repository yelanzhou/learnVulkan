// Vulkan.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#include "VulkanGlobal.h"
#include "VulkanSurface.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanGpuManager.h"
#include "VulkanGpu.h"
#include "VulkanSwapChainKHR.h"
#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"
#include "VertexData.h"
#include "VulkanHostVisibleVertexBuffer.h"
#include "VulkanDeviceLocalVertexBuffer.h"
#include "VulkanHostVisibleIndexBuffer.h"
#include "VulkanDeviceLocalIndexBuffer.h"


#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFence.h"
#include "VulkanSemaphore.h"
#include "VulkanQueue.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanPipelineLayout.h"
#include "VulkanUniformBuffer.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"
#include "ImageData.h"
#include "VulkanDeviceLocalImage.h"
#include "VulkanImageHostVisibleBuffer.h"


int main()
{

    const std::vector<VertexData> vertices = {
          {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
     0, 1, 2, 2, 3, 0
    };

    UniformData data;

    auto imageData = std::make_shared<ImageData>("../../textures/statue.jpg");

    const int maxImageInLlight = 2;
 
    VulkanGlobal::getSingleton().init();
    auto surfacePtr = VulkanGlobal::getSingleton().getVulkanSurface();
    auto devicePtr = VulkanGlobal::getSingleton().getVulkanDevice();
    auto swapChainPtr = VulkanGlobal::getSingleton().getVulkanSwapChain();
    auto gpu = VulkanGlobal::getSingleton().getGpuManager()->getSuitableGpu();

    std::shared_ptr<std::vector<VkDescriptorSetLayoutBinding>> binds = std::make_shared<std::vector<VkDescriptorSetLayoutBinding>>(2);

    binds->at(0).binding = 0;
    binds->at(0).descriptorCount = 1;
    binds->at(0).descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binds->at(0).pImmutableSamplers = nullptr;
    binds->at(0).stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    binds->at(1).binding = 1;
    binds->at(1).descriptorCount = 1;
    binds->at(1).descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    binds->at(1).pImmutableSamplers = nullptr;
    binds->at(1).stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;


    auto descriptorSetLayoutPtr = std::make_shared<VulkanDescriptorSetLayout>(devicePtr,binds);

    auto pipelineLayoutPtr = std::make_shared<VulkanPipelineLayout>(devicePtr, descriptorSetLayoutPtr);

    auto renderPassPtr = std::make_shared<VulkanRenderPass>(devicePtr, swapChainPtr);

    auto pipelinePtr = std::make_shared<VulkanPipeline>(devicePtr, swapChainPtr,renderPassPtr, pipelineLayoutPtr);

    std::vector<std::shared_ptr<VulkanFramebuffer>> framebuffers;

    for (auto imageViews : swapChainPtr->getImageViews())
    {
        framebuffers.push_back(std::make_shared<VulkanFramebuffer>(devicePtr,renderPassPtr,imageViews,swapChainPtr->getExtent()));
    }
        
    //vertex
    std::shared_ptr<VulkanHostVisibleVertexBuffer> vertexBufferPtr = std::make_shared<VulkanHostVisibleVertexBuffer>(devicePtr, gpu, vertices);
    uint64_t bufferSize = static_cast<uint64_t>(sizeof(VertexData)) * vertices.size();
    std::shared_ptr<VulkanDeviceLocalVertexBuffer> localDeviceVertexPtr = std::make_shared<VulkanDeviceLocalVertexBuffer>(devicePtr, gpu, bufferSize);
    VulkanBuffer::Copy(vertexBufferPtr, localDeviceVertexPtr, bufferSize);


    //index 
    auto indexBufferPtr = std::make_shared<VulkanHostVisibleIndexBuffer>(devicePtr, gpu, indices);
    bufferSize = static_cast<uint16_t>(sizeof(uint16_t) * indices.size());
    auto deviceLocalIndexBufferPtr = std::make_shared<VulkanDeviceLocalIndexBuffer>(devicePtr, gpu, bufferSize);
    VulkanBuffer::Copy(indexBufferPtr, deviceLocalIndexBufferPtr, bufferSize);

    auto commandPoolPtr = std::make_shared<VulkanCommandPool>(devicePtr, devicePtr->getGraphicQueue()->getFamilyIndex());


    auto uniformBufferPtrs = std::vector<std::shared_ptr<VulkanUniformBuffer>>(swapChainPtr->getImageViews().size(), std::make_shared<VulkanUniformBuffer>(devicePtr, gpu, data));

    auto descriptorSetPoolPtr = std::make_shared<VulkanDescriptorPool>(devicePtr, descriptorSetLayoutPtr,swapChainPtr->getImageViews().size());

    std::vector<std::shared_ptr<VulkanDescriptorSet>>  descriptorSetPtrs;
    for (auto uniformBufferPtr : uniformBufferPtrs)
    {
        descriptorSetPtrs.push_back(std::make_shared<VulkanDescriptorSet>(devicePtr, descriptorSetPoolPtr,descriptorSetLayoutPtr));
       
    }

    std::vector<std::shared_ptr<VulkanCommandBuffer>> drawCommandBuffers;

    for (int i = 0 ; i < framebuffers.size(); ++i)
    {
        //push dat to set
        descriptorSetPtrs[i]->update(uniformBufferPtrs[i]);

        auto commandBufferPtr = std::make_shared<VulkanCommandBuffer>(devicePtr, commandPoolPtr);
        drawCommandBuffers.push_back(commandBufferPtr);
        commandBufferPtr->beginCommandBuffer(false);
        commandBufferPtr->pushRenderPassBeginCmd(renderPassPtr, framebuffers[i], swapChainPtr->getExtent());
        commandBufferPtr->pushBindPipelineCmd(pipelinePtr);
        commandBufferPtr->pushBindVertexBuffersCmd(0, 1, localDeviceVertexPtr, { 0 });
        commandBufferPtr->pushBindIndexBufferCmd(deviceLocalIndexBufferPtr, 0, VK_INDEX_TYPE_UINT16);
        commandBufferPtr->pushBindDescriptorSetsCmd(pipelineLayoutPtr, 0, { descriptorSetPtrs[i] });
        commandBufferPtr->pushDrawIndexedCmd(indices.size());
        commandBufferPtr->pushRenderPassEndCmd();
        commandBufferPtr->endCommandBuffer();

    }


    std::vector<std::shared_ptr<VulkanFence>> imageInflightFences(drawCommandBuffers.size());
    std::vector<std::shared_ptr<VulkanFence>> fences(maxImageInLlight,std::make_shared<VulkanFence>(devicePtr));
    std::vector<std::shared_ptr<VulkanSemaphore>> imageAvaliabelSemaphore(maxImageInLlight,std::make_shared<VulkanSemaphore>(devicePtr));
    std::vector<std::shared_ptr<VulkanSemaphore>> imageRenderFinishSemaphore(maxImageInLlight,std::make_shared<VulkanSemaphore>(devicePtr));

    size_t currentFrame = 0;

    auto imageBufferPtr = std::make_shared<VulkanImageHostVisibleBuffer>(devicePtr, gpu, imageData);
    auto imagePtr = std::make_shared<VulkanDeviceLocalImage>(devicePtr, gpu, imageData);
    imagePtr->setImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    VulkanDeviceLocalImage::copyBufferToImage(imageBufferPtr, imagePtr);
    imagePtr->setImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    while (VulkanGlobal::getSingleton().renderLoop()) {

        fences[currentFrame]->wait();

        auto index = swapChainPtr->getNextImageForDrawing(imageAvaliabelSemaphore[currentFrame]);

        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        
        data.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        data.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        data.proj = glm::perspective(glm::radians(45.0f),800 / (float)600, 0.1f, 10.0f);
        data.proj[1][1] *= -1;

        uniformBufferPtrs[index]->updateData(data);
       

        if (imageInflightFences[index] != nullptr)
        {
            imageInflightFences[index]->wait();
        }

        imageInflightFences[index] = fences[currentFrame];

        std::vector<std::shared_ptr<VulkanSemaphore>> waitSeamphore = { imageAvaliabelSemaphore[currentFrame] };
        std::vector<std::shared_ptr<VulkanSemaphore>> signalSeamphore = { imageRenderFinishSemaphore[currentFrame] };
        fences[currentFrame]->reset();

        devicePtr->getGraphicQueue()->submit(drawCommandBuffers[index], waitSeamphore, signalSeamphore, fences[currentFrame], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        swapChainPtr->presentKHR(imageRenderFinishSemaphore[currentFrame], devicePtr->getPresentQueue(), index);

        currentFrame = (currentFrame + 1) % maxImageInLlight;
        glfwPollEvents();
    }

   // devicePtr->getGraphicQueue()->waitIdle();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
