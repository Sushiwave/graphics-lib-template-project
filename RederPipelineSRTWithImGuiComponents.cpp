#include "RenderPipelineSRTWithImGuiComponents.hpp"





RenderPipelineSRTWithImGuiComponents::RenderPipelineSRTWithImGuiComponents(const std::string& name, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::IRenderTarget> renderTarget, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, std::shared_ptr<cg::IDepthStencilTesterState> depthStencilTester, std::shared_ptr<cg::IRasterizerState> rasterizer, std::shared_ptr<cg::IAlphaBlendState> alphaBlender, const ShaderDict& shaders, std::shared_ptr<cg::MaterialConstantBuffer> materialConstantBuffer, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, std::shared_ptr<cg::LightConstantBuffer> lightConstantBuffer) noexcept
	: RenderPipelineSRT(name, targetRenderingGroupNameList, renderTarget, depthStencilBuffer, depthStencilTester, rasterizer, alphaBlender, shaders,  materialConstantBuffer, transformConstantBuffer, lightConstantBuffer)
{
}
