#pragma once
#include "RenderPipelineSRTWithImGuiComponents.hpp"





class SampleShadingRenderPipeline
	: public RenderPipelineSRTWithImGuiComponents
{
private:
	std::shared_ptr<cg::IDepthStencilBuffer> m_shadowMap;
	std::shared_ptr<cg::ITextureSampler> m_shadowMapSampler;
public:
	SampleShadingRenderPipeline(const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, std::shared_ptr<cg::IDepthStencilTesterState> depthStencilTester, std::shared_ptr<cg::IDepthStencilBuffer> shadowMap, std::shared_ptr<cg::ITextureSampler> shadowMapSampler);
	SampleShadingRenderPipeline(const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::IRenderTarget> renderTarget, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, std::shared_ptr<cg::IDepthStencilTesterState> depthStencilTester, std::shared_ptr<cg::IDepthStencilBuffer> shadowMap, std::shared_ptr<cg::ITextureSampler> shadowMapSampler);

	void render(const cg::Scene& scene) override;
	void render() override;

	void drawImGuiComponents() override;
};
