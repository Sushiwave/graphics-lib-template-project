#include "RenderPipelineMRTWithImGuiComponents.hpp"





RenderPipelineMRTWithImGuiComponents::RenderPipelineMRTWithImGuiComponents(const std::string& name, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::IMultipleRenderTargets> mrt, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, std::shared_ptr<cg::MaterialConstantBuffer> materialConstantBuffer, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, std::shared_ptr<cg::LightConstantBuffer> lightConstantBuffer, std::shared_ptr<cg::IDepthStencilTester> depthStencilTester, const ShaderDict& shaders) noexcept
	: RenderPipelineMRT(name, targetRenderingGroupNameList, mrt, depthStencilBuffer, materialConstantBuffer, transformConstantBuffer, lightConstantBuffer, depthStencilTester, shaders)
{
}
