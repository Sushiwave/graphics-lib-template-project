#include "ImGuiCameraInspector.hpp"
#include "ImGuiTransformInspector.hpp"
#include "ImGuiCameraControllerInspector.hpp"
#include "ImGuiProjectionInspector.hpp"





namespace ImGui
{
	void CameraInspector::draw(cg::Camera& camera, CameraController& cameraController)
	{
		bool isCameraControlEnabled = cameraController.isEnabled();

		if (ImGui::TreeNode("Transform"))
		{
			auto& style = ImGui::GetStyle();
			if (isCameraControlEnabled)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				style.Colors->w = 0.5;
			}

			ImGui::TransformInspector(*camera.transform);

			if (isCameraControlEnabled)
			{
				ImGui::PopItemFlag();
				style.Colors->w = 1.0;
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Projection"))
		{
			ImGui::ProjectionInspector(camera.projection);

			ImGui::TreePop();
		}

		ImGui::Checkbox("Mouse Control", &isCameraControlEnabled);

		if (isCameraControlEnabled)
		{
			cameraController.enable();
			ImGui::CameraControllerInspector(cameraController, camera);
		}
		else
		{
			cameraController.disable();
		}
	}

	void CameraInspector::reset()
	{
	}
}