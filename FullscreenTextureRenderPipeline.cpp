#include "FullscreenTextureRenderPipeline.hpp"





FullscreenTextureRenderPipeline::FullscreenTextureRenderPipeline(const std::string& name, std::shared_ptr<cg::ITexture2D> target, std::shared_ptr<cg::ITextureSampler> targetSampler)
	: FullscreenTextureRenderPipeline(name, target, targetSampler, cg::MainRenderTarget::shared.get(), FullscreenQuad())
{
}

FullscreenTextureRenderPipeline::FullscreenTextureRenderPipeline(const std::string& name, std::shared_ptr<cg::ITexture2D> target, std::shared_ptr<cg::ITextureSampler> targetSampler, std::shared_ptr<cg::IRenderTarget> renderTarget, const FullscreenQuad& quad)
	: FullscreenTextureRenderPipeline(name, target, targetSampler, renderTarget, quad, std::dynamic_pointer_cast<cg::IPixelShader>(cg::ShaderPool::shared.createShaderFromFileAndPool(cg::ShaderStage::ps, "FullscreenTexture.psh", "FullscreenTexture.psh", "ps_main", "ps_4_0")), [](){})
{
}

FullscreenTextureRenderPipeline::FullscreenTextureRenderPipeline(const std::string& name, std::shared_ptr<cg::ITexture2D> target, std::shared_ptr<cg::ITextureSampler> targetSampler, std::shared_ptr<cg::IPixelShader> customPixelShader, AdditionalSetCall additionalSetCall)
	: FullscreenTextureRenderPipeline(name, target, targetSampler, cg::MainRenderTarget::shared.get(), FullscreenQuad(), customPixelShader, additionalSetCall)
{
}

FullscreenTextureRenderPipeline::FullscreenTextureRenderPipeline(const std::string& name, std::shared_ptr<cg::ITexture2D> target, std::shared_ptr<cg::ITextureSampler> targetSampler, std::shared_ptr<cg::IRenderTarget> renderTarget, const FullscreenQuad& quad, std::shared_ptr<cg::IPixelShader> customPixelShader, AdditionalSetCall additionalSetCall)
	: FullscreenQuadRenderPipeline(name, 
		                           renderTarget,
		                           nullptr,
		                           nullptr,
		                           customPixelShader,
								   [&, additionalSetCall, customPixelShader]() 
		                           {
									   const auto locationDict = customPixelShader->getResourceLocationDict();
									   const auto targetBufferLocation = locationDict.at(cg::ShaderResourceType::Texture).at("target");
									   const auto targetSamplerBufferLocation = locationDict.at(cg::ShaderResourceType::TextureSampler).at("targetSampler");
								       m_target->set(cg::ShaderStage::ps, targetBufferLocation, cg::GPUAccessType::R);
									   m_targetSampler->set(cg::ShaderStage::ps, targetSamplerBufferLocation);
									
									   additionalSetCall();
								   },
		                           quad),
	  m_target(target),
	  m_targetSampler(targetSampler)
{
}

void FullscreenTextureRenderPipeline::changeTarget(std::shared_ptr<cg::ITexture2D> target)
{
	m_target = target;
}
