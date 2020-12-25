#pragma once
#include "ImGuiGeometryInspector.hpp"





namespace ImGui
{
	class DrawableObjectInspector
	{
	private:
		GeometryInspector m_geometryInspector;
	public:
		void draw(cg::DrawableObject& object);

		void reset();
	};
}
