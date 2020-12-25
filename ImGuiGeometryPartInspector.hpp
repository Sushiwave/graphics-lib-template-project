#pragma once
#include "ImGuiDrawableObjectMaterialInspector.hpp"





namespace ImGui
{
	class GeometryPartInspector
	{
	private:
		DrawableObjectMaterialInspector m_materialInspector;
	public:
		void draw(cg::Geometry::Part& part);
		void reset();
	};
}