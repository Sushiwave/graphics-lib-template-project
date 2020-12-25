#include "ImGuiDrawableObjectInspector.hpp"
#include "ImGuiTransformInspector.hpp"
#include "ConstantWithImGuiComponents.hpp"
#include "ImGuiDrawableObjectMaterialInspector.hpp"




namespace ImGui
{
	void DrawableObjectInspector::draw(cg::DrawableObject& object)
	{
		if (ImGui::TreeNode("Transform"))
		{
			ImGui::TransformInspector(*object.transform);
			ImGui::TreePop();
		}
		if (object.geometry.empty() == false)
		{
			if (ImGui::TreeNode("Geometry"))
			{
				m_geometryInspector.draw(object.geometry);
				ImGui::TreePop();
			}
		}
	}
	void DrawableObjectInspector::reset()
	{
		m_geometryInspector.reset();
	}
}