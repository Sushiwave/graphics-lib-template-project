#pragma once
#include <GraphicsLib.hpp>





namespace ImGui
{
	class DrawableObjectMaterialInspector
	{
	private:
		std::string m_previewShaderStage;
	public:
		void draw(cg::Material& material);

		void reset();
	};
}
