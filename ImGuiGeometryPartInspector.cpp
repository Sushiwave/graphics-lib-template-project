#include "ImGuiGeometryPartInspector.hpp"





namespace ImGui
{
	void GeometryPartInspector::draw(cg::Geometry::Part& part)
	{
		if (TreeNode("Material"))
		{
			m_materialInspector.draw(part.material);

			TreePop();
		}
	}

	void GeometryPartInspector::reset()
	{
		m_materialInspector.reset();
	}
}
