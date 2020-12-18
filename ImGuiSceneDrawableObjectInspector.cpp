#include "ImGuiDrawableObjectInspector.hpp"
#include "ImGuiSceneDrawableObjectInspector.hpp"
#include "ImGuiCameraJumpButton.hpp"

#include <GraphicsLib.hpp>





namespace ImGui
{
	void SceneDrawableObjectInspector::draw(cg::Scene& scene, CameraController& cameraController)
	{
		const auto groupNameList = scene.makeGroupNameList();
		
		int selectedGroupIndex = -1;
		m_groupSearchBar.draw("Group", "Name...", groupNameList, 8, selectedGroupIndex);

		if (selectedGroupIndex == -1)
		{
			m_objectSearchBar.reset();
			return; 
		}
		
		std::vector<std::string> objectNameList;
		std::vector<std::weak_ptr<cg::DrawableObject>> objectList;

		const auto groupName = groupNameList.at(selectedGroupIndex);
		const auto objects = scene.getGroup(groupName)->makeObjectDict();
		for (const auto pair : objects)
		{
			auto object = pair.second;
			objectList.emplace_back(object);
			objectNameList.emplace_back(object->getName());
		}

		int selectedObjectIndex = -1;
		m_objectSearchBar.draw("Object", "Name...", objectNameList, 8, selectedObjectIndex);

		if (selectedObjectIndex == -1) 
		{
			m_drawableObjectInspector.reset();
			return; 
		}

		if (auto object = objectList[selectedObjectIndex].lock())
		{
			m_drawableObjectInspector.draw(*object);
			ImGui::CameraJumpButton(cameraController, scene.camera, *object->transform);
		}
	}
	void SceneDrawableObjectInspector::reset()
	{
		m_groupSearchBar.reset();
		m_objectSearchBar.reset();
		m_drawableObjectInspector.reset();
	}
}
