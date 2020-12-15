#include <GraphicsLib.hpp>
#include "ImGuiCameraControllerInspector.hpp"





namespace ImGui
{
	void CameraControllerInspector(CameraController& cameraController, cg::Camera& camera)
	{
		constexpr float minMouseSmoothness = cpp::Math::degreeToRadian(0.01f);
		constexpr float maxMouseSmoothness = cpp::Math::degreeToRadian(1.0f);
		constexpr float mouseSmoothnessRange = maxMouseSmoothness-minMouseSmoothness;
		const auto sliderAngleDelta = [&]() 
		{
			float angleDelta0to1 = (cameraController.angleDeltaRadian-minMouseSmoothness)/mouseSmoothnessRange;
			ImGui::SliderFloat("Angle Delta", &angleDelta0to1, 0.0, 1.0);
			cameraController.angleDeltaRadian = angleDelta0to1*mouseSmoothnessRange+minMouseSmoothness;
		};
		const auto inputMovementDelta = [&]()
		{
			ImGui::InputFloat("Movement Delta", &cameraController.movementDelta);
		};



		constexpr const char* itemLookAt   = "LookAt";
		constexpr const char* itemFreeMove = "FreeMove+WASD";
		static const std::vector<std::string> itemList = { itemLookAt, itemFreeMove };



		std::string previewItem = "";
		int previewItemIndex = -1;
		switch (cameraController.getControllingMethod())
		{
		case ObjectControllingMethod::lookAt:
			previewItem = itemLookAt;
			break;
		case ObjectControllingMethod::freeLookWASD:
			previewItem = itemFreeMove;
			break;
		}
		ImGui::Combo("Control Method", itemList, previewItem, previewItemIndex);





		auto& transform = *camera.transform;
		if (previewItem == itemLookAt)
		{
			cameraController.changeControllingMethod(ObjectControllingMethod::lookAt);

			auto position = transform.calcPositionWorld();

			transform.tweakCenterWorld([&](cpp::Vector3D<float>& c)
			{
				ImGui::InputVector3D("Focus Position", c);
			});
			transform.tweakUpWorld([&](cpp::Vector3D<float>& u)
			{
				ImGui::InputVector3D("Up", u);
			});
			position = transform.calcPositionWorld();



			sliderAngleDelta();
			inputMovementDelta();
		}
		else if (previewItem == itemFreeMove)
		{
			cameraController.changeControllingMethod(ObjectControllingMethod::freeLookWASD);



			inputMovementDelta();
		}
	}
}
