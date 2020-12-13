#include "DrawableObjectCreator.hpp"

#include "VertexShaderInput.hpp"
#include "ShaderConstant.hpp"

#include "SimpleDirectionalLight.hpp"
#include "SimplePointLight.hpp"

#include "ImGuiGraphicsLibDefaultInspector.hpp"

#include "ForwardSampleRenderPipeline.hpp"
#include "ForwardZPrePassSampleRenderPipeline.hpp"

#include "SimplePBRMaterialConstant.hpp"
#include "DefferedSampleRenderPipeline.hpp"





int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif



	// Initialize GraphicsLib
	const auto& screenUpdater = cg::System::initialize(1500, 800, false, 1, "Sample");
	ImGui_ImplGraphicsLib_Init();



	const auto windowInfo = cg::System::getWindowInfo();
	const auto windowSize = windowInfo.getSize();
	const auto aspect = windowInfo.calcAspect();



	// Setup render target
	auto mainRenderTarget = cg::MainRenderTarget::shared.get();



	// Setup render pipeline
	std::shared_ptr<RenderPipelineWithImGuiComponents> renderPipeline;
	std::string renderingGroupName;
	try
	{
		renderPipeline = std::make_shared<DefferedSampleRenderPipeline>(mainRenderTarget);
		renderingGroupName = DefferedSampleRenderPipeline::targetRenderingGroupName;

		//renderPipeline = std::make_shared<ForwardSampleRenderPipeline>(mainRenderTarget);
		//renderingGroupName = ForwardSampleRenderPipeline::targetRenderingGroupName;

		//renderPipeline = std::make_shared<ForwardZPrePassSampleRenderPipeline>(mainRenderTarget);
		//renderingGroupName = ForwardZPrePassSampleRenderPipeline::targetRenderingGroupName;
	}
	catch (cpp::com_runtime_error e)
	{
		Log(e.message());
	}

	// Prepare material
	auto materialConstant = std::make_shared<SimplePBRMaterialConstant>();
	materialConstant->changeIOR(1.35f);
	materialConstant->changeMetalness(0.8f);
	materialConstant->changeRoughness(0.6f);

	auto materialConstantDict = cg::Material::ConstantDict();
	materialConstantDict.emplace(cg::ShaderStage::ps, materialConstant);
	auto material = cg::Material(materialConstantDict);

	auto planeMaterial = material.clone();
	auto sphereMaterial = material.clone();
	auto boxMaterial = material.clone();

	std::dynamic_pointer_cast<SimplePBRMaterialConstant>(planeMaterial.getConstantP(cg::ShaderStage::ps))->changeColor(1.0f, 1.0f, 1.0f);
	std::dynamic_pointer_cast<SimplePBRMaterialConstant>(sphereMaterial.getConstantP(cg::ShaderStage::ps))->changeColor(0.9f, 0.05f, 0.5f);
	std::dynamic_pointer_cast<SimplePBRMaterialConstant>(boxMaterial.getConstantP(cg::ShaderStage::ps))->changeColor(0.3f, 0.3f, 0.8f);



	// Prepare objects
	auto plane = DrawableObjectCreator::createBox<vsinput::Position3Normal3>("Plane", 5.0f, 0.1f, 5.0f, planeMaterial);
	auto sphere = DrawableObjectCreator::createSphere<vsinput::Position3Normal3>("Sphere", 0.75f * 0.5f, 20, 20, sphereMaterial);
	auto box = DrawableObjectCreator::createBox<vsinput::Position3Normal3>("Box", 0.75f, 0.75f, 0.75f, boxMaterial);

	plane->getTransformRef().changePosition(0.0f, -0.05f, 0.0f);
	sphere->getTransformRef().changePosition(0.75f, 0.75f * 0.5f, 0.0f);
	box->getTransformRef().changePosition(-0.75f, 0.75f * 0.5f, 0.0f);

	plane->moveTo(renderingGroupName);
	sphere->moveTo(renderingGroupName);
	box->moveTo(renderingGroupName);



	// Setup Scene
	auto scene = cg::Scene("World");

	auto mainGroup = std::make_shared<cg::DrawableObjectGroup>("MainGroup");
	mainGroup->add(sphere);
	mainGroup->add(box);
	mainGroup->add(plane);

	scene.addObjectGroup(mainGroup);



	// Setup Camera
	auto& cameraTransform = scene.camera.getTransformRef();
	cameraTransform.changePosition(0.0f, 1.0f, -2.0f);
	cameraTransform.changeCoordinateSystem(cg::CoordinateSystem::rectangular);
	cameraTransform.changeRotationMethod(cg::RotationMethod::lookAt);
	cameraTransform.changeFocusPositionLocal(0.0f, 0.0f, 0.0f);
	cameraTransform.changeUpLocal(0.0f, 1.0f, 0.0f);

	auto& cameraProjection = scene.camera.projection;
	cameraProjection.changeFov(cpp::Math::degreeToRadian(60.0f));
	cameraProjection.changeAspect(aspect);
	cameraProjection.changeNearZ(0.001f);
	cameraProjection.changeFarZ(10.0f);



	// Setup key light
	auto keyLight = std::make_shared<SimpleDirectionalLight>("Key");
	auto& keyLightCameraTransform = keyLight->perspective.getTransformRef();
	auto& keyLightCameraProjection = keyLight->perspective.projection;

	const auto keyLightDirection = cpp::Vector3D<float>(0.0f, -0.5f, 1.0f);
	auto keyLightCameraPosition = -3.0f * keyLightDirection;

	keyLight->accessToColorParam().changeColor(0.9f, 0.9f, 0.9f);

	keyLightCameraProjection.changeType(cg::ProjectionType::orthographic3D);
	keyLightCameraProjection.changeViewSize(2.5f * aspect, 2.5f);
	keyLightCameraProjection.changeNearZ(0.001f);
	keyLightCameraProjection.changeFarZ(10.0f);

	keyLightCameraTransform.changeRotationMethod(cg::RotationMethod::directionBase);
	keyLightCameraTransform.changeDirectionWorld(keyLightDirection);
	keyLightCameraTransform.changeUpWorld(0.0f, 1.0f, 0.0f);
	keyLightCameraTransform.changePosition(keyLightCameraPosition);

	scene.addLight(keyLight);



	// Setup back light
	auto pointLight = std::make_shared<SimplePointLight>("Back");
	auto& pointLightTransform = pointLight->getTransformRef();

	pointLight->accessToColorParam().changeColor(1.0f, 1.0f, 1.0f);

	auto pointLightAttenuParam = pointLight->accessToAttenuationParam();
	pointLightAttenuParam.changeConstant(1.0f);
	pointLightAttenuParam.changeLinear(0.6f);
	pointLightAttenuParam.changeQuadratic(0.3f);

	pointLightTransform.changePosition(cpp::Vector3D<float>(0.0f, 0.1f, 3.0f));

	scene.addLight(pointLight);



	// Game Loop
	CameraController cameraController;
	ImGui::GraphicsLibDefaultInspector inspector;
	while (screenUpdater->update())
	{
		ImGui_ImplGraphicsLib_NewFrame();

		renderPipeline->render(scene);

		inspector.draw(scene, cameraController, renderPipeline);

		cameraController.control(scene.camera);

		ImGui_ImplGraphicsLib_Render();
	}



	// Finalize GraphicsLib
	ImGui_ImplGraphicsLib_Shutdown();
	cg::System::finalize();
}
