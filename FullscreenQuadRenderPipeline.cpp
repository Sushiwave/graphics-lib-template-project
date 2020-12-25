#include "FullscreenQuadRenderPipeline.hpp"





FullscreenQuadRenderPipeline::FullscreenQuadRenderPipeline(const std::string& name, std::shared_ptr<cg::IRenderTarget> renderTarget, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, std::shared_ptr<cg::LightConstantBuffer> lightConstantBuffer, std::shared_ptr<cg::IPixelShader> pixelShader, const AdditionalSetCall& setCallPixelShaderResource, const FullscreenQuad& quad)
	: RenderPipelineSRTWithImGuiComponents
	  (
	      name, 
		  { "" },
		  renderTarget, 
		  nullptr,
		  nullptr,
		  cg::API::shared.graphics()->createRasterizer(),
		  nullptr,
		  {
			  { cg::ShaderStage::vs, std::dynamic_pointer_cast<cg::IVertexShader>(cg::ShaderPool::shared.createFromFileAndAdd(cg::ShaderStage::vs, "FullscreenQuad.vsh", "FullscreenQuad.vsh", "vs_main", "vs_5_0")) },
			  { cg::ShaderStage::ps, pixelShader }
		  },
		  nullptr,
		  transformConstantBuffer, 
		  lightConstantBuffer
	  ),
	  m_setCall
	  (
	      [=]()
	      {
	          renderTarget->set(nullptr);
			  setCallPixelShaderResource();
	      }
	  ),
	  m_drawCall
	  (
		  [=]()
		  {
			  auto part = quad->geometry.parts.get("main");
			  part.getGeometryBuffer()->draw(part.primitiveTopology, quad->instanceCount);
		  }
	  )
{
}

void FullscreenQuadRenderPipeline::render()
{
	accessToRenderTarget()->refresh();
	renderDefault(m_setCall, m_drawCall);
}

void FullscreenQuadRenderPipeline::render(const cg::Scene& scene)
{
	accessToRenderTarget()->refresh();
	renderDefault(scene, false, [&](const cg::Scene&) { m_setCall(); }, m_drawCall);
}

void FullscreenQuadRenderPipeline::drawImGuiComponents()
{
}
