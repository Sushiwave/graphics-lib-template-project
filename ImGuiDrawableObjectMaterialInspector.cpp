#include "ImGuiDrawableObjectMaterialInspector.hpp"
#include "IImGuiComponentsHolder.hpp"

#include <GraphicsLib.hpp>





namespace ImGui
{
	void DrawableObjectMaterialInspector::draw(cg::Material& material)
	{
		using ShaderStageNameWithIndex = std::pair<const char*, int>;
		std::vector<std::shared_ptr<IImGuiComponentsHolder>> imGuiComponentsList;
		std::vector<ShaderStageNameWithIndex> shaderStageNameWithIndexList;
		auto count = static_cast<int>(cg::ShaderStageAll.size());
		for (int i = 0; i < count; ++i)
		{
			auto stage = cg::ShaderStageAll.at(i);
			auto& stageName = cg::ShaderStageNameAll.at(i);
			auto components = std::dynamic_pointer_cast<IImGuiComponentsHolder>(material.getConstantP(stage));
			if (components == nullptr) { continue; }

			imGuiComponentsList.emplace_back(components);
			shaderStageNameWithIndexList.emplace_back(std::make_pair(stageName.c_str(), i));
		}
		
		std::vector<std::string> shaderStageNameList;
		for (auto pair : shaderStageNameWithIndexList)
		{
			shaderStageNameList.emplace_back(pair.first);
		}

		int previewShaderStageIndex = -1;
		ImGui::Combo("Stage", shaderStageNameList, m_previewShaderStage, previewShaderStageIndex);

		if (previewShaderStageIndex == -1) { return; }

		imGuiComponentsList.at(previewShaderStageIndex)->drawImGuiComponents();
	}
	void DrawableObjectMaterialInspector::reset()
	{
		m_previewShaderStage = "";
	}
}
