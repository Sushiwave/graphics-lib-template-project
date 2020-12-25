#include "DefferedSampleRenderPipeline.hpp"
#include "SimpleDirectionalLight.hpp"
#include "SimplePointLight.hpp"
#include "SampleGeometryRenderPipeline.hpp"
#include "SampleGBuffer.hpp"




const std::string DefferedSampleRenderPipeline::targetRenderingGroupName = "Sample";
const cg::RasterizationBasedRenderPipeline::TargetRenderingGroupNameList targetRenderingGroupNameList = { DefferedSampleRenderPipeline::targetRenderingGroupName };





DefferedSampleRenderPipeline::DefferedSampleRenderPipeline(const std::shared_ptr<cg::IMultipleRenderTargets> geometryPassMRT, const std::shared_ptr<cg::IDepthStencilBuffer> geometryPassDepthStencilBuffer, std::shared_ptr<cg::IRenderTarget> lightingPassRenderTarget, std::shared_ptr<cg::ITextureSampler> gbufferSampler, std::shared_ptr<cg::IDepthStencilBuffer> shadowMap)
	: DefferedRenderPipeline
	  (
		  "Deffered Sample Render Pipeline",
		  DefferedRenderPipeline::GeometryPass
		  (
			  "Sample Geometry Pass",
			  {
				  std::make_shared<SampleGeometryRenderPipeline>
				  (
					  geometryPassMRT,
					  geometryPassDepthStencilBuffer,
					  shadowMap,
					  targetRenderingGroupNameList
				  )
			  }
		  ),
		  DefferedRenderPipeline::LightingPass
		  (
			  "Sample Lighting Pass",
			  lightingPassRenderTarget,
			  std::make_shared<cg::TransformConstantBuffer>
			  (
				  cg::TransformConstantBuffer::BufferDict
				  {
					  {
						  cg::ShaderStage::ps,

						  std::make_shared<cg::TransformConstantBuffer::ElementBuffer>
						  (
							  cg::TransformConstantBuffer::ElementBuffer::constructor<constant::Transform_InvV_InvP_LVP>
							  (
								  [](constant::Transform_InvV_InvP_LVP& data, const cg::Scene& s, const cg::Transform& t, const cg::Shape& sh, const cg::Camera& c)
								  {
									  cg::TransformConstantBufferHelper::storeInvV(&data.invV, c);
									  cg::TransformConstantBufferHelper::storeInvP(&data.invP, c);
									
									  const auto keyLight = std::dynamic_pointer_cast<SimpleDirectionalLight>(s.makeLightDict().at("Key"));
									  cg::TransformConstantBufferHelper::storeVP(&data.lvp, keyLight->perspective);
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
		      ),
			  std::dynamic_pointer_cast<cg::IPixelShader>(cg::ShaderPool::shared.createFromFileAndAdd(cg::ShaderStage::ps, "SampleLightingPass.psh", "SampleLightingPass.psh", "ps_main", "ps_4_0")),
			  SampleGBuffer
			  (
				  geometryPassMRT->getRenderingResult(0),                  // Base Color + Roughness
				  geometryPassMRT->getRenderingResult(1),                  // Normal + Metalness
				  geometryPassMRT->getRenderingResult(2),                  // IOR
				  geometryPassDepthStencilBuffer->getDepthBufferTexture(), // Depth
				  shadowMap->getDepthBufferTexture()                       // Shadow Map
			  ),
			  gbufferSampler,
			  [=](const cg::GBuffer& gbuffer_, std::shared_ptr<cg::ITextureSampler> gbufferSampler_)
			  {
				  auto locationDict = cg::ShaderPool::shared.get(cg::ShaderStage::ps, "SampleLightingPass.psh")->getResourceLocationDict();
			      
				  auto gbufferLocationDict = locationDict.at(cg::ShaderResourceType::Texture);
				  gbuffer_.get(SampleGBuffer::BaseColorRoughness)->set(cg::ShaderStage::ps, gbufferLocationDict.at("baseColorRoughnessMap"), cg::GPUAccessType::R);
				  gbuffer_.get(SampleGBuffer::NormalMetalness)   ->set(cg::ShaderStage::ps, gbufferLocationDict.at("normalMetalnessMap"),    cg::GPUAccessType::R);
				  gbuffer_.get(SampleGBuffer::IOR)               ->set(cg::ShaderStage::ps, gbufferLocationDict.at("iorMap"),                cg::GPUAccessType::R);
				  gbuffer_.get(SampleGBuffer::Depth)             ->set(cg::ShaderStage::ps, gbufferLocationDict.at("depthMap"),              cg::GPUAccessType::R);
				  gbuffer_.get(SampleGBuffer::ShadowMap)         ->set(cg::ShaderStage::ps, gbufferLocationDict.at("shadowMap"),             cg::GPUAccessType::R);

				  gbufferSampler_->set(cg::ShaderStage::ps, locationDict.at(cg::ShaderResourceType::TextureSampler).at("gbufferSampler"));
			  }
		  )
	  ),
	  m_shadowMapRenderingPass
	  (
		  "Shadow Map Rendering Pass", 
		  {
			  std::make_shared<Position3Normal3DepthRenderPipeline>(targetRenderingGroupNameList, shadowMap)
		  },
		  shadowMap,
		  true
	  )
{
}

DefferedSampleRenderPipeline::DefferedSampleRenderPipeline(std::shared_ptr<cg::IRenderTarget> lightingPassRenderTarget)
	: DefferedSampleRenderPipeline(lightingPassRenderTarget, cg::API::shared.graphics()->createTextureSampler())
{
}

DefferedSampleRenderPipeline::DefferedSampleRenderPipeline(std::shared_ptr<cg::IRenderTarget> lightingPassRenderTarget, std::shared_ptr<cg::ITextureSampler> gbufferSampler)
	: DefferedSampleRenderPipeline
 	  (
		  cg::API::shared.graphics()->createMultipleRenderTargets
		  (
			  lightingPassRenderTarget->getSize(),
			  {
				  cg::TextureFormat::R32G32B32A32_FLOAT, // Base Color + Roughness
				  cg::TextureFormat::R32G32B32A32_FLOAT, // Normal + Metalness
				  cg::TextureFormat::R32_FLOAT           // IOR
			  }
		  ),
		  cg::API::shared.graphics()->createDepthStencilBuffer(lightingPassRenderTarget->getSize(), cg::TextureFormat::D32_FLOAT),
	      lightingPassRenderTarget,
		  gbufferSampler,
		  cg::API::shared.graphics()->createDepthStencilBuffer(lightingPassRenderTarget->getSize(), cg::TextureFormat::D32_FLOAT, lightingPassRenderTarget->getMSAASampleCount(), lightingPassRenderTarget->getMSAAQualityLevel())
	  )
{
}

void DefferedSampleRenderPipeline::render(const cg::Scene& scene)
{
	const auto keyLight = scene.getLight<SimpleDirectionalLight>("Key");
	
	m_shadowMapRenderingPass.render(scene, keyLight->perspective);

	accessToGeometryPass().render(scene);
	accessToLightingPass().render(scene);
}

void DefferedSampleRenderPipeline::render()
{
}

void DefferedSampleRenderPipeline::drawImGuiComponents()
{
	if (ImGui::TreeNode(name().c_str()))
	{
		if (ImGui::TreeNode("Lighting Pass"))
		{
			accessToLightingPass().drawImGuiComponents();

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
