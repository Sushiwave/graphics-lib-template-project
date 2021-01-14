#include "FullscreenDepthTextureOrthographicRenderPipeline.hpp"






FullscreenDepthTextureOrthographicRenderPipeline::FullscreenDepthTextureOrthographicRenderPipeline(const std::string& name, std::shared_ptr<cg::IDepthStencilBuffer> target, std::shared_ptr<cg::ITextureSampler> targetSampler)
	: FullscreenDepthTextureOrthographicRenderPipeline(name, target, targetSampler, cg::MainRenderTarget::shared.get(), FullscreenQuad())
{
}

FullscreenDepthTextureOrthographicRenderPipeline::FullscreenDepthTextureOrthographicRenderPipeline(const std::string& name, std::shared_ptr<cg::IDepthStencilBuffer> target, std::shared_ptr<cg::ITextureSampler> targetSampler, std::shared_ptr<cg::IRenderTarget> renderTarget, const FullscreenQuad& quad)
	: FullscreenTextureRenderPipeline(name,
 									  target->getDepthBufferTexture(),
		                              targetSampler,
									  renderTarget,
									  quad,
									  std::dynamic_pointer_cast<cg::IPixelShader>(cg::ShaderPool::shared.createShaderFromFileAndPool(cg::ShaderStage::ps, "FullscreenDepthTextureOrthographic.psh", "FullscreenDepthTextureOrthographic.psh", "ps_main", "ps_4_0")))
{
}

void FullscreenDepthTextureOrthographicRenderPipeline::changeTarget(std::shared_ptr<cg::IDepthStencilBuffer> target)
{
	FullscreenTextureRenderPipeline::changeTarget(target->getDepthBufferTexture());
}

void FullscreenDepthTextureOrthographicRenderPipeline::render()
{
	FullscreenTextureRenderPipeline::render();
}
