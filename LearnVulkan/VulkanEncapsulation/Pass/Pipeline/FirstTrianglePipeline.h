#pragma once
#include "Private/VulkanGraphicPipelineBase.h"

class FirstTrianglePipeline :public VulkanGraphicPipelineBase
{
public:
	virtual void initShaderStageInfo() override;
	virtual void initVertexInputBindingDesciption() override;
	virtual void initVertexInputAttributeDescription()override;
private:
};