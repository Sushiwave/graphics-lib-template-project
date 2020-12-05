#include "GeometryRenderPipeline.hpp"





GeometryRenderPipeline::GeometryRenderPipeline(const std::string& name, const cg::GBuffer& gbuffer, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::MaterialConstantBuffer> materialConstantBuffer, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, const cg::RasterizationBasedRenderPipeline::ShaderDict& shaderDict, AdditionalSetCallScene additionalSetCall)
	: GeometryRenderPipeline(name, gbuffer, targetRenderingGroupNameList, materialConstantBuffer, transformConstantBuffer, cg::API::shared.graphics()->createDepthStencilTester(cg::ComparisonFunction::less, cg::ComparisonFunction::always, true, false, true), shaderDict, additionalSetCall)
{
}

GeometryRenderPipeline::GeometryRenderPipeline(const std::string& name, const cg::GBuffer& gbuffer, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::MaterialConstantBuffer> materialConstantBuffer, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, std::shared_ptr<cg::IDepthStencilTester> depthTesterLessFunction, const cg::RasterizationBasedRenderPipeline::ShaderDict& shaderDict, AdditionalSetCallScene additionalSetCall)
	: RenderPipelineMRTWithImGuiComponents(name, targetRenderingGroupNameList, nullptr, nullptr, materialConstantBuffer, transformConstantBuffer, nullptr, depthTesterLessFunction, shaderDict),
	  m_additionalSetCall(additionalSetCall)
{
	m_GBuffer = gbuffer;
}

void GeometryRenderPipeline::render(const cg::Scene& scene, const cg::Camera& customCamera)
{
	m_multipleRenderTarget->refreshAll();
	m_depthStencilBuffer->refresh();

	renderDefault(scene, customCamera, [&](const cg::Scene&) { m_multipleRenderTarget->set(m_depthStencilBuffer); m_additionalSetCall(scene); }, []() {}, true);
}

void GeometryRenderPipeline::render(const cg::Scene& scene)
{
	render(scene, scene.camera);
}

void GeometryRenderPipeline::render()
{

}

void GeometryRenderPipeline::initializeMultipleRenderTarget(std::shared_ptr<cg::IMultipleRenderTarget> mrt)
{
	m_multipleRenderTarget = mrt;
}

void GeometryRenderPipeline::initializeDepthStencilBuffer(std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer)
{
	m_depthStencilBuffer = depthStencilBuffer;
}

void GeometryRenderPipeline::drawImGuiComponents()
{
	if (ImGui::TreeNode("GBuffer"))
	{
		std::string bufferName = "";
		const auto contents = m_GBuffer.getAllContents();

		for (const auto content : contents)
		{
			if (ImGui::Button(content.name.c_str()))
			{
				bufferName = content.name;
			}
		}
		if (bufferName == "") { return; }

		const auto image = m_GBuffer.get(bufferName);
		const auto imageSize = image->getSize()/5;
		ImGui::Image(image, ImVec2(imageSize.x, imageSize.y));

		ImGui::TreePop();
	}
}
