#pragma once
#include "DefferedRenderPipeline.hpp"
#include "FullscreenTextureRenderPipeline.hpp"
#include "FullscreenDepthTextureRenderPipeline.hpp"
#include "DepthPass.hpp"





class DefferedSampleRenderPipeline
	: public DefferedRenderPipeline
{
private:
	DepthPass m_shadowMapRenderingPass;
public:
	static const std::string targetRenderingGroupName;
private:
	DefferedSampleRenderPipeline(const std::shared_ptr<cg::IMultipleRenderTargets> geometryPassMRT, const std::shared_ptr<cg::IDepthStencilBuffer> geometryPassDepthStencilBuffer, std::shared_ptr<cg::IRenderTarget> lightingPassRenderTarget, std::shared_ptr<cg::ITextureSampler> gbufferSampler, std::shared_ptr<cg::IDepthStencilBuffer> shadowMap);
public:
	DefferedSampleRenderPipeline(std::shared_ptr<cg::IRenderTarget> lightingPassRenderTarget);
	DefferedSampleRenderPipeline(std::shared_ptr<cg::IRenderTarget> lightingPassRenderTarget, std::shared_ptr<cg::ITextureSampler> gbufferSampler);
	virtual ~DefferedSampleRenderPipeline() = default;

	void render(const cg::Scene& scene) override;
	void render() override;
	
	void drawImGuiComponents() override;
};
