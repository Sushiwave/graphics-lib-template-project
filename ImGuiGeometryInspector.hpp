#pragma once
#include "ImGuiGeometryPartInspector.hpp"





namespace ImGui
{
	class GeometryInspector
	{
	private:
		GeometryPartInspector m_partInspector;
		SearchBar m_searchBar;
	public:
		void draw(cg::Geometry& geometry);

		void reset();
	};
}