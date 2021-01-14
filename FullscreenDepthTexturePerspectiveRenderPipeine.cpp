#include "FullscreenDepthTexturePerspectiveRenderPipeline.hpp"





FullscreenDepthTexturePerspectiveRenderPipeline::FullscreenDepthTexturePerspectiveRenderPipeline(const std::string& name, std::shared_ptr<cg::IDepthStencilBuffer> target, std::shared_ptr<cg::ITextureSampler> targetSampler)
	: FullscreenDepthTexturePerspectiveRenderPipeline(name, target, targetSampler, cg::MainRenderTarget::shared.get(), FullscreenQuad())
{
}

FullscreenDepthTexturePerspectiveRenderPipeline::FullscreenDepthTexturePerspectiveRenderPipeline(const std::string& name, std::shared_ptr<cg::IDepthStencilBuffer> target, std::shared_ptr<cg::ITextureSampler> targetSampler, std::shared_ptr<cg::IRenderTarget> renderTarget, const FullscreenQuad& quad)
	: FullscreenTextureRenderPipeline(name,
									  target->getDepthBufferTexture(),
		                              targetSampler,
									  renderTarget,
									  quad,
  									  std::dynamic_pointer_cast<cg::IPixelShader>(cg::ShaderPool::shared.createShaderFromFileAndPool(cg::ShaderStage::ps, "FullscreenDepthTexturePerspective.psh", "FullscreenDepthTexturePerspective.psh", "ps_main", "ps_4_0")),
									  [&]() { m_constantBuffer->set(cg::ShaderStage::ps, cg::ShaderPool::shared.get(cg::ShaderStage::ps, "FullscreenDepthTexturePerspective.psh")->getResourceLocationDict().at(cg::ShaderResourceType::ConstantBuffer).at("DepthLinearizer")); })
{
	m_constantBuffer = cg::API::shared.graphics()->createDynamicConstantBuffer(sizeof(constant::DepthLinearizer));
}

void FullscreenDepthTexturePerspectiveRenderPipeline::changeTarget(std::shared_ptr<cg::IDepthStencilBuffer> target)
{
	FullscreenTextureRenderPipeline::changeTarget(target->getDepthBufferTexture());
}

void FullscreenDepthTexturePerspectiveRenderPipeline::render(const cg::Projection& projection)
{
	m_constant.update(projection.getNearZ(), projection.getFarZ());
	m_constantBuffer->update(m_constant);
	FullscreenTextureRenderPipeline::render();
}

FullscreenDepthTexturePerspectiveRenderPipeline::DepthLinearizerConstant::DepthLinearizerConstant()
	: Constant(std::make_shared<constant::DepthLinearizer>())
{
}

void FullscreenDepthTexturePerspectiveRenderPipeline::DepthLinearizerConstant::update(float nearZ, float farZ)
{
	tweak<constant::DepthLinearizer>([=](constant::DepthLinearizer& depthLinearizer)
	{
		depthLinearizer.nearZ = nearZ;
		depthLinearizer.farZ[0] = farZ;
	});
}
