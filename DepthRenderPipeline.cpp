#include "DepthRenderPipeline.hpp"





DepthRenderPipeline::DepthRenderPipeline(const std::string& name, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, const cg::RasterizationBasedRenderPipeline::ShaderDict& shaderDict, AdditionalSetCallScene additionalSetCall)
	: DepthRenderPipeline(name, targetRenderingGroupNameList, depthStencilBuffer, cg::API::shared.graphics()->createDepthStencilTesterState(cg::ComparisonFunction::less, cg::ComparisonFunction::always, true, false, true), transformConstantBuffer, shaderDict, additionalSetCall)
{
}

DepthRenderPipeline::DepthRenderPipeline(const std::string& name, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, std::shared_ptr<cg::IDepthStencilTesterState> depthTesterLessFunction, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, const cg::RasterizationBasedRenderPipeline::ShaderDict& shaderDict, AdditionalSetCallScene additionalSetCall)
	: RenderPipelineSRTWithImGuiComponents(name, targetRenderingGroupNameList, cg::MainRenderTarget::shared.get(), depthStencilBuffer, depthTesterLessFunction, nullptr, nullptr, shaderDict, nullptr, transformConstantBuffer, nullptr),
	  m_additionalSetCall(additionalSetCall)
{
}

void DepthRenderPipeline::render(const cg::Scene& scene, const cg::Camera& customCamera)
{
	const auto renderTarget = accessToRenderTarget();
	const auto depthStencilBuffer = accessToDepthStencilBuffer();
	renderDefault(scene, customCamera, true, [&](const cg::Scene&){ renderTarget->set(depthStencilBuffer); m_additionalSetCall(scene); }, [](){});
}

void DepthRenderPipeline::render(const cg::Scene& scene)
{
	renderDefault(scene, scene.camera, true);
}

void DepthRenderPipeline::render()
{
}

void DepthRenderPipeline::drawImGuiComponents()
{
	const auto depthStencilBuffer = accessToDepthStencilBuffer();
	const auto depthTexture = depthStencilBuffer->getDepthBufferTexture();
	const auto textureSize = depthTexture->getSize();
	const auto aspect = textureSize.y/textureSize.x;
	const auto displayingSize = cpp::Vector2D<float>(1.0, aspect)*500.0;
	ImGui::ImageDepth(depthStencilBuffer, { displayingSize.x, displayingSize.y });
}
