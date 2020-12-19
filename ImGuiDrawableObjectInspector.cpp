#include "ImGuiDrawableObjectInspector.hpp"
#include "ImGuiTransformInspector.hpp"
#include "ConstantWithImGuiComponents.hpp"
#include "ImGuiDrawableObjectMaterialInspector.hpp"




namespace ImGui
{
	void DrawableObjectInspector::draw(cg::DrawableObject& object)
	{
		auto components = object.geometry.getShapeP<IImGuiComponentsHolder>();
		if (components)
		{
			if (ImGui::TreeNode("Shape"))
			{
				components->drawImGuiComponents();

				ImGui::TreePop();
			}
		}
		if (ImGui::TreeNode("Transform"))
		{
			ImGui::TransformInspector(*object.transform);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Material"))
		{
			m_materialInspector.draw(object);

			ImGui::TreePop();
		}
	}
	void DrawableObjectInspector::reset()
	{
		m_materialInspector.reset();
	}
}