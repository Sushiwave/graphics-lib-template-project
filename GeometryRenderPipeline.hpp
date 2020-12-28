#pragma once
#include "RenderPipelineMRTWithImGuiComponents.hpp"
#include "ShaderConstant.hpp"





class GeometryRenderPipeline
	: public RenderPipelineMRTWithImGuiComponents
{
private:
	AdditionalSetCallScene m_additionalSetCall;
public:
	GeometryRenderPipeline(const std::string& name, std::shared_ptr<cg::IMultipleRenderTargets> multipleRenderTargets, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::MaterialConstantBuffer> materialConstantBuffer, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, const cg::RasterizationBasedRenderPipeline::ShaderDict& shaderDict, AdditionalSetCallScene additionalSetCall = [](const cg::Scene& scene) {});
	GeometryRenderPipeline(const std::string& name, std::shared_ptr<cg::IMultipleRenderTargets> multipleRenderTargets, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::MaterialConstantBuffer> materialConstantBuffer, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, std::shared_ptr<cg::IDepthStencilTesterState> depthTesterLessFunction, const cg::RasterizationBasedRenderPipeline::ShaderDict& shaderDict, AdditionalSetCallScene additionalSetCall = [](const cg::Scene& scene) {});
	virtual ~GeometryRenderPipeline() = default;

	void render(const cg::Scene& scene, const cg::Camera& customCamera);
	void render(const cg::Scene& scene) override;
	void render() override;

	void drawImGuiComponents() override;
};
