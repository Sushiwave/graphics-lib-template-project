#include "SampleShadingRenderPipeline.hpp"
#include "ShaderConstant.hpp"
#include "SimplePointLight.hpp"
#include "SimpleDirectionalLight.hpp"





SampleShadingRenderPipeline::SampleShadingRenderPipeline(const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, std::shared_ptr<cg::IDepthStencilTesterState> depthStencilTester, std::shared_ptr<cg::IDepthStencilBuffer> shadowMap, std::shared_ptr<cg::ITextureSampler> shadowMapSampler)
	: SampleShadingRenderPipeline(targetRenderingGroupNameList, cg::MainRenderTarget::shared.get(), depthStencilBuffer, depthStencilTester, shadowMap, shadowMapSampler)
{
}

SampleShadingRenderPipeline::SampleShadingRenderPipeline(const TargetRenderingGroupNameList& targetRenderingGroupNameList, std::shared_ptr<cg::IRenderTarget> renderTarget, std::shared_ptr<cg::IDepthStencilBuffer> depthStencilBuffer, std::shared_ptr<cg::IDepthStencilTesterState> depthStencilTester, std::shared_ptr<cg::IDepthStencilBuffer> shadowMap, std::shared_ptr<cg::ITextureSampler> shadowMapSampler)
	: RenderPipelineSRTWithImGuiComponents
      (
		  "Sample Shading Render Pipeline",
		  targetRenderingGroupNameList,
		  renderTarget, 
		  depthStencilBuffer,
		  depthStencilTester,
		  cg::API::shared.graphics()->createRasterizerState(cg::CullMode::back, false, false, false, false, false, false),
          cg::API::shared.graphics()->createAlphaBlendState(cg::AlphaBlendDescriptor()),
		  {
			  {
				  cg::ShaderStage::vs,
				  std::dynamic_pointer_cast<cg::IVertexShader>(cg::ShaderPool::shared.createFromFileAndAdd(cg::ShaderStage::vs, "SamplePosition3Normal3.vsh", "SamplePosition3Normal3.vsh", "vs_main", "vs_4_0"))
			  },
			  {
				  cg::ShaderStage::ps,
				  std::dynamic_pointer_cast<cg::IPixelShader>(cg::ShaderPool::shared.createFromFileAndAdd(cg::ShaderStage::ps, "SampleShading.psh", "SampleShading.psh", "ps_main", "ps_4_0"))
			  }
		  },
		  std::make_shared<cg::MaterialConstantBuffer>
		  (
			  cg::MaterialConstantBuffer::BufferDict
			  { 
				  {
					  cg::ShaderStage::ps,
									
					  std::make_shared<cg::MaterialConstantBuffer::ElementBuffer>
					  (
						  cg::MaterialConstantBuffer::ElementBuffer(sizeof(constant::SimplePBRMaterial))
					  )
				  }
			  }
		  ), 
		  std::make_shared<cg::TransformConstantBuffer>
		  (
			  cg::TransformConstantBuffer::BufferDict
			  { 
				  {
					  cg::ShaderStage::vs, 

					  std::make_shared<cg::TransformConstantBuffer::ElementBuffer>
					  (
						  cg::TransformConstantBuffer::ElementBuffer::constructor<constant::TransformW_WVP_N_LWVP>
						  (
							  [](constant::TransformW_WVP_N_LWVP& data, const cg::Scene& s, const cg::Transform& t, const cg::Shape& sh, const cg::Camera& c)
							  {
								  cg::TransformConstantBufferHelper::storeW(&data.w, t, sh);
								  cg::TransformConstantBufferHelper::storeWVP(&data.wvp, t, sh, c);
								  cg::TransformConstantBufferHelper::storeN(&data.n, t, sh);

								  const auto keyLight = std::dynamic_pointer_cast<SimpleDirectionalLight>(s.makeLightDict().at("Key"));
								  cg::TransformConstantBufferHelper::storeWVP(&data.lwvp, t, keyLight->perspective);
							  }
						  )
					  ) 
				  } 
			  }
		  ),
		  std::make_shared<cg::LightConstantBuffer>
		  (
			  cg::LightConstantBuffer::BufferDict
			  { 
				  {
					  cg::ShaderStage::ps,
									
					  std::make_shared<cg::LightConstantBuffer::ElementBuffer>
					  (
						  cg::LightConstantBuffer::ElementBuffer::constructor<constant::SimpleLight>
						  (
							  [&](constant::SimpleLight& data, const cg::Scene& scene)
							  {
								  auto pointLights = scene.getLights(SimplePointLight::type);
								  auto pointLightSrc = std::dynamic_pointer_cast<SimplePointLight>(scene.getLights(SimplePointLight::type).at("Back"));
								  auto& pointLightDest = data.pointLights[0];
								  pointLightDest = pointLightSrc->getConstant().get<constant::SimplePointLight>();
								  cpp::assignVector3DToArray4(&pointLightDest.position, pointLightSrc->transform->calcPositionWorld());
												
								  auto directionalLightSrc = std::dynamic_pointer_cast<SimpleDirectionalLight>(scene.getLights(SimpleDirectionalLight::type).at("Key"));
								  auto& directionalLightDest = data.directionalLight;
								  directionalLightDest = directionalLightSrc->getConstant().get<constant::SimpleDirectionalLight>();
								  cpp::assignVector3DToArray4(&directionalLightDest.direction, directionalLightSrc->perspective.transform->calcForwardWorld());
										
								  cpp::assignVector3DToArray4(&data.cameraPosition, scene.camera.transform->calcPositionLocal());
							  }
						  )
					  )
				  }
			  }
		  )
      ),
	  m_shadowMap(shadowMap),
	  m_shadowMapSampler(shadowMapSampler)
{
}

void SampleShadingRenderPipeline::render(const cg::Scene& scene)
{
	const auto pixelShaderLocationDict = getDictOfShadersSetInPipeline().at(cg::ShaderStage::ps)->getResourceLocationDict();
	const auto renderTarget = accessToRenderTarget();
	const auto depthStencilBuffer = accessToDepthStencilBuffer();
	auto setCall = [&](const cg::Scene scene)
	{
		renderTarget->set(depthStencilBuffer);

		m_shadowMap->getDepthBufferTexture()->set(cg::ShaderStage::ps, pixelShaderLocationDict.at(cg::ShaderResourceType::Texture).at("shadowMap"), cg::GPUAccessType::R);
		m_shadowMapSampler->set(cg::ShaderStage::ps, pixelShaderLocationDict.at(cg::ShaderResourceType::TextureSampler).at("shadowMapSampler"));
	};
	auto drawCall = [&]()
	{
	};
	renderDefault(scene, true, setCall, drawCall);
}

void SampleShadingRenderPipeline::render()
{
}

void SampleShadingRenderPipeline::drawImGuiComponents()
{
}