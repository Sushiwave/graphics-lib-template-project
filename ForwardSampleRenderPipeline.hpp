#pragma once
#include "SampleShadingRenderPipeline.hpp"
#include "ForwardRenderPipeline.hpp"
#include "DepthPass.hpp"





class ForwardSampleRenderPipeline
	: public ForwardRenderPipeline
{
private:
	std::shared_ptr<cg::IDepthStencilBuffer> m_shadowMap;
	std::shared_ptr<cg::ITextureSampler> m_shadowMapSampler;

	DepthPass m_shadowMapRenderingPass;
	SampleShadingRenderPipeline m_shadingRenderPipeline;
	
	bool m_shouldRefreshRenderTarget;
	bool m_shouldRefreshDepthStencilBuffer;
public:
	static const std::string targetRenderingGroupName;
private:
	ForwardSampleRenderPipeline(std::shared_ptr<cg::IRenderTarget> renderTarget, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, std::shared_ptr<cg::IDepthStencilTester> depthStencilTester, bool shouldRefreshRenderTarget, bool shouldRefreshDepthStencilBuffer);
public:
	ForwardSampleRenderPipeline(std::shared_ptr<cg::IRenderTarget> renderTarget);
	ForwardSampleRenderPipeline(std::shared_ptr<cg::IRenderTarget> renderTarget, std::shared_ptr<cg::IDepthStencilTester> depthStencilTester, bool shouldRefreshRenderTarget = true, bool shouldRefreshDepthStencilBuffer = true);
	virtual ~ForwardSampleRenderPipeline() = default;

	void render(const cg::Scene& scene) override;
	void render() override;

	void drawImGuiComponents() override;
};
