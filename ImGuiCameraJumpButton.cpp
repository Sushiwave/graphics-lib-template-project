#include "ImGuiCameraJumpButton.hpp"





namespace ImGui
{
	bool CameraJumpButton(CameraController& cameraController, cg::Camera& camera, const cg::Transform& target)
	{
		if (ImGui::Button("Jump"))
		{
			cameraController.enable();
			cameraController.changeControllingMethod(ObjectControllingMethod::lookAt);
			cameraController.control(camera); // << Hmmmmmmmmm

			const auto positionWorld = target.calcPositionWorld();
			const auto scale = target.calcScaleLocal();
			const auto distanceFromObject = max(max(scale.x, scale.y), scale.z)*1.5f;

			auto& cameraTransform = *camera.transform;
			cameraTransform.changeUpWorld(0.0, 1.0, 0.0);
			cameraTransform.changeCenterWorld(positionWorld);
			cameraTransform.changeFocusPositionWorld(positionWorld);
			cameraTransform.changeRadiusPhiTheta(distanceFromObject, 0.0f, -static_cast<float>(cpp::Math::HALF_PI));

			return true;
		}
		else
		{
			return false;
		}
	}
}
