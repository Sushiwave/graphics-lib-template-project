#include "GeometryRenderPipeline.hpp"





GeometryRenderPipeline::GeometryRenderPipeline(const std::string& name, std::shared_ptr<cg::IMultipleRenderTargets> mrt, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::MaterialConstantBuffer> materialConstantBuffer, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, const cg::RasterizationBasedRenderPipeline::ShaderDict& shaderDict, AdditionalSetCallScene additionalSetCall)
	: GeometryRenderPipeline(name, mrt, depthStencilBuffer, targetRenderingGroupNameList, materialConstantBuffer, transformConstantBuffer, cg::API::shared.graphics()->createDepthStencilTesterState(cg::ComparisonFunction::less, cg::ComparisonFunction::always, true, false, true), shaderDict, additionalSetCall)
{
}

GeometryRenderPipeline::GeometryRenderPipeline(const std::string& name, std::shared_ptr<cg::IMultipleRenderTargets> mrt, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::MaterialConstantBuffer> materialConstantBuffer, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, std::shared_ptr<cg::IDepthStencilTesterState> depthTesterLessFunction, const cg::RasterizationBasedRenderPipeline::ShaderDict& shaderDict, AdditionalSetCallScene additionalSetCall)
	: RenderPipelineMRTWithImGuiComponents(name, targetRenderingGroupNameList, mrt, depthStencilBuffer, depthTesterLessFunction, cg::API::shared.graphics()->createRasterizerState(cg::CullMode::back, false, false, false, false, false, false), nullptr, shaderDict, materialConstantBuffer, transformConstantBuffer, nullptr),
	  m_additionalSetCall(additionalSetCall)
{
}

void GeometryRenderPipeline::render(const cg::Scene& scene, const cg::Camera& customCamera)
{
	const auto multipleRenderTargets = accessToMultipleRenderTargets();
	const auto depthStencilBuffer = accessToDepthStencilBuffer();

	multipleRenderTargets->refreshAll();
	depthStencilBuffer->refresh();

	renderDefault(scene, customCamera, true, [&](const cg::Scene&) { multipleRenderTargets->set(depthStencilBuffer); m_additionalSetCall(scene); }, []() {});
}

void GeometryRenderPipeline::render(const cg::Scene& scene)
{
	render(scene, scene.camera);
}

void GeometryRenderPipeline::render()
{

}

void GeometryRenderPipeline::drawImGuiComponents()
{
}
