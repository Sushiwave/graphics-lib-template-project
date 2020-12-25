#pragma once
#include "RenderPipelineSRTWithImGuiComponents.hpp"
#include "ShaderConstant.hpp"





class DepthRenderPipeline
	: public RenderPipelineSRTWithImGuiComponents
{
private:
	AdditionalSetCallScene m_additionalSetCall;
public:
	DepthRenderPipeline(const std::string& name, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, const cg::RasterizationBasedRenderPipeline::ShaderDict& shaderDict, AdditionalSetCallScene additionalSetCall = [](const cg::Scene& scene) {});
	DepthRenderPipeline(const std::string& name, const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, std::shared_ptr<cg::IDepthStencilTester> depthTesterLessFunction, std::shared_ptr<cg::TransformConstantBuffer> transformConstantBuffer, const cg::RasterizationBasedRenderPipeline::ShaderDict& shaderDict, AdditionalSetCallScene additionalSetCall = [](const cg::Scene& scene) {});
	virtual ~DepthRenderPipeline() = default;

	void render(const cg::Scene& scene, const cg::Camera& customCamera);
	void render(const cg::Scene& scene) override;
	void render() override;

	void drawImGuiComponents() override;
};





#define DEFINE_Z_RENDER_PIPELINE_(NAME)\
	class NAME##DepthRenderPipeline\
		: public DepthRenderPipeline\
	{\
	public:\
		NAME##DepthRenderPipeline(const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer)\
			: DepthRenderPipeline("Depth Render Pipeline",\
			  targetRenderingGroupNameList,\
              depthStencilBuffer,\
			  std::make_shared<cg::TransformConstantBuffer>\
              (\
				  cg::TransformConstantBuffer::BufferDict\
				  {\
					  {\
					      cg::ShaderStage::vs,\
\
						  std::make_shared<cg::TransformConstantBuffer::ElementBuffer>\
						  (\
							  cg::TransformConstantBuffer::ElementBuffer::constructor<constant::TransformWVP>\
							  (\
								  [](constant::TransformWVP& data, const cg::Scene& s, const cg::Transform& t, const cg::Shape& sh, const cg::Camera& c)\
								  {\
									  cg::TransformConstantBufferHelper::storeWVP(&data.wvp, t, sh, c);\
								  }\
							  )\
						  )\
					  }\
				  }\
              ),\
              { { cg::ShaderStage::ps, std::dynamic_pointer_cast<cg::IVertexShader>(cg::ShaderPool::shared.createFromFileAndAdd(cg::ShaderStage::vs, "DepthPass.vsh/vs_main_"#NAME"", "DepthPass.vsh", "vs_main_"#NAME"", "vs_4_0")) } })\
		{\
		}\
	}

DEFINE_Z_RENDER_PIPELINE_(Position3);
DEFINE_Z_RENDER_PIPELINE_(Position3Normal3);
DEFINE_Z_RENDER_PIPELINE_(Position3Normal4);
DEFINE_Z_RENDER_PIPELINE_(Position3UV2);
DEFINE_Z_RENDER_PIPELINE_(Position3UV2Normal3);
DEFINE_Z_RENDER_PIPELINE_(Position3UV2Normal4);

DEFINE_Z_RENDER_PIPELINE_(Position4);
DEFINE_Z_RENDER_PIPELINE_(Position4Normal3);
DEFINE_Z_RENDER_PIPELINE_(Position4Normal4);
DEFINE_Z_RENDER_PIPELINE_(Position4UV2);
DEFINE_Z_RENDER_PIPELINE_(Position4UV2Normal3);
DEFINE_Z_RENDER_PIPELINE_(Position4UV2Normal4);