#include "ImGuiGeometryInspector.hpp"
#include "IImGuiComponentsHolder.hpp"





namespace ImGui
{
	void GeometryInspector::draw(cg::Geometry& geometry)
	{
		auto components = std::dynamic_pointer_cast<IImGuiComponentsHolder>(geometry.shape);
		if (components)
		{
			if (ImGui::TreeNode("Shape"))
			{
				components->drawImGuiComponents();

				ImGui::TreePop();
			}
		}

		if (ImGui::TreeNode("Parts"))
		{
			const auto partNameList = geometry.parts.makePartNameList();

			int selectedPartIndex = -1;
			m_searchBar.draw("Part", "Name...", partNameList, 8, selectedPartIndex);

			if (selectedPartIndex == -1) { return; }

			const auto partName = partNameList[selectedPartIndex];
			auto part = geometry.parts.get(partName);

			m_partInspector.draw(part);
		}
	}

	void GeometryInspector::reset()
	{
		m_searchBar.reset();
		m_partInspector.reset();
	}
}