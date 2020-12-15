#include "ImGUiSceneLightInspector.hpp"
#include "IImGuiComponentsHolder.hpp"
#include "ImGuiProjectionInspector.hpp"
#include "ImGuiTransformInspector.hpp"





namespace ImGui 
{
	void SceneLightInspector::draw(const cg::Scene& scene)
	{
		auto lightDict = scene.getLightDict();
		std::vector<std::shared_ptr<cg::Light>> lightList;
		std::vector<std::string> lightNameList;
		for (auto pair : lightDict)
		{
			auto light = pair.second;
			lightList.emplace_back(light);
			lightNameList.emplace_back(light->getName());
		}

		int selectedLightIndex;
		m_searchBar.draw("Light", "Name...", lightNameList, 8, selectedLightIndex);
		
		if (selectedLightIndex == -1) { return; }
		
		if (auto selectedLight = lightList[selectedLightIndex])
		{
			if(const auto directionalLight = std::dynamic_pointer_cast<cg::DirectionalLight>(selectedLight))
			{
				if (ImGui::TreeNode("Perspective"))
				{
					if (ImGui::TreeNode("Transform"))
					{
						ImGui::TransformInspector(*directionalLight->perspective.transform);

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Projection"))
					{
						ImGui::ProjectionInspector(directionalLight->perspective.projection);
						
						ImGui::TreePop();
					}

					ImGui::TreePop();
				}
			}
			else if (const auto spotLight = std::dynamic_pointer_cast<cg::SpotLight>(selectedLight))
			{
				if (ImGui::TreeNode("Transform"))
				{
					ImGui::TransformInspector(*spotLight->transform);

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Projection"))
				{
					ImGui::ProjectionInspector(spotLight->projection);
				
					ImGui::TreePop();
				}
			}
			else if (const auto pointLight = std::dynamic_pointer_cast<cg::PointLight>(selectedLight))
			{
				if (ImGui::TreeNode("Transform"))
				{
					ImGui::TransformInspector(*pointLight->transform);

					ImGui::TreePop();
				}
			}

			selectedLight->getConstantP<IImGuiComponentsHolder>()->drawImGuiComponents();
		}
		else
		{
			ImGui::Text("No data");
		}
	}
	void SceneLightInspector::reset()
	{
		m_searchBar.reset();
	}
}